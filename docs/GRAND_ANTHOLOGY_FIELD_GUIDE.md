# Grand Anthology Field Guide
**For human players** · 25 October 2026 · [Back to README](../README.md) · [Machine-readable source atlas](../data/catalog/world_acquisition.jsonl)
> **Unfinished game, source-indexed guide.** A documented location is a candidate in source code, not proof the map is reachable or the encounter works. Randomization, postgame gates, catchability and save behavior need separate testing. Older design ledgers can be obsolete.
This reference includes all **1,025 National Pokédex species**, all **874 explicitly numbered item constants**, and **92 Mega Stones**. Use your browser's Find function to search any name. The tables give representative locations; Astra's catalog retains the full indexed source list.
## Your journey
| Region | Progression | Major special opportunities |
| --- | --- | --- |
| **Kanto** | Original gyms and League, one persistent party | Regional rare habitats, Pokémon Mansion's Mew challenge, and Champion-gated areas like Cerulean Cave |
| **Johto** | Continue with the same trainer and collection, then eight gyms and the League | Zacian and Zamazenta stand together on Route 27 after all eight badges, **before** Johto Champion |
| **Hoenn** | Continue the shared adventure and defeat Hoenn Champion | Regular Terapagos on Route 114; permanent Complete Zygarde and permanent Stellar Terapagos available **after** Hoenn Champion |

The story is meant to run **Kanto → Johto → Hoenn**, without forced level resets or fresh teams. The source is still experimental; regional continuity has representative tests, not a full accepted playthrough.
## Major special encounters
| Encounter | Where | How / notes |
| --- | --- | --- |
| **Zacian + Zamazenta** | Johto Route 27 | Two standing sprites; after eight Johto badges, interact with either and defeat both Lv50 Crowned wolves. Receive both holding Rusted Sword/Shield. No Balls; runtime test pending. |
| **Mew** | Kanto Pokémon Mansion 1F | Three disguise fights followed by catchable Lv35 Mew. Player-reported capture and revisit success. |
| **Zygarde 10%** | Hoenn Petalburg Woods | Lv10 Aura Break. |
| **Zygarde 50%** | Hoenn Meteor Falls | Lv44 Power Construct. |
| **Zygarde Complete** | Hoenn Route 101 | Permanent Lv70 Complete; Hoenn Champion required. |
| **Regular Terapagos** | Hoenn Route 114 | Lv31, separate from Stellar. |
| **Stellar Terapagos** | Hoenn Cave of Origin B1F | Permanent Lv75 Stellar; Hoenn Champion required. |
| **Hoopa** | Hoenn Route 111 Ring Chamber | Available after fourth Hoenn badge. |
| **Koraidon** | Underwater Route 128 Ancient Grotto | After seventh Hoenn badge and Space Center Magma victory. |
| **Miraidon** | New Mauville Generator Annex | After seventh badge, Space Center victory and generator shutdown. |
| **Kyurem** | Johto Ice Path B1F | Requires caught Reshiram and Zekrom; DNA Splicers tied to encounter. |
| **Eternatus** | Johto Route 44 (current source) | Lv50 static. Proposed Victory Road move is NOT implemented. |

## Find any Pokémon by National number
Each collapsed generation opens into a searchable table. An **evolution candidate** does not certify that its exact evolution requirement was tested.

<details>
<summary><strong>Generation I · National #1–151</strong></summary>

| # | Pokémon | Source-candidate locations / method |
| ---: | --- | --- |
| 1 | **BULBASAUR** | Grass at ROUTE 1; Grass at ROUTE 26 |
| 2 | **IVYSAUR** | Evolve BULBASAUR |
| 3 | **VENUSAUR** | Grass at MT SILVER OUTSIDE; Evolve IVYSAUR |
| 4 | **CHARMANDER** | Grass at ROUTE 1; Grass at ROUTE 26 |
| 5 | **CHARMELEON** | Evolve CHARMANDER |
| 6 | **CHARIZARD** | Grass at MT SILVER OUTSIDE; Evolve CHARMELEON |
| 7 | **SQUIRTLE** | Grass at ROUTE 1 |
| 8 | **WARTORTLE** | Evolve SQUIRTLE |
| 9 | **BLASTOISE** | Grass at MT SILVER OUTSIDE; Evolve WARTORTLE |
| 10 | **CATERPIE** | Grass at ILEX FOREST; Grass at NATIONAL PARK BUG CONTEST; +8 other sources in machine catalog |
| 11 | **METAPOD** | Grass at ILEX FOREST; Grass at NATIONAL PARK BUG CONTEST; Evolve CATERPIE; +4 other sources in machine catalog |
| 12 | **BUTTERFREE** | Grass at NATIONAL PARK BUG CONTEST; Rock Smash at ILEX FOREST; Evolve METAPOD |
| 13 | **WEEDLE** | Grass at ILEX FOREST; Grass at NATIONAL PARK BUG CONTEST; +8 other sources in machine catalog |
| 14 | **KAKUNA** | Grass at ILEX FOREST; Grass at NATIONAL PARK BUG CONTEST; Evolve WEEDLE; +4 other sources in machine catalog |
| 15 | **BEEDRILL** | Grass at NATIONAL PARK BUG CONTEST; Rock Smash at ILEX FOREST; Evolve KAKUNA |
| 16 | **PIDGEY** | Grass at FIVE ISLAND MEADOW; Grass at ROUTE 1; +20 other sources in machine catalog |
| 17 | **PIDGEOTTO** | Grass at FIVE ISLAND MEADOW; Grass at ROUTE 13; Evolve PIDGEY; +9 other sources in machine catalog |
| 18 | **PIDGEOT** | Evolve PIDGEOTTO |
| 19 | **RATTATA** | Grass at BURNED TOWER 1F; Grass at POKEMON MANSION 1F; +29 other sources in machine catalog |
| 20 | **RATICATE** | Grass at BURNED TOWER 1F; Grass at MT MORTAR 2F; Evolve RATTATA; +15 other sources in machine catalog |
| 21 | **SPEAROW** | Grass at MT EMBER EXTERIOR; Grass at ONE ISLAND KINDLE ROAD; +17 other sources in machine catalog |
| 22 | **FEAROW** | Grass at MT EMBER EXTERIOR; Grass at ONE ISLAND KINDLE ROAD; Evolve SPEAROW; +13 other sources in machine catalog |
| 23 | **EKANS** | Grass at ROUTE 10; Grass at ROUTE 11; +11 other sources in machine catalog |
| 24 | **ARBOK** | Grass at ROUTE 23; Grass at ROUTE 26; Evolve EKANS; +7 other sources in machine catalog |
| 25 | **PIKACHU** | Grass at POWER PLANT; Grass at SAFARI ZONE SOUTH; Evolve PICHU; +3 other sources in machine catalog |
| 26 | **RAICHU** | Supplemental grass at ROUTE 15 [RAICHU ALOLA]; Evolve PIKACHU |
| 27 | **SANDSHREW** | Grass at MIRAGE TOWER 1F; Grass at MIRAGE TOWER 2F; +12 other sources in machine catalog |
| 28 | **SANDSLASH** | Grass at JOHTO VICTORY ROAD 1F; Grass at JOHTO VICTORY ROAD B 1F; Evolve SANDSHREW; +6 other sources in machine catalog |
| 29 | **NIDORAN F** | Grass at NATIONAL PARK NORMAL; Grass at ROUTE 3; +4 other sources in machine catalog |
| 30 | **NIDORINA** | Grass at SAFARI ZONE CENTER; Grass at SAFARI ZONE EAST; Evolve NIDORAN F; +3 other sources in machine catalog |
| 31 | **NIDOQUEEN** | Evolve NIDORINA |
| 32 | **NIDORAN M** | Grass at NATIONAL PARK NORMAL; Grass at ROUTE 3; +4 other sources in machine catalog |
| 33 | **NIDORINO** | Grass at SAFARI ZONE CENTER; Grass at SAFARI ZONE EAST; Evolve NIDORAN M; +3 other sources in machine catalog |
| 34 | **NIDOKING** | Evolve NIDORINO |
| 35 | **CLEFAIRY** | Grass at MT MOON 1F; Grass at MT MOON B 2F; Evolve CLEFFA |
| 36 | **CLEFABLE** | Evolve CLEFAIRY |
| 37 | **VULPIX** | Grass at MT PYRE EXTERIOR; Grass at POKEMON MANSION 1F; +8 other sources in machine catalog |
| 38 | **NINETALES** | Evolve VULPIX |
| 39 | **JIGGLYPUFF** | Grass at ROUTE 115; Grass at ROUTE 3; Evolve IGGLYBUFF; +1 other sources in machine catalog |
| 40 | **WIGGLYTUFF** | Evolve JIGGLYPUFF |
| 41 | **ZUBAT** | Grass at ALTERING CAVE; Grass at BURNED TOWER B 1F; +71 other sources in machine catalog |
| 42 | **GOLBAT** | Grass at CAVE OF ORIGIN 1F; Grass at CAVE OF ORIGIN ENTRANCE; Evolve ZUBAT; +88 other sources in machine catalog |
| 43 | **ODDISH** | Grass at ILEX FOREST; Grass at ROUTE 110; +22 other sources in machine catalog |
| 44 | **GLOOM** | Grass at ROUTE 12; Grass at ROUTE 121; Evolve ODDISH; +14 other sources in machine catalog |
| 45 | **VILEPLUME** | Evolve GLOOM |
| 46 | **PARAS** | Grass at ILEX FOREST; Grass at MT MOON 1F; +5 other sources in machine catalog |
| 47 | **PARASECT** | Grass at CERULEAN CAVE 1F; Grass at CERULEAN CAVE 2F; Evolve PARAS; +6 other sources in machine catalog |
| 48 | **VENONAT** | Grass at NATIONAL PARK BUG CONTEST; Grass at ROUTE 12; +9 other sources in machine catalog |
| 49 | **VENOMOTH** | Grass at SAFARI ZONE NORTH FRLG; Grass at SAFARI ZONE WEST; Evolve VENONAT; +1 other sources in machine catalog |
| 50 | **DIGLETT** | Grass at DIGLETTS CAVE B 1F; Supplemental grass at DIGLETTS CAVE B 1F [DIGLETT ALOLA] |
| 51 | **DUGTRIO** | Grass at DIGLETTS CAVE B 1F; Evolve DIGLETT |
| 52 | **MEOWTH** | Grass at FIVE ISLAND MEADOW; Grass at ONE ISLAND KINDLE ROAD; +16 other sources in machine catalog |
| 53 | **PERSIAN** | Grass at FIVE ISLAND MEADOW; Grass at ONE ISLAND KINDLE ROAD; Evolve MEOWTH; +7 other sources in machine catalog |
| 54 | **PSYDUCK** | Fishing at CERULEAN CAVE 1F; Fishing at CERULEAN CAVE B 1F; +87 other sources in machine catalog |
| 55 | **GOLDUCK** | Fishing at JOHTO VICTORY ROAD B 2F; Grass at MT SILVER 2F; Evolve PSYDUCK; +21 other sources in machine catalog |
| 56 | **MANKEY** | Grass at ROCK TUNNEL 1F; Grass at ROCK TUNNEL B 1F; +6 other sources in machine catalog |
| 57 | **PRIMEAPE** | Grass at CERULEAN CAVE 1F; Grass at ROUTE 23; Evolve MANKEY; +1 other sources in machine catalog |
| 58 | **GROWLITHE** | Grass at POKEMON MANSION 1F; Grass at POKEMON MANSION 2F; +9 other sources in machine catalog |
| 59 | **ARCANINE** | Evolve GROWLITHE |
| 60 | **POLIWAG** | Fishing at BLACKTHORN CITY; Fishing at CERULEAN CAVE 1F; +43 other sources in machine catalog |
| 61 | **POLIWHIRL** | Fishing at BLACKTHORN CITY; Fishing at CERULEAN CAVE 1F; Evolve POLIWAG; +35 other sources in machine catalog |
| 62 | **POLIWRATH** | Evolve POLIWHIRL |
| 63 | **ABRA** | Grass at GRANITE CAVE 1F; Grass at GRANITE CAVE B 1F; +7 other sources in machine catalog |
| 64 | **KADABRA** | Grass at CERULEAN CAVE 2F; Grass at CERULEAN CAVE B 1F; Evolve ABRA |
| 65 | **ALAKAZAM** | Evolve KADABRA |
| 66 | **MACHOP** | Grass at CLIFF EDGE CAVE; Grass at FIERY PATH; +17 other sources in machine catalog |
| 67 | **MACHOKE** | Grass at CERULEAN CAVE 1F; Grass at CERULEAN CAVE 2F; Evolve MACHOP; +16 other sources in machine catalog |
| 68 | **MACHAMP** | Evolve MACHOKE |
| 69 | **BELLSPROUT** | Grass at ROUTE 12; Grass at ROUTE 13; +15 other sources in machine catalog |
| 70 | **WEEPINBELL** | Grass at ROUTE 12; Grass at ROUTE 13; Evolve BELLSPROUT; +8 other sources in machine catalog |
| 71 | **VICTREEBEL** | Evolve WEEPINBELL |
| 72 | **TENTACOOL** | Gift at Cianwood Pokecenter Lv25; Fishing at ABANDONED SHIP HIDDEN FLOOR CORRIDORS; +125 other sources in machine catalog |
| 73 | **TENTACRUEL** | Scripted encounter at Route 41 Lv100; Fishing at ABANDONED SHIP HIDDEN FLOOR CORRIDORS; Evolve TENTACOOL; +40 other sources in machine catalog |
| 74 | **GEODUDE** | Grass at CLIFF EDGE CAVE; Grass at DARK CAVE NORTH SIDE; +57 other sources in machine catalog |
| 75 | **GRAVELER** | Grass at CLIFF EDGE CAVE; Grass at DARK CAVE NORTH SIDE; Evolve GEODUDE; +31 other sources in machine catalog |
| 76 | **GOLEM** | Grass at MT SILVER 1F WATERFALL ROOM; Grass at MT SILVER 2F; Evolve GRAVELER; +3 other sources in machine catalog |
| 77 | **PONYTA** | Grass at MT EMBER EXTERIOR; Grass at ONE ISLAND KINDLE ROAD; +6 other sources in machine catalog |
| 78 | **RAPIDASH** | Grass at MT EMBER EXTERIOR; Grass at MT SILVER MOUNTAIN SIDE; Evolve PONYTA; +6 other sources in machine catalog |
| 79 | **SLOWPOKE** | Fishing at CERULEAN CAVE 1F; Fishing at CERULEAN CAVE B 1F; +90 other sources in machine catalog |
| 80 | **SLOWBRO** | Fishing at CINNABAR ISLAND; Fishing at JOHTO VICTORY ROAD B 2F; Evolve SLOWPOKE; +17 other sources in machine catalog |
| 81 | **MAGNEMITE** | Grass at NEW MAUVILLE ENTRANCE; Grass at NEW MAUVILLE INSIDE; +4 other sources in machine catalog |
| 82 | **MAGNETON** | Grass at CERULEAN CAVE 1F; Grass at CERULEAN CAVE 2F; Evolve MAGNEMITE; +4 other sources in machine catalog |
| 83 | **FARFETCHD** | Grass at ROUTE 35 [FARFETCHD GALAR]; Grass at ROUTE 43; +6 other sources in machine catalog |
| 84 | **DODUO** | Grass at ROUTE 16; Grass at ROUTE 17; +9 other sources in machine catalog |
| 85 | **DODRIO** | Grass at ROUTE 26; Grass at ROUTE 27; Evolve DODUO; +2 other sources in machine catalog |
| 86 | **SEEL** | Grass at FOUR ISLAND ICEFALL CAVE 1F; Grass at FOUR ISLAND ICEFALL CAVE B 1F; +16 other sources in machine catalog |
| 87 | **DEWGONG** | Grass at FOUR ISLAND ICEFALL CAVE BACK; Grass at FOUR ISLAND ICEFALL CAVE ENTRANCE; Evolve SEEL; +8 other sources in machine catalog |
| 88 | **GRIMER** | Fishing at CELADON CITY; Grass at FIERY PATH; +6 other sources in machine catalog |
| 89 | **MUK** | Grass at POKEMON MANSION 1F; Grass at POKEMON MANSION 2F; Evolve GRIMER; +2 other sources in machine catalog |
| 90 | **SHELLDER** | Fishing at CIANWOOD CITY; Fishing at CINNABAR ISLAND; +12 other sources in machine catalog |
| 91 | **CLOYSTER** | Evolve SHELLDER |
| 92 | **GASTLY** | Grass at FIVE ISLAND LOST CAVE ROOM 1; Grass at FIVE ISLAND LOST CAVE ROOM 10; +26 other sources in machine catalog |
| 93 | **HAUNTER** | Grass at FIVE ISLAND LOST CAVE ROOM 1; Grass at FIVE ISLAND LOST CAVE ROOM 10; Evolve GASTLY; +18 other sources in machine catalog |
| 94 | **GENGAR** | Evolve HAUNTER |
| 95 | **ONIX** | Grass at CLIFF EDGE CAVE; Grass at JOHTO VICTORY ROAD 1F; +17 other sources in machine catalog |
| 96 | **DROWZEE** | Grass at ROUTE 11; Grass at THREE ISLAND BERRY FOREST; +2 other sources in machine catalog |
| 97 | **HYPNO** | Scripted encounter at Three Island Berry Forest Frlg Lv30; Grass at THREE ISLAND BERRY FOREST; Evolve DROWZEE |
| 98 | **KRABBY** | Fishing at CERULEAN CITY; Fishing at CHERRYGROVE CITY; +50 other sources in machine catalog |
| 99 | **KINGLER** | Fishing at CHERRYGROVE CITY; Fishing at FIVE ISLAND; Evolve KRABBY; +33 other sources in machine catalog |
| 100 | **VOLTORB** | Scripted encounter at New Mauville Inside Lv25; Grass at NEW MAUVILLE ENTRANCE; +4 other sources in machine catalog |
| 101 | **ELECTRODE** | Scripted encounter at Aqua Hideout B 1F Lv30; Scripted encounter at Mahogany Hideout B 2F Lv30; Evolve VOLTORB; +5 other sources in machine catalog |
| 102 | **EXEGGCUTE** | Grass at SAFARI ZONE CENTER; Grass at SAFARI ZONE EAST; +24 other sources in machine catalog |
| 103 | **EXEGGUTOR** | Supplemental grass at ROUTE 16 [EXEGGUTOR ALOLA]; Evolve EXEGGCUTE |
| 104 | **CUBONE** | Grass at MT MORTAR 1F NORTH; Grass at MT MORTAR 1F SOUTH; +10 other sources in machine catalog |
| 105 | **MAROWAK** | Scripted encounter at Pokemon Tower 6F Frlg Lv30; Grass at SEVEN ISLAND SEVAULT CANYON; Evolve CUBONE; +4 other sources in machine catalog |
| 106 | **HITMONLEE** | Supplemental grass at ROUTE 23; Evolve TYROGUE |
| 107 | **HITMONCHAN** | Supplemental grass at ROUTE 15; Evolve TYROGUE |
| 108 | **LICKITUNG** | Grass at ROUTE 44; Supplemental grass at ROUTE 13 |
| 109 | **KOFFING** | Grass at BURNED TOWER 1F; Grass at BURNED TOWER B 1F; +6 other sources in machine catalog |
| 110 | **WEEZING** | Grass at POKEMON MANSION 1F; Grass at POKEMON MANSION 2F; Evolve KOFFING; +3 other sources in machine catalog |
| 111 | **RHYHORN** | Grass at JOHTO VICTORY ROAD 1F; Grass at JOHTO VICTORY ROAD B 1F; +9 other sources in machine catalog |
| 112 | **RHYDON** | Evolve RHYHORN |
| 113 | **CHANSEY** | Grass at ROUTE 47; Grass at SAFARI ZONE CENTER; Evolve HAPPINY; +1 other sources in machine catalog |
| 114 | **TANGELA** | Grass at ONE ISLAND TREASURE BEACH; Grass at ROUTE 21 NORTH; +3 other sources in machine catalog |
| 115 | **KANGASKHAN** | Grass at SAFARI ZONE EAST; Grass at SAFARI ZONE LOW LEFT; +3 other sources in machine catalog |
| 116 | **HORSEA** | Fishing at CERULEAN CITY; Fishing at CINNABAR ISLAND; +49 other sources in machine catalog |
| 117 | **SEADRA** | Fishing at CINNABAR ISLAND; Fishing at DRAGONS DEN CAVERN; Evolve HORSEA; +27 other sources in machine catalog |
| 118 | **GOLDEEN** | Fishing at CERULEAN CAVE 1F; Fishing at CERULEAN CAVE B 1F; +56 other sources in machine catalog |
| 119 | **SEAKING** | Fishing at DARK CAVE NORTH SIDE; Fishing at DARK CAVE SOUTH SIDE; Evolve GOLDEEN; +30 other sources in machine catalog |
| 120 | **STARYU** | Fishing at CINNABAR ISLAND; Fishing at FIVE ISLAND; +11 other sources in machine catalog |
| 121 | **STARMIE** | Evolve STARYU |
| 122 | **MR MIME** | Grass at SAFARI ZONE TOP MID; Supplemental grass at ICE PATH B 2F [MR MIME GALAR]; Evolve MIME JR |
| 123 | **SCYTHER** | Grass at NATIONAL PARK BUG CONTEST; Grass at NATIONAL PARK NORMAL; +6 other sources in machine catalog |
| 124 | **JYNX** | Grass at ICE PATH B 1F; Grass at ICE PATH B 2F; Evolve SMOOCHUM; +3 other sources in machine catalog |
| 125 | **ELECTABUZZ** | Grass at MT SILVER 1F ITEM ROOM; Grass at MT SILVER 1F MOLTRES ROOM; Evolve ELEKID; +4 other sources in machine catalog |
| 126 | **MAGMAR** | Grass at BURNED TOWER B 1F; Grass at MT EMBER EXTERIOR; Evolve MAGBY; +4 other sources in machine catalog |
| 127 | **PINSIR** | Grass at NATIONAL PARK BUG CONTEST; Grass at SAFARI ZONE CENTER; +3 other sources in machine catalog |
| 128 | **TAUROS** | Grass at ROUTE 38; Grass at ROUTE 39; +8 other sources in machine catalog |
| 129 | **MAGIKARP** | Gift at Route 4 Pokemon Center 1F Frlg Lv5; Fishing at ABANDONED SHIP HIDDEN FLOOR CORRIDORS; +165 other sources in machine catalog |
| 130 | **GYARADOS** | Fishing at CERULEAN CAVE 1F; Fishing at CERULEAN CAVE B 1F; Evolve MAGIKARP; +47 other sources in machine catalog |
| 131 | **LAPRAS** | Scripted encounter at Union Cave B 2F Lv20; Surf at FOUR ISLAND ICEFALL CAVE BACK; +3 other sources in machine catalog |
| 132 | **DITTO** | Grass at CERULEAN CAVE 1F; Grass at CERULEAN CAVE 2F; +8 other sources in machine catalog |
| 133 | **EEVEE** | Gift at Celadon City Condominiums Roof Room Frlg Lv25; Supplemental grass at ROUTE 115; +2 other sources in machine catalog |
| 134 | **VAPOREON** | Evolve EEVEE |
| 135 | **JOLTEON** | Evolve EEVEE |
| 136 | **FLAREON** | Evolve EEVEE |
| 137 | **PORYGON** | Supplemental grass at ROUTE 17; Supplemental grass at ROUTE 39 |
| 138 | **OMANYTE** | Gift at Cinnabar Island Pokemon Lab Experiment Room Frlg Lv5; Grass at WHIRL ISLANDS DESCENT |
| 139 | **OMASTAR** | Evolve OMANYTE |
| 140 | **KABUTO** | Gift at Cinnabar Island Pokemon Lab Experiment Room Frlg Lv5; Grass at WHIRL ISLANDS DESCENT |
| 141 | **KABUTOPS** | Evolve KABUTO |
| 142 | **AERODACTYL** | Fossil restoration at Cinnabar Island Pokemon Lab Experiment Room Frlg; Fossil restoration at Ruins Of Alph Lab |
| 143 | **SNORLAX** | Scripted encounter at Route 12 Frlg Lv30; Scripted encounter at Route 16 Frlg Lv30; Evolve MUNCHLAX |
| 144 | **ARTICUNO** | Scripted encounter at Route 40 Lv45 [ARTICUNO GALAR]; Scripted encounter at Seafoam Islands B 4F Frlg Lv50 |
| 145 | **ZAPDOS** | Scripted encounter at Power Plant Frlg Lv50; Scripted encounter at Route 38 Lv45 [ZAPDOS GALAR] |
| 146 | **MOLTRES** | Scripted encounter at Burned Tower 1F Lv45 [MOLTRES GALAR]; Scripted encounter at Mt Ember Summit Frlg Lv50; +1 other sources in machine catalog |
| 147 | **DRATINI** | Gift at Dragons Den Shrine Lv15; Rare grass at SKY PILLAR 1F; +10 other sources in machine catalog |
| 148 | **DRAGONAIR** | Fishing at DRAGONS DEN CAVERN; Fishing at ROUTE 45; Evolve DRATINI; +5 other sources in machine catalog |
| 149 | **DRAGONITE** | Evolve DRAGONAIR |
| 150 | **MEWTWO** | Scripted encounter at Cerulean Cave B 1F Frlg Lv70 |
| 151 | **MEW** | Catch after three battles at Pokemon Mansion 1F Frlg Lv35; Scripted encounter at Faraway Island Interior Lv30 |

</details>

<details>
<summary><strong>Generation II · National #152–251</strong></summary>

| # | Pokémon | Source-candidate locations / method |
| ---: | --- | --- |
| 152 | **CHIKORITA** | Grass at ROUTE 27; Grass at ROUTE 29 |
| 153 | **BAYLEEF** | Evolve CHIKORITA |
| 154 | **MEGANIUM** | Evolve BAYLEEF |
| 155 | **CYNDAQUIL** | Gift at Littleroot Town Professor Birchs Lab Lv5; Grass at ROUTE 27; +1 other sources in machine catalog |
| 156 | **QUILAVA** | Evolve CYNDAQUIL |
| 157 | **TYPHLOSION** | Supplemental grass at BURNED TOWER B 1F [TYPHLOSION HISUI]; Evolve QUILAVA |
| 158 | **TOTODILE** | Gift at Littleroot Town Professor Birchs Lab Lv5; Grass at ROUTE 29 |
| 159 | **CROCONAW** | Evolve TOTODILE |
| 160 | **FERALIGATR** | Evolve CROCONAW |
| 161 | **SENTRET** | Grass at FIVE ISLAND MEADOW; Grass at ROUTE 29; +2 other sources in machine catalog |
| 162 | **FURRET** | Grass at ROUTE 43; Evolve SENTRET |
| 163 | **HOOTHOOT** | Grass at ROUTE 29; Grass at SAFARI ZONE NORTHEAST; +1 other sources in machine catalog |
| 164 | **NOCTOWL** | Evolve HOOTHOOT |
| 165 | **LEDYBA** | Grass at ROUTE 30; Grass at ROUTE 31; +18 other sources in machine catalog |
| 166 | **LEDIAN** | Grass at ROUTE 37; Evolve LEDYBA |
| 167 | **SPINARAK** | Grass at SAFARI ZONE SOUTHEAST; Grass at SIX ISLAND PATTERN BUSH; +17 other sources in machine catalog |
| 168 | **ARIADOS** | Evolve SPINARAK |
| 169 | **CROBAT** | Grass at MT SILVER 3F; Evolve GOLBAT |
| 170 | **CHINCHOU** | Fishing at CIANWOOD CITY; Fishing at NEW BARK TOWN; +6 other sources in machine catalog |
| 171 | **LANTURN** | Fishing at CIANWOOD CITY; Fishing at NEW BARK TOWN; Evolve CHINCHOU; +4 other sources in machine catalog |
| 172 | **PICHU** | Grass at ROUTE 31 |
| 173 | **CLEFFA** | Supplemental grass at ROUTE 36 |
| 174 | **IGGLYBUFF** | Supplemental grass at ROUTE 35 |
| 175 | **TOGEPI** | Supplemental grass at SPROUT TOWER 3F |
| 176 | **TOGETIC** | Evolve TOGEPI |
| 177 | **NATU** | Grass at RUINS OF ALPH OUTSIDE; Grass at SAFARI ZONE NORTH; +6 other sources in machine catalog |
| 178 | **XATU** | Grass at SAFARI ZONE NORTH; Grass at WHIRL ISLANDS DESCENT; Evolve NATU |
| 179 | **MAREEP** | Grass at ALTERING CAVE; Grass at ROUTE 31; +3 other sources in machine catalog |
| 180 | **FLAAFFY** | Grass at ROUTE 43; Evolve MAREEP |
| 181 | **AMPHAROS** | Evolve FLAAFFY |
| 182 | **BELLOSSOM** | Evolve GLOOM |
| 183 | **MARILL** | Grass at ROUTE 104; Grass at ROUTE 112; Evolve AZURILL; +18 other sources in machine catalog |
| 184 | **AZUMARILL** | Evolve MARILL |
| 185 | **SUDOWOODO** | Scripted encounter at Battle Frontier Outside East Lv40; Scripted encounter at Route 36 Lv20; Evolve BONSLY |
| 186 | **POLITOED** | Evolve POLIWHIRL |
| 187 | **HOPPIP** | Grass at FIVE ISLAND MEADOW; Grass at FIVE ISLAND MEMORIAL PILLAR; +6 other sources in machine catalog |
| 188 | **SKIPLOOM** | Grass at ROUTE 48; Evolve HOPPIP |
| 189 | **JUMPLUFF** | Evolve SKIPLOOM |
| 190 | **AIPOM** | Grass at ALTERING CAVE; Grass at SAFARI ZONE NORTHEAST; +17 other sources in machine catalog |
| 191 | **SUNKERN** | Grass at NATIONAL PARK NORMAL; Grass at SAFARI ZONE NORTHEAST; +1 other sources in machine catalog |
| 192 | **SUNFLORA** | Evolve SUNKERN |
| 193 | **YANMA** | Grass at ROUTE 35; Grass at SAFARI ZONE TOP LEFT; +2 other sources in machine catalog |
| 194 | **WOOPER** | Grass at CLIFF EDGE CAVE; Grass at ROUTE 32; +12 other sources in machine catalog |
| 195 | **QUAGSIRE** | Fishing at SAFARI ZONE LOW RIGHT; Grass at CLIFF EDGE CAVE; Evolve WOOPER; +12 other sources in machine catalog |
| 196 | **ESPEON** | Evolve EEVEE |
| 197 | **UMBREON** | Evolve EEVEE |
| 198 | **MURKROW** | Grass at FIVE ISLAND LOST CAVE ROOM 1; Grass at FIVE ISLAND LOST CAVE ROOM 10; +14 other sources in machine catalog |
| 199 | **SLOWKING** | Supplemental grass at WHIRL ISLANDS 1F [SLOWKING GALAR]; Evolve SLOWPOKE |
| 200 | **MISDREAVUS** | Rare grass at MT PYRE 6F; Grass at BURNED TOWER B 1F; +17 other sources in machine catalog |
| 201 | **UNOWN** | Grass at RUINS OF ALPH B 1F; Grass at RUINS OF ALPH OUTSIDE; +7 other sources in machine catalog |
| 202 | **WOBBUFFET** | Grass at CERULEAN CAVE 1F; Grass at CERULEAN CAVE 2F; Evolve WYNAUT; +5 other sources in machine catalog |
| 203 | **GIRAFARIG** | Grass at ROUTE 43; Grass at ROUTE 48; +2 other sources in machine catalog |
| 204 | **PINECO** | Grass at ALTERING CAVE; Grass at SAFARI ZONE NORTHEAST; +32 other sources in machine catalog |
| 205 | **FORRETRESS** | Rock Smash at MT SILVER MOUNTAIN SIDE; Rock Smash at MT SILVER OUTSIDE; Evolve PINECO; +1 other sources in machine catalog |
| 206 | **DUNSPARCE** | Grass at DARK CAVE SOUTH SIDE; Grass at THREE ISLAND PORT; +1 other sources in machine catalog |
| 207 | **GLIGAR** | Grass at ROUTE 42; Grass at ROUTE 45; +2 other sources in machine catalog |
| 208 | **STEELIX** | Grass at CLIFF EDGE CAVE; Evolve ONIX |
| 209 | **SNUBBULL** | Grass at ROUTE 34; Grass at ROUTE 35; +1 other sources in machine catalog |
| 210 | **GRANBULL** | Evolve SNUBBULL |
| 211 | **QWILFISH** | Fishing at FIVE ISLAND MEADOW; Fishing at FIVE ISLAND MEMORIAL PILLAR; +10 other sources in machine catalog |
| 212 | **SCIZOR** | Evolve SCYTHER |
| 213 | **SHUCKLE** | Grass at ALTERING CAVE; Grass at SIX ISLAND ALTERING CAVE; +2 other sources in machine catalog |
| 214 | **HERACROSS** | Grass at SAFARI ZONE NORTH; Grass at SIX ISLAND PATTERN BUSH; +13 other sources in machine catalog |
| 215 | **SNEASEL** | Grass at FOUR ISLAND ICEFALL CAVE 1F; Grass at FOUR ISLAND ICEFALL CAVE B 1F; +8 other sources in machine catalog |
| 216 | **TEDDIURSA** | Grass at ALTERING CAVE; Grass at DARK CAVE SOUTH SIDE; +2 other sources in machine catalog |
| 217 | **URSARING** | Grass at DARK CAVE NORTH SIDE; Grass at JOHTO VICTORY ROAD 1F; Evolve TEDDIURSA; +9 other sources in machine catalog |
| 218 | **SLUGMA** | Grass at BURNED TOWER 1F; Grass at FIERY PATH; +7 other sources in machine catalog |
| 219 | **MAGCARGO** | Rock Smash at MT EMBER RUBY PATH B 3F; Evolve SLUGMA |
| 220 | **SWINUB** | Grass at FOUR ISLAND ICEFALL CAVE 1F; Grass at FOUR ISLAND ICEFALL CAVE B 1F; +6 other sources in machine catalog |
| 221 | **PILOSWINE** | Grass at MT SILVER SNOW; Evolve SWINUB |
| 222 | **CORSOLA** | Fishing at CHERRYGROVE CITY; Fishing at EVER GRANDE CITY; +9 other sources in machine catalog |
| 223 | **REMORAID** | Fishing at FIVE ISLAND MEADOW; Fishing at FIVE ISLAND MEMORIAL PILLAR; +14 other sources in machine catalog |
| 224 | **OCTILLERY** | Fishing at SAFARI ZONE SOUTHEAST; Evolve REMORAID |
| 225 | **DELIBIRD** | Grass at FOUR ISLAND ICEFALL CAVE 1F; Grass at FOUR ISLAND ICEFALL CAVE B 1F; +1 other sources in machine catalog |
| 226 | **MANTINE** | Surf at ROUTE 41; Surf at SEVEN ISLAND TANOBY RUINS; Evolve MANTYKE; +3 other sources in machine catalog |
| 227 | **SKARMORY** | Grass at MT SILVER MOUNTAIN SIDE; Grass at MT SILVER OUTSIDE; +7 other sources in machine catalog |
| 228 | **HOUNDOUR** | Grass at ALTERING CAVE; Grass at BURNED TOWER 1F; +2 other sources in machine catalog |
| 229 | **HOUNDOOM** | Grass at MT SILVER MOUNTAIN SIDE; Grass at MT SILVER SNOW; Evolve HOUNDOUR |
| 230 | **KINGDRA** | Evolve SEADRA |
| 231 | **PHANPY** | Grass at ROUTE 46; Grass at SAFARI ZONE NORTH; +2 other sources in machine catalog |
| 232 | **DONPHAN** | Grass at JOHTO VICTORY ROAD 1F; Grass at JOHTO VICTORY ROAD B 1F; Evolve PHANPY; +3 other sources in machine catalog |
| 233 | **PORYGON 2** | Evolve PORYGON |
| 234 | **STANTLER** | Grass at ALTERING CAVE; Grass at ROUTE 37; +2 other sources in machine catalog |
| 235 | **SMEARGLE** | Grass at ALTERING CAVE; Grass at ARTISAN CAVE 1F; +3 other sources in machine catalog |
| 236 | **TYROGUE** | Gift at Mt Mortar B 1F Lv25; Supplemental grass at MT MORTAR 1F NORTH |
| 237 | **HITMONTOP** | Evolve TYROGUE |
| 238 | **SMOOCHUM** | Supplemental grass at ICE PATH B 1F |
| 239 | **ELEKID** | Supplemental grass at ROUTE 43 |
| 240 | **MAGBY** | Supplemental grass at ROUTE 39 |
| 241 | **MILTANK** | Grass at ROUTE 38; Grass at ROUTE 39; +2 other sources in machine catalog |
| 242 | **BLISSEY** | Evolve CHANSEY |
| 243 | **RAIKOU** | Scripted encounter at Route 38 Lv30 |
| 244 | **ENTEI** | Scripted encounter at Mt Mortar 2F Lv40 |
| 245 | **SUICUNE** | Scripted encounter at Lake Of Rage Lv35; Scripted encounter at Route 25 Frlg Lv50 |
| 246 | **LARVITAR** | Grass at DARK CAVE SOUTH SIDE; Grass at MT SILVER 1F ITEM ROOM; +4 other sources in machine catalog |
| 247 | **PUPITAR** | Grass at MT SILVER 1F ITEM ROOM; Grass at MT SILVER 1F MOLTRES ROOM; Evolve LARVITAR; +3 other sources in machine catalog |
| 248 | **TYRANITAR** | Evolve PUPITAR |
| 249 | **LUGIA** | Scripted encounter at Navel Rock Base Frlg Lv70; Scripted encounter at Navel Rock Bottom Lv70; +3 other sources in machine catalog |
| 250 | **HO OH** | Scripted encounter at Navel Rock Summit Frlg Lv70; Scripted encounter at Navel Rock Top Lv70; +2 other sources in machine catalog |
| 251 | **CELEBI** | Scripted encounter at Ilex Forest Lv40 |

