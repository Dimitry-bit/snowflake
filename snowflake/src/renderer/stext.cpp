#include "stext.h"
#include "core/logger.h"
#include "core/sassert.h"
#include "core/smemory.h"
#include "srenderer_internal.h"

#include <ft2build.h>
#include FT_FREETYPE_H

static Texture2D FontGenerateFontAtlas(Glyph* glyphs, Rectanglei** texRects, i32 glyphCount, u32 baseFontSize);
static Glyph FontGetGlyph(FT_Face face, u8 glyphID);

Font FontLoadFromFile(const char* filePath, u32 baseSize)
{
    Font font = { };
    font.baseSize = baseSize;
    font.glyphCount = 128;

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        LOG_ERROR("Could not init FreeType library");
        return font;
    } else {
        LOG_TRACE("FreeType initialized successfully");
    }

    FT_Face face;
    if (FT_New_Face(ft, filePath, 0, &face)) {
        LOG_ERROR("Failed to load font '%s'", filePath);
        return font;
    } else {
        LOG_TRACE("Font '%s' loaded successfully", filePath);
    }
    FT_Set_Pixel_Sizes(face, 0, font.baseSize);

    font.glyphTable = (Glyph*) SMalloc(font.glyphCount * sizeof(Glyph), MEMORY_TAG_FONT);
    for (i32 c = 0; c < font.glyphCount; c++) {
        Glyph glyph = FontGetGlyph(face, (u8) c);
        font.glyphTable[c] = glyph;
    }

    font.texture = FontGenerateFontAtlas(font.glyphTable, &font.texRects, font.glyphCount, font.baseSize);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return font;
}

void FontUnload(Font* font)
{
    TextureUnload(&font->texture);

    for (u8 c = 0; c < font->glyphCount; c++) {
        ImageUnload(&font->glyphTable[c].bitmap);
    }

    SFree(font->glyphTable);
    SFree(font->texRects);
    SMemZero(font, sizeof(Font));
}

static Texture2D FontGenerateFontAtlas(Glyph* glyphs, Rectanglei** texRects, i32 glyphCount, u32 baseFontSize)
{
    SASSERT(glyphs);
    SASSERT(texRects);

    Texture2D texture = { };

    if (glyphCount <= 0) {
        return texture;
    }

    // NOTE(Tony): Generate Squared power of 2 texture
    // TODO(Tony): Pack Texture
    i32 padding = 5;
    u32 squareSize = 1;
    u32 fontArea = glyphCount * (baseFontSize + padding) * (baseFontSize + padding);
    for (u32 i = 0; i < 32 && ((squareSize * squareSize) < fontArea); i++) {
        squareSize <<= 1;
    }
    texture.width = (i32) squareSize;
    texture.height = (i32) squareSize;

    GLCall(glGenTextures(1, &texture.rendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture.rendererID));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height,
                        0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

    (*texRects) = (Rectanglei*) SRealloc(*texRects, glyphCount * sizeof(Rectanglei), MEMORY_TAG_FONT);

    i32 xOffset = 0;
    i32 yOffset = 0;
    for (i32 c = 0; c < glyphCount; c++) {
        Glyph glyph = glyphs[c];

        if (xOffset + glyph.width + padding >= (u32) texture.width) {
            xOffset = 0;
            yOffset += (i32) (baseFontSize + padding);
        }
        SASSERT(xOffset < texture.width);
        SASSERT((i32) (yOffset + baseFontSize + padding) < texture.height);

        (*texRects)[c].width = (i32) glyph.width;
        (*texRects)[c].height = (i32) glyph.height;
        (*texRects)[c].left = xOffset;
        (*texRects)[c].top = yOffset;

        GLCall(glTextureSubImage2D(texture.rendererID, 0, xOffset, yOffset, glyph.width, glyph.height,
                                   GL_RGBA, GL_UNSIGNED_BYTE, glyph.bitmap.pixels));

        xOffset += (i32) (glyph.width + padding);
    }

    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    LOG_TRACE("FontAtlas created w:%d h:%d", texture.width, texture.height);

    return texture;
}

static Glyph FontGetGlyph(FT_Face face, u8 glyphID)
{
    Glyph glyph = { };

    if (FT_Load_Char(face, glyphID, FT_LOAD_RENDER)) {
        LOG_ERROR("Failed to load glyph(%u) , '%s'", (u32) glyphID, face->family_name);
        return glyph;
    }
    FT_Bitmap bitmap = face->glyph->bitmap;

    glyph.width = bitmap.width;
    glyph.height = bitmap.rows;
    glyph.bearingX = face->glyph->bitmap_left;
    glyph.bearingY = face->glyph->bitmap_top;
    glyph.advance = face->glyph->advance.x;
    glyph.bitmap = ImageCreate((i32) glyph.width, (i32) glyph.height, WHITE);

    u8* pixels = bitmap.buffer;
    for (u32 y = 0; y < glyph.height; ++y) {
        for (u32 x = 0; x < glyph.width; ++x) {
            // Fill alpha channel
            const u64 index = x + y * glyph.width;
            glyph.bitmap.pixels[index * 4 + 3] = pixels[x];
        }
        pixels += bitmap.pitch;
    }

    return glyph;
}

void DrawText(Text text, Vec2 pos)
{
    if (!text.font) {
        return;
    }

    VertexBuffer vb = VertexBufferInit((Vertex*) nullptr, 6);

    Texture2D texture = text.font->texture;
    f32 scale = (f32) text.characterSize / (f32) text.font->baseSize;

    Vec4 colorNormalized = ColorNormalize(text.fillColor);
    ShaderSetUniform4f(rContext.boundShader, "uColor", colorNormalized);

    for (const char* s = text.string; *s != '\0'; s++) {
        Glyph glyph = text.font->glyphTable[(u8) *s];
        f32 xPos = pos.x + (f32) glyph.bearingX * scale;
        f32 yPos = pos.y - (f32) glyph.bearingY * scale;

        Rectanglei texRect = text.font->texRects[(u8) *s];
        f32 texCoordLeft = (f32) texRect.left / (f32) texture.width;
        f32 texCoordRight = (f32) (texRect.left + texRect.width) / (f32) texture.width;
        f32 texCoordTop = (f32) texRect.top / (f32) texture.height;
        f32 texCoordBottom = (f32) (texRect.top + texRect.height) / (f32) texture.height;

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

        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

        RendererDraw(TRIANGLES, vb, 6, texture, Matrix4Identity());

        pos.x += (f32) (glyph.advance >> 6) * scale;
    }

    VertexBufferDelete(&vb);
}

Text TextCreate(const Font* font, Color color)
{
    Text text = { };
    text.font = font;
    text.fillColor = color;
    text.characterSize = font->baseSize;

    return text;
}

void TextDelete(Text* text)
{
    SASSERT(text);
    SFree(text->string);
}

void TextSetCharacterSize(Text* text, u32 size)
{
    SASSERT(text);
    text->characterSize = size;
}

void TextSetString(Text* text, const char* string)
{
    SASSERT(text);

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

void TextSetString(Text* text, const StringViewer* stringViewer)
{
    SASSERT(text);
    SASSERT(stringViewer);

    SFree(text->string);
    text->string = nullptr;

    if (!stringViewer->data || stringViewer->length <= 0) {
        return;
    }

    text->string = (char*) SMalloc(stringViewer->length, MEMORY_TAG_STRING);
    SMemCopy(text->string, stringViewer->data, stringViewer->length - 1);
    text->string[stringViewer->length - 1] = '\0';
}