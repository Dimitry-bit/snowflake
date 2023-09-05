#include "texture.h"
#include "core/logger.h"
#include "core/sassert.h"
#include "core/smemory.h"
#include "simage_loader.h"
#include "srenderer_internal.h"

#include <GL/glew.h>

struct Texture2D {
    u32 rendererID;
    i32 width, height;
    i32 format;
    i32 mipmaps;
    i32 filterMode;
    i32 wrapMode;
};

struct Image {
    i32 width, height;
    i32 format;
    u8* pixels;
};

Texture2D* TextureCreate(i32 width, i32 height, Color color)
{
    Image* image = ImageCreate(width, height, color);
    if (!image) {
        return nullptr;
    }

    Texture2D* texture = TextureLoadFromImage(image);
    if (!texture) {
        return nullptr;
    }

    ImageUnload(&image);

    return texture;
}

Texture2D* TextureLoadFromMemory(const u8* pixels, i32 width, i32 height)
{
    SASSERT_MSG(pixels, "pixels can't be null");
    SASSERT_MSG(width > 0 && height > 0, "invalid texture dimensions");

    Texture2D* texture = (Texture2D*) SMalloc(sizeof(Texture2D), MEMORY_TAG_TEXTURE);

    texture->width = width;
    texture->height = height;
    texture->format = PIXEL_FORMAT_RGBA8;
    texture->mipmaps = 1;

    GLCall(glGenTextures(1, &texture->rendererID));

    TextureSetFilter(texture, TEXTURE_FILTER_TRILINEAR);
    TextureSetWrap(texture, TEXTURE_WRAP_MIRROR_REPEAT);
    TextureGenerateMipmap(texture);

    TextureBind(texture, 0);

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->width, texture->height, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, pixels));
    TextureUnbind();

    return texture;
}

Texture2D* TextureLoadFromFile(const char* filePath)
{
    Texture2D* texture = nullptr;

    i32 width = 0;
    i32 height = 0;
    i32 nrChannels = 0;

    u8* pixels = SImageLoad(filePath, &width, &height, &nrChannels);
    if (pixels) {
        texture = TextureLoadFromMemory(pixels, width, height);
        SImageUnload(pixels);
    }

    if (!texture) {
        LOG_ERROR("Failed to load texture path: %s", filePath);
        return nullptr;
    }

    return texture;
}

Texture2D* TextureLoadFromImage(const Image* image)
{
    SASSERT_MSG(image, "image can't be null");
    return TextureLoadFromMemory(image->pixels, image->width, image->height);
}

void TextureUpdatePixels(Texture2D* texture, const u8* pixels, i32 xOffset, i32 yOffset, u32 width, u32 height)
{
    SASSERT_MSG(texture, "texture can't be null");
    SASSERT_MSG(pixels, "pixels can't be null");
    SASSERT_MSG(xOffset >= 0 && yOffset >= 0, "invalid texture offsets");
    SASSERT_MSG(width > 0 && height > 0, "invalid texture dimensions");

    GLCall(glTextureSubImage2D(texture->rendererID, 0, xOffset, yOffset, width, height,
                               GL_RGBA, GL_UNSIGNED_BYTE, pixels));
}

void TextureUnload(Texture2D** texture)
{
    if (!texture || !(*texture)) {
        return;
    }

    GLCall(glDeleteTextures(1, &(*texture)->rendererID));
    SFree(*texture);
    *texture = nullptr;
}

void TextureBind(const Texture2D* texture, i32 slot)
{
    SASSERT_MSG(texture, "texture can't be null");

    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture->rendererID));
}

void TextureUnbind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Vec2 TextureGetSize(const Texture2D* texture)
{
    SASSERT_MSG(texture, "texture can't be null");

    Vec2 result = { };
    result.x = (f32) texture->width;
    result.y = (f32) texture->height;

    return result;
}

Rectanglei TextureGetTextureRect(const Texture2D* texture)
{
    SASSERT_MSG(texture, "texture can't be null");

    Rectanglei result = { };
    result.width = texture->width;
    result.height = texture->height;

    return result;
}

i32 TextureGetPixelFormat(const Texture2D* texture)
{
    SASSERT_MSG(texture, "texture can't be null");
    return texture->format;
}