</details>

<details>
<summary><strong>Generation III · National #252–386</strong></summary>

| # | Pokémon | Source-candidate locations / method |
| ---: | --- | --- |
| 252 | **TREECKO** | Grass at ROUTE 101 |
| 253 | **GROVYLE** | Evolve TREECKO |
| 254 | **SCEPTILE** | Evolve GROVYLE |
| 255 | **TORCHIC** | Grass at ROUTE 101 |
| 256 | **COMBUSKEN** | Evolve TORCHIC |
| 257 | **BLAZIKEN** | Evolve COMBUSKEN |
| 258 | **MUDKIP** | Grass at ROUTE 101 |
| 259 | **MARSHTOMP** | Evolve MUDKIP |
| 260 | **SWAMPERT** | Evolve MARSHTOMP |
| 261 | **POOCHYENA** | Grass at PETALBURG WOODS; Grass at ROUTE 101; +9 other sources in machine catalog |
| 262 | **MIGHTYENA** | Grass at ROUTE 120; Grass at ROUTE 121; Evolve POOCHYENA; +1 other sources in machine catalog |
| 263 | **ZIGZAGOON** | Grass at ROUTE 101; Grass at ROUTE 102; +7 other sources in machine catalog |
| 264 | **LINOONE** | Grass at ROUTE 118; Grass at ROUTE 119; Evolve ZIGZAGOON |
| 265 | **WURMPLE** | Grass at PETALBURG WOODS; Grass at ROUTE 101; +2 other sources in machine catalog |
| 266 | **SILCOON** | Grass at PETALBURG WOODS; Evolve WURMPLE |
| 267 | **BEAUTIFLY** | Evolve SILCOON |
| 268 | **CASCOON** | Grass at PETALBURG WOODS; Evolve WURMPLE |
| 269 | **DUSTOX** | Evolve CASCOON |
| 270 | **LOTAD** | Grass at ROUTE 102; Grass at ROUTE 114; +2 other sources in machine catalog |
| 271 | **LOMBRE** | Grass at ROUTE 114; Evolve LOTAD |
| 272 | **LUDICOLO** | Evolve LOMBRE |
| 273 | **SEEDOT** | Grass at ROUTE 120 |
| 274 | **NUZLEAF** | Grass at ROUTE 114; Evolve SEEDOT |
| 275 | **SHIFTRY** | Evolve NUZLEAF |
| 276 | **TAILLOW** | Grass at PETALBURG WOODS; Grass at ROUTE 104; +2 other sources in machine catalog |
| 277 | **SWELLOW** | Grass at ROUTE 115; Evolve TAILLOW |
| 278 | **WINGULL** | Grass at MT PYRE EXTERIOR; Grass at ROUTE 103; +37 other sources in machine catalog |
| 279 | **PELIPPER** | Surf at DEWFORD TOWN; Surf at EVER GRANDE CITY; Evolve WINGULL; +29 other sources in machine catalog |
| 280 | **RALTS** | Grass at ROUTE 102 |
| 281 | **KIRLIA** | Evolve RALTS |
| 282 | **GARDEVOIR** | Evolve KIRLIA |
| 283 | **SURSKIT** | Grass at ROUTE 102; Grass at ROUTE 117 |
| 284 | **MASQUERAIN** | Evolve SURSKIT |
| 285 | **SHROOMISH** | Grass at PETALBURG WOODS |
| 286 | **BRELOOM** | Evolve SHROOMISH |
| 287 | **SLAKOTH** | Grass at PETALBURG WOODS |
| 288 | **VIGOROTH** | Evolve SLAKOTH |
| 289 | **SLAKING** | Evolve VIGOROTH |
| 290 | **NINCADA** | Grass at ROUTE 116 |
| 291 | **NINJASK** | Evolve NINCADA |
| 292 | **SHEDINJA** | Evolve NINCADA |
| 293 | **WHISMUR** | Grass at DESERT UNDERPASS; Grass at ROUTE 116; +2 other sources in machine catalog |
| 294 | **LOUDRED** | Grass at DESERT UNDERPASS; Grass at VICTORY ROAD 1F; Evolve WHISMUR |
| 295 | **EXPLOUD** | Evolve LOUDRED |
| 296 | **MAKUHITA** | Grass at GRANITE CAVE 1F; Grass at GRANITE CAVE B 1F; +2 other sources in machine catalog |
| 297 | **HARIYAMA** | Grass at VICTORY ROAD 1F; Grass at VICTORY ROAD B 1F; Evolve MAKUHITA |
| 298 | **AZURILL** | Supplemental grass at ROUTE 104 |
| 299 | **NOSEPASS** | Rock Smash at GRANITE CAVE B 2F |
| 300 | **SKITTY** | Grass at ROUTE 116 |
| 301 | **DELCATTY** | Evolve SKITTY |
| 302 | **SABLEYE** | Grass at CAVE OF ORIGIN 1F; Grass at CAVE OF ORIGIN UNUSED RUBY SAPPHIRE MAP 1; +8 other sources in machine catalog |
| 303 | **MAWILE** | Grass at VICTORY ROAD B 1F; Grass at VICTORY ROAD B 2F |
| 304 | **ARON** | Grass at GRANITE CAVE B 1F; Grass at GRANITE CAVE B 2F; +2 other sources in machine catalog |
| 305 | **LAIRON** | Grass at VICTORY ROAD 1F; Grass at VICTORY ROAD B 1F; Evolve ARON; +1 other sources in machine catalog |
| 306 | **AGGRON** | Evolve LAIRON |
| 307 | **MEDITITE** | Rare grass at MT MORTAR 1F NORTH; Supplemental grass at GRANITE CAVE 1F |
| 308 | **MEDICHAM** | Gift at Safari Zone Gate Safari Zone Entrance Lv5; Evolve MEDITITE |
| 309 | **ELECTRIKE** | Grass at ROUTE 110; Grass at ROUTE 118 |
| 310 | **MANECTRIC** | Grass at ROUTE 118; Evolve ELECTRIKE |
| 311 | **PLUSLE** | Grass at ROUTE 110 |
| 312 | **MINUN** | Grass at ROUTE 110 |
| 313 | **VOLBEAT** | Grass at ROUTE 117 |
| 314 | **ILLUMISE** | Grass at ROUTE 117 |
| 315 | **ROSELIA** | Grass at ROUTE 117; Grass at ROUTE 119; Evolve BUDEW; +2 other sources in machine catalog |
| 316 | **GULPIN** | Grass at ROUTE 110 |
| 317 | **SWALOT** | Evolve GULPIN |
| 318 | **CARVANHA** | Fishing at ROUTE 118; Fishing at ROUTE 119 |
| 319 | **SHARPEDO** | Fishing at MOSSDEEP CITY; Fishing at PACIFIDLOG TOWN; Evolve CARVANHA; +13 other sources in machine catalog |
| 320 | **WAILMER** | Fishing at DEWFORD TOWN; Fishing at EVER GRANDE CITY; +31 other sources in machine catalog |
| 321 | **WAILORD** | Surf at ROUTE 129; Evolve WAILMER |
| 322 | **NUMEL** | Grass at FIERY PATH; Grass at JAGGED PASS; +1 other sources in machine catalog |
| 323 | **CAMERUPT** | Evolve NUMEL |
| 324 | **TORKOAL** | Grass at FIERY PATH; Grass at MAGMA HIDEOUT 1F; +7 other sources in machine catalog |
| 325 | **SPOINK** | Grass at JAGGED PASS |
| 326 | **GRUMPIG** | Evolve SPOINK |
| 327 | **SPINDA** | Grass at ROUTE 113 |
| 328 | **TRAPINCH** | Grass at MIRAGE TOWER 1F; Grass at MIRAGE TOWER 2F; +3 other sources in machine catalog |
| 329 | **VIBRAVA** | Evolve TRAPINCH |
| 330 | **FLYGON** | Evolve VIBRAVA |
| 331 | **CACNEA** | Grass at ROUTE 111 |
| 332 | **CACTURNE** | Evolve CACNEA |
| 333 | **SWABLU** | Grass at ROUTE 114; Grass at ROUTE 115 |
| 334 | **ALTARIA** | Grass at SKY PILLAR 5F; Evolve SWABLU |
| 335 | **ZANGOOSE** | Grass at ROUTE 114 |
| 336 | **SEVIPER** | Grass at ROUTE 114 |
| 337 | **LUNATONE** | Grass at METEOR FALLS 1F 1R; Grass at METEOR FALLS 1F 2R; +3 other sources in machine catalog |
| 338 | **SOLROCK** | Grass at METEOR FALLS 1F 1R; Grass at METEOR FALLS 1F 2R; +7 other sources in machine catalog |
| 339 | **BARBOACH** | Fishing at METEOR FALLS 1F 1R; Fishing at METEOR FALLS 1F 2R; +7 other sources in machine catalog |
| 340 | **WHISCASH** | Fishing at METEOR FALLS 1F 2R; Fishing at METEOR FALLS B 1F 1R; Evolve BARBOACH; +2 other sources in machine catalog |
| 341 | **CORPHISH** | Fishing at PETALBURG CITY; Fishing at ROUTE 102; +1 other sources in machine catalog |
| 342 | **CRAWDAUNT** | Evolve CORPHISH |
| 343 | **BALTOY** | Grass at ROUTE 111 |
| 344 | **CLAYDOL** | Grass at SKY PILLAR 1F; Grass at SKY PILLAR 3F; Evolve BALTOY; +1 other sources in machine catalog |
| 345 | **LILEEP** | Gift at Ruins Of Alph Lab Lv20; Gift at Rustboro City Devon Corp 2F Lv20; +1 other sources in machine catalog |
| 346 | **CRADILY** | Evolve LILEEP |
| 347 | **ANORITH** | Gift at Ruins Of Alph Lab Lv20; Gift at Rustboro City Devon Corp 2F Lv20; +1 other sources in machine catalog |
| 348 | **ARMALDO** | Evolve ANORITH |
| 349 | **FEEBAS** | Supplemental fishing at ROUTE 120 |
| 350 | **MILOTIC** | Evolve FEEBAS |
| 351 | **CASTFORM** | Gift at Route 119 Weather Institute 2F Lv25 [CASTFORM NORMAL]; Gift at Safari Zone Gate Safari Zone Entrance Lv5 |
| 352 | **KECLEON** | Scripted encounter at Route 120 Lv30; Grass at ROUTE 118; +4 other sources in machine catalog |
| 353 | **SHUPPET** | Grass at MT PYRE 1F; Grass at MT PYRE 2F; +8 other sources in machine catalog |
| 354 | **BANETTE** | Grass at SKY PILLAR 1F; Grass at SKY PILLAR 3F; Evolve SHUPPET; +1 other sources in machine catalog |
| 355 | **DUSKULL** | Grass at MT PYRE 4F; Grass at MT PYRE 5F; +2 other sources in machine catalog |
| 356 | **DUSCLOPS** | Evolve DUSKULL |
| 357 | **TROPIUS** | Grass at ROUTE 119 |
| 358 | **CHIMECHO** | Grass at MT PYRE SUMMIT; Evolve CHINGLING |
| 359 | **ABSOL** | Grass at ROUTE 120 |
| 360 | **WYNAUT** | Grass at ROUTE 130 |
| 361 | **SNORUNT** | Grass at SHOAL CAVE LOW TIDE ICE ROOM |
| 362 | **GLALIE** | Evolve SNORUNT |
| 363 | **SPHEAL** | Grass at SHOAL CAVE LOW TIDE ENTRANCE ROOM; Grass at SHOAL CAVE LOW TIDE ICE ROOM; +5 other sources in machine catalog |
| 364 | **SEALEO** | Evolve SPHEAL |
| 365 | **WALREIN** | Evolve SEALEO |
| 366 | **CLAMPERL** | Surf at UNDERWATER ROUTE 124; Surf at UNDERWATER ROUTE 126 |
| 367 | **HUNTAIL** | Evolve CLAMPERL |
| 368 | **GOREBYSS** | Evolve CLAMPERL |
| 369 | **RELICANTH** | Surf at UNDERWATER ROUTE 124; Surf at UNDERWATER ROUTE 126 |
| 370 | **LUVDISC** | Fishing at EVER GRANDE CITY; Fishing at ROUTE 128 |
| 371 | **BAGON** | Gift at Safari Zone Gate Safari Zone Entrance Lv5; Grass at METEOR FALLS B 1F 2R |
| 372 | **SHELGON** | Evolve BAGON |
| 373 | **SALAMENCE** | Evolve SHELGON |
| 374 | **BELDUM** | Gift at Mossdeep City Stevens House Lv5; Gift at Safari Zone Gate Safari Zone Entrance Lv5; +1 other sources in machine catalog |
| 375 | **METANG** | Evolve BELDUM |
| 376 | **METAGROSS** | Evolve METANG |
| 377 | **REGIROCK** | Scripted encounter at Desert Ruins Lv40 |
| 378 | **REGICE** | Scripted encounter at Island Cave Lv40 |
| 379 | **REGISTEEL** | Scripted encounter at Ancient Tomb Lv40 |
| 380 | **LATIAS** | Scripted encounter at Route 120 Lv35; Scripted encounter at Southern Island Interior Lv50 |
| 381 | **LATIOS** | Scripted encounter at Mossdeep City Space Center 1F Lv40; Scripted encounter at Southern Island Interior Lv50 |
| 382 | **KYOGRE** | Scripted encounter at Embedded Tower Lv70; Scripted encounter at Marine Cave End Lv70 |
| 383 | **GROUDON** | Scripted encounter at Embedded Tower Lv70; Scripted encounter at Terra Cave End Lv70 |
| 384 | **RAYQUAZA** | Scripted encounter at Sky Pillar Top Lv70 |
| 385 | **JIRACHI** | Scripted encounter at Route 102 Lv9 |
| 386 | **DEOXYS** | Scripted encounter at Birth Island Exterior Lv30 [DEOXYS NORMAL]; Scripted encounter at Birth Island Exterior Frlg Lv30 |

</details>

<details>
<summary><strong>Generation IV · National #387–493</strong></summary>

| # | Pokémon | Source-candidate locations / method |
| ---: | --- | --- |
| 387 | **TURTWIG** | Rare grass at ROUTE 2 |
| 388 | **GROTLE** | Evolve TURTWIG |
| 389 | **TORTERRA** | Evolve GROTLE |
| 390 | **CHIMCHAR** | Rare grass at ROUTE 3 |
| 391 | **MONFERNO** | Evolve CHIMCHAR |
| 392 | **INFERNAPE** | Evolve MONFERNO |
| 393 | **PIPLUP** | Rare grass at MT MOON 1F |
| 394 | **PRINPLUP** | Evolve PIPLUP |
| 395 | **EMPOLEON** | Evolve PRINPLUP |
| 396 | **STARLY** | Rare grass at ROUTE 6 |
| 397 | **STARAVIA** | Evolve STARLY |
| 398 | **STARAPTOR** | Evolve STARAVIA |
| 399 | **BIDOOF** | Supplemental grass at ROUTE 18 |
| 400 | **BIBAREL** | Evolve BIDOOF |
| 401 | **KRICKETOT** | Supplemental grass at MT MOON B 2F |
| 402 | **KRICKETUNE** | Evolve KRICKETOT |
| 403 | **SHINX** | Rare grass at ROUTE 4 |
| 404 | **LUXIO** | Evolve SHINX |
| 405 | **LUXRAY** | Evolve LUXIO |
| 406 | **BUDEW** | Supplemental grass at ROUTE 7 |
| 407 | **ROSERADE** | Evolve ROSELIA |
| 408 | **CRANIDOS** | Supplemental grass at MT MOON B 1F |
| 409 | **RAMPARDOS** | Evolve CRANIDOS |
| 410 | **SHIELDON** | Supplemental grass at MT MOON B 2F |
| 411 | **BASTIODON** | Evolve SHIELDON |
| 412 | **BURMY** | Grass at VIRIDIAN FOREST [BURMY PLANT]; Supplemental grass at ROUTE 17 [BURMY SANDY]; +2 other sources in machine catalog |
| 413 | **WORMADAM** | Evolve BURMY |
| 414 | **MOTHIM** | Supplemental grass at ROUTE 7 [MOTHIM PLANT]; Evolve BURMY |
| 415 | **COMBEE** | Rare grass at NATIONAL PARK NORMAL; Supplemental grass at ROUTE 3 |
| 416 | **VESPIQUEN** | Evolve COMBEE |
| 417 | **PACHIRISU** | Supplemental grass at POWER PLANT |
| 418 | **BUIZEL** | Rare grass at ROUTE 118; Supplemental grass at ROUTE 4 |
| 419 | **FLOATZEL** | Evolve BUIZEL |
| 420 | **CHERUBI** | Supplemental grass at ROUTE 5 |
| 421 | **CHERRIM** | Evolve CHERUBI |
| 422 | **SHELLOS** | Supplemental fishing at ROUTE 132 [SHELLOS EAST]; Supplemental Surf at ROUTE 131 [SHELLOS EAST]; +1 other sources in machine catalog |
| 423 | **GASTRODON** | Evolve SHELLOS |
| 424 | **AMBIPOM** | Evolve AIPOM |
| 425 | **DRIFLOON** | Supplemental grass at POKEMON TOWER 6F |
| 426 | **DRIFBLIM** | Evolve DRIFLOON |
| 427 | **BUNEARY** | Rare grass at ROUTE 5 |
| 428 | **LOPUNNY** | Evolve BUNEARY |
| 429 | **MISMAGIUS** | Evolve MISDREAVUS |
| 430 | **HONCHKROW** | Evolve MURKROW |
| 431 | **GLAMEOW** | Supplemental grass at ROUTE 8 |
| 432 | **PURUGLY** | Evolve GLAMEOW |
| 433 | **CHINGLING** | Rare grass at ROUTE 44; Supplemental grass at MT MOON 1F |
| 434 | **STUNKY** | Supplemental grass at POKEMON TOWER 7F |
| 435 | **SKUNTANK** | Evolve STUNKY |
| 436 | **BRONZOR** | Supplemental grass at ROUTE 11 |
| 437 | **BRONZONG** | Evolve BRONZOR |
| 438 | **BONSLY** | Supplemental grass at ROCK TUNNEL B 1F |
| 439 | **MIME JR** | Supplemental grass at ROUTE 25 |
| 440 | **HAPPINY** | Supplemental grass at ROUTE 10 |
| 441 | **CHATOT** | Supplemental grass at ROUTE 9 |
| 442 | **SPIRITOMB** | Rare grass at MT PYRE SUMMIT; Supplemental grass at POKEMON TOWER 5F |
| 443 | **GIBLE** | Rare grass at VICTORY ROAD 3F |
| 444 | **GABITE** | Evolve GIBLE |
| 445 | **GARCHOMP** | Evolve GABITE |
| 446 | **MUNCHLAX** | Supplemental grass at ROUTE 4 |
| 447 | **RIOLU** | Rare grass at MT MORTAR 2F; Supplemental grass at ROUTE 23 |
| 448 | **LUCARIO** | Rare grass at VICTORY ROAD B 1F; Evolve RIOLU |
| 449 | **HIPPOPOTAS** | Supplemental grass at DIGLETTS CAVE B 1F |
| 450 | **HIPPOWDON** | Evolve HIPPOPOTAS |
| 451 | **SKORUPI** | Supplemental grass at MT MOON 1F |
| 452 | **DRAPION** | Evolve SKORUPI |
| 453 | **CROAGUNK** | Supplemental grass at ROCK TUNNEL 1F |
| 454 | **TOXICROAK** | Evolve CROAGUNK |
| 455 | **CARNIVINE** | Supplemental grass at ROUTE 6 |
| 456 | **FINNEON** | Supplemental fishing at ROUTE 105; Supplemental fishing at ROUTE 20; +2 other sources in machine catalog |
| 457 | **LUMINEON** | Evolve FINNEON |
| 458 | **MANTYKE** | Rare grass at WHIRL ISLANDS B 2F; Supplemental fishing at ROUTE 128; +3 other sources in machine catalog |
| 459 | **SNOVER** | Rare grass at MT SILVER OUTSIDE; Supplemental grass at SEAFOAM ISLANDS 1F |
| 460 | **ABOMASNOW** | Evolve SNOVER |
| 461 | **WEAVILE** | Evolve SNEASEL |
| 462 | **MAGNEZONE** | Evolve MAGNETON |
| 463 | **LICKILICKY** | Supplemental grass at ROUTE 13; Evolve LICKITUNG |
| 464 | **RHYPERIOR** | Evolve RHYDON |
| 465 | **TANGROWTH** | Evolve TANGELA |
| 466 | **ELECTIVIRE** | Evolve ELECTABUZZ |
| 467 | **MAGMORTAR** | Evolve MAGMAR |
| 468 | **TOGEKISS** | Supplemental grass at ROUTE 24; Evolve TOGETIC |
| 469 | **YANMEGA** | Evolve YANMA |
| 470 | **LEAFEON** | Evolve EEVEE |
| 471 | **GLACEON** | Rare grass at MT SILVER SNOW; Supplemental grass at SEAFOAM ISLANDS B 2F; Evolve EEVEE |
| 472 | **GLISCOR** | Supplemental grass at ROCK TUNNEL 1F; Evolve GLIGAR |
| 473 | **MAMOSWINE** | Evolve PILOSWINE |
| 474 | **PORYGON Z** | Evolve PORYGON 2 |
| 475 | **GALLADE** | Supplemental grass at ROUTE 25; Evolve KIRLIA |
| 476 | **PROBOPASS** | Supplemental grass at ROCK TUNNEL B 1F; Evolve NOSEPASS |
| 477 | **DUSKNOIR** | Supplemental grass at POKEMON TOWER 6F; Evolve DUSCLOPS |
| 478 | **FROSLASS** | Supplemental grass at SEAFOAM ISLANDS B 2F; Evolve SNORUNT |
| 479 | **ROTOM** | Rare grass at POKEMON MANSION 2F |
| 480 | **UXIE** | Scripted encounter at Rock Tunnel B 1F Frlg Lv45 |
| 481 | **MESPRIT** | Scripted event at Rock Tunnel B 1F Frlg |
| 482 | **AZELF** | Scripted event at Rock Tunnel B 1F Frlg |
| 483 | **DIALGA** | Scripted encounter at Mt Moon B 1F Frlg Lv45 |
| 484 | **PALKIA** | Scripted event at Mt Moon B 1F Frlg |
| 485 | **HEATRAN** | Scripted event at Rock Tunnel B 1F Frlg |
| 486 | **REGIGIGAS** | Scripted encounter at Rock Tunnel B 1F Frlg Lv45 |
| 487 | **GIRATINA** | Scripted encounter at Mt Moon B 2F Frlg Lv52 |
| 488 | **CRESSELIA** | Scripted encounter at Mt Moon B 1F Frlg Lv45 |
| 489 | **PHIONE** | Breeding at Route 5 Day Care Frlg |
| 490 | **MANAPHY** | Scripted encounter at Seafoam Islands B 2F Frlg Lv35 |
| 491 | **DARKRAI** | Scripted encounter at Pokemon Tower 7F Frlg Lv45 |
| 492 | **SHAYMIN** | Scripted encounter at Viridian Forest Frlg Lv10 |
| 493 | **ARCEUS** | Scripted encounter at Indigo Plateau Exterior Frlg Lv50 |

</details>

<details>
<summary><strong>Generation V · National #494–649</strong></summary>

