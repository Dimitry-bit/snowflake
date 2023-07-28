#pragma once

#include "color.h"
#include "core/defines.h"
#include "math/smath.h"
#include "texture.h"

SAPI void ClearBackground(u8 r, u8 g, u8 b, u8 a);
SAPI void ClearBackground(Color color);

SAPI void DrawPixel(Vec2 pos, Color color);
SAPI void DrawLine(Vec2 startPos, Vec2 endPos, f32 width, Color color);
SAPI void DrawTriangle(Vec2 v1, Vec2 v2, Vec2 v3, Color color);

SAPI void DrawCirclePro(Mat4 transformMatrix, i32 pointCount, Color color);
SAPI void DrawCircle(Vec2 pos, f32 radius, i32 pointCount, Color color);

SAPI void DrawEllipsePro(Mat4 transformMatrix, i32 pointCount, Color color);
SAPI void DrawEllipse(Vec2 pos, f32 radiusV, f32 radiusH, i32 pointCount, Color color);

SAPI void DrawRingPro(Mat4 transformMatrix, f32 innerRadius, f32 outerRadius, i32 quadCount, Color color);
SAPI void DrawRing(Vec2 pos, f32 innerRadius, f32 outerRadius, i32 quadCount, Color color);

SAPI void DrawRectanglePro(Mat4 transformMatrix, Color color);
SAPI void DrawRectangle(Vec2 pos, Vec2 size, f32 rotation, Color color);

SAPI void DrawSpritePro(Texture2D texture, Rectanglei texRect, Mat4 transformMatrix, Color tint);
SAPI void DrawSprite(SubTexture2D subTexture, Vec2 pos, f32 rotation, Color tint);
SAPI void DrawSprite(Texture2D texture, Vec2 pos, f32 rotation, Color tint);