> **Publication note, 24 October 2026:** This 2026-10-24 design record was accumulated during a user-requested discussion-only pause. The maintainer subsequently explicitly authorized **publishing the ledger and a documentation handoff to the public repository**. Prior "do not access/modify GitHub" statements in individual dated sections record the earlier pause; they do **not** prohibit this documentation commit. They also do **not** authorize gameplay implementation. Later sections supersede earlier tentative designs. **See [the current handoff](NEXT_ASTRA_HANDOFF.md) for instructions and authority levels.**

# Pokémon Grand Anthology: discussion-only design ledger

**Recorded:** 2026-10-24  
**Source baseline:** `DevElectrum/grand-anthology`, public `main`, handoff `8825f648e997cd0cd1c6c992fe476d4a4b9e0e3d`, as inspected in earlier discussion.  
**Authority:** User and assistant design conversation; this is **not** a repository document or implementation authorization.  
**Absolute instruction:** Do **not** edit repository code, maps, scripts, docs, issues, PRs, or anything on GitHub until user explicitly authorizes repository work. Continue discussing and updating **offline notes only**. No build, CI, or tests from this discussion.

## 1. Design principles agreed

- Continuous Kanto → Johto → Hoenn adventure with a persistent party, trainer and save. Legendary/Mythical encounter planning should spread interesting discoveries **throughout each region** and its postgame. **Do not** gate powerful Pokémon merely to avoid early access, or assume every species must be attainable before the **first** Champion. The ultimate aim remains all 1,025 species across the full adventure.
- Prefer native map accessibility and familiar story rewards, plus a small number of meaningful lore-linked capture dependencies, to arbitrary badge locks. Encounter levels should suit the **point of acquisition**. Revisits can be rewarding if natural.
- Legendary/Mythical species ordinarily use **scripted static** encounters. **Species-by-species lore exception:** a species documented to have multiple individuals may be eligible for natural wild/discovery encounters. Keep the post-Champion discovery rotation and targeted Research mechanism, but review which Legendary/Mythical species they can select; do not blanket-disable both systems. Ultra Beasts and Paradox Pokémon remain rare naturally occurring Pokémon, deliberately including Kartana in Viridian Forest and Poipole on Route 1.
- Remove these **five ordinary wild habitat entries**, preserving appropriate statics: Groudon at Hoenn Route 111; Kyogre at Seafloor Cavern Room 1; Galarian Zapdos at Kanto Route 22; Galarian Articuno at Seafoam B1F; Galarian Moltres at Pokémon Mansion 1F. Their static/story acquisition sources require separate review, not automatic deletion.
- Region-generation allocation: **Kanto Gen 1+4+7**, Johto Gen 2+5+8, Hoenn Gen 3+6+9. Review one region, generation by generation, then an integrated geographical and progression audit.
- Keep ordinary campaign plots and native puzzles. Existing location/map source is an authored candidate, **not runtime-verified playability**.
- Existing PC capacity (420) remains for now; extended PC, Tera and Gigantamax work are late priorities. No code work authorized during design.

## 2. Kanto Generation 1: accepted provisional arrangement

| Species | Planned location | Availability and relationship | Encounter level | Notes |
| --- | --- | --- | --- | --- |
| Articuno | Seafoam Islands B4F, existing native static | Proposed six Kanto badges; original Strength/Surf puzzle and access retained | 40 | Keep classic location. Badge minimum was accepted as fine primarily to preserve original, not as a new universal strength rule. |
| Zapdos | Power Plant, existing native static | Proposed five Kanto badges, naturally after Surf | 35 | Keep classic location. |
| Moltres | Mt. Ember summit, existing native static | Proposed seven Kanto badges, naturally after Blaine/Sevii travel | 45 | Keep classic location. |
| Mew | Pokémon Mansion 1F | Proposed seven Kanto badges | 40 | This location remains despite Mew's original Faraway Island event source. |
| Mewtwo | Cerulean Cave B1F | Kanto Champion **and Mew caught**, with existing Celio/Sevii Network Machine cave guard preserved | 65 | **Remove or disable pre-League duplicate Mewtwo static at Pokémon Mansion 1F.** Mew caught means Pokédex caught registration, not necessarily in current party/PC. |

Exact enforcement of the tentative Gen 1 badge floors should be reassessed against natural access and existing story flags; user accepted leaving the original birds largely unchanged.

## 3. Kanto Generation 4: location, dependencies, tentative levels

| Species | Intended location | Agreed requirements / timing | Suggested level | Decision status |
| --- | --- | --- | --- | --- |
| **Shaymin** | **Move Route 5 → Viridian Forest** | Available **before Brock**, first visit; no badge gate | 8–10 | **Agreed** relocation and early availability. Sky Forme handling not settled. |
| **Uxie** | **Keep Rock Tunnel B1F** | Three Kanto badges tentatively | 25 | Location accepted; exact badge/level provisional. |
| **Mesprit** | **Keep Rock Tunnel B1F** | Four Kanto badges tentatively | 30 | Location accepted; exact badge/level provisional. |
| **Azelf** | **Keep Rock Tunnel B1F** | Five Kanto badges tentatively | 35 | Location accepted; exact badge/level provisional. |
| **Heatran** | **Keep Rock Tunnel B1F** | Around midgame; previously proposed six badges, but user questioned strength rationale | 42 | Location agreed; unlock/level not final. |
| **Cresselia** | Mt. Moon B1F, as currently authored | Around midgame; previous four-badge proposal | 30 | No relocation agreed. Catch is prerequisite for Darkrai. |
| **Manaphy** | Seafoam Islands B2F, currently authored | Available with natural Seafoam access unless another meaningful reason to gate | 38 proposed | No relocation agreed; actual authored level may differ. |
| **Phione** | Breed Manaphy + Ditto at two-parent daycare | Subject to acquiring Manaphy and usable breeding | Egg | Verify actual in-game egg/pickup/hatching flow later. |
| **Dialga** | Mt. Moon B1F, currently authored | Late Kanto; **previous** seven-badge suggestion | 48 | Later gate still a proposal; required for Giratina and Arceus. |
| **Palkia** | Mt. Moon B1F, currently authored | Late Kanto; **previous** seven-badge suggestion | 48 | Later gate still a proposal; required for Giratina and Arceus. |
| **Giratina** | **Move Rock Tunnel B1F → Mt. Moon B2F** | **Catch Dialga and Palkia**, proposed eight Kanto badges | 52 | **Agreed** location and capture dependency; avoid original Rocket/fossil script tiles. |
| **Darkrai** | **Move Rock Tunnel B1F → Pokémon Tower 7F** | **Catch Cresselia** and rescue Mr. Fuji; previous seven-badge suggestion | 45 | Location was strongly proposed/positively advanced, but user did not explicitly say “yes” to this exact floor; confirm if needed. Do not interfere with Marowak ghost on 6F or Fuji/Rocket story on 7F. |
| **Regigigas** | **Keep Rock Tunnel B1F** | **Catch Regirock + Regice + Registeel**, requiring Hoenn progression | 40 **outdated** | Agreed dependency/location. Revisit level and timing because encounter is now realistically cross-region. |
| **Arceus** | Indigo Plateau exterior, currently authored | **Kanto Champion + caught Dialga, Palkia and Giratina** | 70 | Agreed caught-trio dependency; high-level postgame proposal. |

**Important:** Initial power-based 2/3/4/5/6/7/8-badge proposals were made *before* user clarified that the design purpose is even distribution of exploration/rewards, **not** barring strong Pokémon. Do not elevate those badge numbers to inflexible accepted requirements without review. Cross-region dependencies and native story access take precedence.

## 4. Kanto Generation 7: accepted or proposed placement

| Species | Intended source | Access, evolution, or encounter level | Decision status |
| --- | --- | --- | --- |
| **Cosmog #1** | Keep Mt. Moon 1F static | Early first pass through Mt. Moon; ~level 5 suggested | Accepted in principle, no extra badge gate. |
| **Cosmog #2** | Keep Seafoam Islands 1F static | Naturally with Surf/Seafoam access; second copy for other evolution branch; ~level 35 suggested | Accepted in principle. |
| Cosmoem | Evolve Cosmog | Level 43 | Evolution, not additional field static. |
| Solgaleo | Evolve Cosmoem | Level 53, relevant branch | Ensure branch obtainable in this game. |
| Lunala | Evolve second Cosmoem | Level 53, other branch | Ensure branch obtainable in this game. |
| **Tapu Bulu** | Keep Route 5 static | First natural Route 5 access, rather than prior proposed 4-badge restriction | Accepted location/access; early-appropriate encounter level still needs review. |
| **Tapu Lele** | **Move Rock Tunnel B1F → Celadon City gardens / near Erika's Gym** | Naturally available with Celadon access rather than a late arbitrary lock | Agreed relocation in principle; exact tile TBD. |
| **Tapu Koko** | Keep Power Plant static | Naturally available once Power Plant accessible with Surf | User explicitly says Koko is fine. Original proposed level 38 / five badges not rigid. |
| **Zeraora** | Keep Power Plant static for now | Original proposed level 42; natural Power Plant access plausible | No relocation decision; avoid unnecessary artificial badge gate. |
| **Tapu Fini** | Keep Seafoam Islands B2F static | Natural Seafoam access; currently authored encounter level **45** | User accepted keeping location. B2F also hosts Manaphy. |
| **Type: Null** | Keep Pokémon Mansion 1F static | Cinnabar visit; authored level **35** | Accepted. Evolves to Silvally through friendship. |
| Silvally | Evolve Type: Null | Friendship evolution | Derived; not additional static. |
| **Magearna** | **Replace Silph Co. 7F Lapras gift**, rather than retaining Power Plant static | Gift from existing Silph employee **after the rival battle**, **level 25** (same as original Lapras) | **Agreed.** Avoid duplicate Power Plant Magearna static. Preserve original story reward interaction. |
| **Marshadow** | **Move Mt. Moon 1F → Saffron City Fighting Dojo** | Appears after defeating Karate Master; ~level 35–40 suggested | **Agreed.** Preserve Hitmonlee/Hitmonchan choice; select safe tile/event. |
| **Necrozma** | **Move Mt. Moon B1F → Cerulean Cave 2F** | Existing postgame Cerulean Cave access sufficient, **no extra caught/badge prereq**; proposed level **65** | **Agreed.** Different floor from Mewtwo B1F; authored 2F has no other Legendary static. |
| **Meltan** | **Move Pokémon Mansion 1F → Vermilion construction site near Machop** | **First Vermilion visit, level 15, no badge gate** | **Agreed.** Existing authored Mansion static to be removed/relocated. |
| Melmetal | Evolve Meltan | **Level 40 while holding Metal Coat**, as existing source implements | Derived, no additional static. |

