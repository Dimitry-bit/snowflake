#include <cstring>
#include <cstdio>
#include "snowflake.h"
#include "srenderer.h"
#include "srenderer_internal.h"
#include "smemory.h"

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

    SetWindowSize(800, 600);

    SetWindowPosition(100, 100);
    MaximizeWindow();
    MinimizeWindow();

    HideCursor();
    SASSERT(IsCursorHidden() == true);
    ShowCursor();
    SASSERT(IsCursorHidden() == false);
    DisableCursor();
    EnableCursor();

    SASSERT(Min(5, 10) == 5);
    SASSERT(Min(5.0f, 10.0f) == 5.0f);
    SASSERT(Max(5, 10) == 10);
    SASSERT(Max(5.0f, 10.0f) == 10.0f);
    SASSERT(Clamp(2, 0, 1) == 1);
    SASSERT(Clamp(2.0f, 0.0f, 1.0f) == 1.0f);
    SASSERT(Clamp(-2, 0, 1) == 0);
    SASSERT(Clamp(-2.0f, 0.0f, 0.0f) == 0.0f);
    SASSERT(Abs(-2) == 2);
    SASSERT(Abs(-2.0f) == 2.0f);
    SASSERT(Square(-2) == 4);
    SASSERT(Square(-2.0f) == 4.0f);

    Texture2D tex = TextureLoadFromFile("../resources/wall.bmp");

    StringViewer fn = FileGetFileName("../resources/wall.bmp");
    StringViewer fe = FileGetExtension("../resources/wall.bmp");
    StringViewer fne = FileGetFileNameNoExtension("../resources/wall.bmp");
    StringViewer path = FileGetPath("../resources/wall.bmp");

    SASSERT(strncmp(fn.data, "wall.bmp", fn.length) == 0 && fn.length == 8);
    SASSERT(strncmp(fe.data, ".bmp", fe.length) == 0 && fe.length == 4);
    SASSERT(strncmp(fne.data, "wall", fne.length) == 0 && fne.length == 4);
    SASSERT(strncmp(path.data, "../resources/", path.length) == 0 && path.length == 13);
    LOG_DEBUG("FileName '%.*s', Extension '%.*s', FileNameOnly '%.*s', Path '%.*s'",
              fn.length, fn.data, fe.length, fe.data, fne.length, fne.data, path.length, path.data);

    LOG_INFO(SMemUsage());

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(OLDBLACK);

        {
            const u32 titleLen = 512;
            char title[titleLen] = { };
            snprintf(title, titleLen, "WinPos:(X:%g, Y:%g) | MPos:(X:%g, Y:%g) | FTime:'%.1f'ms | FPS:%u",
                     GetWindowPosition().x, GetWindowPosition().y,
                     GetMousePosition().x, GetMousePosition().y,
                     GetFrameTime() * 1000, GetFPS());
            SetWindowTitle(title);
        }

        {
            if (IsKeyPressed(KEY_1)) {
                RendererSetPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                LOG_DEBUG("'%d' Key is Pressed", KEY_1);
            }
            if (IsKeyPressed(KEY_2)) {
                RendererSetPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            if (IsKeyPressed(KEY_3)) {
                RendererSetPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            }
            if (IsKeyReleased(KEY_1)) {
                LOG_DEBUG("'%d' Key is Released", KEY_1);
            }

            if (IsMousePressed(MOUSE_BUTTON_LEFT)) {
                LOG_DEBUG("'%d' Key is Pressed", MOUSE_BUTTON_LEFT);
            }
            if (IsMouseReleased(MOUSE_BUTTON_LEFT)) {
                LOG_DEBUG("'%d' Key is Released", MOUSE_BUTTON_LEFT);
            }
            if (GetMouseWheel().y > 0 || GetMouseWheel().y < 0) {
                LOG_DEBUG("MouseWheel move %f", GetMouseWheel().y);
            }
        }

        {
            {
                f32 width = 50.0f;
                f32 height = 50.0f;
                f32 padding = 20.0f;
                i32 countX = (GetWindowWidth() - (padding * 2)) / width;
                i32 countY = (GetWindowHeight() - (padding * 2)) / height;
                for (i32 i = 0; i < countX; ++i) {
                    for (i32 j = 0; j < countY; ++j) {
                        f32 posX = padding + i * (width + padding);
                        f32 posY = padding + j * (height + padding);
                        Sprite sprite = SpriteCreate(posX, posY, width, height);
                        sprite.transform.rotation = Sin((f32) GetTime());
                        sprite.transform.origin = Vec2{ 0.5f * sprite.width, 0.5f * sprite.height };
                        sprite.texture = &tex;
                        DrawSprite(&sprite);
                    }
                }
            }

            f32 scrWidth = GetWindowWidth();
            f32 scrHeight = GetWindowHeight();

            Vec2 v1 = { scrWidth / 2.0f, scrHeight / 2.0f - 100.0f };
            Vec2 v2 = { scrWidth / 2.0f - 100.0f, scrHeight / 2.0f + 100.0f };
            Vec2 v3 = { scrWidth / 2.0f + 100.0f, scrHeight / 2.0f + 100.0f };

            DrawPixel(scrWidth - 100.0f, 50.0f, WHITE);
            DrawPixel(scrWidth - 150.0f, 50.0f, WHITE);
            DrawLine(500.0f, 50.0f, 600.0f, 50.0f, 1.0f, DEEPMAGENTA);

            CircleShape circle = CircleCreate(0.0f, 0.0f, 40.0f, GLOSSYCYAN);
            DrawCirclePro(&circle);

            EllipseShape ellipse = EllipseCreate(100.0f, 0.0f, 20.0f, 40.0f, GOLD);
            DrawEllipsePro(&ellipse);

            RingShape ring = RingCreate(200.0f, 0.0f, 35.0f, 40.0f, COSMICPINK);
            DrawRingPro(&ring);

            DrawTriangle(v1, v2, v3, CANDYPURPLE);
        }

        EndDrawing();
        PollInputEvents();
    }

    TextureDelete(&tex);

    CloseWindow();

    return 0;
}