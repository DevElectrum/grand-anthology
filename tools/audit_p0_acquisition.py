#!/usr/bin/env python3
"""Source-level regional obtainability audit, National #1..1025.

Deliberately optimistic, not real catchability verification. A positive count
does not prove map tile reachability, story gates, item/move evolutions, save
persistence, wild RNG, or usable alternate-form mechanisms. A missing source
can also be a custom fossil or indirect gift the parser cannot recognize.
"""
from collections import defaultdict
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
def read(rel):
    return (ROOT / rel).read_text(encoding="utf-8")

bounds = ((1,151,"KANTO"), (152,251,"JOHTO"), (252,386,"HOENN"),
          (387,493,"KANTO"), (494,649,"JOHTO"), (650,721,"HOENN"),
          (722,809,"KANTO"), (810,905,"JOHTO"), (906,1025,"HOENN"))
owner = {n:r for low,high,r in bounds for n in range(low,high+1)}
enum = read("include/constants/pokedex.h").split("enum NationalDexOrder",1)[1].split("};",1)[0]
names = re.findall(r"\bNATIONAL_DEX_([A-Z0-9_]+)\b(?=\s*[,=])",enum)
assert len(names)>=1026 and names[0]=="NONE",len(names)
names = names[1:1026]
assert len(set(names))==1025
by_name = {name:i for i,name in enumerate(names,1)}
species_dex = dict(by_name)
all_declared_form_species = set()
evolutions = set()
for i in range(1,10):
    source = read(f"src/data/pokemon/species_info/gen_{i}_families.h")
    mons = list(re.finditer(r"^\s*\[SPECIES_([A-Z0-9_]+)\]\s*=\s*", source, re.M))
    for j,m in enumerate(mons):
        mon = m.group(1)
        all_declared_form_species.add(mon)
        end = mons[j+1].start() if j+1<len(mons) else len(source)
        entry = source[m.end():end]
        nat = re.search(r"\.natDexNum\s*=\s*NATIONAL_DEX_([A-Z0-9_]+)",entry)
        if nat and nat.group(1) in by_name:
            species_dex[mon] = by_name[nat.group(1)]
        if ".evolutions =" in entry:
            evol = entry.split(".evolutions =",1)[1]
            for child in re.findall(r"\{\s*EVO_[A-Z0-9_]+\s*,\s*[^,{}]+\s*,\s*SPECIES_([A-Z0-9_]+)",evol):
                evolutions.add((mon,child))

# SpeciesInfo macros for permanent variants often expand natDexNum in C rather
# than listing it in source. Match only actual declared forms, using longest
# official National base name first; never count battle-only forms as sources.
for mon in all_declared_form_species:
    if mon in species_dex:
        continue
    for national in sorted(by_name, key=len, reverse=True):
        if mon.startswith(national + "_"):
            species_dex[mon] = by_name[national]
            break

# Normalize alias constants to the actual forms defined in SpeciesInfo.
# E.g. SPECIES_GIMMIGHOUL names the chest form, whose evolution to
# Gholdengo is stored under SPECIES_GIMMIGHOUL_CHEST. The field catalog
# intentionally spells the canonical alias. Mothim is similarly a macro-
# initialized permanent form, not an unavailable Pokédex number.
species_enum = read("include/constants/species.h")
form_aliases = dict(re.findall(
    r"\bSPECIES_([A-Z0-9_]+)\s*=\s*SPECIES_([A-Z0-9_]+)\s*,", species_enum))
def actual_species(mon):
    seen = set()
    while mon in form_aliases and mon not in seen:
        seen.add(mon)
        mon = form_aliases[mon]
    return mon

groups = json.loads(read("data/maps/map_groups.json"))
order = groups["group_order"]
k = order.index("gMapGroup_Link_Frlg")
j = order.index("gMapGroup_JohtoTownsAndRoutes")
h = order.index("gMapGroup_RegionHub")
group_number = {name:idx for idx,group in enumerate(order) for name in groups[group]}
regions = {}
map_path = {}
for path in (ROOT/"data/maps").glob("*/map.json"):
    m = json.loads(path.read_text(encoding="utf-8"))
    group_idx = group_number.get(path.parent.name,-1)
    r = m.get("region","")
    if r in ("REGION_KANTO","REGION_JOHTO","REGION_HOENN"):
        r = r[7:]
    else:
        r = ("HOENN" if 0<=group_idx<k else "KANTO" if k<=group_idx<j
             else "JOHTO" if j<=group_idx<h else "OTHER")
    regions[m["id"]]=r
    map_path[path.parent.name]=m["id"]

# These are post-League areas. Sky Pillar is NOT: the Sootopolis crisis
# advances VAR_SKY_PILLAR_STATE to 2/3 and unlocks Rayquaza before Champion.
POSTGAME = ("CERULEAN_CAVE","MT_SILVER","BIRTH_ISLAND",
            "FARAWAY_ISLAND","NAVEL_ROCK","SOUTHERN_ISLAND")
