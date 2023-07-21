#include "texture.h"
#include "sassert.h"
#include "simage_loader.h"
#include "smemory.h"
#include "srenderer_internal.h"

#include <GL/glew.h>

Texture2D TextureCreate(i32 width, i32 height, Color color)
{
    Texture2D defaultTex = { };
    Image image = ImageCreate(width, height, color);
    defaultTex = TextureLoadFromImage(&image);
    ImageDelete(&image);

    return defaultTex;
}

Texture2D TextureLoadFromMemory(u8* pixels, i32 width, i32 height)
{
    Texture2D texture = { };

    if (!pixels) {
        return texture;
    }

    texture.width = width;
    texture.height = height;
    texture.nrChannel = 4;

    GLCall(glGenTextures(1, &texture.rendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture.rendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, pixels));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

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

void TextureDelete(Texture2D* texture)
{
    SASSERT(texture);

    GLCall(glDeleteTextures(1, &texture->rendererID));
    SMemZero(texture, sizeof(Texture2D));
}

void TextureBind(const Texture2D* texture, i32 slot)
{
    SASSERT(texture);

    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture->rendererID));
}

void TextureUnbind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Image ImageCreate(i32 width, i32 height, Color color)
{
    Image image = { };
    image.nrChannel = 4;

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

Image ImageLoadFromMemory(u8* pixels, i32 width, i32 height)
{
    Image image = { };
    image.nrChannel = 4;

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

void ImageDelete(Image* image)
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