#include "srenderer.h"
#include "core/sassert.h"
#include "core/smemory.h"
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

void DrawPixel(Vec2 pos, Color color)
{
    Vertex vertices[] = { { pos, Vector2Zero() } };

    Vec4 colorNormalized = ColorNormalize(color);
    ShaderSetUniform4f(&rContext.boundShader, "uColor", colorNormalized);

    RendererDraw(POINTS, vertices, 1, nullptr);
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

void DrawCirclePro(const Mat4* transform, i32 pointCount, Color color)
{
    SASSERT(transform);

    i32 vertexCount = (pointCount + 2);
    i64 verticesSize = vertexCount * sizeof(Vertex);
    Vertex* vertices = (Vertex*) SAlloca(verticesSize);
    SMemZero(vertices, verticesSize);

    vertices[0].position.x = 1.0f;
    vertices[0].position.y = 1.0f;

    const f32 stepAngle = 2 * S_PI32 / (f32) pointCount;
    for (i32 i = 0; i <= pointCount; i++) {
        vertices[i + 1].position.x = 1.0f + Cos((f32) i * stepAngle);
        vertices[i + 1].position.y = 1.0f + Sin((f32) i * stepAngle);
    }

    Vec4 colorNormalized = ColorNormalize(color);
    ShaderSetUniform4f(&rContext.boundShader, "uColor", colorNormalized);

    RendererDraw(TRIANGLE_FAN, vertices, vertexCount, nullptr, *transform);
}

void DrawCirclePro(const CircleShape* circle)
{
    Mat4 transformMatrix = TransformGenerateMatrix(&circle->transform);
    transformMatrix = MatrixScale(transformMatrix, Vec3{ circle->radius, circle->radius, 1.0f });
    DrawCirclePro(&transformMatrix, circle->pointCount, circle->color);
}

void DrawCircle(Vec2 pos, f32 radius, i32 pointCount, Color color)
{
    Transform transform = TransformCreate(Vector3(pos, 0.0f), Vector3Zero(), Vec3{ radius, radius, 1.0f });
    Mat4 transformMatrix = TransformGenerateMatrix(&transform);
    transformMatrix = MatrixScale(transformMatrix, Vec3{ radius, radius, 1.0f });
    DrawCirclePro(&transformMatrix, pointCount, color);
}

void DrawEllipsePro(const Mat4* transform, i32 pointCount, Color color)
{
    SASSERT(transform);

    i32 vertexCount = (pointCount + 2);
    i64 verticesSize = vertexCount * sizeof(Vertex);
    Vertex* vertices = (Vertex*) SAlloca(verticesSize);
    SMemZero(vertices, verticesSize);

    vertices[0].position.x = 1.0f;
    vertices[0].position.y = 1.0f;

    const f32 stepAngle = 2 * S_PI32 / (f32) pointCount;
    for (i32 i = 0; i <= pointCount; i++) {
        vertices[i + 1].position.x = 1.0f + Cos((f32) i * stepAngle);
        vertices[i + 1].position.y = 1.0f + Sin((f32) i * stepAngle);
    }

    Vec4 colorNormalized = ColorNormalize(color);
    ShaderSetUniform4f(&rContext.boundShader, "uColor", colorNormalized);

    RendererDraw(TRIANGLE_FAN, vertices, vertexCount, nullptr, *transform);
}

void DrawEllipsePro(const EllipseShape* ellipse)
{
    Mat4 transformMatrix = TransformGenerateMatrix(&ellipse->transform);
    transformMatrix = MatrixScale(transformMatrix, Vec3{ ellipse->radiusV, ellipse->radiusH, 1.0f });
    DrawEllipsePro(&transformMatrix, ellipse->pointCount, ellipse->color);
}

void DrawEllipse(Vec2 pos, f32 radiusV, f32 radiusH, i32 pointCount, Color color)
{
    Transform transform = TransformCreate(Vector3(pos, 0.0f), Vector3Zero(), Vec3{ radiusV, radiusH });
    Mat4 transformMatrix = TransformGenerateMatrix(&transform);
    transformMatrix = MatrixScale(transformMatrix, Vec3{ radiusV, radiusH, 1.0f });
    DrawEllipsePro(&transformMatrix, pointCount, color);
}

void DrawRingPro(const Mat4* transform, f32 innerRadius, f32 outerRadius, i32 quadCount, Color color)
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
    const f32 normalizedOuterRadius = 1.0f;
    const f32 stepAngle = 2 * S_PI32 / (f32) quadCount;
    f32 angle = 0.0f;
    for (i32 i = 0, quadCounter = 0; quadCounter < quadCount; i += 6, quadCounter++) {
        // First Triangle
        vertices[i].position.x = 1.0f + Sin(angle) * normalizedInnerRadius;
        vertices[i].position.y = 1.0f + Cos(angle) * normalizedInnerRadius;

        vertices[i + 1].position.x = 1.0f + Sin(angle) * normalizedOuterRadius;
        vertices[i + 1].position.y = 1.0f + Cos(angle) * normalizedOuterRadius;

        vertices[i + 2].position.x = 1.0f + Sin(angle + stepAngle) * normalizedInnerRadius;
        vertices[i + 2].position.y = 1.0f + Cos(angle + stepAngle) * normalizedInnerRadius;

        // Second Triangle
        vertices[i + 3].position.x = 1.0f + Sin(angle + stepAngle) * normalizedInnerRadius;
        vertices[i + 3].position.y = 1.0f + Cos(angle + stepAngle) * normalizedInnerRadius;

        vertices[i + 4].position.x = 1.0f + Sin(angle) * normalizedOuterRadius;
        vertices[i + 4].position.y = 1.0f + Cos(angle) * normalizedOuterRadius;

        vertices[i + 5].position.x = 1.0f + Sin(angle + stepAngle) * normalizedOuterRadius;
        vertices[i + 5].position.y = 1.0f + Cos(angle + stepAngle) * normalizedOuterRadius;

        angle += stepAngle;
    }

    Vec4 colorNormalized = ColorNormalize(color);
    ShaderSetUniform4f(&rContext.boundShader, "uColor", colorNormalized);

    RendererDraw(TRIANGLES, vertices, 6 * quadCount, nullptr, *transform);
}

