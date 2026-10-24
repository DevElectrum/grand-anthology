#!/usr/bin/env python3
"""Grand Anthology: lossless external Pokémon PC vault, first persistent storage layer.

A vanilla 128 KiB GBA battery save cannot fit thousands of BoxPokemon records.
The playable ROM retains its original shared 14x30 boxes. This *offline* tool
moves encrypted, byte-exact 80-byte BoxPokemon records between the cartridge PC
and an expandable host-side vault (up to 65,535 records).

This does not pretend the in-game PC can browse archived Pokémon yet. That
needs an emulator/host integration layer. Close mGBA before processing a save.

Examples (run from repo root):
  python3 tools/grand_vault.py info --save mygame.sav
  python3 tools/grand_vault.py deposit --save mygame.sav --box 1 --slot 1 --out backups/step01
  python3 tools/grand_vault.py sweep --save mygame.sav --keep 30 --out backups/bulk01
  python3 tools/grand_vault.py withdraw --save backups/step01/cartridge.sav \
      --vault backups/step01/collection.vault.json --id MON_ID --out backups/step02
  python3 tools/grand_vault.py info --save backups/step02/cartridge.sav \
      --vault backups/step02/collection.vault.json

NEVER edits its input save or vault; outputs are bundled into a *new* directory.
Three files appear together by atomic directory rename: cartridge.sav,
collection.vault.json and manifest.json (SHA-256 of both). Avoid mixing the
save and vault from different output generations.

On every transfer:
 - source save geometry, original 14-sector checksum, old-format pinning,
   no-op byte-for-byte writeback and intact active slot are verified;
 - v12 save version, exact trainer fingerprint and intact Pokémon secure
   substruct checksum are verified;
 - no file is overwritten, a corrupt archive is rejected;
 - transfer is a MOVE, not a copy: the source location is zeroed;
 - the output save is reparsed and revalidated before publication.

Not an in-game PC expansion, not hot-swap while the emulator runs.
"""
import argparse
import base64
import binascii
import hashlib
import json
import os
from pathlib import Path
import secrets
import shutil
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "test" / "overworld"))
import SavePatch as sp

FORMAT = "pga-pc-vault-v1"
MON_BYTES = 80
BOXES = 14
SLOTS = 30
MAX_ARCHIVED = 65535
PK_BOX_OFFSET = 4
# src/pokemon.c::sSubstructOffsets[SUBSTRUCT_TYPE_0]; the decrypted 48-byte
# secure area permutes the four 12-byte substructs based on PID % 24.
# This determines where the species/form, item and exp live.
SUBSTRUCT_ZERO_OFFSETS = (
    0,0,0,0,0,0,1,1,2,3,2,3,1,1,2,3,2,3,1,1,2,3,2,3,
)
assert len(SUBSTRUCT_ZERO_OFFSETS) == 24
assert PK_BOX_OFFSET + BOXES * SLOTS * MON_BYTES <= sp.PK_SIZE

class VaultError(Exception):
    pass

def digest(raw):
    return hashlib.sha256(raw).hexdigest()

def identity(sv):
    # Player name[0:8], gender byte 8, two-byte alignment filler at 9,
    # trainerId[10:14]. Excludes all volatile money/time/region fields.
    return digest(bytes(sv.sb2[:9]) + bytes(sv.sb2[10:14]))

def slot_offset(box, slot):
    if not 1 <= box <= BOXES or not 1 <= slot <= SLOTS:
        raise VaultError(f"box must be 1..{BOXES}, slot must be 1..{SLOTS}")
    return PK_BOX_OFFSET + ((box - 1) * SLOTS + (slot - 1)) * MON_BYTES

def is_occupied(data):
    if len(data) != MON_BYTES:
        raise VaultError("BoxPokemon record must be exactly 80 bytes")
    return bool(data[19] & 2)

def validate_mon(data):
    if not is_occupied(data):
        raise VaultError("empty BoxPokemon record cannot enter a vault")
    if data[19] & 1:
        raise VaultError("refusing corrupt/bad-egg BoxPokemon")
    # Secure substruct is XOR-encrypted in 12 little-endian u32 words with
    # (personality ^ original trainer ID). Sum decrypted 24 u16, independent
    # of personality-selected substruct permutation. This is the SAME stored
    # checksum GetBoxMonData uses, without changing any Pokémon byte.
    key = int.from_bytes(data[:4], "little") ^ int.from_bytes(data[4:8], "little")
    secure = bytearray(data[32:80])
    for i in range(0, 48, 4):
        x = int.from_bytes(secure[i:i + 4], "little") ^ key
        secure[i:i + 4] = x.to_bytes(4, "little")
    calculated = sum(int.from_bytes(secure[i:i + 2], "little")
                     for i in range(0, 48, 2)) & 0xFFFF
    stored = int.from_bytes(data[28:30], "little")
    if calculated != stored:
        raise VaultError(f"BoxPokemon secure checksum mismatch: {calculated:04x} != {stored:04x}")