| # | Pokémon | Source-candidate locations / method |
| ---: | --- | --- |
| 494 | **VICTINI** | Scripted encounter at National Park Normal Lv42 |
| 495 | **SNIVY** | Rare grass at ROUTE 29 |
| 496 | **SERVINE** | Evolve SNIVY |
| 497 | **SERPERIOR** | Evolve SERVINE |
| 498 | **TEPIG** | Rare grass at ROUTE 30 |
| 499 | **PIGNITE** | Evolve TEPIG |
| 500 | **EMBOAR** | Evolve PIGNITE |
| 501 | **OSHAWOTT** | Rare grass at ROUTE 31 |
| 502 | **DEWOTT** | Evolve OSHAWOTT |
| 503 | **SAMUROTT** | Supplemental grass at ROUTE 47 [SAMUROTT HISUI]; Evolve DEWOTT |
| 504 | **PATRAT** | Supplemental grass at UNION CAVE B 2F |
| 505 | **WATCHOG** | Evolve PATRAT |
| 506 | **LILLIPUP** | Rare grass at ROUTE 46 |
| 507 | **HERDIER** | Evolve LILLIPUP |
| 508 | **STOUTLAND** | Evolve HERDIER |
| 509 | **PURRLOIN** | Supplemental grass at BURNED TOWER 1F |
| 510 | **LIEPARD** | Evolve PURRLOIN |
| 511 | **PANSAGE** | Rare grass at ROUTE 43 |
| 512 | **SIMISAGE** | Evolve PANSAGE |
| 513 | **PANSEAR** | Supplemental grass at ROUTE 37 |
| 514 | **SIMISEAR** | Evolve PANSEAR |
| 515 | **PANPOUR** | Supplemental grass at UNION CAVE B 1F |
| 516 | **SIMIPOUR** | Evolve PANPOUR |
| 517 | **MUNNA** | Rare grass at ROUTE 114; Supplemental grass at ROUTE 29 |
| 518 | **MUSHARNA** | Evolve MUNNA |
| 519 | **PIDOVE** | Rare grass at ROUTE 104; Supplemental grass at ROUTE 30 |
| 520 | **TRANQUILL** | Evolve PIDOVE |
| 521 | **UNFEZANT** | Evolve TRANQUILL |
| 522 | **BLITZLE** | Supplemental grass at ROUTE 32 |
| 523 | **ZEBSTRIKA** | Evolve BLITZLE |
| 524 | **ROGGENROLA** | Rare grass at ROCK TUNNEL 1F; Supplemental grass at DARK CAVE SOUTH SIDE |
| 525 | **BOLDORE** | Evolve ROGGENROLA |
| 526 | **GIGALITH** | Evolve BOLDORE |
| 527 | **WOOBAT** | Rare grass at GRANITE CAVE 1F; Supplemental grass at DARK CAVE NORTH SIDE |
| 528 | **SWOOBAT** | Evolve WOOBAT |
| 529 | **DRILBUR** | Rare grass at DIGLETTS CAVE B 1F; Supplemental grass at UNION CAVE 1F |
| 530 | **EXCADRILL** | Evolve DRILBUR |
| 531 | **AUDINO** | Supplemental grass at ROUTE 38 |
| 532 | **TIMBURR** | Rare grass at DARK CAVE SOUTH SIDE |
| 533 | **GURDURR** | Evolve TIMBURR |
| 534 | **CONKELDURR** | Evolve GURDURR |
| 535 | **TYMPOLE** | Supplemental fishing at ROUTE 42; Supplemental grass at SLOWPOKE WELL B 1F; +1 other sources in machine catalog |
| 536 | **PALPITOAD** | Evolve TYMPOLE |
| 537 | **SEISMITOAD** | Evolve PALPITOAD |
| 538 | **THROH** | Supplemental grass at MT MORTAR 1F SOUTH |
| 539 | **SAWK** | Supplemental grass at MT MORTAR B 1F |
| 540 | **SEWADDLE** | Rare grass at SPROUT TOWER 2F |
| 541 | **SWADLOON** | Evolve SEWADDLE |
| 542 | **LEAVANNY** | Evolve SWADLOON |
| 543 | **VENIPEDE** | Supplemental grass at NATIONAL PARK NORMAL |
| 544 | **WHIRLIPEDE** | Evolve VENIPEDE |
| 545 | **SCOLIPEDE** | Evolve WHIRLIPEDE |
| 546 | **COTTONEE** | Grass at ILEX FOREST; Supplemental grass at ROUTE 30; +1 other sources in machine catalog |
| 547 | **WHIMSICOTT** | Evolve COTTONEE |
| 548 | **PETILIL** | Rare grass at ROUTE 24; Supplemental grass at ILEX FOREST |
| 549 | **LILLIGANT** | Supplemental grass at ROUTE 36 [LILLIGANT HISUI]; Evolve PETILIL |
| 550 | **BASCULIN** | Supplemental Surf at ROUTE 104 [BASCULIN RED STRIPED]; Supplemental Surf at ROUTE 119 [BASCULIN BLUE STRIPED]; +1 other sources in machine catalog |
| 551 | **SANDILE** | Rare grass at DARK CAVE NORTH SIDE; Rare grass at MIRAGE TOWER 1F |
| 552 | **KROKOROK** | Rare grass at MIRAGE TOWER 2F; Evolve SANDILE |
| 553 | **KROOKODILE** | Evolve KROKOROK |
| 554 | **DARUMAKA** | Supplemental grass at BURNED TOWER B 1F; Supplemental grass at ICE PATH 1F [DARUMAKA GALAR] |
| 555 | **DARMANITAN** | Evolve DARUMAKA |
| 556 | **MARACTUS** | Supplemental grass at ILEX FOREST |
| 557 | **DWEBBLE** | Supplemental grass at UNION CAVE 1F |
| 558 | **CRUSTLE** | Evolve DWEBBLE |
| 559 | **SCRAGGY** | Supplemental grass at ICE PATH B 2F |
| 560 | **SCRAFTY** | Evolve SCRAGGY |
| 561 | **SIGILYPH** | Rare grass at RUINS OF ALPH B 1F |
| 562 | **YAMASK** | Rare grass at MT PYRE 1F; Supplemental grass at BURNED TOWER 1F; +1 other sources in machine catalog |
| 563 | **COFAGRIGUS** | Evolve YAMASK |
| 564 | **TIRTOUGA** | Supplemental grass at WHIRL ISLANDS 1F |
| 565 | **CARRACOSTA** | Evolve TIRTOUGA |
| 566 | **ARCHEN** | Supplemental grass at RUINS OF ALPH OUTSIDE |
| 567 | **ARCHEOPS** | Evolve ARCHEN |
| 568 | **TRUBBISH** | Supplemental grass at ROUTE 42 |
| 569 | **GARBODOR** | Evolve TRUBBISH |
| 570 | **ZORUA** | Supplemental grass at ICE PATH B 3F; Supplemental grass at ILEX FOREST [ZORUA HISUI] |
| 571 | **ZOROARK** | Evolve ZORUA |
| 572 | **MINCCINO** | Supplemental grass at ROUTE 35 |
| 573 | **CINCCINO** | Evolve MINCCINO |
| 574 | **GOTHITA** | Supplemental grass at ROUTE 34 |
| 575 | **GOTHORITA** | Evolve GOTHITA |
| 576 | **GOTHITELLE** | Evolve GOTHORITA |
| 577 | **SOLOSIS** | Supplemental grass at RUINS OF ALPH OUTSIDE |
| 578 | **DUOSION** | Evolve SOLOSIS |
| 579 | **REUNICLUS** | Evolve DUOSION |
| 580 | **DUCKLETT** | Rare grass at ROUTE 12; Supplemental Surf at ROUTE 35 |
| 581 | **SWANNA** | Evolve DUCKLETT |
| 582 | **VANILLITE** | Rare grass at ICE PATH 1F |
| 583 | **VANILLISH** | Evolve VANILLITE |
| 584 | **VANILLUXE** | Evolve VANILLISH |
| 585 | **DEERLING** | Rare grass at ROUTE 13; Supplemental grass at ROUTE 37 [DEERLING AUTUMN]; +1 other sources in machine catalog |
| 586 | **SAWSBUCK** | Grass at ROUTE 37 [SAWSBUCK SPRING]; Evolve DEERLING |
| 587 | **EMOLGA** | Rare grass at ROUTE 110; Supplemental grass at ROUTE 32 |
| 588 | **KARRABLAST** | Rare grass at UNION CAVE B 2F |
| 589 | **ESCAVALIER** | Evolve KARRABLAST |
| 590 | **FOONGUS** | Rare grass at ROUTE 42 |
| 591 | **AMOONGUSS** | Evolve FOONGUS |
| 592 | **FRILLISH** | Rare grass at WHIRL ISLANDS 1F; Supplemental fishing at ROUTE 40; +1 other sources in machine catalog |
| 593 | **JELLICENT** | Evolve FRILLISH |
| 594 | **ALOMOMOLA** | Supplemental fishing at ROUTE 118; Supplemental fishing at ROUTE 19; +1 other sources in machine catalog |
| 595 | **JOLTIK** | Rare grass at ROUTE 116; Supplemental grass at SPROUT TOWER 2F |
| 596 | **GALVANTULA** | Evolve JOLTIK |
| 597 | **FERROSEED** | Supplemental grass at ROUTE 36 |
| 598 | **FERROTHORN** | Evolve FERROSEED |
| 599 | **KLINK** | Rare grass at GRANITE CAVE B 1F; Supplemental grass at UNION CAVE B 1F |
| 600 | **KLANG** | Evolve KLINK |
| 601 | **KLINKLANG** | Evolve KLANG |
| 602 | **TYNAMO** | Rare grass at NEW MAUVILLE INSIDE; Supplemental grass at ROUTE 43 |
| 603 | **EELEKTRIK** | Evolve TYNAMO |
| 604 | **EELEKTROSS** | Evolve EELEKTRIK |
| 605 | **ELGYEM** | Rare grass at METEOR FALLS 1F 1R; Supplemental grass at RUINS OF ALPH B 1F |
| 606 | **BEHEEYEM** | Evolve ELGYEM |
| 607 | **LITWICK** | Rare grass at POKEMON TOWER 3F; Supplemental grass at BURNED TOWER B 1F |
| 608 | **LAMPENT** | Rare grass at MT PYRE 3F; Evolve LITWICK |
| 609 | **CHANDELURE** | Evolve LAMPENT |
| 610 | **AXEW** | Rare grass at CERULEAN CAVE 1F; Supplemental grass at ROUTE 45 |
| 611 | **FRAXURE** | Evolve AXEW |
| 612 | **HAXORUS** | Evolve FRAXURE |
| 613 | **CUBCHOO** | Rare grass at ICE PATH B 1F; Rare grass at SHOAL CAVE LOW TIDE STAIRS ROOM |
| 614 | **BEARTIC** | Evolve CUBCHOO |
| 615 | **CRYOGONAL** | Rare grass at ICE PATH B 2F |
| 616 | **SHELMET** | Grass at ROUTE 32; Supplemental grass at ROUTE 31; +1 other sources in machine catalog |
| 617 | **ACCELGOR** | Evolve SHELMET |
| 618 | **STUNFISK** | Supplemental Surf at ROUTE 45 [STUNFISK GALAR] |
| 619 | **MIENFOO** | Rare grass at MT MORTAR 1F SOUTH |
| 620 | **MIENSHAO** | Evolve MIENFOO |
| 621 | **DRUDDIGON** | Rare grass at CERULEAN CAVE B 1F; Supplemental grass at ROUTE 45 |
| 622 | **GOLETT** | Rare grass at MT MORTAR B 1F |
| 623 | **GOLURK** | Rare grass at GRANITE CAVE B 2F; Evolve GOLETT |
| 624 | **PAWNIARD** | Rare grass at ROUTE 23; Supplemental grass at ROUTE 45 |
| 625 | **BISHARP** | Evolve PAWNIARD |
| 626 | **BOUFFALANT** | Supplemental grass at ROUTE 33 |
| 627 | **RUFFLET** | Supplemental grass at ROUTE 30 |
| 628 | **BRAVIARY** | Supplemental grass at ROUTE 48 [BRAVIARY HISUI]; Evolve RUFFLET |
| 629 | **VULLABY** | Supplemental grass at ICE PATH B 4F |
| 630 | **MANDIBUZZ** | Evolve VULLABY |
| 631 | **HEATMOR** | Rare grass at ROUTE 113; Supplemental grass at ROUTE 42 |
| 632 | **DURANT** | Rare grass at METEOR FALLS B 1F 2R; Rare grass at ROUTE 45 |
| 633 | **DEINO** | Rare grass at VICTORY ROAD 2F; Supplemental grass at ICE PATH B 4F |
| 634 | **ZWEILOUS** | Evolve DEINO |
| 635 | **HYDREIGON** | Evolve ZWEILOUS |
| 636 | **LARVESTA** | Rare grass at FIERY PATH; Supplemental grass at BURNED TOWER 1F |
| 637 | **VOLCARONA** | Evolve LARVESTA |
| 638 | **COBALION** | Scripted encounter at Mt Mortar 1F South Lv28 |
| 639 | **TERRAKION** | Scripted encounter at Union Cave 1F Lv20 |
| 640 | **VIRIZION** | Scripted encounter at National Park Normal Lv23 |
| 641 | **TORNADUS** | Scripted encounter at Route 44 Lv40 |
| 642 | **THUNDURUS** | Scripted encounter at Route 41 Lv36 |
| 643 | **RESHIRAM** | Scripted encounter at Tin Tower 2F Lv44 |
| 644 | **ZEKROM** | Scripted encounter at Dark Cave North Side Lv44 |
| 645 | **LANDORUS** | Scripted encounter at Route 43 Lv42 |
| 646 | **KYUREM** | Scripted encounter at Ice Path B 1F Lv49 |
| 647 | **KELDEO** | Scripted encounter at Route 32 Lv18 |
| 648 | **MELOETTA** | Scripted encounter at Ecruteak City Theater Lv45 |
| 649 | **GENESECT** | Scripted encounter at Goldenrod City Lv45 |

</details>

<details>
<summary><strong>Generation VI · National #650–721</strong></summary>

| # | Pokémon | Source-candidate locations / method |
| ---: | --- | --- |
| 650 | **CHESPIN** | Rare grass at ROUTE 101 |
| 651 | **QUILLADIN** | Evolve CHESPIN |
| 652 | **CHESNAUGHT** | Evolve QUILLADIN |
| 653 | **FENNEKIN** | Rare grass at ROUTE 102 |
| 654 | **BRAIXEN** | Evolve FENNEKIN |
| 655 | **DELPHOX** | Evolve BRAIXEN |
| 656 | **FROAKIE** | Rare grass at ROUTE 103 |
| 657 | **FROGADIER** | Evolve FROAKIE |
| 658 | **GRENINJA** | Supplemental Surf at ROUTE 129 [GRENINJA BATTLE BOND]; Evolve FROGADIER |
| 659 | **BUNNELBY** | Supplemental grass at ROUTE 114 |
| 660 | **DIGGERSBY** | Evolve BUNNELBY |
| 661 | **FLETCHLING** | Rare grass at ROUTE 8; Supplemental grass at ROUTE 104 |
| 662 | **FLETCHINDER** | Rare grass at ROUTE 16; Evolve FLETCHLING |
| 663 | **TALONFLAME** | Evolve FLETCHINDER |
| 664 | **SCATTERBUG** | Grass at PETALBURG WOODS [SCATTERBUG ARCHIPELAGO]; Supplemental grass at PETALBURG WOODS; +2 other sources in machine catalog |
| 665 | **SPEWPA** | Supplemental grass at ROUTE 104 [SPEWPA MEADOW]; Evolve TALONFLAME |
| 666 | **VIVILLON** | Supplemental grass at PETALBURG WOODS [VIVILLON MEADOW]; Evolve SPEWPA |
| 667 | **LITLEO** | Supplemental grass at ROUTE 113 |
| 668 | **PYROAR** | Evolve LITLEO |
| 669 | **FLABEBE** | Rare grass at TIN TOWER 8F; Supplemental grass at ROUTE 117; +2 other sources in machine catalog |
| 670 | **FLOETTE** | Grass at ROUTE 117 [FLOETTE RED]; Supplemental grass at ROUTE 121 [FLOETTE RED]; Evolve PYROAR |
| 671 | **FLORGES** | Supplemental grass at ROUTE 115 [FLORGES RED]; Evolve FLOETTE |
| 672 | **SKIDDO** | Rare grass at ROUTE 7; Supplemental grass at ROUTE 110 |
| 673 | **GOGOAT** | Evolve SKIDDO |
| 674 | **PANCHAM** | Supplemental grass at JAGGED PASS |
| 675 | **PANGORO** | Evolve PANCHAM |
| 676 | **FURFROU** | Grass at ROUTE 117 [FURFROU NATURAL]; Supplemental grass at ROUTE 114 [FURFROU NATURAL]; +1 other sources in machine catalog |
| 677 | **ESPURR** | Rare grass at TIN TOWER 2F; Supplemental grass at ROUTE 114 |
| 678 | **MEOWSTIC** | Evolve ESPURR |
| 679 | **HONEDGE** | Rare grass at POKEMON TOWER 6F; Supplemental grass at GRANITE CAVE B 2F |
| 680 | **DOUBLADE** | Evolve HONEDGE |
| 681 | **AEGISLASH** | Evolve DOUBLADE |
| 682 | **SPRITZEE** | Supplemental grass at ROUTE 112 |
| 683 | **AROMATISSE** | Evolve SPRITZEE |
| 684 | **SWIRLIX** | Rare grass at ROUTE 117 |
| 685 | **SLURPUFF** | Evolve SWIRLIX |
| 686 | **INKAY** | Rare grass at ROUTE 33; Supplemental grass at ROUTE 102 |
| 687 | **MALAMAR** | Evolve INKAY |
| 688 | **BINACLE** | Supplemental grass at GRANITE CAVE B 1F; Supplemental Surf at ROUTE 108; +1 other sources in machine catalog |
| 689 | **BARBARACLE** | Evolve BINACLE |
| 690 | **SKRELP** | Supplemental fishing at ROUTE 127; Supplemental grass at ROUTE 117; +3 other sources in machine catalog |
| 691 | **DRAGALGE** | Evolve SKRELP |
| 692 | **CLAUNCHER** | Rare grass at SEAFLOOR CAVERN ROOM 1; Supplemental fishing at ROUTE 44; +3 other sources in machine catalog |
| 693 | **CLAWITZER** | Evolve CLAUNCHER |
| 694 | **HELIOPTILE** | Supplemental grass at ROUTE 118 |
| 695 | **HELIOLISK** | Evolve HELIOPTILE |
| 696 | **TYRUNT** | Supplemental grass at GRANITE CAVE B 1F |
| 697 | **TYRANTRUM** | Evolve TYRUNT |
| 698 | **AMAURA** | Supplemental grass at SHOAL CAVE LOW TIDE STAIRS ROOM |
| 699 | **AURORUS** | Evolve AMAURA |
| 700 | **SYLVEON** | Evolve EEVEE |
| 701 | **HAWLUCHA** | Supplemental grass at GRANITE CAVE 1F |
| 702 | **DEDENNE** | Supplemental grass at ROUTE 110 |
| 703 | **CARBINK** | Rare grass at GRANITE CAVE STEVENS ROOM; Rare grass at MT MOON B 1F |
| 704 | **GOOMY** | Rare grass at SEAFLOOR CAVERN ROOM 8 |
| 705 | **SLIGGOO** | Supplemental grass at METEOR FALLS 1F 2R [SLIGGOO HISUI]; Evolve GOOMY |
| 706 | **GOODRA** | Evolve SLIGGOO |
| 707 | **KLEFKI** | Supplemental grass at VICTORY ROAD B 1F |
| 708 | **PHANTUMP** | Rare grass at POKEMON TOWER 7F; Supplemental grass at MT PYRE 1F |
| 709 | **TREVENANT** | Evolve PHANTUMP |
| 710 | **PUMPKABOO** | Rare grass at BURNED TOWER 1F; Rare grass at MT PYRE 4F |
| 711 | **GOURGEIST** | Grass at MT PYRE 1F [GOURGEIST AVERAGE]; Evolve PUMPKABOO |
| 712 | **BERGMITE** | Rare grass at SEAFOAM ISLANDS B 2F; Rare grass at SHOAL CAVE LOW TIDE INNER ROOM |
| 713 | **AVALUGG** | Supplemental grass at SHOAL CAVE LOW TIDE INNER ROOM [AVALUGG HISUI]; Evolve BERGMITE |
| 714 | **NOIBAT** | Rare grass at ROCK TUNNEL B 1F; Supplemental grass at GRANITE CAVE B 1F |
| 715 | **NOIVERN** | Evolve NOIBAT |
| 716 | **XERNEAS** | Scripted encounter at Route 119 Lv35 |
| 717 | **YVELTAL** | Scripted encounter at Mt Pyre Exterior Lv39 |
| 718 | **ZYGARDE** | Permanent Complete at Route 101 Lv70 [ZYGARDE COMPLETE]; Scripted encounter at Meteor Falls 1F 1R Lv44 [ZYGARDE 50 POWER CONSTRUCT]; +2 other sources in machine catalog |
| 719 | **DIANCIE** | Scripted encounter at Granite Cave Stevens Room Lv18 |
| 720 | **HOOPA** | Scripted encounter at Route 111 Hoopa Ring Chamber Lv30 [HOOPA CONFINED] |
| 721 | **VOLCANION** | Scripted encounter at Mt Chimney Lv32 |

</details>

<details>
<summary><strong>Generation VII · National #722–809</strong></summary>

| # | Pokémon | Source-candidate locations / method |
| ---: | --- | --- |
| 722 | **ROWLET** | Supplemental grass at ROUTE 24 |
| 723 | **DARTRIX** | Evolve ROWLET |
| 724 | **DECIDUEYE** | Supplemental grass at ROUTE 24 [DECIDUEYE HISUI]; Evolve DARTRIX |
| 725 | **LITTEN** | Supplemental grass at ROUTE 1 |
| 726 | **TORRACAT** | Evolve LITTEN |
| 727 | **INCINEROAR** | Evolve TORRACAT |
| 728 | **POPPLIO** | Supplemental grass at SEAFOAM ISLANDS B 2F |
| 729 | **BRIONNE** | Evolve POPPLIO |
| 730 | **PRIMARINA** | Evolve BRIONNE |
| 731 | **PIKIPEK** | Rare grass at ROUTE 48; Supplemental grass at ROUTE 2 |
| 732 | **TRUMBEAK** | Evolve PIKIPEK |
| 733 | **TOUCANNON** | Evolve TRUMBEAK |
| 734 | **YUNGOOS** | Supplemental grass at ROUTE 3 |
| 735 | **GUMSHOOS** | Evolve YUNGOOS |
| 736 | **GRUBBIN** | Supplemental grass at MT MOON B 1F |
| 737 | **CHARJABUG** | Rare grass at NEW MAUVILLE ENTRANCE; Evolve GRUBBIN |
| 738 | **VIKAVOLT** | Evolve CHARJABUG |
| 739 | **CRABRAWLER** | Rare grass at SEAFLOOR CAVERN ROOM 2; Supplemental grass at SEAFOAM ISLANDS B 3F |
| 740 | **CRABOMINABLE** | Evolve CRABRAWLER |
| 741 | **ORICORIO** | Rare grass at TIN TOWER 4F; Supplemental grass at ROUTE 6 |
| 742 | **CUTIEFLY** | Rare grass at SPROUT TOWER 3F; Supplemental grass at ROUTE 5 |
| 743 | **RIBOMBEE** | Evolve CUTIEFLY |
| 744 | **ROCKRUFF** | Rare grass at ROUTE 9; Supplemental grass at ROUTE 9 [ROCKRUFF OWN TEMPO] |
| 745 | **LYCANROC** | Evolve ROCKRUFF |
| 746 | **WISHIWASHI** | Supplemental grass at SEAFOAM ISLANDS B 4F; Supplemental Surf at ROUTE 103; +2 other sources in machine catalog |
| 747 | **MAREANIE** | Rare grass at ROUTE 39; Supplemental grass at ROUTE 25; +2 other sources in machine catalog |
| 748 | **TOXAPEX** | Evolve MAREANIE |
| 749 | **MUDBRAY** | Supplemental grass at POKEMON TOWER 3F |
| 750 | **MUDSDALE** | Evolve MUDBRAY |
| 751 | **DEWPIDER** | Rare grass at ROUTE 119; Rare grass at SLOWPOKE WELL B 1F; +3 other sources in machine catalog |
| 752 | **ARAQUANID** | Evolve DEWPIDER |
| 753 | **FOMANTIS** | Rare grass at ROUTE 121; Rare grass at ROUTE 14 |
| 754 | **LURANTIS** | Evolve FOMANTIS |
| 755 | **MORELULL** | Rare grass at ILEX FOREST; Supplemental grass at VIRIDIAN FOREST |
| 756 | **SHIINOTIC** | Evolve MORELULL |
| 757 | **SALANDIT** | Rare grass at ROUTE 112; Supplemental grass at POKEMON MANSION 3F |
| 758 | **SALAZZLE** | Evolve SALANDIT |
| 759 | **STUFFUL** | Supplemental grass at ROUTE 22 |
| 760 | **BEWEAR** | Evolve STUFFUL |
| 761 | **BOUNSWEET** | Supplemental grass at ROUTE 12 |
| 762 | **STEENEE** | Evolve BOUNSWEET |
| 763 | **TSAREENA** | Evolve STEENEE |
| 764 | **COMFEY** | Supplemental grass at POKEMON TOWER 4F |
| 765 | **ORANGURU** | Supplemental grass at ROUTE 2 |
| 766 | **PASSIMIAN** | Supplemental grass at VICTORY ROAD 2F |
| 767 | **WIMPOD** | Rare grass at WHIRL ISLANDS B 1F; Supplemental Surf at ROUTE 107; +2 other sources in machine catalog |
| 768 | **GOLISOPOD** | Evolve WIMPOD |
| 769 | **SANDYGAST** | Rare grass at SHOAL CAVE LOW TIDE LOWER ROOM; Supplemental grass at ROUTE 13 |
| 770 | **PALOSSAND** | Evolve SANDYGAST |
| 771 | **PYUKUMUKU** | Supplemental grass at SEAFOAM ISLANDS B 3F |
| 772 | **TYPE NULL** | Scripted encounter at Pokemon Mansion 1F Frlg Lv35 |
| 773 | **SILVALLY** | Evolve TYPE NULL |
| 774 | **MINIOR** | Rare grass at METEOR FALLS 1F 2R; Supplemental grass at MT MOON B 1F |
| 775 | **KOMALA** | Supplemental grass at ROUTE 23 |
| 776 | **TURTONATOR** | Rare grass at METEOR FALLS B 1F 1R; Supplemental grass at POKEMON MANSION B 1F |
| 777 | **TOGEDEMARU** | Supplemental grass at VIRIDIAN FOREST |
| 778 | **MIMIKYU** | Rare grass at BURNED TOWER B 1F; Supplemental grass at POKEMON TOWER 4F |
| 779 | **BRUXISH** | Supplemental fishing at ROUTE 106; Supplemental Surf at FUCHSIA CITY; +2 other sources in machine catalog |
| 780 | **DRAMPA** | Rare grass at VICTORY ROAD B 2F; Supplemental grass at VICTORY ROAD 2F |
| 781 | **DHELMISE** | Rare grass at WHIRL ISLANDS B 1F INNER; Supplemental fishing at ROUTE 107; +3 other sources in machine catalog |
| 782 | **JANGMO O** | Rare grass at METEOR FALLS STEVENS CAVE; Supplemental grass at VICTORY ROAD 3F |
| 783 | **HAKAMO O** | Evolve JANGMO O |
| 784 | **KOMMO O** | Evolve HAKAMO O |
| 785 | **TAPU KOKO** | Scripted encounter at Power Plant Frlg Lv45 |
| 786 | **TAPU LELE** | Scripted encounter at Celadon City Frlg Lv36 |
| 787 | **TAPU BULU** | Scripted encounter at Route 5 Frlg Lv45 |
| 788 | **TAPU FINI** | Scripted encounter at Seafoam Islands B 2F Frlg Lv45 |
| 789 | **COSMOG** | Scripted encounter at Mt Moon 1F Frlg Lv15; Scripted encounter at Seafoam Islands 1F Frlg Lv15 |
| 790 | **COSMOEM** | Evolve COSMOG |
| 791 | **SOLGALEO** | Evolve COSMOEM |
| 792 | **LUNALA** | Evolve COSMOEM |
| 793 | **NIHILEGO** | Rare grass at SEAFOAM ISLANDS 1F |
| 794 | **BUZZWOLE** | Supplemental grass at ROUTE 23 |
| 795 | **PHEROMOSA** | Supplemental grass at VIRIDIAN FOREST |
| 796 | **XURKITREE** | Rare grass at POWER PLANT |
| 797 | **CELESTEELA** | Supplemental grass at POWER PLANT |
| 798 | **KARTANA** | Rare grass at VIRIDIAN FOREST |
| 799 | **GUZZLORD** | Supplemental grass at POKEMON MANSION B 1F |
| 800 | **NECROZMA** | Scripted encounter at Cerulean Cave 2F Frlg Lv65 |
| 801 | **MAGEARNA** | Gift at Silph Co 7F Frlg Lv25 |
| 802 | **MARSHADOW** | Scripted encounter at Saffron City Dojo Frlg Lv38 |
| 803 | **POIPOLE** | Rare grass at ROUTE 1 |
| 804 | **NAGANADEL** | Evolve POIPOLE |
| 805 | **STAKATAKA** | Supplemental grass at ROCK TUNNEL B 1F |
| 806 | **BLACEPHALON** | Rare grass at POKEMON MANSION 1F |
| 807 | **ZERAORA** | Scripted encounter at Power Plant Frlg Lv45 |
| 808 | **MELTAN** | Scripted encounter at Vermilion City Frlg Lv15 |
| 809 | **MELMETAL** | Evolve MELTAN |

</details>

<details>
<summary><strong>Generation VIII · National #810–905</strong></summary>

| # | Pokémon | Source-candidate locations / method |
| ---: | --- | --- |
| 810 | **GROOKEY** | Rare grass at ROUTE 32 |
| 811 | **THWACKEY** | Evolve GROOKEY |
| 812 | **RILLABOOM** | Evolve THWACKEY |
| 813 | **SCORBUNNY** | Rare grass at ROUTE 34 |
| 814 | **RABOOT** | Evolve SCORBUNNY |
| 815 | **CINDERACE** | Evolve RABOOT |
| 816 | **SOBBLE** | Rare grass at ROUTE 35 |
| 817 | **DRIZZILE** | Evolve SOBBLE |
| 818 | **INTELEON** | Evolve DRIZZILE |
| 819 | **SKWOVET** | Supplemental grass at SPROUT TOWER 2F |
| 820 | **GREEDENT** | Evolve SKWOVET |
| 821 | **ROOKIDEE** | Supplemental grass at ROUTE 31 |
| 822 | **CORVISQUIRE** | Evolve ROOKIDEE |
| 823 | **CORVIKNIGHT** | Evolve CORVISQUIRE |
| 824 | **BLIPBUG** | Supplemental grass at RUINS OF ALPH B 1F |
| 825 | **DOTTLER** | Evolve BLIPBUG |
| 826 | **ORBEETLE** | Evolve DOTTLER |
| 827 | **NICKIT** | Rare grass at TIN TOWER 6F |
| 828 | **THIEVUL** | Evolve NICKIT |
| 829 | **GOSSIFLEUR** | Rare grass at ROUTE 47 |
| 830 | **ELDEGOSS** | Evolve GOSSIFLEUR |
| 831 | **WOOLOO** | Rare grass at ROUTE 38 |
| 832 | **DUBWOOL** | Evolve WOOLOO |
| 833 | **CHEWTLE** | Supplemental grass at ROUTE 29; Supplemental Surf at ROUTE 120; +1 other sources in machine catalog |
| 834 | **DREDNAW** | Rare grass at WHIRL ISLANDS B 3F; Evolve CHEWTLE |
| 835 | **YAMPER** | Rare grass at ROUTE 15; Supplemental grass at ROUTE 34 |
| 836 | **BOLTUND** | Evolve YAMPER |
| 837 | **ROLYCOLY** | Rare grass at UNION CAVE 1F |
| 838 | **CARKOL** | Evolve ROLYCOLY |
| 839 | **COALOSSAL** | Evolve CARKOL |
| 840 | **APPLIN** | Rare grass at ROUTE 37; Supplemental grass at ROUTE 119 |
| 841 | **FLAPPLE** | Evolve APPLIN |
| 842 | **APPLETUN** | Evolve APPLIN |
| 843 | **SILICOBRA** | Rare grass at MIRAGE TOWER 3F; Supplemental grass at ROUTE 46 |
| 844 | **SANDACONDA** | Rare grass at MIRAGE TOWER 4F; Evolve SILICOBRA |
| 845 | **CRAMORANT** | Supplemental grass at WHIRL ISLANDS B 2F; Supplemental Surf at ROUTE 121 |
| 846 | **ARROKUDA** | Rare grass at SEAFLOOR CAVERN ROOM 5; Supplemental fishing at ROUTE 119; +3 other sources in machine catalog |
| 847 | **BARRASKEWDA** | Evolve ARROKUDA |
| 848 | **TOXEL** | Rare grass at ROUTE 18; Supplemental grass at ROUTE 38 |
| 849 | **TOXTRICITY** | Evolve TOXEL |
| 850 | **SIZZLIPEDE** | Rare grass at JAGGED PASS; Supplemental grass at ROUTE 37 |
| 851 | **CENTISKORCH** | Evolve SIZZLIPEDE |
| 852 | **CLOBBOPUS** | Rare grass at SHOAL CAVE LOW TIDE ENTRANCE ROOM; Rare grass at SLOWPOKE WELL B 2F; +4 other sources in machine catalog |
| 853 | **GRAPPLOCT** | Evolve CLOBBOPUS |
| 854 | **SINISTEA** | Rare grass at POKEMON TOWER 5F; Supplemental grass at BURNED TOWER 1F [SINISTEA ANTIQUE] |
| 855 | **POLTEAGEIST** | Grass at BURNED TOWER B 1F [POLTEAGEIST PHONY]; Evolve SINISTEA |
| 856 | **HATENNA** | Rare grass at TIN TOWER 3F |
| 857 | **HATTREM** | Rare grass at TIN TOWER 5F; Evolve HATENNA |
| 858 | **HATTERENE** | Evolve HATTREM |
| 859 | **IMPIDIMP** | Rare grass at ROUTE 120; Supplemental grass at ILEX FOREST |
| 860 | **MORGREM** | Evolve IMPIDIMP |
| 861 | **GRIMMSNARL** | Evolve MORGREM |
| 862 | **OBSTAGOON** | Evolve LINOONE |
| 863 | **PERRSERKER** | Evolve MEOWTH |
| 864 | **CURSOLA** | Evolve CORSOLA |
| 865 | **SIRFETCHD** | Evolve FARFETCHD |
| 866 | **MR RIME** | Evolve MR MIME |
| 867 | **RUNERIGUS** | Rare grass at RUINS OF ALPH OUTSIDE; Evolve YAMASK |
| 868 | **MILCERY** | Rare grass at ROUTE 36 |
| 869 | **ALCREMIE** | Evolve MILCERY |
| 870 | **FALINKS** | Supplemental grass at MT MORTAR 2F |
| 871 | **PINCURCHIN** | Supplemental grass at ROUTE 44 |
| 872 | **SNOM** | Rare grass at SEAFOAM ISLANDS B 4F; Rare grass at SHOAL CAVE LOW TIDE ICE ROOM; +1 other sources in machine catalog |
| 873 | **FROSMOTH** | Evolve SNOM |
| 874 | **STONJOURNER** | Supplemental grass at DARK CAVE SOUTH SIDE |
| 875 | **EISCUE** | Rare grass at ICE PATH B 4F |
| 876 | **INDEEDEE** | Rare grass at TIN TOWER 7F |
| 877 | **MORPEKO** | Grass at ROUTE 39 [MORPEKO FULL BELLY]; Supplemental grass at ROUTE 37 [MORPEKO FULL BELLY]; +1 other sources in machine catalog |
| 878 | **CUFANT** | Supplemental grass at WHIRL ISLANDS B 3F |
| 879 | **COPPERAJAH** | Evolve CUFANT |
| 880 | **DRACOZOLT** | Supplemental grass at RUINS OF ALPH B 1F |
| 881 | **ARCTOZOLT** | Supplemental grass at RUINS OF ALPH B 1F |
| 882 | **DRACOVISH** | Supplemental grass at RUINS OF ALPH OUTSIDE |
| 883 | **ARCTOVISH** | Supplemental grass at RUINS OF ALPH OUTSIDE |
| 884 | **DURALUDON** | Rare grass at SKY PILLAR 3F; Supplemental grass at ROUTE 46 |
| 885 | **DREEPY** | Rare grass at CERULEAN CAVE 2F; Supplemental grass at DARK CAVE NORTH SIDE |
| 886 | **DRAKLOAK** | Evolve DREEPY |
| 887 | **DRAGAPULT** | Evolve DRAKLOAK |
| 888 | **ZACIAN** | Defeat Royal Duo at Route 27 Lv50 |
| 889 | **ZAMAZENTA** | Defeat Royal Duo at Route 27 Lv50 |
| 890 | **ETERNATUS** | Scripted encounter at Route 44 Lv50 |
| 891 | **KUBFU** | Scripted encounter at Route 34 Lv18; Scripted encounter at Sprout Tower 2F Lv12 |
| 892 | **URSHIFU** | Evolve KUBFU |
| 893 | **ZARUDE** | Scripted event at Ilex Forest |
| 894 | **REGIELEKI** | Scripted encounter at Olivine City Lighthouse Lv37 |
| 895 | **REGIDRAGO** | Scripted encounter at Dragons Den Cavern Lv46 |
| 896 | **GLASTRIER** | Scripted encounter at Ice Path B 2F Lv42; Scripted encounter at Shoal Cave Low Tide Ice Room Lv45 |
| 897 | **SPECTRIER** | Scripted encounter at Cliff Edge Cave Lv34 |
| 898 | **CALYREX** | Scripted encounter at Route 36 Lv45 |
| 899 | **WYRDEER** | Evolve STANTLER |
| 900 | **KLEAVOR** | Supplemental grass at DARK CAVE NORTH SIDE; Evolve SCYTHER |
| 901 | **URSALUNA** | Evolve URSARING |
| 902 | **BASCULEGION** | Evolve BASCULIN |
| 903 | **SNEASLER** | Evolve SNEASEL |
| 904 | **OVERQWIL** | Evolve QWILFISH |
| 905 | **ENAMORUS** | Scripted encounter at Route 39 Lv43 |

