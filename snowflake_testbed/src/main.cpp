#include <cstdio>
#include "snowflake.h"

int main()
{
    i32 configFlags = FLAG_WINDOW_RESIZEABLE | FLAG_CONTEXT_OPENGL_3 | FLAG_CONTEXT_OPENGL_CORE_PROFILE;

    if (!InitWindow("TestWindow", 800, 600, configFlags)) {
        return -1;
    }

    SASSERT(IsWindowState(configFlags) == true);

    LOG_INFO("Screen Size: %dx%d", (i32) GetWindowSize().x, (i32) GetWindowSize().y);

    SASSERT(IsWindowHidden() == false);
    SASSERT(IsWindowFullscreen() == false);

    EnableVsync();
    SASSERT(IsVsyncEnabled() == true);
    DisableVsync();
    SASSERT(IsVsyncEnabled() == false);

    SetWindowSize(1280, 720);
    LOG_INFO("Screen Size: %dx%d", (i32) GetWindowSize().x, (i32) GetWindowSize().y);
    SASSERT(GetWindowWidth() == 1280);
    SASSERT(GetWindowHeight() == 720);

    SetWindowPosition(100, 100);
    MaximizeWindow();
    MinimizeWindow();

    HideCursor();
    SASSERT(IsCursorHidden() == true);
    ShowCursor();
    SASSERT(IsCursorHidden() == false);
    DisableCursor();
    EnableCursor();

    while (!WindowShouldClose()) {

        char title[32] = { 0 };
        snprintf(title, 32, "%g,%g", GetWindowPosition().x, GetWindowPosition().y);
        SetWindowTitle(title);

        PollInputEvents();
    }

    CloseWindow();

    return 0;
}