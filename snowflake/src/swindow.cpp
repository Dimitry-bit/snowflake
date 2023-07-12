#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "snowflake.h"
#include "srenderer_internal.h"
#include "smemory.h"

#define ERROR_STR_WINDOW_INIT "Window is not initialized, Invoke InitWindow()"
#define MAX_KEYBOARD_KEYS 512
#define MAX_MOUSE_BUTTONS 8

struct WindowContext {
    GLFWwindow* handle;
    WindowConfig config;
};

struct KeyboardInput {
    bool8 prevKeyState[MAX_KEYBOARD_KEYS];
    bool8 currentKeyState[MAX_KEYBOARD_KEYS];
};

struct MouseInput {
    bool8 prevButtonState[MAX_MOUSE_BUTTONS];
    bool8 currentButtonState[MAX_MOUSE_BUTTONS];
    Vec2 scrollWheel;
};

struct STime {
    f32 currentFrameTime;
    f32 prevFrameTime;
    f32 deltaTime;
};

struct FPS {
    f32 timer;
    u32 frameCounter;
    u32 fps;
};

struct SnowContext {
    WindowContext window;
    KeyboardInput keyboard;
    MouseInput mouse;
    STime time;
    FPS fps;
};

static void WindowSizeCallback(GLFWwindow* window, i32 width, i32 height);
static void KeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods);
static void MouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods);
static void MouseScrollCallback(GLFWwindow* window, f64 xOffset, f64 yOffset);

static SnowContext snowflake = { };
static bool8 isDrawing = false;

bool8 InitWindow(const char* title, i32 width, i32 height, WindowConfig config)
{
    if (snowflake.window.handle) {
        LOG_ERROR("Window is already created");
        return false;
    }

    if (!glfwInit()) {
        LOG_FATAL("Failed to initialize GLFW");
        return false;
    } else {
        LOG_TRACE("GLFW initialized successfully");
    }

    u32 configFlags = config.flags;

    if ((configFlags & FLAG_CONTEXT_OPENGL_CORE_PROFILE) > 0) {
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    if ((configFlags & FLAG_CONTEXT_OPENGL_3) > 0) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    }

    if (config.antialiasingLevel <= 0 || config.antialiasingLevel > 16) {
        LOG_WARN("%d MSAA level not supported, MSAA is default", config.antialiasingLevel);
    } else {
        glfwWindowHint(GLFW_SAMPLES, config.antialiasingLevel);
        LOG_INFO("MSAA: x%d", config.antialiasingLevel);
    }

    snowflake.window.handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    GLFWwindow* windowHandle = snowflake.window.handle;

    if (!windowHandle) {
        LOG_FATAL("Failed to create GLFW handle");
        glfwTerminate();
        return false;
    } else {
        LOG_TRACE("GLFW window created successfully");
    }

    glfwMakeContextCurrent(windowHandle);
    glfwSetWindowSizeCallback(windowHandle, WindowSizeCallback);
    glfwSetKeyCallback(windowHandle, KeyCallback);
    glfwSetMouseButtonCallback(windowHandle, MouseButtonCallback);
    glfwSetScrollCallback(windowHandle, MouseScrollCallback);

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

    if (glewInit() != GLEW_OK) {
        LOG_FATAL("Failed to initialize GLEW");
        CloseWindow();
        return false;
    } else {
        LOG_TRACE("GLEW initialized successfully");
    }

    snowflake.window.config = config;

    MemoryStartup();
    RendererStartup((f32) width, (f32) height);

    LOG_INFO("Snowflake initialized successfully");

    return true;
}

void CloseWindow()
{
    RendererShutdown();

    glfwTerminate();

    SMemZero(&snowflake, sizeof(SnowContext));

    MemoryShutdown();
    LOG_INFO("Snowflake window closed successfully");
}

bool8 WindowShouldClose()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    return glfwWindowShouldClose(snowflake.window.handle);
}

bool8 IsWindowFullscreen()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    return glfwGetWindowMonitor(snowflake.window.handle);
}

bool8 IsWindowHidden()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    return !glfwGetWindowAttrib(snowflake.window.handle, GLFW_VISIBLE);
}

bool8 IsWindowFocused()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    return glfwGetWindowAttrib(snowflake.window.handle, GLFW_FOCUSED);
}

void ToggleFullscreen()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(snowflake.window.handle, monitor, 0, 0,
                         mode->width, mode->height, mode->refreshRate);
}

SAPI bool8 IsWindowState(i32 flags)
{
    return ((snowflake.window.config.flags & flags) > 0);
}

void MaximizeWindow()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    glfwMaximizeWindow(snowflake.window.handle);
}

void MinimizeWindow()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    glfwRestoreWindow(snowflake.window.handle);
}

void SetWindowTitle(const char* title)
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    glfwSetWindowTitle(snowflake.window.handle, title);
}

void SetWindowPosition(i32 x, i32 y)
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    glfwSetWindowPos(snowflake.window.handle, x, y);
}

void SetWindowSize(i32 width, i32 height)
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    glfwSetWindowSize(snowflake.window.handle, width, height);
}

SAPI Vec2 GetWindowSize()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);

    i32 width = 0;
    i32 height = 0;
    glfwGetWindowSize(snowflake.window.handle, &width, &height);
    Vec2 size = { (f32) width, (f32) height };
    return size;
}

SAPI Vec2 GetWindowPosition()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);

    i32 x = 0;
    i32 y = 0;
    glfwGetWindowPos(snowflake.window.handle, &x, &y);
    Vec2 position = { (f32) x, (f32) y };
    return position;
}

i32 GetWindowWidth()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);

    i32 width = 0;
    glfwGetWindowSize(snowflake.window.handle, &width, nullptr);
    return width;
}

