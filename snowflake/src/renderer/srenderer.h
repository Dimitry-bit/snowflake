#pragma once

#include "color.h"
#include "core/defines.h"
#include "math/smath.h"
#include "shapes.h"
#include "texture.h"

struct SAPI Vertex {
    Vec2 position;
    Vec2 texCord;
};

SAPI void ClearBackground(u8 r, u8 g, u8 b, u8 a);
SAPI void ClearBackground(Color color);

SAPI void DrawPixel(Vec2 pos, Color color);
SAPI void DrawLine(Vec2 startPos, Vec2 endPos, f32 width, Color color);
SAPI void DrawTriangle(Vec2 v1, Vec2 v2, Vec2 v3, Color color);

SAPI void DrawCirclePro(const Mat4* transform, i32 pointCount, Color color);
SAPI void DrawCirclePro(const CircleShape* circle);
SAPI void DrawCircle(Vec2 pos, f32 radius, i32 pointCount, Color color);

SAPI void DrawEllipsePro(const Mat4* transform, i32 pointCount, Color color);
SAPI void DrawEllipsePro(const EllipseShape* ellipse);
SAPI void DrawEllipse(Vec2 pos, f32 radiusV, f32 radiusH, i32 pointCount, Color color);

SAPI void DrawRingPro(const Mat4* transform, f32 innerRadius, f32 outerRadius, i32 quadCount, Color color);
SAPI void DrawRingPro(const RingShape* ring);
SAPI void DrawRing(Vec2 pos, f32 innerRadius, f32 outerRadius, i32 quadCount, Color color);

SAPI void DrawRectanglePro(const Mat4* transform, Color color);
SAPI void DrawRectanglePro(const RectangleShape* rect);
SAPI void DrawRectangle(Vec2 pos, Vec2 size, f32 rotation, Color color);

SAPI void DrawSpritePro(const Texture2D* texture, Rectanglei texRect, const Mat4* transform, Color tint);
SAPI void DrawSpritePro(const Sprite* sprite);
SAPI void DrawSprite(const SubTexture2D* subTexture, Vec2 pos, f32 rotation, Color tint);
SAPI void DrawSprite(const Texture2D* texture, Vec2 pos, f32 rotation, Color tint);