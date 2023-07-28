#pragma once

#include "color.h"
#include "core/defines.h"

typedef i32 TextureFilter;
typedef i32 TextureWrap;

enum SAPI PixelFormat {
    PIXEL_FORMAT_RGBA8 = 0,
};

enum SAPI TextureFilters {
    TEXTURE_FILTER_POINT = 0,
    TEXTURE_FILTER_BILINEAR,
    TEXTURE_FILTER_TRILINEAR,
};

enum SAPI TextureWraps {
    TEXTURE_WRAP_REPEAT = 0,
    TEXTURE_WRAP_CLAMP,
    TEXTURE_WRAP_MIRROR_REPEAT,
    TEXTURE_WRAP_MIRROR_CLAMP
};

struct SAPI Texture2D {
    u32 rendererID;
    i32 mipmaps;
    i32 width, height;
    i32 format;
};

struct SAPI Image {
    i32 width, height;
    i32 format;
    u8* pixels;
};

struct SAPI Rectanglef {
    f32 left, top, width, height;
};

struct SAPI Rectanglei {
    i32 left, top, width, height;
};

struct SAPI SubTexture2D {
    const Texture2D* texture;
    Rectanglei rect;
};

SAPI Texture2D TextureCreate(i32 width, i32 height, Color color);
SAPI Texture2D TextureLoadFromMemory(const u8* pixels, i32 width, i32 height);
SAPI Texture2D TextureLoadFromFile(const char* filePath);
SAPI Texture2D TextureLoadFromImage(const Image* image);
SAPI void TextureUnload(Texture2D* texture);
SAPI void TextureBind(Texture2D texture, i32 slot);
SAPI void TextureUnbind();
SAPI void TextureSetFilter(Texture2D texture, TextureFilter filter);
SAPI void TextureSetWrap(Texture2D texture, TextureWrap wrap);
SAPI void TextureGenerateMipmap(Texture2D texture);

SAPI Image ImageCreate(i32 width, i32 height, Color color);
SAPI Image ImageLoadFromMemory(const u8* pixels, i32 width, i32 height);
SAPI Image ImageLoadFromFile(const char* filePath);
SAPI void ImageUnload(Image* image);

SAPI SubTexture2D SubTexture2DCreate(const Texture2D* texture, Vec2 pos, Vec2 cellSize, Vec2 spriteSize);