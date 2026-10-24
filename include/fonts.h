#ifndef GUARD_FONTS_H
#define GUARD_FONTS_H

extern const u8 gFontNormalLatinGlyphWidths[];
extern const u8 gFontSmallLatinGlyphWidths[];
extern const u8 gFontShortLatinGlyphWidths[];
extern const u8 gFontShortJapaneseGlyphWidths[];
extern const u8 gFontNarrowLatinGlyphWidths[];
extern const u8 gFontSmallNarrowLatinGlyphWidths[];
extern const u8 gFontNarrowerLatinGlyphWidths[];
extern const u8 gFontSmallNarrowerLatinGlyphWidths[];
extern const u8 gFontShortNarrowLatinGlyphWidths[];
extern const u8 gFontShortNarrowerLatinGlyphWidths[];

extern const u8 gFontOutlinedLatinGlyphWidths[];
extern const u8 gFontOutlinedNarrowLatinGlyphWidths[];
extern const u8 gFontBattleUIElementsLatinGlyphSizes[][2];

// These IDs match LATIN_FONT_VARIANTS in Makefile.
enum PackedLatinFont
{
    FONT_PACK_SMALL_NARROW,
    FONT_PACK_SMALL,
    FONT_PACK_NARROW,
    FONT_PACK_SHORT,
    FONT_PACK_NORMAL,
    FONT_PACK_NARROWER,
    FONT_PACK_SMALL_NARROWER,
    FONT_PACK_SHORT_NARROW,
    FONT_PACK_SHORT_NARROWER,
    FONT_PACK_OUTLINED,
    FONT_PACK_OUTLINED_NARROW,
    FONT_PACK_BATTLE_UI,
    PACKED_LATIN_FONT_COUNT
};
const u16 *GetPackedLatinFontTile(u8 fontIndex, u32 wordOffset);

#endif // GUARD_FONTS_H
