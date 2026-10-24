> **Publication note, 24 October 2026:** This is an offline discussion-based design audit, now published at the maintainer's request. It is **not** a fresh current-branch source analysis or proof of gameplay reachability. Pair with [design ledger](LEGENDARY_MYTHICAL_DESIGN_LEDGER_2026-10-24.md), [authored source location snapshot](LEGENDARY_MYTHICAL_LOCATION_AUDIT_2026-10-24.md) and [current handoff](NEXT_ASTRA_HANDOFF.md). Historical repo access restrictions in this dated audit are superseded solely for the documentation handoff, not gameplay changes.

# Pokémon Grand Anthology | Pre-Implementation Design Audit

**Date:** October 24, 2026  
**Status:** READ-ONLY, OFFLINE PLANNING REVIEW. **Not** authorization to access, inspect, edit, commit, build, test, or otherwise touch either GitHub repository.  
**Source materials:** Latest offline discussion ledger (`grand_anthology_design_decisions_late_priority_forms.md`, 595 lines), original October 24 source-location audit (`grand_anthology_legendary_mythical_location_audit(1).md`), latest user decisions in this conversation, and public Pokémon Emerald progression references. No fresh repository inspection, emulator test, or current source parsing was performed.

## Executive verdict

**Design coverage: 94/94 National Legendary/Mythical species have an authored or planned acquisition method.** The 94 excludes Ultra Beasts/Paradox Pokémon, which deliberately use rare natural encounters, and counts alternative forms under their base National species. This is design/source *coverage*, not evidence that all 94 are currently catchable in-game. It is not a proof of runtime availability of all 1,025 National species.

| Region / assigned generations | National Legendary/Mythical species | Assessment |
|---|---:|---|
| Kanto: Gen 1 (5), Gen 4 (14), Gen 7 (16) | **35** | Main encounter plan settled; inherited duplicates, clustered cave statics, certain specific sites/flags still need cleanup |
| Johto: Gen 2 (6), Gen 5 (13), Gen 8 (12) | **31** | Lore chains settled; Tao Trio sites, two Kubfu sites, Zacian/Zamazenta battle site, Eternatus site not decided |
| Hoenn: Gen 3 (10), Gen 6 (6), Gen 9 (12) | **28** | Most encounter locations agreed; Koraidon/Miraidon sites, Ogerpon quest location ratification, and Mirage Tower accessibility remain |
| **Total** | **94** | **No absent base Legendary/Mythical species in the plan, but technical/runtime completeness unverified** |

**Decision state:** Good enough to plan engineering batches, but not yet a complete unambiguous static-location specification. Avoid treating assistant suggestions and historical append-only proposals as user approvals.

## A. Finalized boundaries, not up for re-litigation

- One persistent trainer, party and storage system across original Kanto → Johto → Hoenn journeys. All 1,025 National Pokémon species are the full-game goal; they do **not** all have to be caught before the first Champion. Accessibility and interesting geographic discovery take precedence over power-based badge gates.
- Legendary/Mythical Pokémon usually appear as deliberately discoverable **static** encounters. Lore-supported multiple-individual exceptions may have natural habitats. **Ultra Beasts and Paradox Pokémon** are rare normal species for encounter purposes; keep deliberate early Kartana and Poipole paths.
- **No mandatory roaming**: Johto beasts and Hoenn Eon duo must have fixed catches. Keep original important plot events and Regi puzzles.
- Fainted statics must be retryable after leaving/re-entering; capture removes the specific encounter persistently; fleeing does not permanently remove it. Caught prerequisites check Pokédex registration, not current party/PC possession.
- The three Regi puzzles, native iconic battle sites, optional story scenes, and fossil acquisition must not be broken by additions. No event-distribution ticket should be required for Deoxys.
- Postgame special engineering **last**: Zacian + Zamazenta custom double-battle reward; independent permanent Complete Zygarde; independent permanent Stellar Terapagos. Their forms/locations remain planned even though their *implementation* is deferred. Ordinary Zygarde and Terapagos continue in normal queue.
- Extended PC, universal Terastallization, Mega, Z-Moves, Dynamax and cosmetics remain later project phases where applicable. Do not reopen settled preferences merely because the engine offers more options.