</details>

<details>
<summary><strong>Generation IX · National #906–1025</strong></summary>

| # | Pokémon | Source-candidate locations / method |
| ---: | --- | --- |
| 906 | **SPRIGATITO** | Rare grass at PETALBURG WOODS |
| 907 | **FLORAGATO** | Evolve SPRIGATITO |
| 908 | **MEOWSCARADA** | Evolve FLORAGATO |
| 909 | **FUECOCO** | Supplemental grass at FIERY PATH |
| 910 | **CROCALOR** | Evolve FUECOCO |
| 911 | **SKELEDIRGE** | Evolve CROCALOR |
| 912 | **QUAXLY** | Supplemental grass at ROUTE 102 |
| 913 | **QUAXWELL** | Evolve QUAXLY |
| 914 | **QUAQUAVAL** | Evolve QUAXWELL |
| 915 | **LECHONK** | Rare grass at ROUTE 22; Supplemental grass at ROUTE 101 |
| 916 | **OINKOLOGNE** | Evolve LECHONK |
| 917 | **TAROUNTULA** | Supplemental grass at ROUTE 116 |
| 918 | **SPIDOPS** | Evolve TAROUNTULA |
| 919 | **NYMBLE** | Supplemental grass at PETALBURG WOODS |
| 920 | **LOKIX** | Evolve NYMBLE |
| 921 | **PAWMI** | Rare grass at ROUTE 11; Supplemental grass at ROUTE 103 |
| 922 | **PAWMO** | Evolve PAWMI |
| 923 | **PAWMOT** | Evolve PAWMO |
| 924 | **TANDEMAUS** | Supplemental grass at ROUTE 101 |
| 925 | **MAUSHOLD** | Evolve TANDEMAUS |
| 926 | **FIDOUGH** | Supplemental grass at ROUTE 120 |
| 927 | **DACHSBUN** | Evolve FIDOUGH |
| 928 | **SMOLIV** | Rare grass at ROUTE 25; Supplemental grass at ROUTE 118 |
| 929 | **DOLLIV** | Evolve SMOLIV |
| 930 | **ARBOLIVA** | Evolve DOLLIV |
| 931 | **SQUAWKABILLY** | Grass at ROUTE 118 [SQUAWKABILLY GREEN]; Supplemental grass at ROUTE 101 [SQUAWKABILLY GREEN]; +1 other sources in machine catalog |
| 932 | **NACLI** | Supplemental grass at GRANITE CAVE STEVENS ROOM |
| 933 | **NACLSTACK** | Evolve NACLI |
| 934 | **GARGANACL** | Evolve NACLSTACK |
| 935 | **CHARCADET** | Rare grass at POKEMON MANSION 3F; Supplemental grass at FIERY PATH |
| 936 | **ARMAROUGE** | Evolve CHARCADET |
| 937 | **CERULEDGE** | Evolve CHARCADET |
| 938 | **TADBULB** | Rare grass at ROUTE 10; Supplemental Surf at ROUTE 102 |
| 939 | **BELLIBOLT** | Evolve TADBULB |
| 940 | **WATTREL** | Rare grass at ROUTE 115 |
| 941 | **KILOWATTREL** | Evolve WATTREL |
| 942 | **MASCHIFF** | Supplemental grass at ROUTE 103 |
| 943 | **MABOSSTIFF** | Evolve MASCHIFF |
| 944 | **SHROODLE** | Supplemental grass at ROUTE 121 |
| 945 | **GRAFAIAI** | Evolve SHROODLE |
| 946 | **BRAMBLIN** | Supplemental grass at ROUTE 123 |
| 947 | **BRAMBLEGHAST** | Evolve BRAMBLIN |
| 948 | **TOEDSCOOL** | Supplemental grass at ROUTE 111 |
| 949 | **TOEDSCRUEL** | Evolve TOEDSCOOL |
| 950 | **KLAWF** | Supplemental grass at METEOR FALLS STEVENS CAVE; Supplemental grass at ROUTE 115 |
| 951 | **CAPSAKID** | Rare grass at ROUTE 123 |
| 952 | **SCOVILLAIN** | Evolve CAPSAKID |
| 953 | **RELLOR** | Supplemental grass at MIRAGE TOWER 1F |
| 954 | **RABSCA** | Rare grass at TIN TOWER 9F; Evolve RELLOR |
| 955 | **FLITTLE** | Rare grass at MT PYRE 2F |
| 956 | **ESPATHRA** | Evolve FLITTLE |
| 957 | **TINKATINK** | Rare grass at UNION CAVE B 1F; Supplemental grass at GRANITE CAVE STEVENS ROOM |
| 958 | **TINKATUFF** | Evolve TINKATINK |
| 959 | **TINKATON** | Evolve TINKATUFF |
| 960 | **WIGLETT** | Rare grass at SEAFLOOR CAVERN ROOM 7; Supplemental fishing at ROUTE 110; +1 other sources in machine catalog |
| 961 | **WUGTRIO** | Evolve WIGLETT |
| 962 | **BOMBIRDIER** | Supplemental grass at ROUTE 119; Supplemental grass at SKY PILLAR 1F |
| 963 | **FINIZEN** | Supplemental grass at MT PYRE EXTERIOR; Supplemental Surf at OLIVINE CITY; +1 other sources in machine catalog |
| 964 | **PALAFIN** | Evolve FINIZEN |
| 965 | **VAROOM** | Rare grass at ROUTE 17; Supplemental grass at NEW MAUVILLE ENTRANCE |
| 966 | **REVAVROOM** | Evolve VAROOM |
| 967 | **CYCLIZAR** | Supplemental grass at METEOR FALLS B 1F 2R |
| 968 | **ORTHWORM** | Supplemental grass at VICTORY ROAD 1F |
| 969 | **GLIMMET** | Rare grass at MT MOON B 2F; Supplemental grass at FIERY PATH |
| 970 | **GLIMMORA** | Evolve GLIMMET |
| 971 | **GREAVARD** | Rare grass at POKEMON TOWER 4F; Supplemental grass at MT PYRE 2F |
| 972 | **HOUNDSTONE** | Evolve GREAVARD |
| 973 | **FLAMIGO** | Supplemental grass at METEOR FALLS 1F 1R |
| 974 | **CETODDLE** | Rare grass at SEAFOAM ISLANDS B 3F; Supplemental grass at SHOAL CAVE LOW TIDE ICE ROOM |
| 975 | **CETITAN** | Evolve CETODDLE |
| 976 | **VELUZA** | Supplemental fishing at ROUTE 108; Supplemental grass at SHOAL CAVE LOW TIDE ICE ROOM; +2 other sources in machine catalog |
| 977 | **DONDOZO** | Supplemental grass at SHOAL CAVE LOW TIDE INNER ROOM; Supplemental Surf at ROUTE 43 |
| 978 | **TATSUGIRI** | Rare grass at SEAFLOOR CAVERN ROOM 6; Supplemental fishing at ROUTE 109; +1 other sources in machine catalog |
| 979 | **ANNIHILAPE** | Evolve PRIMEAPE |
| 980 | **CLODSIRE** | Evolve WOOPER |
| 981 | **FARIGIRAF** | Evolve GIRAFARIG |
| 982 | **DUDUNSPARCE** | Evolve DUNSPARCE |
| 983 | **KINGAMBIT** | Rare grass at MT SILVER 3F; Supplemental grass at GRANITE CAVE B 2F; Evolve BISHARP |
| 984 | **GREAT TUSK** | Rare grass at ROUTE 111 |
| 985 | **SCREAM TAIL** | Supplemental grass at VICTORY ROAD B 2F |
| 986 | **BRUTE BONNET** | Supplemental grass at ROUTE 119 |
| 987 | **FLUTTER MANE** | Rare grass at MT PYRE EXTERIOR |
| 988 | **SLITHER WING** | Supplemental grass at METEOR FALLS 1F 2R |
| 989 | **SANDY SHOCKS** | Supplemental grass at ROUTE 111; Supplemental grass at SKY PILLAR 3F |
| 990 | **IRON TREADS** | Supplemental grass at ROUTE 110; Supplemental grass at SKY PILLAR 5F |
| 991 | **IRON BUNDLE** | Supplemental grass at SHOAL CAVE LOW TIDE ENTRANCE ROOM |
| 992 | **IRON HANDS** | Supplemental grass at METEOR FALLS B 1F 1R |
| 993 | **IRON JUGULIS** | Supplemental grass at SEAFLOOR CAVERN ROOM 1 |
| 994 | **IRON MOTH** | Supplemental grass at SEAFLOOR CAVERN ROOM 2 |
| 995 | **IRON THORNS** | Supplemental grass at SEAFLOOR CAVERN ROOM 3 |
| 996 | **FRIGIBAX** | Rare grass at SEAFOAM ISLANDS B 1F; Supplemental grass at SHOAL CAVE LOW TIDE STAIRS ROOM |
| 997 | **ARCTIBAX** | Rare grass at ICE PATH B 3F; Evolve FRIGIBAX |
| 998 | **BAXCALIBUR** | Rare grass at MT SILVER 2F; Supplemental grass at SHOAL CAVE LOW TIDE ICE ROOM; Evolve ARCTIBAX |
| 999 | **GIMMIGHOUL** | Rare grass at POKEMON MANSION B 1F; Supplemental grass at NEW MAUVILLE INSIDE |
| 1000 | **GHOLDENGO** | Evolve GIMMIGHOUL |
| 1001 | **WO CHIEN** | Scripted event at Route 120 |
| 1002 | **CHIEN PAO** | Scripted encounter at Route 105 Lv33 |
| 1003 | **TING LU** | Scripted encounter at Route 111 Lv34 |
| 1004 | **CHI YU** | Scripted encounter at Sealed Chamber Outer Room Lv45 |
| 1005 | **ROARING MOON** | Rare grass at SKY PILLAR 5F; Supplemental grass at METEOR FALLS 1F 1R |
| 1006 | **IRON VALIANT** | Rare grass at VICTORY ROAD 1F |
| 1007 | **KORAIDON** | Scripted encounter at Route 128 Ancient Grotto Lv45 |
| 1008 | **MIRAIDON** | Scripted encounter at New Mauville Generator Annex Lv45 |
| 1009 | **WALKING WAKE** | Scripted encounter at Route 125 Lv50 |
| 1010 | **IRON LEAVES** | Scripted encounter at Route 120 Lv50 |
| 1011 | **DIPPLIN** | Evolve APPLIN |
| 1012 | **POLTCHAGEIST** | Rare grass at MT PYRE 5F; Supplemental grass at MT PYRE 2F [POLTCHAGEIST COUNTERFEIT] |
| 1013 | **SINISTCHA** | Grass at MT PYRE 2F [SINISTCHA UNREMARKABLE]; Evolve POLTCHAGEIST |
| 1014 | **OKIDOGI** | Scripted encounter at Route 112 Lv27 |
| 1015 | **MUNKIDORI** | Scripted encounter at Route 114 Lv31 |
| 1016 | **FEZANDIPITI** | Scripted encounter at Route 113 Lv29 |
| 1017 | **OGERPON** | Scripted encounter at Route 117 Lv20 [OGERPON TEAL] |
| 1018 | **ARCHALUDON** | Supplemental grass at METEOR FALLS B 1F 1R; Evolve DURALUDON |
| 1019 | **HYDRAPPLE** | Evolve DIPPLIN |
| 1020 | **GOUGING FIRE** | Scripted encounter at Jagged Pass Lv50 |
| 1021 | **RAGING BOLT** | Scripted encounter at Route 119 Lv50 |
| 1022 | **IRON BOULDER** | Scripted encounter at Route 118 Lv50 |
| 1023 | **IRON CROWN** | Scripted encounter at Route 124 Lv50 |
| 1024 | **TERAPAGOS** | Permanent Stellar at Cave Of Origin B 1F Lv75 [TERAPAGOS STELLAR]; Scripted encounter at Cave Of Origin B 1F Lv75 [TERAPAGOS STELLAR]; +1 other sources in machine catalog |
| 1025 | **PECHARUNT** | Scripted encounter at Route 117 Lv36 |

</details>

## Browse by location
Want everything recorded for a route, cave or building? Open a region below. Wild lists combine the native encounter tables and supplemental/rare habitat paths; the scripted column includes source candidates, not necessarily all capture-ready gifts. Some FRLG/HGSS map variants are shown separately, and a map's story/access gates must still be checked.

<details>
<summary><strong>Kanto · 115 locations</strong></summary>

| Area | Wild / rare Pokémon | Scripted encounters / gifts | Indexed items |
| --- | --- | --- | --- |
| **CELADON CITY** | GRIMER, KOFFING, MAGIKARP, PSYDUCK, SLOWPOKE | — | — |
| **Celadon City Condominiums Roof Room Frlg** | — | EEVEE | — |
| **Celadon City Frlg** | — | TAPU LELE | VICTREEBELITE |
| **Celadon City Restaurant Frlg** | — | — | COIN CASE |
| **CERULEAN CAVE 1F** | AXEW, DITTO, ELECTRODE, GEODUDE, GOLBAT, GOLDEEN, GOLDUCK, GRAVELER, GYARADOS, MACHOKE, MAGIKARP, MAGNETON, PARASECT, POLIWAG, POLIWHIRL, PRIMEAPE, PSYDUCK, SLOWBRO, SLOWPOKE, WOBBUFFET | — | — |
| **CERULEAN CAVE 2F** | DITTO, DREEPY, ELECTRODE, GEODUDE, GOLBAT, GRAVELER, KADABRA, MACHOKE, MAGNETON, PARASECT, WOBBUFFET | — | — |
| **Cerulean Cave 2F Frlg** | — | NECROZMA | — |
| **CERULEAN CAVE B 1F** | DITTO, DRUDDIGON, ELECTRODE, GEODUDE, GOLBAT, GOLDEEN, GOLDUCK, GRAVELER, GYARADOS, KADABRA, MACHOKE, MAGIKARP, MAGNETON, PARASECT, POLIWAG, POLIWHIRL, PSYDUCK, SLOWBRO, SLOWPOKE, WOBBUFFET | — | — |
| **Cerulean Cave B 1F Frlg** | — | MEWTWO | — |
| **CERULEAN CITY** | GYARADOS, HORSEA, KRABBY, MAGIKARP, PSYDUCK, SLOWPOKE, TENTACOOL | — | — |
| **CINNABAR ISLAND** | GYARADOS, HORSEA, KRABBY, MAGIKARP, PSYDUCK, SEADRA, SHELLDER, SLOWBRO, SLOWPOKE, STARYU, TENTACOOL | — | — |
| **Cinnabar Island Pokemon Lab Experiment Room Frlg** | — | AERODACTYL, KABUTO, OMANYTE | — |
| **DIGLETTS CAVE B 1F** | DIGLETT, DRILBUR, DUGTRIO, HIPPOPOTAS | — | — |
| **FUCHSIA CITY** | BRUXISH, GOLDEEN, GYARADOS, MAGIKARP, POLIWAG, PSYDUCK, SEAKING, SLOWPOKE | — | — |
| **Fuchsia City House 2 Frlg** | — | — | GOOD ROD |
| **Indigo Plateau Exterior Frlg** | — | ARCEUS | — |
| **JOHTO VICTORY ROAD 1F** | DONPHAN, GOLBAT, GRAVELER, MACHOKE, ONIX, RHYHORN, SANDSLASH, URSARING | — | MAX ELIXIR, PP UP |
| **MT EMBER EXTERIOR** | FEAROW, GEODUDE, GRAVELER, MACHOP, MAGMAR, PONYTA, RAPIDASH, SPEAROW | — | — |
| **MT EMBER RUBY PATH 1F** | GEODUDE, GRAVELER, MACHOKE, MACHOP | — | — |
| **MT EMBER RUBY PATH B 1F** | GEODUDE, GRAVELER, SLUGMA | — | — |
| **MT EMBER RUBY PATH B 1F STAIRS** | GEODUDE, GRAVELER, SLUGMA | — | — |
| **MT EMBER RUBY PATH B 2F** | GEODUDE, GRAVELER, SLUGMA | — | — |
| **MT EMBER RUBY PATH B 2F STAIRS** | GEODUDE, GRAVELER, SLUGMA | — | — |
| **MT EMBER RUBY PATH B 3F** | MAGCARGO, SLUGMA | — | — |
| **Mt Ember Ruby Path B 3F Frlg** | — | — | RUBY |
| **Mt Ember Summit Frlg** | — | MOLTRES | — |
| **MT EMBER SUMMIT PATH 1F** | GEODUDE, MACHOP | — | — |
| **MT EMBER SUMMIT PATH 2F** | GEODUDE, GRAVELER, MACHOKE, MACHOP | — | — |
| **MT EMBER SUMMIT PATH 3F** | GEODUDE, MACHOP | — | — |
| **MT MOON 1F** | CHINGLING, CLEFAIRY, GEODUDE, PARAS, PIPLUP, SKORUPI, ZUBAT | — | — |
| **Mt Moon 1F Frlg** | — | COSMOG | CLEFABLITE |
| **MT MOON B 1F** | CARBINK, CRANIDOS, GRUBBIN, MINIOR, PARAS | — | — |
| **Mt Moon B 1F Frlg** | — | CRESSELIA, DIALGA, PALKIA | — |
| **MT MOON B 2F** | CLEFAIRY, GEODUDE, GLIMMET, KRICKETOT, PARAS, SHIELDON, ZUBAT | — | — |
| **Mt Moon B 2F Frlg** | — | GIRATINA | DOME FOSSIL, HELIX FOSSIL |
| **PALLET TOWN** | GYARADOS, HORSEA, KINGLER, KRABBY, MAGIKARP, PSYDUCK, SEADRA, SHELLDER, SLOWPOKE, STARYU, TENTACOOL | — | — |
| **POKEMON MANSION 1F** | BLACEPHALON, GRIMER, GROWLITHE, KOFFING, MUK, RATICATE, RATTATA, VULPIX, WEEZING | — | — |
| **Pokemon Mansion 1F Frlg** | — | MEW, TYPE NULL | — |
| **POKEMON MANSION 2F** | GRIMER, GROWLITHE, KOFFING, MUK, RATICATE, RATTATA, ROTOM, VULPIX, WEEZING | — | — |
| **POKEMON MANSION 3F** | CHARCADET, GRIMER, GROWLITHE, KOFFING, MUK, RATICATE, RATTATA, SALANDIT, VULPIX, WEEZING | — | — |
| **POKEMON MANSION B 1F** | DITTO, GIMMIGHOUL, GRIMER, GROWLITHE, GUZZLORD, KOFFING, MUK, RATICATE, RATTATA, TURTONATOR, VULPIX, WEEZING | — | — |
| **POKEMON TOWER 3F** | CUBONE, GASTLY, HAUNTER, LITWICK, MAROWAK, MUDBRAY | — | — |
| **POKEMON TOWER 4F** | COMFEY, CUBONE, GASTLY, GREAVARD, HAUNTER, MIMIKYU, MISDREAVUS | — | — |
| **Pokemon Tower 4F Frlg** | — | — | CHANDELURITE |
| **POKEMON TOWER 5F** | CUBONE, GASTLY, HAUNTER, SINISTEA, SPIRITOMB, WEEZING | — | — |
| **POKEMON TOWER 6F** | CUBONE, DRIFLOON, DUSKNOIR, GASTLY, HAUNTER, HONEDGE, MURKROW | — | — |
| **Pokemon Tower 6F Frlg** | — | MAROWAK | — |
| **POKEMON TOWER 7F** | CUBONE, GASTLY, HAUNTER, PHANTUMP, STUNKY | — | — |
| **Pokemon Tower 7F Frlg** | — | DARKRAI | DARKRANITE |
| **POWER PLANT** | CELESTEELA, ELECTABUZZ, MAGNEMITE, MAGNETON, PACHIRISU, PIKACHU, VOLTORB, XURKITREE | — | — |
| **Power Plant Frlg** | — | ELECTRODE, TAPU KOKO, ZAPDOS, ZERAORA | ZERAORITE |
| **ROCK TUNNEL 1F** | CROAGUNK, GEODUDE, GLISCOR, MACHOP, MANKEY, ONIX, ROGGENROLA, ZUBAT | — | — |
| **ROCK TUNNEL B 1F** | BONSLY, GEODUDE, GRAVELER, MACHOP, MANKEY, NOIBAT, ONIX, PROBOPASS, STAKATAKA, ZUBAT | — | — |
| **Rock Tunnel B 1F Frlg** | — | AZELF, HEATRAN, MESPRIT, REGIGIGAS, UXIE | HEATRANITE |
| **ROUTE 1** | BULBASAUR, CHARMANDER, LITTEN, PIDGEY, POIPOLE, RATTATA, SQUIRTLE | — | — |
| **ROUTE 10** | DRATINI, EKANS, GYARADOS, HAPPINY, HORSEA, KRABBY, LOTAD, MAGIKARP, PSYDUCK, SANDSHREW, SLOWBRO, SLOWPOKE, SPEAROW, TADBULB, TENTACOOL, VOLTORB | — | — |
| **Route 10 Frlg** | — | — | LUCARIONITE |
| **ROUTE 11** | ARROKUDA, BRONZOR, DROWZEE, EKANS, GYARADOS, HORSEA, KRABBY, MAGIKARP, MEOWTH, PAWMI, PSYDUCK, SANDSHREW, SLOWPOKE, SPEAROW, TAUROS, TENTACOOL | — | — |
| **ROUTE 12** | BELLSPROUT, BOUNSWEET, DUCKLETT, FRILLISH, GLOOM, GYARADOS, HORSEA, KRABBY, MAGIKARP, MANTYKE, ODDISH, PIDGEY, PSYDUCK, SLOWPOKE, TAUROS, TENTACOOL, VENONAT, WEEPINBELL, YANMA | — | — |
| **Route 12 Fishing House Frlg** | — | — | NET BALL, SUPER ROD |
| **Route 12 Frlg** | — | SNORLAX | — |
| **ROUTE 13** | BELLSPROUT, DEERLING, DEWPIDER, DITTO, GLOOM, GYARADOS, HORSEA, KRABBY, LICKILICKY, LICKITUNG, MAGIKARP, ODDISH, PIDGEOTTO, PIDGEY, PSYDUCK, SANDYGAST, SLOWPOKE, TENTACOOL, VENONAT, WEEPINBELL | — | — |
| **ROUTE 14** | BELLSPROUT, DITTO, FARFETCHD, FOMANTIS, GLOOM, ODDISH, PIDGEOTTO, PIDGEY, TAUROS, VENONAT, WEEPINBELL | — | — |
| **ROUTE 15** | BELLSPROUT, DITTO, GLOOM, HITMONCHAN, ODDISH, PIDGEOTTO, PIDGEY, RAICHU, VENONAT, WEEPINBELL, YAMPER | — | — |
| **ROUTE 16** | AIPOM, DODUO, EEVEE, EXEGGUTOR, FLETCHINDER, RATICATE, RATTATA, SPEAROW | — | — |
| **Route 16 Frlg** | — | SNORLAX | STARAPTITE |
| **Route 16 House Frlg** | — | — | HM 02 |
| **ROUTE 17** | BURMY, DODUO, FEAROW, PORYGON, RATICATE, RATTATA, SPEAROW, VAROOM | — | — |
| **ROUTE 18** | BIDOOF, DODUO, FEAROW, RATICATE, RATTATA, SPEAROW, TOXEL | — | — |
| **ROUTE 19** | ALOMOMOLA, GYARADOS, HORSEA, KINGLER, KRABBY, MAGIKARP, PSYDUCK, SEADRA, SLOWPOKE, TENTACOOL, WIMPOD | — | — |
| **ROUTE 2** | CATERPIE, ORANGURU, PIDGEY, PIKIPEK, RATTATA, TURTWIG, WEEDLE | — | — |
| **Route 2 East Building Frlg** | — | — | HM 05 |
| **ROUTE 20** | FINNEON, GYARADOS, HORSEA, KINGLER, KRABBY, LAPRAS, MAGIKARP, PSYDUCK, SEADRA, SLOWPOKE, TENTACOOL | — | — |
| **Route 20 Frlg** | — | — | STARMINITE |
| **ROUTE 21 NORTH** | GYARADOS, HORSEA, KINGLER, KRABBY, MAGIKARP, PSYDUCK, SEADRA, SLOWPOKE, TANGELA, TENTACOOL | — | — |
| **ROUTE 21 SOUTH** | GYARADOS, HORSEA, KINGLER, KRABBY, MAGIKARP, PSYDUCK, SEADRA, SLOWPOKE, TANGELA, TENTACOOL | — | — |
| **ROUTE 22** | GOLDEEN, GYARADOS, LECHONK, MAGIKARP, MANKEY, POLIWAG, POLIWHIRL, PSYDUCK, RATTATA, SHELLOS, SLOWPOKE, SPEAROW, STUFFUL | — | — |
| **ROUTE 23** | ARBOK, BUZZWOLE, EKANS, FEAROW, FINIZEN, GOLDEEN, GYARADOS, HITMONLEE, KOMALA, MAGIKARP, MANKEY, PAWNIARD, POLIWAG, POLIWHIRL, PRIMEAPE, PSYDUCK, RIOLU, SANDSHREW, SANDSLASH, SLOWPOKE, SPEAROW | — | — |
| **Route 23 Frlg** | — | — | FALINKSITE |
| **ROUTE 24** | ABRA, BELLSPROUT, CATERPIE, DECIDUEYE, GYARADOS, HORSEA, KAKUNA, KRABBY, MAGIKARP, METAPOD, ODDISH, PETILIL, PIDGEY, PSYDUCK, ROWLET, SLOWPOKE, TENTACOOL, TOGEKISS, TYMPOLE, WEEDLE | — | — |
| **ROUTE 25** | ABRA, BELLSPROUT, CATERPIE, GALLADE, GOLDEEN, GYARADOS, KAKUNA, MAGIKARP, MAREANIE, METAPOD, MIME JR, ODDISH, PIDGEY, POLIWAG, POLIWHIRL, PSYDUCK, SLOWPOKE, SMOLIV, WEEDLE | — | — |
| **Route 25 Frlg** | — | SUICUNE | — |
| **ROUTE 3** | CHIMCHAR, COMBEE, JIGGLYPUFF, MANKEY, MEOWTH, NIDORAN F, NIDORAN M, PIDGEY, SPEAROW, YUNGOOS | — | — |
| **ROUTE 4** | BUIZEL, EKANS, GYARADOS, HORSEA, KRABBY, MAGIKARP, MANKEY, MUNCHLAX, PSYDUCK, RATTATA, SANDSHREW, SHINX, SLOWPOKE, SPEAROW, TENTACOOL | — | — |
| **Route 4 Pokemon Center 1F Frlg** | — | MAGIKARP | — |
| **ROUTE 5** | BELLSPROUT, BUNEARY, CHERUBI, CUTIEFLY, MEOWTH, ODDISH, PIDGEY | — | — |
| **Route 5 Day Care Frlg** | — | PHIONE | — |
| **Route 5 Frlg** | — | TAPU BULU | — |
| **ROUTE 6** | BELLSPROUT, BURMY, CARNIVINE, GOLDEEN, GYARADOS, MAGIKARP, MEOWTH, ODDISH, ORICORIO, PIDGEY, PIKACHU, POLIWAG, POLIWHIRL, PSYDUCK, SLOWPOKE, STARLY | — | — |
| **ROUTE 7** | BELLSPROUT, BUDEW, BURMY, GROWLITHE, MEOWTH, MOTHIM, ODDISH, PIDGEY, SKIDDO, VULPIX | — | — |
| **ROUTE 8** | EKANS, FARFETCHD, FLETCHLING, GLAMEOW, GROWLITHE, MEOWTH, PIDGEY, PONYTA, SANDSHREW, VULPIX | — | — |
| **ROUTE 9** | CHATOT, EKANS, GROWLITHE, RATTATA, ROCKRUFF, SANDSHREW, SPEAROW | — | — |
| **Saffron City Dojo Frlg** | — | MARSHADOW | HAWLUCHANITE |
| **Saffron City Frlg** | — | — | MEOWSTICITE |
| **SEAFOAM ISLANDS 1F** | GOLBAT, NIHILEGO, PSYDUCK, SLOWPOKE, SNOVER, ZUBAT | — | — |
| **Seafoam Islands 1F Frlg** | — | COSMOG | — |
| **SEAFOAM ISLANDS B 1F** | FRIGIBAX, GOLBAT, GOLDUCK, JYNX, PSYDUCK, SANDSHREW, SEEL, SLOWBRO, SLOWPOKE, VULPIX, ZUBAT | — | — |
| **SEAFOAM ISLANDS B 2F** | BERGMITE, FROSLASS, GLACEON, GOLBAT, GOLDUCK, POPPLIO, PSYDUCK, SEEL, SLOWBRO, SLOWPOKE, ZUBAT | — | — |
| **Seafoam Islands B 2F Frlg** | — | MANAPHY, TAPU FINI | — |
| **SEAFOAM ISLANDS B 3F** | CETODDLE, CRABRAWLER, DEWGONG, GOLBAT, GOLDUCK, GYARADOS, HORSEA, KRABBY, MAGIKARP, PSYDUCK, PYUKUMUKU, SEEL, SLOWBRO, SLOWPOKE, SNEASEL, ZUBAT | — | — |
| **Seafoam Islands B 3F Frlg** | — | — | ABOMASITE |
| **SEAFOAM ISLANDS B 4F** | DEWGONG, DHELMISE, GOLBAT, GOLDUCK, GYARADOS, HORSEA, KRABBY, MAGIKARP, PSYDUCK, SEEL, SLOWBRO, SLOWPOKE, SNOM, SWINUB, WISHIWASHI | — | — |
| **Seafoam Islands B 4F Frlg** | — | ARTICUNO | — |
| **Silph Co 7F Frlg** | — | MAGEARNA | MAGEARNITE |
| **VERMILION CITY** | CLAUNCHER, GYARADOS, HORSEA, KRABBY, MAGIKARP, PSYDUCK, SHELLDER, SLOWPOKE, STARYU, TENTACOOL | — | — |
| **Vermilion City Frlg** | — | MELTAN | — |
| **Vermilion City House 1 Frlg** | — | — | OLD ROD |
| **VICTORY ROAD 1F** | ARON, GOLBAT, HARIYAMA, IRON VALIANT, LAIRON, LOUDRED, MAKUHITA, ORTHWORM, WHISMUR, ZUBAT | — | — |
| **VICTORY ROAD 1F FRLG** | ARBOK, GEODUDE, GOLBAT, MACHOKE, MACHOP, MAROWAK, ONIX, SANDSLASH, ZUBAT | — | — |
| **VICTORY ROAD 2F** | ARBOK, DEINO, DRAMPA, GEODUDE, GOLBAT, MACHOKE, MACHOP, MAROWAK, ONIX, PASSIMIAN, PRIMEAPE, SANDSLASH, ZUBAT | — | — |
| **Victory Road 2F Frlg** | — | — | GLIMMORANITE |
| **VICTORY ROAD 3F** | ARBOK, GEODUDE, GIBLE, GOLBAT, JANGMO O, MACHOKE, MACHOP, MAROWAK, ONIX, SANDSLASH, ZUBAT | — | — |
| **VIRIDIAN CITY** | GOLDEEN, GYARADOS, MAGIKARP, POLIWAG, POLIWHIRL, PSYDUCK, SLOWPOKE | — | — |
| **VIRIDIAN FOREST** | BURMY, CATERPIE, KAKUNA, KARTANA, METAPOD, MORELULL, PHEROMOSA, PIKACHU, TOGEDEMARU, WEEDLE | — | — |
| **Viridian Forest Frlg** | — | SHAYMIN | — |

</details>

<details>
<summary><strong>Johto · 101 locations</strong></summary>

