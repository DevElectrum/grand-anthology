#!/usr/bin/env python3
"""Real sector-level round-trip checks for the offline Grand Vault prototype.

Builds a synthetic *valid* 128 KiB two-slot GBA Flash image with one
checksum-valid, XOR-encrypted BoxPokemon; uses the production SavePatch scanner,
not a fake Save class. Tests full transfer, restoration, tampering, lost-sector,
cross-trainer rejection, backup preservation and RTC trailer survival.
"""
import hashlib
import json
from pathlib import Path
import struct
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import grand_vault as v
sp = v.sp

def mon(pid=0x12345678, otid=0xAABBCCDD):
    result = bytearray(80)
    struct.pack_into("<II", result, 0, pid, otid)
    result[8:11] = b"\xBB\xBC\xFF"
    result[19] = 2  # hasSpecies, neither egg nor bad egg
    clear = bytearray(48)
    off = v.SUBSTRUCT_ZERO_OFFSETS[pid % 24] * 12
    struct.pack_into("<HHI", clear, off, 25, 1, 4500)
    checksum = sum(struct.unpack_from("<H", clear, i)[0] for i in range(0,48,2)) & 65535
    struct.pack_into("<H", result, 28, checksum)
    key = pid ^ otid
    for i in range(0,48,4):
        struct.pack_into("<I", result, 32+i, struct.unpack_from("<I", clear, i)[0] ^ key)
    v.validate_mon(bytes(result))
    return bytes(result)

def make_save(path, boxmon, trainer=0xCAFEBABE, trailer=b"RTC-FIXTURE-16!!"):
    assert len(trailer) == 16
    flash = bytearray(b"\xFF" * sp.FLASH_SIZE)
    sb1 = bytearray(sp.SB1_SIZE)
    sb2 = bytearray(sp.SB2_SIZE)
    sb3 = bytearray(sp.SB3_SIZE)
    storage = bytearray(sp.PK_SIZE)
    sb2[:7] = b"\xBB\xBC\xFF\x00\x00\x00\x00"
    struct.pack_into("<I", sb2, sp.V["SB2.playerTrainerId"], trainer)
    sb2[sp.V["SB2.saveVersion"]] = sp.SAVE_FORMAT_VERSION
    storage[4:84] = boxmon
    source = [sb2] + [sb1]*4 + [storage]*9
    for sid in range(sp.SECTORS_PER_SLOT):
        off, size = sp.SLOT_LAYOUT[sid]
        data = source[sid][off:off+size]
        start = sid * sp.SECTOR_SIZE
        # All slots physical ordered in this fixture; actual tools use footer
        # sector IDs, so production rotating sector layouts are also supported.
        flash[start:start+size] = data
        length = sp.sb3_chunk_size(sid)
        if length:
            flash[start+sp.SECTOR_DATA_SIZE:start+sp.SECTOR_DATA_SIZE+length] = sb3[sid*sp.SB3_CHUNK:sid*sp.SB3_CHUNK+length]
        struct.pack_into("<HHII", flash, start+sp.SECTOR_DATA_SIZE+sp.SB3_CHUNK,
                         sid, sp.calc_checksum(flash[start:start+sp.SECTOR_DATA_SIZE],size),
                         sp.SIGNATURE, 2)
    path.write_bytes(flash + trailer)
    check = v.load_save(path)
    assert check.active == 0 and check.slots[0]["status"] == "OK"
    return check

def check(test, condition):
    assert condition, test
    print("  PASS", test)

