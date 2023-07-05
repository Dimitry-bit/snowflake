#include <cstring>
#include <cstdio>
#include "GL/glew.h"

#include "snowflake.h"
#include "srenderer_internal.h"

static u32 GLGetSizeofType(u32 type);

static u32 ShaderCreate(char* vertexShader, char* fragmentShader);
static u32 ShaderCompile(u32 type, char* source);
static i32 ShaderGetUniformLocation(Shader* shader, const char* uniformName);

RendererContext rContext = { };
static bool isInit;

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool8 GLLogCall(const char* function)
{
    if (u32 error = glGetError()) {
        u32 errorMsgLen = 256;
        char* errorMsg = (char*) alloca(errorMsgLen);
        memset(errorMsg, '\0', errorMsgLen);

        switch (error) {
            case GL_INVALID_ENUM: {
                snprintf(errorMsg, errorMsgLen, "GL_INVALID_ENUM:GLenum argument out of range.");
            }
                break;
            case GL_INVALID_VALUE: {
                snprintf(errorMsg, errorMsgLen, "GL_INVALID_VALUE:Numeric argument out of range.");
            }
                break;
            case GL_INVALID_OPERATION: {
                snprintf(errorMsg, errorMsgLen, "GL_INVALID_OPERATION:Operation illegal in current state.");
            }
                break;
            case GL_STACK_OVERFLOW: {
                snprintf(errorMsg, errorMsgLen, "GL_STACK_OVERFLOW:Function would cause a stack overflow.");
            }
                break;
            case GL_STACK_UNDERFLOW: {
                snprintf(errorMsg, errorMsgLen, "GL_STACK_UNDERFLOW:Function would cause a stack underflow.");
            }
                break;
            case GL_OUT_OF_MEMORY: {
                snprintf(errorMsg, errorMsgLen, "GL_OUT_OF_MEMORY:Not enough memory left to execute function.");
            }
                break;
            default: {
                snprintf(errorMsg, errorMsgLen, "%d", error);
            }
                break;
        }

        LOG_FATAL("[OpenGL Error] (%d): %s", errorMsg, function);
        return false;
    }
    return true;
}

static u32 GLGetSizeofType(u32 type)
{
    switch (type) {
        case GL_BYTE: return sizeof(GLbyte);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        case GL_BOOL: return sizeof(GLboolean);
        case GL_SHORT: return sizeof(GLshort);
        case GL_UNSIGNED_SHORT: return sizeof(GLushort);
        case GL_INT: return sizeof(GLint);
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        case GL_FLOAT: return sizeof(GLfloat);
        case GL_DOUBLE: return sizeof(GLdouble);
    }
    SASSERT_MSG(false, "Unknown GL_TYPE");
    return 0;
}

void RendererStartup(f32 width, f32 height)
{
    SASSERT_MSG(isInit == false, "Renderer is already started");

    // NOTE(Tony): Replace with ShaderLoadFromMemory
    rContext.boundShader = ShaderLoadFromFiles("../resources/shaders/basic_vertex.glsl",
                                               "../resources/shaders/basic_fragment.glsl");
    ShaderBind(&rContext.boundShader);

    RendererCreateViewport(width, height);

    // Initialize default white texture
    TextureDefault();

    rContext.layout = VertexBufferLayoutInit();
    VertexBufferLayoutPushVec2(&rContext.layout, 1);
    VertexBufferLayoutPushVec2(&rContext.layout, 1);

    isInit = true;

    LOG_INFO("Renderer Startup");
}

void RendererShutdown()
{
    SASSERT_MSG(isInit == true, "Renderer is already shutdown");

    ShaderDelete(&rContext.boundShader);
    VertexBufferLayoutDelete(&rContext.layout);

    // NOTE(Tony): Delete default shader
    // NOTE(Tony): Delete default texture

    memset(&rContext, 0, sizeof(RendererContext));

    LOG_INFO("Renderer Shutdown");
}