| Area | Wild / rare Pokémon | Scripted encounters / gifts | Indexed items |
| --- | --- | --- | --- |
| **BLACKTHORN CITY** | AIPOM, HERACROSS, MAGIKARP, PINECO, POLIWAG, POLIWHIRL | — | — |
| **BURNED TOWER 1F** | HOUNDOUR, KOFFING, LARVESTA, PUMPKABOO, PURRLOIN, RATICATE, RATTATA, SINISTEA, SLUGMA, YAMASK | MOLTRES | ABSOLITE Z, ETHER, HP UP |
| **BURNED TOWER B 1F** | DARUMAKA, KOFFING, LITWICK, MAGMAR, MIMIKYU, MISDREAVUS, POLTEAGEIST, TYPHLOSION, YAMASK, ZUBAT | — | ULTRA BALL |
| **CIANWOOD CITY** | CHINCHOU, GEODUDE, LANTURN, MAGIKARP, SHELLDER, SHUCKLE, TENTACOOL, TENTACRUEL, VELUZA | — | — |
| **Cianwood Gym** | — | — | SCROLL OF WATERS |
| **Cianwood Pokecenter** | — | TENTACOOL | — |
| **DARK CAVE NORTH SIDE** | DREEPY, GEODUDE, GOLBAT, GOLDEEN, GRAVELER, KLEAVOR, MAGIKARP, SANDILE, SEAKING, URSARING, WOOBAT, ZUBAT | ZEKROM | BLACK FLUTE, BLACK GLASSES, STAR PIECE |
| **DARK CAVE SOUTH SIDE** | DUNSPARCE, GEODUDE, GOLDEEN, LARVITAR, MAGIKARP, ROGGENROLA, SEAKING, STONJOURNER, TEDDIURSA, TIMBURR, ZUBAT | — | POTION, REVIVE |
| **DRAGONS DEN CAVERN** | DRAGONAIR, DRATINI, HORSEA, MAGIKARP, SEADRA | REGIDRAGO | DRAGON FANG, GARCHOMPITE, MAX POTION |
| **Dragons Den Shrine** | — | DRATINI | — |
| **ECRUTEAK CITY** | EXEGGCUTE, LEDYBA, MAGIKARP, PINECO, POLIWAG, POLIWHIRL, SPINARAK | — | RARE CANDY |
| **Ecruteak City Theater** | — | MELOETTA | — |
| **Gate National Park** | — | — | SAFARI BALL |
| **GOLDENROD CITY** | EXEGGCUTE, LEDYBA, PINECO, SPINARAK | GENESECT | — |
| **Goldenrod City Bike Shop** | — | — | MACH BIKE |
| **Goldenrod City Department Store Basement** | — | — | AMULET COIN, ETHER |
| **Goldenrod City Flower Shop** | — | — | FLOETTITE, WAILMER PAIL |
| **Goldenrod City Underground Storage** | — | — | ULTRA BALL |
| **Goldenrod City Underground Tunnel** | — | — | COIN CASE, SCRAFTINITE, SUPER POTION |
| **ICE PATH 1F** | DARUMAKA, DELIBIRD, GOLBAT, SNOM, SWINUB, VANILLITE, ZUBAT | — | PP UP, PROTEIN |
| **ICE PATH B 1F** | CUBCHOO, GOLBAT, JYNX, SMOOCHUM, SNEASEL, SWINUB | KYUREM | DNA SPLICERS, FROSLASSITE, IRON, REVIVE |
| **ICE PATH B 2F** | CRYOGONAL, GOLBAT, JYNX, MR MIME, SCRAGGY, SNEASEL, SWINUB | GLASTRIER | FULL HEAL, MAX POTION |
| **ICE PATH B 3F** | ARCTIBAX, GOLBAT, JYNX, SNEASEL, SWINUB, ZORUA | — | — |
| **ICE PATH B 4F** | DEINO, EISCUE, GOLBAT, JYNX, SNEASEL, SWINUB, VULLABY | — | NEVER MELT ICE |
| **ILEX FOREST** | BEEDRILL, BUTTERFREE, CATERPIE, COTTONEE, EXEGGCUTE, GOLDUCK, IMPIDIMP, KAKUNA, MAGIKARP, MARACTUS, METAPOD, MORELULL, ODDISH, PARAS, PETILIL, PINECO, POLIWAG, POLIWHIRL, PSYDUCK, WEEDLE, ZORUA | CELEBI, ZARUDE | ETHER, SCOLIPITE, SUPER POTION |
| **LAKE OF RAGE** | EXEGGCUTE, GYARADOS, MAGIKARP, PINECO, VENONAT | SUICUNE | BLACK BELT, FULL RESTORE, MAX REVIVE |
| **Mahogany Hideout B 1F** | — | — | BIG NUGGET, HYPER POTION |
| **Mahogany Hideout B 2F** | — | ELECTRODE | — |
| **Mahogany Hideout B 3F** | — | — | ICE HEAL, PROTEIN |
| **MAHOGANYTOWN** | AIPOM, HERACROSS, PINECO | — | RAGE CANDY BAR |
| **MT MORTAR 1F NORTH** | CUBONE, GEODUDE, GOLBAT, GOLDEEN, MACHOP, MAGIKARP, MARILL, MEDITITE, RHYHORN, SEAKING, TYROGUE, ZUBAT | — | ESCAPE ROPE, MAX REPEL |
| **MT MORTAR 1F SOUTH** | CUBONE, GEODUDE, GOLBAT, GOLDEEN, MACHOP, MAGIKARP, MARILL, MIENFOO, RHYHORN, SEAKING, THROH, ZUBAT | COBALION | DRAMPANITE, ELIXIR, REVIVE |
| **MT MORTAR 2F** | FALINKS, GEODUDE, GOLBAT, GOLDEEN, GRAVELER, MACHOKE, MACHOP, MAGIKARP, MARILL, RATICATE, RIOLU, SEAKING | ENTEI | IRON, MAX REVIVE |
| **MT MORTAR B 1F** | CUBONE, GEODUDE, GOLBAT, GOLDEEN, GOLETT, MACHOP, MAGIKARP, MARILL, RHYHORN, SAWK, SEAKING, ZUBAT | TYROGUE | CARBOS, HYPER POTION, SCROLL OF DARKNESS |
| **MT SILVER 1F ITEM ROOM** | ELECTABUZZ, GOLBAT, GRAVELER, LARVITAR, MAGIKARP, MAGMAR, ONIX, POLIWAG, POLIWHIRL, PUPITAR, URSARING | — | — |
| **MT SILVER 1F MOLTRES ROOM** | ELECTABUZZ, GOLBAT, GOLDEEN, GOLDUCK, GRAVELER, MACHOKE, MAGIKARP, MAGMAR, ONIX, PARASECT, POLIWAG, POLIWHIRL, PUPITAR, SEAKING | MOLTRES | — |
| **MT SILVER 1F WATERFALL ROOM** | ELECTABUZZ, GOLBAT, GOLDUCK, GOLEM, LAPRAS, LARVITAR, MAGIKARP, MAGMAR, ONIX, PARASECT, POLIWAG, POLIWHIRL, PUPITAR, SEAKING, URSARING | — | DIRE HIT, MAX POTION |
| **MT SILVER 2F** | BAXCALIBUR, ELECTABUZZ, GOLBAT, GOLDUCK, GOLEM, LAPRAS, MACHOKE, MAGIKARP, MAGMAR, PARASECT, POLIWAG, POLIWHIRL, PUPITAR, QUAGSIRE | — | — |
| **MT SILVER 3F** | CROBAT, GOLBAT, GOLEM, KINGAMBIT, MACHOKE, ONIX, PUPITAR, URSARING | — | — |
| **MT SILVER MOUNTAIN SIDE** | AIPOM, FORRETRESS, GOLEM, HERACROSS, HOUNDOOM, MACHOKE, MAGIKARP, ONIX, POLIWAG, POLIWHIRL, RAPIDASH, SCYTHER, SKARMORY, URSARING | — | — |
| **MT SILVER OUTSIDE** | AIPOM, BLASTOISE, CHARIZARD, DONPHAN, FORRETRESS, GOLEM, HERACROSS, MAGIKARP, POLIWAG, POLIWHIRL, RAPIDASH, SCYTHER, SKARMORY, SNOVER, URSARING, VENUSAUR | — | — |
| **MT SILVER SNOW** | GLACEON, HOUNDOOM, MACHOKE, MISDREAVUS, PILOSWINE, RAPIDASH, SKARMORY, SNEASEL, URSARING | — | NEVER MELT ICE, RARE CANDY |
| **NATIONAL PARK BUG CONTEST** | BEEDRILL, BUTTERFREE, CATERPIE, KAKUNA, METAPOD, PARAS, PINSIR, SCYTHER, VENONAT, WEEDLE | — | — |
| **NATIONAL PARK NORMAL** | CATERPIE, COMBEE, NIDORAN F, NIDORAN M, SCYTHER, SUNKERN, VENIPEDE, WEEDLE | VICTINI, VIRIZION | SOOTHE BELL |
| **OLIVINE CITY** | CORSOLA, EXEGGCUTE, FINIZEN, KINGLER, KRABBY, LEDYBA, MAGIKARP, PINECO, SPINARAK, TENTACOOL, TENTACRUEL | — | — |
| **Olivine City House 3** | — | — | GOOD ROD |
| **Olivine City Lighthouse** | — | REGIELEKI | ETHER, RARE CANDY |
| **OLIVINE CITY PORT OUTSIDE** | CHINCHOU, LANTURN, MAGIKARP, SHELLDER, TENTACOOL, TENTACRUEL | — | — |
| **ROUTE 26** | AIPOM, ARBOK, BASCULIN, BULBASAUR, CHARMANDER, DODRIO, HERACROSS, MAGIKARP, PINECO, PONYTA, RAPIDASH, TENTACOOL, TENTACRUEL | — | — |
| **ROUTE 27** | ARBOK, CHIKORITA, CYNDAQUIL, DODRIO, EKANS, EXEGGCUTE, MAGIKARP, PINECO, PONYTA, QWILFISH, RAPIDASH, TENTACOOL, TENTACRUEL | ZACIAN, ZAMAZENTA | MOON STONE, RARE CANDY, RUSTED SHIELD, RUSTED SWORD |
| **ROUTE 28** | AIPOM, ARBOK, DODRIO, DODUO, FORRETRESS, HERACROSS, MAGIKARP, POLIWAG, POLIWHIRL, PONYTA, RAPIDASH, SCYTHER, TANGELA, WISHIWASHI | — | — |
| **ROUTE 29** | CHEWTLE, CHIKORITA, CYNDAQUIL, EXEGGCUTE, HOOTHOOT, HOPPIP, LEDYBA, MUNNA, PIDGEY, PINECO, RATTATA, SENTRET, SNIVY, SPINARAK, TOTODILE | — | POTION, SILK SCARF |
| **ROUTE 30** | CATERPIE, COTTONEE, EXEGGCUTE, LEDYBA, LOTAD, MAGIKARP, PIDGEY, PIDOVE, PINECO, POLIWAG, POLIWHIRL, RUFFLET, SPINARAK, TEPIG, WEEDLE, WOOPER, ZIGZAGOON | — | — |
| **ROUTE 31** | BELLSPROUT, CATERPIE, EXEGGCUTE, LEDYBA, MAGIKARP, MAREEP, OSHAWOTT, PICHU, PINECO, POLIWAG, POLIWHIRL, ROOKIDEE, SHELMET, SPINARAK, WEEDLE | — | ANTIDOTE, POKE BALL |
| **ROUTE 32** | BELLSPROUT, BLITZLE, DEWPIDER, EKANS, EMOLGA, EXEGGCUTE, GROOKEY, MAGIKARP, MAREEP, PIDGEY, PINECO, QUAGSIRE, QWILFISH, SHELMET, TENTACOOL, TENTACRUEL, WOOPER | KELDEO | GREAT BALL, POISON BARB, POTION |
| **Route 32 Pokemon Center** | — | — | OLD ROD |
| **ROUTE 33** | AIPOM, BOUFFALANT, EKANS, INKAY, MACHOP, PINECO, RATTATA, SPEAROW | — | — |
| **ROUTE 34** | ABRA, CORSOLA, DITTO, EXEGGCUTE, GOTHITA, GRIMER, JIGGLYPUFF, KINGLER, KRABBY, LEDYBA, MAGIKARP, MANKEY, PINECO, SCORBUNNY, SNUBBULL, SPINARAK, STARYU, TENTACOOL, TENTACRUEL, WIMPOD, YAMPER, ZIGZAGOON | KUBFU | LOPUNNITE, RARE CANDY, SOFT SAND |
| **ROUTE 35** | ABRA, COTTONEE, DUCKLETT, EXEGGCUTE, FARFETCHD, GOLDUCK, GROWLITHE, IGGLYBUFF, LEDYBA, MAGIKARP, MINCCINO, PIDGEY, PINECO, POLIWAG, POLIWHIRL, PSYDUCK, SNUBBULL, SOBBLE, SPINARAK, YANMA | — | BIG NUGGET |
| **ROUTE 36** | BELLSPROUT, CLEFFA, EXEGGCUTE, FERROSEED, GROWLITHE, LEDYBA, LILLIGANT, MILCERY, PIDGEY, PINECO, RATTATA, SPINARAK | CALYREX, SUDOWOODO | HARD STONE |
| **ROUTE 37** | APPLIN, DEERLING, EXEGGCUTE, GROWLITHE, LEDIAN, LEDYBA, MORPEKO, PANSEAR, PIDGEOTTO, PIDGEY, PINECO, SAWSBUCK, SIZZLIPEDE, SPINARAK, STANTLER, VULPIX | — | MAGNET |
| **ROUTE 38** | AUDINO, DODUO, EEVEE, EXEGGCUTE, LEDYBA, MAGNEMITE, MEOWTH, MILTANK, PINECO, RATICATE, SPINARAK, TAUROS, TOXEL, WOOLOO, ZIGZAGOON | RAIKOU, ZAPDOS | LAX INCENSE |
| **ROUTE 39** | DODUO, EXEGGCUTE, LEDYBA, MAGBY, MAGNEMITE, MAREANIE, MILTANK, MORPEKO, PINECO, PONYTA, PORYGON, RATICATE, SPINARAK, TAUROS | ENAMORUS | BIG NUGGET |
| **ROUTE 40** | CORSOLA, FRILLISH, KINGLER, KRABBY, MAGIKARP, MANTYKE, SHELLDER, STARYU, TENTACOOL, TENTACRUEL | ARTICUNO, LUGIA | SHARP BEAK |
| **ROUTE 41** | CHINCHOU, HORSEA, LANTURN, MAGIKARP, MANTINE, QWILFISH, SHELLDER, SKRELP, TENTACOOL, TENTACRUEL | TENTACRUEL, THUNDURUS | — |
| **ROUTE 42** | AIPOM, ARBOK, CHEWTLE, EKANS, FEAROW, FOONGUS, GLIGAR, GOLDEEN, HEATMOR, HERACROSS, MAGIKARP, MORPEKO, PINECO, RATICATE, REMORAID, SEAKING, TRUBBISH, TYMPOLE | — | MODEST MINT, SUPER POTION |
| **ROUTE 43** | ARROKUDA, BARBOACH, DONDOZO, ELEKID, EXEGGCUTE, FARFETCHD, FLAAFFY, FURRET, GIRAFARIG, MAGIKARP, MEOWTH, PANSAGE, PINECO, POLIWAG, RATICATE, SKARMORY, TYNAMO, VENONAT | LANDORUS | MAX ETHER |
| **ROUTE 44** | AIPOM, CHINGLING, CLAUNCHER, DEERLING, ELECTABUZZ, HERACROSS, LICKITUNG, MAGIKARP, PIDGEOTTO, PINCURCHIN, PINECO, POLIWAG, POLIWHIRL, SHELMET, TANGELA, WEEPINBELL, WISHIWASHI, WOBBUFFET | ETERNATUS, TORNADUS | MAX REPEL, ULTRA BALL |
| **ROUTE 45** | AIPOM, AXEW, DONPHAN, DRAGONAIR, DRATINI, DRUDDIGON, DURANT, FINNEON, GLIGAR, GRAVELER, HERACROSS, MAGIKARP, PAWNIARD, PINECO, SKARMORY, SNEASEL, STUNFISK, URSARING | — | ELIXIR, MAX POTION, SKARMORITE |
| **ROUTE 46** | AIPOM, DURALUDON, GEODUDE, HERACROSS, LILLIPUP, PHANPY, PINECO, RATTATA, SILICOBRA, SPEAROW | — | REVIVE |
| **ROUTE 47** | AIPOM, ALOMOMOLA, CHANSEY, CHINCHOU, CLOBBOPUS, DHELMISE, DITTO, FARFETCHD, FEAROW, GLOOM, GOSSIFLEUR, HERACROSS, LANTURN, MAGIKARP, MILTANK, PINECO, RATICATE, SAMUROTT, SEEL, SHELLDER, STARYU, TENTACOOL, TENTACRUEL | — | PEARL, REVIVE |
| **ROUTE 48** | AIPOM, BRAVIARY, FARFETCHD, FEAROW, GIRAFARIG, GLOOM, GROWLITHE, HERACROSS, PIKIPEK, PINECO, SKIPLOOM, TAUROS, VULPIX | — | — |
| **RUINS OF ALPH B 1F** | ARCTOZOLT, BLIPBUG, DRACOZOLT, ELGYEM, SIGILYPH, UNOWN | — | DOME FOSSIL, HELIX FOSSIL |
| **Ruins Of Alph Lab** | — | AERODACTYL, ANORITH, LILEEP | — |
| **RUINS OF ALPH OUTSIDE** | ARCHEN, ARCTOVISH, DRACOVISH, MAGIKARP, NATU, POLIWAG, POLIWHIRL, QUAGSIRE, RUNERIGUS, SMEARGLE, SOLOSIS, UNOWN, WOOPER | — | GOLURKITE |
| **Ruins Of Alph Puzzle And Reward Chambers** | — | — | CHARCOAL, HEAL POWDER |
| **SLOWPOKE WELL B 1F** | DEWPIDER, GOLDEEN, MAGIKARP, SEAKING, SLOWBRO, SLOWPOKE, TYMPOLE, ZUBAT | — | SUPER POTION |
| **SLOWPOKE WELL B 2F** | CLOBBOPUS, GOLBAT, GOLDEEN, MAGIKARP, SEAKING, SLOWBRO, SLOWPOKE, ZUBAT | — | KINGS ROCK |
| **Sprout Tower 1F** | — | — | PARALYZE HEAL |
| **SPROUT TOWER 2F** | JOLTIK, RATTATA, SEWADDLE, SKWOVET | KUBFU | — |
| **SPROUT TOWER 3F** | CUTIEFLY, GASTLY, RATTATA, TOGEPI | — | ESCAPE ROPE, POTION |
| **TIN TOWER 2F** | ESPURR, GASTLY, RATTATA | RESHIRAM | — |
| **TIN TOWER 3F** | GASTLY, HATENNA, RATTATA | — | FULL HEAL, ULTRA BALL |
| **TIN TOWER 4F** | GASTLY, ORICORIO, RATTATA | — | — |
| **TIN TOWER 5F** | GASTLY, HATTREM, RATTATA | — | — |
| **TIN TOWER 6F** | GASTLY, NICKIT, RATTATA | — | — |
| **TIN TOWER 7F** | GASTLY, INDEEDEE, RATTATA | — | — |
| **TIN TOWER 8F** | FLABEBE, GASTLY, RATTATA | — | — |
| **TIN TOWER 9F** | GASTLY, RABSCA, RATTATA | — | — |
| **Tin Tower Roof Day** | — | HO OH | — |
| **TOHJO FALLS CAVERN** | GOLBAT, GOLDEEN, MAGIKARP, RATICATE, RATTATA, SEAKING, SLOWPOKE, ZUBAT | — | HEART SCALE |
| **UNION CAVE 1F** | CUBONE, DRILBUR, DWEBBLE, GEODUDE, GOLDEEN, MAGIKARP, ONIX, QUAGSIRE, ROLYCOLY, SANDSHREW, SEAKING, WOOPER, ZUBAT | TERRAKION | EXCADRITE, GREAT BALL, POTION |
| **UNION CAVE B 1F** | GEODUDE, GOLDEEN, KLINK, MAGIKARP, MARILL, ONIX, PANPOUR, QUAGSIRE, SEAKING, TINKATINK, WOOPER, ZUBAT | — | — |
| **UNION CAVE B 2F** | CHINCHOU, CUBONE, GOLBAT, KANGASKHAN, KARRABLAST, LANTURN, MAGIKARP, ONIX, PATRAT, QUAGSIRE, RATICATE, SHELLDER, TENTACOOL, TENTACRUEL | LAPRAS | ETHER, HYPER POTION |
| **WHIRL ISLANDS 1F** | CORSOLA, FRILLISH, HORSEA, KINGLER, KRABBY, MAGIKARP, MANTINE, NATU, PIDGEOTTO, SEADRA, SEEL, SLOWKING, STARYU, TENTACOOL, TIRTOUGA | — | GOLISOPITE, RARE CANDY, ULTRA BALL |
| **WHIRL ISLANDS B 1F** | CORSOLA, HORSEA, KINGLER, KRABBY, MAGIKARP, MANTINE, NATU, PIDGEOTTO, SEADRA, SEEL, STARYU, TENTACOOL, WIMPOD | — | BIG NUGGET, FULL RESTORE |
| **WHIRL ISLANDS B 1F INNER** | CORSOLA, DHELMISE, GOLBAT, KRABBY, NATU, SEEL | — | — |
| **WHIRL ISLANDS B 2F** | CRAMORANT, DEWGONG, DROWZEE, GOLBAT, GOLDUCK, HORSEA, KINGLER, KRABBY, MAGIKARP, MANTYKE, SEADRA, SEEL, SKARMORY, SLOWBRO, TENTACRUEL | — | — |
| **WHIRL ISLANDS B 3F** | CUFANT, DEWGONG, DREDNAW, DROWZEE, GOLBAT, GOLDUCK, HORSEA, KINGLER, KRABBY, MAGIKARP, SEADRA, SEEL, SKARMORY, SLOWBRO, TENTACRUEL | — | — |
| **WHIRL ISLANDS DESCENT** | DEWGONG, GOLBAT, HORSEA, KABUTO, KINGLER, KRABBY, MAGIKARP, OMANYTE, SEADRA, SEEL, TENTACRUEL, XATU | — | FULL RESTORE, MAX REVIVE |
| **Whirl Islands Lugia Chamber** | — | LUGIA | — |

</details>

<details>
<summary><strong>Hoenn · 115 locations</strong></summary>

| Area | Wild / rare Pokémon | Scripted encounters / gifts | Indexed items |
| --- | --- | --- | --- |
| **ABANDONED SHIP HIDDEN FLOOR CORRIDORS** | MAGIKARP, TENTACOOL, TENTACRUEL | — | — |
| **Abandoned Ship Rooms 1F** | — | — | MALAMARITE |
| **ABANDONED SHIP ROOMS B 1F** | MAGIKARP, TENTACOOL, TENTACRUEL | — | — |
| **CAVE OF ORIGIN 1F** | GOLBAT, SABLEYE, ZUBAT | — | — |
| **Cave Of Origin B 1F** | — | TERAPAGOS | — |
| **CAVE OF ORIGIN ENTRANCE** | GOLBAT, ZUBAT | — | — |
| **CAVE OF ORIGIN UNUSED RUBY SAPPHIRE MAP 1** | GOLBAT, SABLEYE, ZUBAT | — | — |
| **CAVE OF ORIGIN UNUSED RUBY SAPPHIRE MAP 2** | GOLBAT, SABLEYE, ZUBAT | — | — |
| **CAVE OF ORIGIN UNUSED RUBY SAPPHIRE MAP 3** | GOLBAT, SABLEYE, ZUBAT | — | — |
| **EVER GRANDE CITY** | CORSOLA, LUVDISC, MAGIKARP, PELIPPER, TENTACOOL, WAILMER, WINGULL | — | — |
| **FIERY PATH** | CHARCADET, FUECOCO, GLIMMET, GRIMER, KOFFING, LARVESTA, MACHOP, NUMEL, SLUGMA, TORKOAL | — | — |
| **Fortree City House 4** | — | — | MENTAL HERB |
| **GRANITE CAVE 1F** | ABRA, GEODUDE, HAWLUCHA, MAKUHITA, MEDITITE, WOOBAT, ZUBAT | — | — |
| **GRANITE CAVE B 1F** | ABRA, ARON, BINACLE, KLINK, MAKUHITA, NOIBAT, SABLEYE, TYRUNT, ZUBAT | — | DIANCITE |
| **GRANITE CAVE B 2F** | ABRA, ARON, BELDUM, GEODUDE, GOLURK, HONEDGE, KINGAMBIT, NOSEPASS, SABLEYE, ZUBAT | — | — |
| **GRANITE CAVE STEVENS ROOM** | ABRA, ARON, CARBINK, MAKUHITA, NACLI, TINKATINK, ZUBAT | DIANCIE | — |
| **JAGGED PASS** | MACHOP, NUMEL, PANCHAM, SIZZLIPEDE, SPOINK | GOUGING FIRE | — |
| **LILYCOVE CITY** | MAGIKARP, PELIPPER, STARYU, TENTACOOL, WAILMER, WINGULL | — | — |
| **Littleroot Town Professor Birchs Lab** | — | CYNDAQUIL, TOTODILE | — |
| **Mauville City Bike Shop** | — | — | ACRO BIKE, MACH BIKE |
| **Mauville City House 2** | — | — | COIN CASE |
| **METEOR FALLS 1F 1R** | BARBOACH, ELGYEM, FLAMIGO, GOLDEEN, LUNATONE, MAGIKARP, ROARING MOON, SOLROCK, ZUBAT | ZYGARDE | BAXCALIBRITE |
| **METEOR FALLS 1F 2R** | BARBOACH, GOLBAT, GOLDEEN, LUNATONE, MAGIKARP, MINIOR, SLIGGOO, SLITHER WING, SOLROCK, WHISCASH | — | — |
| **METEOR FALLS B 1F 1R** | ARCHALUDON, BARBOACH, GOLBAT, GOLDEEN, IRON HANDS, LUNATONE, MAGIKARP, SOLROCK, TURTONATOR, WHISCASH | — | — |
| **METEOR FALLS B 1F 2R** | BAGON, BARBOACH, CYCLIZAR, DURANT, GOLBAT, GOLDEEN, LUNATONE, MAGIKARP, SOLROCK, WHISCASH | — | — |
| **METEOR FALLS STEVENS CAVE** | GOLBAT, JANGMO O, KLAWF, LUNATONE, SOLROCK | — | — |
| **MIRAGE TOWER 1F** | RELLOR, SANDILE, SANDSHREW, TRAPINCH | — | — |
| **MIRAGE TOWER 2F** | KROKOROK, SANDSHREW, TRAPINCH | — | — |
| **MIRAGE TOWER 3F** | SANDSHREW, SILICOBRA, TRAPINCH | — | — |
| **MIRAGE TOWER 4F** | SANDACONDA, SANDSHREW, TRAPINCH | — | CLAW FOSSIL, ROOT FOSSIL |
| **MOSSDEEP CITY** | MAGIKARP, PELIPPER, SHARPEDO, TENTACOOL, WAILMER, WINGULL | — | — |
| **Mossdeep City House 3** | — | — | SUPER ROD |
| **Mossdeep City Space Center 1F** | — | LATIOS | — |
| **Mossdeep City Stevens House** | — | BELDUM | — |
| **MT PYRE 1F** | GOURGEIST, PHANTUMP, SHUPPET, YAMASK | — | CLEANSE TAG |
| **MT PYRE 2F** | FLITTLE, GREAVARD, POLTCHAGEIST, SHUPPET, SINISTCHA | — | — |
| **MT PYRE 3F** | LAMPENT, SHUPPET | — | — |
| **MT PYRE 4F** | DUSKULL, PUMPKABOO, SHUPPET | — | — |
| **MT PYRE 5F** | DUSKULL, POLTCHAGEIST, SHUPPET | — | CHIMECHITE |
| **MT PYRE 6F** | DUSKULL, MISDREAVUS, SHUPPET | — | — |
| **MT PYRE EXTERIOR** | FINIZEN, FLUTTER MANE, SHUPPET, VULPIX, WINGULL | YVELTAL | — |
| **MT PYRE SUMMIT** | CHIMECHO, DUSKULL, SHUPPET, SPIRITOMB | — | — |
| **NEW MAUVILLE ENTRANCE** | CHARJABUG, MAGNEMITE, VAROOM, VOLTORB | — | — |
| **New Mauville Generator Annex** | — | MIRAIDON | — |
| **NEW MAUVILLE INSIDE** | ELECTRODE, GIMMIGHOUL, MAGNEMITE, MAGNETON, TYNAMO, VOLTORB | VOLTORB | EELEKTROSSITE |
| **PETALBURG CITY** | CORPHISH, GOLDEEN, MAGIKARP, MARILL | — | — |
| **PETALBURG WOODS** | CASCOON, NYMBLE, POOCHYENA, SCATTERBUG, SHROOMISH, SILCOON, SLAKOTH, SPRIGATITO, TAILLOW, VIVILLON, WURMPLE | ZYGARDE | — |
| **ROUTE 101** | CHESPIN, LECHONK, MUDKIP, POOCHYENA, SQUAWKABILLY, TANDEMAUS, TORCHIC, TREECKO, WURMPLE, ZIGZAGOON | ZYGARDE | ZYGARDITE |
| **ROUTE 102** | CORPHISH, FENNEKIN, GOLDEEN, INKAY, LOTAD, MAGIKARP, MARILL, POOCHYENA, QUAXLY, RALTS, SCATTERBUG, SURSKIT, TADBULB, WURMPLE, ZIGZAGOON | JIRACHI | — |
| **ROUTE 103** | FROAKIE, MAGIKARP, MASCHIFF, PAWMI, PELIPPER, POOCHYENA, SHARPEDO, TENTACOOL, WAILMER, WINGULL, WISHIWASHI, ZIGZAGOON | — | — |
| **ROUTE 104** | AZURILL, BASCULIN, FLETCHLING, MAGIKARP, MARILL, PELIPPER, PIDOVE, POOCHYENA, SPEWPA, TAILLOW, WINGULL, WURMPLE | — | — |
| **ROUTE 105** | CLAUNCHER, FINNEON, MAGIKARP, PELIPPER, TENTACOOL, WAILMER, WINGULL | CHIEN PAO | — |
| **ROUTE 106** | BRUXISH, MAGIKARP, PELIPPER, SKRELP, TENTACOOL, WAILMER, WINGULL | — | BARBARACITE |
| **ROUTE 107** | DHELMISE, MAGIKARP, PELIPPER, TENTACOOL, WAILMER, WIMPOD, WINGULL | — | — |
| **ROUTE 108** | BINACLE, MAGIKARP, PELIPPER, TENTACOOL, VELUZA, WAILMER, WINGULL | — | — |
| **ROUTE 109** | BRUXISH, MAGIKARP, PELIPPER, TATSUGIRI, TENTACOOL, WAILMER, WINGULL | — | — |
| **Route 109 Seashore House** | — | — | SODA POP |
| **ROUTE 110** | ARROKUDA, DEDENNE, DUNSPARCE, ELECTRIKE, EMOLGA, GULPIN, IRON TREADS, MAGIKARP, MINUN, ODDISH, PELIPPER, PLUSLE, POOCHYENA, SKIDDO, TENTACOOL, WAILMER, WIGLETT, WINGULL | — | DOWSING MACHINE |
| **Route 110 Trick House End** | — | — | RARE CANDY, TIMER BALL |
| **ROUTE 111** | BALTOY, BARBOACH, CACNEA, GEODUDE, GOLDEEN, GREAT TUSK, MAGIKARP, MARILL, SANDSHREW, SANDY SHOCKS, TOEDSCOOL, TRAPINCH, WOOPER | TING LU | — |
| **Route 111 Hoopa Ring Chamber** | — | HOOPA | — |
| **ROUTE 112** | MARILL, NUMEL, SALANDIT, SPRITZEE, ZIGZAGOON | OKIDOGI | SCOVILLAINITE |
| **ROUTE 113** | HEATMOR, LITLEO, SKARMORY, SLUGMA, SPINDA | FEZANDIPITI | — |
| **Route 113 Glass Workshop** | — | — | SOOT SACK |
| **ROUTE 114** | BARBOACH, BUNNELBY, DEWPIDER, ESPURR, FURFROU, GEODUDE, GOLDEEN, LOMBRE, LOTAD, MAGIKARP, MARILL, MUNNA, NUZLEAF, SEVIPER, SQUAWKABILLY, SWABLU, ZANGOOSE | MUNKIDORI, TERAPAGOS | — |
| **ROUTE 115** | EEVEE, FLORGES, JIGGLYPUFF, KLAWF, MAGIKARP, PELIPPER, SWABLU, SWELLOW, TAILLOW, TENTACOOL, WAILMER, WATTREL, WINGULL | — | — |
| **ROUTE 116** | ABRA, JOLTIK, MANKEY, NINCADA, POOCHYENA, SKITTY, TAILLOW, TAROUNTULA, WHISMUR | — | — |
| **ROUTE 117** | CORPHISH, FLABEBE, FLOETTE, FURFROU, GOLDEEN, ILLUMISE, MAGIKARP, MARILL, ODDISH, POOCHYENA, ROSELIA, SKRELP, SURSKIT, SWIRLIX, TATSUGIRI, VOLBEAT | OGERPON, PECHARUNT | — |
| **ROUTE 118** | ALOMOMOLA, BUIZEL, CARVANHA, ELECTRIKE, FURFROU, HELIOPTILE, KECLEON, LINOONE, MAGIKARP, MANECTRIC, PELIPPER, SHARPEDO, SMOLIV, SQUAWKABILLY, TENTACOOL, VELUZA, WINGULL, ZIGZAGOON | IRON BOULDER | TATSUGIRINITE |
| **ROUTE 119** | APPLIN, ARROKUDA, BASCULIN, BOMBIRDIER, BRUTE BONNET, CARVANHA, DEWPIDER, KECLEON, LINOONE, MAGIKARP, ODDISH, PELIPPER, ROSELIA, SCATTERBUG, TENTACOOL, TROPIUS, WINGULL, ZIGZAGOON | RAGING BOLT, XERNEAS | — |
| **Route 119 Weather Institute 2F** | — | CASTFORM | — |
| **ROUTE 120** | ABSOL, BARBOACH, CHEWTLE, FEEBAS, FIDOUGH, GOLDEEN, IMPIDIMP, KECLEON, MAGIKARP, MARILL, MIGHTYENA, ODDISH, POOCHYENA, ROSELIA, SEEDOT | IRON LEAVES, KECLEON, LATIAS, WO CHIEN | DEVON SCOPE |
| **ROUTE 121** | CRAMORANT, FLABEBE, FLOETTE, FOMANTIS, GLOOM, KECLEON, MAGIKARP, MIGHTYENA, ODDISH, PELIPPER, POOCHYENA, SHROODLE, SHUPPET, TENTACOOL, WAILMER, WINGULL | — | — |
| **ROUTE 122** | DHELMISE, MAGIKARP, PELIPPER, SHARPEDO, TENTACOOL, WAILMER, WINGULL | — | — |
| **ROUTE 123** | BRAMBLIN, CAPSAKID, FLABEBE, GLOOM, KECLEON, MAGIKARP, MAREANIE, MIGHTYENA, ODDISH, PELIPPER, POOCHYENA, ROSELIA, SHUPPET, TENTACOOL, WAILMER, WINGULL | — | — |
| **Route 123 Berry Masters House** | — | — | PAMTRE BERRY, SPELON BERRY |
| **ROUTE 124** | CLOBBOPUS, MAGIKARP, MAREANIE, PELIPPER, SHARPEDO, TENTACOOL, WAILMER, WINGULL | IRON CROWN | — |
| **ROUTE 125** | CLOBBOPUS, MAGIKARP, PELIPPER, SHARPEDO, TENTACOOL, WAILMER, WINGULL | WALKING WAKE | — |
| **ROUTE 126** | MAGIKARP, PELIPPER, SHARPEDO, TENTACOOL, WAILMER, WIGLETT, WINGULL | — | — |
| **ROUTE 127** | MAGIKARP, MANTYKE, PELIPPER, SHARPEDO, SKRELP, TENTACOOL, WAILMER, WINGULL | — | — |
| **ROUTE 128** | CORSOLA, LUVDISC, MAGIKARP, MANTYKE, PELIPPER, TENTACOOL, WAILMER, WINGULL | — | — |
| **Route 128 Ancient Grotto** | — | KORAIDON | — |
| **ROUTE 129** | GRENINJA, MAGIKARP, PELIPPER, SHARPEDO, TENTACOOL, WAILMER, WAILORD, WINGULL | — | — |
| **ROUTE 130** | FINNEON, MAGIKARP, PELIPPER, SHARPEDO, TENTACOOL, WAILMER, WINGULL, WYNAUT | — | — |
| **ROUTE 131** | MAGIKARP, PELIPPER, SHARPEDO, SHELLOS, TENTACOOL, WAILMER, WINGULL | — | — |
| **ROUTE 132** | BINACLE, HORSEA, MAGIKARP, PELIPPER, SHARPEDO, SHELLOS, TENTACOOL, WAILMER, WINGULL | — | — |
| **ROUTE 133** | CLAUNCHER, HORSEA, MAGIKARP, PELIPPER, SHARPEDO, TENTACOOL, WAILMER, WINGULL | — | — |
| **ROUTE 134** | HORSEA, MAGIKARP, PELIPPER, SHARPEDO, SKRELP, TENTACOOL, WAILMER, WINGULL | — | — |
| **Rustboro City** | — | — | AUDINITE |
| **Rustboro City Devon Corp 2F** | — | ANORITH, LILEEP | — |
| **Rustboro City Flat 2 2F** | — | — | PREMIER BALL |
| **SEAFLOOR CAVERN ENTRANCE** | GOLBAT, MAGIKARP, TENTACOOL, WAILMER, ZUBAT | — | — |
| **SEAFLOOR CAVERN ROOM 1** | CLAUNCHER, GOLBAT, IRON JUGULIS, ZUBAT | — | — |
| **SEAFLOOR CAVERN ROOM 2** | CRABRAWLER, GOLBAT, IRON MOTH, ZUBAT | — | — |
| **SEAFLOOR CAVERN ROOM 3** | ANORITH, GOLBAT, IRON THORNS, ZUBAT | — | — |
| **SEAFLOOR CAVERN ROOM 4** | GOLBAT, LILEEP, ZUBAT | — | — |
| **SEAFLOOR CAVERN ROOM 5** | ARROKUDA, GOLBAT, ZUBAT | — | — |
| **SEAFLOOR CAVERN ROOM 6** | GOLBAT, MAGIKARP, TATSUGIRI, TENTACOOL, WAILMER, ZUBAT | — | — |
| **SEAFLOOR CAVERN ROOM 7** | GOLBAT, MAGIKARP, TENTACOOL, WAILMER, WIGLETT, ZUBAT | — | — |
| **SEAFLOOR CAVERN ROOM 8** | GOLBAT, GOOMY, ZUBAT | — | — |
| **SHOAL CAVE LOW TIDE ENTRANCE ROOM** | CLOBBOPUS, GOLBAT, IRON BUNDLE, MAGIKARP, SPHEAL, TENTACOOL, WAILMER, ZUBAT | — | — |
| **SHOAL CAVE LOW TIDE ICE ROOM** | BAXCALIBUR, CETODDLE, GOLBAT, SNOM, SNORUNT, SPHEAL, VELUZA, ZUBAT | GLASTRIER | CRABOMINITE |
| **SHOAL CAVE LOW TIDE INNER ROOM** | AVALUGG, BERGMITE, DONDOZO, GOLBAT, MAGIKARP, SPHEAL, TENTACOOL, WAILMER, ZUBAT | — | SHOAL SHELL |
| **SHOAL CAVE LOW TIDE LOWER ROOM** | GOLBAT, SANDYGAST, SPHEAL, ZUBAT | — | FOCUS BAND, SHOAL SALT |
| **SHOAL CAVE LOW TIDE STAIRS ROOM** | AMAURA, CUBCHOO, FRIGIBAX, GOLBAT, SPHEAL, ZUBAT | — | SHOAL SALT |
| **SKY PILLAR 1F** | BANETTE, BOMBIRDIER, CLAYDOL, DRATINI, GOLBAT, SABLEYE | — | — |
| **SKY PILLAR 3F** | BANETTE, CLAYDOL, DURALUDON, GOLBAT, SABLEYE, SANDY SHOCKS | — | — |
| **SKY PILLAR 5F** | ALTARIA, BANETTE, CLAYDOL, GOLBAT, IRON TREADS, ROARING MOON, SABLEYE | — | — |
| **Sky Pillar Top** | — | RAYQUAZA | — |
| **SLATEPORT CITY** | MAGIKARP, PELIPPER, TENTACOOL, WAILMER, WINGULL | — | — |
| **Slateport City Pokemon Fan Club** | — | — | BLUE SCARF, RED SCARF |
| **SOOTOPOLIS CITY** | GYARADOS, MAGIKARP, TENTACOOL | — | — |
| **UNDERWATER ROUTE 124** | BRUXISH, CHINCHOU, CLAMPERL, RELICANTH | — | DRAGALGITE |
| **UNDERWATER ROUTE 126** | CHINCHOU, CLAMPERL, CLOBBOPUS, RELICANTH | — | — |
| **Verdanturf Town** | — | — | GALLADITE |