### Lapras protection

- Replacing Silph Co. Lapras with Magearna **must not eliminate Lapras acquisition**.
- Keep inherited postgame **Icefall Cave, Four Island** Lapras encounter.
- Add a reasonable **Kanto Surf water-patch wild encounter** for Lapras around the broader Seafoam/Route 20 coastline or another location to be selected. User explicitly said choose a water patch and not to overcomplicate this. No special gate. Specific wild table, rarity, and tile remain open.

## 5. Johto Generation 2: agreed encounter plan

**Johto overriding preference:** The user **hates roaming Legendaries**. Preserve the original Burned Tower scene that awakens Raikou, Entei, and Suicune, but replace roaming acquisition with **fixed, deliberately discoverable static encounters**. Do not implement mandatory roaming for other species unless user explicitly opts in. Each beast requires the Burned Tower awakening before appearing, and should stay in its fixed location for repeat attempts according to the general static-encounter persistence design. This is **planned**, not implemented.

| Species | Intended location | Agreed appearance condition | Tentative encounter level | Status / notes |
| --- | --- | --- | --- | --- |
| **Celebi** | **Ilex Forest shrine** | Available on first visit after the Farfetch'd/Cut forest section; **no GS Ball or badge-specific gate** | **18** | **User agreed.** Early special discovery. Preserve shrine location, remove legacy limited-event barrier if one exists. |
| **Raikou** | **Route 38**, west of Ecruteak toward Olivine | Burned Tower beast-awakening event completed | **30** | **User agreed** to fixed location; no roaming. |
| **Suicune** | **Lake of Rage** | Burned Tower awakening **and** Mahogany Rocket Hideout cleared / signal shut down | **35** | **User agreed**; ties water-purification theme to lake story. |
| **Entei** | **Mt. Mortar, deep Waterfall-accessible area** | Burned Tower awakening **and** naturally reaching chamber with Waterfall | **40** | **User agreed**; no extra arbitrary badge count. |
| **Ho-Oh** | **Bell Tower, Ecruteak** | **Raikou + Entei + Suicune all registered caught**; awakening necessarily occurred first | **TBD** | **User explicitly proposed and agreed** lore prerequisite. Ho-Oh revived the beasts; use caught Pokédex registration, not current ownership. |
| **Lugia** | **Whirl Islands**, original iconic site | Preserve natural Whirl Islands / story accessibility; no additional capture prerequisite agreed | **TBD** | User: “Lugia is fine wherever.” Original Whirl Islands preferred, but precise gating and level not yet decided. |

**Reachability caveat:** Johto and Hoenn map layouts are not fresh in the user's memory. For new encounter candidates, briefly explain the **geographic location, ordinary story progression / HMs required, and existing map occupants** before asking the user to approve. The above placements were accepted based on those explanations; exact tile/collision and event-flag checks are implementation-time work only.

**Next planning scope:** Johto **Generation 5 (13 species)**, then Gen 8, then a single integrated Johto encounter timeline. Do not turn this into a power-gating exercise; the goal is geographically and chronologically distributed discoveries.

## 5A. Johto Generation 5: Victini decision (agreed gate, location tentative)

- **Victini is unlocked after the player defeats any ONE regional Champion at least once:** Kanto, Johto, or Hoenn. This is a **global cross-region completion check**, not a Johto badge requirement, and not a requirement to complete all three championships.
- **Reasoning:** Victini is the Victory Pokémon. The user wants encounter opportunities distributed throughout the adventure, not power-based late-game bans; here the Championship requirement is a thematic achievement exception specifically requested by the user.
- **Location:** Johto **National Park** remains the assistant's **tentative suggestion**, **not yet explicitly approved** by the user. This is an earlier-access location, so a Champion from another region can unlock Victini on the first visit, while Johto-first runs can return afterward.
- **Encounter:** Fixed **static**, level **TBD** (earlier level 20–22 was suggested for a first-visit encounter **before** the user requested the Championship condition and is no longer settled).
- Implement by checking any regional first-Champion-cleared flag / equivalent progression marker. Do not treat repeat Elite Four victories, badges, or Battle Frontier achievements as necessary. Check source story flags and runtime behavior only once code work is explicitly authorized.
- No code or repository changes now. Continue Johto Gen 5 planning with the Swords of Justice, Forces of Nature, Tao Trio, Keldeo, Meloetta, Genesect.

## 5B. Johto Generation 5: Swords of Justice / Keldeo decisions

**Explicit user decision:** **All four** members of the Swords of Justice group (Cobalion, Terrakion, Virizion and Keldeo) must be obtainable **early to mid Johto**. Do not postpone any until late badges, Champion or postgame on the basis of high stats. They are **independent** static encounters; user expressly rejected Cobalion + Terrakion + Virizion as a capture prerequisite for Keldeo. No mandatory roaming.

| Pokémon | Proposed new Johto static location | Natural first visit / progression | Proposed encounter level | Status |
| --- | --- | --- | --- | --- |
| **Keldeo** | **Route 32**, near the waterfront south of Violet | After the Violet/Falkner early passage, on the way toward Union Cave (roughly 1 badge) | **18** | **Early-to-mid and independent are agreed; precise spot is assistant-selected and should be confirmed if challenged.** |
| **Terrakion** | **Union Cave**, along its ordinary accessible ground-floor route | Initial journey to Azalea, between Gyms 1–2; avoid Surf-locked deeper branches | **20** | Assistant location/level proposal, consistent with approved early–mid band. |
| **Virizion** | **National Park**, a secluded grassy section north of Goldenrod | On the Goldenrod/Route 35 leg, around Gyms 2–3, no special postgame gate | **23** | Assistant location/level proposal, consistent with approved early–mid band. |
| **Cobalion** | **Mt. Mortar**, accessible **western entrance / ordinary early cave area** (not deep waterfall chamber) | East of Ecruteak toward Mahogany, around Gym 4 | **28** | Assistant location/level proposal, consistent with approved early–mid band. Do not overlap Entei's late Waterfall-accessible depth. |

**Presentation note:** As the user does not remember Johto map layouts, explain these geographic placements and natural availability succinctly. The location and levels are the assistant's immediate response to the user's instruction to *place* all four; unlike the early-to-mid timing and Keldeo independence, individual coordinates/levels have not been separately ratified by the user. All require an eventual check of Grand Anthology's actual existing static/obstacle placement and story path **only after authorization for repo work**. These are **not implemented**.

## 6. Distinctly noted local distribution principles

- Viridian Forest is underused for scripted discoveries; its currently authored additions include wild Kartana, Pheromosa, Morelull, Togedemaru, but **no authored Legendary static**. New early **Shaymin static** is the agreed improvement. Preserve Ultra Beasts as wild.
- Keep five of Rock Tunnel B1F's originally placed special encounters: **Uxie, Mesprit, Azelf, Heatran, Regigigas**. Move the other three: **Giratina → Mt. Moon B2F**, **Darkrai → Pokémon Tower 7F proposed**, **Tapu Lele → Celadon garden**.
- Power Plant currently packs Tapu Koko, Zeraora, and Magearna; moving Magearna to the Silph Lapras gift leaves two electricity-themed special encounters.
- Mt. Moon B1F currently packs Dialga, Palkia, Cresselia and Necrozma; moving Necrozma postgame reduces crowding. Mt. Moon 1F loses Marshadow, keeps first Cosmog. Mt. Moon B2F gains Giratina for a later revisit after its original Rocket/fossil events.
- Cerulean Cave retains pre-entry guard until Celio network quest, despite exterior being Surf-accessible earlier. Postgame 2F Necrozma + B1F Mewtwo.
- Several species currently have *no additional gate* in their authored scripts; this ledger describes **future intended changes**, not implemented functionality.

## 7. Outstanding decisions / future design discussion

1. Finish Kanto's **integrated route-by-route discovery timeline**, including gaps, clusters, actual map reachability, and levels aligned with natural story progress. Previous 29-static-opportunity table was a brainstorming baseline, **not** final schedule.
2. Confirm Darkrai Tower **7F** (positive proposal, not a separately explicit floor approval).
3. Revisit old provisional badge numbers across Gen 4/7 using the user's clarified **exploration cadence rather than strength restriction** rule, especially on first-access locations and return encounters (Mt. Moon, Rock Tunnel).
4. Decide exact coordinates and conflict-free field sprite placements for Viridian Shaymin, garden Tapu Lele, Mt. Moon B2F Giratina, Tower Darkrai, Dojo Marshadow, Cave 2F Necrozma, and Vermilion Meltan; do not interfere with story scripts, warp tiles, trainer movement, existing pickups or native legendary puzzles.
5. Choose Lapras Surf location and frequency. Preserve Icefall Cave Lapras.
6. Confirm forms/evolution access where it matters: two Cosmog branches, Shaymin Sky Forme and Meltan→Melmetal; ensure compatible with current game mechanics and items. No need to implement new mechanics unnecessarily.
7. Special capture prerequisites should check *caught Pokédex registration*, **not** current party or living-storage copy. Verify semantics later when engineering is authorized.
8. Audit **94** Legendary/Mythical species overall for lore permitting multiple wild individuals and targeted Research/post-Champion rotation eligibility; separately remove five specified canonical Legendary wild habitat entries.
9. **Johto Gen 2 has been agreed and recorded above.** Design Johto Gen 5 and 8, then Hoenn Gen 3, 6, 9 through discussion before repository work. For example, Hoenn **Diancie** in Granite Cave Steven's Room was tentatively agreed after Hoenn Gym 2 and letter delivery at ~level 18, with Mega access later; preserve this prior proposal for Hoenn discussion.
10. No code/build/test/CI/repo changes unless user **explicitly authorizes** them. At that future point make a consolidated implementation handoff rather than silently implementing during design.

## 8. Source-state caveat

This document consolidates user–assistant design decisions and previously inspected repository state. All listed **new** encounters, replacements, relocations, gates and level changes are **planned only**; as of writing, none were implemented in this discussion. This offline document itself does not touch GitHub. Do not mistake an authored static source for demonstrated in-emulator catchability.

## 5C. Johto Gen 5: Forces of Nature and Tao Trio (discussion update)