def mon_fields(data):
    """Display-only metadata, read from exact encrypted BoxPokemon bytes.

    Species is an *exact species/form ID*, NOT national Dex number.
    The nickname is best-effort for non-English game glyphs; the record
    bytes themselves remain completely unmodified.
    """
    validate_mon(data)
    pid = int.from_bytes(data[:4], "little")
    otid = int.from_bytes(data[4:8], "little")
    key = pid ^ otid
    off = SUBSTRUCT_ZERO_OFFSETS[pid % 24] * 12
    encrypted = data[32 + off:32 + off + 12]
    decrypted = bytearray(12)
    for i in range(0,12,4):
        x = int.from_bytes(encrypted[i:i+4], "little") ^ key
        decrypted[i:i+4] = x.to_bytes(4,"little")
    species = int.from_bytes(decrypted[:2], "little") & 0x7FF
    item = int.from_bytes(decrypted[2:4], "little") & 0x3FF
    chars=[]
    for c in data[8:18]:
        if c == 0xFF:
            break
        chars.append(sp.CHARMAP.get(c, "?"))
    nickname = "".join(chars).strip()
    return {"species": species, "item": item, "nickname": nickname,
            "personality": pid, "otid": otid}

def load_save(path):
    if not path.is_file():
        raise VaultError(f"missing save: {path}")
    sp.require_pins()
    try:
        sv = sp.self_test(str(path))
        sp.require_intact_slot(sv)
    except SystemExit as exc:
        raise VaultError(str(exc)) from exc
    if sv.slots[sv.active]["status"] != "OK":
        raise VaultError("selected save slot is not fully healthy")
    version = sv.sb2[sp.V["SB2.saveVersion"]]
    if version != sp.SAVE_FORMAT_VERSION:
        raise VaultError(f"save version {version} != {sp.SAVE_FORMAT_VERSION}; migrate first")
    if sv.u16(sv.sb2, sp.V["SB2.regionChecksum"]) != sv.region_checksum():
        raise VaultError("region bank checksum mismatch; refusing Pokémon transfers")
    return sv

def fresh_vault(trainer):
    return {"format": FORMAT, "trainer": trainer, "revision": 0, "records": []}

def load_vault(path, trainer):
    if path is None:
        return fresh_vault(trainer)
    with path.open("rb") as stream:
        # 65,535 records * ~220 bytes of JSON each, plus metadata.
        raw = stream.read(24_000_001)
    if len(raw) > 24_000_000:
        raise VaultError("vault file too large / malformed")
    try:
        obj = json.loads(raw)
    except (ValueError, UnicodeError) as exc:
        raise VaultError("invalid vault JSON") from exc
    if not isinstance(obj, dict) or set(obj) != {"format", "trainer", "revision", "records"}:
        raise VaultError("unexpected vault schema")
    if obj["format"] != FORMAT or obj["trainer"] != trainer:
        raise VaultError("vault format or trainer identity mismatch")
    if not isinstance(obj["revision"], int) or not 0 <= obj["revision"] <= 2**31:
        raise VaultError("invalid vault revision")
    records = obj["records"]
    if not isinstance(records, list) or len(records) > MAX_ARCHIVED:
        raise VaultError("invalid vault size")
    ids = set()
    for record in records:
        if not isinstance(record, dict) or set(record) != {"id", "sha256", "pokemon"}:
            raise VaultError("malformed vault Pokémon entry")
        rid = record["id"]
        if not isinstance(rid, str) or not 1 <= len(rid) <= 64 or rid in ids:
            raise VaultError("duplicate/malformed vault entry ID")
        ids.add(rid)
        data = decode_record(record)
        validate_mon(data)
    return obj

def decode_record(record):
    try:
        data = base64.b64decode(record["pokemon"], validate=True)
    except (ValueError, TypeError, binascii.Error) as exc:
        raise VaultError("invalid vault Pokémon encoding") from exc
    if len(data) != MON_BYTES or digest(data) != record["sha256"]:
        raise VaultError("vault Pokémon record SHA-256/size mismatch")
    return data

def read_box(sv, box, slot):
    offset = slot_offset(box, slot)
    return bytes(sv.pk[offset:offset + MON_BYTES])

def write_box(sv, box, slot, data):
    if len(data) != MON_BYTES:
        raise VaultError("refusing wrong-length Pokémon write")
    start = slot_offset(box, slot)
    sv.pk[start:start + MON_BYTES] = data

def find_empty(sv):
    for box in range(1, BOXES + 1):
        for slot in range(1, SLOTS + 1):
            if not is_occupied(read_box(sv, box, slot)):
                return box, slot
    raise VaultError("all 420 in-cartridge PC slots are full")