</details>

<details>
<summary><strong>Shared, event and unclassified maps · 124 locations</strong></summary>

| Area | Wild / rare Pokémon | Scripted encounters / gifts | Indexed items |
| --- | --- | --- | --- |
| **ALTERING CAVE** | AIPOM, HOUNDOUR, MAREEP, PINECO, SHUCKLE, SMEARGLE, STANTLER, TEDDIURSA, ZUBAT | — | — |
| **Ancient Tomb** | — | REGISTEEL | — |
| **Aqua Hideout B 1F** | — | ELECTRODE | — |
| **ARTISAN CAVE 1F** | SMEARGLE | — | — |
| **ARTISAN CAVE B 1F** | SMEARGLE | — | — |
| **AZALEA TOWN** | AIPOM, HERACROSS, PINECO, SPINARAK | — | — |
| **Azalea Town House 1** | — | — | CHARCOAL |
| **Azalea Town Kurts House** | — | — | FAST BALL, LOVE BALL |
| **Battle Frontier Outside East** | — | SUDOWOODO | — |
| **Battle Frontier Scotts House** | — | — | LANSAT BERRY, STARF BERRY |
| **Bellchime Trail** | — | — | BIG MUSHROOM |
| **Birth Island Exterior** | — | DEOXYS | — |
| **Birth Island Exterior Frlg** | — | DEOXYS | — |
| **CHERRYGROVE CITY** | CORSOLA, EXEGGCUTE, KINGLER, KRABBY, LEDYBA, MAGIKARP, PINECO, SPINARAK, TENTACOOL, TENTACRUEL | — | — |
| **CLIFF EDGE CAVE** | GEODUDE, GOLBAT, GRAVELER, KINGLER, KRABBY, MACHOKE, MACHOP, ONIX, QUAGSIRE, STEELIX, WOOPER | SPECTRIER | — |
| **CLIFF EDGE GATE** | QUAGSIRE, WOOPER | — | — |
| **Desert Ruins** | — | REGIROCK | — |
| **DESERT UNDERPASS** | DITTO, LOUDRED, WHISMUR | — | CLAW FOSSIL, ROOT FOSSIL |
| **DEWFORD TOWN** | MAGIKARP, PELIPPER, TENTACOOL, WAILMER, WINGULL | — | — |
| **Dewford Town House 2** | — | — | SILK SCARF |
| **Embedded Tower** | — | GROUDON, KYOGRE | — |
| **Faraway Island Interior** | — | MEW | — |
| **FIVE ISLAND** | GYARADOS, HOPPIP, HORSEA, KINGLER, KRABBY, MAGIKARP, PSYDUCK, SEADRA, SHELLDER, SLOWPOKE, STARYU, TENTACOOL, TENTACRUEL | — | — |
| **FIVE ISLAND LOST CAVE ROOM 1** | GASTLY, GOLBAT, HAUNTER, MISDREAVUS, MURKROW, ZUBAT | — | — |
| **FIVE ISLAND LOST CAVE ROOM 10** | GASTLY, GOLBAT, HAUNTER, MISDREAVUS, MURKROW, ZUBAT | — | — |
| **FIVE ISLAND LOST CAVE ROOM 11** | GASTLY, GOLBAT, HAUNTER, MISDREAVUS, MURKROW, ZUBAT | — | — |
| **FIVE ISLAND LOST CAVE ROOM 12** | GASTLY, GOLBAT, HAUNTER, MISDREAVUS, MURKROW, ZUBAT | — | — |
| **FIVE ISLAND LOST CAVE ROOM 13** | GASTLY, GOLBAT, HAUNTER, MISDREAVUS, MURKROW, ZUBAT | — | — |
| **FIVE ISLAND LOST CAVE ROOM 14** | GASTLY, GOLBAT, HAUNTER, MISDREAVUS, MURKROW, ZUBAT | — | — |
| **FIVE ISLAND LOST CAVE ROOM 2** | GASTLY, GOLBAT, HAUNTER, MISDREAVUS, MURKROW, ZUBAT | — | — |
| **FIVE ISLAND LOST CAVE ROOM 3** | GASTLY, GOLBAT, HAUNTER, MISDREAVUS, MURKROW, ZUBAT | — | — |
| **FIVE ISLAND LOST CAVE ROOM 4** | GASTLY, GOLBAT, HAUNTER, MISDREAVUS, MURKROW, ZUBAT | — | — |
| **FIVE ISLAND LOST CAVE ROOM 5** | GASTLY, GOLBAT, HAUNTER, MISDREAVUS, MURKROW, ZUBAT | — | — |
| **FIVE ISLAND LOST CAVE ROOM 6** | GASTLY, GOLBAT, HAUNTER, MISDREAVUS, MURKROW, ZUBAT | — | — |
| **FIVE ISLAND LOST CAVE ROOM 7** | GASTLY, GOLBAT, HAUNTER, MISDREAVUS, MURKROW, ZUBAT | — | — |
| **FIVE ISLAND LOST CAVE ROOM 8** | GASTLY, GOLBAT, HAUNTER, MISDREAVUS, MURKROW, ZUBAT | — | — |
| **FIVE ISLAND LOST CAVE ROOM 9** | GASTLY, GOLBAT, HAUNTER, MISDREAVUS, MURKROW, ZUBAT | — | — |
| **FIVE ISLAND MEADOW** | GYARADOS, HOPPIP, HORSEA, KINGLER, KRABBY, MAGIKARP, MEOWTH, PERSIAN, PIDGEOTTO, PIDGEY, PSYDUCK, QWILFISH, REMORAID, SEADRA, SENTRET, SLOWPOKE, TENTACOOL, TENTACRUEL | — | — |
| **FIVE ISLAND MEMORIAL PILLAR** | GYARADOS, HOPPIP, HORSEA, KINGLER, KRABBY, MAGIKARP, PSYDUCK, QWILFISH, REMORAID, SEADRA, SLOWPOKE, TENTACOOL, TENTACRUEL | — | — |
| **Five Island Memorial Pillar Frlg** | — | — | TM 42 |
| **FIVE ISLAND RESORT GORGEOUS** | GYARADOS, HOPPIP, HORSEA, KINGLER, KRABBY, MAGIKARP, PSYDUCK, QWILFISH, REMORAID, SEADRA, SLOWPOKE, TENTACOOL, TENTACRUEL | — | — |
| **FIVE ISLAND WATER LABYRINTH** | GYARADOS, HOPPIP, HORSEA, KINGLER, KRABBY, MAGIKARP, PSYDUCK, QWILFISH, REMORAID, SEADRA, SLOWPOKE, TENTACOOL, TENTACRUEL | — | — |
| **FOUR ISLAND** | GOLDEEN, GYARADOS, MAGIKARP, MARILL, POLIWAG, POLIWHIRL, PSYDUCK, SLOWPOKE, WOOPER | — | — |
| **FOUR ISLAND ICEFALL CAVE 1F** | DELIBIRD, GOLBAT, SEEL, SNEASEL, SWINUB, ZUBAT | — | — |
| **FOUR ISLAND ICEFALL CAVE B 1F** | DELIBIRD, GOLBAT, SEEL, SNEASEL, SWINUB, ZUBAT | — | — |
| **FOUR ISLAND ICEFALL CAVE BACK** | DEWGONG, GOLBAT, GYARADOS, HORSEA, KINGLER, KRABBY, LAPRAS, MAGIKARP, PSYDUCK, SEADRA, SEEL, SHELLDER, SLOWPOKE, STARYU, TENTACOOL, TENTACRUEL, ZUBAT | — | — |
| **FOUR ISLAND ICEFALL CAVE ENTRANCE** | DEWGONG, GOLBAT, GOLDEEN, GYARADOS, MAGIKARP, MARILL, POLIWAG, POLIWHIRL, PSYDUCK, SEEL, SLOWPOKE, WOOPER, ZUBAT | — | — |
| **Island Cave** | — | REGICE | — |
| **JOHTO VICTORY ROAD B 1F** | DONPHAN, GEODUDE, GOLBAT, GRAVELER, MACHOKE, ONIX, RHYHORN, SANDSLASH, URSARING | — | FULL RESTORE, TM 29 |
| **JOHTO VICTORY ROAD B 2F** | DONPHAN, GOLBAT, GOLDEEN, GOLDUCK, GRAVELER, MACHOKE, MAGIKARP, ONIX, POLIWAG, POLIWHIRL, QUAGSIRE, RHYHORN, SANDSLASH, SEAKING, SLOWBRO, URSARING | — | FULL HEAL |
| **Lavaridge Town** | — | — | GO GOGGLES |
| **Lavaridge Town Herb Shop** | — | — | CHARCOAL |
| **MAGMA HIDEOUT 1F** | GEODUDE, GRAVELER, TORKOAL | — | — |
| **MAGMA HIDEOUT 2F 1R** | GEODUDE, GRAVELER, TORKOAL | — | — |
| **MAGMA HIDEOUT 2F 2R** | GEODUDE, GRAVELER, TORKOAL | — | — |
| **MAGMA HIDEOUT 2F 3R** | GEODUDE, GRAVELER, TORKOAL | — | — |
| **MAGMA HIDEOUT 3F 1R** | GEODUDE, GRAVELER, TORKOAL | — | — |
| **MAGMA HIDEOUT 3F 2R** | GEODUDE, GRAVELER, TORKOAL | — | — |
| **MAGMA HIDEOUT 3F 3R** | GEODUDE, GRAVELER, TORKOAL | — | — |
| **MAGMA HIDEOUT 4F** | GEODUDE, GRAVELER, TORKOAL | — | — |
| **Marine Cave End** | — | KYOGRE | — |
| **Mt Chimney** | — | VOLCANION | LAVA COOKIE, PYROARITE |
| **Navel Rock Base Frlg** | — | LUGIA | — |
| **Navel Rock Bottom** | — | LUGIA | — |
| **Navel Rock Summit Frlg** | — | HO OH | — |
| **Navel Rock Top** | — | HO OH | — |
| **NEW BARK TOWN** | CHINCHOU, EXEGGCUTE, LANTURN, LEDYBA, MAGIKARP, PINECO, SHELLDER, SPINARAK, TENTACOOL, TENTACRUEL | — | — |
| **ONE ISLAND** | GYARADOS, HORSEA, KINGLER, KRABBY, MAGIKARP, PSYDUCK, SEADRA, SHELLDER, SLOWPOKE, STARYU, TENTACOOL, TENTACRUEL | — | — |
| **ONE ISLAND KINDLE ROAD** | FEAROW, GEODUDE, GRAVELER, GYARADOS, HORSEA, KINGLER, KRABBY, MAGIKARP, MEOWTH, PERSIAN, PONYTA, PSYDUCK, RAPIDASH, SEADRA, SLOWPOKE, SPEAROW, TENTACOOL, TENTACRUEL | — | — |
| **ONE ISLAND TREASURE BEACH** | FEAROW, GYARADOS, HORSEA, KINGLER, KRABBY, MAGIKARP, MEOWTH, PERSIAN, PSYDUCK, SEADRA, SLOWPOKE, SPEAROW, TANGELA, TENTACOOL, TENTACRUEL | — | — |
| **PACIFIDLOG TOWN** | MAGIKARP, PELIPPER, SHARPEDO, TENTACOOL, WAILMER, WINGULL | — | — |
| **Rocket Hideout B 4F Frlg** | — | — | SILPH SCOPE |
| **RUSTURF TUNNEL** | WHISMUR | — | — |
| **SAFARI ZONE CENTER** | CHANSEY, DRAGONAIR, DRATINI, EXEGGCUTE, GOLDEEN, MAGIKARP, NIDORAN F, NIDORAN M, NIDORINA, NIDORINO, PARASECT, PINSIR, POLIWAG, PSYDUCK, RHYHORN, SCYTHER, SEAKING, SLOWPOKE, VENONAT | — | — |
| **SAFARI ZONE EAST** | DODUO, DRAGONAIR, DRATINI, EXEGGCUTE, GOLDEEN, KANGASKHAN, MAGIKARP, NIDORAN F, NIDORAN M, NIDORINA, NIDORINO, PARAS, PARASECT, PINSIR, POLIWAG, PSYDUCK, SCYTHER, SEAKING, SLOWPOKE | — | — |
| **SAFARI ZONE GATE** | EXEGGCUTE, LEDYBA, PINECO, SPINARAK | — | — |
| **Safari Zone Gate Safari Zone Entrance** | — | BAGON, BELDUM, CASTFORM, MEDICHAM | — |
| **SAFARI ZONE LOW LEFT** | CUBONE, FEAROW, GLIGAR, KANGASKHAN, LARVITAR, RHYHORN, SANDSHREW, SANDSLASH | — | — |
| **SAFARI ZONE LOW MID** | GEODUDE, GOLDEEN, GRAVELER, KINGLER, KRABBY, MACHOP, MAGIKARP, MAGNEMITE, MAGNETON, NIDORINA, NIDORINO, ONIX, POLIWAG, POLIWHIRL, REMORAID, RHYHORN, SEAKING, TAUROS | — | — |
| **SAFARI ZONE LOW RIGHT** | DRAGONAIR, DRATINI, GOLDUCK, LARVITAR, MAGIKARP, POLIWAG, POLIWHIRL, QUAGSIRE, REMORAID, WOOPER | — | — |
| **SAFARI ZONE NORTH** | GEODUDE, GLOOM, HERACROSS, NATU, ODDISH, PHANPY, XATU | — | — |
| **SAFARI ZONE NORTH FRLG** | CHANSEY, DRAGONAIR, DRATINI, EXEGGCUTE, GOLDEEN, MAGIKARP, NIDORAN F, NIDORAN M, NIDORINA, NIDORINO, PARAS, POLIWAG, PSYDUCK, RHYHORN, SEAKING, SLOWPOKE, TAUROS, VENOMOTH | — | — |
| **SAFARI ZONE NORTHEAST** | AIPOM, HOOTHOOT, HOUNDOUR, LEDYBA, MILTANK, PINECO, SHUCKLE, SUNKERN, TEDDIURSA | — | — |
| **SAFARI ZONE NORTHWEST** | DODRIO, DODUO, GLOOM, GOLDEEN, GOLDUCK, MAGIKARP, ODDISH, PINSIR, PSYDUCK, RHYHORN, SEAKING | — | — |
| **Safari Zone Secret House Frlg** | — | — | HM 03 |
| **SAFARI ZONE SOUTH** | DODUO, GIRAFARIG, GLOOM, NATU, ODDISH, PIKACHU, WOBBUFFET | — | — |
| **SAFARI ZONE SOUTHEAST** | AIPOM, GLIGAR, GOLDEEN, HOOTHOOT, MAGIKARP, MAREEP, MARILL, OCTILLERY, QUAGSIRE, REMORAID, SNUBBULL, SPINARAK, STANTLER, SUNKERN, WOOPER | — | — |
| **SAFARI ZONE SOUTHWEST** | DODUO, GIRAFARIG, GLOOM, GOLDEEN, MAGIKARP, NATU, ODDISH, PIKACHU, PSYDUCK, SEAKING, WOBBUFFET | — | — |
| **SAFARI ZONE TOP LEFT** | EKANS, KANGASKHAN, PINSIR, PONYTA, SCYTHER, YANMA | — | — |
| **SAFARI ZONE TOP MID** | ARBOK, GOLDEEN, HAUNTER, KINGLER, KRABBY, MAGIKARP, MISDREAVUS, MR MIME, MURKROW, PIDGEOTTO, POLIWAG, POLIWHIRL, REMORAID, SEAKING, WEEPINBELL | — | — |
| **SAFARI ZONE TOP RIGHT** | EXEGGCUTE, GYARADOS, HORSEA, KINGLER, KRABBY, MAGIKARP, PSYDUCK, RATICATE, RATTATA, SEADRA, TAUROS | — | — |
| **SAFARI ZONE WEST** | DODUO, DRAGONAIR, DRATINI, EXEGGCUTE, GOLDEEN, KANGASKHAN, MAGIKARP, NIDORAN F, NIDORAN M, NIDORINA, NIDORINO, POLIWAG, PSYDUCK, SEAKING, SLOWPOKE, TAUROS, VENOMOTH, VENONAT | — | — |
| **Sealed Chamber Outer Room** | — | CHI YU | — |
| **SEVEN ISLAND SEVAULT CANYON** | CUBONE, FEAROW, GEODUDE, GRAVELER, LARVITAR, MAROWAK, MEOWTH, ONIX, PERSIAN, PHANPY, SKARMORY | — | — |
| **SEVEN ISLAND SEVAULT CANYON ENTRANCE** | FEAROW, MEOWTH, PERSIAN, PHANPY, PSYDUCK, SENTRET, SLOWPOKE, SPEAROW | — | — |
| **SEVEN ISLAND TANOBY RUINS** | GYARADOS, HORSEA, KINGLER, KRABBY, MAGIKARP, MANTINE, PSYDUCK, QWILFISH, REMORAID, SEADRA, SLOWPOKE, TENTACOOL, TENTACRUEL | — | — |
| **SEVEN ISLAND TANOBY RUINS DILFORD CHAMBER** | UNOWN | — | — |
| **SEVEN ISLAND TANOBY RUINS LIPTOO CHAMBER** | UNOWN | — | — |
| **SEVEN ISLAND TANOBY RUINS MONEAN CHAMBER** | UNOWN | — | — |
| **SEVEN ISLAND TANOBY RUINS RIXY CHAMBER** | UNOWN | — | — |
| **SEVEN ISLAND TANOBY RUINS SCUFIB CHAMBER** | UNOWN | — | — |
| **SEVEN ISLAND TANOBY RUINS VIAPOIS CHAMBER** | UNOWN | — | — |
| **SEVEN ISLAND TANOBY RUINS WEEPTH CHAMBER** | UNOWN | — | — |
| **SEVEN ISLAND TRAINER TOWER** | GYARADOS, HORSEA, KINGLER, KRABBY, MAGIKARP, MANTINE, PSYDUCK, QWILFISH, REMORAID, SEADRA, SLOWPOKE, TENTACOOL, TENTACRUEL | — | — |
| **SIX ISLAND ALTERING CAVE** | AIPOM, HOUNDOUR, MAREEP, PINECO, SHUCKLE, SMEARGLE, STANTLER, TEDDIURSA, ZUBAT | — | — |
| **SIX ISLAND GREEN PATH** | GYARADOS, HORSEA, KINGLER, KRABBY, MAGIKARP, PSYDUCK, QWILFISH, REMORAID, SEADRA, SLOWPOKE, TENTACOOL, TENTACRUEL | — | — |
| **SIX ISLAND OUTCAST ISLAND** | GYARADOS, HORSEA, KINGLER, KRABBY, MAGIKARP, PSYDUCK, QWILFISH, REMORAID, SEADRA, SLOWPOKE, TENTACOOL, TENTACRUEL | — | — |
| **SIX ISLAND PATTERN BUSH** | CATERPIE, HERACROSS, KAKUNA, LEDYBA, METAPOD, SPINARAK, WEEDLE | — | — |
| **SIX ISLAND RUIN VALLEY** | FEAROW, GOLDEEN, GYARADOS, MAGIKARP, MARILL, MEOWTH, NATU, PERSIAN, POLIWAG, POLIWHIRL, PSYDUCK, SLOWPOKE, SPEAROW, WOBBUFFET, WOOPER, YANMA | — | — |
| **SIX ISLAND WATER PATH** | BELLSPROUT, FEAROW, GLOOM, GYARADOS, HORSEA, KINGLER, KRABBY, MAGIKARP, MEOWTH, ODDISH, PERSIAN, PSYDUCK, QWILFISH, REMORAID, SEADRA, SENTRET, SLOWPOKE, SPEAROW, TENTACOOL, TENTACRUEL, WEEPINBELL | — | — |
| **Southern Island Interior** | — | LATIAS, LATIOS | — |
| **SSAnne Captains Office Frlg** | — | — | HM 01 |
| **SSANNE EXTERIOR** | GYARADOS, HORSEA, KRABBY, MAGIKARP, PSYDUCK, SHELLDER, SLOWPOKE, STARYU, TENTACOOL | — | — |
| **SSAqua Room SSE** | — | — | METAL COAT |
| **Terra Cave End** | — | GROUDON | — |
| **THREE ISLAND BERRY FOREST** | BELLSPROUT, DROWZEE, EXEGGCUTE, GLOOM, GOLDEEN, GOLDUCK, GYARADOS, HYPNO, MAGIKARP, ODDISH, PIDGEOTTO, PIDGEY, POLIWAG, PSYDUCK, SEAKING, SLOWBRO, SLOWPOKE, VENOMOTH, VENONAT, WEEPINBELL | — | — |
| **Three Island Berry Forest Frlg** | — | HYPNO | IAPAPA BERRY |
| **THREE ISLAND BOND BRIDGE** | BELLSPROUT, GLOOM, GYARADOS, HORSEA, KINGLER, KRABBY, MAGIKARP, MEOWTH, ODDISH, PERSIAN, PIDGEOTTO, PIDGEY, PSYDUCK, SEADRA, SLOWPOKE, TENTACOOL, TENTACRUEL, VENONAT, WEEPINBELL | — | — |
| **Three Island Dunsparce Tunnel Frlg** | — | — | NUGGET |
| **THREE ISLAND PORT** | DUNSPARCE | — | — |
| **TWO ISLAND CAPE BRINK** | BELLSPROUT, FEAROW, GLOOM, GOLDEEN, GOLDUCK, GYARADOS, MAGIKARP, MEOWTH, ODDISH, PERSIAN, POLIWAG, POLIWHIRL, PSYDUCK, SLOWBRO, SLOWPOKE, SPEAROW, WEEPINBELL | — | — |
| **VICTORY ROAD B 1F** | GEODUDE, GOLBAT, GRAVELER, HARIYAMA, KLEFKI, LAIRON, LUCARIO, MAWILE | — | — |
| **VICTORY ROAD B 2F** | BARBOACH, DRAMPA, GOLBAT, GOLDEEN, LAIRON, MAGIKARP, MAWILE, SABLEYE, SCREAM TAIL, WHISCASH | — | — |
| **VIOLET CITY** | BELLSPROUT, LEDYBA, MAGIKARP, POLIWAG, POLIWHIRL, RATTATA, SPINARAK | — | PECHA BERRY, RARE CANDY |

</details>

