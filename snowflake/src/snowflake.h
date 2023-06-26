#pragma once

#include "defines.h"
#include "sassert.h"
#include "logger.h"
#include "smath.h"

enum SAPI ConfigFlags {
    FLAG_WINDOW_VSYNC_HINT = 1 << 1,
    FLAG_WINDOW_RESIZEABLE = 1 << 2,
    FLAG_WINDOW_UNDECORATED = 1 << 3,
    FLAG_CONTEXT_OPENGL_CORE_PROFILE = 1 << 4,
    FLAG_CONTEXT_OPENGL_3 = 1 << 5,
};

SAPI bool8 InitWindow(const char* title, i32 width, i32 height, i32 cFlags);
SAPI void CloseWindow();
SAPI bool8 WindowShouldClose();
SAPI bool8 IsWindowFullscreen();
SAPI bool8 IsWindowHidden();
SAPI bool8 IsWindowFocused();
SAPI bool8 IsWindowState(i32 flags);
SAPI void ToggleFullscreen();
SAPI void MaximizeWindow();
SAPI void MinimizeWindow();
SAPI void SetWindowTitle(const char* title);
SAPI void SetWindowPosition(i32 x, i32 y);
SAPI void SetWindowSize(i32 width, i32 height);
SAPI Vec2 GetWindowSize();
SAPI Vec2 GetWindowPosition();

SAPI i32 GetWindowWidth();
SAPI i32 GetWindowHeight();

SAPI void PollInputEvents();

SAPI void ShowCursor();
SAPI void HideCursor();
SAPI bool8 IsCursorHidden();
SAPI void EnableCursor();
SAPI void DisableCursor();

SAPI f64 GetTime();
SAPI f32 GetFrameTime();
SAPI i32 GetFPS();

SAPI void EnableVsync();
SAPI void DisableVsync();
SAPI bool8 IsVsyncEnabled();