void RendererCreateViewport(f32 width, f32 height)
{
    rContext.projMatrix = MatrixOrthogonal(0.0f, width, height, 0.0f, 0.0f, 1.0f);
    rContext.viewMatrix = Matrix4Identity();
    GLCall(glViewport(0, 0, width, height));
}

void RendererSetPolygonMode(u32 face, u32 mode)
{
    GLCall(glPolygonMode(face, mode));
}

VertexBuffer VertexBufferInit(const void* data, u32 size)
{
    VertexBuffer result = { };
    GLCall(glGenBuffers(1, &result.rendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, result.rendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

    return result;
}

SAPI VertexBuffer VertexBufferInit(const Vertex* data, u32 count)
{
    VertexBuffer result = { };
    GLCall(glGenBuffers(1, &result.rendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, result.rendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), data, GL_STATIC_DRAW));

    return result;
}

void VertexBufferDelete(VertexBuffer* vb)
{
    SASSERT_MSG(vb, "VertexBuffer can't be null");

    GLCall(glDeleteBuffers(1, &vb->rendererID));

    memset(vb, 0, sizeof(VertexBuffer));
}

void VertexBufferBind(const VertexBuffer* vb)
{
    SASSERT_MSG(vb, "VertexBuffer can't be null");
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb->rendererID));
}

void VertexBufferUnbind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

IndexBuffer IndexBufferInit(const void* data, u32 count)
{
    IndexBuffer result = { };
    result.count = count;

    GLCall(glGenBuffers(1, &result.rendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.rendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), data, GL_STATIC_DRAW));

    return result;
}

void IndexBufferDelete(IndexBuffer* ib)
{
    SASSERT_MSG(ib, "IndexBuffer can't be null");

    GLCall(glDeleteBuffers(1, &ib->rendererID));

    memset(ib, 0, sizeof(IndexBuffer));
}

void IndexBufferBind(const IndexBuffer* ib)
{
    SASSERT_MSG(ib, "IndexBuffer can't be null");
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->rendererID));
}

void IndexBufferUnbind()
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

VertexArray VertexArrayInit()
{
    VertexArray result = { };
    GLCall(glGenVertexArrays(1, &result.rendererID));

    return result;
}

void VertexArrayDelete(VertexArray* va)
{
    SASSERT_MSG(va, "VertexArray can't be null");
    GLCall(glDeleteVertexArrays(1, &va->rendererID));

    memset(va, 0, sizeof(VertexArray));
}

void VertexArrayAddBuffer(const VertexArray* va, const VertexBuffer* vb, const VertexBufferLayout* layout)
{
    SASSERT_MSG(layout, "VertexBufferLayout can't be null");

    VertexArrayBind(va);
    VertexBufferBind(vb);

    u32 offset = 0;
    i32 i = 0;
    for (VertexBufferElement* element = layout->elementsBegin; element != nullptr; element = element->next) {
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element->count, element->type, element->normalized, layout->stride,
                                     (const void*) (uintptr_t) offset));

        offset += element->count * GLGetSizeofType(element->type);
        i++;
    }
}

void VertexArrayBind(const VertexArray* va)
{
    SASSERT_MSG(va, "VertexArray can't be null");
    GLCall(glBindVertexArray(va->rendererID));
}

void VertexArrayUnbind()
{
    GLCall(glBindVertexArray(0));
}

VertexBufferLayout VertexBufferLayoutInit()
{
    VertexBufferLayout result = { };
    return result;
}

void VertexBufferLayoutDelete(VertexBufferLayout* layout)
{
    SASSERT_MSG(layout, "VertexBufferLayout can't be null");

    VertexBufferElement* tmp = nullptr;
    while (layout->elementsBegin) {
        tmp = layout->elementsBegin;
        layout->elementsBegin = layout->elementsBegin->next;
        free(tmp);
    }

    layout->elementsBegin = nullptr;
    layout->elementsEnd = nullptr;
}