## All 92 Mega Stones
Current source tests index **30 hard-rematch rewards, 21 Battle Net vendor stones and 41 overworld pickups**; 13 stones have an additional free starter-choice path. These are source-level acquisition references, not successful pickup tests. The older Z-A proposal ledger was superseded by implemented sources.
| Mega Stone | Source paths |
| --- | --- |
| **VENUSAURITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| **CHARIZARDITE X** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| **CHARIZARDITE Y** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| **BLASTOISINITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| **BEEDRILLITE** | Hard rematch: LEADER KOGA |
| **PIDGEOTITE** | Hard rematch: FALKNER 1 |
| **ALAKAZITE** | Hard rematch: LEADER SABRINA |
| **SLOWBRONITE** | Hard rematch: WILL 2 |
| **GENGARITE** | Hard rematch: ELITE FOUR AGATHA 2 |
| **KANGASKHANITE** | Hard rematch: NORMAN 1 |
| **PINSIRITE** | Battle Net vendor (Hub 2F) |
| **GYARADOSITE** | Hard rematch: LEADER MISTY |
| **AERODACTYLITE** | Hard rematch: LEADER BROCK |
| **MEWTWONITE X** | Hard rematch: CHAMPION REMATCH SQUIRTLE; Hard rematch: CHAMPION REMATCH BULBASAUR; Hard rematch: CHAMPION REMATCH CHARMANDER |
| **MEWTWONITE Y** | Hard rematch: CHAMPION REMATCH SQUIRTLE; Hard rematch: CHAMPION REMATCH BULBASAUR; Hard rematch: CHAMPION REMATCH CHARMANDER |
| **AMPHAROSITE** | Hard rematch: CLAIR 1 |
| **STEELIXITE** | Hard rematch: JASMINE 1 |
| **SCIZORITE** | Hard rematch: BUGSY 1 |
| **HERACRONITE** | Hard rematch: CHUCK 1 |
| **HOUNDOOMINITE** | Hard rematch: LEADER BLAINE |
| **TYRANITARITE** | Hard rematch: SIDNEY |
| **SCEPTILITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| **BLAZIKENITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| **SWAMPERTITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| **GARDEVOIRITE** | Battle Net vendor (Hub 2F) |
| **SABLENITE** | Hard rematch: MORTY 1 |
| **MAWILITE** | Battle Net vendor (Hub 2F) |
| **AGGRONITE** | Hard rematch: ROXANNE 1 |
| **MEDICHAMITE** | Hard rematch: BRAWLY 1 |
| **MANECTITE** | Hard rematch: WATTSON 1 |
| **SHARPEDONITE** | Hard rematch: JUAN 1 |
| **CAMERUPTITE** | Hard rematch: FLANNERY 1 |
| **ALTARIANITE** | Hard rematch: WINONA 1 |
| **BANETTITE** | Hard rematch: PHOEBE |
| **ABSOLITE** | Hard rematch: KAREN 2 JT |
| **GLALITITE** | Hard rematch: GLACIA |
| **SALAMENCITE** | Hard rematch: LANCE 2 |
| **METAGROSSITE** | Hard rematch: TATE AND LIZA 1 |
| **LATIASITE** | Hard rematch: WALLACE |
| **LATIOSITE** | Hard rematch: DRAKE |
| **LOPUNNITE** | Item ball at Route 34 |
| **GARCHOMPITE** | Item ball at Dragons Den Cavern |
| **LUCARIONITE** | Item ball at Route 10 Frlg |
| **ABOMASITE** | Item ball at Seafoam Islands B 3F Frlg |
| **GALLADITE** | Item ball at Verdanturf Town |
| **AUDINITE** | Item ball at Rustboro City |
| **DIANCITE** | Item ball at Granite Cave B 1F |
| **CLEFABLITE** | Item ball at Mt Moon 1F Frlg |
| **VICTREEBELITE** | Item ball at Celadon City Frlg |
| **STARMINITE** | Item ball at Route 20 Frlg |
| **DRAGONINITE** | Battle Net vendor (Hub 2F) |
| **MEGANIUMITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| **FERALIGITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| **SKARMORITE** | Item ball at Route 45 |
| **FROSLASSITE** | Item ball at Ice Path B 1F |
| **EMBOARITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| **EXCADRITE** | Item ball at Union Cave 1F |
| **SCOLIPITE** | Item ball at Ilex Forest |
| **SCRAFTINITE** | Item ball at Goldenrod City Underground Tunnel |
| **EELEKTROSSITE** | Item ball at New Mauville Inside |
| **CHANDELURITE** | Item ball at Pokemon Tower 4F Frlg |
| **CHESNAUGHTITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| **DELPHOXITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| **GRENINJITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| **PYROARITE** | Item ball at Mt Chimney |
| **FLOETTITE** | Item ball at Goldenrod City Flower Shop |
| **MALAMARITE** | Item ball at Abandoned Ship Rooms 1F |
| **BARBARACITE** | Item ball at Route 106 |
| **DRAGALGITE** | Item ball at Underwater Route 124 |
| **HAWLUCHANITE** | Item ball at Saffron City Dojo Frlg |
| **ZYGARDITE** | Item ball at Route 101; Pickup at Route 101 |
| **DRAMPANITE** | Item ball at Mt Mortar 1F South |
| **FALINKSITE** | Item ball at Route 23 Frlg |
| **HEATRANITE** | Item ball at Rock Tunnel B 1F Frlg; Pickup at Rock Tunnel B 1F Frlg |
| **DARKRANITE** | Item ball at Pokemon Tower 7F Frlg; Pickup at Pokemon Tower 7F Frlg |
| **ZERAORITE** | Item ball at Power Plant Frlg; Pickup at Power Plant Frlg |
| **RAICHUNITE X** | Battle Net vendor (Hub 2F) |
| **RAICHUNITE Y** | Battle Net vendor (Hub 2F) |
| **CHIMECHITE** | Item ball at Mt Pyre 5F |
| **ABSOLITE Z** | Item ball at Burned Tower 1F |
| **STARAPTITE** | Item ball at Route 16 Frlg |
| **GARCHOMPITE Z** | Battle Net vendor (Hub 2F) |
| **LUCARIONITE Z** | Battle Net vendor (Hub 2F) |
| **GOLURKITE** | Item ball at Ruins Of Alph Outside |
| **MEOWSTICITE** | Item ball at Saffron City Frlg |
| **CRABOMINITE** | Item ball at Shoal Cave Low Tide Ice Room |
| **GOLISOPITE** | Item ball at Whirl Islands 1F |
| **MAGEARNITE** | Item ball at Silph Co 7F Frlg; Pickup at Silph Co 7F Frlg |
| **SCOVILLAINITE** | Item ball at Route 112 |
| **BAXCALIBRITE** | Item ball at Meteor Falls 1F 1R |
| **TATSUGIRINITE** | Item ball at Route 118 |
| **GLIMMORANITE** | Item ball at Victory Road 2F Frlg |

## All defined items (searchable)
The listing below includes every explicitly numbered item. **Source not indexed** means this audit did not find an explicit map pickup, scripted reward or Battle Net source. Shops, inherited game systems, held-item drops and other mechanics may still distribute it.

<details>
<summary><strong>Open complete 874-item lookup</strong></summary>

