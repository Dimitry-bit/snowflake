#include "stext.h"
#include "core/logger.h"
#include "core/sassert.h"
#include "core/smemory.h"
#include "srenderer_internal.h"

#include <ft2build.h>
#include FT_FREETYPE_H

struct Glyph {
    u32 width;
    u32 height;
    i32 bearingX;
    i32 bearingY;
    u32 advance;
    Image* bitmap;
};

struct Font {
    char* familyName;
    u32 baseSize;
    i32 glyphCount;
    Glyph* glyphTable;
    Texture2D* texture;
    Rectanglei* texRects;
};

struct Text {
    const Font* font;
    char* string;
    u32 characterSize;
    Color fillColor;
};

static Texture2D* FontGenerateFontAtlas(Glyph* glyphs, Rectanglei** texRects, i32 glyphCount, u32 baseFontSize);
static Glyph FontGetGlyph(FT_Face face, u8 glyphID);

Font* FontLoadFromFile(const char* filePath, u32 baseSize)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        LOG_ERROR("Could not init FreeType library");
        return nullptr;
    } else {
        LOG_TRACE("FreeType initialized successfully");
    }

    FT_Face face;
    if (FT_New_Face(ft, filePath, 0, &face)) {
        LOG_ERROR("Failed to load font '%s'", filePath);
        return nullptr;
    } else {
        LOG_TRACE("Font '%s' loaded successfully", filePath);
    }

    Font* font = (Font*) SMalloc(sizeof(Font), MEMORY_TAG_FONT);
    font->baseSize = baseSize;
    font->glyphCount = 128;
    font->glyphTable = (Glyph*) SMalloc(font->glyphCount * sizeof(Glyph), MEMORY_TAG_FONT);

    FT_Set_Pixel_Sizes(face, 0, font->baseSize);

    for (i32 c = 0; c < font->glyphCount; c++) {
        Glyph glyph = FontGetGlyph(face, (u8) c);
        font->glyphTable[c] = glyph;
    }

    font->texture = FontGenerateFontAtlas(font->glyphTable, &font->texRects, font->glyphCount, font->baseSize);
    if (!font->texture) {
        FontUnload(&font);
        LOG_ERROR("Failed to load font path: %s", face->family_name);
        return nullptr;
    }

    u32 len = strlen(face->family_name);
    font->familyName = (char*) SMalloc(len + 1, MEMORY_TAG_STRING);
    SMemCopy(font->familyName, face->family_name, len);
    font->familyName[len] = '\0';

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return font;
}

void FontUnload(Font** font)
{
    if (!font || !(*font)) {
        return;
    }

    u32 glyphCount = (*font)->glyphCount;
    for (u8 c = 0; c < glyphCount; c++) {
        ImageUnload(&(*font)->glyphTable[c].bitmap);
    }

    TextureUnload(&(*font)->texture);
    SFree((*font)->familyName);
    SFree((*font)->glyphTable);
    SFree((*font)->texRects);
    SFree(*font);
    *font = nullptr;
}

const char* FontGetFamilyName(const Font* font)
{
    SASSERT_MSG(font, "font can't be null");
    return font->familyName;
}

u32 FontGetBaseSize(const Font* font)
{
    SASSERT_MSG(font, "font can't be null");
    return font->baseSize;
}

