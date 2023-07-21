#pragma once

#include "core/defines.h"
#include "math/smath.h"
#include "srenderer.h"
#include "texture.h"

#include <GL/glew.h>

#ifndef GLCall
#define GLCall(x)                                                  \
        GLClearError();                                            \
        x;                                                         \
        SASSERT(GLLogCall(#x))
#endif

enum SAPI DrawMode {
    POINTS = GL_POINTS,
    LINE_STRIP = GL_LINE_STRIP,
    LINE_LOOP = GL_LINE_LOOP,
    LINES = GL_LINES,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN = GL_TRIANGLE_FAN,
    TRIANGLES = GL_TRIANGLES
};

struct SAPI VertexBuffer {
    u32 rendererID;
};

struct SAPI IndexBuffer {
    u32 rendererID;
    u32 count;
};

struct SAPI VertexArray {
    u32 rendererID;
};

struct SAPI VertexBufferElement {
    u32 type;
    u32 count;
    u32 normalized;

    VertexBufferElement* next;
};

struct SAPI VertexBufferLayout {
    VertexBufferElement* elementsBegin;
    VertexBufferElement* elementsEnd;
    u32 stride;
};

struct SAPI Shader {
    u32 rendererID;
    char* vsFilePath;
    char* fsFilePath;
};

struct RendererContext {
    Mat4 projMatrix;
    Mat4 viewMatrix;
    Shader boundShader;
    VertexBufferLayout layout;
};

extern RendererContext rContext;

void GLClearError();
bool8 GLLogCall(const char* function);

void RendererStartup(f32 width, f32 height);
void RendererShutdown();
SAPI void RendererCreateViewport(f32 width, f32 height);
SAPI void RendererSetPolygonMode(u32 face, u32 mode);

SAPI VertexBuffer VertexBufferInit(const void* data, u32 size);
SAPI VertexBuffer VertexBufferInit(const Vertex* data, u32 count);
SAPI void VertexBufferDelete(VertexBuffer* vb);
SAPI void VertexBufferBind(const VertexBuffer* vb);
SAPI void VertexBufferUnbind();

SAPI IndexBuffer IndexBufferInit(const void* data, u32 count);
SAPI void IndexBufferDelete(IndexBuffer* ib);
SAPI void IndexBufferBind(const IndexBuffer* ib);
SAPI void IndexBufferUnbind();

SAPI VertexArray VertexArrayInit();
SAPI void VertexArrayDelete(VertexArray* va);
SAPI void VertexArrayBind(const VertexArray* va);
SAPI void VertexArrayUnbind();
SAPI void VertexArrayAddBuffer(const VertexArray* va, const VertexBuffer* vb, const VertexBufferLayout* layout);

SAPI VertexBufferLayout VertexBufferLayoutInit();
SAPI void VertexBufferLayoutDelete(VertexBufferLayout* layout);
SAPI void VertexBufferLayoutPushFloat(VertexBufferLayout* layout, u32 count);
SAPI void VertexBufferLayoutPushUInt(VertexBufferLayout* layout, u32 count);
SAPI void VertexBufferLayoutPushUByte(VertexBufferLayout* layout, u32 count);
SAPI void VertexBufferLayoutPushVec2(VertexBufferLayout* layout, u32 count);

SAPI Shader ShaderLoadFromFiles(const char* vsFilePath, const char* fsFilePath);
SAPI void ShaderDelete(Shader* shader);
SAPI void ShaderBind(const Shader* shader);
SAPI void ShaderUnbind();
SAPI void ShaderSetUniform1f(Shader* shader, const char* uniformName, f32 v);
SAPI void ShaderSetUniform2f(Shader* shader, const char* uniformName, f32 v0, f32 v1);
SAPI void ShaderSetUniform2f(Shader* shader, const char* uniformName, Vec2 v);
SAPI void ShaderSetUniform3f(Shader* shader, const char* uniformName, f32 v0, f32 v1, f32 v2);
SAPI void ShaderSetUniform3f(Shader* shader, const char* uniformName, Vec3 v);
SAPI void ShaderSetUniform4f(Shader* shader, const char* uniformName, f32 v0, f32 v1, f32 v2, f32 v3);
SAPI void ShaderSetUniform4f(Shader* shader, const char* uniformName, Vec4 v);
SAPI void ShaderSetUniform1i(Shader* shader, const char* uniformName, i32 v);
SAPI void ShaderSetMatrix4(Shader* shader, const char* uniformName, Mat4 mat);

SAPI void RendererDraw(DrawMode mode, const VertexArray* va, const IndexBuffer* ib, const Texture2D* texture,
                       Mat4 modelMatrix = Matrix4Identity());
SAPI void RendererDraw(DrawMode mode, const VertexArray* va, u32 count, const Texture2D* texture,
                       Mat4 transformMatrix = Matrix4Identity());
SAPI void RendererDraw(DrawMode mode, const VertexBuffer* vb, u32 count, const Texture2D* texture,
                       Mat4 transformMatrix = Matrix4Identity());
SAPI void RendererDraw(DrawMode mode, const Vertex* vertices, u32 count, const Texture2D* texture,
                       Mat4 transformMatrix = Matrix4Identity());