i32 GetWindowHeight()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);

    i32 height = 0;
    glfwGetWindowSize(snowflake.window.handle, nullptr, &height);
    return height;
}

void* GetGLFWwindowHandle()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    return snowflake.window.handle;
}

void PollInputEvents()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);

    SMemCopy(snowflake.keyboard.prevKeyState, snowflake.keyboard.currentKeyState, MAX_KEYBOARD_KEYS);
    SMemCopy(snowflake.mouse.prevButtonState, snowflake.mouse.currentButtonState, MAX_MOUSE_BUTTONS);
    snowflake.mouse.scrollWheel = Vector2Zero();

    glfwPollEvents();
}

void ShowCursor()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    glfwSetInputMode(snowflake.window.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    LOG_TRACE("CursorState: Show");
}

void HideCursor()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    glfwSetInputMode(snowflake.window.handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    LOG_TRACE("CursorState: Hidden");
}

bool8 IsCursorHidden()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    return (glfwGetInputMode(snowflake.window.handle, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN);
}

void EnableCursor()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    glfwSetInputMode(snowflake.window.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    LOG_TRACE("CursorState: Normal");
}

void DisableCursor()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    glfwSetInputMode(snowflake.window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    LOG_TRACE("CursorState: Disabled");
}

f64 GetTime()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    return glfwGetTime();
}

f32 GetFrameTime()
{
    return snowflake.time.deltaTime;
}

u32 GetFPS()
{
    return snowflake.fps.fps;
}

void EnableVsync()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    glfwSwapInterval(1);
    snowflake.window.config.flags |= FLAG_WINDOW_VSYNC_HINT;

    LOG_INFO("Vsync: Enabled");
}

void DisableVsync()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    glfwSwapInterval(0);
    snowflake.window.config.flags &= ~FLAG_WINDOW_VSYNC_HINT;

    LOG_INFO("Vsync: Disabled");
}

bool8 IsVsyncEnabled()
{
    return ((snowflake.window.config.flags & FLAG_WINDOW_VSYNC_HINT) > 0);
}

bool8 IsKeyPressed(KeyboardKey key)
{
    return (!snowflake.keyboard.prevKeyState[key] && snowflake.keyboard.currentKeyState[key]);
}

bool8 IsKeyReleased(KeyboardKey key)
{
    return (snowflake.keyboard.prevKeyState[key] && !snowflake.keyboard.currentKeyState[key]);
}

bool8 IsKeyDown(KeyboardKey key)
{
    return (snowflake.keyboard.currentKeyState[key]);
}

bool8 IsKeyUp(KeyboardKey key)
{
    return (!snowflake.keyboard.currentKeyState[key]);
}

Vec2 GetMousePosition()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);

    f64 xPos = 0;
    f64 yPos = 0;
    glfwGetCursorPos(snowflake.window.handle, &xPos, &yPos);
    Vec2 mousePosition = { (f32) xPos, (f32) yPos };
    return mousePosition;
}

Vec2 GetMouseWheel()
{
    return snowflake.mouse.scrollWheel;
}

bool8 IsMousePressed(MouseButton button)
{
    return (!snowflake.mouse.prevButtonState[button] && snowflake.mouse.currentButtonState[button]);
}

bool8 IsMouseReleased(MouseButton button)
{
    return (snowflake.mouse.prevButtonState[button] && !snowflake.mouse.currentButtonState[button]);
}

bool8 IsMouseDown(MouseButton button)
{
    return (snowflake.mouse.currentButtonState[button]);
}

bool8 IsMouseUp(MouseButton button)
{
    return (!snowflake.mouse.currentButtonState[button]);
}

static void WindowSizeCallback(GLFWwindow* window, i32 width, i32 height)
{
    RendererCreateViewport(width, height);
    LOG_TRACE("WindowResizeCallback: (NewWidth:%d, NewHeight:%d)", width, height);
}

static void KeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods)
{
    // NOTE: KEY_UNKNOWN = -1
    if (key < 0) {
        LOG_WARN("Unknown key: %d", key);
        return;
    }

    snowflake.keyboard.currentKeyState[key] = (action != GLFW_RELEASE);
}

static void MouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods)
{
    snowflake.mouse.currentButtonState[button] = (action != GLFW_RELEASE);
}

static void MouseScrollCallback(GLFWwindow* window, f64 xOffset, f64 yOffset)
{
    snowflake.mouse.scrollWheel.x = (f32) xOffset;
    snowflake.mouse.scrollWheel.y = (f32) yOffset;
}

void BeginDrawing()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    SASSERT_MSG(!isDrawing, "Can't call 'BeginDrawing()' more than once")

    isDrawing = true;

    snowflake.time.currentFrameTime = (f32) glfwGetTime();
    snowflake.time.deltaTime = snowflake.time.currentFrameTime - snowflake.time.prevFrameTime;
    snowflake.time.prevFrameTime = snowflake.time.currentFrameTime;
}

void EndDrawing()
{
    SASSERT_MSG(snowflake.window.handle, ERROR_STR_WINDOW_INIT);
    SASSERT_MSG(isDrawing, "Can't call 'EndDrawing()' more than once");

    isDrawing = false;

    glfwSwapBuffers((GLFWwindow*) GetGLFWwindowHandle());

    snowflake.fps.frameCounter++;
    snowflake.fps.timer += snowflake.time.deltaTime;

    if (snowflake.fps.timer >= 1.0f) {
        snowflake.fps.fps = (u32) ((f32) snowflake.fps.frameCounter / snowflake.fps.timer);
        snowflake.fps.timer = 0.0f;
        snowflake.fps.frameCounter = 0;
    }
}