| ID | Item | Indexed acquisition path |
| ---: | --- | --- |
| 0 | **NONE** | *Source not indexed* |
| 1 | **POKE BALL** | Pickup at Route 31 |
| 2 | **GREAT BALL** | Pickup at Route 32; Pickup at Union Cave 1F |
| 3 | **ULTRA BALL** | Pickup at Burned Tower B 1F; Pickup at Goldenrod City Underground Storage; Pickup at Route 44; +2 additional source records |
| 4 | **MASTER BALL** | *Source not indexed* |
| 5 | **PREMIER BALL** | Scripted gift at Rustboro City Flat 2 2F |
| 6 | **HEAL BALL** | *Source not indexed* |
| 7 | **NET BALL** | Scripted gift at Route 12 Fishing House Frlg |
| 8 | **NEST BALL** | *Source not indexed* |
| 9 | **DIVE BALL** | *Source not indexed* |
| 10 | **DUSK BALL** | *Source not indexed* |
| 11 | **TIMER BALL** | Scripted gift at Route 110 Trick House End |
| 12 | **QUICK BALL** | *Source not indexed* |
| 13 | **REPEAT BALL** | *Source not indexed* |
| 14 | **LUXURY BALL** | *Source not indexed* |
| 15 | **LEVEL BALL** | *Source not indexed* |
| 16 | **LURE BALL** | *Source not indexed* |
| 17 | **MOON BALL** | *Source not indexed* |
| 18 | **FRIEND BALL** | *Source not indexed* |
| 19 | **LOVE BALL** | Scripted gift at Azalea Town Kurts House |
| 20 | **FAST BALL** | Scripted gift at Azalea Town Kurts House |
| 21 | **HEAVY BALL** | *Source not indexed* |
| 22 | **DREAM BALL** | *Source not indexed* |
| 23 | **SAFARI BALL** | Scripted gift at Gate National Park |
| 24 | **SPORT BALL** | *Source not indexed* |
| 25 | **PARK BALL** | *Source not indexed* |
| 26 | **BEAST BALL** | *Source not indexed* |
| 27 | **CHERISH BALL** | *Source not indexed* |
| 28 | **POTION** | Pickup at Dark Cave South Side; Pickup at Route 29; Pickup at Route 32; +2 additional source records |
| 29 | **SUPER POTION** | Pickup at Goldenrod City Underground Tunnel; Pickup at Ilex Forest; Pickup at Route 42; +1 additional source records |
| 30 | **HYPER POTION** | Pickup at Mahogany Hideout B 1F; Pickup at Mt Mortar B 1F; Pickup at Union Cave B 2F |
| 31 | **MAX POTION** | Pickup at Dragons Den Cavern; Pickup at Ice Path B 2F; Pickup at Mt Silver 1F Waterfall Room; +1 additional source records |
| 32 | **FULL RESTORE** | Pickup at Johto Victory Road B 1F; Pickup at Lake Of Rage; Pickup at Whirl Islands B 1F; +1 additional source records |
| 33 | **REVIVE** | Pickup at Dark Cave South Side; Pickup at Ice Path B 1F; Pickup at Mt Mortar 1F South; +2 additional source records |
| 34 | **MAX REVIVE** | Pickup at Lake Of Rage; Pickup at Mt Mortar 2F; Pickup at Whirl Islands Descent |
| 35 | **FRESH WATER** | *Source not indexed* |
| 36 | **SODA POP** | Scripted gift at Route 109 Seashore House |
| 37 | **LEMONADE** | *Source not indexed* |
| 38 | **MOOMOO MILK** | *Source not indexed* |
| 39 | **ENERGY POWDER** | *Source not indexed* |
| 40 | **ENERGY ROOT** | *Source not indexed* |
| 41 | **HEAL POWDER** | Pickup at Ruins Of Alph Puzzle And Reward Chambers |
| 42 | **REVIVAL HERB** | *Source not indexed* |
| 43 | **ANTIDOTE** | Pickup at Route 31 |
| 44 | **PARALYZE HEAL** | Pickup at Sprout Tower 1F |
| 45 | **BURN HEAL** | *Source not indexed* |
| 46 | **ICE HEAL** | Pickup at Mahogany Hideout B 3F |
| 47 | **AWAKENING** | *Source not indexed* |
| 48 | **FULL HEAL** | Pickup at Ice Path B 2F; Pickup at Johto Victory Road B 2F; Pickup at Tin Tower 3F |
| 49 | **ETHER** | Pickup at Burned Tower 1F; Pickup at Goldenrod City Department Store Basement; Pickup at Ilex Forest; +2 additional source records |
| 50 | **MAX ETHER** | Pickup at Route 43 |
| 51 | **ELIXIR** | Pickup at Mt Mortar 1F South; Pickup at Route 45 |
| 52 | **MAX ELIXIR** | Pickup at Johto Victory Road 1F |
| 53 | **BERRY JUICE** | *Source not indexed* |
| 54 | **SACRED ASH** | *Source not indexed* |
| 55 | **SWEET HEART** | *Source not indexed* |
| 56 | **MAX HONEY** | *Source not indexed* |
| 57 | **PEWTER CRUNCHIES** | *Source not indexed* |
| 58 | **RAGE CANDY BAR** | Scripted gift at Mahoganytown |
| 59 | **LAVA COOKIE** | Scripted gift at Mt Chimney |
| 60 | **OLD GATEAU** | *Source not indexed* |
| 61 | **CASTELIACONE** | *Source not indexed* |
| 62 | **LUMIOSE GALETTE** | *Source not indexed* |
| 63 | **SHALOUR SABLE** | *Source not indexed* |
| 64 | **BIG MALASADA** | *Source not indexed* |
| 65 | **HP UP** | Pickup at Burned Tower 1F |
| 66 | **PROTEIN** | Pickup at Ice Path 1F; Pickup at Mahogany Hideout B 3F |
| 67 | **IRON** | Pickup at Ice Path B 1F; Pickup at Mt Mortar 2F |
| 68 | **CALCIUM** | *Source not indexed* |
| 69 | **ZINC** | *Source not indexed* |
| 70 | **CARBOS** | Pickup at Mt Mortar B 1F |
| 71 | **PP UP** | Pickup at Ice Path 1F; Pickup at Johto Victory Road 1F |
| 72 | **PP MAX** | *Source not indexed* |
| 73 | **HEALTH FEATHER** | *Source not indexed* |
| 74 | **MUSCLE FEATHER** | *Source not indexed* |
| 75 | **RESIST FEATHER** | *Source not indexed* |
| 76 | **GENIUS FEATHER** | *Source not indexed* |
| 77 | **CLEVER FEATHER** | *Source not indexed* |
| 78 | **SWIFT FEATHER** | *Source not indexed* |
| 79 | **ABILITY CAPSULE** | *Source not indexed* |
| 80 | **ABILITY PATCH** | *Source not indexed* |
| 81 | **LONELY MINT** | *Source not indexed* |
| 82 | **ADAMANT MINT** | *Source not indexed* |
| 83 | **NAUGHTY MINT** | *Source not indexed* |
| 84 | **BRAVE MINT** | *Source not indexed* |
| 85 | **BOLD MINT** | *Source not indexed* |
| 86 | **IMPISH MINT** | *Source not indexed* |
| 87 | **LAX MINT** | *Source not indexed* |
| 88 | **RELAXED MINT** | *Source not indexed* |
| 89 | **MODEST MINT** | Pickup at Route 42 |
| 90 | **MILD MINT** | *Source not indexed* |
| 91 | **RASH MINT** | *Source not indexed* |
| 92 | **QUIET MINT** | *Source not indexed* |
| 93 | **CALM MINT** | *Source not indexed* |
| 94 | **GENTLE MINT** | *Source not indexed* |
| 95 | **CAREFUL MINT** | *Source not indexed* |
| 96 | **SASSY MINT** | *Source not indexed* |
| 97 | **TIMID MINT** | *Source not indexed* |
| 98 | **HASTY MINT** | *Source not indexed* |
| 99 | **JOLLY MINT** | *Source not indexed* |
| 100 | **NAIVE MINT** | *Source not indexed* |
| 101 | **SERIOUS MINT** | *Source not indexed* |
| 102 | **RARE CANDY** | Pickup at Ecruteak City; Pickup at Mt Silver Snow; Pickup at Olivine City Lighthouse; +5 additional source records |
| 103 | **EXP CANDY XS** | *Source not indexed* |
| 104 | **EXP CANDY S** | *Source not indexed* |
| 105 | **EXP CANDY M** | *Source not indexed* |
| 106 | **EXP CANDY L** | *Source not indexed* |
| 107 | **EXP CANDY XL** | *Source not indexed* |
| 108 | **DYNAMAX CANDY** | *Source not indexed* |
| 109 | **BLUE FLUTE** | *Source not indexed* |
| 110 | **YELLOW FLUTE** | *Source not indexed* |
| 111 | **RED FLUTE** | *Source not indexed* |
| 112 | **BLACK FLUTE** | Pickup at Dark Cave North Side |
| 113 | **WHITE FLUTE** | *Source not indexed* |
| 114 | **REPEL** | *Source not indexed* |
| 115 | **SUPER REPEL** | *Source not indexed* |
| 116 | **MAX REPEL** | Pickup at Mt Mortar 1F North; Pickup at Route 44 |
| 117 | **LURE** | *Source not indexed* |
| 118 | **SUPER LURE** | *Source not indexed* |
| 119 | **MAX LURE** | *Source not indexed* |
| 120 | **ESCAPE ROPE** | Pickup at Mt Mortar 1F North; Pickup at Sprout Tower 3F |
| 121 | **X ATTACK** | *Source not indexed* |
| 122 | **X DEFENSE** | *Source not indexed* |
| 123 | **X SP ATK** | *Source not indexed* |
| 124 | **X SP DEF** | *Source not indexed* |
| 125 | **X SPEED** | *Source not indexed* |
| 126 | **X ACCURACY** | *Source not indexed* |
| 127 | **DIRE HIT** | Pickup at Mt Silver 1F Waterfall Room |
| 128 | **GUARD SPEC** | *Source not indexed* |
| 129 | **POKE DOLL** | *Source not indexed* |
| 130 | **FLUFFY TAIL** | *Source not indexed* |
| 131 | **POKE TOY** | *Source not indexed* |
| 132 | **MAX MUSHROOMS** | *Source not indexed* |
| 133 | **BOTTLE CAP** | *Source not indexed* |
| 134 | **GOLD BOTTLE CAP** | *Source not indexed* |
| 135 | **NUGGET** | Scripted gift at Three Island Dunsparce Tunnel Frlg |
| 136 | **BIG NUGGET** | Pickup at Mahogany Hideout B 1F; Pickup at Route 35; Pickup at Route 39; +1 additional source records |
| 137 | **TINY MUSHROOM** | *Source not indexed* |
| 138 | **BIG MUSHROOM** | Pickup at Bellchime Trail |
| 139 | **BALM MUSHROOM** | *Source not indexed* |
| 140 | **PEARL** | Pickup at Route 47 |
| 141 | **BIG PEARL** | *Source not indexed* |
| 142 | **PEARL STRING** | *Source not indexed* |
| 143 | **STARDUST** | *Source not indexed* |
| 144 | **STAR PIECE** | Pickup at Dark Cave North Side |
| 145 | **COMET SHARD** | *Source not indexed* |
| 146 | **SHOAL SALT** | Scripted gift at Shoal Cave Low Tide Lower Room; Scripted gift at Shoal Cave Low Tide Stairs Room |
| 147 | **SHOAL SHELL** | Scripted gift at Shoal Cave Low Tide Inner Room |
| 148 | **RED SHARD** | *Source not indexed* |
| 149 | **BLUE SHARD** | *Source not indexed* |
| 150 | **YELLOW SHARD** | *Source not indexed* |
| 151 | **GREEN SHARD** | *Source not indexed* |
| 152 | **HEART SCALE** | Pickup at Tohjo Falls Cavern |
| 153 | **HONEY** | *Source not indexed* |
| 154 | **RARE BONE** | *Source not indexed* |
| 155 | **ODD KEYSTONE** | *Source not indexed* |
| 156 | **PRETTY FEATHER** | *Source not indexed* |
| 157 | **RELIC COPPER** | *Source not indexed* |
| 158 | **RELIC SILVER** | *Source not indexed* |
| 159 | **RELIC GOLD** | *Source not indexed* |
| 160 | **RELIC VASE** | *Source not indexed* |
| 161 | **RELIC BAND** | *Source not indexed* |
| 162 | **RELIC STATUE** | *Source not indexed* |
| 163 | **RELIC CROWN** | *Source not indexed* |
| 164 | **STRANGE SOUVENIR** | *Source not indexed* |
| 165 | **HELIX FOSSIL** | Pickup at Ruins Of Alph B 1F; Scripted gift at Mt Moon B 2F Frlg |
| 166 | **DOME FOSSIL** | Pickup at Ruins Of Alph B 1F; Scripted gift at Mt Moon B 2F Frlg |
| 167 | **OLD AMBER** | *Source not indexed* |
| 168 | **ROOT FOSSIL** | Scripted gift at Desert Underpass; Scripted gift at Mirage Tower 4F |
| 169 | **CLAW FOSSIL** | Scripted gift at Desert Underpass; Scripted gift at Mirage Tower 4F |
| 170 | **ARMOR FOSSIL** | *Source not indexed* |
| 171 | **SKULL FOSSIL** | *Source not indexed* |
| 172 | **COVER FOSSIL** | *Source not indexed* |
| 173 | **PLUME FOSSIL** | *Source not indexed* |
| 174 | **JAW FOSSIL** | *Source not indexed* |
| 175 | **SAIL FOSSIL** | *Source not indexed* |
| 176 | **FOSSILIZED BIRD** | *Source not indexed* |
| 177 | **FOSSILIZED FISH** | *Source not indexed* |
| 178 | **FOSSILIZED DRAKE** | *Source not indexed* |
| 179 | **FOSSILIZED DINO** | *Source not indexed* |
| 180 | **GROWTH MULCH** | *Source not indexed* |
| 181 | **DAMP MULCH** | *Source not indexed* |
| 182 | **STABLE MULCH** | *Source not indexed* |
| 183 | **GOOEY MULCH** | *Source not indexed* |
| 184 | **RICH MULCH** | *Source not indexed* |
| 185 | **SURPRISE MULCH** | *Source not indexed* |
| 186 | **BOOST MULCH** | *Source not indexed* |
| 187 | **AMAZE MULCH** | *Source not indexed* |
| 188 | **RED APRICORN** | *Source not indexed* |
| 189 | **BLUE APRICORN** | *Source not indexed* |
| 190 | **YELLOW APRICORN** | *Source not indexed* |
| 191 | **GREEN APRICORN** | *Source not indexed* |
| 192 | **PINK APRICORN** | *Source not indexed* |
| 193 | **WHITE APRICORN** | *Source not indexed* |
| 194 | **BLACK APRICORN** | *Source not indexed* |
| 195 | **WISHING PIECE** | *Source not indexed* |
| 196 | **GALARICA TWIG** | *Source not indexed* |
| 197 | **ARMORITE ORE** | *Source not indexed* |
| 198 | **DYNITE ORE** | *Source not indexed* |
| 200 | **HARBOR MAIL** | *Source not indexed* |
| 201 | **GLITTER MAIL** | *Source not indexed* |
| 202 | **MECH MAIL** | *Source not indexed* |
| 203 | **WOOD MAIL** | *Source not indexed* |
| 204 | **WAVE MAIL** | *Source not indexed* |
| 205 | **BEAD MAIL** | *Source not indexed* |
| 206 | **SHADOW MAIL** | *Source not indexed* |
| 207 | **TROPIC MAIL** | *Source not indexed* |
| 208 | **DREAM MAIL** | *Source not indexed* |
| 209 | **FAB MAIL** | *Source not indexed* |
| 210 | **RETRO MAIL** | *Source not indexed* |
| 211 | **FIRE STONE** | *Source not indexed* |
| 212 | **WATER STONE** | *Source not indexed* |
| 213 | **THUNDER STONE** | *Source not indexed* |
| 214 | **LEAF STONE** | *Source not indexed* |
| 215 | **ICE STONE** | *Source not indexed* |
| 216 | **SUN STONE** | *Source not indexed* |
| 217 | **MOON STONE** | Pickup at Route 27 |
| 218 | **SHINY STONE** | *Source not indexed* |
| 219 | **DUSK STONE** | *Source not indexed* |
| 220 | **DAWN STONE** | *Source not indexed* |
| 221 | **SWEET APPLE** | *Source not indexed* |
| 222 | **TART APPLE** | *Source not indexed* |
| 223 | **CRACKED POT** | *Source not indexed* |
| 224 | **CHIPPED POT** | *Source not indexed* |
| 225 | **GALARICA CUFF** | *Source not indexed* |
| 226 | **GALARICA WREATH** | *Source not indexed* |
| 227 | **DRAGON SCALE** | *Source not indexed* |
| 228 | **UPGRADE** | *Source not indexed* |
| 229 | **PROTECTOR** | *Source not indexed* |
| 230 | **ELECTIRIZER** | *Source not indexed* |
| 231 | **MAGMARIZER** | *Source not indexed* |
| 232 | **DUBIOUS DISC** | *Source not indexed* |
| 233 | **REAPER CLOTH** | *Source not indexed* |
| 234 | **PRISM SCALE** | *Source not indexed* |
| 235 | **WHIPPED DREAM** | *Source not indexed* |
| 236 | **SACHET** | *Source not indexed* |
| 237 | **OVAL STONE** | *Source not indexed* |
| 238 | **STRAWBERRY SWEET** | *Source not indexed* |
| 239 | **LOVE SWEET** | *Source not indexed* |
| 240 | **BERRY SWEET** | *Source not indexed* |
| 241 | **CLOVER SWEET** | *Source not indexed* |
| 242 | **FLOWER SWEET** | *Source not indexed* |
| 243 | **STAR SWEET** | *Source not indexed* |
| 244 | **RIBBON SWEET** | *Source not indexed* |
| 245 | **EVERSTONE** | *Source not indexed* |
| 246 | **RED NECTAR** | *Source not indexed* |
| 247 | **YELLOW NECTAR** | *Source not indexed* |
| 248 | **PINK NECTAR** | *Source not indexed* |
| 249 | **PURPLE NECTAR** | *Source not indexed* |
| 250 | **FLAME PLATE** | *Source not indexed* |
| 251 | **SPLASH PLATE** | *Source not indexed* |
| 252 | **ZAP PLATE** | *Source not indexed* |
| 253 | **MEADOW PLATE** | *Source not indexed* |
| 254 | **ICICLE PLATE** | *Source not indexed* |
| 255 | **FIST PLATE** | *Source not indexed* |
| 256 | **TOXIC PLATE** | *Source not indexed* |
| 257 | **EARTH PLATE** | *Source not indexed* |
| 258 | **SKY PLATE** | *Source not indexed* |
| 259 | **MIND PLATE** | *Source not indexed* |
| 260 | **INSECT PLATE** | *Source not indexed* |
| 261 | **STONE PLATE** | *Source not indexed* |
| 262 | **SPOOKY PLATE** | *Source not indexed* |
| 263 | **DRACO PLATE** | *Source not indexed* |
| 264 | **DREAD PLATE** | *Source not indexed* |
| 265 | **IRON PLATE** | *Source not indexed* |
| 266 | **PIXIE PLATE** | *Source not indexed* |
| 267 | **DOUSE DRIVE** | *Source not indexed* |
| 268 | **SHOCK DRIVE** | *Source not indexed* |
| 269 | **BURN DRIVE** | *Source not indexed* |
| 270 | **CHILL DRIVE** | *Source not indexed* |
| 271 | **FIRE MEMORY** | *Source not indexed* |
| 272 | **WATER MEMORY** | *Source not indexed* |
| 273 | **ELECTRIC MEMORY** | *Source not indexed* |
| 274 | **GRASS MEMORY** | *Source not indexed* |
| 275 | **ICE MEMORY** | *Source not indexed* |
| 276 | **FIGHTING MEMORY** | *Source not indexed* |
| 277 | **POISON MEMORY** | *Source not indexed* |
| 278 | **GROUND MEMORY** | *Source not indexed* |
| 279 | **FLYING MEMORY** | *Source not indexed* |
| 280 | **PSYCHIC MEMORY** | *Source not indexed* |
| 281 | **BUG MEMORY** | *Source not indexed* |
| 282 | **ROCK MEMORY** | *Source not indexed* |
| 283 | **GHOST MEMORY** | *Source not indexed* |
| 284 | **DRAGON MEMORY** | *Source not indexed* |
| 285 | **DARK MEMORY** | *Source not indexed* |
| 286 | **STEEL MEMORY** | *Source not indexed* |
| 287 | **FAIRY MEMORY** | *Source not indexed* |
| 288 | **RUSTED SWORD** | Held by Royal Duo reward |
| 289 | **RUSTED SHIELD** | Held by Royal Duo reward |
| 290 | **RED ORB** | *Source not indexed* |
| 291 | **BLUE ORB** | *Source not indexed* |
| 292 | **VENUSAURITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| 293 | **CHARIZARDITE X** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| 294 | **CHARIZARDITE Y** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| 295 | **BLASTOISINITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| 296 | **BEEDRILLITE** | Hard rematch: LEADER KOGA |
| 297 | **PIDGEOTITE** | Hard rematch: FALKNER 1 |
| 298 | **ALAKAZITE** | Hard rematch: LEADER SABRINA |
| 299 | **SLOWBRONITE** | Hard rematch: WILL 2 |
| 300 | **GENGARITE** | Hard rematch: ELITE FOUR AGATHA 2 |
| 301 | **KANGASKHANITE** | Hard rematch: NORMAN 1 |
| 302 | **PINSIRITE** | Battle Net vendor (Hub 2F) |
| 303 | **GYARADOSITE** | Hard rematch: LEADER MISTY |
| 304 | **AERODACTYLITE** | Hard rematch: LEADER BROCK |
| 305 | **MEWTWONITE X** | Hard rematch: CHAMPION REMATCH SQUIRTLE; Hard rematch: CHAMPION REMATCH BULBASAUR; Hard rematch: CHAMPION REMATCH CHARMANDER |
| 306 | **MEWTWONITE Y** | Hard rematch: CHAMPION REMATCH SQUIRTLE; Hard rematch: CHAMPION REMATCH BULBASAUR; Hard rematch: CHAMPION REMATCH CHARMANDER |
| 307 | **AMPHAROSITE** | Hard rematch: CLAIR 1 |
| 308 | **STEELIXITE** | Hard rematch: JASMINE 1 |
| 309 | **SCIZORITE** | Hard rematch: BUGSY 1 |
| 310 | **HERACRONITE** | Hard rematch: CHUCK 1 |
| 311 | **HOUNDOOMINITE** | Hard rematch: LEADER BLAINE |
| 312 | **TYRANITARITE** | Hard rematch: SIDNEY |
| 313 | **SCEPTILITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| 314 | **BLAZIKENITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| 315 | **SWAMPERTITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| 316 | **GARDEVOIRITE** | Battle Net vendor (Hub 2F) |
| 317 | **SABLENITE** | Hard rematch: MORTY 1 |
| 318 | **MAWILITE** | Battle Net vendor (Hub 2F) |
| 319 | **AGGRONITE** | Hard rematch: ROXANNE 1 |
| 320 | **MEDICHAMITE** | Hard rematch: BRAWLY 1 |
| 321 | **MANECTITE** | Hard rematch: WATTSON 1 |
| 322 | **SHARPEDONITE** | Hard rematch: JUAN 1 |
| 323 | **CAMERUPTITE** | Hard rematch: FLANNERY 1 |
| 324 | **ALTARIANITE** | Hard rematch: WINONA 1 |
| 325 | **BANETTITE** | Hard rematch: PHOEBE |
| 326 | **ABSOLITE** | Hard rematch: KAREN 2 JT |
| 327 | **GLALITITE** | Hard rematch: GLACIA |
| 328 | **SALAMENCITE** | Hard rematch: LANCE 2 |
| 329 | **METAGROSSITE** | Hard rematch: TATE AND LIZA 1 |
| 330 | **LATIASITE** | Hard rematch: WALLACE |
| 331 | **LATIOSITE** | Hard rematch: DRAKE |
| 332 | **LOPUNNITE** | Item ball at Route 34 |
| 333 | **GARCHOMPITE** | Item ball at Dragons Den Cavern |
| 334 | **LUCARIONITE** | Item ball at Route 10 Frlg |
| 335 | **ABOMASITE** | Item ball at Seafoam Islands B 3F Frlg |
| 336 | **GALLADITE** | Item ball at Verdanturf Town |
| 337 | **AUDINITE** | Item ball at Rustboro City |
| 338 | **DIANCITE** | Item ball at Granite Cave B 1F |
| 339 | **NORMAL GEM** | *Source not indexed* |
| 340 | **FIRE GEM** | *Source not indexed* |
| 341 | **WATER GEM** | *Source not indexed* |
| 342 | **ELECTRIC GEM** | *Source not indexed* |
| 343 | **GRASS GEM** | *Source not indexed* |
| 344 | **ICE GEM** | *Source not indexed* |
| 345 | **FIGHTING GEM** | *Source not indexed* |
| 346 | **POISON GEM** | *Source not indexed* |
| 347 | **GROUND GEM** | *Source not indexed* |
| 348 | **FLYING GEM** | *Source not indexed* |
| 349 | **PSYCHIC GEM** | *Source not indexed* |
| 350 | **BUG GEM** | *Source not indexed* |
| 351 | **ROCK GEM** | *Source not indexed* |
| 352 | **GHOST GEM** | *Source not indexed* |
| 353 | **DRAGON GEM** | *Source not indexed* |
| 354 | **DARK GEM** | *Source not indexed* |
| 355 | **STEEL GEM** | *Source not indexed* |
| 356 | **FAIRY GEM** | *Source not indexed* |
| 357 | **NORMALIUM Z** | *Source not indexed* |
| 358 | **FIRIUM Z** | *Source not indexed* |
| 359 | **WATERIUM Z** | *Source not indexed* |
| 360 | **ELECTRIUM Z** | *Source not indexed* |
| 361 | **GRASSIUM Z** | *Source not indexed* |
| 362 | **ICIUM Z** | *Source not indexed* |
| 363 | **FIGHTINIUM Z** | *Source not indexed* |
| 364 | **POISONIUM Z** | *Source not indexed* |
| 365 | **GROUNDIUM Z** | *Source not indexed* |
| 366 | **FLYINIUM Z** | *Source not indexed* |
| 367 | **PSYCHIUM Z** | *Source not indexed* |
| 368 | **BUGINIUM Z** | *Source not indexed* |
| 369 | **ROCKIUM Z** | *Source not indexed* |
| 370 | **GHOSTIUM Z** | *Source not indexed* |
| 371 | **DRAGONIUM Z** | *Source not indexed* |
| 372 | **DARKINIUM Z** | *Source not indexed* |
| 373 | **STEELIUM Z** | *Source not indexed* |
| 374 | **FAIRIUM Z** | *Source not indexed* |
| 375 | **PIKANIUM Z** | *Source not indexed* |
| 376 | **EEVIUM Z** | *Source not indexed* |
| 377 | **SNORLIUM Z** | *Source not indexed* |
| 378 | **MEWNIUM Z** | *Source not indexed* |
| 379 | **DECIDIUM Z** | *Source not indexed* |
| 380 | **INCINIUM Z** | *Source not indexed* |
| 381 | **PRIMARIUM Z** | *Source not indexed* |
| 382 | **LYCANIUM Z** | *Source not indexed* |
| 383 | **MIMIKIUM Z** | *Source not indexed* |
| 384 | **KOMMONIUM Z** | *Source not indexed* |
| 385 | **TAPUNIUM Z** | *Source not indexed* |
| 386 | **SOLGANIUM Z** | *Source not indexed* |
| 387 | **LUNALIUM Z** | *Source not indexed* |
| 388 | **MARSHADIUM Z** | *Source not indexed* |
| 389 | **ALORAICHIUM Z** | *Source not indexed* |
| 390 | **PIKASHUNIUM Z** | *Source not indexed* |
| 391 | **ULTRANECROZIUM Z** | *Source not indexed* |
| 392 | **LIGHT BALL** | *Source not indexed* |
| 393 | **LEEK** | *Source not indexed* |
| 394 | **THICK CLUB** | *Source not indexed* |
| 395 | **LUCKY PUNCH** | *Source not indexed* |
| 396 | **METAL POWDER** | *Source not indexed* |
| 397 | **QUICK POWDER** | *Source not indexed* |
| 398 | **DEEP SEA SCALE** | *Source not indexed* |
| 399 | **DEEP SEA TOOTH** | *Source not indexed* |
| 400 | **SOUL DEW** | *Source not indexed* |
| 401 | **ADAMANT ORB** | *Source not indexed* |
| 402 | **LUSTROUS ORB** | *Source not indexed* |
| 403 | **GRISEOUS ORB** | *Source not indexed* |
| 404 | **SEA INCENSE** | *Source not indexed* |
| 405 | **LAX INCENSE** | Pickup at Route 38 |
| 406 | **ODD INCENSE** | *Source not indexed* |
| 407 | **ROCK INCENSE** | *Source not indexed* |
| 408 | **FULL INCENSE** | *Source not indexed* |
| 409 | **WAVE INCENSE** | *Source not indexed* |
| 410 | **ROSE INCENSE** | *Source not indexed* |
| 411 | **LUCK INCENSE** | *Source not indexed* |
| 412 | **PURE INCENSE** | *Source not indexed* |
| 413 | **RED SCARF** | Scripted gift at Slateport City Pokemon Fan Club |
| 414 | **BLUE SCARF** | Scripted gift at Slateport City Pokemon Fan Club |
| 415 | **PINK SCARF** | *Source not indexed* |
| 416 | **GREEN SCARF** | *Source not indexed* |
| 417 | **YELLOW SCARF** | *Source not indexed* |
| 418 | **MACHO BRACE** | *Source not indexed* |
| 419 | **POWER WEIGHT** | *Source not indexed* |
| 420 | **POWER BRACER** | *Source not indexed* |
| 421 | **POWER BELT** | *Source not indexed* |
| 422 | **POWER LENS** | *Source not indexed* |
| 423 | **POWER BAND** | *Source not indexed* |
| 424 | **POWER ANKLET** | *Source not indexed* |
| 425 | **SILK SCARF** | Scripted gift at Dewford Town House 2; Scripted gift at Route 29 |
| 426 | **CHARCOAL** | Pickup at Ruins Of Alph Puzzle And Reward Chambers; Scripted gift at Azalea Town House 1; Scripted gift at Lavaridge Town Herb Shop |
| 427 | **MYSTIC WATER** | *Source not indexed* |
| 428 | **MAGNET** | Scripted gift at Route 37 |
| 429 | **MIRACLE SEED** | *Source not indexed* |
| 430 | **NEVER MELT ICE** | Pickup at Ice Path B 4F; Pickup at Mt Silver Snow |
| 431 | **BLACK BELT** | Scripted gift at Lake Of Rage |
| 432 | **POISON BARB** | Scripted gift at Route 32 |
| 433 | **SOFT SAND** | Pickup at Route 34 |
| 434 | **SHARP BEAK** | Scripted gift at Route 40 |
| 435 | **TWISTED SPOON** | *Source not indexed* |
| 436 | **SILVER POWDER** | *Source not indexed* |
| 437 | **HARD STONE** | Scripted gift at Route 36 |
| 438 | **SPELL TAG** | *Source not indexed* |
| 439 | **DRAGON FANG** | Pickup at Dragons Den Cavern |
| 440 | **BLACK GLASSES** | Scripted gift at Dark Cave North Side |
| 441 | **METAL COAT** | Scripted gift at SSAqua Room SSE |
| 442 | **CHOICE BAND** | *Source not indexed* |
| 443 | **CHOICE SPECS** | *Source not indexed* |
| 444 | **CHOICE SCARF** | *Source not indexed* |
| 445 | **FLAME ORB** | *Source not indexed* |
| 446 | **TOXIC ORB** | *Source not indexed* |
| 447 | **DAMP ROCK** | *Source not indexed* |
| 448 | **HEAT ROCK** | *Source not indexed* |
| 449 | **SMOOTH ROCK** | *Source not indexed* |
| 450 | **ICY ROCK** | *Source not indexed* |
| 451 | **ELECTRIC SEED** | *Source not indexed* |
| 452 | **PSYCHIC SEED** | *Source not indexed* |
| 453 | **MISTY SEED** | *Source not indexed* |
| 454 | **GRASSY SEED** | *Source not indexed* |
| 455 | **ABSORB BULB** | *Source not indexed* |
| 456 | **CELL BATTERY** | *Source not indexed* |
| 457 | **LUMINOUS MOSS** | *Source not indexed* |
| 458 | **SNOWBALL** | *Source not indexed* |
| 459 | **BRIGHT POWDER** | *Source not indexed* |
| 460 | **WHITE HERB** | *Source not indexed* |
| 461 | **EXP SHARE** | *Source not indexed* |
| 462 | **QUICK CLAW** | *Source not indexed* |
| 463 | **SOOTHE BELL** | Pickup at National Park Normal |
| 464 | **MENTAL HERB** | Scripted gift at Fortree City House 4 |
| 465 | **KINGS ROCK** | Pickup at Slowpoke Well B 2F |
| 466 | **AMULET COIN** | Pickup at Goldenrod City Department Store Basement |
| 467 | **CLEANSE TAG** | Scripted gift at Mt Pyre 1F |
| 468 | **SMOKE BALL** | *Source not indexed* |
| 469 | **FOCUS BAND** | Scripted gift at Shoal Cave Low Tide Lower Room |
| 470 | **LUCKY EGG** | *Source not indexed* |
| 471 | **SCOPE LENS** | *Source not indexed* |
| 472 | **LEFTOVERS** | *Source not indexed* |
| 473 | **SHELL BELL** | *Source not indexed* |
| 474 | **WIDE LENS** | *Source not indexed* |
| 475 | **MUSCLE BAND** | *Source not indexed* |
| 476 | **WISE GLASSES** | *Source not indexed* |
| 477 | **EXPERT BELT** | *Source not indexed* |
| 478 | **LIGHT CLAY** | *Source not indexed* |
| 479 | **LIFE ORB** | *Source not indexed* |
| 480 | **POWER HERB** | *Source not indexed* |
| 481 | **FOCUS SASH** | *Source not indexed* |
| 482 | **ZOOM LENS** | *Source not indexed* |
| 483 | **METRONOME** | *Source not indexed* |
| 484 | **IRON BALL** | *Source not indexed* |
| 485 | **LAGGING TAIL** | *Source not indexed* |
| 486 | **DESTINY KNOT** | *Source not indexed* |
| 487 | **BLACK SLUDGE** | *Source not indexed* |
| 488 | **GRIP CLAW** | *Source not indexed* |
| 489 | **STICKY BARB** | *Source not indexed* |
| 490 | **SHED SHELL** | *Source not indexed* |
| 491 | **BIG ROOT** | *Source not indexed* |
| 492 | **RAZOR CLAW** | *Source not indexed* |
| 493 | **RAZOR FANG** | *Source not indexed* |
| 494 | **EVIOLITE** | *Source not indexed* |
| 495 | **FLOAT STONE** | *Source not indexed* |
| 496 | **ROCKY HELMET** | *Source not indexed* |
| 497 | **AIR BALLOON** | *Source not indexed* |
| 498 | **RED CARD** | *Source not indexed* |
| 499 | **RING TARGET** | *Source not indexed* |
| 500 | **BINDING BAND** | *Source not indexed* |
| 501 | **EJECT BUTTON** | *Source not indexed* |
| 502 | **WEAKNESS POLICY** | *Source not indexed* |
| 503 | **ASSAULT VEST** | *Source not indexed* |
| 504 | **SAFETY GOGGLES** | *Source not indexed* |
| 505 | **ADRENALINE ORB** | *Source not indexed* |
| 506 | **TERRAIN EXTENDER** | *Source not indexed* |
| 507 | **PROTECTIVE PADS** | *Source not indexed* |
| 508 | **THROAT SPRAY** | *Source not indexed* |
| 509 | **EJECT PACK** | *Source not indexed* |
| 510 | **HEAVY DUTY BOOTS** | *Source not indexed* |
| 511 | **BLUNDER POLICY** | *Source not indexed* |
| 512 | **ROOM SERVICE** | *Source not indexed* |
| 513 | **UTILITY UMBRELLA** | *Source not indexed* |
| 515 | **CHESTO BERRY** | *Source not indexed* |
| 516 | **PECHA BERRY** | Pickup at Violet City |
| 517 | **RAWST BERRY** | *Source not indexed* |
| 518 | **ASPEAR BERRY** | *Source not indexed* |
| 519 | **LEPPA BERRY** | *Source not indexed* |
| 520 | **ORAN BERRY** | *Source not indexed* |
| 521 | **PERSIM BERRY** | *Source not indexed* |
| 522 | **LUM BERRY** | *Source not indexed* |
| 523 | **SITRUS BERRY** | *Source not indexed* |
| 524 | **FIGY BERRY** | *Source not indexed* |
| 525 | **WIKI BERRY** | *Source not indexed* |
| 526 | **MAGO BERRY** | *Source not indexed* |
| 527 | **AGUAV BERRY** | *Source not indexed* |
| 528 | **IAPAPA BERRY** | Scripted gift at Three Island Berry Forest Frlg |
| 529 | **RAZZ BERRY** | *Source not indexed* |
| 530 | **BLUK BERRY** | *Source not indexed* |
| 531 | **NANAB BERRY** | *Source not indexed* |
| 532 | **WEPEAR BERRY** | *Source not indexed* |
| 533 | **PINAP BERRY** | *Source not indexed* |
| 534 | **POMEG BERRY** | *Source not indexed* |
| 535 | **KELPSY BERRY** | *Source not indexed* |
| 536 | **QUALOT BERRY** | *Source not indexed* |
| 537 | **HONDEW BERRY** | *Source not indexed* |
| 538 | **GREPA BERRY** | *Source not indexed* |
| 539 | **TAMATO BERRY** | *Source not indexed* |
| 540 | **CORNN BERRY** | *Source not indexed* |
| 541 | **MAGOST BERRY** | *Source not indexed* |
| 542 | **RABUTA BERRY** | *Source not indexed* |
| 543 | **NOMEL BERRY** | *Source not indexed* |
| 544 | **SPELON BERRY** | Scripted gift at Route 123 Berry Masters House |
| 545 | **PAMTRE BERRY** | Scripted gift at Route 123 Berry Masters House |
| 546 | **WATMEL BERRY** | *Source not indexed* |
| 547 | **DURIN BERRY** | *Source not indexed* |
| 548 | **BELUE BERRY** | *Source not indexed* |
| 549 | **CHILAN BERRY** | *Source not indexed* |
| 550 | **OCCA BERRY** | *Source not indexed* |
| 551 | **PASSHO BERRY** | *Source not indexed* |
| 552 | **WACAN BERRY** | *Source not indexed* |
| 553 | **RINDO BERRY** | *Source not indexed* |
| 554 | **YACHE BERRY** | *Source not indexed* |
| 555 | **CHOPLE BERRY** | *Source not indexed* |
| 556 | **KEBIA BERRY** | *Source not indexed* |
| 557 | **SHUCA BERRY** | *Source not indexed* |
| 558 | **COBA BERRY** | *Source not indexed* |
| 559 | **PAYAPA BERRY** | *Source not indexed* |
| 560 | **TANGA BERRY** | *Source not indexed* |
| 561 | **CHARTI BERRY** | *Source not indexed* |
| 562 | **KASIB BERRY** | *Source not indexed* |
| 563 | **HABAN BERRY** | *Source not indexed* |
| 564 | **COLBUR BERRY** | *Source not indexed* |
| 565 | **BABIRI BERRY** | *Source not indexed* |
| 566 | **ROSELI BERRY** | *Source not indexed* |
| 567 | **LIECHI BERRY** | *Source not indexed* |
| 568 | **GANLON BERRY** | *Source not indexed* |
| 569 | **SALAC BERRY** | *Source not indexed* |
| 570 | **PETAYA BERRY** | *Source not indexed* |
| 571 | **APICOT BERRY** | *Source not indexed* |
| 572 | **LANSAT BERRY** | Scripted gift at Battle Frontier Scotts House |
| 573 | **STARF BERRY** | Scripted gift at Battle Frontier Scotts House |
| 574 | **ENIGMA BERRY** | *Source not indexed* |
| 575 | **MICLE BERRY** | *Source not indexed* |
| 576 | **CUSTAP BERRY** | *Source not indexed* |
| 577 | **JABOCA BERRY** | *Source not indexed* |
| 578 | **ROWAP BERRY** | *Source not indexed* |
| 579 | **KEE BERRY** | *Source not indexed* |
| 580 | **MARANGA BERRY** | *Source not indexed* |
| 581 | **ENIGMA BERRY E READER** | *Source not indexed* |
| 582 | **TM 01** | *Source not indexed* |
| 583 | **TM 02** | *Source not indexed* |
| 584 | **TM 03** | *Source not indexed* |
| 585 | **TM 04** | *Source not indexed* |
| 586 | **TM 05** | *Source not indexed* |
| 587 | **TM 06** | *Source not indexed* |
| 588 | **TM 07** | *Source not indexed* |
| 589 | **TM 08** | *Source not indexed* |
| 590 | **TM 09** | *Source not indexed* |
| 591 | **TM 10** | *Source not indexed* |
| 592 | **TM 11** | *Source not indexed* |
| 593 | **TM 12** | *Source not indexed* |
| 594 | **TM 13** | *Source not indexed* |
| 595 | **TM 14** | *Source not indexed* |
| 596 | **TM 15** | *Source not indexed* |
| 597 | **TM 16** | *Source not indexed* |
| 598 | **TM 17** | *Source not indexed* |
| 599 | **TM 18** | *Source not indexed* |
| 600 | **TM 19** | *Source not indexed* |
| 601 | **TM 20** | *Source not indexed* |
| 602 | **TM 21** | *Source not indexed* |
| 603 | **TM 22** | *Source not indexed* |
| 604 | **TM 23** | *Source not indexed* |
| 605 | **TM 24** | *Source not indexed* |
| 606 | **TM 25** | *Source not indexed* |
| 607 | **TM 26** | *Source not indexed* |
| 608 | **TM 27** | *Source not indexed* |
| 609 | **TM 28** | *Source not indexed* |
| 610 | **TM 29** | Pickup at Johto Victory Road B 1F |
| 611 | **TM 30** | *Source not indexed* |
| 612 | **TM 31** | *Source not indexed* |
| 613 | **TM 32** | *Source not indexed* |
| 614 | **TM 33** | *Source not indexed* |
| 615 | **TM 34** | *Source not indexed* |
| 616 | **TM 35** | *Source not indexed* |
| 617 | **TM 36** | *Source not indexed* |
| 618 | **TM 37** | *Source not indexed* |
| 619 | **TM 38** | *Source not indexed* |
| 620 | **TM 39** | *Source not indexed* |
| 621 | **TM 40** | *Source not indexed* |
| 622 | **TM 41** | *Source not indexed* |
| 623 | **TM 42** | Scripted gift at Five Island Memorial Pillar Frlg |
| 624 | **TM 43** | *Source not indexed* |
| 625 | **TM 44** | *Source not indexed* |
| 626 | **TM 45** | *Source not indexed* |
| 627 | **TM 46** | *Source not indexed* |
| 628 | **TM 47** | *Source not indexed* |
| 629 | **TM 48** | *Source not indexed* |
| 630 | **TM 49** | *Source not indexed* |
| 631 | **TM 50** | *Source not indexed* |
| 632 | **TM 51** | *Source not indexed* |
| 633 | **TM 52** | *Source not indexed* |
| 634 | **TM 53** | *Source not indexed* |
| 635 | **TM 54** | *Source not indexed* |
| 636 | **TM 55** | *Source not indexed* |
| 637 | **TM 56** | *Source not indexed* |
| 638 | **TM 57** | *Source not indexed* |
| 639 | **TM 58** | *Source not indexed* |
| 640 | **TM 59** | *Source not indexed* |
| 641 | **TM 60** | *Source not indexed* |
| 642 | **TM 61** | *Source not indexed* |
| 643 | **TM 62** | *Source not indexed* |
| 644 | **TM 63** | *Source not indexed* |
| 645 | **TM 64** | *Source not indexed* |
| 646 | **TM 65** | *Source not indexed* |
| 647 | **TM 66** | *Source not indexed* |
| 648 | **TM 67** | *Source not indexed* |
| 649 | **TM 68** | *Source not indexed* |
| 650 | **TM 69** | *Source not indexed* |
| 651 | **TM 70** | *Source not indexed* |
| 652 | **TM 71** | *Source not indexed* |
| 653 | **TM 72** | *Source not indexed* |
| 654 | **TM 73** | *Source not indexed* |
| 655 | **TM 74** | *Source not indexed* |
| 656 | **TM 75** | *Source not indexed* |
| 657 | **TM 76** | *Source not indexed* |
| 658 | **TM 77** | *Source not indexed* |
| 659 | **TM 78** | *Source not indexed* |
| 660 | **TM 79** | *Source not indexed* |
| 661 | **TM 80** | *Source not indexed* |
| 662 | **TM 81** | *Source not indexed* |
| 663 | **TM 82** | *Source not indexed* |
| 664 | **TM 83** | *Source not indexed* |
| 665 | **TM 84** | *Source not indexed* |
| 666 | **TM 85** | *Source not indexed* |
| 667 | **TM 86** | *Source not indexed* |
| 668 | **TM 87** | *Source not indexed* |
| 669 | **TM 88** | *Source not indexed* |
| 670 | **TM 89** | *Source not indexed* |
| 671 | **TM 90** | *Source not indexed* |
| 672 | **TM 91** | *Source not indexed* |
| 673 | **TM 92** | *Source not indexed* |
| 674 | **TM 93** | *Source not indexed* |
| 675 | **TM 94** | *Source not indexed* |
| 676 | **TM 95** | *Source not indexed* |
| 677 | **TM 96** | *Source not indexed* |
| 678 | **TM 97** | *Source not indexed* |
| 679 | **TM 98** | *Source not indexed* |
| 680 | **TM 99** | *Source not indexed* |
| 681 | **TM 100** | *Source not indexed* |
| 682 | **HM 01** | Scripted gift at SSAnne Captains Office Frlg |
| 683 | **HM 02** | Scripted gift at Route 16 House Frlg |
| 684 | **HM 03** | Scripted gift at Safari Zone Secret House Frlg |
| 685 | **HM 04** | *Source not indexed* |
| 686 | **HM 05** | Scripted gift at Route 2 East Building Frlg |
| 687 | **HM 06** | *Source not indexed* |
| 688 | **HM 07** | *Source not indexed* |
| 689 | **HM 08** | *Source not indexed* |
| 690 | **OVAL CHARM** | *Source not indexed* |
| 691 | **SHINY CHARM** | *Source not indexed* |
| 692 | **CATCHING CHARM** | *Source not indexed* |
| 693 | **EXP CHARM** | *Source not indexed* |
| 694 | **ROTOM CATALOG** | *Source not indexed* |
| 695 | **GRACIDEA** | *Source not indexed* |
| 696 | **REVEAL GLASS** | *Source not indexed* |
| 697 | **DNA SPLICERS** | Scripted gift at Ice Path B 1F |
| 698 | **ZYGARDE CUBE** | *Source not indexed* |
| 699 | **PRISON BOTTLE** | *Source not indexed* |
| 700 | **N SOLARIZER** | *Source not indexed* |
| 701 | **N LUNARIZER** | *Source not indexed* |
| 702 | **REINS OF UNITY** | *Source not indexed* |
| 703 | **MEGA RING** | *Source not indexed* |
| 704 | **Z POWER RING** | *Source not indexed* |
| 705 | **DYNAMAX BAND** | *Source not indexed* |
| 706 | **BICYCLE** | *Source not indexed* |
| 707 | **MACH BIKE** | Scripted gift at Goldenrod City Bike Shop; Scripted gift at Mauville City Bike Shop |
| 708 | **ACRO BIKE** | Scripted gift at Mauville City Bike Shop |
| 709 | **OLD ROD** | Scripted gift at Route 32 Pokemon Center; Scripted gift at Vermilion City House 1 Frlg |
| 710 | **GOOD ROD** | Scripted gift at Fuchsia City House 2 Frlg; Scripted gift at Olivine City House 3 |
| 711 | **SUPER ROD** | Scripted gift at Mossdeep City House 3; Scripted gift at Route 12 Fishing House Frlg |
| 712 | **DOWSING MACHINE** | Scripted gift at Route 110 |
| 713 | **TOWN MAP** | *Source not indexed* |
| 714 | **VS SEEKER** | *Source not indexed* |
| 715 | **TM CASE** | *Source not indexed* |
| 716 | **BERRY POUCH** | *Source not indexed* |
| 717 | **POKEMON BOX LINK** | *Source not indexed* |
| 718 | **COIN CASE** | Pickup at Goldenrod City Underground Tunnel; Scripted gift at Celadon City Restaurant Frlg; Scripted gift at Mauville City House 2 |
| 719 | **POWDER JAR** | *Source not indexed* |
| 720 | **WAILMER PAIL** | Scripted gift at Goldenrod City Flower Shop |
| 721 | **POKE RADAR** | *Source not indexed* |
| 722 | **POKEBLOCK CASE** | *Source not indexed* |
| 723 | **SOOT SACK** | Scripted gift at Route 113 Glass Workshop |
| 724 | **POKE FLUTE** | *Source not indexed* |
| 725 | **FAME CHECKER** | *Source not indexed* |
| 726 | **TEACHY TV** | *Source not indexed* |
| 727 | **SS TICKET** | *Source not indexed* |
| 728 | **EON TICKET** | *Source not indexed* |
| 729 | **MYSTIC TICKET** | *Source not indexed* |
| 730 | **AURORA TICKET** | *Source not indexed* |
| 731 | **OLD SEA MAP** | *Source not indexed* |
| 732 | **LETTER** | *Source not indexed* |
| 733 | **DEVON PARTS** | *Source not indexed* |
| 734 | **GO GOGGLES** | Scripted gift at Lavaridge Town |
| 735 | **DEVON SCOPE** | Scripted gift at Route 120 |
| 736 | **BASEMENT KEY** | *Source not indexed* |
| 737 | **SCANNER** | *Source not indexed* |
| 738 | **STORAGE KEY** | *Source not indexed* |
| 739 | **KEY TO ROOM 1** | *Source not indexed* |
| 740 | **KEY TO ROOM 2** | *Source not indexed* |
| 741 | **KEY TO ROOM 4** | *Source not indexed* |
| 742 | **KEY TO ROOM 6** | *Source not indexed* |
| 743 | **METEORITE** | *Source not indexed* |
| 744 | **MAGMA EMBLEM** | *Source not indexed* |
| 745 | **CONTEST PASS** | *Source not indexed* |
| 746 | **PARCEL** | *Source not indexed* |
| 747 | **SECRET KEY** | *Source not indexed* |
| 748 | **BIKE VOUCHER** | *Source not indexed* |
| 749 | **GOLD TEETH** | *Source not indexed* |
| 750 | **CARD KEY** | *Source not indexed* |
| 751 | **LIFT KEY** | *Source not indexed* |
| 752 | **SILPH SCOPE** | Pickup at Rocket Hideout B 4F Frlg |
| 753 | **TRI PASS** | *Source not indexed* |
| 754 | **RAINBOW PASS** | *Source not indexed* |
| 755 | **TEA** | *Source not indexed* |
| 756 | **RUBY** | Scripted gift at Mt Ember Ruby Path B 3F Frlg |
| 757 | **SAPPHIRE** | *Source not indexed* |
| 758 | **ABILITY SHIELD** | *Source not indexed* |
| 759 | **CLEAR AMULET** | *Source not indexed* |
| 760 | **PUNCHING GLOVE** | *Source not indexed* |
| 761 | **COVERT CLOAK** | *Source not indexed* |
| 762 | **LOADED DICE** | *Source not indexed* |
| 763 | **AUSPICIOUS ARMOR** | *Source not indexed* |
| 764 | **BOOSTER ENERGY** | *Source not indexed* |
| 765 | **BIG BAMBOO SHOOT** | *Source not indexed* |
| 766 | **GIMMIGHOUL COIN** | *Source not indexed* |
| 767 | **LEADERS CREST** | *Source not indexed* |
| 768 | **MALICIOUS ARMOR** | *Source not indexed* |
| 769 | **MIRROR HERB** | *Source not indexed* |
| 770 | **SCROLL OF DARKNESS** | Scripted gift at Mt Mortar B 1F |
| 771 | **SCROLL OF WATERS** | Scripted gift at Cianwood Gym |
| 772 | **TERA ORB** | *Source not indexed* |
| 773 | **TINY BAMBOO SHOOT** | *Source not indexed* |
| 774 | **BUG TERA SHARD** | *Source not indexed* |
| 775 | **DARK TERA SHARD** | *Source not indexed* |
| 776 | **DRAGON TERA SHARD** | *Source not indexed* |
| 777 | **ELECTRIC TERA SHARD** | *Source not indexed* |
| 778 | **FAIRY TERA SHARD** | *Source not indexed* |
| 779 | **FIGHTING TERA SHARD** | *Source not indexed* |
| 780 | **FIRE TERA SHARD** | *Source not indexed* |
| 781 | **FLYING TERA SHARD** | *Source not indexed* |
| 782 | **GHOST TERA SHARD** | *Source not indexed* |
| 783 | **GRASS TERA SHARD** | *Source not indexed* |
| 784 | **GROUND TERA SHARD** | *Source not indexed* |
| 785 | **ICE TERA SHARD** | *Source not indexed* |
| 786 | **NORMAL TERA SHARD** | *Source not indexed* |
| 787 | **POISON TERA SHARD** | *Source not indexed* |
| 788 | **PSYCHIC TERA SHARD** | *Source not indexed* |
| 789 | **ROCK TERA SHARD** | *Source not indexed* |
| 790 | **STEEL TERA SHARD** | *Source not indexed* |
| 791 | **WATER TERA SHARD** | *Source not indexed* |
| 792 | **ADAMANT CRYSTAL** | *Source not indexed* |
| 793 | **GRISEOUS CORE** | *Source not indexed* |
| 794 | **LUSTROUS GLOBE** | *Source not indexed* |
| 795 | **BLACK AUGURITE** | *Source not indexed* |
| 796 | **LINKING CORD** | *Source not indexed* |
| 797 | **PEAT BLOCK** | *Source not indexed* |
| 798 | **BERSERK GENE** | *Source not indexed* |
| 799 | **FAIRY FEATHER** | *Source not indexed* |
| 800 | **SYRUPY APPLE** | *Source not indexed* |
| 801 | **UNREMARKABLE TEACUP** | *Source not indexed* |
| 802 | **MASTERPIECE TEACUP** | *Source not indexed* |
| 803 | **CORNERSTONE MASK** | *Source not indexed* |
| 804 | **WELLSPRING MASK** | *Source not indexed* |
| 805 | **HEARTHFLAME MASK** | *Source not indexed* |
| 806 | **HEALTH MOCHI** | *Source not indexed* |
| 807 | **MUSCLE MOCHI** | *Source not indexed* |
| 808 | **RESIST MOCHI** | *Source not indexed* |
| 809 | **GENIUS MOCHI** | *Source not indexed* |
| 810 | **CLEVER MOCHI** | *Source not indexed* |
| 811 | **SWIFT MOCHI** | *Source not indexed* |
| 812 | **FRESH START MOCHI** | *Source not indexed* |
| 813 | **GLIMMERING CHARM** | *Source not indexed* |
| 814 | **METAL ALLOY** | *Source not indexed* |
| 815 | **STELLAR TERA SHARD** | *Source not indexed* |
| 816 | **JUBILIFE MUFFIN** | *Source not indexed* |
| 817 | **REMEDY** | *Source not indexed* |
| 818 | **FINE REMEDY** | *Source not indexed* |
| 819 | **SUPERB REMEDY** | *Source not indexed* |
| 820 | **AUX EVASION** | *Source not indexed* |
| 821 | **AUX GUARD** | *Source not indexed* |
| 822 | **AUX POWER** | *Source not indexed* |
| 823 | **AUX POWERGUARD** | *Source not indexed* |
| 824 | **CHOICE DUMPLING** | *Source not indexed* |
| 825 | **SWAP SNACK** | *Source not indexed* |
| 826 | **TWICE SPICED RADISH** | *Source not indexed* |
| 827 | **POKESHI DOLL** | *Source not indexed* |
| 828 | **STRANGE BALL** | *Source not indexed* |
| 829 | **CLEFABLITE** | Item ball at Mt Moon 1F Frlg |
| 830 | **VICTREEBELITE** | Item ball at Celadon City Frlg |
| 831 | **STARMINITE** | Item ball at Route 20 Frlg |
| 832 | **DRAGONINITE** | Battle Net vendor (Hub 2F) |
| 833 | **MEGANIUMITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| 834 | **FERALIGITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| 835 | **SKARMORITE** | Item ball at Route 45 |
| 836 | **FROSLASSITE** | Item ball at Ice Path B 1F |
| 837 | **EMBOARITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| 838 | **EXCADRITE** | Item ball at Union Cave 1F |
| 839 | **SCOLIPITE** | Item ball at Ilex Forest |
| 840 | **SCRAFTINITE** | Item ball at Goldenrod City Underground Tunnel |
| 841 | **EELEKTROSSITE** | Item ball at New Mauville Inside |
| 842 | **CHANDELURITE** | Item ball at Pokemon Tower 4F Frlg |
| 843 | **CHESNAUGHTITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| 844 | **DELPHOXITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| 845 | **GRENINJITE** | Free starter stone (Hub 2F); Battle Net vendor (Hub 2F) |
| 846 | **PYROARITE** | Item ball at Mt Chimney |
| 847 | **FLOETTITE** | Item ball at Goldenrod City Flower Shop |
| 848 | **MALAMARITE** | Item ball at Abandoned Ship Rooms 1F |
| 849 | **BARBARACITE** | Item ball at Route 106 |
| 850 | **DRAGALGITE** | Item ball at Underwater Route 124 |
| 851 | **HAWLUCHANITE** | Item ball at Saffron City Dojo Frlg |
| 852 | **ZYGARDITE** | Item ball at Route 101; Pickup at Route 101 |
| 853 | **DRAMPANITE** | Item ball at Mt Mortar 1F South |
| 854 | **FALINKSITE** | Item ball at Route 23 Frlg |
| 855 | **HEATRANITE** | Item ball at Rock Tunnel B 1F Frlg; Pickup at Rock Tunnel B 1F Frlg |
| 856 | **DARKRANITE** | Item ball at Pokemon Tower 7F Frlg; Pickup at Pokemon Tower 7F Frlg |
| 857 | **ZERAORITE** | Item ball at Power Plant Frlg; Pickup at Power Plant Frlg |
| 858 | **RAICHUNITE X** | Battle Net vendor (Hub 2F) |
| 859 | **RAICHUNITE Y** | Battle Net vendor (Hub 2F) |
| 860 | **CHIMECHITE** | Item ball at Mt Pyre 5F |
| 861 | **ABSOLITE Z** | Item ball at Burned Tower 1F |
| 862 | **STARAPTITE** | Item ball at Route 16 Frlg |
| 863 | **GARCHOMPITE Z** | Battle Net vendor (Hub 2F) |
| 864 | **LUCARIONITE Z** | Battle Net vendor (Hub 2F) |
| 865 | **GOLURKITE** | Item ball at Ruins Of Alph Outside |
| 866 | **MEOWSTICITE** | Item ball at Saffron City Frlg |
| 867 | **CRABOMINITE** | Item ball at Shoal Cave Low Tide Ice Room |
| 868 | **GOLISOPITE** | Item ball at Whirl Islands 1F |
| 869 | **MAGEARNITE** | Item ball at Silph Co 7F Frlg; Pickup at Silph Co 7F Frlg |
| 870 | **SCOVILLAINITE** | Item ball at Route 112 |
| 871 | **BAXCALIBRITE** | Item ball at Meteor Falls 1F 1R |
| 872 | **TATSUGIRINITE** | Item ball at Route 118 |
| 873 | **GLIMMORANITE** | Item ball at Victory Road 2F Frlg |
| 874 | **POKEVIAL** | *Source not indexed* |
| 881 | **GS BALL** | *Source not indexed* |

</details>

## Reading the evidence
**A code source is not gameplay acceptance.** Native wild tables, supplemental habitats, static scripts, gifts and items may be gated, unreachable, or overridden at runtime. The machine catalog records original source paths. A known exception is Mew Nine Masks, whose capture/revisit was reported working by the maintainer. Encounter randomization and cheats do not constitute ordinary acquisition paths.
For individual flags and implementation details, use the [structured source atlas](../data/catalog/world_acquisition.jsonl) and [catalog instructions](../data/catalog/README.md). The human and Astra versions should be updated together when locations, rewards or gates change.