static Texture2D* FontGenerateFontAtlas(Glyph* glyphs, Rectanglei** texRects, i32 glyphCount, u32 baseFontSize)
{
    SASSERT_MSG(glyphs, "glyphs can't be null");
    SASSERT_MSG(texRects, "texRects can't be null");
    SASSERT_MSG(glyphCount > 0, "invalid glyph count");

    // NOTE(Tony): Generate Squared power of 2 texture
    // TODO(Tony): Pack Texture
    i32 padding = 5;
    u32 squareSize = 1;
    u32 fontArea = glyphCount * (baseFontSize + padding) * (baseFontSize + padding);
    for (u32 i = 0; i < 32 && ((squareSize * squareSize) < fontArea); i++) {
        squareSize <<= 1;
    }

    u32 width = squareSize;
    u32 height = squareSize;

    Texture2D* texture = TextureCreate((i32) width, (i32) height, MAGENTA);
    TextureSetWrap(texture, TEXTURE_WRAP_MIRROR_CLAMP);
    TextureSetFilter(texture, TEXTURE_FILTER_TRILINEAR);

    *texRects = (Rectanglei*) SRealloc(*texRects, glyphCount * sizeof(Rectanglei), MEMORY_TAG_FONT);

    i32 xOffset = 0;
    i32 yOffset = 0;
    for (i32 c = 0; c < glyphCount; c++) {
        Glyph glyph = glyphs[c];

        if (xOffset + glyph.width + padding >= width) {
            xOffset = 0;
            yOffset += (i32) (baseFontSize + padding);
        }
        SASSERT(xOffset < width);
        SASSERT(yOffset + baseFontSize + padding < height);

        (*texRects)[c].width = (i32) glyph.width;
        (*texRects)[c].height = (i32) glyph.height;
        (*texRects)[c].left = xOffset;
        (*texRects)[c].top = yOffset;

        if (glyph.bitmap) {
            u8* bitmapPixels = ImageGetPixels(glyph.bitmap);
            TextureUpdatePixels(texture, bitmapPixels, xOffset, yOffset, glyph.width, glyph.height);
        }

        xOffset += (i32) (glyph.width + padding);
    }

    TextureGenerateMipmap(texture);
    LOG_TRACE("FontAtlas created w:%d h:%d", width, height);

    return texture;
}

static Glyph FontGetGlyph(FT_Face face, u8 glyphID)
{
    Glyph glyph = { };

    if (FT_Load_Char(face, glyphID, FT_LOAD_RENDER)) {
        LOG_ERROR("Failed to load glyph(%u), '%s'", (u32) glyphID, face->family_name);
        return glyph;
    }
    FT_Bitmap bitmap = face->glyph->bitmap;

    glyph.width = bitmap.width;
    glyph.height = bitmap.rows;
    glyph.bearingX = face->glyph->bitmap_left;
    glyph.bearingY = face->glyph->bitmap_top;
    glyph.advance = face->glyph->advance.x;

    if (glyph.width && glyph.height) {
        glyph.bitmap = ImageCreate((i32) glyph.width, (i32) glyph.height, WHITE);
    }

    if (!glyph.bitmap) {
        LOG_ERROR("Failed to load glyph(%u) , '%s'", (u32) glyphID, face->family_name);
        return glyph;
    }

    u8* pixels = bitmap.buffer;
    u8* bitmapPixels = ImageGetPixels(glyph.bitmap);
    for (u32 y = 0; y < glyph.height; ++y) {
        for (u32 x = 0; x < glyph.width; ++x) {
            // Fill alpha channel
            const u64 index = x + y * glyph.width;
            bitmapPixels[index * 4 + 3] = pixels[x];
        }
        pixels += bitmap.pitch;
    }

    return glyph;
}

Text* TextCreate(const Font* font, Color color)
{
    Text* text = (Text*) SMalloc(sizeof(Text), MEMORY_TAG_UNKNOWN);

    if (font) {
        text->font = font;
        text->characterSize = font->baseSize;
    }

    text->fillColor = color;

    return text;
}

void TextDelete(Text** text)
{
    if (!text || !(*text)) {
        return;
    }

    SFree((*text)->string);
    SFree(*text);
    *text = nullptr;
}

void TextSetFont(Text* text, const Font* font)
{
    SASSERT_MSG(text, "text can't be null");

    text->font = font;
    if (font) {
        text->characterSize = font->baseSize;
    }
}

const Font* TextGetFont(const Text* text)
{
    SASSERT_MSG(text, "text can't be null");
    return text->font;
}