def preleague(loc):
    return loc in regions and regions[loc] in ("KANTO","JOHTO","HOENN") and not any(v in loc for v in POSTGAME)

seed = defaultdict(set)
provenance = defaultdict(lambda:defaultdict(set))
def add(loc,mon,how):
    mon = actual_species(mon)
    if preleague(loc) and mon in species_dex:
        region = regions[loc]
        seed[region].add(mon)
        provenance[region][mon].add(how+":"+loc)

encounters = json.loads(read("src/data/wild_encounters.json"))
for group in encounters["wild_encounter_groups"]:
    for encounter in group.get("encounters",[]):
        loc = encounter.get("map","")
        for method in ("land_mons","water_mons","fishing_mons","rock_smash_mons"):
            for item in encounter.get(method,{}).get("mons",[]):
                add(loc,item["species"].removeprefix("SPECIES_"),"native_"+method)

for filename,pattern,method in (
    ("src/data/anthology_rare_habitats.h",r"^RARE_ON_MAP\((MAP_\w+), ([A-Z0-9_]+)\)", "rare"),
    ("src/data/anthology_extra_habitats.h",r"^ECOLOGY_ON_MAP\((MAP_\w+), ([A-Z0-9_]+)\)", "ecology"),
    ("src/data/anthology_aquatic_habitats.h",r"^AQUATIC_ON_MAP\((MAP_\w+), (?:WATER|FISHING), ([A-Z0-9_]+)\)", "aquatic"),
):
    for loc,mon in re.findall(pattern,read(filename),re.M):
        add(loc,mon,method)

for path in (ROOT/"data/maps").glob("*/scripts.inc"):
    loc = map_path.get(path.parent.name)
    if loc is None:
        continue
    text = path.read_text(encoding="utf-8")
    for mon in re.findall(r"(?m)^\s*(?:setwildbattle|seteventmon|givepokemon|givemon)\s+SPECIES_([A-Z0-9_]+)\b",text):
        add(loc,mon,"script")

# Starter menu source is useful but not independent runtime proof.
for named in set(re.findall(r"\bSPECIES_([A-Z0-9_]+)\b",read("src/starter_choose.c"))):
    mon = actual_species(named)
    if mon in species_dex:
        r = owner[species_dex[mon]]
        seed[r].add(mon)
        provenance[r][mon].add("starter_menu:review")

# Special breeding branch, not an ordinary evolution.
if "MANAPHY" in seed["KANTO"]:
    seed["KANTO"].add("PHIONE")
    provenance["KANTO"]["PHIONE"].add("Manaphy_breeding:review")

# Regression for common name aliases. In the wild C catalog
# SPECIES_GIMMIGHOUL = SPECIES_GIMMIGHOUL_CHEST; the chest species alone
# stores the level + 30 Gimmighoul Coins evolution to Gholdengo.
assert actual_species("GIMMIGHOUL") == "GIMMIGHOUL_CHEST", "missing Gimmighoul enum alias"
assert ("GIMMIGHOUL_CHEST", "GHOLDENGO") in evolutions
assert species_dex["MOTHIM_PLANT"] == by_name["MOTHIM"]

print("P0 NATIONAL SOURCE REACHABILITY (optimistic candidates, not gameplay acceptance)")
total=0
for region in ("KANTO","JOHTO","HOENN"):
    reach=set(seed[region])
    changed=True
    while changed:
        size=len(reach)
        reach.update(child for parent,child in evolutions if parent in reach)
        changed=len(reach)!=size
    nums={species_dex[mon] for mon in reach if mon in species_dex}
    wanted={n for n,r in owner.items() if r==region}
    missing=sorted(wanted-nums)
    total+=len(wanted & nums)
    print(f"{region}: {len(wanted & nums)}/{len(wanted)} source-reachable (optimistic); {len(missing)} candidate gaps")
    print("CANDIDATE GAPS "+region+": "+(", ".join(f"#{n} {names[n-1]}" for n in missing) or "none"))
    # The National count intentionally collapses stable variants. Track the
    # separate acquisition sources of Alolan/Galarian/Hisuian/Paldean forms
    # before declaring the *forms* portion of P0 implemented.
    regional_forms = sorted(mon for mon in all_declared_form_species
                            if re.search(r"_(?:ALOLA|GALAR|HISUI|PALDEA)(?:_|$)", mon)
                            and species_dex.get(mon) in wanted)
    absent_forms = [mon for mon in regional_forms if mon not in reach]
    print(f"{region}: {len(regional_forms) - len(absent_forms)}/{len(regional_forms)}"
          " regional form source candidates (not gameplay verification)")
    print("REGIONAL FORM GAPS "+region+": "+(", ".join(absent_forms) or "none"))
print(f"COMBINED optimistic {total}/1025.")
assert total == 1025, f"National optimistic source routes incomplete: {total}/1025"
print("ALL 1025 numbered National entries have an optimistic region-source/evolution route.")
print("NOT VERIFIED: fossil revival, breeding graph, item/move/level gates, map passage, event script outcomes, forms, real ROM catch/save.")
