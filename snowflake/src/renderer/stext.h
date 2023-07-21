#pragma once

#include "color.h"
#include "core/defines.h"
#include "srenderer.h"
#include "utils/utils.h"

struct SAPI Glyph {
    u32 width;
    u32 height;
    i32 bearingX;
    i32 bearingY;
    u32 advance;
    Image bitmap;
};

struct SAPI Font {
    u32 baseSize;
    i32 glyphCount;
    Glyph* glyphTable;
    Texture2D texture;
    Rectanglei* texRects;
};

struct SAPI Text {
    const Font* font;
    char* string;
    u32 characterSize;
    Color fillColor;
};

SAPI Font FontLoadFromFile(const char* filePath, u32 baseSize = 48);
SAPI void FontDelete(Font* font);

SAPI void DrawText(const Text* text, Vec2 pos);

SAPI Text TextCreate(const Font* font, Color color = WHITE);
SAPI void TextDelete(Text* text);
SAPI void TextSetCharacterSize(Text* text, u32 size);
SAPI void TextSetString(Text* text, const char* string);
SAPI void TextSetString(Text* text, const StringViewer* stringViewer);