## B. Actual design gaps before a clean static encounter implementation spec

### B1. Site not finalized (high priority)

| Region | Encounter | Approved constraints | Missing decision |
|---|---|---|---|
| Johto | Reshiram | Separate catch at ~7–8 Johto badges | Exact location, natural access, level |
| Johto | Zekrom | Separate catch at ~7–8 badges | Exact location, natural access, level |
| Johto | Kyurem | Both Reshiram and Zekrom caught; **before Johto Champion**; DNA Splicers reward | Exact pre-League location; Ice Path is only a proposal and overlaps Glastrier |
| Johto | Kubfu **two opportunities** | Early availability, both Urshifu styles attainable | Two independent sources/sites, levels, no mutually exclusive unlock |
| Johto | Zacian + Zamazenta | Joint battle; defeat both, automatically receive both; late ENGINEERING priority | One battle site, timing/level, Crowned forms/items; do not implement early |
| Johto | Eternatus | Both wolves caught | Site, level, time; defer Eternamax separately |
| Hoenn | Koraidon | Around badge 7, static, independent | Final thematic site/level; earlier authored Route 111 is reachable too early unless specifically gated |
| Hoenn | Miraidon | Around badge 7, static, independent | Final thematic site/level; earlier authored Route 110 is reachable too early unless specifically gated |

**Count:** 9 major encounter placements awaiting site selection when the two Kubfu opportunities and joint wolf battle are counted as separate placements. Not every missing exact level is a design blocker; level fitting can be safely delegated to the implementation pass after expected access is known.

### B2. Sites chosen as proposals but not individually ratified

- Johto **Victini** National Park after **any one** regional Championship (gate is approved; National Park site tentative).
- Johto **Genesect** Goldenrod City (assistant suggestion, no elaborate quest; not explicit individual user approval).
- Johto **Enamorus** Route 39 near MooMoo Farm (gate: caught Tornadus + Thundurus + Landorus approved; place proposed).
- Johto Gen 5 early Swords of Justice site assignments (Route 32, Union Cave, National Park, western Mt Mortar) fit user's approved early-mid timing, but not every precise place was individually ratified.
- Johto **Urshifu scrolls**: Waters from Chuck and Darkness from Mt Mortar Karate King are proposed; explicit timing is approved, sites not yet.
- Hoenn **Ogerpon quest**: user approved required sequence (catch Ogerpon → catch all Loyal Three in any order → catch Pecharunt), badges 2–5; the Route 117/112/113/114/117 per-species static layout is still an assistant suggestion. No need to demand individual approvals if user delegates specifics later.
- Kanto **Darkrai** Lavender Pokémon Tower 7F is the current plan but original ledger notes exact floor was not individually ratified. Lapras Surf patch location remains open.

### B3. Two real map-access traps identified from Emerald game geography

1. **Hoopa in Mirage Tower, Hoenn Route 111.** The user approved the location. In the original Emerald game, Mirage Tower permanently vanishes when one fossil is selected. If Hoopa is left inside the vanishing map without protection, choosing a fossil could permanently eliminate the only intended Hoopa catch. **Suggested fix (not yet approved):** preserve the original fossil scene, but make the Mirage Tower re-enterable specifically for Hoopa, or use a non-disappearing adjacent desert ruin/alcove. Preserve the user's Mirage Tower theme if possible. Public reference: https://www.serebii.net/emerald/illusionpillar.shtml .
2. **Deoxys ferry to Birth Island.** In Emerald, an AuroraTicket **and an S.S. Ticket** are part of event-island transport; the S.S. Ticket normally becomes available after the Hall of Fame. The user approved **Rayquaza caught** as Deoxys's capture requirement and removal of the unavailable AuroraTicket dependency, but did **not** explicitly require pre-Champion Deoxys. If preserving postgame ferry access, Deoxys remains postgame **despite** catching Rayquaza pre-League. If intent is immediately after catching Rayquaza, a separate pre-Champion transport unlock must be added; avoid silently inventing this. Sources: https://bulbapedia.bulbagarden.net/wiki/AuroraTicket and https://pokemondb.net/pokebase/427121/what-requirements-needed-access-faraway-birth-island-emerald .