void TextSetCharacterSize(Text* text, u32 size)
{
    SASSERT_MSG(text, "text can't be null");
    text->characterSize = size;
}

u32 TextGetCharacterSize(const Text* text)
{
    SASSERT_MSG(text, "text can't be null");
    return text->characterSize;
}

void TextSetString(Text* text, const char* string)
{
    SASSERT_MSG(text, "text can't be null");

    SFree(text->string);
    text->string = nullptr;

    if (!string) {
        return;
    }

    u64 size = strlen(string);
    text->string = (char*) SMalloc(size + 1, MEMORY_TAG_STRING);
    SMemCopy(text->string, string, size);
    text->string[size] = '\0';
}

void TextSetString(Text* text, StringViewer stringViewer)
{
    SASSERT_MSG(text, "text can't be null");

    SFree(text->string);
    text->string = nullptr;

    if (!stringViewer.data || stringViewer.length <= 0) {
        return;
    }

    text->string = (char*) SMalloc(stringViewer.length, MEMORY_TAG_STRING);
    SMemCopy(text->string, stringViewer.data, stringViewer.length - 1);
    text->string[stringViewer.length - 1] = '\0';
}

const char* TextGetString(const Text* text)
{
    SASSERT_MSG(text, "text can't be null");
    return text->string;
}

void TextSetColor(Text* text, Color color)
{
    SASSERT_MSG(text, "text can't be null");
    text->fillColor = color;
}

Color TextGetColor(const Text* text)
{
    SASSERT_MSG(text, "text can't be null");
    return text->fillColor;
}

void DrawText(const Text* text, Vec2 pos)
{
    SASSERT_MSG(text, "text can't be null");
    if (!text->font || !text->string) {
        return;
    }
    SASSERT_MSG(text->font->glyphTable && text->font->texture && text->font->texture, "can't render broken font");

    f32 scale = (f32) text->characterSize / (f32) text->font->baseSize;

    Texture2D* texture = text->font->texture;
    Vec2 textureSize = TextureGetSize(texture);

    Vec4 colorNormalized = ColorNormalize(text->fillColor);
    ShaderSetUniformVec4(*ShaderGetBound(), "uColor", colorNormalized);

    for (const char* s = text->string; *s != '\0'; s++) {
        Glyph glyph = text->font->glyphTable[(u8) *s];
        f32 xPos = pos.x + (f32) glyph.bearingX * scale;
        f32 yPos = pos.y - (f32) glyph.bearingY * scale;

        Rectanglei texRect = text->font->texRects[(u8) *s];
        f32 texCoordLeft = (f32) texRect.left / (f32) textureSize.x;
        f32 texCoordRight = (f32) (texRect.left + texRect.width) / (f32) textureSize.x;
        f32 texCoordTop = (f32) texRect.top / (f32) textureSize.y;
        f32 texCoordBottom = (f32) (texRect.top + texRect.height) / (f32) textureSize.y;

        f32 w = (f32) glyph.width * scale;
        f32 h = (f32) glyph.height * scale;

        Vertex vertices[] = {
            { Vec2{ xPos, yPos + h }, Vec2{ texCoordLeft, texCoordBottom } },
            { Vec2{ xPos, yPos }, Vec2{ texCoordLeft, texCoordTop } },
            { Vec2{ xPos + w, yPos }, Vec2{ texCoordRight, texCoordTop } },
            { Vec2{ xPos, yPos + h }, Vec2{ texCoordLeft, texCoordBottom } },
            { Vec2{ xPos + w, yPos }, Vec2{ texCoordRight, texCoordTop } },
            { Vec2{ xPos + w, yPos + h }, Vec2{ texCoordRight, texCoordBottom } }
        };

        RendererDraw(TRIANGLES, vertices, 6, texture, Matrix4Identity());

        pos.x += (f32) (glyph.advance >> 6) * scale;
    }
}