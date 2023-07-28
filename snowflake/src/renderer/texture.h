#pragma once

#include "color.h"
#include "core/defines.h"

struct SAPI Texture2D {
    u32 rendererID;
    i32 width, height;
    i32 nrChannel;
};

struct SAPI Image {
    i32 width, height;
    i32 nrChannel;
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
SAPI Texture2D TextureLoadFromMemory(u8* pixels, i32 width, i32 height);
SAPI Texture2D TextureLoadFromFile(const char* filePath);
SAPI Texture2D TextureLoadFromImage(const Image* image);
SAPI void TextureUnload(Texture2D* texture);
SAPI void TextureBind(Texture2D texture, i32 slot);
SAPI void TextureUnbind();

SAPI Image ImageCreate(i32 width, i32 height, Color color);
SAPI Image ImageLoadFromMemory(u8* pixels, i32 width, i32 height);
SAPI Image ImageLoadFromFile(const char* filePath);
SAPI void ImageUnload(Image* image);

SAPI SubTexture2D SubTexture2DCreate(const Texture2D* texture, Vec2 pos, Vec2 cellSize, Vec2 spriteSize);