- **Forces of Nature:** User approved Thundurus around Johto badge 6 (proposed Route 41, Lv. 36), Tornadus around badge 7 (proposed Route 44, Lv. 40), and Landorus around badge 7 (proposed Route 43/Lake of Rage, Lv. 42). All fixed static encounters, no roaming. These locations and levels are proposals, not verified map tiles. Enamorus will be discussed under Gen 8.
- **Tao Trio:** User agreed **Kyurem requires catching both Reshiram and Zekrom**. The two dragons are independent catches. **Ice Path** is a potential Kyurem site; it has not yet been approved. Exact dragon locations and levels remain undecided.
- **Kyurem fusion:** The upstream `pokeemerald-expansion` engine supports Kyurem fusion through DNA Splicers, but this does **not** establish that DNA Splicers are obtainable or fusion fully works in Grand Anthology. Design goal: reversible **White Kyurem (Kyurem + Reshiram)** and **Black Kyurem (Kyurem + Zekrom)**, preserving both constituent Pokémon. Future authorized development should audit item distribution and in-game fuse/unfuse/save behavior.
- **Repository constraint:** Do not access, edit, test, or commit to the Grand Anthology GitHub repository during this planning phase.

### Johto Tao Trio progression (explicitly approved, 2026-10-24)

- **Reshiram**: available in late Johto, within the **7–8 Johto badge** window; independently obtainable. Exact location, level and badge-specific trigger remain to be settled.
- **Zekrom**: likewise available within the **7–8 Johto badge** window, independently of Reshiram. Exact location, level and badge-specific trigger remain to be settled.
- **Kyurem**: **must be catchable before defeating the Johto Champion**, and only after **Reshiram and Zekrom have both been caught** (Pokédex caught registrations, not simultaneous party/PC ownership). No Champion victory prerequisite. Ice Path is an *unapproved proposal* for its location; final location and level TBD. Ensure all three are accessible before the Johto League so this dependency is feasible.
- **Fusion**: the previously discussed DNA Splicers award on catching Kyurem is approved in principle; White and Black Kyurem should be reversible. Working mechanics, item delivery and persistence are to be verified **only after future authorization to develop**.
- **Strict planning boundary:** This is a local design-ledger update, not a GitHub change. Do not access or modify the repository, run tests, or start implementation.

### Johto Gen 5: Meloetta and Genesect (discussion update, 2026-10-24)

- **Meloetta: AGREED.** Relocate/place its Johto static encounter at **Ecruteak Dance Theater**, associated with the Kimono Girls and the dance/music theme. Naturally around the fourth Johto Gym; no extra capture or championship prerequisites were suggested. Specific floor/tile and encounter level remain TBD until the authorized implementation phase. User explicitly approved the location.
- **Genesect: NOT YET FORMALLY SETTLED.** User called it "meh" and did not request a complicated quest. Assistant proposed keeping the existing authored static at **Goldenrod City**, available during normal early-to-mid exploration with no extra prerequisites, and level TBD. User's subsequent approval explicitly praised the Meloetta proposal; don't silently treat the Genesect location as an individually ratified decision without reconfirmation if it matters.
- **Repository strictly untouched.** This text is part of the offline design ledger only.

### Johto Gen 8: Zacian, Zamazenta and Eternatus (2026-10-24)

- **APPROVED:** Eternatus is catchable **only after both Zacian and Zamazenta have been caught** (caught Pokédex registrations rather than simultaneous ownership in the party/PC). This follows their role in defeating Eternatus in Sword/Shield.
- **APPROVED:** Postpone any decision about making **Eternamax Eternatus** usable or boss-only to **P1/P2**. No such feature work is authorized now.
- **NOT SETTLED:** Zacian and Zamazenta's exact Johto sites, timing, levels, and whether their Crowned forms / Rusted Sword / Rusted Shield are available immediately or later. **SUPERSEDED BY THE SPECIAL-BATTLE DECISION BELOW:** Zacian and Zamazenta are no longer planned as independent solo static catches. The user explicitly chose a shared double battle with both awarded only upon victory. See the final Zacian/Zamazenta section. Locations, levels and timing are still undecided. Eternatus site, level and exact position in progression also remain undecided.
- **Power notes for design only:** Current Generation IX base-stat totals are **660 Hero / 700 Crowned** for both. Zacian Crowned is Fairy/Steel, 150 Attack and 148 Speed; Zamazenta Crowned is Fighting/Steel, 140 Defense and Special Defense. Specific stats, items and form functionality in Grand Anthology have NOT been verified, and must not be assumed. Strong Pokémon are not grounds alone for pushing encounters to postgame: the user's priority is evenly distributed discoveries throughout adventure.
- **NO REPO WORK:** This is an offline planning note. Do not read/write repository code, maps, docs or trigger CI until explicitly authorized.

## 9. FLAGGED SPECIAL FEATURE: Zacian and Zamazenta double battle (DEFER UNTIL AFTER ALL OTHER LEGENDARY/MYTHICAL FIXES)

**Priority/status: EXPLICIT USER DESIGN APPROVAL; DO NOT IMPLEMENT YET.**

- **Replace the initial independent encounter proposal:** Zacian and Zamazenta are encountered **together in a single special opposing double battle** in Johto. The player fights them as a pair.
- **Win condition:** The player must **defeat BOTH Zacian and Zamazenta** in the same successful battle. Only then are **BOTH automatically caught and awarded together** to the player's party or PC/storage and registered as caught in the Pokédex. They are not individually awarded as soon as either one faints.
- **Unsuccessful / interrupted battle:** Defeating only one and then losing, fleeing or otherwise failing to win the overall two-opponent encounter should **not grant either Pokémon**. The encounter must remain retryable until completed and must not create duplicate rewards.
- **Automatic award semantics:** No thrown Poké Balls are required for the successful reward. During future implementation, preserve canonical encounter Pokémon attributes wherever applicable (species/forms, nature, IVs, shiny state, etc.), safely handle a full party and available storage, and ensure both caught registrations and save/reload persistence. The exact UX for full storage and item/form behavior is not yet decided; never silently lose either Pokémon.
- **Eternatus dependency remains:** Catching/awarding both wolves through this battle satisfies the already approved **Zacian + Zamazenta caught** prerequisites for the later Eternatus static encounter.
- **Exact encounter location, level, trigger/availability, visual presentation and whether they use Hero or Crowned forms are NOT decided.** Do not invent or hard-code defaults when development resumes.
- **Sequencing mandate:** **Only attempt this custom battle AFTER all OTHER Legendary and Mythical Pokémon encounter locations, gates, levels and required regular mechanics have been fixed/settled.** It is intentionally a late implementation task, not part of the current location pass; the user explicitly called out this deferment.
- **Eternamax Eternatus:** Separate earlier decision; postpone to P1/P2, not part of this special battle.
- **Repository boundary:** All of this is offline planning. The user prohibited ANY access to or modification of the GitHub repository during this discussion; do not open, inspect, edit, commit, build or trigger tests/CI until explicitly authorized.


### Johto Gen 8: Regieleki and Regidrago (approved in discussion, 2026-10-24)

- **Regieleki:** Olivine Lighthouse, appearing **after Ampharos has recovered**. This naturally falls near Johto Gym 6; an assistant-proposed level was not finalized. User explicitly accepted the placement. It is a **normal scripted static encounter**. Avoid disrupting the Amphy/Jasmine lighthouse sequence and choose a safe location during later authorized implementation.
- **Regidrago:** Dragon's Den near Blackthorn City, naturally **around/after Johto Gym 8**. User explicitly accepted the placement. Exact coordinate and level remain open; it is a normal scripted static encounter.
- Both are **independent**, with no new inter-Regi capture chain. **Regigigas continues to depend only on Regirock, Regice and Registeel**, not on these two.
- **Next discussion:** Enamorus, then Calyrex, Glastrier/Spectrier, Kubfu/Urshifu and Zarude. No Enamorus prerequisites, encounter site or level have yet been agreed.
- **No repository access or modifications** during this planning phase.

### Johto Gen 8: Enamorus dependency (approved in discussion, 2026-10-24)

- **APPROVED:** Enamorus is unlocked only after the player has **caught Tornadus, Thundurus and Landorus**. This parallels the Forces of Nature progression in *Pokémon Legends: Arceus*. Use caught Pokédex registrations, not simultaneous ownership of all three in party or boxes. No further mandatory badge gate or championship gate has been approved.
- **PROVISIONAL LOCATION:** A grassy clearing on **Johto Route 39, near MooMoo Farm**, was suggested but not explicitly ratified. Exact static position and encounter level are open. Natural availability should arise after collecting the three other Forces, currently proposed across badges 6–7; avoid inventing extra delays.
- **Implementation boundary:** Design-only note. No access, editing, tests or commits to the GitHub repository during this conversation unless the user expressly authorizes it.

### Johto Gen 8: Calyrex, Glastrier, Spectrier, and Reins of Unity (discussion 2026-10-24)

- **APPROVED:** Both **Glastrier and Spectrier** must be independently obtainable in the **same playthrough**, along with Calyrex; the player must have the option to use either fusion. Do not force a one-horse-only choice or mutually exclusive story branch.
- **APPROVED:** **Reins of Unity are not sold in any shop.** Make them an interesting exploration reward.
- **LOCATION PLAN CHOSEN IN DISCUSSION (VERIFY LATER):** **Ruins of Alph, in a hidden puzzle chamber**. Prefer the secret chamber associated with the **Flash inscription** (in HeartGold/SoulSilver the chamber requires Surf to access, and Flash reveals the hidden item room). Put the Reins of Unity there as a special pickup, ideally replacing or adding to a modest existing hidden-room reward, without breaking the ruins' original puzzle, Unown encounters or related items. If Grand Anthology's inherited Johto map differs, choose a similarly accessible Ruins of Alph secret room after map inspection. This is a design intention, **not a confirmed existing placement**.
- **Mechanics goal:** Reversible **Ice Rider Calyrex** (Calyrex + Glastrier) and **Shadow Rider Calyrex** (Calyrex + Spectrier); no Pokémon lost on fuse/unfuse. One Calyrex can fuse with one horse at a time; players may keep both horses for switching freely. Confirm fusion item support, item pickup, PC/party interactions, and save persistence during future **authorized** implementation.
- **NOT DECIDED:** Individual static locations, encounter levels and timing for Calyrex, Glastrier, and Spectrier. No need for extra quests or arbitrary Champion gate. Whether the hidden-room pickup requires having caught Calyrex or a horse is **not agreed**; default recommendation is that it stays an ordinary discoverable item, useful later.
- **DO NOT ACCESS OR MODIFY REPO** during discussion; source research cited for HGSS puzzle mechanics is public game reference, not an inspection of Grand Anthology's current map.

### Johto Gen 8: Approved Calyrex and horse locations (2026-10-24)