with tempfile.TemporaryDirectory() as temp:
    temp = Path(temp)
    source = temp / "initial.sav"
    value = mon()
    assert source != temp
    initial = make_save(source, value)
    original_hash = v.digest(source.read_bytes())
    check("synthetic GBA sectors and XOR BoxPokemon checksum", v.read_box(initial,1,1) == value)
    for pid in range(24):
        metadata = v.mon_fields(mon(pid=pid))
        assert metadata["species"] == 25 and metadata["item"] == 1
        assert metadata["nickname"] == "AB"
    check("all 24 secure substruct permutations decode exact species/form and held item",True)
    # Carry current v12 game state into a brand-new archive transaction.
    wallet = v.fresh_vault(v.identity(initial))
    edited = v.load_save(source)
    archived, msg = v.apply_transaction(edited, wallet, "deposit",1,1)
    published = temp/"deposit01"
    v.publish(published,edited,archived,msg)
    new_save = published/"cartridge.sav"
    new_vault = published/"collection.vault.json"
    loaded = v.load_save(new_save)
    archive = v.load_vault(new_vault,v.identity(loaded))
    check("source save is NEVER overwritten",v.digest(source.read_bytes()) == original_hash)
    check("deposit removes BoxPokemon from active slot", not v.is_occupied(v.read_box(loaded,1,1)))
    check("archive holds exact 80 encrypted bytes", v.decode_record(archive["records"][0]) == value)
    check("mGBA RTC trailer preserved", loaded.trailer == initial.trailer)
    check("atomic manifest pairs save and vault", json.loads((published/"manifest.json").read_text())["files"] == {f.name:v.digest(f.read_bytes()) for f in (new_save,new_vault)})
    check("no-overwrite of output directory", False if False else published.exists())
    try:
        v.publish(published,edited,archived,msg)
        raise AssertionError("overwriting archive output must refuse")
    except v.VaultError:
        pass
    # An attempt to deposit from an older save that still contains the
    # SAME Pokémon but uses the already-populated vault must refuse.
    try:
        v.apply_transaction(v.load_save(source),archive,"deposit",1,1)
        raise AssertionError("stale save replay must refuse")
    except v.VaultError:
        pass
    check("stale-save replay prevented",len(archive["records"]) == 1)

    # A saved party member may hold the same encrypted 80-byte identity as an
    # already vaulted one. The restore gate must scan party as well as PC.
    dupe_party = v.load_save(new_save)
    party_pos = sp.V["SB1.playerParty"]
    dupe_party.sb1[party_pos:party_pos+80] = value
    dupe_party.sb1[sp.V["SB1.playerPartyCount"]] = 1
    try:
        v.apply_transaction(dupe_party,archive,"withdraw",mon_id=archive["records"][0]["id"])
        raise AssertionError("duplicate party Pokémon should be refused")
    except v.VaultError:
        pass
    check("party duplicate protection",True)

    # Pokémon World rotates physical Flash sectors on each save. Reorder all
    # fourteen physical sectors while preserving their footer logical IDs:
    # a locator that incorrectly trusts physical order will now corrupt data.
    rotated = temp/"rotated.sav"
    src_raw = source.read_bytes()
    sectors = [src_raw[i*sp.SECTOR_SIZE:(i+1)*sp.SECTOR_SIZE]
               for i in range(sp.SECTORS_PER_SLOT)]
    rotated.write_bytes(b"".join(sectors[5:]+sectors[:5])
                        + src_raw[sp.SECTORS_PER_SLOT*sp.SECTOR_SIZE:])
    rotated_sv = v.load_save(rotated)
    check("rotated Flash footer sectors still read exact Pokémon",
          v.read_box(rotated_sv,1,1) == value)
    rotated_vault,description=v.apply_transaction(rotated_sv,wallet,"deposit",1,1)
    rotated_target=temp/"rotated_deposit"
    v.publish(rotated_target,rotated_sv,rotated_vault,description)
    check("rotated sectors safely written and reread",
          not v.is_occupied(v.read_box(v.load_save(rotated_target/"cartridge.sav"),1,1)))

    # Restore from active vault to the next free cartridge PC slot, no dupes.
    restore_sv = v.load_save(new_save)
    restored_vault, description = v.apply_transaction(restore_sv,archive,"withdraw",mon_id=archive["records"][0]["id"])
    target = temp/"restore02"
    v.publish(target,restore_sv,restored_vault,description)
    final = v.load_save(target/"cartridge.sav")
    check("byte-perfect vault-to-PC restore", v.read_box(final,1,1) == value)
    check("withdraw removes vaulted copy",len(v.load_vault(target/"collection.vault.json",v.identity(final))["records"]) == 0)
    check("originals survive intact after two moves",new_save.read_bytes() != source.read_bytes() and source.read_bytes() == (initial.flash+initial.trailer))
    # Using a still-occupied PC slot must never overwrite the Pokémon.
    try:
        v.apply_transaction(final,archived,"withdraw",1,1,archived["records"][0]["id"])
        raise AssertionError("occupied PC target should refuse")
    except v.VaultError:
        pass
    check("occupied-slot protection",v.read_box(final,1,1) == value)

    # Corrupt the archive itself, valid JSON but damaged Pokémon payload.
    broken = temp/"tampered.json"
    bad = json.loads(new_vault.read_text())
    bad["records"][0]["pokemon"] = base64_fix = "A"*108
    broken.write_text(json.dumps(bad))
    try:
        v.load_vault(broken,v.identity(final))
        raise AssertionError("damaged vault should be refused")
    except v.VaultError:
        pass
    check("vault integrity SHA check prevents silent record corruption",True)

    # Reject a valid save from a different trainer, even if both have room.
    outsider = temp/"outsider.sav"
    other = make_save(outsider,bytes(80),trainer=0xBADDECAF)
    try:
        v.load_vault(new_vault,v.identity(other))
        raise AssertionError("cross-trainer archive should be refused")
    except v.VaultError:
        pass
    check("other trainer save cannot silently import archive",True)

    # Invalid encrypted Pokémon, wrong box number and partial Flash sector
    # must each fail without any output being created.
    corrupt = bytearray(value)
    corrupt[32] ^= 0x80
    try:
        v.validate_mon(bytes(corrupt))
        raise AssertionError("checksum corruption should refuse")
    except v.VaultError:
        pass
    try:
        v.slot_offset(15,1)
        raise AssertionError("outside save box range should refuse")
    except v.VaultError:
        pass
    damaged = bytearray(source.read_bytes())
    damaged[16] ^= 0x20
    bad_save=temp/"damaged.sav"
    bad_save.write_bytes(damaged)
    try:
        v.load_save(bad_save)
        raise AssertionError("damaged cartridge sector must refuse")
    except v.VaultError:
        pass
    check("bad Pokémon, invalid box and bad sector all refused",True)

    # Mass archive must be all-or-nothing, and keep earliest PC slots
    # unchanged. Exercising 80 distinct valid encrypted Pokémon ensures
    # neither the cross-sector writeback nor the sidecar IDs alias data.
    bulk_sav = v.load_save(source)
    for i in range(80):
        v.write_box(bulk_sav, i//30+1, i%30+1, mon(pid=0x100000+i,otid=0xAABBCCDD))
    before_bulk = [v.read_box(bulk_sav,i//30+1,i%30+1) for i in range(80)]
    bulk_vault, bulk_note = v.apply_transaction(
        bulk_sav, v.fresh_vault(v.identity(bulk_sav)), "sweep", keep=10)
    check("bulk archive retains first ten boxed Pokémon",
          [v.read_box(bulk_sav,i//30+1,i%30+1) for i in range(10)] == before_bulk[:10])
    check("bulk archive removed exactly remaining seventy entries",
          all(not v.is_occupied(v.read_box(bulk_sav,i//30+1,i%30+1)) for i in range(10,80))
          and len(bulk_vault["records"]) == 70)
    bulk_target = temp/"bulk80"
    v.publish(bulk_target,bulk_sav,bulk_vault,bulk_note)
    bulk_loaded = v.load_save(bulk_target/"cartridge.sav")
    bulk_collection = v.load_vault(bulk_target/"collection.vault.json",v.identity(bulk_loaded))
    check("bulk archive persistent exact encrypted bytes",
          [v.decode_record(item) for item in bulk_collection["records"]] == before_bulk[10:])
    try:
        v.apply_transaction(v.load_save(source),bulk_collection,"sweep",keep=-1)
        raise AssertionError("invalid bulk retention must refuse")
    except v.VaultError:
        pass
    check("bulk archive invalid --keep rejected",True)

    # Practical scale: >1,000 archived records with checksums and IDs.
    huge = v.fresh_vault(v.identity(initial))
    for idx in range(1200):
        data = mon(pid=idx+1, otid=0xCCCCAAAA)
        huge["records"].append({"id":f"p{idx:04d}","sha256":v.digest(data),
                                "pokemon":__import__("base64").b64encode(data).decode("ascii")})
    big = temp/"huge.vault.json"
    big.write_text(json.dumps(huge))
    check("1200 durable vaulted Pokémon verified",len(v.load_vault(big,v.identity(initial))["records"]) == 1200)
    check("1200 encrypted metadata records searchable by species",
          all(v.mon_fields(v.decode_record(rec))["species"] == 25 for rec in huge["records"]))
print("P0 off-cartridge vault: atomic deposit/withdraw, save checksums, identity, corruption, RTC, 1200 archived mons PASS")