void TextureSetFilter(Texture2D* texture, TextureFilter filter)
{
    TextureBind(texture, 0);

    switch (filter) {
        case TEXTURE_FILTER_POINT: {
            if (texture->mipmaps > 1) {
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
            } else {
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            }
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        }
            break;
        case TEXTURE_FILTER_BILINEAR: {
            if (texture->mipmaps > 1) {
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
            } else {
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            }
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        }
            break;
        case TEXTURE_FILTER_TRILINEAR: {
            if (texture->mipmaps > 1) {
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

    texture->filterMode = filter;
}

TextureFilter TextureGetFilter(const Texture2D* texture)
{
    SASSERT_MSG(texture, "texture can't be null");
    return texture->filterMode;
}

void TextureSetWrap(Texture2D* texture, TextureWrap wrap)
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

    texture->wrapMode = wrap;
}

TextureWrap TextureGetWrap(const Texture2D* texture)
{
    SASSERT_MSG(texture, "texture can't be null");
    return texture->wrapMode;
}

i32 TextureGenerateMipmap(Texture2D* texture)
{
    SASSERT_MSG(texture, "texture can't be null");

    TextureBind(texture, 0);

    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    texture->mipmaps = 1 + (i32) Floor(Log((f32) Max(texture->width, texture->height)) / Log(2));
    LOG_INFO("Texture [ID: %d]: Mipmaps generated, total %d", texture->rendererID, texture->mipmaps);

    return texture->mipmaps;
}

i32 TextureGetMipmapLevel(const Texture2D* texture)
{
    SASSERT_MSG(texture, "texture can't be null");
    return texture->mipmaps;
}

Image* ImageCreate(i32 width, i32 height, Color color)
{
    SASSERT_MSG(width > 0 && height > 0, "invalid image dimension");

    Image* image = (Image*) SMalloc(sizeof(Image), MEMORY_TAG_IMAGE);

    image->format = PIXEL_FORMAT_RGBA8;
    image->width = width;
    image->height = height;

    u64 sizeInBytes = width * height * 4 * sizeof(u8);
    u32* pixels = (u32*) SMalloc(sizeInBytes, MEMORY_TAG_IMAGE);

    image->pixels = (u8*) pixels;
    for (i32 i = 0; i < width * height; i++) {
        pixels[i] = (color.r << 0) | (color.g << 8) |
                    (color.b << 16) | (color.a << 24);
    }

    return image;
}

Image* ImageLoadFromMemory(const u8* pixels, i32 width, i32 height)
{
    SASSERT_MSG(pixels, "pixels can't be null");
    SASSERT_MSG(width > 0 && height > 0, "invalid image dimension");

    Image* image = (Image*) SMalloc(sizeof(Image), MEMORY_TAG_IMAGE);

    image->format = PIXEL_FORMAT_RGBA8;
    image->width = width;
    image->height = height;

    u64 sizeInBytes = width * height * 4 * sizeof(u8);
    image->pixels = (u8*) SMalloc(sizeInBytes, MEMORY_TAG_IMAGE);
    SMemCopy(image->pixels, pixels, sizeInBytes);

    return image;
}

Image* ImageLoadFromFile(const char* filePath)
{
    i32 width = 0;
    i32 height = 0;
    i32 nrChannels = 0;

    u8* pixels = SImageLoad(filePath, &width, &height, &nrChannels);
    Image* image = ImageLoadFromMemory(pixels, width, height);
    SImageUnload(pixels);

    if (!image) {
        LOG_ERROR("Failed to load image path: %s", filePath);
    }

    return image;
}

void ImageUnload(Image** image)
{
    if (!image || !(*image)) {
        return;
    }

    SFree((*image)->pixels);
    SFree(*image);
    *image = nullptr;
}

Vec2 ImageGetSize(const Image* image)
{
    SASSERT_MSG(image, "image can't be null");

    Vec2 result = { };
    result.x = (f32) image->width;
    result.y = (f32) image->height;

    return result;
}

i32 ImageGetPixelFormat(const Image* image)
{
    SASSERT_MSG(image, "image can't be null");
    return image->format;
}

u8* ImageGetPixels(const Image* image)
{
    SASSERT_MSG(image, "image can't be null");
    return image->pixels;
}

SubTexture2D SubTexture2DCreate(const Texture2D* texture, Vec2 pos, Vec2 cellSize, Vec2 spriteSize)
{
    SASSERT_MSG(texture, "texture can't be null");

    SubTexture2D subTexture = { };
    subTexture.texture = texture;
    subTexture.rect.left = pos.x * cellSize.x;
    subTexture.rect.top = pos.y * cellSize.y;
    subTexture.rect.width = cellSize.x * spriteSize.x;
    subTexture.rect.height = cellSize.y * spriteSize.y;

    return subTexture;
}