The user explicitly approved all three proposed locations on 2026-10-24. These are DESIGN DECISIONS only, informed by public HGSS geography, NOT implemented or verified against the Grand Anthology maps.

| Pokémon | Proposed Johto encounter | Plausible first access | Suggested level | Additional requirement |
|---|---|---|---:|---|
| Calyrex | Route 36, a peaceful clearing toward the Ruins of Alph entrance | After the Sudowoodo roadblock (often ~3 badges) | 25 | Reach location; no extra capture/Champion condition |
| Spectrier | Cliff Cave, Route 47, a quiet side area away from the through-route | After helping Jasmine's Ampharos and unlocking the route west of Cianwood (roughly badges 5–6) | 34 | Reach location; no extra capture/Champion condition |
| Glastrier | Ice Path B2F, a secluded pocket beyond ice puzzles | Late Johto en route to Blackthorn, roughly 7 badges | 42 | Reach location; no extra capture/Champion condition |

**Reasoning:** Calyrex is the King of Bountiful Harvests, fitting a grassy spot near ancient ruins and the planned hidden Reins of Unity. Spectrier prefers quiet and isolation and fits Cliff Cave on a comparatively underused southwest Johto route. Glastrier fits Johto's only famous icy dungeon. Locations distribute discoveries across the early-mid, mid, and late journey. Independent captures and both-horses-in-one-playthrough are APPROVED from prior discussion; all three locations are now APPROVED by the user; exact coordinates, script mechanics and feasibility remain to be checked when implementation is authorized.

**Potential overlap to resolve:** Kyurem's previously floated *Ice Path* site is only tentative, not approved. Don't silently cram Kyurem and Glastrier together. Revisit Kyurem's exact position when finalizing the Tao Trio. Also verify Cliff Cave and selected tiles exist and are usable in Grand Anthology after explicit authorization; original HGSS geography alone does not establish this.

**Reins:** Keep already selected Ruins of Alph hidden puzzle chamber, no shop. Fusion/defusion should work with either horse once all are caught and item obtained; test later only when authorized.

**Absolute boundary:** Do not access the Grand Anthology repository, change files, run builds/tests, or trigger CI during design discussion.


### Johto Gen 8: Kubfu / Urshifu progression (user approval, 2026-10-24)

- **APPROVED:** Make **Kubfu available early** in Johto, and make evolving it into either Urshifu style a **later exploration reward**. Do NOT simply push Kubfu to late game because Urshifu is powerful.
- **APPROVED overarching collection goal:** Both Single Strike (Dark/Fighting) and Rapid Strike (Water/Fighting) Urshifu must be obtainable in one playthrough; therefore arrange for two Kubfu or a repeatable, non-exclusive Kubfu acquisition path. One-time choice of form must not lock the other permanently.
- **PROPOSED ONLY, not yet explicitly approved:** Scroll of Waters after defeating Chuck in Cianwood Fighting Gym; Scroll of Darkness from the Mt. Mortar Karate King. These should be meaningful rewards, not arbitrary shop stock. Verify item/form-evolution support later when repository work is authorized.
- **STILL TO DESIGN:** Two plausible early Kubfu encounter sites, their levels and any story gates; exact scroll distribution; whether forms/evolutions operate through key items, map interactions or another supported mechanic. Never claim these implementation details are already functioning.
- **STRICT NO-REPO BOUNDARY:** During this discussion, do not inspect/access or modify the Grand Anthology GitHub repo, code, documentation, build, test or CI. This is an offline design note only.

### Johto Gen 8: Zarude encounter in Ilex Forest (APPROVED 2026-10-24)

- **APPROVED:** Move/place **Zarude**, a **Dark/Grass Mythical**, as a **scripted static encounter in Johto's Ilex Forest**, alongside Celebi as the forest's other planned Legendary/Mythical encounter.
- **Design reasoning:** Zarude is a jungle/forest-dwelling Pokemon, and Ilex Forest is a natural thematic fit. User explicitly approved even though Celebi is there, as only these two planned special Legendary/Mythical statics would share the forest. Preserve the forest's native story, Ilex Shrine/Celebi encounter, Farfetch'd/Cut sequence, wild ecology, and traversability.
- **Timing/level:** **Not yet specified by user**. Proposed natural availability during normal Ilex Forest exploration (~2 Johto badges), with an appropriate encounter level around 18–22, unless later region-wide pacing review changes it. Do not add an artificial high badge/Champion requirement without user approval.
- **Status:** Chosen design, **not implemented or map-coordinate verified**. Other ordinary wild Pokemon may of course be present; "only Celebi" refers to planned special Legendary/Mythical encounters, not the entire forest population.
- **Strict constraint:** Continue discussion and offline notes only. No access to, inspection of, or edits/tests/commits on the Grand Anthology GitHub repository until explicitly authorized.


### Hoenn Gen 3: Regirock, Regice and Registeel (APPROVED 2026-10-24)

**Explicit user decision: keep the original Hoenn Regi locations and puzzles unchanged.**

| Pokémon | Original location | Decision |
|---|---|---|
| Regirock | Desert Ruins | Preserve native encounter and puzzle as-is |
| Regice | Island Cave | Preserve native encounter and puzzle as-is |
| Registeel | Ancient Tomb | Preserve native encounter and puzzle as-is |

- Do not simplify, replace or bypass native Regi puzzles. Preserve the original unlock sequence, story and navigation. Do not introduce additional artificial badge prerequisites.
- The already approved interregional **Regigigas** dependency remains: Regigigas in Kanto's Rock Tunnel B1F becomes available after the player's Pokédex has registered **Regirock, Regice and Registeel as caught**. Regieleki and Regidrago do **not** enter this requirement.
- This decision concerns preserving the original Regi encounter structures; runtime/map implementation validation should happen only **after explicit permission** to work on the repository.
- **Next Hoenn discussion:** Groudon, Kyogre, Rayquaza. Preserve the original Emerald story resolution as far as feasible, and discuss whether Rayquaza capture should require Groudon and Kyogre despite the original post-Champion capture timing for the latter two. No decision to change that sequence has yet been reached.
- **No repository access, edits, tests, builds, or CI during this planning phase.** This note is an offline artifact only.

### Hoenn Gen 3: Weather Trio capture order (APPROVED 2026-10-24)

**User-approved theme: "To take the two children, we need Rayray."** Rayquaza is the prerequisite for the Groudon and Kyogre captures, not the other way around.

| Pokémon | Encounter timing | Capture requirement | Location and events |
|---|---|---|---|
| **Rayquaza** | **Before first Hoenn Champion**; available during the original Emerald story progression | No requirement to catch Groudon or Kyogre | Preserve the main Sky Pillar/Sootopolis Rayquaza resolution sequence as far as possible; do not rewrite existing story just to impose new gating. |
| **Groudon** | Original **Hoenn post-Champion** opportunity | **Rayquaza must be registered as caught** | Retain the native Groudon postgame location/encounter mechanics where practicable. Do not add ordinary-grass wild Groudon. |
| **Kyogre** | Original **Hoenn post-Champion** opportunity | **Rayquaza must be registered as caught** | Retain the native Kyogre postgame location/encounter mechanics where practicable. Do not add ordinary-water wild Kyogre. |

- **Groudon and Kyogre do not require one another.** They may be caught in either order once Rayquaza is caught and the postgame is available.
- Use persistent **caught Pokédex registration**, not current party or storage possession, for the Rayquaza prerequisite.
- Distinguish **Rayquaza's story appearance** from its capture: keep the scripted intervention intact and ensure a Rayquaza catch can be accomplished before the Champion without breaking subsequent story flags or preventing the postgame unlock.
- **Exact capture trigger/levels** still require design and later implementation audit. Original Hoenn story/puzzle continuity has priority over arbitrary badge gates.
- No live repository inspection or modification during this discussion. Keep the entire agreement in offline planning notes for later authorized handoff.

## Hoenn Gen 3: Eon Duo static placements (2026-10-24)

**User direction:** Latias and Latios must be **fixed static encounters** in Hoenn; absolutely no mandatory roaming or region-hopping. Spread encounters naturally through the journey, not by barring powerful Pokémon.

**Assistant-selected location plan** (can be revised by user; precise tile/object placement unverified):

| Species | Static location | Plausible encounter window | Initial proposed level | Conditions |
|---|---|---|---|---|
| **Latias** | Hoenn Route 120, secluded grassy clearing by a pond | Fortree/Route 120 journey, around badge 6 | 35 | Reach the location naturally; no sibling catch requirement |
| **Latios** | Mossdeep City near the Space Center | After the existing Team Magma Space Center storyline, around badge 7 | 40 | Existing story completion; no sibling catch requirement |

- Preserve main Hoenn progression and don't collide with Kecleon/Registeel Route 120 scripts or the Space Center Team Magma event. Choose nonblocking walkable map tiles during future authorized implementation.
- Both stay in their designated locations, not random roaming encounters. Catch/faint/revisit/save handling should follow the general static encounter standard.
- Keep original Southern Island/Eon Ticket content if practical as an optional supplemental encounter; no event ticket should be required to obtain either species in the main adventure.
- These are planned placements, not changes to the live game; no GitHub access, commits, code modifications, or CI runs during design.

## Hoenn Gen 3: Jirachi early encounter (APPROVED 2026-10-24)

- **Pokémon:** Jirachi, fixed scripted static encounter.
- **Location:** A quiet land-accessible spot at **Hoenn Route 102 pond**, between Oldale Town and Petalburg City. This deliberately evokes the feeling of Lake Verity from Diamond/Pearl; it is not a lake in Emerald where Red Gyarados appears.
- **Earliest availability:** On the player's initial Route 102 journey, **before Roxanne/first Hoenn badge**. **No Surf and no special unlock prerequisites**.
- **Proposed encounter level:** **8–10** (choose an appropriate exact value during later authorized implementation).
- **User's intention:** Jirachi is a favorite and should be available early enough to accompany the player through nearly the entire Hoenn adventure; don't defer it to Meteor Falls/postgame or require an event ticket.
- **Status:** Route 102 pond and early access **approved by user**; precise map tile, sprite, dialogue and exact level still to verify when authorized.
- **Remaining Hoenn Gen 3 design topic:** Deoxys, including its static location and accessibility. No Deoxys location selected yet.
- **Strict scope:** Documentation only in this offline file. Do not access or modify the Grand Anthology repository until the user explicitly authorizes it.

## Hoenn Gen 3: Deoxys on Birth Island (APPROVED 2026-10-24)

**User choice:** Keep Deoxys's classic **Birth Island** encounter and its original moving-triangle puzzle. Deoxys becomes accessible **after Rayquaza has been caught**, building on the famous Rayquaza/Deoxys rivalry (as seen in *Pokémon: Destiny Deoxys*).

