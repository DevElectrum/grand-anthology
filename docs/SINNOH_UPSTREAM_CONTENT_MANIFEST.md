# Sinnoh upstream content inventory v1.0 (pinned, READ ONLY)

**Snapshot:** [Platinum c79120f](https://github.com/sinnoh-remakes/pokeemerald-platinum/tree/c79120fbab7d62c66785c90b491e6a370249d5db); [Grand Anthology 6061928](https://github.com/DevElectrum/grand-anthology/tree/6061928f0a20d67b00b676e41c1d1d53f3c3b44c). This is a source-file comparison, not a dependency-closed, licensed or gameplay-complete import. See [full execution plan](SINNOH_FULL_IMPORT_EXECUTION_PLAN.md).

Source-only means a path present in upstream and absent in the baseline, not necessarily a file that should be imported. There are **3359** source-only blobs and **4025** same-path blobs with different content across the complete trees. Never merge whole trees.

## Inventory summary

| Candidate | Count | Note |
|---|---:|---|
| Sinnoh map groups | 13 | Append after 101 existing groups |
| Sinnoh maps | 93 | Existing baseline has 1,194 maps |
| Minimal scripts | 21 | Heuristic file length <=55 bytes; not all indicate broken content |
| Source-only layout binaries | 128 | 64 paired layouts |
| Source-only primary tileset dirs | 23 | Some future location artwork |
| Source-only secondary tileset dirs | 44 | Some future location artwork |
| Source-only overworld pics | 135 | File count, not character count |
| Source-only overworld palettes | 127 | File count |
| Source-only trainer front pictures | 92 | Only select referenced portraits |
| Source-only trainer back pics/palettes | 8 | Only select referenced pictures/palettes |
| Source-only MIDI | 191 | Must resolve voicegroup/sample closure |
| Sinnoh encounters | 60 | Morning/day/night records |

## All 93 upstream Sinnoh maps

A bold **[skeleton]** marker means only that scripts.inc is <=55 bytes. Each map needs dependency, event, connection, collision, progression, warp and trainer-ID verification. Source paths: data/maps/<name>/map.json and scripts.inc.

| Upstream map group | Map count | Names and script triage |
|---|---:|---|
| gMapGroup_Towns | 8 | TwinleafTown, SandgemTown, JubilifeCity, OreburghCityNorth, OreburghCitySouth, FloaromaTown, EternaCityNorth, EternaCitySouth |
| gMapGroup_Routes | 11 | Route201, Route202, Route203, Route204North, Route204South, Route205North, Route205South1, Route205South2, Route207, Route211West, Route219 |
| gMapGroup_Caves | 5 | OreburghGate, OreburghMine_B1F, OreburghMine_B2F, RavagedPath **[skeleton]**, MtCoronet_N1F1 **[skeleton]** |
| gMapGroup_Lakes | 2 | LakeVerity, LakefrontVerity |
| gMapGroup_Gates | 3 | Gate_Jubilife **[skeleton]**, Gate_Cycling_North **[skeleton]**, Gate_Cycling_South **[skeleton]** |
| gMapGroup_Miscellaneous | 6 | ValleyWindworks, FloaromaMeadow, EternaForest1, EternaForest2, EternaForestOuter1 **[skeleton]**, EternaForestOuter2 **[skeleton]** |
| gMapGroup_IndoorTwinleaf | 6 | TwinleafTown_PlayersHouse_1F, TwinleafTown_PlayersHouse_2F, TwinleafTown_RivalsHouse_1F, TwinleafTown_RivalsHouse_2F, TwinleafTown_HouseA, TwinleafTown_HouseB |
| gMapGroup_IndoorSandgem | 7 | SandgemTown_ProfessorRowansLab, SandgemTown_HouseA, SandgemTown_HelperHouse_1F, SandgemTown_HelperHouse_2F **[skeleton]**, SandgemTown_PokemonCenter_1F, SandgemTown_PokemonCenter_2F **[skeleton]**, SandgemTown_Mart |
| gMapGroup_IndoorJubilife | 10 | JubilifeCity_TrainerSchool, JubilifeCity_TowerA_1F, JubilifeCity_TowerA_2F, JubilifeCity_TowerB_1F **[skeleton]**, JubilifeCity_TowerB_2F **[skeleton]**, JubilifeCity_TowerC_1F, JubilifeCity_TowerC_2F, JubilifeCity_PokemonCenter_1F, JubilifeCity_PokemonCenter_2F **[skeleton]**, JubilifeCity_Mart |
| gMapGroup_IndoorOreburgh | 13 | OreburghCity_TowerA_1F, OreburghCity_TowerA_2F, OreburghCity_TowerB_1F, OreburghCity_TowerB_2F, OreburghCity_TowerC_1F, OreburghCity_TowerC_2F, OreburghCity_HouseA, OreburghCity_HouseB, OreburghCity_HouseC, OreburghCity_PokemonCenter_1F, OreburghCity_PokemonCenter_2F **[skeleton]**, OreburghCity_Mart, OreburghCity_Gym |
| gMapGroup_IndoorFloaroma | 5 | FloaromaTown_HouseA, FloaromaTown_HouseB, FloaromaTown_PokemonCenter_1F, FloaromaTown_PokemonCenter_2F **[skeleton]**, FloaromaTown_Mart |
| gMapGroup_IndoorEterna | 14 | EternaCity_CycleShop **[skeleton]**, EternaCity_Galactic_1F **[skeleton]**, EternaCity_Galactic_2F **[skeleton]**, EternaCity_Galactic_3F **[skeleton]**, EternaCity_Galactic_4F **[skeleton]**, EternaCity_TowerA_1F, EternaCity_TowerA_2F, EternaCity_TowerA_3F, EternaCity_HouseA, EternaCity_HouseB **[skeleton]**, EternaCity_PokemonCenter_1F, EternaCity_PokemonCenter_2F **[skeleton]**, EternaCity_Mart, EternaCity_Gym |
| gMapGroup_IndoorMiscellaneous | 3 | FloaromaMeadow_House, ValleyWindworksIndoor, Route205South1_House |

## Minimal script candidates (21)

- RavagedPath (34 bytes)
- MtCoronet_N1F1 (37 bytes)
- Gate_Jubilife (36 bytes)
- Gate_Cycling_North (41 bytes)
- Gate_Cycling_South (41 bytes)
- EternaForestOuter1 (41 bytes)
- EternaForestOuter2 (41 bytes)
- SandgemTown_HelperHouse_2F (49 bytes)
- SandgemTown_PokemonCenter_2F (51 bytes)
- JubilifeCity_TowerB_1F (45 bytes)
- JubilifeCity_TowerB_2F (45 bytes)
- JubilifeCity_PokemonCenter_2F (52 bytes)
- OreburghCity_PokemonCenter_2F (52 bytes)
- FloaromaTown_PokemonCenter_2F (52 bytes)
- EternaCity_CycleShop (43 bytes)
- EternaCity_Galactic_1F (45 bytes)
- EternaCity_Galactic_2F (45 bytes)
- EternaCity_Galactic_3F (45 bytes)
- EternaCity_Galactic_4F (45 bytes)
- EternaCity_HouseB (40 bytes)
- EternaCity_PokemonCenter_2F (50 bytes)

**Known blocking gap:** EternaCity_Galactic_1F to _4F have map definitions, but nearly empty scripts. Passive house/PC/gate scripts may legitimately rely on common engine behavior and must be classified, not automatically rewritten.

## Source-only primary tileset directories (23)

- cave_coronet_north
- cave_coronet_south
- cave_oreburgh
- gym_eterna
- gym_oreburgh
- indoor_center
- indoor_cycle_shop
- indoor_forest
- indoor_galactic_eterna
- indoor_galactic_space
- indoor_jubilife
- indoor_mart
- indoor_sandgem_lab
- indoor_sunyshore
- indoor_twinleaf
- indoor_twinleaf_rival
- indoor_windworks
- outdoor_eterna
- outdoor_floaroma
- outdoor_jubilife
- outdoor_oreburgh
- outdoor_route_204
- outdoor_route_205

## Source-only secondary tileset directories (44)

- common_center_1f
- common_center_2f
- common_mart
- cycle_shop
- distortion_world
- eterna_gym
- eterna_indoor
- eterna_north
- eterna_south
- floaroma
- floaroma_meadow
- galactic_eterna_indoor
- galactic_space_indoor
- gatehouse
- jubilife
- jubilife_school
- jubilife_tower
- lake
- oreburgh_gym
- oreburgh_mine
- oreburgh_north
- oreburgh_south
- ravaged_path
- route_201
- route_203
- route_204_north
- route_204_south
- route_205_north
- route_205_south_1
- route_205_south_2
- route_207
- route_211_west
- sandgem
- sandgem_helper
- sandgem_lab
- sandgem_living
- sunyshore_house
- twinleaf
- twinleaf_house
- twinleaf_living
- twinleaf_player
- twinleaf_rival
- valley_windworks
- valley_windworks_indoor

Later-only art, including the distortion_world secondary directory, is a reusable *candidate*, not evidence that a Distortion World map or story exists. Avoid linking dormant content.

## Sinnoh wild encounter records (60 across 20 maps)

- MAP_ETERNA_FOREST_1: gEternaForest1_Morning, gEternaForest1_Day, gEternaForest1_Night
- MAP_ETERNA_FOREST_2: gEternaForest2_Morning, gEternaForest2_Day, gEternaForest2_Night
- MAP_FLOAROMA_MEADOW: gFloaromaMeadow_Morning, gFloaromaMeadow_Day, gFloaromaMeadow_Night
- MAP_LAKE_VERITY: gLakeVerity_Morning, gLakeVerity_Day, gLakeVerity_Night
- MAP_MT_CORONET_N1F1: gMtCoronet_N1F1_Morning, gMtCoronet_N1F1_Day, gMtCoronet_N1F1_Night
- MAP_OREBURGH_GATE: gOreburghGate_Morning, gOreburghGate_Day, gOreburghGate_Night
- MAP_OREBURGH_MINE_B1F: gOreburghMine_B1F_Morning, gOreburghMine_B1F_Day, gOreburghMine_B1F_Night
- MAP_OREBURGH_MINE_B2F: gOreburghMine_B2F_Morning, gOreburghMine_B2F_Day, gOreburghMine_B2F_Night
- MAP_RAVAGED_PATH: gRavagedPath_Morning, gRavagedPath_Day, gRavagedPath_Night
- MAP_ROUTE201: gRoute201_Morning, gRoute201_Day, gRoute201_Night
- MAP_ROUTE202: gRoute202_Morning, gRoute202_Day, gRoute202_Night
- MAP_ROUTE203: gRoute203_Morning, gRoute203_Day, gRoute203_Night
- MAP_ROUTE204_NORTH: gRoute204North_Day, gRoute204North_Morning, gRoute204North_Night
- MAP_ROUTE204_SOUTH: gRoute204_Morning, gRoute204_Day, gRoute204_Night
- MAP_ROUTE205_NORTH: gRoute205North_Morning, gRoute205North_Day, gRoute205North_Night
- MAP_ROUTE205_SOUTH1: gRoute205South1_Day, gRoute205South1_Morning, gRoute205South1_Night
- MAP_ROUTE205_SOUTH2: gRoute205South2_Day, gRoute205South2_Morning, gRoute205South2_Night
- MAP_ROUTE207: gRoute207_Morning, gRoute207_Day, gRoute207_Night
- MAP_ROUTE211_WEST: gRoute211West_Morning, gRoute211West_Day, gRoute211West_Night
- MAP_VALLEY_WINDWORKS: gValleyWindworks_Morning, gValleyWindworks_Day, gValleyWindworks_Night

These records do not include every scripted/static encounter, gift, trade, Honey Tree or missing route. Separate coverage and RTC behavior review is required.

## All source-only MIDI candidate names (191)

Filename inventory only. Select the tracks actually required by map music, encounters, story scripts and cutscenes; import transitively required soundbanks, samples, voicegroups and ID tables, then verify playback.

- mus_dp_amity_square.mid
- mus_dp_azure_flute.mid
- mus_dp_b_tower.mid
- mus_dp_canalave_day.mid
- mus_dp_canalave_night.mid
- mus_dp_catastrophe.mid
- mus_dp_caught_intro.mid
- mus_dp_contest.mid
- mus_dp_contest_dress_up.mid
- mus_dp_contest_lobby.mid
- mus_dp_contest_results.mid
- mus_dp_contest_winner.mid
- mus_dp_credits.mid
- mus_dp_cycling.mid
- mus_dp_dance_difficult.mid
- mus_dp_dance_easy.mid
- mus_dp_dawn.mid
- mus_dp_dex_rating.mid
- mus_dp_encounter_ace_trainer.mid
- mus_dp_encounter_artist.mid
- mus_dp_encounter_boy.mid
- mus_dp_encounter_champion.mid
- mus_dp_encounter_cyclist.mid
- mus_dp_encounter_elite_four.mid
- mus_dp_encounter_galactic.mid
- mus_dp_encounter_girl.mid
- mus_dp_encounter_hiker.mid
- mus_dp_encounter_intense.mid
- mus_dp_encounter_lady.mid
- mus_dp_encounter_rich.mid
- mus_dp_encounter_sailor.mid
- mus_dp_encounter_suspicious.mid
- mus_dp_encounter_twins.mid
- mus_dp_eterna_day.mid
- mus_dp_eterna_forest.mid
- mus_dp_eterna_night.mid
- mus_dp_evolution.mid
- mus_dp_evolved.mid
- mus_dp_fight_area_day.mid
- mus_dp_fight_area_night.mid
- mus_dp_flag_captured.mid
- mus_dp_floaroma_day.mid
- mus_dp_floaroma_night.mid
- mus_dp_follow_me.mid
- mus_dp_galactic_eterna_building.mid
- mus_dp_galactic_hq.mid
- mus_dp_galactic_hq_basement.mid
- mus_dp_game_corner.mid
- mus_dp_great_marsh.mid
- mus_dp_gts.mid
- mus_dp_gym.mid
- mus_dp_hall_of_fame.mid
- mus_dp_hall_of_fame_room.mid
- mus_dp_hall_of_origin.mid
- mus_dp_heal.mid
- mus_dp_hearthome_day.mid
- mus_dp_hearthome_night.mid
- mus_dp_inside_pokemon_league.mid
- mus_dp_intro.mid
- mus_dp_jubilife_day.mid
- mus_dp_jubilife_night.mid
- mus_dp_lake.mid
- mus_dp_lake_caverns.mid
- mus_dp_lake_event.mid
- mus_dp_legend_appears.mid
- mus_dp_lets_go_together.mid
- mus_dp_level_up.mid
- mus_dp_lucas.mid
- mus_dp_move_deleted.mid
- mus_dp_mt_coronet.mid
- mus_dp_mystery_gift.mid
- mus_dp_obtain_accessory.mid
- mus_dp_obtain_badge.mid
- mus_dp_obtain_berry.mid
- mus_dp_obtain_item.mid
- mus_dp_obtain_key_item.mid
- mus_dp_obtain_tmhm.mid
- mus_dp_old_chateau.mid
- mus_dp_oreburgh_day.mid
- mus_dp_oreburgh_gate.mid
- mus_dp_oreburgh_mine.mid
- mus_dp_oreburgh_night.mid
- mus_dp_poffins.mid
- mus_dp_poke_center_day.mid
- mus_dp_poke_center_night.mid
- mus_dp_poke_mart.mid
- mus_dp_poke_radar.mid
- mus_dp_pokemon_league_day.mid
- mus_dp_pokemon_league_night.mid
- mus_dp_poketch.mid
- mus_dp_rival.mid
- mus_dp_route201_day.mid
- mus_dp_route201_night.mid
- mus_dp_route203_day.mid
- mus_dp_route203_night.mid
- mus_dp_route205_day.mid
- mus_dp_route205_night.mid
- mus_dp_route206_day.mid
- mus_dp_route206_night.mid
- mus_dp_route209_day.mid
- mus_dp_route209_night.mid
- mus_dp_route210_day.mid
- mus_dp_route210_night.mid
- mus_dp_route216_day.mid
- mus_dp_route216_night.mid
- mus_dp_route225_day.mid
- mus_dp_route225_night.mid
- mus_dp_route228_day.mid
- mus_dp_route228_night.mid
- mus_dp_rowan.mid
- mus_dp_rowan_lab.mid
- mus_dp_sandgem_day.mid
- mus_dp_sandgem_night.mid
- mus_dp_slots_jackpot.mid
- mus_dp_slots_unused.mid
- mus_dp_slots_win.mid
- mus_dp_snowpoint_day.mid
- mus_dp_snowpoint_night.mid
- mus_dp_solaceon_day.mid
- mus_dp_solaceon_night.mid
- mus_dp_spear_pillar.mid
- mus_dp_stark_mountain.mid
- mus_dp_sunyshore_day.mid
- mus_dp_sunyshore_night.mid
- mus_dp_surf.mid
- mus_dp_title.mid
- mus_dp_tv_broadcast.mid
- mus_dp_tv_end.mid
- mus_dp_tv_station.mid
- mus_dp_twinleaf_day.mid
- mus_dp_twinleaf_night.mid
- mus_dp_underground.mid
- mus_dp_valor_lakefront_day.mid
- mus_dp_valor_lakefront_night.mid
- mus_dp_veilstone_day.mid
- mus_dp_veilstone_night.mid
- mus_dp_victory_champion.mid
- mus_dp_victory_elite_four.mid
- mus_dp_victory_galactic.mid
- mus_dp_victory_gym_leader.mid
- mus_dp_victory_road.mid
- mus_dp_victory_trainer.mid
- mus_dp_victory_wild.mid
- mus_dp_vs_arceus.mid
- mus_dp_vs_champion.mid
- mus_dp_vs_dialga_palkia.mid
- mus_dp_vs_elite_four.mid
- mus_dp_vs_galactic.mid
- mus_dp_vs_galactic_boss.mid
- mus_dp_vs_galactic_commander.mid
- mus_dp_vs_gym_leader.mid
- mus_dp_vs_legend.mid
- mus_dp_vs_rival.mid
- mus_dp_vs_trainer.mid
- mus_dp_vs_uxie_mesprit_azelf.mid
- mus_dp_vs_wild.mid
- mus_dp_wfc.mid
- mus_pl_b_arcade.mid
- mus_pl_b_castle.mid
- mus_pl_b_factory.mid
- mus_pl_b_hall.mid
- mus_pl_contest_beauty.mid
- mus_pl_contest_cool.mid
- mus_pl_contest_cute.mid
- mus_pl_contest_smart.mid
- mus_pl_contest_tough.mid
- mus_pl_distortion_world.mid
- mus_pl_fight_area_day.mid
- mus_pl_giratina_appears_1.mid
- mus_pl_giratina_appears_2.mid
- mus_pl_global_terminal.mid
- mus_pl_intro.mid
- mus_pl_lilycove_bossa_nova.mid
- mus_pl_looker.mid
- mus_pl_mystery_gift.mid
- mus_pl_obtain_arcade_points.mid
- mus_pl_obtain_b_points.mid
- mus_pl_obtain_castle_points.mid
- mus_pl_spin_trade.mid
- mus_pl_title.mid
- mus_pl_tv_broadcast.mid
- mus_pl_tv_end.mid
- mus_pl_twinleaf_music_box.mid
- mus_pl_victory_frontier_brain.mid
- mus_pl_vs_frontier_brain.mid
- mus_pl_vs_giratina.mid
- mus_pl_vs_regi.mid
- mus_pl_wifi_minigames.mid
- mus_pl_wifi_parade.mid
- mus_pl_wifi_plaza.mid
- mus_pl_win_minigame.mid

## Dependency and source ledger

- Canonical source: map_groups.json; layouts.json; map.json and scripts.inc for each map; src/data/wild_encounters.json; src/data/trainers.party; data/text/npc_sinnoh.inc; data/text/trainers_sinnoh.inc; data/scripts/honey_tree.inc; map, trainer, object and audio registries.
- Current collision sites: include/constants/regions.h; include/regions.h; src/event_data.c; src/story_progress.c; src/region_switch.c; src/region_map.c; include/global.h; src/load_save.c; existing trainer defeat flag routing and 24-badge world-tour screen.
- Roark/Gardenia upstream scripts use native Hoenn badge flag names. These require Sinnoh-specific translation; never copy their numeric IDs directly.
- Grand Anthology already has Rock Climb/Defog field-move handlers, but Sinnoh unlock and metatile behavior is unverified.
- Honey Tree's upstream helper calls HoneyTreeWildEncounter in src/wild_encounter.c; adapt only the necessary feature rather than overwrite the encounter engine.
- Later Sinnoh geography, full Galactic arc, all eight gyms, League and Distortion World must be authored where absent.
- Upstream root contains CREDITS.md, but no top-level LICENSE in pinned tree. Verify permissions/provenance for assets/text/music before publishing. Public source does not by itself grant redistribution rights.

## Next generated machine-readable manifest (required in I0)
This human inventory is the seed for a deterministic source-to-destination manifest containing source path/blob SHA, destination path/symbol, deps, namespace, license/attribution status and disposition. No source asset or runtime behavior was imported or modified in this planning pass.
