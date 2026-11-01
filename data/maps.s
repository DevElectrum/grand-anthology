#include "constants/global.h"
#include "constants/layouts.h"
#include "constants/map_types.h"
#include "constants/maps.h"
#include "constants/weather.h"
#include "constants/region_map_sections.h"
#include "constants/songs.h"
#include "constants/trainer_hill.h"
	.include "asm/macros.inc"
	.include "constants/constants.inc"

	.section .rodata

#if XGBA_A1
	.include "build/xgba-a1-release/xgba-a1-layouts.inc"
#else
	.include "data/layouts/layouts.inc"
#endif
	.include "data/layouts/layouts_table.inc"
	.include "data/maps/headers.inc"
	.include "data/maps/groups.inc"
	.include "data/maps/connections.inc"
