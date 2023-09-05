#pragma once

#include "color.h"
#include "core/defines.h"
#include "srenderer.h"
#include "utils/utils.h"

struct SAPI Font;
struct SAPI Text;

SAPI Font* FontLoadFromFile(const char* filePath, u32 baseSize = 48);
SAPI void FontUnload(Font** font);

SAPI const char* FontGetFamilyName(const Font* font);
SAPI u32 FontGetBaseSize(const Font* font);

SAPI Text* TextCreate(const Font* font, Color color = WHITE);
SAPI void TextDelete(Text** text);

SAPI void TextSetFont(Text* text, const Font* font);
SAPI const Font* TextGetFont(const Text* text);
SAPI void TextSetCharacterSize(Text* text, u32 size);
SAPI u32 TextGetCharacterSize(const Text* text);
SAPI void TextSetString(Text* text, const char* string);
SAPI void TextSetString(Text* text, StringViewer stringViewer);
SAPI const char* TextGetString(const Text* text);
SAPI void TextSetColor(Text* text, Color color);
SAPI Color TextGetColor(Text* text);

SAPI void DrawText(const Text* text, Vec2 pos);