void VertexBufferLayoutPushFloat(VertexBufferLayout* layout, u32 count)
{
    SASSERT(layout);

    VertexBufferElement* element = (VertexBufferElement*) calloc(1, sizeof(VertexBufferElement));
    element->type = GL_FLOAT;
    element->count = count;
    element->normalized = GL_FALSE;

    if (layout->elementsEnd) {
        layout->elementsEnd->next = element;
    } else {
        layout->elementsBegin = element;
    }

    layout->elementsEnd = element;
    layout->stride += count * GLGetSizeofType(GL_FLOAT);
}

void VertexBufferLayoutPushUInt(VertexBufferLayout* layout, u32 count)
{
    SASSERT(layout);

    VertexBufferElement* element = (VertexBufferElement*) calloc(1, sizeof(VertexBufferElement));
    element->type = GL_UNSIGNED_INT;
    element->count = count;
    element->normalized = GL_FALSE;

    if (layout->elementsEnd) {
        layout->elementsEnd->next = element;
    } else {
        layout->elementsBegin = element;
    }

    layout->elementsEnd = element;
    layout->stride += count * GLGetSizeofType(GL_UNSIGNED_INT);
}

void VertexBufferLayoutPushUByte(VertexBufferLayout* layout, u32 count)
{
    SASSERT(layout);

    VertexBufferElement* element = (VertexBufferElement*) calloc(1, sizeof(VertexBufferElement));
    element->type = GL_UNSIGNED_BYTE;
    element->count = count;
    element->normalized = GL_FALSE;

    if (layout->elementsEnd) {
        layout->elementsEnd->next = element;
    } else {
        layout->elementsBegin = element;
    }

    layout->elementsEnd = element;
    layout->stride += count * GLGetSizeofType(GL_UNSIGNED_BYTE);
}

void VertexBufferLayoutPushVec2(VertexBufferLayout* layout, u32 count)
{
    SASSERT(layout);

    VertexBufferElement* element = (VertexBufferElement*) calloc(1, sizeof(VertexBufferElement));
    element->type = GL_FLOAT;
    element->count = 2 * count;
    element->normalized = GL_FALSE;

    if (layout->elementsEnd) {
        layout->elementsEnd->next = element;
    } else {
        layout->elementsBegin = element;
    }

    layout->elementsEnd = element;
    layout->stride += 2 * count * GLGetSizeofType(GL_FLOAT);
}

Shader ShaderLoadFromFiles(const char* vsFilePath, const char* fsFilePath)
{
    Shader result = { };

    char* vertexShader = FileLoad(vsFilePath);
    char* fragmentShader = FileLoad(fsFilePath);

    u64 vsFilePathLen = strlen(vsFilePath);
    result.vsFilePath = (char*) malloc(vsFilePathLen + 1);
    memcpy(result.vsFilePath, vsFilePath, vsFilePathLen + 1);

    u64 fsFilePathLen = strlen(fsFilePath);
    result.fsFilePath = (char*) malloc(fsFilePathLen + 1);
    memcpy(result.fsFilePath, fsFilePath, fsFilePathLen + 1);

    result.rendererID = ShaderCreate(vertexShader, fragmentShader);

    FileUnload(vertexShader);
    FileUnload(fragmentShader);

    LOG_DEBUG("Shader(ID:%d): Loaded successfully", result.rendererID);

    return result;
}

void ShaderDelete(Shader* shader)
{
    SASSERT_MSG(shader, "Shader can't be null");

    free(shader->vsFilePath);
    free(shader->fsFilePath);
    GLCall(glDeleteProgram(shader->rendererID));

    LOG_TRACE("Shader(ID:%d): Deleted successfully", shader->rendererID);
    memset(shader, 0, sizeof(Shader));
}

static i32 ShaderGetUniformLocation(Shader* shader, const char* uniformName)
{
    SASSERT_MSG(shader, "Shader can't be null");

    GLCall(i32 location = glGetUniformLocation(shader->rendererID, uniformName));
    if (location == -1) {
        LOG_WARN("Failed to find '%s' uniform location", uniformName);
    }

    return location;
}

void ShaderBind(const Shader* shader)
{
    SASSERT_MSG(shader, "Shader can't be null");
    GLCall(glUseProgram(shader->rendererID));
}

