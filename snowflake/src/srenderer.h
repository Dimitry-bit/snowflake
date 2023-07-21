#pragma once

#include "color.h"
#include "defines.h"
#include "smath.h"

struct SAPI Vertex {
    Vec2 position;
    Vec2 texCord;
};

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

struct SAPI Transform {
    Vec2 position;
    Vec2 scale;
    Vec2 origin;
    f32 rotation;
};

struct SAPI CircleShape {
    Transform transform;
    Color color;
    i32 pointCount;
    f32 radius;
};

struct SAPI EllipseShape {
    Transform transform;
    Color color;
    i32 pointCount;
    f32 radiusH;
    f32 radiusV;
};

struct SAPI RingShape {
    Transform transform;
    Color color;
    i32 quadCount;
    f32 innerRadius;
    f32 outerRadius;
};

struct SAPI RectangleShape {
    Transform transform;
    Color color;
    f32 width;
    f32 height;
};

struct SAPI Sprite {
    Transform transform;
    Color tint;
    f32 width;
    f32 height;
    const Texture2D* texture;
    Rectanglei textureRect;
};

// NOTE: Implemented in srenderer_internal.cpp
SAPI Texture2D TextureCreate(i32 width, i32 height, Color color);
SAPI Texture2D TextureLoadFromMemory(u8* pixels, i32 width, i32 height);
SAPI Texture2D TextureLoadFromFile(const char* filePath);
SAPI Texture2D TextureLoadFromImage(const Image* image);
SAPI void TextureDelete(Texture2D* texture);
SAPI void TextureBind(const Texture2D* texture, i32 slot);
SAPI void TextureUnbind();

SAPI Image ImageCreate(i32 width, i32 height, Color color);
SAPI Image ImageLoadFromMemory(u8* pixels, i32 width, i32 height);
SAPI Image ImageLoadFromFile(const char* filePath);
SAPI void ImageDelete(Image* image);
// EndNote

SAPI void ClearBackground(u8 r, u8 g, u8 b, u8 a);
SAPI void ClearBackground(Color color);

SAPI void DrawPixel(f32 posX, f32 posY, Color color);
SAPI void DrawPixel(Vec2 pos, Color color);

SAPI void DrawLine(f32 startPosX, f32 startPosY, f32 endPosX, f32 endPosY, f32 width, Color color);
SAPI void DrawLine(Vec2 startPos, Vec2 endPos, f32 width, Color color);

SAPI void DrawCirclePro(f32 radius, i32 pointCount, const Transform* transform, Color color);
SAPI void DrawCirclePro(const CircleShape* circle);
SAPI void DrawCircle(f32 posX, f32 posY, f32 radius, i32 pointCount, Color color);
SAPI void DrawCircle(Vec2 pos, f32 radius, i32 pointCount, Color color);

SAPI void DrawEllipsePro(f32 radiusH, f32 radiusV, i32 pointCount, const Transform* transform, Color color);
SAPI void DrawEllipsePro(const EllipseShape* ellipse);
SAPI void DrawEllipse(Vec2 pos, f32 radiusH, f32 radiusV, i32 pointCount, Color color);
SAPI void DrawEllipse(f32 posX, f32 posY, f32 radiusH, f32 radiusV, i32 pointCount, Color color);

SAPI void DrawRingPro(f32 innerRadius, f32 outerRadius, i32 quadCount, const Transform* transform, Color color);
SAPI void DrawRingPro(const RingShape* ring);
SAPI void DrawRing(f32 posX, f32 posY, f32 innerRadius, f32 outerRadius, i32 quadCount, Color color);
SAPI void DrawRing(Vec2 pos, f32 innerRadius, f32 outerRadius, i32 quadCount, Color color);

SAPI void DrawRectanglePro(f32 width, f32 height, const Transform* transform, Color color);
SAPI void DrawRectanglePro(const RectangleShape* rect);
SAPI void DrawRectangle(Vec2 pos, Vec2 size, f32 rotation, Color color);
SAPI void DrawRectangle(f32 left, f32 top, f32 width, f32 height, f32 rotation, Color color);

void DrawSprite(const Texture2D* texture, Rectanglei texRect, f32 width, f32 height,
                const Transform* transform, Color tint);
SAPI void DrawSprite(const Sprite* sprite);

SAPI void DrawTriangle(Vec2 v1, Vec2 v2, Vec2 v3, Color color);

SAPI CircleShape CircleCreate(f32 posX, f32 posY, f32 radius, Color color = WHITE, i32 pointCount = 30);
SAPI CircleShape CircleCreate(Vec2 pos, f32 radius, Color color = WHITE, i32 pointCount = 30);

SAPI EllipseShape EllipseCreate(f32 posX, f32 posY, f32 radiusH, f32 radiusV, Color color = WHITE, i32 pointCount = 30);
SAPI EllipseShape EllipseCreate(Vec2 pos, f32 radiusH, f32 radiusV, Color color = WHITE, i32 pointCount = 30);

SAPI RingShape RingCreate(f32 posX, f32 posY, f32 innerRadius, f32 outerRadius, Color color = WHITE,
                          i32 quadCount = 25);
SAPI RingShape RingCreate(Vec2 pos, f32 innerRadius, f32 outerRadius, Color color = WHITE, i32 quadCount = 25);

SAPI RectangleShape RectangleCreate(f32 x, f32 y, f32 width, f32 height, Color color = WHITE);
SAPI RectangleShape RectangleCreate(Vec2 pos, Vec2 size, Color color = WHITE);

SAPI Sprite SpriteCreate(f32 x, f32 y, f32 width, f32 height);
SAPI Sprite SpriteCreate(Vec2 pos, Vec2 size);
SAPI void SpriteSetTexture(Sprite* sprite, const Texture2D* texture);
SAPI void SpriteSetTexture(Sprite* sprite, const SubTexture2D* subTexture);

SAPI SubTexture2D SubTexture2DCreate(const Texture2D* texture, Vec2 pos, Vec2 cellSize, Vec2 spriteSize);