void DrawRingPro(const RingShape* ring)
{
    Mat4 transformMatrix = TransformGenerateMatrix(&ring->transform);
    transformMatrix = MatrixScale(transformMatrix, Vec3{ ring->outerRadius, ring->outerRadius, 1.0f });
    DrawRingPro(&transformMatrix, ring->innerRadius, ring->outerRadius, ring->quadCount, ring->color);
}

void DrawRing(Vec2 pos, f32 innerRadius, f32 outerRadius, i32 quadCount, Color color)
{
    Transform transform = TransformCreate(Vector3(pos, 0.0f), Vector3Zero(), Vec3{ outerRadius, outerRadius, 1.0f });
    Mat4 transformMatrix = TransformGenerateMatrix(&transform);
    transformMatrix = MatrixScale(transformMatrix, Vec3{ outerRadius, outerRadius, 1.0f });
    DrawRingPro(&transformMatrix, innerRadius, outerRadius, quadCount, color);
}

void DrawRectanglePro(const Mat4* transform, Color color)
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

    Vec4 colorNormalized = ColorNormalize(color);
    ShaderSetUniform4f(&rContext.boundShader, "uColor", colorNormalized);

    RendererDraw(TRIANGLES, vertices, 6, nullptr, *transform);
}

void DrawRectanglePro(const RectangleShape* rect)
{
    Mat4 transformMatrix = TransformGenerateMatrix(&rect->transform);
    transformMatrix = MatrixScale(transformMatrix, Vec3{ rect->width, rect->height, 1.0f });
    DrawRectanglePro(&transformMatrix, rect->color);
}

void DrawRectangle(Vec2 pos, Vec2 size, f32 rotation, Color color)
{
    Transform transform = TransformCreate(Vector3(pos, 0.0f), Vec3{ 0.0f, 0.0f, rotation }, Vector3(size, 1.0f));
    Mat4 transformMatrix = TransformGenerateMatrix(&transform);
    transformMatrix = MatrixScale(transformMatrix, Vec3{ size.x, size.y, 1.0f });
    DrawRectanglePro(&transformMatrix, color);
}

void DrawSpritePro(const Texture2D* texture, Rectanglei texRect, const Mat4* transform, Color tint)
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

    Vec4 colorNormalized = ColorNormalize(tint);
    ShaderSetUniform4f(&rContext.boundShader, "uColor", colorNormalized);

    RendererDraw(TRIANGLES, vertices, 6, texture, *transform);
}

void DrawSpritePro(const Sprite* sprite)
{
    Mat4 transformMatrix = TransformGenerateMatrix(&sprite->transform);
    transformMatrix = MatrixScale(transformMatrix, Vec3{ (f32) sprite->textureRect.width,
                                                         (f32) sprite->textureRect.height, 1.0f });
    DrawSpritePro(sprite->texture, sprite->textureRect, &transformMatrix, sprite->tint);
}

void DrawSprite(const SubTexture2D* subTexture, Vec2 pos, f32 rotation, Color tint)
{
    Transform transform = TransformCreate(Vector3(pos, 0.0f),
                                          Vec3{ 0.0f, 0.0f, rotation },
                                          Vec3{ (f32) subTexture->rect.width, (f32) subTexture->rect.height, 1.0f });
    Mat4 transformMatrix = TransformGenerateMatrix(&transform);
    DrawSpritePro(subTexture->texture, subTexture->rect, &transformMatrix, tint);
}

void DrawSprite(const Texture2D* texture, Vec2 pos, f32 rotation, Color tint)
{
    Transform transform = TransformCreate(Vector3(pos, 0.0f),
                                          Vec3{ 0.0f, 0.0f, rotation },
                                          Vec3{ (f32) texture->width, (f32) texture->height, 1.0f });
    Mat4 transformMatrix = TransformGenerateMatrix(&transform);
    Rectanglei texCoord = { 0, 0, texture->width, texture->height };
    DrawSpritePro(texture, texCoord, &transformMatrix, tint);
}