### B4. More map-and-story access checks; not necessarily design blockers

- **Zygarde 50% Meteor Falls ~6–7 badges:** choose a room accessible at that stage; Waterfall-locked chambers are too late. Keep ordinary 50% Power Construct transforming only temporarily and separately catchable permanent Complete at post-Champion Route 101.
- **Regi-area Treasures of Ruin:** Ting-Lu Desert Ruins, Chien-Pao Island Cave, Wo-Chien Ancient Tomb, Chi-Yu Sealed Chamber. Add neighboring object/chamber, **do not** alter original Braille puzzles or stand on event triggers. Chi-Yu's Dive-access stage is naturally after Gym 7. If the other three are placed strictly inside sealed rooms, they also inherit original late unlocks; determine whether they should be accessible alongside the ruins without requiring the Sealed Chamber puzzle.
- **Rayquaza pre-League:** Emerald has a separate awaken-story visit and a later return for the catch, still before the Champion. Do not mix flags for these phases. Groudon (Terra Cave) and Kyogre (Marine Cave) are postgame and independently check Rayquaza caught. Public mechanics: https://www.smogon.com/ingame/guides/capturing_rse_legendaries and https://www.serebii.net/emerald/story.shtml .
- **Cave of Origin:** normally closes after Emerald's story resolution; reopen after the **Hoenn Champion** for permanent Stellar Terapagos, while preserving the Wallace/Rayquaza scenes. **Route 114 base Terapagos** needs a *new* crystal grotto, not merely an object on a map already known to exist.
- **Johto story conflicts:** Burned Tower awakening and Beast Roamer machinery, Suicune/Eusine, Ho-Oh Bell Tower tower access, Lugia Whirl Islands, Olivine Lighthouse/Ampharos, Ice Path late access, Route 47 Cliff Cave access, and Ruins of Alph hidden Reins chamber all need flag/warp/collision tests.
- **Multi-region checkpoints:** Regigigas in Kanto Rock Tunnel after *Hoenn* Regis necessitates a meaningful revisitable Kanto site; Victini unlocked by *any* Champion should work after a Kanto championship before Johto; Kyurem prerequisite dragons must be reachable **before** Johto League.

## C. Old authored placements that clash with our *new* decisions

The original October 24 source audit is a historical snapshot. None of these are claims about today's live code state. Compare, remove/move/retire sources **only upon explicit authorization**.

