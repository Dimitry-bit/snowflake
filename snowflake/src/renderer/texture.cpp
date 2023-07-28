#include "texture.h"
#include "core/logger.h"
#include "core/sassert.h"
#include "core/smemory.h"
#include "simage_loader.h"
#include "srenderer_internal.h"

#include <GL/glew.h>

Texture2D TextureCreate(i32 width, i32 height, Color color)
{
    Texture2D defaultTex = { };
    Image image = ImageCreate(width, height, color);
    defaultTex = TextureLoadFromImage(&image);
    ImageUnload(&image);

    return defaultTex;
}

Texture2D TextureLoadFromMemory(const u8* pixels, i32 width, i32 height)
{
    Texture2D texture = { };

    if (!pixels) {
        return texture;
    }

    texture.width = width;
    texture.height = height;
    texture.format = PIXEL_FORMAT_RGBA8;
    texture.mipmaps = 1;

    GLCall(glGenTextures(1, &texture.rendererID));

    TextureGenerateMipmap(texture);
    TextureSetFilter(texture, TEXTURE_FILTER_TRILINEAR);
    TextureSetWrap(texture, TEXTURE_WRAP_MIRROR_REPEAT);

    TextureBind(texture, 0);
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, pixels));
    TextureUnbind();

    return texture;
}

Texture2D TextureLoadFromFile(const char* filePath)
{
    i32 width = 0;
    i32 height = 0;
    i32 nrChannels = 0;

    u8* pixels = SImageLoad(filePath, &width, &height, &nrChannels);
    Texture2D texture = TextureLoadFromMemory(pixels, width, height);
    SImageUnload(pixels);

    return texture;
}

Texture2D TextureLoadFromImage(const Image* image)
{
    return TextureLoadFromMemory(image->pixels, image->width, image->height);
}

void TextureUnload(Texture2D* texture)
{
    SASSERT(texture);

    GLCall(glDeleteTextures(1, &texture->rendererID));
    SMemZero(texture, sizeof(Texture2D));
}

void TextureBind(Texture2D texture, i32 slot)
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture.rendererID));
}

void TextureUnbind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void TextureSetFilter(Texture2D texture, TextureFilter filter)
{
    TextureBind(texture, 0);

    switch (filter) {
        case TEXTURE_FILTER_POINT: {
            if (texture.mipmaps > 1) {
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
            } else {
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            }
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        }
            break;
        case TEXTURE_FILTER_BILINEAR: {
            if (texture.mipmaps > 1) {
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
            } else {
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            }
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        }
            break;
        case TEXTURE_FILTER_TRILINEAR: {
            if (texture.mipmaps > 1) {
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
            } else {
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            }
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        }
            break;
        default: SASSERT_MSG(false, "Texture filter not supported!");
            break;
    }
}

void TextureSetWrap(Texture2D texture, TextureWrap wrap)
{
    TextureBind(texture, 0);

    switch (wrap) {
        case TEXTURE_WRAP_REPEAT: {
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        }
            break;
        case TEXTURE_WRAP_CLAMP: {
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        }
            break;
        case TEXTURE_WRAP_MIRROR_REPEAT: {
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
        }
            break;
        case TEXTURE_WRAP_MIRROR_CLAMP: {
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE));
        }
            break;
        default: SASSERT_MSG(false, "Texture wrap mode not supported!");
            break;
    }
}

void TextureGenerateMipmap(Texture2D texture)
{
    TextureBind(texture, 0);

    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    texture.mipmaps = 1 + (i32) Floor(Log((f32) Max(texture.width, texture.height)) / Log(2));

    LOG_INFO("Texture [ID: %d]: Mipmaps generated, total %d", texture.rendererID, texture.mipmaps);
}

Image ImageCreate(i32 width, i32 height, Color color)
{
    Image image = { };
    image.format = PIXEL_FORMAT_RGBA8;

    if (width <= 0 || height <= 0) {
        return image;
    }

    image.width = width;
    image.height = height;

    u64 sizeInBytes = width * height * 4 * sizeof(u8);
    image.pixels = (u8*) SMalloc(sizeInBytes, MEMORY_TAG_IMAGE);

    u32* pixels = (u32*) image.pixels;
    for (i32 i = 0; i < width * height; i++) {
        pixels[i] = (color.r << 0) | (color.g << 8) |
                    (color.b << 16) | (color.a << 24);
    }

    return image;
}

Image ImageLoadFromMemory(const u8* pixels, i32 width, i32 height)
{
    Image image = { };
    image.format = PIXEL_FORMAT_RGBA8;

    if (width <= 0 || height <= 0) {
        return image;
    }

    image.width = width;
    image.height = height;

    u64 sizeInBytes = width * height * 4 * sizeof(u8);
    image.pixels = (u8*) SMalloc(sizeInBytes, MEMORY_TAG_IMAGE);
    if (pixels) {
        SMemCopy(image.pixels, pixels, sizeInBytes);
    }

    return image;
}

Image ImageLoadFromFile(const char* filePath)
{
    i32 width = 0;
    i32 height = 0;
    i32 nrChannels = 0;

    u8* pixels = SImageLoad(filePath, &width, &height, &nrChannels);
    Image image = ImageLoadFromMemory(pixels, width, height);
    SImageUnload(pixels);

    return image;
}

void ImageUnload(Image* image)
{
    SASSERT(image);

    SFree(image->pixels);
    SMemZero(image, sizeof(Image));
}

SubTexture2D SubTexture2DCreate(const Texture2D* texture, Vec2 pos, Vec2 cellSize, Vec2 spriteSize)
{
    SubTexture2D result = { };
    result.texture = texture;

    result.rect.left = pos.x * cellSize.x;
    result.rect.top = pos.y * cellSize.y;
    result.rect.width = cellSize.x * spriteSize.x;
    result.rect.height = cellSize.y * spriteSize.y;

    return result;
}