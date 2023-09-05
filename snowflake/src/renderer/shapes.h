#pragma once

#include "color.h"
#include "core/defines.h"
#include "math/smath.h"
#include "texture.h"

struct SAPI Transform {
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
    Vec3 origin;
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
    const Texture2D* texture;
    Rectanglei textureRect;
};

SAPI Transform TransformCreate(Vec3 pos = Vector3Zero(), Vec3 rotation = Vector3Zero(),
                               Vec3 scale = Vector3One(), Vec3 origin = Vector3Zero());
SAPI Mat4 TransformGenerateMatrix(const Transform* transform);
SAPI void TransformMove(Transform* transform, Vec3 delta);
SAPI void TransformScale(Transform* transform, Vec3 factor);

SAPI CircleShape CircleCreate(Vec2 pos, f32 radius, Color color = WHITE, i32 pointCount = 30);
SAPI EllipseShape EllipseCreate(Vec2 pos, f32 radiusV, f32 radiusH, Color color = WHITE, i32 pointCount = 30);
SAPI RingShape RingCreate(Vec2 pos, f32 innerRadius, f32 outerRadius, Color color = WHITE, i32 quadCount = 25);
SAPI RectangleShape RectangleCreate(Vec2 pos, Vec2 size, Color color = WHITE);

SAPI Sprite SpriteCreate(Vec2 pos, const Texture2D* texture, Rectanglei texRect);
SAPI Sprite SpriteCreate(Vec2 pos, const SubTexture2D* subTexture);
SAPI void SpriteSetTexture(Sprite* sprite, const Texture2D* texture, bool8 resetRect = false);
SAPI void SpriteSetTexture(Sprite* sprite, SubTexture2D subTexture, bool8 resetRect = false);

SAPI void DrawCirclePro(const CircleShape* circle);
SAPI void DrawEllipsePro(const EllipseShape* ellipse);
SAPI void DrawRingPro(const RingShape* ring);
SAPI void DrawRectanglePro(const RectangleShape* rect);
SAPI void DrawSpritePro(const Sprite* sprite);