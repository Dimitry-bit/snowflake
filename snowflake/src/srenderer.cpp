#include "srenderer.h"
#include "sassert.h"
#include "smemory.h"
#include "srenderer_internal.h"

#include <GL/glew.h>

void ClearBackground(u8 r, u8 g, u8 b, u8 a)
{
    f32 outR = (f32) r / 255.0f;
    f32 outG = (f32) g / 255.0f;
    f32 outB = (f32) b / 255.0f;
    f32 outA = (f32) a / 255.0f;

    GLCall(glClearColor(outR, outG, outB, outA));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void ClearBackground(Color color)
{
    ClearBackground(color.r, color.g, color.b, color.a);
}

void DrawPixel(f32 posX, f32 posY, Color color)
{
    Vertex vertices[] = { { Vec2{ posX, posY }, Vector2Zero() } };

    Vec4 colorNormalized = ColorNormalize(color);
    ShaderSetUniform4f(&rContext.boundShader, "uColor", colorNormalized);

    RendererDraw(POINTS, vertices, 1, nullptr);
}

void DrawPixel(Vec2 pos, Color color)
{
    DrawPixel(pos.x, pos.y, color);
}

void DrawLine(f32 startPosX, f32 startPosY, f32 endPosX, f32 endPosY, f32 width, Color color)
{
    DrawLine(Vec2{ startPosX, startPosY }, Vec2{ endPosX, endPosY }, width, color);
}

void DrawLine(Vec2 startPos, Vec2 endPos, f32 width, Color color)
{
    Vertex vertices[] = {
        { startPos, Vec2{ 0.0f, 0.0f } },
        { endPos, Vec2{ 1.0f, 0.0f } }
    };

    Vec4 colorNormalized = ColorNormalize(color);
    ShaderSetUniform4f(&rContext.boundShader, "uColor", colorNormalized);

    GLCall(glLineWidth(width));
    RendererDraw(LINES, vertices, 2, nullptr);
}

void DrawCirclePro(f32 radius, i32 pointCount, const Transform* transform, Color color)
{
    SASSERT(transform);

    i32 vertexCount = (pointCount + 2);
    i64 verticesSize = vertexCount * sizeof(Vertex);
    Vertex* vertices = (Vertex*) SAlloca(verticesSize);
    SMemZero(vertices, verticesSize);

    vertices[0].position.x = 0;
    vertices[0].position.y = 0;

    const f32 stepAngle = 2 * S_PI32 / (f32) pointCount;
    for (i32 i = 0; i <= pointCount; i++) {
        vertices[i + 1].position.x = Cos((f32) i * stepAngle);
        vertices[i + 1].position.y = Sin((f32) i * stepAngle);
    }

    Vec3 originPos = { -(transform->origin.x - radius) * transform->scale.x,
                       -(transform->origin.y - radius) * transform->scale.y, 0.0f };
    Vec3 scale = { transform->scale.x * radius, transform->scale.y * radius, 0.0f };

    Mat4 transformMatrix = Matrix4Identity();
    transformMatrix = MatrixTranslate(transformMatrix, Vector3(transform->position, 0.0f));
    transformMatrix = MatrixRotate(transformMatrix, transform->rotation, Vec3{ 0.0f, 0.0f, 1.0f });
    transformMatrix = MatrixTranslate(transformMatrix, originPos);
    transformMatrix = MatrixScale(transformMatrix, scale);

    Vec4 colorNormalized = ColorNormalize(color);
    ShaderSetUniform4f(&rContext.boundShader, "uColor", colorNormalized);

    RendererDraw(TRIANGLE_FAN, vertices, vertexCount, nullptr, transformMatrix);
}

void DrawCirclePro(const CircleShape* circle)
{
    DrawCirclePro(circle->radius, circle->pointCount, &circle->transform, circle->color);
}

void DrawCircle(f32 posX, f32 posY, f32 radius, i32 pointCount, Color color)
{
    Transform transform = { };
    transform.position = Vec2{ posX, posY };
    transform.scale = Vector2One();

    DrawCirclePro(radius, pointCount, &transform, color);
}

void DrawCircle(Vec2 pos, f32 radius, i32 pointCount, Color color)
{
    Transform transform = { };
    transform.position = pos;
    transform.scale = Vector2One();

    DrawCirclePro(radius, pointCount, &transform, color);
}

void DrawEllipsePro(f32 radiusH, f32 radiusV, i32 pointCount, const Transform* transform, Color color)
{
    SASSERT(transform);

    i32 vertexCount = (pointCount + 2);
    i64 verticesSize = vertexCount * sizeof(Vertex);
    Vertex* vertices = (Vertex*) SAlloca(verticesSize);
    SMemZero(vertices, verticesSize);

    vertices[0].position.x = 0;
    vertices[0].position.y = 0;

    const f32 stepAngle = 2 * S_PI32 / (f32) pointCount;
    for (i32 i = 0; i <= pointCount; i++) {
        vertices[i + 1].position.x = Cos((f32) i * stepAngle);
        vertices[i + 1].position.y = Sin((f32) i * stepAngle);
    }

    Vec3 originPos = { -(transform->origin.x - radiusV) * transform->scale.x,
                       -(transform->origin.y - radiusH) * transform->scale.y, 0.0f };
    Vec3 scale = { transform->scale.x * radiusV, transform->scale.y * radiusH, 0.0f };

    Mat4 transformMatrix = Matrix4Identity();
    transformMatrix = MatrixTranslate(transformMatrix, Vector3(transform->position, 0.0f));
    transformMatrix = MatrixRotate(transformMatrix, transform->rotation, Vec3{ 0.0f, 0.0f, 1.0f });
    transformMatrix = MatrixTranslate(transformMatrix, originPos);
    transformMatrix = MatrixScale(transformMatrix, scale);

    Vec4 colorNormalized = ColorNormalize(color);
    ShaderSetUniform4f(&rContext.boundShader, "uColor", colorNormalized);

    RendererDraw(TRIANGLE_FAN, vertices, vertexCount, nullptr, transformMatrix);
}

void DrawEllipsePro(const EllipseShape* ellipse)
{
    DrawEllipsePro(ellipse->radiusH, ellipse->radiusV, ellipse->pointCount, &ellipse->transform, ellipse->color);
}

void DrawEllipse(Vec2 pos, f32 radiusH, f32 radiusV, i32 pointCount, Color color)
{
    Transform transform = { };
    transform.position = pos;
    transform.scale = Vector2One();

    DrawEllipsePro(radiusH, radiusV, pointCount, &transform, color);
}

void DrawEllipse(f32 posX, f32 posY, f32 radiusH, f32 radiusV, i32 pointCount, Color color)
{
    Transform transform = { };
    transform.position = Vec2{ posX, posY };
    transform.scale = Vector2One();

    DrawEllipsePro(radiusH, radiusV, pointCount, &transform, color);
}

void DrawRingPro(f32 innerRadius, f32 outerRadius, i32 quadCount, const Transform* transform, Color color)
{
    SASSERT(transform);

    i64 verticesSize = 6 * quadCount * sizeof(Vertex);
    Vertex* vertices = (Vertex*) SAlloca(verticesSize);
    SMemZero(vertices, verticesSize);

    if (outerRadius < innerRadius) {
        f32 tmp = outerRadius;
        outerRadius = innerRadius;
        innerRadius = tmp;
    }

    const f32 normalizedInnerRadius = innerRadius / outerRadius;
    const f32 normalizedOuterRadius = 1;
    const f32 stepAngle = 2 * S_PI32 / (f32) quadCount;
    f32 angle = 0.0f;
    for (i32 i = 0, quadCounter = 0; quadCounter < quadCount; i += 6, quadCounter++) {
        // First Triangle
        vertices[i].position.x = Sin(angle) * normalizedInnerRadius;
        vertices[i].position.y = Cos(angle) * normalizedInnerRadius;

        vertices[i + 1].position.x = Sin(angle) * normalizedOuterRadius;
        vertices[i + 1].position.y = Cos(angle) * normalizedOuterRadius;

        vertices[i + 2].position.x = Sin(angle + stepAngle) * normalizedInnerRadius;
        vertices[i + 2].position.y = Cos(angle + stepAngle) * normalizedInnerRadius;

        // Second Triangle
        vertices[i + 3].position.x = Sin(angle + stepAngle) * normalizedInnerRadius;
        vertices[i + 3].position.y = Cos(angle + stepAngle) * normalizedInnerRadius;

        vertices[i + 4].position.x = Sin(angle) * normalizedOuterRadius;
        vertices[i + 4].position.y = Cos(angle) * normalizedOuterRadius;

        vertices[i + 5].position.x = Sin(angle + stepAngle) * normalizedOuterRadius;
        vertices[i + 5].position.y = Cos(angle + stepAngle) * normalizedOuterRadius;

        angle += stepAngle;
    }

    Vec3 originPos = { -(transform->origin.x - outerRadius) * transform->scale.x,
                       -(transform->origin.y - outerRadius) * transform->scale.y, 0.0f };
    Vec3 scale = { transform->scale.x * outerRadius, transform->scale.y * outerRadius, 0.0f };

    Mat4 transformMatrix = Matrix4Identity();
    transformMatrix = MatrixTranslate(transformMatrix, Vector3(transform->position, 0.0f));
    transformMatrix = MatrixRotate(transformMatrix, transform->rotation, Vec3{ 0.0f, 0.0f, 1.0f });
    transformMatrix = MatrixTranslate(transformMatrix, originPos);
    transformMatrix = MatrixScale(transformMatrix, scale);

    Vec4 colorNormalized = ColorNormalize(color);
    ShaderSetUniform4f(&rContext.boundShader, "uColor", colorNormalized);

    RendererDraw(TRIANGLES, vertices, 6 * quadCount, nullptr, transformMatrix);
}

void DrawRingPro(const RingShape* ring)
{
    DrawRingPro(ring->innerRadius, ring->outerRadius, ring->quadCount, &ring->transform, ring->color);
}

void DrawRing(f32 posX, f32 posY, f32 innerRadius, f32 outerRadius, i32 quadCount, Color color)
{
    Transform transform = { };
    transform.position = Vec2{ posX, posY };
    transform.scale = Vector2One();

    DrawRingPro(innerRadius, outerRadius, quadCount, &transform, color);
}

void DrawRing(Vec2 pos, f32 innerRadius, f32 outerRadius, i32 quadCount, Color color)
{
    Transform transform = { };
    transform.position = pos;
    transform.scale = Vector2One();

    DrawRingPro(innerRadius, outerRadius, quadCount, &transform, color);
}

void DrawRectanglePro(f32 width, f32 height, const Transform* transform, Color color)
{
    SASSERT(transform);

    Vertex vertices[] = {
        { Vec2{ 0, 1 }, Vec2{ 0.0f, 1.0f } },
        { Vec2{ 1, 1 }, Vec2{ 1.0f, 1.0f } },
        { Vec2{ 0, 0 }, Vec2{ 0.0f, 0.0f } },
        { Vec2{ 1, 1 }, Vec2{ 1.0f, 1.0f } },
        { Vec2{ 0, 0 }, Vec2{ 0.0f, 0.0f } },
        { Vec2{ 1, 0 }, Vec2{ 1.0f, 0.0f } }
    };

    Vec3 scale = { transform->scale.x * width, transform->scale.y * height, 0.0f };
    Vec3 originPos = { -transform->origin.x * transform->scale.x, -transform->origin.y * transform->scale.y, 0.0f };

    Mat4 transformMatrix = Matrix4Identity();
    transformMatrix = MatrixTranslate(transformMatrix, Vector3(transform->position, 0.0f));
    transformMatrix = MatrixRotate(transformMatrix, transform->rotation, Vec3{ 0.0f, 0.0f, 1.0f });
    transformMatrix = MatrixTranslate(transformMatrix, originPos);
    transformMatrix = MatrixScale(transformMatrix, scale);

    Vec4 colorNormalized = ColorNormalize(color);
    ShaderSetUniform4f(&rContext.boundShader, "uColor", colorNormalized);

    RendererDraw(TRIANGLES, vertices, 6, nullptr, transformMatrix);
}

void DrawRectanglePro(const RectangleShape* rect)
{
    DrawRectanglePro(rect->width, rect->height, &rect->transform, rect->color);
}

void DrawRectangle(Vec2 pos, Vec2 size, f32 rotation, Color color)
{
    Transform transform;
    transform.position = pos;
    transform.scale = Vector2One();
    transform.rotation = rotation;

    DrawRectanglePro(size.x, size.y, &transform, color);
}

void DrawRectangle(f32 left, f32 top, f32 width, f32 height, f32 rotation, Color color)
{
    Transform transform;
    transform.position = Vec2{ left, top };
    transform.scale = Vector2One();
    transform.rotation = rotation;

    DrawRectanglePro(width, height, &transform, color);
}

void DrawSprite(const Texture2D* texture, Rectanglei texRect, f32 width, f32 height,
                const Transform* transform, Color tint)
{
    SASSERT(transform);
    SASSERT(texture);

    f32 texCoordLeft = (f32) texRect.left / (f32) texture->width;
    f32 texCoordRight = (f32) (texRect.left + texRect.width) / (f32) texture->width;

    // NOTE(Tony): texture is vertically flipped
    f32 texCoordBottom = (f32) texRect.top / (f32) texture->height;
    f32 texCoordTop = (f32) (texRect.top + texRect.height) / (f32) texture->height;

    Vertex vertices[] = {
        { Vec2{ 0, 1 }, Vec2{ texCoordLeft, texCoordTop } },
        { Vec2{ 1, 1 }, Vec2{ texCoordRight, texCoordTop } },
        { Vec2{ 0, 0 }, Vec2{ texCoordLeft, texCoordBottom } },
        { Vec2{ 1, 1 }, Vec2{ texCoordRight, texCoordTop } },
        { Vec2{ 0, 0 }, Vec2{ texCoordLeft, texCoordBottom } },
        { Vec2{ 1, 0 }, Vec2{ texCoordRight, texCoordBottom } }
    };

    Vec3 scale = { transform->scale.x * width, transform->scale.y * height, 0.0f };
    Vec3 originPos = { -transform->origin.x * transform->scale.x, -transform->origin.y * transform->scale.y, 0.0f };

    Mat4 transformMatrix = Matrix4Identity();
    transformMatrix = MatrixTranslate(transformMatrix, Vector3(transform->position, 0.0f));
    transformMatrix = MatrixRotate(transformMatrix, transform->rotation, Vec3{ 0.0f, 0.0f, 1.0f });
    transformMatrix = MatrixTranslate(transformMatrix, originPos);
    transformMatrix = MatrixScale(transformMatrix, scale);

    Vec4 colorNormalized = ColorNormalize(tint);
    ShaderSetUniform4f(&rContext.boundShader, "uColor", colorNormalized);

    RendererDraw(TRIANGLES, vertices, 6, texture, transformMatrix);
}

void DrawSprite(const Sprite* sprite)
{
    DrawSprite(sprite->texture, sprite->textureRect, sprite->width, sprite->height, &sprite->transform, sprite->tint);
}

void DrawTriangle(Vec2 v1, Vec2 v2, Vec2 v3, Color color)
{
    Vertex vertices[] = {
        { Vec2{ v1.x, v1.y }, Vector2Zero() },
        { Vec2{ v2.x, v2.y }, Vector2Zero() },
        { Vec2{ v3.x, v3.y }, Vector2Zero() }
    };

    Vec4 colorNormalized = ColorNormalize(color);
    ShaderSetUniform4f(&rContext.boundShader, "uColor", colorNormalized);

    RendererDraw(TRIANGLES, vertices, 3, nullptr);
}

CircleShape CircleCreate(f32 posX, f32 posY, f32 radius, Color color, i32 pointCount)
{
    CircleShape circle = { };
    circle.transform.position.x = posX;
    circle.transform.position.y = posY;
    circle.transform.scale = Vector2One();
    circle.color = color;
    circle.pointCount = pointCount;
    circle.radius = radius;

    return circle;
}

CircleShape CircleCreate(Vec2 pos, f32 radius, Color color, i32 pointCount)
{
    return CircleCreate(pos.x, pos.y, radius, color, pointCount);
}

EllipseShape EllipseCreate(f32 posX, f32 posY, f32 radiusH, f32 radiusV, Color color, i32 pointCount)
{
    EllipseShape ellipse = { };
    ellipse.transform.position.x = posX;
    ellipse.transform.position.y = posY;
    ellipse.transform.scale = Vector2One();
    ellipse.color = color;
    ellipse.pointCount = pointCount;
    ellipse.radiusH = radiusH;
    ellipse.radiusV = radiusV;

    return ellipse;
}

EllipseShape EllipseCreate(Vec2 pos, f32 radiusH, f32 radiusV, Color color, i32 pointCount)
{
    return EllipseCreate(pos.x, pos.y, radiusH, radiusV, color, pointCount);
}

RingShape RingCreate(f32 posX, f32 posY, f32 innerRadius, f32 outerRadius, Color color, i32 quadCount)
{
    RingShape ring = { };
    ring.transform.position.x = posX;
    ring.transform.position.y = posY;
    ring.transform.scale = Vector2One();
    ring.color = color;
    ring.quadCount = quadCount;
    ring.innerRadius = innerRadius;
    ring.outerRadius = outerRadius;

    return ring;
}

RingShape RingCreate(Vec2 pos, f32 innerRadius, f32 outerRadius, Color color, i32 quadCount)
{
    return RingCreate(pos.x, pos.y, innerRadius, outerRadius, color, quadCount);
}

RectangleShape RectangleCreate(f32 x, f32 y, f32 width, f32 height, Color color)
{
    RectangleShape rect = { };
    rect.transform.position.x = x;
    rect.transform.position.y = y;
    rect.transform.scale = Vector2One();
    rect.color = color;
    rect.width = width;
    rect.height = height;

    return rect;
}

RectangleShape RectangleCreate(Vec2 pos, Vec2 size, Color color)
{
    return RectangleCreate(pos.x, pos.y, size.x, size.y, color);
}

Sprite SpriteCreate(f32 x, f32 y, f32 width, f32 height)
{
    Sprite sprite = { };
    sprite.transform.position.x = x;
    sprite.transform.position.y = y;
    sprite.transform.scale = Vector2One();
    sprite.tint = WHITE;
    sprite.width = width;
    sprite.height = height;

    return sprite;
}

Sprite SpriteCreate(Vec2 pos, Vec2 size)
{
    Sprite sprite = { };
    sprite.transform.position = pos;
    sprite.transform.scale = Vector2One();
    sprite.tint = WHITE;
    sprite.width = size.x;
    sprite.height = size.y;

    return sprite;
}

void SpriteSetTexture(Sprite* sprite, const Texture2D* texture)
{
    SASSERT(sprite);

    sprite->texture = texture;
    sprite->textureRect = Rectanglei{ 0, 0, texture->width, texture->height };
}

void SpriteSetTexture(Sprite* sprite, const SubTexture2D* subTexture)
{
    SASSERT(sprite);

    sprite->texture = subTexture->texture;
    sprite->textureRect = subTexture->rect;
}