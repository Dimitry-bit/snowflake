#include "shapes.h"
#include "core/sassert.h"
#include "srenderer.h"

Transform TransformCreate(Vec3 pos, Vec3 rotation, Vec3 scale, Vec3 origin)
{
    Transform transform = { };
    transform.position = pos;
    transform.rotation = rotation;
    transform.scale = scale;
    transform.origin = origin;

    return transform;
}

Mat4 TransformGenerateMatrix(const Transform* transform)
{
    SASSERT(transform);

    Mat4 result = Matrix4Identity();
    result = MatrixTranslate(result, transform->position);
    // TODO(Tony): Transform euler angles to rotations;
    result = MatrixRotate(result, transform->rotation.z, Vec3{ 0.0f, 0.0f, 1.0f });
    result = MatrixTranslate(result, -transform->origin);
    result = MatrixScale(result, transform->scale);

    return result;
}

void TransformMove(Transform* transform, Vec3 delta)
{
    SASSERT(transform);
    transform->position += delta;
}

void TransformScale(Transform* transform, Vec3 factor)
{
    SASSERT(transform);

    transform->scale.x *= factor.x;
    transform->scale.y *= factor.y;
    transform->scale.z *= factor.z;
}

CircleShape CircleCreate(Vec2 pos, f32 radius, Color color, i32 pointCount)
{
    CircleShape circle = { };
    circle.transform = TransformCreate(Vector3(pos, 0.0f));
    circle.color = color;
    circle.pointCount = pointCount;
    circle.radius = radius;

    return circle;
}

EllipseShape EllipseCreate(Vec2 pos, f32 radiusV, f32 radiusH, Color color, i32 pointCount)
{
    EllipseShape ellipse = { };
    ellipse.transform = TransformCreate(Vector3(pos, 0.0f));
    ellipse.color = color;
    ellipse.pointCount = pointCount;
    ellipse.radiusH = radiusH;
    ellipse.radiusV = radiusV;

    return ellipse;
}

RingShape RingCreate(Vec2 pos, f32 innerRadius, f32 outerRadius, Color color, i32 quadCount)
{
    RingShape ring = { };
    ring.transform = TransformCreate(Vector3(pos, 0.0f));
    ring.color = color;
    ring.quadCount = quadCount;
    ring.innerRadius = innerRadius;
    ring.outerRadius = outerRadius;

    return ring;
}

RectangleShape RectangleCreate(Vec2 pos, Vec2 size, Color color)
{
    RectangleShape rect = { };
    rect.transform = TransformCreate(Vector3(pos, 0.0f));
    rect.color = color;
    rect.width = size.x;
    rect.height = size.y;

    return rect;
}

Sprite SpriteCreate(Vec2 pos, const Texture2D* texture)
{
    Sprite sprite = { };
    sprite.tint = WHITE;
    sprite.transform = TransformCreate(Vector3(pos, 0.0f));

    if (texture) {
        sprite.texture = texture;
        sprite.textureRect = TextureGetTextureRect(texture);
    }

    return sprite;
}

Sprite SpriteCreate(Vec2 pos, const SubTexture2D* subTexture)
{
    Sprite sprite = { };
    sprite.tint = WHITE;
    sprite.transform = TransformCreate(Vector3(pos, 0.0f));

    if (subTexture) {
        sprite.texture = subTexture->texture;
        sprite.textureRect = subTexture->rect;
    }

    return sprite;
}

void SpriteSetTexture(Sprite* sprite, const Texture2D* texture, bool8 resetRect)
{
    SASSERT(sprite);

    if (texture) {
        sprite->texture = texture;
    }

    if (resetRect) {
        return;
    }

    if (texture) {
        sprite->textureRect = TextureGetTextureRect(texture);
    } else {
        sprite->textureRect = Rectanglei{ };
    }
}

void SpriteSetTexture(Sprite* sprite, SubTexture2D subTexture, bool8 resetRect)
{
    SASSERT(sprite);

    sprite->texture = subTexture.texture;
    if (resetRect) {
        sprite->textureRect = subTexture.rect;
    }
}

void SpriteSetTexRect(Sprite* sprite, Rectanglei rect)
{
    SASSERT(sprite);
    sprite->textureRect = rect;
}

void DrawRingPro(const RingShape* ring)
{
    Mat4 transformMatrix = TransformGenerateMatrix(&ring->transform);
    transformMatrix = MatrixScale(transformMatrix, Vec3{ ring->outerRadius, ring->outerRadius, 1.0f });
    DrawRingPro(transformMatrix, ring->innerRadius, ring->outerRadius, ring->quadCount, ring->color);
}

void DrawRectanglePro(const RectangleShape* rect)
{
    Mat4 transformMatrix = TransformGenerateMatrix(&rect->transform);
    transformMatrix = MatrixScale(transformMatrix, Vec3{ rect->width, rect->height, 1.0f });
    DrawRectanglePro(transformMatrix, rect->color);
}

void DrawCirclePro(const CircleShape* circle)
{
    Mat4 transformMatrix = TransformGenerateMatrix(&circle->transform);
    transformMatrix = MatrixScale(transformMatrix, Vec3{ circle->radius, circle->radius, 1.0f });
    DrawCirclePro(transformMatrix, circle->pointCount, circle->color);
}

void DrawSpritePro(const Sprite* sprite)
{
    Mat4 transformMatrix = TransformGenerateMatrix(&sprite->transform);
    transformMatrix = MatrixScale(transformMatrix, Vec3{ (f32) sprite->textureRect.width,
                                                         (f32) sprite->textureRect.height, 1.0f });
    DrawSpritePro(sprite->texture, sprite->textureRect, transformMatrix, sprite->tint);
}