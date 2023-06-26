#include "snowflake.h"

int main()
{
    i32 configFlags = FLAG_WINDOW_RESIZEABLE | FLAG_CONTEXT_OPENGL_3 | FLAG_CONTEXT_OPENGL_CORE_PROFILE;

    if (!InitWindow("TestWindow", 800, 600, configFlags)) {
        return -1;
    }

    SASSERT(IsWindowState(configFlags) == true);

    LOG_INFO("Screen Size: %dx%d", GetScreenWidth(), GetScreenHeight());

    SASSERT(IsWindowHidden() == false);
    SASSERT(IsWindowFullscreen() == false);

    EnableVsync();
    SASSERT(IsVsyncEnabled() == true);
    DisableVsync();
    SASSERT(IsVsyncEnabled() == false);

    SetWindowSize(1280, 720);
    LOG_INFO("Screen Size: %dx%d", GetScreenWidth(), GetScreenHeight());
    SASSERT(GetScreenWidth() == 1280);
    SASSERT(GetScreenHeight() == 720);

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
        PollInputEvents();
    }

    CloseWindow();

    return 0;
}