| Attribute | Decision |
|---|---|
| Species | Deoxys |
| Static location | Birth Island (original Emerald event-island map/encounter) |
| Unlock requirement | Persistent **Rayquaza caught Pokédex registration**; Rayquaza need not be in party or PC |
| Island travel | Provide ordinary reachable ferry/island access **without a distributed AuroraTicket**, once Rayquaza has been caught; exact ferry integration to audit during authorized implementation |
| Encounter interaction | Preserve original moving black-triangle puzzle and Deoxys static battle |
| Level | Original Emerald level **30** as current starting choice; no user-requested adjustment |
| Other prerequisites | None added by this decision; ordinary geographic/story ferry access may still apply as needed |

**Important implementation boundaries:** Birth Island is an event island historically reached in Emerald by S.S. Tidal from Lilycove/Slateport through the AuroraTicket; it is not an ordinary walkable mainland Hoenn location. Do not merely gate the Deoxys battle while leaving island transportation locked behind the unavailable event ticket. Ensure the relevant existing ferry/flags and original puzzle remain functional. A direct forced Rayquaza-versus-Deoxys battle, Rayquaza-in-party requirement, or new cinematic were **not** requested or approved. Pokemon may be caught normally after the triangle encounter.

**Do not access or modify the public/private game repository; no code, scripts, maps, builds, tests, or CI. This document is offline planning only.**

## Hoenn Generation 6: Diancie, Xerneas and Yveltal (2026-10-24)

**User-approved decisions:**
- **Diancie** retains its scripted static location at **Granite Cave, Steven's Room**. It must not become available until the player has **at least two Hoenn Gym badges**, AND **Steven's letter has been delivered**. Both requirements matter because the original letter event can occur before the second badge. Set encounter **level 18**, replacing the originally authored level 45. No new elaborate quest or postgame gate. Mega Diancie considerations remain deferred.
- **Xerneas and Yveltal** should both be obtainable in **midgame Hoenn**, instead of being postponed to late game or postgame. The goal is steady exploration rewards, not power-based restriction. No capture dependency between them has been approved.

**Existing authored locations according to the earlier offline source audit, not newly inspected live code:**
- Xerneas currently has a static encounter at **Hoenn Route 120** (Fortree journey, approximately 5–6 badges).
- Yveltal currently has a static encounter at **Mt. Pyre exterior** (roughly sixth-Gym/Lilycove progression).
- Diancie is in Granite Cave Steven's Room as above.

**Proposed level ranges, not separately approved by user:** Xerneas around 35, Yveltal around 38–40.
**Open distribution issue:** Latias was previously proposed for Route 120 as well, so Xerneas and Latias would share one route. To improve geographic distribution, consider **relocating Xerneas to Route 119's wooded area**, accessible in the Fortree approach after Surf (around five badges). This is an **assistant proposal requiring user approval**, not a finalized move. Yveltal at Mt. Pyre fits its life-draining lore and can remain there.

**Scope:** Design ledger only, stored outside GitHub. No repository access, modifications, CI, tests or implementation without explicit user permission.


## Hoenn Generation 6: Xerneas / Yveltal location signoff (APPROVED 2026-10-24)

The user explicitly approved both location recommendations made after the preceding Hoenn Gen 6 section:

- **Xerneas:** Relocate the authored static from **Hoenn Route 120 to Route 119**, in a lush wooded/rainforest area. Accessible during natural midgame progression toward Fortree, approximately badges 5–6. Assistant's earlier suggested level **~35 is provisional**, not explicitly chosen by user. No artificial capture dependency. Avoid overlapping the Route 120 Latias discovery.
- **Yveltal:** **Keep** the authored static at **Mt. Pyre exterior**, naturally available in the Hoenn midgame around badge 6. Assistant's earlier level range **~38–40 remains provisional**. No new prerequisite.
- **Diancie (previously approved):** Granite Cave Steven's Room at level 18, unlocked **after two Hoenn badges AND Steven's letter delivered**.

**Next Hoenn Gen 6 discussions:** Zygarde, Hoopa, Volcanion. Their locations and constraints are not settled.

**Strict boundary:** Offline planning document only. Do not read, touch or modify the GitHub repository, implement, build, test, or run CI without explicit authorization.


## Hoenn Generation 6: Zygarde gradual form progression (APPROVED in principle, 2026-10-24)

**User-approved high-level design:** Zygarde should first be obtainable as its **10% Forme early in the Hoenn adventure**, grow into **50% Forme later**, and **Complete Forme must be kept for the endgame**. The user prefers this over collecting 100 scattered Zygarde Cells.

| Milestone | Suggested timing | Mechanics / status |
|---|---|---|
| **10% Forme** | Early Hoenn, roughly 2–3 badges | Scripted catchable static, level/location TBD; initial **Aura Break**, **not** Power Construct, so Complete Forme cannot trigger early. Timing and species design approved; exact level/site not settled. |
| **50% Forme** | Mid-to-late Hoenn, roughly 6–7 badges | Earn a special exploration/story discovery or Zygarde Cube-like feature that changes the same captured Zygarde between 10% and 50%; no requirement for a second catch and no 100-cell chore. Timing and mechanism proposed, not separately ratified. |
| **Complete Forme** | **Endgame only**, candidate unlock after first Hoenn Champion | Only then permit Zygarde to acquire **Power Construct** through a dedicated quest/key-item interaction. Precise endgame trigger/quest/location still to be chosen; user explicitly insists Complete is NOT available early or midgame. |

**Canonical mechanics reminder:** In standard main-series games with Abilities, Complete Forme manifests **during battle** when a 10% or 50% Zygarde with **Power Construct** has 50% or less of its maximum HP at the end of a turn. It reverts after battle. Avoid treating Complete Forme as a normal permanent form. Zygarde's switch between Aura Break and Power Construct is form-mechanics-dependent and **not** a standard Ability Patch/Capsule change, so the game's general ability-change item should not bypass the endgame restriction. References: https://bulbapedia.bulbagarden.net/wiki/Zygarde_(Pok%C3%A9mon) and https://www.smogon.com/dex/ss/abilities/power-construct/ (public mechanics verification, 2026-10-24).

**OPEN:** Specific early Zygarde site (Meteor Falls floated earlier, but whether it really suits early 2–3 badge access requires review), level, 50% form reward/site, exact endgame quest/unlock, and any engine-supported variation. Do not silently mark these unsettled details approved.

**SCOPE:** Offline design notes only. No access, search, inspection, changes, tests or CI for Grand Anthology repository without express user approval.


## Hoenn Generation 6: Zygarde as THREE separately obtainable forms (REVISED, 2026-10-24)

**This section supersedes the preceding proposal to upgrade one captured 10% Zygarde through the 50% and Complete forms.** The user explicitly confirmed **three individually catchable Zygardes**, including Complete as a **permanent, usable and catchable form**, not just a temporary in-battle transformation. All three forms may coexist in the player's collection. They remain a single National Pokédex species; forms do not add National Dex numbers. A no-100-cell-collecting philosophy remains.

| Zygarde form | Acquisition | Power Construct policy | Confirmation |
|---|---|---|---|
| 10% Forme | Standalone early Hoenn static catch, roughly 2–3 badges | **Power Construct is forbidden**, including via generic Ability Capsule/Patch/shop items. Give ordinary Aura Break or suitable equivalent. | Separately catchable and Power Construct ban **user-approved**; exact static site/level TBD. |
| 50% Forme | Separate catchable static approximately 6–7 badges, exact location/level TBD | User suggests **Power Construct could belong exclusively to this form**. **Recommended restriction:** 50% initially has Aura Break and gains access to Power Construct **only after the Hoenn Champion**, so it cannot summon Complete during midgame. Exact availability and ability method **await user signoff**. | Separate catch and progression agreed; timing and Power Construct gating provisional. |
| Complete Forme | **Separate permanent catchable boss encounter, post-Champion/endgame only**, exact location/level TBD | A permanent Complete form needs a specially supported ability/form definition. Do not simply assume a vanilla Power Construct-triggered form or automatically assign any unapproved final ability. | Separate permanent postgame catch **user-approved**; ability/engine specifics TBD. |

**Crucial design interaction:** In canonical mechanics, **50% Zygarde with Power Construct can transform into Complete during battle upon reaching half HP**, which would allow players to use Complete *before* catching the permanent endgame form. To honor the user's specific instruction that Complete be endgame, the assistant recommends withholding Power Construct from the catchable midgame 50% form until post-Champion. This precise postgame ability unlock has **not yet been expressly confirmed**. Ask about this interaction before locking the implementation.

**Engineering follow-up (only once expressly authorized):** audit current engine form definitions/abilities, distinguish permanent Complete from battle-triggered Complete, prevent regular ability items from giving 10% Power Construct, and test saves, PC, Dex, transformations, and permanent-form persistence. Do not touch the repository in this planning phase.


## Hoenn Generation 6: Zygarde Power Construct BEFORE Champion (USER-APPROVED, 2026-10-24)

**This latest decision supersedes the preceding suggestion to withhold Power Construct from 50% Zygarde until post-Champion.** The user explicitly wants **a glimpse of Zygarde's true power during pre-Champion battles**, but a permanent Complete/100% Forme must only be obtainable through the independently catchable postgame encounter. Do not mistakenly impose a post-Champion Power Construct restriction on the 50% form.

**Finalized three-form behavior:**

| Form | Catch timing (approximate; final site/level TBD) | Ability and Complete Forme access |
|---|---|---|
| **10%** | Early Hoenn, ~2–3 badges | Separately catchable; **Aura Break only, no Power Construct**; ordinary Ability Capsule/Patch must not circumvent this. |
| **50%** | Hoenn ~6–7 badges | Separately catchable; **Power Construct available immediately upon obtaining this form, BEFORE the Champion**. It can transform into Complete Forme in battle when reduced to half HP or below (canonically, at the end of a turn), representing a temporary glimpse of true power. It **returns to 50% after the battle**. |
| **Complete/100%** | **Post-Champion/endgame** | A third, **separate permanent catchable static boss**. Remains in Complete Forme across battles, storage, and saves without needing the in-battle Power Construct trigger. Its exact ability and engine form logic still to be designed and audited later. |

The player may own all three individually caught forms simultaneously; only one species entry in the National Pokédex. Do not add cell-collection chores or a required progression from one captured form into the next. **Availability of temporary in-battle Complete before Champion is INTENDED**, not a loophole. The endgame gate applies specifically to **continuous/permanent Complete ownership and uptime**, not its brief transformation through the 50% form. Avoid other ability-changing items unlocking Power Construct for 10%.