def check_not_duplicated(sv, mon):
    # Guard common stale-cartridge/previous-vault mistakes. Raw identity is
    # exact byte-wise, not fuzzy nickname/species: unrelated Pokémon survive.
    for box in range(1, BOXES + 1):
        for slot in range(1, SLOTS + 1):
            data = read_box(sv, box, slot)
            if is_occupied(data) and data == mon:
                raise VaultError(f"identical Pokémon already in cartridge PC box {box} slot {slot}")
    # Party Pokémon embed exactly the same 80-byte BoxPokemon header.
    # A stale cartridge containing the same archived Pokémon *in party*
    # must not silently duplicate it on restore.
    count = sv.sb1[sp.V["SB1.playerPartyCount"]]
    if count > 6:
        raise VaultError("invalid saved party count")
    for slot in range(count):
        offset = sp.V["SB1.playerParty"] + slot * 100
        data = bytes(sv.sb1[offset:offset + MON_BYTES])
        if is_occupied(data) and data == mon:
            raise VaultError(f"identical Pokémon already in party slot {slot+1}")

def apply_transaction(sv, vault, mode, box=None, slot=None, mon_id=None, keep=30):
    vault = json.loads(json.dumps(vault))  # never mutate loaded input data
    if mode == "deposit":
        if box is None or slot is None:
            raise VaultError("deposit requires --box and --slot")
        if len(vault["records"]) >= MAX_ARCHIVED:
            raise VaultError("vault reached 65,535 Pokémon limit")
        mon = read_box(sv, box, slot)
        validate_mon(mon)
        # An identical blob already in the archive is most likely an old-save
        # replay. Reject rather than silently duplicating a Pokémon.
        if any(record["sha256"] == digest(mon) and decode_record(record) == mon
               for record in vault["records"]):
            raise VaultError("this exact Pokémon is already archived; refusing stale-save replay")
        rid = secrets.token_hex(12)
        vault["records"].append({"id": rid, "sha256": digest(mon),
                                 "pokemon": base64.b64encode(mon).decode("ascii")})
        write_box(sv, box, slot, bytes(MON_BYTES))
        description = f"deposited box {box} slot {slot} to vault ID {rid}"
        assert not is_occupied(read_box(sv, box, slot))
    elif mode == "sweep":
        # Bulk deposit is a single all-or-nothing transaction. Leave the first
        # KEEP valid Pokémon in natural box order on the actual cartridge and
        # archive all later occupied slots. Never act on an invalid/corrupt mon.
        if not 0 <= keep <= BOXES * SLOTS:
            raise VaultError(f"--keep must be 0..{BOXES*SLOTS}")
        occupied = []
        for bx in range(1, BOXES + 1):
            for sl in range(1, SLOTS + 1):
                candidate = read_box(sv, bx, sl)
                if is_occupied(candidate):
                    validate_mon(candidate)
                    occupied.append((bx, sl, candidate))
        moving = occupied[keep:]
        if len(vault["records"]) + len(moving) > MAX_ARCHIVED:
            raise VaultError("bulk deposit exceeds vault capacity; no changes made")
        known = {entry["sha256"] for entry in vault["records"]}
        for _, _, candidate in moving:
            sha = digest(candidate)
            if sha in known:
                raise VaultError("archive already contains an identical Pokémon; refusing stale-save replay")
            known.add(sha)
        for bx, sl, candidate in moving:
            vault["records"].append({
                "id": secrets.token_hex(12),
                "sha256": digest(candidate),
                "pokemon": base64.b64encode(candidate).decode("ascii"),
            })
            write_box(sv, bx, sl, bytes(MON_BYTES))
        description = f"bulk archived {len(moving)} Pokémon, kept {len(occupied)-len(moving)} in cartridge PC"
    elif mode == "withdraw":
        if not mon_id:
            raise VaultError("withdraw requires --id from info")
        idx = next((i for i, r in enumerate(vault["records"]) if r["id"] == mon_id), None)
        if idx is None:
            raise VaultError("vault Pokémon ID not found")
        mon = decode_record(vault["records"][idx])
        validate_mon(mon)
        check_not_duplicated(sv, mon)
        if box is None and slot is None:
            box, slot = find_empty(sv)
        elif box is None or slot is None:
            raise VaultError("specify both --box and --slot, or neither")
        if is_occupied(read_box(sv, box, slot)):
            raise VaultError("destination PC slot occupied; refusing overwrite")
        write_box(sv, box, slot, mon)
        vault["records"].pop(idx)
        description = f"withdrew vault ID {mon_id} to box {box} slot {slot}"
        assert read_box(sv, box, slot) == mon
    else:
        raise VaultError("unknown operation")
    vault["revision"] += 1
    return vault, description

