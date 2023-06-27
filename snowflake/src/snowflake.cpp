#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "snowflake.h"

#define ERROR_STR_WINDOW_INIT "Window is not initialized, Invoke InitWindow()"

static void WindowSizeCallback(GLFWwindow* window, i32 width, i32 height);

static GLFWwindow* windowHandle = nullptr;
static i32 configFlags = 0;

bool8 InitWindow(const char* title, i32 width, i32 height, i32 cFlags)
{
    if (windowHandle) {
        LOG_ERROR("Window is already created");
        return false;
    }

    if (!glfwInit()) {
        LOG_FATAL("Failed to initialize GLFW");
        return false;
    }

    configFlags = cFlags;

    if ((configFlags & FLAG_CONTEXT_OPENGL_CORE_PROFILE) > 0) {
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    if ((configFlags & FLAG_CONTEXT_OPENGL_3) > 0) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    }

    windowHandle = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!windowHandle) {
        LOG_FATAL("Failed to create GLFW windowHandle");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(windowHandle);
    glfwSetWindowSizeCallback(windowHandle, WindowSizeCallback);

    if ((configFlags & FLAG_WINDOW_RESIZEABLE) > 0) {
        glfwSetWindowAttrib(windowHandle, GLFW_RESIZABLE, GLFW_TRUE);
    } else {
        glfwSetWindowAttrib(windowHandle, GLFW_RESIZABLE, GLFW_FALSE);
    }

    if ((configFlags & FLAG_WINDOW_UNDECORATED) > 0) {
        glfwSetWindowAttrib(windowHandle, GLFW_DECORATED, GLFW_FALSE);
    }

    if ((configFlags & FLAG_WINDOW_VSYNC_HINT) > 0) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    LOG_INFO("OpenGL Version: %s", glGetString(GL_VERSION));

    i32 glProfile = 0;
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &glProfile);
    if ((glProfile & GL_CONTEXT_CORE_PROFILE_BIT) > 0) {
        LOG_INFO("OpenGL Profile: Core");
    } else {
        LOG_INFO("OpenGL Profile: Compatibility");
    }

    return true;
}

void CloseWindow()
{
    glfwTerminate();

    windowHandle = nullptr;
    configFlags = 0;
}

bool8 WindowShouldClose()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    return glfwWindowShouldClose(windowHandle);
}

bool8 IsWindowFullscreen()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    return glfwGetWindowMonitor(windowHandle);
}

bool8 IsWindowHidden()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    return !glfwGetWindowAttrib(windowHandle, GLFW_VISIBLE);
}

bool8 IsWindowFocused()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    return glfwGetWindowAttrib(windowHandle, GLFW_FOCUSED);
}

void ToggleFullscreen()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(windowHandle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}

SAPI bool8 IsWindowState(i32 flags)
{
    return ((configFlags & flags) > 0);
}

void MaximizeWindow()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwMaximizeWindow(windowHandle);
}

void MinimizeWindow()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwRestoreWindow(windowHandle);
}

void SetWindowTitle(const char* title)
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwSetWindowTitle(windowHandle, title);
}

void SetWindowPosition(i32 x, i32 y)
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwSetWindowPos(windowHandle, x, y);
}

void SetWindowSize(i32 width, i32 height)
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwSetWindowSize(windowHandle, width, height);
}

SAPI Vec2 GetWindowSize()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);

    i32 width = 0;
    i32 height = 0;
    glfwGetWindowSize(windowHandle, &width, &height);
    Vec2 size = { (f32) width, (f32) height };
    return size;
}

SAPI Vec2 GetWindowPosition()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);

    i32 x = 0;
    i32 y = 0;
    glfwGetWindowPos(windowHandle, &x, &y);
    Vec2 position = { (f32) x, (f32) y };
    return position;
}

i32 GetWindowWidth()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);

    i32 width = 0;
    glfwGetWindowSize(windowHandle, &width, nullptr);
    return width;
}

i32 GetWindowHeight()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);

    i32 height = 0;
    glfwGetWindowSize(windowHandle, nullptr, &height);
    return height;
}

void PollInputEvents()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwPollEvents();
}

void ShowCursor()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void HideCursor()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

bool8 IsCursorHidden()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    return (glfwGetInputMode(windowHandle, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN);
}

void EnableCursor()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void DisableCursor()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

f64 GetTime()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    return glfwGetTime();
}

f32 GetFrameTime()
{
    // TODO(Tony): Implement GetFrameTime() !!
    return 0.0f;
}

i32 GetFPS()
{
    // TODO(Tony): Implement GetFPS() !!
    return 0;
}

void EnableVsync()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwSwapInterval(1);
    configFlags |= FLAG_WINDOW_VSYNC_HINT;
}

void DisableVsync()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwSwapInterval(0);
    configFlags &= ~FLAG_WINDOW_VSYNC_HINT;
}

bool8 IsVsyncEnabled()
{
    return ((configFlags & FLAG_WINDOW_VSYNC_HINT) > 0);
}

static void WindowSizeCallback(GLFWwindow* window, i32 width, i32 height)
{
    // NOTE(Tony): Implement WindowSizeCallback()
}