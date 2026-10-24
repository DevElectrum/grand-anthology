#!/usr/bin/env python3
"""All-generation Pokémon family availability validator (host-side, no emulator).

P0 requires *every* family, not the earlier Gen 1-3-only world. Check the full
family registry, flag any disabled family and any obtainable references to it.
This does NOT prove every species/form is obtainable, nor that all assets fit.
That requires separate encounter/form audits and the real GBA compilation.

Keep this filename for existing Makefile/CI references; its former Gen 1-3-only
semantics were superseded by docs/P0_P1_ROADMAP.md (October 21, 2026).
"""
import re, sys, glob, os, json

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

# A parse that finds nothing must be an error, not a pass: every check below is a lookup that
# silently succeeds against an empty table, so a moved file or a drifted regex would turn this
# script into a no-op that still prints OK. Floors are set well under the real counts.
MIN_DISABLED_FAMILIES = 0
MIN_SPECIES_MAPPED = 800

def read(path):
    with open(os.path.join(ROOT, path), encoding="utf-8", errors="replace") as f:
        return f.read()

def die(msg):
    print(f"FAIL — {msg}")
    sys.exit(1)

# 1. Disabled families from species_enabled.h (FALSE = world-stripped).
disabled_families = set()
for m in re.finditer(r"#define\s+(P_FAMILY_\w+)\s+FALSE\b", read("include/config/species_enabled.h")):
    disabled_families.add(m.group(1))

# 2. species -> family from the gen_*_families.h #if P_FAMILY_* nesting.
#    Tracks the innermost P_FAMILY guard; collects SPECIES_ ids and display names.
species_family = {}       # "SPECIES_LUXRAY" -> "P_FAMILY_SHINX"
name_family = {}          # "luxray" (lowercased display name) -> family
families_by_gen = {}      # 4 -> {"P_FAMILY_SHINX", ...}
for path in sorted(glob.glob(os.path.join(ROOT, "src/data/pokemon/species_info/gen_*_families.h"))):
    gen = int(re.search(r"gen_(\d+)_families\.h$", path).group(1))
    families_by_gen.setdefault(gen, set())
    stack = []
    cur_species = None
    for line in open(path, encoding="utf-8", errors="replace"):
        m = re.match(r"\s*#if\s+(P_FAMILY_\w+)", line)
        if m:
            stack.append(m.group(1))
            families_by_gen[gen].add(m.group(1))
            continue
        if re.match(r"\s*#(if|ifdef|ifndef)\b", line):
            stack.append(None); continue
        # An #else/#elif arm is NOT guarded by the family above it. Without this the species in
        # the else arm get attributed to a family that does not gate them.
        if re.match(r"\s*#(else|elif)\b", line):
            if stack: stack[-1] = None
            continue
        if re.match(r"\s*#endif", line):
            if stack: stack.pop()
            continue
        fam = next((s for s in reversed(stack) if s), None)
        m = re.match(r"\s*\[(SPECIES_\w+)\]\s*=", line)
        if m and fam:
            cur_species = m.group(1)
            species_family[cur_species] = fam
            continue
        m = re.search(r'\.speciesName\s*=\s*_\(\s*"([^"]+)"', line)
        if m and fam and cur_species:
            name_family.setdefault(m.group(1).lower(), fam)

if disabled_families:
    die(f"{len(disabled_families)} family/families still disabled in species_enabled.h: "
        + ", ".join(sorted(disabled_families)[:20]))
if len(species_family) < MIN_SPECIES_MAPPED:
    die(f"only {len(species_family)} species mapped from gen_*_families.h "
        f"(expected >= {MIN_SPECIES_MAPPED}). The glob or the regex drifted — "
        f"this scan would pass vacuously.")

def family_of(token):
    """token: SPECIES_X or a display name. Returns family or None if unresolvable."""
    if token.startswith("SPECIES_"):
        return species_family.get(token)
    return name_family.get(token.lower())

violations = []
def check(token, where):
    fam = family_of(token)
    if fam and fam in disabled_families:
        violations.append((where, token, fam))

# 3. Trainer party files (BOTH formats: display names + SPECIES_ constants).
#    Scanned from the .party SOURCE, not the trainerproc-generated trainers.h.
KEY_LINE = re.compile(r"^\s*(\w[\w .']*?):\s")   # "Level: 5", "Ability: ..." etc.
for pf in ["src/data/trainers.party", "src/data/trainers_frlg.party",
           "src/data/battle_partners.party", "src/data/debug_trainers.party"]:
    if not os.path.exists(os.path.join(ROOT, pf)):
        continue
    for ln, line in enumerate(read(pf).splitlines(), 1):
        s = line.strip()
        if not s or s.startswith("===") or s.startswith("/*") or s.startswith("*") or s.startswith("//"):
            continue
        if KEY_LINE.match(s) or s.startswith("-") or s.startswith("."):
            continue
        # A mon header line: "Name (M) @ Item" / "SPECIES_X @ Item" / "Nickname (Name)"
        head = s.split("@")[0].strip()
        m = re.match(r"(SPECIES_\w+)", head)
        if m:
            check(m.group(1), f"{pf}:{ln}")
            continue
        base = re.sub(r"\s*\((M|F)\)\s*$", "", head)
        inner = re.match(r".*\(([^)]+)\)\s*$", base)  # "Nickname (Species)"
        cand = inner.group(1) if inner else base
        if cand.lower() in name_family:
            check(cand, f"{pf}:{ln}")