def publish(out, sv, vault, action):
    """Only the whole new directory is published; source files never change."""
    if out.exists():
        raise VaultError("output directory already exists; refusing overwrite")
    if not out.parent.is_dir():
        raise VaultError("output parent directory does not exist")
    tmp = Path(tempfile.mkdtemp(prefix=".grand-vault-", dir=out.parent))
    try:
        cartridge = tmp / "cartridge.sav"
        vault_file = tmp / "collection.vault.json"
        manifest = tmp / "manifest.json"
        sv.writeback()
        cartridge.write_bytes(bytes(sv.flash) + sv.trailer)
        vault_file.write_text(json.dumps(vault, indent=2, sort_keys=True) + "\n")
        # Read bytes back through the original game's save scanner, not our
        # assumptions about physical sector positions. RTC trailer untouched.
        check = load_save(cartridge)
        if bytes(check.pk) != bytes(sv.pk):
            raise VaultError("written storage block did not round-trip")
        if check.trailer != sv.trailer:
            raise VaultError("RTC trailer changed unexpectedly")
        load_vault(vault_file, identity(check))
        files = {f.name: digest(f.read_bytes()) for f in (cartridge, vault_file)}
        manifest.write_text(json.dumps({"format": FORMAT, "action": action,
                                         "revision": vault["revision"],
                                         "files": files}, indent=2, sort_keys=True) + "\n")
        # os.rename is atomic within the output parent filesystem.
        if out.exists():
            raise VaultError("output path appeared during transaction")
        tmp.rename(out)
        return files
    finally:
        if tmp.exists():
            shutil.rmtree(tmp)

def run(args):
    save_path = Path(args.save).expanduser()
    sv = load_save(save_path)
    vault_path = Path(args.vault).expanduser() if args.vault else None
    vault = load_vault(vault_path, identity(sv))
    if args.action == "info":
        if args.limit < 0 or args.limit > MAX_ARCHIVED:
            raise VaultError(f"--limit must be 0..{MAX_ARCHIVED}")
        print(f"trainer={sv.name()} vault_count={len(vault['records'])} vault_revision={vault['revision']}")
        print(f"cartridge_pc_capacity={BOXES*SLOTS} archive_capacity={MAX_ARCHIVED}")
        shown = 0
        filtered = 0
        for r in vault["records"]:
            meta = mon_fields(decode_record(r))
            if args.species is not None and meta["species"] != args.species:
                continue
            if args.nickname and args.nickname.casefold() not in meta["nickname"].casefold():
                continue
            filtered += 1
            if shown < args.limit:
                print(f"id={r['id']} species={meta['species']} nick={meta['nickname']!r} item={meta['item']} "
                      f"pid={meta['personality']:08x}")
                shown += 1
        print(f"matching={filtered} shown={shown}; use --limit N, --species ID or --nickname TEXT to search")
        return
    updated, action = apply_transaction(sv, vault, args.action, args.box, args.slot, args.id, getattr(args, "keep", 30))
    out = Path(args.out).expanduser()
    publish(out, sv, updated, action)
    print(action)
    print(f"new save: {out/'cartridge.sav'}")
    print(f"new vault: {out/'collection.vault.json'}")
    print("Use these two TOGETHER; originals were never changed. Emulator must be closed.")

def main():
    p = argparse.ArgumentParser(description="Offline, lossless Pokémon PC archive, 65,535 additional slots")
    sub = p.add_subparsers(dest="action", required=True)
    for cmd in ("info", "deposit", "sweep", "withdraw"):
        q = sub.add_parser(cmd)
        q.add_argument("--save", required=True, help="offline .sav or .srm (close emulator first)")
        q.add_argument("--vault", help="collection.vault.json; omit only for first deposit/empty vault")
        if cmd != "info":
            q.add_argument("--box", type=int, help="1-based PC box")
            q.add_argument("--slot", type=int, help="1-based PC slot")
            q.add_argument("--out", required=True, help="NEW output DIRECTORY, must not exist")
        if cmd == "info":
            q.add_argument("--species", type=int, help="exact Pokémon species/form ID")
            q.add_argument("--nickname", help="case-insensitive nickname text")
            q.add_argument("--limit", type=int, default=30, help="max archive records displayed (default 30)")
        if cmd == "sweep":
            q.add_argument("--keep", type=int, default=30,
                           help="keep this many boxed Pokémon (box order), archive remaining, default 30")
        if cmd == "withdraw":
            q.add_argument("--id", required=True, help="vault entry ID printed by info")
    args = p.parse_args()
    try:
        run(args)
        return 0
    except (VaultError, OSError, ValueError) as exc:
        print(f"Grand Vault refused operation: {exc}", file=sys.stderr)
        return 1

if __name__ == "__main__":
    raise SystemExit(main())
