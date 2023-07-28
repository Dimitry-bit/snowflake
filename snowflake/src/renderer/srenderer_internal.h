#pragma once

#include "core/defines.h"
#include "math/smath.h"
#include "texture.h"

#include <GL/glew.h>

#ifndef GLCall
#define GLCall(x)                                                  \
        GLClearError();                                            \
        x;                                                         \
        SASSERT(GLLogCall(#x))
#endif

typedef i32 DrawMode;

enum SAPI DrawModes {
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

struct SAPI Vertex {
    Vec2 position;
    Vec2 texCord;
};

struct SAPI Shader {
    u32 rendererID;
    char* vsFilePath;
    char* fsFilePath;
};

void GLClearError();
bool8 GLLogCall(const char* function);

void RendererStartup(f32 width, f32 height);
void RendererShutdown();
SAPI void RendererCreateViewport(f32 width, f32 height);
SAPI void RendererSetPolygonMode(u32 face, u32 mode);

SAPI VertexBuffer VertexBufferInit(const void* data, u32 size);
SAPI VertexBuffer VertexBufferInit(const Vertex* data, u32 count);
SAPI void VertexBufferDelete(VertexBuffer* vb);
SAPI void VertexBufferBind(VertexBuffer vb);
SAPI void VertexBufferUnbind();

SAPI IndexBuffer IndexBufferInit(const u32* data, u32 count);
SAPI void IndexBufferDelete(IndexBuffer* ib);
SAPI void IndexBufferBind(IndexBuffer ib);
SAPI void IndexBufferUnbind();

SAPI VertexArray VertexArrayInit();
SAPI void VertexArrayDelete(VertexArray* va);
SAPI void VertexArrayBind(VertexArray va);
SAPI void VertexArrayUnbind();
SAPI void VertexArrayAddBuffer(VertexArray va, VertexBuffer vb, const VertexBufferLayout* layout);

SAPI VertexBufferLayout VertexBufferLayoutInit();
SAPI void VertexBufferLayoutDelete(VertexBufferLayout* layout);
SAPI void VertexBufferLayoutPushFloat(VertexBufferLayout* layout, u32 count);
SAPI void VertexBufferLayoutPushUInt(VertexBufferLayout* layout, u32 count);
SAPI void VertexBufferLayoutPushUByte(VertexBufferLayout* layout, u32 count);
SAPI void VertexBufferLayoutPushVec2(VertexBufferLayout* layout, u32 count);

SAPI Shader ShaderLoadFromFiles(const char* vsFilePath, const char* fsFilePath);
SAPI Shader ShaderLoadFromMemory(const char* vsShader, const char* fsShader);
SAPI Shader ShaderLoadDefault();
SAPI void ShaderUnload(Shader* shader);
SAPI void ShaderBind(Shader shader);
SAPI void ShaderUnbind();
SAPI Shader* ShaderGetBound();
SAPI void ShaderSetUniform1f(Shader shader, const char* uniformName, f32 v);
SAPI void ShaderSetUniform2f(Shader shader, const char* uniformName, f32 v0, f32 v1);
SAPI void ShaderSetUniform3f(Shader shader, const char* uniformName, f32 v0, f32 v1, f32 v2);
SAPI void ShaderSetUniform4f(Shader shader, const char* uniformName, f32 v0, f32 v1, f32 v2, f32 v3);
SAPI void ShaderSetUniform1i(Shader shader, const char* uniformName, i32 v);
SAPI void ShaderSetUniformVec2(Shader shader, const char* uniformName, Vec2 v);
SAPI void ShaderSetUniformVec3(Shader shader, const char* uniformName, Vec3 v);
SAPI void ShaderSetUniformVec4(Shader shader, const char* uniformName, Vec4 v);
SAPI void ShaderSetMatrix2(Shader shader, const char* uniformName, Mat2 mat);
SAPI void ShaderSetMatrix3(Shader shader, const char* uniformName, Mat3 mat);
SAPI void ShaderSetMatrix4(Shader shader, const char* uniformName, Mat4 mat);

SAPI void RendererDraw(DrawMode mode, VertexArray va, IndexBuffer ib, Texture2D texture, Mat4 transformMatrix);
SAPI void RendererDraw(DrawMode mode, VertexArray va, u32 count, Texture2D texture, Mat4 transformMatrix);
SAPI void RendererDraw(DrawMode mode, const Vertex* vertices, u32 count, Texture2D texture, Mat4 transformMatrix);