| Encounter / mechanism | Earlier source arrangement | Final design intention |
|---|---|---|
| Mewtwo duplicate | Kanto Pokémon Mansion pre-League plus Cerulean Cave | Only Cerulean Cave B1F, after Kanto Champion, Mew caught and existing cave unlock |
| Shaymin | Kanto Route 5 | Viridian Forest before Brock |
| Giratina, Darkrai, Tapu Lele | Rock Tunnel | Giratina Mt Moon B2F; Darkrai Lavender Tower 7F; Tapu Lele Celadon gardens |
| Magearna and Lapras | Magearna Power Plant; Lapras Silph Co gift | Magearna substitutes **Silph gift**; Lapras gains Kanto Surf patch and keeps Icefall Cave |
| Marshadow, Necrozma, Meltan | Mt Moon; Mt Moon; Pokémon Mansion | Fighting Dojo; Cerulean Cave 2F; Vermilion Machop construction site |
| Johto beasts | Roaming engine and multiple backup statics | Burned Tower story preserved; fixed Raikou Route 38, Entei deep Mt Mortar, Suicune Lake of Rage |
| Johto Swords / forces / Tao Trio | Many source statics on Routes 38/42/45 | Distribute across agreed Johto progression and remove conflicts/duplicates |
| Galarian birds | Johto statics plus 3 Kanto ordinary wild species | Keep eligible statics, remove specified Kanto wild-grass source entries; form coexistence must work |
| Latias/Latios | Hoenn roamers, older Route 118/120 statics | Fixed Latias Route 120, Latios Mossdeep Space Center after Magma event; remove mandatory roamers |
| Jirachi / Deoxys | Jirachi Mossdeep Space Center; Deoxys Space Center plus Birth Island | Jirachi early Route 102 pond; Deoxys Birth Island after Rayquaza caught, no AuroraTicket requirement |
| Diancie / Xerneas / Zygarde | Diancie level 45; Xerneas Route 120; base Zygarde Granite Cave | Diancie level 18 and two badges/letter; Xerneas Route 119; 10% Zygarde Petalburg Woods, 50% Meteor Falls |
| Volcanion | Fiery Path | Mt Chimney around badges 4–5 |
| Four Treasures of Ruin | Previously scattered (Mt Pyre, Granite Cave, Fiery Path) | Regi ancient ruin sites as paired above |
| Ogerpon + Loyal Three + Pecharunt | Unrelated existing scripted locations | Optional ordered hunt gated by caught records, sites tentatively Routes 117/112/113/114/117 |
| Koraidon/Miraidon | Route 111/Route 110 (early) | Around seventh Hoenn badge, new sites TBD |
| Regular Terapagos | Mossdeep Space Center (late) | Route 114 crystal grotto at Gym ~4; permanently Stellar at Cave of Origin postgame |
| Wild canonical Legendary habitats | Groudon Route 111, Kyogre Seafloor Room 1, Galar birds three Kanto grass sites | Remove **those five specific wild entries**; never remove UB and Paradox rare wild entries wholesale |

## D. Engine risks and mechanical acceptance tests (when authorized)

| Risk | Required behavior | Test idea |
|---|---|---|
| **One species, several required captures** | The engine must **not hide form #2** merely because base National species is registered caught: 3 Zygarde forms, 2 Terapagos forms, 2 Cosmog catches, two Kubfu catches, Hoenn/Johto regional variant birds | Capture first, leave, reload, return for second, inspect species/form IDs, save/PC, catch both |
| **Static catch/faint/revisit** | One caught flag per encounter, no infinite gifts; faint/flee leaves reproducible encounter | Catch, save/reload; faint, exit/re-enter; test with storage full |
| **Original story events** | The existing event/warp/map scripts remain functional | Rayquaza dual visits, Regi Braille doors, Burned Tower awakening, Lighthouse, Mirage fossil, Birth Island triangle |
| **Cross-region caught dependencies** | Work by Dex caught registration even after release | Entei/Raikou/Suicune→Ho-Oh; Regis→Regigigas; Dialga/Palkia→Giratina; creation trio→Arceus; Cresselia→Darkrai; Reshiram/Zekrom→Kyurem; forces→Enamorus; wolves→Eternatus; Rayquaza→weather duo and Deoxys; Ogerpon+Loyal Three→Pecharunt |
| **Wild and post-League discovery** | Wild exceptions (UB/Paradox) preserved, five explicit bad habitats removed; generic Research and wild rotation filtered **case by case** to avoid bypassing important deliberate static, while preserving authorized multi-individual lore exceptions | Compare eligible pool before/after first Champion, with randomizer and Research target variations |
| **Forms and critical items** | Zygarde 10% Aura Break only; 50% Power Construct pre-League, temporary Complete revert; base Terapagos Tera Shift to 600-BST battle form; two Urshifu styles; reversible Kyurem and Calyrex fusions; intended Ogerpon masks; appropriate inventory | Encounter/evolve/fuse, ability-change patch exploit, reload, PC box cycle, postbattle revert; full storage edge cases |
| **Late custom forms** | Independently permanently catchable Complete Zygarde and Stellar Terapagos, persist PC/saves, proper moves/abilities; no assumption upstream already supports it | Implement **last**, then specialized persistence/battle tests |
| **Late custom double battle** | Both wolves awarded only if **both defeated and the full battle won**, none if partially defeated then failure; storage overflow safe | Specialized finish/loss/flee, duplicate prevention, save/load tests **last** |
| **All-1,025 proof** | Source-level candidate paths alone are not enough; the Dex and gameplay must confirm evolutions, branching forms, eggs, held items, TMs, shops, HM gates | Automated source inventory + runtime sample runs; no current proof from offline notes |

