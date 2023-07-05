#include <cstring>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "snowflake.h"
#include "srenderer_internal.h"

#define ERROR_STR_WINDOW_INIT "Window is not initialized, Invoke InitWindow()"
#define MAX_KEYBOARD_KEYS 512
#define MAX_MOUSE_BUTTONS 8

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

static void WindowSizeCallback(GLFWwindow* window, i32 width, i32 height);
static void KeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods);
static void MouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods);
static void MouseScrollCallback(GLFWwindow* window, f64 xOffset, f64 yOffset);

static GLFWwindow* windowHandle = nullptr;
static i32 configFlags = 0;

static bool8 prevKeyState[MAX_KEYBOARD_KEYS] = { false };
static bool8 currentKeyState[MAX_KEYBOARD_KEYS] = { false };
static bool8 prevMouseButtonState[MAX_MOUSE_BUTTONS] = { false };
static bool8 currentMouseButtonState[MAX_MOUSE_BUTTONS] = { false };
static Vec2 mouseWheelMovement = { };

static STime time = { };
static FPS fps = { };

static bool8 isDrawing = false;

bool8 InitWindow(const char* title, i32 width, i32 height, i32 cFlags)
{
    if (windowHandle) {
        LOG_ERROR("Window is already created");
        return false;
    }

    if (!glfwInit()) {
        LOG_FATAL("Failed to initialize GLFW");
        return false;
    } else {
        LOG_TRACE("GLFW initialized successfully");
    }

    configFlags = cFlags;

    if ((configFlags & FLAG_CONTEXT_OPENGL_CORE_PROFILE) > 0) {
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    if ((configFlags & FLAG_CONTEXT_OPENGL_3) > 0) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    }

    // TODO(Tony): Should be extracted a setting
    glfwWindowHint(GLFW_SAMPLES, 4);

    windowHandle = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!windowHandle) {
        LOG_FATAL("Failed to create GLFW windowHandle");
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

    LOG_INFO("Snowflake initialized successfully");

    return true;
}

void CloseWindow()
{
    glfwTerminate();

    windowHandle = nullptr;
    configFlags = 0;

    for (u32 i = 0; i < MAX_KEYBOARD_KEYS; ++i) {
        prevKeyState[i] = false;
        currentKeyState[i] = false;
    }

    for (u32 i = 0; i < MAX_MOUSE_BUTTONS; ++i) {
        prevMouseButtonState[i] = false;
        currentMouseButtonState[i] = false;
    }

    mouseWheelMovement = Vector2Zero();

    memset(&time, 0, sizeof(time));
    memset(&fps, 0, sizeof(FPS));

    LOG_INFO("Snowflake window closed successfully");
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

void* GetGLFWwindowHandle()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    return windowHandle;
}

void PollInputEvents()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);

    for (u32 i = 0; i < MAX_KEYBOARD_KEYS; ++i) {
        prevKeyState[i] = currentKeyState[i];
    }

    for (u32 i = 0; i < MAX_MOUSE_BUTTONS; ++i) {
        prevMouseButtonState[i] = currentMouseButtonState[i];
    }

    mouseWheelMovement = Vector2Zero();

    glfwPollEvents();
}

void ShowCursor()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    LOG_TRACE("CursorState: Show");
}

void HideCursor()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    LOG_TRACE("CursorState: Hidden");
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

    LOG_TRACE("CursorState: Normal");
}

void DisableCursor()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    LOG_TRACE("CursorState: Disabled");
}

f64 GetTime()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    return glfwGetTime();
}

f32 GetFrameTime()
{
    return time.deltaTime;
}

u32 GetFPS()
{
    return fps.fps;
}

void EnableVsync()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwSwapInterval(1);
    configFlags |= FLAG_WINDOW_VSYNC_HINT;

    LOG_INFO("Vsync: Enabled");
}

void DisableVsync()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    glfwSwapInterval(0);
    configFlags &= ~FLAG_WINDOW_VSYNC_HINT;

    LOG_INFO("Vsync: Disabled");
}

bool8 IsVsyncEnabled()
{
    return ((configFlags & FLAG_WINDOW_VSYNC_HINT) > 0);
}

bool8 IsKeyPressed(KeyboardKey key)
{
    return (!prevKeyState[key] && currentKeyState[key]);
}

bool8 IsKeyReleased(KeyboardKey key)
{
    return (prevKeyState[key] && !currentKeyState[key]);
}

bool8 IsKeyDown(KeyboardKey key)
{
    return (currentKeyState[key]);
}

bool8 IsKeyUp(KeyboardKey key)
{
    return (!currentKeyState[key]);
}

Vec2 GetMousePosition()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);

    f64 xPos = 0;
    f64 yPos = 0;
    glfwGetCursorPos(windowHandle, &xPos, &yPos);
    Vec2 mousePosition = { (f32) xPos, (f32) yPos };
    return mousePosition;
}

Vec2 GetMouseWheel()
{
    return mouseWheelMovement;
}

bool8 IsMousePressed(MouseButton button)
{
    return (!prevMouseButtonState[button] && currentMouseButtonState[button]);
}

bool8 IsMouseReleased(MouseButton button)
{
    return (prevMouseButtonState[button] && !currentMouseButtonState[button]);
}

bool8 IsMouseDown(MouseButton button)
{
    return (currentMouseButtonState[button]);
}

bool8 IsMouseUp(MouseButton button)
{
    return (!currentMouseButtonState[button]);
}

static void WindowSizeCallback(GLFWwindow* window, i32 width, i32 height)
{
    GLCall(glViewport(0, 0, width, height));
    LOG_TRACE("WindowResizeCallback: (NewWidth:%d, NewHeight:%d)", width, height);
}

static void KeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods)
{
    // NOTE: KEY_UNKNOWN = -1
    if (key < 0) {
        LOG_WARN("Unknown key: %d", key);
        return;
    }

    currentKeyState[key] = (action != GLFW_RELEASE);
}

static void MouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods)
{
    currentMouseButtonState[button] = (action != GLFW_RELEASE);
}

static void MouseScrollCallback(GLFWwindow* window, f64 xOffset, f64 yOffset)
{
    mouseWheelMovement.x = (f32) xOffset;
    mouseWheelMovement.y = (f32) yOffset;
}

void BeginDrawing()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    SASSERT_MSG(!isDrawing, "Can't call 'BeginDrawing()' more than once")

    isDrawing = true;

    time.currentFrameTime = (f32) glfwGetTime();
    time.deltaTime = time.currentFrameTime - time.prevFrameTime;
    time.prevFrameTime = time.currentFrameTime;
}

void EndDrawing()
{
    SASSERT_MSG(windowHandle, ERROR_STR_WINDOW_INIT);
    SASSERT_MSG(isDrawing, "Can't call 'EndDrawing()' more than once");

    isDrawing = false;

    glfwSwapBuffers((GLFWwindow*) GetGLFWwindowHandle());

    fps.frameCounter++;
    fps.timer += time.deltaTime;

    if (fps.timer >= 1.0f) {
        fps.fps = (u32) ((f32) fps.frameCounter / fps.timer);
        fps.timer = 0.0f;
        fps.frameCounter = 0;
    }
}