Engineering when explicitly authorized: distinguish temporary Power Construct Complete from permanent separate catch; ensure correct revert at battle conclusion for the transformed 50%, avoid reverting separately caught permanent Complete, persist identity correctly through party/PC, ensure Dex and ability items behave properly.

**Open:** Exact static encounter locations and levels for each form; particular ability for permanent Complete; exact post-Champion access/boss placement.

**Strict scope remains:** offline design discussion/documentation only. Do not access or modify GitHub repositories, maps, source, game builds, tests, or CI until express authorization.


## Hoenn Generation 6: Zygarde 10% Petalburg Woods signoff (USER-APPROVED, 2026-10-24)

The user explicitly approved the assistant's early static location suggestion:

- **Zygarde 10% Forme**: a **static, separately catchable encounter in a secluded clearing of Petalburg Woods**, Hoenn, during normal traversal **before the first Gym**, with **no badge, Surf, or other special prerequisite**.
- **Suggested encounter level 8–10** (as in the approved location proposal).
- **Ability: Aura Break only**. Power Construct must not be available on this form, including through general Ability Capsule/Patch items.
- This **updates/supersedes** any earlier provisional guidance for this form being found specifically at 2–3 badges or at Meteor Falls; Petalburg Woods is accessible earlier and is now the confirmed location.
- The **50% form remains a separate static catch around 6–7 badges**, with immediately usable Power Construct for a *temporary* battle transformation into Complete at <= 50% HP, reverting after battle. Its exact location remains TBD.
- **Complete/100% remains a separately catchable, permanently Complete, post-Champion boss**; site and ability TBD.

**Next conversation:** Select an appropriate mid-to-late-Hoenn location for 50% Zygarde, then a memorable postgame location for the permanent Complete encounter, while avoiding excessive overlapping locations.

**Strict boundary:** This is an offline design-only record. GitHub repository, source code, builds, tests, and CI were not accessed or modified.


## Hoenn Generation 6: Zygarde 50% and Complete locations (DISCUSSION, 2026-10-24)

**Explicit user approval:** The proposed separately catchable **Zygarde 50% Forme** in **Meteor Falls**, during mid-to-late Hoenn (~6–7 badges), is **approved**. Precise room, encounter level, path accessibility, and HM requirements remain to audit/choose once implementation is authorized. Power Construct is available on this 50% form *before* Champion, granting its *temporary* Complete in-battle transformation at low HP and reverting after battle. This is intentional.

**User-approved concept for Complete Forme:** Separately catchable, permanent **Zygarde Complete/100%** should appear **near the player's home in Littleroot Town, only after becoming Hoenn Champion**, inspired by the Pokémon XY&Z anime moment when Zygarde seeks out Ash and Greninja to request help protecting Kalos. The feel is a guardian deliberately coming to find the trainer, not an ordinary random cave encounter. The protagonist's home-area appearance is user-proposed, not a mechanic requiring Greninja in the party.

**Assistant's precise placement proposal (pending explicit approval if needed):** A peaceful secluded grove at the **southern, Littleroot-facing end of Route 101**, mere steps north of Littleroot Town, available on returning home as Champion. No pre-Champion object/spoiler blocking the starting route: spawn/enable the static encounter only after the Hoenn Champion flag. The assistant suggests that this be an evocative, calm encounter rather than a long side quest. Route 101 precisely connects Littleroot and Oldale (public reference: https://bulbapedia.bulbagarden.net/wiki/Route_101 ).

**Open:** Exact tile/clearing and whether any bespoke dialogue or cutscene is warranted, Complete encounter level, ability implementation for permanent Complete, catch/faint/revisit behavior. Do not add a Greninja-in-party gate or a Power Construct prerequisite to the permanent Complete boss unless the user asks.

**Status of all three forms:** (1) **10%**: Petalburg Woods, early, static, level ~8–10, Aura Break only; (2) **50%**: Meteor Falls, ~6–7 badges, static, Power Construct immediately functional; (3) **permanent Complete**: just outside Littleroot near Route 101 after Hoenn Champion, user-approved near-home concept, suggested precise grove. All separate captures and simultaneously ownable, one National Dex species. No Zygarde cell chores.

**Absolute scope restriction:** OFFLINE DESIGN ONLY. Do not read/search/edit/commit user GitHub project, builds, tests, CI, or implementation until expressly authorized by the user.

## Hoenn Generation 6: Zygarde Complete Route 101 location approved (2026-10-24)

The user replied **"Kool"** to the proposed secluded Route 101 grove, thereby confirming the precise **Route 101 clearing near Littleroot Town** for the distinct, permanently Complete-form Zygarde encounter. Unlock after becoming **Hoenn Champion**. This confirms the assistant's prior precise-location proposal; it is **no longer pending approval**.

- 10%: **Petalburg Woods**, early static, ~lv 8–10, **Aura Break only**.
- 50%: **Meteor Falls**, ~6–7 badges, separate static, **Power Construct usable immediately**, temporary Complete in battle at HP threshold and reversion after battle.
- Complete: **Route 101 grove, very near Littleroot**, post-Hoenn-Champion, separate permanent Complete static, no dependence on Greninja.
- All three individually ownable. Permanent Complete ability, levels and exact map coordinates left for future planning/authorized technical audit.

Continue with **Hoopa and Volcanion**. Do not access or modify the GitHub repository or perform implementation.


## Hoenn Generation 6: Hoopa and Volcanion locations approved (2026-10-24)

The user replied **“Yea. They're good”** to the two proposed locations. Both are now **approved**, not tentative:

| Pokémon | Confirmed site | Approximate game timing | Encounter policy |
|---|---|---|---|
| **Hoopa** | **Mirage Tower, Route 111 desert** | Roughly **4 Hoenn badges**, as access permits | **Independent scripted static**, no other Legendary/Mythical capture prerequisite |
| **Volcanion** | **Mt. Chimney** | Roughly **4–5 Hoenn badges**, as access permits | **Independent scripted static**, no other Legendary/Mythical capture prerequisite |