# 4. Wild encounters, from the TRACKED JSON.
#    src/data/wild_encounters.h is generated and gitignored (src/data/.gitignore), so the old
#    glob over wild_encounters*.h matched nothing on a fresh clone, nothing after
#    `make clean-generated`, and stale content whenever the JSON was edited but not rebuilt —
#    and still printed OK. Same treatment the .party files already get: scan the source.
#    The National Park Bug-Catching Contest pool routes through this table too.
WILD_JSON = "src/data/wild_encounters.json"
if not os.path.exists(os.path.join(ROOT, WILD_JSON)):
    die(f"{WILD_JSON} not found — wild encounters would go unscanned.")
wild_species_seen = 0
try:
    wild = json.loads(read(WILD_JSON))
except json.JSONDecodeError as e:
    die(f"{WILD_JSON} did not parse: {e}")
for group in wild.get("wild_encounter_groups", []):
    for enc in group.get("encounters", []):
        where_map = enc.get("map") or enc.get("base_label") or "?"
        for field, val in enc.items():
            if not isinstance(val, dict) or "mons" not in val:
                continue
            for mon in val["mons"]:
                sp = mon.get("species")
                if sp:
                    wild_species_seen += 1
                    check(sp, f"{WILD_JSON}:{where_map}/{field}")
if wild_species_seen == 0:
    die(f"{WILD_JSON} parsed but yielded no species — the schema changed.")

# 5. Facility/static species tables. (The old ingame_trades*.h glob matched no file in this
#    repo and was a dead entry; trade.h is the live one.)
SPECIES_SOURCES = (
    glob.glob(os.path.join(ROOT, "src/data/trade.h"))
    + glob.glob(os.path.join(ROOT, "src/data/battle_frontier/*.h"))
)
for path in SPECIES_SOURCES:
    rel = os.path.relpath(path, ROOT)
    for ln, line in enumerate(open(path, encoding="utf-8", errors="replace"), 1):
        for m in re.finditer(r"\bSPECIES_\w+\b", line):
            check(m.group(0), f"{rel}:{ln}")

# 6. Gift/egg/static-battle script macros across data/.
GIVE = re.compile(r"\b(givemon|giveegg|setwildbattle|seteventmon|givenamedmon|giveoddegg)\b[^\n]*?(SPECIES_\w+)")
for path in glob.glob(os.path.join(ROOT, "data/**/*.inc"), recursive=True) + [os.path.join(ROOT, "data/event_scripts.s")]:
    rel = os.path.relpath(path, ROOT)
    try:
        text = open(path, encoding="utf-8", errors="replace").read()
    except OSError:
        continue
    for ln, line in enumerate(text.splitlines(), 1):
        for m in GIVE.finditer(line):
            check(m.group(2), f"{rel}:{ln}")

# Verify registry/parse completeness. This was previously an old Gen 1-3-only
# strip validator; every mapped Gen 4–9 family must now have an enabling define.
config = read("include/config/species_enabled.h")
defined_families = set(re.findall(r"^#define\s+(P_FAMILY_\w+)\s+", config, re.M))
late = {fam for gen in families_by_gen if gen >= 4 for fam in families_by_gen[gen]}
missing = late - defined_families
for gen in range(1, 10):
    if not re.search(rf"^#define\s+P_GEN_{gen}_POKEMON\s+TRUE\b", config, re.M):
        missing.add(f"P_GEN_{gen}_POKEMON is not TRUE")

print(f"families disabled: {len(disabled_families)} | species mapped: {len(species_family)} "
      f"(+{len(name_family)} names) | wild entries: {wild_species_seen}")

failed = False
if missing:
    failed = True
    print(f"FAIL — {len(missing)} family/generation enable declarations missing:")
    for fam in sorted(missing):
        print(" ", fam)
if violations:
    failed = True
    print(f"FAIL — {len(violations)} disabled-species reference(s):")
    for where, token, fam in violations:
        print(f"  {where}: {token} ({fam} is FALSE)")
if failed:
    sys.exit(1)
print("OK — all existing-generation Pokémon families enabled; static content has no disabled references.")
