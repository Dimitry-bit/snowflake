#pragma once

#include "snowflake.h"

#ifndef GLCall
#define GLCall(x)                                                  \
        GLClearError();                                            \
        x;                                                         \
        SASSERT(GLLogCall(#x))
#endif

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

SAPI void GLClearError();
SAPI bool8 GLLogCall(const char* function);

SAPI VertexBuffer VertexBufferInit(const void* data, u32 size);
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

SAPI void DrawLowLevel(const VertexArray* va, const IndexBuffer* ib, const Shader* shader);
SAPI void DrawLowLevel(const VertexArray* va, const Shader* shader, u32 count);