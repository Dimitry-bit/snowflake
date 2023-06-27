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

        {
            char title[120] = { 0 };
            snprintf(title, 120, "W:%g,%g, M:%g,%g",
                     GetWindowPosition().x, GetWindowPosition().y,
                     GetMousePosition().x, GetMousePosition().y);
            SetWindowTitle(title);
        }

        {
            if (IsKeyPressed(KEY_1)) {
                LOG_INFO("'%d' Key is Pressed", KEY_1);
            }
            if (IsKeyReleased(KEY_1)) {
                LOG_INFO("'%d' Key is Released", KEY_1);
            }

            if (IsMousePressed(MOUSE_BUTTON_LEFT)) {
                LOG_INFO("'%d' Key is Pressed", MOUSE_BUTTON_LEFT);
            }
            if (IsMouseReleased(MOUSE_BUTTON_LEFT)) {
                LOG_INFO("'%d' Key is Released", MOUSE_BUTTON_LEFT);
            }
            if (GetMouseWheel().y > 0 || GetMouseWheel().y < 0) {
                LOG_INFO("MouseWheel move %f", GetMouseWheel().y);
            }
        }

        PollInputEvents();
    }

    CloseWindow();

    return 0;
}