void ShaderUnbind()
{
    GLCall(glUseProgram(0));
}

static u32 ShaderCreate(char* vertexShader, char* fragmentShader)
{
    GLCall(u32 program = glCreateProgram());

    u32 vs = ShaderCompile(GL_VERTEX_SHADER, vertexShader);
    u32 fs = ShaderCompile(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    i32 success;
    GLCall(glGetProgramiv(program, GL_VALIDATE_STATUS, &success));
    if (!success) {
        i32 length;
        GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
        char* msg = (char*) alloca(length * sizeof(char));
        GLCall(glGetProgramInfoLog(program, length, &length, msg));
        LOG_ERROR("[OpenGL Error] Shader Validation Failed '%s' for ", msg);
        GLCall(glDeleteProgram(program));

        return 0;
    }

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

static u32 ShaderCompile(u32 type, char* source)
{
    GLCall(u32 id = glCreateShader(type));
    GLCall(glShaderSource(id, 1, &source, nullptr));
    GLCall(glCompileShader(id));

    i32 success;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
    if (!success) {
        i32 length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* msg = (char*) alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, msg));
        LOG_ERROR("[OpenGL Error] Failed to compile '%s' shader, %s",
                  (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), msg);
        GLCall(glDeleteShader(id));

        return 0;
    }

    LOG_TRACE("%s shader compiled successfully", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"));

    return id;
}

void ShaderSetUniform1f(Shader* shader, const char* uniformName, f32 v)
{
    ShaderBind(shader);
    i32 location = ShaderGetUniformLocation(shader, uniformName);
    if (location != -1) {
        GLCall(glUniform1f(location, v));
    }
}

void ShaderSetUniform2f(Shader* shader, const char* uniformName, f32 v0, f32 v1)
{
    ShaderBind(shader);
    i32 location = ShaderGetUniformLocation(shader, uniformName);
    if (location != -1) {
        GLCall(glUniform2f(location, v0, v1));
    }
}

void ShaderSetUniform2f(Shader* shader, const char* uniformName, Vec2 v)
{
    ShaderBind(shader);
    i32 location = ShaderGetUniformLocation(shader, uniformName);
    if (location != -1) {
        GLCall(glUniform2fv(location, 1, v.f));
    }
}

void ShaderSetUniform3f(Shader* shader, const char* uniformName, f32 v0, f32 v1, f32 v2)
{
    ShaderBind(shader);
    i32 location = ShaderGetUniformLocation(shader, uniformName);
    if (location != -1) {
        GLCall(glUniform3f(location, v0, v1, v2));
    }
}

void ShaderSetUniform3f(Shader* shader, const char* uniformName, Vec3 v)
{
    ShaderBind(shader);
    i32 location = ShaderGetUniformLocation(shader, uniformName);
    if (location != -1) {
        GLCall(glUniform3fv(location, 1, v.f));
    }
}

void ShaderSetUniform4f(Shader* shader, const char* uniformName, f32 v0, f32 v1, f32 v2, f32 v3)
{
    ShaderBind(shader);
    i32 location = ShaderGetUniformLocation(shader, uniformName);
    if (location != -1) {
        GLCall(glUniform4f(location, v0, v1, v2, v3));
    }
}

void ShaderSetUniform4f(Shader* shader, const char* uniformName, Vec4 v)
{
    ShaderBind(shader);
    i32 location = ShaderGetUniformLocation(shader, uniformName);
    if (location != -1) {
        GLCall(glUniform4fv(location, 1, (const f32*) v.f));
    }
}

void ShaderSetUniform1i(Shader* shader, const char* uniformName, i32 v)
{
    ShaderBind(shader);
    i32 location = ShaderGetUniformLocation(shader, uniformName);
    if (location != -1) {
        GLCall(glUniform1i(location, v));
    }
}

void ShaderSetMatrix2(Shader* shader, const char* uniformName, Mat2 mat)
{
    ShaderBind(shader);
    i32 location = ShaderGetUniformLocation(shader, uniformName);
    if (location != -1) {
        GLCall(glUniformMatrix4fv(location, 1, GL_TRUE, (f32*) mat.f));
    }
}

void ShaderSetMatrix3(Shader* shader, const char* uniformName, Mat3 mat)
{
    ShaderBind(shader);
    i32 location = ShaderGetUniformLocation(shader, uniformName);
    if (location != -1) {
        GLCall(glUniformMatrix4fv(location, 1, GL_TRUE, (f32*) mat.f));
    }
}

void ShaderSetMatrix4(Shader* shader, const char* uniformName, Mat4 mat)
{
    ShaderBind(shader);
    i32 location = ShaderGetUniformLocation(shader, uniformName);
    if (location != -1) {
        GLCall(glUniformMatrix4fv(location, 1, GL_TRUE, (f32*) mat.f));
    }
}

Texture2D TextureLoadFromMemory(unsigned char* data, i32 width, i32 height)
{
    Texture2D texture = { };
    texture.width = width;
    texture.height = height;

    GLCall(glGenTextures(1, &texture.rendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture.rendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    return texture;
}

const Texture2D* TextureDefault()
{
    static Texture2D defaultTex = { };

    if (defaultTex.rendererID == 0) {
        unsigned char texData[] = { 255, 255, 255, 255 };
        defaultTex = TextureLoadFromMemory(texData, 1, 1);
    }

    return &defaultTex;
}

void TextureDelete(Texture2D* texture)
{
    SASSERT(texture);

    GLCall(glDeleteTextures(1, &texture->rendererID));
    memset(texture, 0, sizeof(Texture2D));
}

void TextureBind(const Texture2D* texture, i32 slot)
{
    SASSERT(texture);

    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture->rendererID));
}

void TextureUnbind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void RendererDraw(DrawMode mode, const VertexArray* va, const IndexBuffer* ib, const Texture2D* texture,
                  Mat4 modelMatrix)
{
    SASSERT_MSG(isInit, "Renderer is not started")

    VertexArrayBind(va);
    IndexBufferBind(ib);

    Mat4 mvp = rContext.projMatrix * rContext.viewMatrix * modelMatrix;
    ShaderSetMatrix4(&rContext.boundShader, "uMvp", mvp);

    if (texture) {
        TextureBind(texture, 0);
    } else {
        TextureBind(TextureDefault(), 0);
    }

    GLCall(glDrawElements(mode, ib->count, GL_UNSIGNED_INT, nullptr));
}

void RendererDraw(DrawMode mode, const VertexArray* va, u32 count, const Texture2D* texture, Mat4 transformMatrix)
{
    SASSERT_MSG(isInit, "Renderer is not started")

    VertexArrayBind(va);

    Mat4 mvp = rContext.projMatrix * rContext.viewMatrix * transformMatrix;
    ShaderSetMatrix4(&rContext.boundShader, "uMvp", mvp);

    if (texture) {
        TextureBind(texture, 0);
    } else {
        TextureBind(TextureDefault(), 0);
    }

    GLCall(glDrawArrays(mode, 0, count));
}

void RendererDraw(DrawMode mode, const Vertex* vertices, u32 count, const Texture2D* texture, Mat4 transformMatrix)
{
    SASSERT_MSG(isInit, "Renderer is not started")

    VertexArray va = VertexArrayInit();
    VertexArrayBind(&va);
    VertexBuffer vb = VertexBufferInit(vertices, count);
    VertexBufferBind(&vb);

    VertexArrayAddBuffer(&va, &vb, &rContext.layout);

    Mat4 mvp = rContext.projMatrix * rContext.viewMatrix * transformMatrix;
    ShaderSetMatrix4(&rContext.boundShader, "uMvp", mvp);

    if (texture) {
        TextureBind(texture, 0);
    } else {
        TextureBind(TextureDefault(), 0);
    }

    GLCall(glDrawArrays(mode, 0, count));

    VertexBufferDelete(&vb);
    VertexArrayDelete(&va);
}