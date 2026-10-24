# Legendary and Mythical location audit (read-only snapshot, 24 October 2026)

**Repo:** [`DevElectrum/grand-anthology`](https://github.com/DevElectrum/grand-anthology), public `main`. **Purpose:** review exactly where Legendary and Mythical Pokémon have been placed, before any gameplay changes. The user explicitly requested an audit first and discussion before implementing. **This document does not approve relocation or altered encounter rules.**

## Design decisions confirmed in conversation

- The P0 completion target is **all 1,025 National Pokédex species obtainable eventually over the complete Kanto / Johto / Hoenn adventure, including postgame**, not before beating the first Champion. Preserve reasonable natural story and postgame gating.
- **Legendary and Mythical Pokémon should be static encounters rather than ordinary grass/water/fishing RNG.** A static does not inherently require a unique quest, exotic map, or post-Champion gate. **Do not invent extra requirements.**
- **Ultra Beasts and Paradox Pokémon are explicitly exempt**: they are intentionally rare natural encounters. **Kartana in Viridian Forest was the user's own requested placement. Poipole on Route 1 is also accepted.** Do not "fix" or remove these as if Legendary static rules applied. Other Ultra Beasts/Paradoxes likewise stay outside this audit.
- No changes to encounter tables, statics, story flags, balancing, or testing infrastructure have been authorized in this discussion. **Next Astra: read, discuss, wait.**

## What this audit covers and what it does not

This is a **source-level** inventory of the nine-generation Legendary/Mythical roster and documented map/event sources. It combines the [89 added static-object inventory](../test/overworld/ValidateAnthologyHabitatRare.py), [supplementary habitat catalog](../src/data/anthology_extra_habitats.h), [rare habitat catalog](../src/data/anthology_rare_habitats.h), [wild generation logic](../src/wild_encounter.c), [roamer logic](../src/roamer.c), and inherited scripted locations under [`data/maps`](../data/maps). "Added static" describes a field object and scripted battle in source; it **does not certify** a walkable route, correct flag, successful capture, repeat-visit lifecycle, or access at any specific badge count. Where multiple existing original-map variants exist (e.g. FRLG/HGSS-style maps), the named destination is given rather than claiming that every map variant is reachable.

Species are listed **once per National Pokédex entry**, with significant alternate forms and multiple source locations described in the notes. Evolution/breeding entries are not independent static encounters. Normal debug/CHEATS species creation is excluded as a designed source.

## Urgent discoveries to discuss, not automatically change

1. **Five Legendary wild-encounter entries exist in `anthology_extra_habitats.h`:** Groudon at Route 111, Kyogre at Seafloor Cavern Room 1, Galarian Zapdos at Route 22, Galarian Articuno at Seafoam Islands B1F, and Galarian Moltres at Pokémon Mansion 1F. All five also have **separate scripted static placements**. Supplemental species are rolled at **2% per eligible ordinary land encounter** in `src/wild_encounter.c` (subject to map, story and DexNav capacity checks). These wild entries conflict with the newly confirmed static-Legendary preference; **do not delete them before discussing the desired placements.**
2. **Post-first-Hall-of-Fame global discovery rotation** (`WorldChoosePostLeagueDiscoverySpecies` in `src/wild_encounter.c`) substitutes an eligible species in **every third qualifying ordinary wild battle**. A player-selected Research target can become that substituted species. `WorldIsFieldStableDiscoverySpecies` excludes battle-only forms but **does not exclude Legendary or Mythical flags**. Consequently, the current logic can generate most field-stable Legendaries/Mythicals in ordinary habitats after one Championship even if all five explicit grass entries are removed. Decide what to do with that mechanism **with the user**, not by assumption.
3. Several newly scripted static Legendaries are on comparatively early maps: Dialga/Palkia/Necrozma/Cresselia on Mt. Moon B1F; Marshadow/Cosmog on Mt. Moon 1F; Shaymin/Tapu Bulu on Route 5; Mew/Meltan/Mewtwo on Pokémon Mansion 1F; Koraidon/Hoopa/Okidogi on Route 111. **These are static rather than grass**, so they do not automatically violate the user's rule, and badge/story availability has **not** been tested. Do not infer that the user wants all Legendaries postponed.

## Per-species map and acquisition catalog

The locations below are actual authored map/script or habitat references. "Added" means Grand Anthology's field objects, not inherited Pokémon World content; in some cases several legitimate source routes coexist.

### Generation 1

| Species | Authored location / method | Additional detail |
|---|---|---|
| Articuno | Seafoam Islands B4F (inherited static) | Galarian Articuno: Route 40 (static); Seafoam Islands B1F (supplemental wild) |
| Zapdos | Kanto Power Plant (inherited static) | Galarian Zapdos: Johto Route 38 (static); Kanto Route 22 (supplemental wild) |
| Moltres | Mt. Ember summit (inherited static); Mt. Silver 1F Moltres Room (Johto static) | Galarian Moltres: Burned Tower 1F (static); Pokémon Mansion 1F (supplemental wild) |
| Mewtwo | Cerulean Cave B1F (inherited static); Pokémon Mansion 1F (additional scripted static, labelled pre-League) | Cerulean Cave is postgame; the Mansion addition is a separate source |
| Mew | Pokémon Mansion 1F (added static); Faraway Island interior (inherited event static) | The Faraway Island event has its own access requirements |

### Generation 2

| Species | Authored location / method | Additional detail |
|---|---|---|
| Raikou | Burned Tower B1F beast-release event -> Johto roaming; Route 43 backup static | Roamer is released in the story, not simply an ordinary grass habitat |
| Entei | Burned Tower B1F beast-release event -> Johto roaming; Route 46 backup static | Separate later Hall of Fame respawn logic |
| Suicune | Burned Tower B1F event; Route 35 backup static; Route 42 story encounter; Kanto Route 25 later static | Route 25 explicitly checks Johto Champion and encounter progression |
| Lugia | Whirl Islands Lugia Chamber (inherited story static); Route 40 (added static); Navel Rock bottom/base (event static) | Multiple scripted sources |
| Ho-Oh | Tin Tower roof (inherited story static); Burned Tower 1F (added static); Navel Rock top/summit (event static) | Multiple scripted sources |
| Celebi | Ilex Forest GS Ball/story event and later secondary static | Secondary Ilex event explicitly checks Johto Champion |

### Generation 3

| Species | Authored location / method | Additional detail |
|---|---|---|
| Regirock | Desert Ruins (inherited static) | Regi-door puzzle requirements apply |
| Regice | Island Cave (inherited static) | Regi-door puzzle requirements apply |
| Registeel | Ancient Tomb (inherited static) | Regi-door puzzle requirements apply |
| Latias | Hoenn Route 118 (added static); Southern Island (event static); one possible Hoenn roamer choice | Roamer choice depends on inherited script state |
| Latios | Hoenn Route 120 (added static); Southern Island (event static); other possible Hoenn roamer choice | Roamer choice depends on inherited script state |
| Kyogre | Route 125 (added static); Marine Cave end (inherited static); Embedded Tower (event static) | **Also supplemental wild at Seafloor Cavern Room 1** |
| Groudon | Fiery Path (added static); Terra Cave end (inherited static); Embedded Tower (event static) | **Also supplemental wild on Route 111** |
| Rayquaza | Sky Pillar top (inherited static); Embedded Tower (event static) | Also part of Sootopolis story |
| Jirachi | Mossdeep Space Center 1F (added static) |  |
| Deoxys | Mossdeep Space Center 1F (added static); Birth Island exterior (inherited event static) | Alternate forms are the same National entry |

### Generation 4

| Species | Authored location / method | Additional detail |
|---|---|---|
| Uxie | Rock Tunnel B1F (added static) |  |
| Mesprit | Rock Tunnel B1F (added static) |  |
| Azelf | Rock Tunnel B1F (added static) |  |
| Dialga | Mt. Moon B1F (added static) | Potentially early, subject to progression review |
| Palkia | Mt. Moon B1F (added static) | Potentially early, subject to progression review |
| Heatran | Rock Tunnel B1F (added static) |  |
| Regigigas | Rock Tunnel B1F (added static) |  |
| Giratina | Rock Tunnel B1F (added static) |  |
| Cresselia | Mt. Moon B1F (added static) |  |
| Phione | Breed Manaphy with Ditto at a two-parent daycare | No separate wild/static placement found; Kanto Route 5 optional breeders were added for the superseded pre-Champion target |
| Manaphy | Seafoam Islands B2F (added static) |  |
| Darkrai | Rock Tunnel B1F (added static) |  |
| Shaymin | Kanto Route 5 (added static) |  |
| Arceus | Indigo Plateau exterior, Kanto (added static) | Not evidence of a Champion gate; actual story access still needs validation |

### Generation 5

| Species | Authored location / method | Additional detail |
|---|---|---|
| Victini | Johto Route 48 (added static) |  |
| Cobalion | Olivine City Lighthouse (added static) |  |
| Terrakion | Johto Route 42 (added static) |  |
| Virizion | Johto Route 36 (added static) |  |
| Tornadus | Johto Route 38 (added static) | Therian is a form, not a separate National species |
| Thundurus | Johto Route 38 (added static) | Therian is a form |
| Landorus | Johto Route 39 (added static) | Therian is a form |
| Reshiram | Johto Route 45 (added static) |  |
| Zekrom | Johto Route 45 (added static) |  |
| Kyurem | Johto Route 44 (added static) | White/Black Kyurem are forms |
| Keldeo | Johto Route 42 (added static) |  |
| Meloetta | Ecruteak City Theater (added static) |  |
| Genesect | Goldenrod City (added static) |  |

### Generation 6

| Species | Authored location / method | Additional detail |
|---|---|---|
| Xerneas | Hoenn Route 120 (added static) |  |
| Yveltal | Mt. Pyre exterior (added static) |  |
| Zygarde | Granite Cave B2F (added static) | 10%/50%/Complete are forms |
| Diancie | Granite Cave, Steven's Room (added static) | Existing emulator smoke never reached the room; this is not an accepted battle |
| Hoopa | Hoenn Route 111 (added static, Confined form) | Unbound is a form |
| Volcanion | Fiery Path (added static) |  |

### Generation 7

| Species | Authored location / method | Additional detail |
|---|---|---|
| Type: Null | Pokémon Mansion 1F (added static) |  |
| Silvally | Evolve Type: Null (friendship/evolution) | No independent static location needed |
| Tapu Koko | Kanto Power Plant (added static) |  |
| Tapu Lele | Rock Tunnel B1F (added static) |  |
| Tapu Bulu | Kanto Route 5 (added static) |  |
| Tapu Fini | Seafoam Islands B2F (added static) |  |
| Cosmog | Mt. Moon 1F and Seafoam Islands 1F (two added statics) |  |
| Cosmoem | Evolve Cosmog |  |
| Solgaleo | Evolve Cosmoem (branch) |  |
| Lunala | Evolve Cosmoem (branch) |  |
| Necrozma | Mt. Moon B1F (added static) | Fused/Ultra states are forms |
| Magearna | Kanto Power Plant (added static) |  |
| Marshadow | Mt. Moon 1F (added static) |  |
| Zeraora | Kanto Power Plant (added static) |  |
| Meltan | Pokémon Mansion 1F (added static) |  |
| Melmetal | Evolve Meltan |  |

### Generation 8

| Species | Authored location / method | Additional detail |
|---|---|---|
| Zacian | Johto Route 48 (added static) | Crowned is a form |
| Zamazenta | Ruins of Alph exterior (added static) | Crowned is a form |
| Eternatus | Johto Route 44 (added static) |  |
| Kubfu | Johto Route 42 (two added statics, west and east) |  |
| Urshifu | Evolve Kubfu (Single/Rapid Strike branches) | No separate primary static |
| Zarude | Johto Route 36 (added static) | Dada is a form |
| Regieleki | Ruins of Alph exterior (added static) |  |
| Regidrago | Johto Route 45 (added static) |  |
| Glastrier | Johto Route 44 and Hoenn Shoal Cave low-tide ice room (added statics) |  |
| Spectrier | Burned Tower B1F (added static) |  |
| Calyrex | Johto Route 36 (two added statics, north and south) | Rider combinations are forms |
| Enamorus | Johto Route 39 (added static) | Therian is a form |

### Generation 9

| Species | Authored location / method | Additional detail |
|---|---|---|
| Wo-Chien | Mt. Pyre exterior (added static) |  |
| Chien-Pao | Granite Cave B2F (added static) |  |
| Ting-Lu | Granite Cave B2F (added static) |  |
| Chi-Yu | Fiery Path (added static) |  |
| Koraidon | Hoenn Route 111 (added static) |  |
| Miraidon | Hoenn Route 110 (added static) |  |
| Okidogi | Hoenn Route 111 (added static) |  |
| Munkidori | Hoenn Route 120 (added static) |  |
| Fezandipiti | Mt. Pyre exterior (added static) |  |
| Ogerpon | Hoenn Route 120 (scripted static, Teal Mask form) | Masks are forms |
| Terapagos | Mossdeep Space Center 2F (added static) | Terastal/Stellar are forms |
| Pecharunt | Mt. Pyre exterior (added static) |  |

## Additional validation boundaries and existing evidence

- **Normal statics were found for each listed new Legendary/Mythical source in the 89-object source validator**. The inventory also includes a few Paradox static placements, deliberately excluded here according to the user's classification. The underlying source checks inspect coordinates, collision neighbors, encounter script tokens and selected path anchors; they are **not actual gameplay catches**.
- **Roamers:** Entei and Raikou are released after Burned Tower B1F; Johto Route 43 and Route 46 provide explicit static fallbacks. Hoenn uses a choice-dependent Latias or Latios roamer; both also have separate Route 118/120 statics and Southern Island scripts. Do not conflate roaming battles with *wild grass habitat authoring*.
- **Special events:** Ilex Forest Celebi; Whirl Islands Lugia; Tin Tower Ho-Oh; Navel Rock Lugia/Ho-Oh; Faraway Island Mew; Birth Island Deoxys; Southern Island Latias/Latios; Embedded Tower Groudon/Kyogre/Rayquaza; original Hoenn Regis, weather caverns and Sky Pillar all have inherited source scripts. Some events are Champion-, item-, ticket- or story-gated. Exact accessibility is not established by this location listing.
- **Existing tests:** The full optimized release build and source checks passed at gameplay code commit `10bf90d` ([build](https://github.com/DevElectrum/grand-anthology/actions/runs/40066826520)). Nine broader mGBA suites passed, but both late synthetic navigation fixtures were unable to enter their target maps ([release smoke](https://github.com/DevElectrum/grand-anthology/actions/runs/40066826492)). Therefore **Diancie, daycare breeding and many actual static battles remain gameplay-unverified**.
- **Scope limitation:** This is a source inventory, not a verified all-save, all-script conditional-flow proof. No effort was made here to determine each static's earliest possible badge, flag, or time condition, or whether a duplicate can be caught twice. Such work should only proceed after a discussion with the user.