These locations are deliberately thematic: Hoopa's dimensional-ring mystery in a vanishing desert tower; Fire/Water Volcanion among Mt. Chimney's volcanic steam. Exact map tile, individual levels, how Mirage Tower access and persistence are handled (including the game's original tower storyline), and technical engine interactions are **not yet approved/verified** and should be reviewed only after authorized engineering work begins. Do not accidentally turn the approximate badge windows into additional scripted locks without reason. All planned static encounter catch/faint/revisit rules remain in force.

**Milestone:** Hoenn Generation 6 initial Legendary/Mythical location planning is now complete: Diancie (Granite Cave Steven's Room after two badges and letter delivery); Xerneas (Route 119), Yveltal (Mt. Pyre exterior); Zygarde 10% (Petalburg Woods), 50% (Meteor Falls), permanent Complete (Route 101 near Littleroot after Champion); Hoopa (Mirage Tower); Volcanion (Mt. Chimney). Zygarde's three forms remain one Pokédex species, with the specific separate-catch and Power Construct mechanics agreed in earlier sections.

**Scope boundary:** Offline discussion/documentation only. No GitHub repository access or changes, coding, builds, CI, or gameplay testing without the user's explicit authorization.


## Hoenn Generation 9: box Legendary timing (USER-APPROVED, 2026-10-24)

The user decided that **both box Legendaries, Koraidon and Miraidon, should become obtainable around Hoenn badge 7** rather than early or post-Champion. This is the intended natural gameplay window; the precise required flags, locations, levels, and scripted encounter details **remain open**. Do not silently treat this approximate timing as a rigid badge-check requirement if natural story access can do the job.

- **Koraidon:** approximately 7 Hoenn badges; location TBD.
- **Miraidon:** approximately 7 Hoenn badges; location TBD.
- Both should follow the project's general policy of deliberate static Legendary encounters, not ordinary wild encounters or roaming, unless subsequently changed by the user.
- All other Hoenn Gen 9 Legendary/Mythical placements, including Treasures of Ruin, Ogerpon, Loyal Three, Terapagos, and Pecharunt, await discussion. Paradox species remain classified as rare normal wild Pokémon under previously approved global policy.

**Strict scope:** Offline design discussion and notes only; do not access or modify GitHub, implementation, builds, tests, or CI without explicit authorization.


## Hoenn Generation 9: Treasures of Ruin at the Regi ancient sites (USER-APPROVED CONCEPT, 2026-10-24)

The user explicitly confirmed that placing all **four Treasures of Ruin at or around Hoenn's original Regi ruin sites** was their intended plan. Adopt the proposed one-to-one pairing as the current distribution concept:

| Treasure of Ruin | Site | Original Regi content |
|---|---|---|
| **Ting-Lu** | **Desert Ruins**, Route 111 | Regirock |
| **Chien-Pao** | **Island Cave**, Route 105 | Regice |
| **Wo-Chien** | **Ancient Tomb**, Route 120 | Registeel |
| **Chi-Yu** | **Sealed Chamber**, Route 134, underwater-access site | Original Regi-unlocking chamber and puzzle |

**Preserve the original Regi rooms, encounters and puzzles exactly as the user previously instructed.** Treasures should be separate scripted statics in new/adjacent spaces or carefully noninterfering places at these sites; precise rooms/tiles and whether the extra paths share puzzle prerequisites remain implementation details to discuss/audit later, not a license to simplify or replace the original Regi puzzles. All four should be independently catchable without requiring another Treasure.

**Dive access clarification, verified against Pokémon Emerald progression:** HM08 Dive is obtained from Steven **at his Mossdeep City house after the Team Magma Space Center event**, after earning the **seventh (Mind) Badge** from Tate & Liza. The Mind Badge is required for field use. Accordingly, **Chi-Yu at the Sealed Chamber is naturally accessible no earlier than the seven-badge stage**, and intended to remain pre-Champion; do not impose an additional late/postgame gate merely for dramatic reasons. References: https://bulbapedia.bulbagarden.net/wiki/Mossdeep_City and https://bulbapedia.bulbagarden.net/wiki/Walkthrough:Pok%C3%A9mon_Emerald/Part_14 . Other three sites follow their natural HM/puzzle accessibility, with no newly invented badge gates. Verify exact room accessibility and no overlap with Regi event objects only during later authorized implementation.

**Strict scope boundary:** OFFLINE DESIGN ONLY; never access or change the user's GitHub repository, source, builds, tests, or CI without explicit authorization.


## Hoenn Generation 9: Ogerpon, Loyal Three and Pecharunt hunt (DISCUSSION, 2026-10-24)

**User-confirmed high-level choice:** Build an **early-to-midgame hunting sequence** connecting Ogerpon, the Loyal Three (Okidogi, Munkidori, Fezandipiti), and the mastermind Pecharunt. Preserve their adversarial relationship from Scarlet/Violet lore without needing a lengthy recreation of the DLC. Keep the quest optional and compatible with normal Hoenn Gym progression. User has *not yet approved* the particular location/time suggestions below.

**Assistant's initial layout, PENDING USER APPROVAL:**

| Pokémon | Suggested static location | Approximate timing | Notes |
|---|---|---|---|
| Ogerpon | Route 117, grove near Verdanturf | 2–3 badges | First encounter starts the hunt |
| Okidogi | Route 112, rocky foothills | ~3 badges | One of three hunters' targets |
| Fezandipiti | Route 113, ash-covered fields | ~3 badges | One of three hunters' targets |
| Munkidori | Route 114, near the river | 3–4 badges | One of three hunters' targets |
| Pecharunt | Route 117, hidden berry grove | 4–5 badges | Final mastermind encounter; return to the starting area |

**Suggested mechanics, also PENDING APPROVAL:** Catching/meeting Ogerpon begins a short clue-led pursuit; Loyal Three are independent scripted static catches that can be obtained in any order; catching all three unlocks the Pecharunt static encounter. Do not require Ogerpon in the active party. No extensive DLC plot, no disruption of Emerald's core story. Keep the usual faint/flee/revisit persistence policy and don't punish unsuccessful attempts. The exact trigger for starting the hunt and exact cutscene/dialogue budget remain to decide.

**Map progression rationale:** Route 117 and Verdanturf can be visited by Mauville, and Routes 112–114 lie along the natural journey north and west of Mauville toward Fallarbor and Meteor Falls. No reason to put this quest off to postgame. Reference for broad geography/progression: https://bulbapedia.bulbagarden.net/wiki/Hoenn_Route_117 and https://bulbapedia.bulbagarden.net/wiki/Hoenn_Route_113 .

**Critical scope:** Offline discussion and note keeping only. No repository access, commits, source changes, builds, CI, or testing until expressly authorized by the user.


## Hoenn Gen 9: Ogerpon hunt order approved (USER-APPROVED, 2026-10-24)

The user explicitly chose **“Ogerpon trigger. Then loyal 3. Then perachaunt.”** This supersedes the prior 'trigger TBD' wording for the hunt. The required quest progression is:

1. **Catch Ogerpon** to activate the optional hunting sequence. Ogerpon is the story's starting point; it does not need to remain in the active party.
2. **Find and catch all three Loyal Three: Okidogi, Munkidori and Fezandipiti.** They may be captured in **any order** once the hunt is activated; no one member should require another member to be caught.
3. **After all three have been caught, unlock Pecharunt** as the final, deliberately scripted encounter. Pecharunt is the mastermind reveal and does not appear as a catchable static before the trio is registered.

This quest should happen **early-to-midgame, roughly badges 2–5**, and should be optional, short, and compatible with the regular Emerald story. Standard static catch/faint/leave/revisit persistence rules apply. No long forced DLC recreation.

**Location distinction:** The earlier proposed Route 117 grove (Ogerpon), Route 112 foothills (Okidogi), Route 113 ash fields (Fezandipiti), Route 114 river (Munkidori), and Route 117 hidden berry grove (Pecharunt) are still *tentative proposals* unless separately approved. The **sequence and capture prerequisites are approved**, not automatically all sites, specific levels or scripts.

**Strict scope:** Offline notes/discussion only. Do not access, inspect, or modify GitHub, code, CI, tests or builds until expressly authorized.


## Hoenn Gen 9: Terapagos Stellar Form permanence (USER-DIRECTED DESIGN, 2026-10-24)

**User asks to make Terapagos's final Stellar Form permanent**, analogous to the separately permanently obtainable Complete Zygarde instead of losing the final form at battle's end. Treat permanent Stellar as an intended design change, not as a claim current engine supports it out of the box.

- **Stellar Form Terapagos retains its own form on capture/party/PC/save/load/entering and leaving battles.** No need to re-Terastallize each battle, and do not force it to revert to Normal/Terastal after a fight.
- Preserve Stellar Form's **700 base-stat total** (HP 160 / Atk 105 / Def 110 / SpA 130 / SpD 110 / Spe 85).
- Its canonical form Ability is **Teraform Zero**, which normally activates *when changing into* Stellar Form to clear field weather and terrain. If permanently Stellar at battle start, ability will likely require a custom **on-entry trigger** so it remains useful. Decide exact behavior at later mechanics review.
- The original game normally has only temporary Stellar Form through Terastallization, with Normal primary type and special Stellar Tera effects; permanent battle eligibility and Tera Starstorm's Stellar behavior therefore require **custom form/typing/move/save/battle handling**, not an assumed simple configuration toggle. Public references: https://bulbapedia.bulbagarden.net/wiki/Terastal_Form ; https://bulbapedia.bulbagarden.net/wiki/Teraform_Zero_(Ability) ; https://www.serebii.net/scarletviolet/formchange.shtml . Public upstream expansion changelog indicates existing Stellar/Terastallization foundation, but the user's private fork integration has not been inspected.
- **Unresolved:** Whether permanent Stellar is a separate postgame static capture or an unlock/item for an existing Terapagos; its timing, location, level and any special encounter story; whether it retains permanent Stellar Tera offensive properties without using Terastallization. Do NOT silently decide this.
- Universal Terastallization remains postponed to P5; the permanent Stellar concept can be planned now, implementation scope to assess only after express authorization.
- **Absolute scope:** offline documentation ONLY. Do not access user GitHub repository, code, CI, tests or builds without explicit user authorization.


## Hoenn Gen 9: Permanently Stellar Terapagos separate post-Champion capture (USER-APPROVED, 2026-10-24)

The user explicitly selected **a separate postgame catch** for **permanent Stellar Terapagos**, rather than an upgrade to the initial Terapagos. This **resolves and supersedes** the unresolved capture-versus-transformation choice in the section immediately above.

- Plan **two independently ownable Terapagos**: a regular Terapagos (Normal appearance, which enters Terastal Form during battle under its normal Tera Shift mechanics), and a **separate permanently Stellar-form Terapagos**.
- **Permanently Stellar Terapagos is encountered as a scripted static only AFTER the Hoenn Champion.** Once caught, it stays Stellar through party/PC storage, saves, out-of-battle and subsequent battles. No repeated manual Terastallization is needed.
- The initial Terapagos is not consumed, replaced or forced to transform permanently; the player can own both simultaneously, with one Terapagos National Pokédex species entry.
- Retain prior intended 700-BST Stellar stats and make its signature mechanics function appropriately through future custom work. **This is a design requirement, not an assertion of existing engine support**. Exact on-entry Teraform Zero and Stellar Tera Starstorm semantics remain to be verified/decided in authorized engineering.
- **Still TBD:** static encounter locations, levels, capture prerequisites beyond the post-Hoenn-Champion condition for permanent Stellar; regular Terapagos timing (~Gym 5–6 was an assistant suggestion, not explicitly approved); how the normal Tera Shift/Terastal-form behavior coexists with the project's P5 Terastallization plan. Do not invent extra requirements.

**Strict boundary:** This document is an OFFLINE design ledger. Do not inspect, access or modify the GitHub repository, source files, CI, builds or gameplay tests without the user's express authorization.


## Hoenn Gen 9: Regular Terapagos early-midgame availability (USER-APPROVED, 2026-10-24)

The user explicitly chose **around Hoenn Gym 4** for the **regular, base Terapagos**, superseding the earlier assistant-only Gym 5–6 suggestion. Its initial out-of-battle Normal Form and Tera Shift to Terastal Form in battle remain the intended ordinary Terapagos form mechanic, subject to later implementation verification. Do not confuse regular Terapagos (450 BST in Normal appearance, 600 BST Terastal battle form) with the permanently Stellar 700-BST post-Champion separate catch.

- **Regular Terapagos:** independently catchable scripted static around **4 Hoenn badges** (Lavaridge/Flannery progression); specific level and map position still **TBD**. Avoid additional Legendary/Pokédex capture prerequisites.
- **Permanent Stellar Terapagos:** remains **a separate scripted post-Hoenn-Champion catch**; player can own both simultaneously.
- Candidate thematic site (ASSISTANT SUGGESTION ONLY, NOT APPROVED): a modest crystal/mineral nook around Route 114 or nearby accessible cave, to fit natural Gym-4 progression without overlapping the already assigned Diancie, Hoopa, Zygarde or Volcanion encounters.
- No changes to the user's GitHub repository, source files, tests or CI. Offline decisions only.


## Hoenn Gen 9: Both Terapagos encounter sites finalized (USER-APPROVED/USER-DELEGATED, 2026-10-24)

The user approved **Route 114** for regular Terapagos and explicitly delegated the choice of postgame cave for permanent Stellar Terapagos to the assistant, noting unfamiliarity with the Hoenn map. The assistant selected **Cave of Origin in Sootopolis City**, specifically its deepest gemstone-lined chamber (B1F), after the Hoenn Champion. These are the current placements, superseding the 'locations TBD' in the preceding Terapagos sections.

| Form | Exact intended site | Natural stage | Special notes |
|---|---|---|---|
| **Regular Terapagos** | A **small newly added crystal grotto/alcove off Route 114** (not a claim such a room exists in stock Emerald) | **Around Hoenn Gym 4** | Scripted static, ordinary Terapagos with Tera Shift / battle Terastal-form behavior subject to future implementation review; level TBD; no other Legendary prerequisites |
| **Permanently Stellar Terapagos** | **Cave of Origin B1F**, Sootopolis City, gemstone-lined deepest chamber | **ONLY after Hoenn Champion** | Separate scripted static catch, 700 BST permanently Stellar in PC/party/battles; level TBD; no other Legendary prerequisites |

**Required accessibility change:** In the original *Pokémon Emerald*, **Cave of Origin becomes inaccessible after Wallace's Rayquaza discussion/story event** and remains closed even after the Champion. During a future authorized implementation, preserve the original Wallace/Rayquaza story behavior, but **reopen entrance access only after the Hoenn Champion** for the separate Stellar Terapagos encounter. Add an encounter to the bottom chamber without interfering with story objects or cutscenes. In the original Emerald layout the lowest floor is lined with sapphire/ruby gems, reinforcing the thematic selection. References: https://bulbapedia.bulbagarden.net/wiki/Cave_of_Origin ; https://bulbapedia.bulbagarden.net/wiki/Appendix:Emerald_walkthrough/Section_16 .

The user already confirmed that the permanent 700-BST Stellar Form should **be a separately catchable permanent form**, not a transformation consuming the ordinary Terapagos. Keep both attainable simultaneously. Normal Terapagos remains ~Gym 4 and must not be delayed until the postgame. The Stellar typing/Starstorm/Teraform Zero on-entry mechanics are **custom work needing later engineering**, not assumed supported yet. No imposed need to catch regular Terapagos, Rayquaza or any other Legendary to unlock Stellar, beyond becoming the Hoenn Champion.

**NON-NEGOTIABLE SCOPE:** This is offline discussion and documentation only. Do NOT access/search/inspect/edit GitHub, trigger CI, run builds/tests, or implement source changes without direct user authorization.


## Deferred implementation priority: special encounter mechanics (USER-APPROVED, 2026-10-24)

The user explicitly instructed: **Put permanently catchable Complete Zygarde and permanently Stellar Terapagos at late implementation priority, alongside the special Zacian + Zamazenta double battle.** These are **deferred engineering features**, to tackle *after ordinary Legendary/Mythical encounter placements and other core gameplay fixes*, rather than blocking completion of the wider static encounter pass. They may require substantial custom engine handling.

**Late-priority feature group:**

1. **Zacian + Zamazenta special double battle** in Johto; defeat both to receive both Pokémon. Location and other details remain unresolved. Preserve previously approved double-battle reward semantics.
2. **Permanent Complete Zygarde**: separately catchable, remains in Complete Form outside and inside battle, located Route 101 near Littleroot after the Hoenn Champion. Independent of regular Zygarde catches.
3. **Permanent Stellar Terapagos**: separately catchable, remains Stellar outside and inside battle, located Cave of Origin B1F after the Hoenn Champion; ensure postgame cave access. Separate from regular Terapagos.

**Do not confuse implementation priority with in-game availability.** The post-Champion requirements for Complete Zygarde and Stellar Terapagos are unchanged, while Zacian/Zamazenta's *in-game* timing remains to be decided. This grouping does not change or defer the ordinary versions: early Zygarde 10% (Petalburg Woods), mid-late Zygarde 50% (Meteor Falls, with usable Power Construct and temporary Complete transformation before Champion), and midgame regular Terapagos (Route 114, around Gym 4) stay in the standard encounter implementation work. Do not postpone all Zygarde or Terapagos gameplay until this late phase.

**Offline design only.** Do NOT inspect or touch the user's GitHub repository, code, CI, builds, or tests without explicit authorization.


## Delegated placement closeout following pre-implementation audit (2026-10-24)

**Authority and scope:** The user, after reviewing the October 24 pre-implementation audit, explicitly delegated final selection of the remaining locations to Astra: “Now that you know where everything else is located, just choose Good places for these lol”. The following **design selections** close the outstanding placement questions without requiring another individual yes/no. This does **not** authorize repository access, source changes, implementation, builds, CI, or tests. All earlier user-approved constraints and requirements override any approximate detail below. These are proposals accepted under delegated design authority, **not verified current source placements**.

### Johto missing encounters, now selected

| Encounter | Selected final design site | Natural unlock / constraints | Proposed level | Collision and engineering notes |
|---|---|---|---|---|
| Kubfu copy 1 | **Sprout Tower**, a meditation alcove away from the Sage Elder's event | During the first Violet City visit, before or near badge 1 | 12 | Independent static; does not replace Flash reward or trainer battles |
| Kubfu copy 2 | **Route 34**, small meadow beside the Day-Care | After Ilex Forest, approx. 2 badges | 17–18 | Independent second catch; both Urshifu branches available within same file |
| Reshiram | **Bell Tower, lower interior floors** in a distinct ancient chamber | Around 7–8 Johto badges; after the native Rocket Radio Tower/Rainbow Wing tower-admission process, NOT after the Beast-trio prerequisite reserved for Ho-Oh on summit | 43–45 | Do not alter Ho-Oh rooftop event; ensure lower-floor tower access doesn't accidentally depend on capturing Ho-Oh or the beasts |
| Zekrom | **Dark Cave, northeastern/Blackthorn-side chamber**, reached from Route 45 | Blackthorn approach, around 7–8 badges | 43–45 | Do not require reaching the Dark Cave entrance from early Route 31 or crossing late HM-blocked internal areas |
| Kyurem | **Ice Path B1F, separate frozen alcove** | Catch both Reshiram and Zekrom; return to Ice Path on ordinary accessible path, **before Johto Champion** | 48–50 | Glastrier remains Ice Path B2F; no collision. DNA Splicers rewarded on Kyurem's catch |
| Zacian + Zamazenta | **Route 27, a secluded shrine/clearing near the Tohjo Falls approach** | After 8 Johto badges, on way to League | ~50 each | **SPECIAL CUSTOM DOUBLE BATTLE LAST** in implementation priority. Defeat both and win the full battle to receive both; neither awarded on partial win/loss. Site and timing set without authorizing engineering |
| Eternatus | **Victory Road, a hidden side cavern** | After receiving both Zacian and Zamazenta, approx. 8 Johto badges / pre-Champion | ~55 | Independent static that doesn't impede existing Victory Road route or rival/story battle. Eternamax still later / not resolved |

**Note on tower timing:** Original HGSS tower admission/mascot/League progression varies by version; preserving actual *Grand Anthology* story logic is mandatory. If access to lower floors is only available after eight badges, adjust Reshiram's effective time to that natural progression rather than breaking the story. The user gave “7–8 badges”, not an imperative exact 7-badge unlock. The Route 27 league path similarly may inherit mandatory story requirements; never shortcut those requirements simply to realize the static.

### Hoenn box dragons, now selected

| Encounter | Selected final design site | Unlock and access | Proposed level | Notes |
|---|---|---|---|---|
| Koraidon | **Ancient grotto off Route 128, branching from an underwater/Dive area** | **After Hoenn Gym 7**, when Dive becomes available following the Mossdeep Space Center Magma event | 44–46 | New, optional small prehistoric cavern; avoid rewriting Team Aqua's Seafloor Cavern / Kyogre events. Independently catchable |
| Miraidon | **New Mauville, a separate sealed generator annex** | **After Hoenn Gym 7 and completion of the Space Center Team Magma event** | 44–46 | Existing New Mauville / Wattson power-plant quest remains intact, accessible earlier as originally; only new side room opens late, no main-generator changes. Independently catchable |

### Previously tentative but now delegated as exact sites / practical fixes

- **Hoopa:** Retain the user's approved **Mirage Tower** theme and Route 111 desert site. Put its static in an accessible **Ring Chamber near the permanent base/ruins of Mirage Tower**. The original fossil tower can collapse after a fossil is chosen, **but the ring-arch doorway to Hoopa's chamber must remain accessible afterward**. Hoopa catch must not depend on which fossil was taken or whether player completed tower puzzle. Implement as a small permanent alcove/map off Route 111 that visually references the tower; *do not* make the fossil tower itself permanently accessible unless easiest safe solution preserving the same behavior. Approx. badge 4, level ~30. Preserve original Mach Bike/cracked-floor puzzle and both-fossil acquisition flow. This resolves the permanent-missability trap with minimum story intrusion.
- **Deoxys:** Original **Birth Island** puzzle and static, after **Rayquaza registered caught**. Choose **a Lilycove Harbor ferry destination that activates immediately when Rayquaza is caught**, without requiring AuroraTicket or a post-Champion S.S. Ticket. Accessible *before* Hoenn Champion because user's only desired unlock was Rayquaza. Ensure original S.S. Aqua journey and normal ticket events are preserved. This is a player-friendly delegation-based selection, not a prior explicit user statement about pre-League access.
- **Victini:** **Johto National Park**, once player won **any one** regional championship; separate from Virizion's earlier park encounter. Revisit after Kanto championship should work when the Johto map is open. No second championship requirement.
- **Genesect:** **Goldenrod City**, quiet industrial/workshop corner rather than Radio Tower interior; ~3 badges. No extra legendary prerequisite.
- **Enamorus:** **Route 39**, grassy clearing near MooMoo Farm; after Tornadus + Thundurus + Landorus caught, before Johto Champion if the natural timing allows.
- **Johto Swords of Justice exact spots:** Keldeo Route 32 waterside; Terrakion Union Cave accessible main level; Virizion National Park in a distinct grassy section from Victini; Cobalion Mt. Mortar accessible western entrance. All independent early/mid encounters; none requires the other three.
- **Urshifu scroll items:** Scroll of Waters awarded after Chuck's Cianwood Gym challenge (~gym 5); Scroll of Darkness as Mt. Mortar Karate King reward (Waterfall access, late Johto around 8 badges). Neither scroll forces an either/or selection, both stay obtainable. Ordinary evolution mechanics and appropriate level requirements to verify during source implementation.
- **Hoenn Ogerpon hunt static layout:** Adopt the previously proposed sequence/sites: catch Ogerpon in Route 117 grove → Okidogi Route 112 rocky foothills, Fezandipiti Route 113 ash field, Munkidori Route 114 riverside in any order → Pecharunt hidden Route 117 berry grove after catching all three. Short optional badges ~2–5, no forced party slot. Exact tiles/scripts later.
- **Kanto Lapras water patch:** Add uncommon wild Surf Lapras on **Route 20 coastal waters outside Seafoam Islands**, while retaining original Icefall Cave source; no special legendary gate. This protects Lapras after Silph Co gift becomes Magearna.
- **Kanto Darkrai:** Lavender Pokémon Tower **7F**, after caught Cresselia and Fuji rescue as previously proposed; protect the Mr. Fuji storyline and don't erase essential story actors.

### Remaining verification items, not grounds for more location debate

- Fine-tune exact battle levels (proposed levels are estimates, not immutable), map tiles, NPC clues, new pocket room graphics, and caught/faint/retry flags only once source work is authorized.
- Verify story gates for Johto Bell Tower and Route 27, the two independent Kubfu sources, Kyurem and Ice Path Glastrier room separation, New Mauville annex, Route 128 Dive grotto, and Hoopa base doorway post-fossil-collapse.
- Do not confuse independent *encounter-caught flags* with species-wide National Dex registration, especially multi-form Zygarde, Terapagos, Cosmog, Kubfu, and Galarian birds.
- **Late engineering remains deferred**: Zacian+Zamazenta dual-battle reward; permanent Complete Zygarde; permanent Stellar Terapagos. Their in-game sites/timing remain design goals.
- **NO GitHub/implementation permission granted**. First authorized source step must be **read-only current `main` audit** comparing historic authored locations to this plan, not silent code changes.