### Notable mechanics pending review (not grounds to restart all design)

- Ogerpon's **masks and typing**; forms are meaningful gameplay, unlike mostly cosmetic transformations. Define how they are obtained alongside the hunt if missing in the existing base.
- Galarian legendary birds must remain obtainable as *forms* even if the original bird species was caught elsewhere; old object disappearance keyed to species could break this.
- Existing roaming and post-League wild/Research/Randomizer systems may bypass designed encounter gates if not reviewed; do not disable them indiscriminately.
- Kyurem DNA Splicers, Calyrex Reins of Unity and both Urshifu scroll rewards should not destroy constituent Pokémon, lock a second form choice, or fail after PC moves.
- *P5 Terastallization deferment* vs regular Terapagos's Tera Shift/Terastal battle form: verify whether the ordinary 600-BST form can function independently of P5 and define a small exception if needed. A separate permanent Stellar 700-BST form is already late priority.

## E. Implementation order recommended only **after user authorizes GitHub work**

1. **Read-only source baseline:** Compare current `main` to this latest ledger, enumerate every old and new static/wild/gift source, prerequisites and flags. Identify exact map names, existing script events, engine implementation support and full acquisition coverage. This is *not done by this audit*.
2. **Core encounter pass:** Fix known wrong habitats/duplicates, early independent and traditional native statics, species-level form coexistence, static flags and repeat attempts; move script objects in sensible location batches.
3. **Quest and multi-region integration:** Ogerpon hunt; Johto beast wake conditions; weather trio and Rayquaza; Tao trio; Regi/Regigigas; event ferry; gifts, item sources, forms and universal Research/wild filters.
4. **Consolidated gameplay acceptance:** Build + emulator navigation checkpoints. Check normal story progression, encounters/catch/faint/save/revisit, level-range plausibility, full party/PC, randomizer, all-1,025 source inventory. Target representative paths before exhaustive regression.
5. **Deferred special-engineering queue:** Zacian–Zamazenta double battle; permanent Complete Zygarde; permanent Stellar Terapagos. Implement and separately test each **only once ordinary encounter work is stable**.

## F. Specific questions/choices to settle without unnecessary extra debate

**Need a conscious answer (or delegate choice):**
1. **Hoopa:** keep Mirage Tower accessible after fossil choice, or instead put its static in a nearby permanent desert ruin? Current literal tower site risks a permanently missable Pokémon.
2. **Johto/Hoenn undefined sites:** allow engineering-time thematic selection with progress window maintained, or approve a short final placement proposal list (Reshiram, Zekrom, Kyurem, two Kubfu, Eternatus, Koraidon, Miraidon; late wolf battle location can stay deferred).
3. **Deoxys timing:** Is Birth Island postgame by inherited S.S. Ticket fine, or should catching Rayquaza before Champion immediately open new ferry transport? Catch Rayquaza remains mandatory in either case.

**Safe to delegate without user reapproval:** exact levels for otherwise approved encounters, final collision-free tiles, respawn flags, minimal NPC dialogue, small hidden access pockets, representative game tests, and most other implementation details. Preserve all approved site/gate preferences.

## Final audit recommendation

- **Do not start implementation silently.** The user's request was to audit **before** implementation, not authorization to make repo changes.
- The design is **strongly coherent**: early favorites, traditional puzzles, lore dependencies, midgame discoveries, meaningful postgames. The most significant hidden issue is not encounter balance, but **one-time story/map triggers and the engine's tendency to key different forms to one caught species**.
- Once the few unassigned sites and Mirage Tower/ferry intention are resolved or explicitly delegated, begin a **read-only live repository implementation audit** when the user authorizes repository access; then implement in batches.