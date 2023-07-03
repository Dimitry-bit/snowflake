#include <cstdio>
#include "snowflake.h"
#include "srenderer.h"
#include "srenderer_internal.h"

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
//    LOG_INFO("Screen Size: %dx%d", (i32) GetWindowSize().x, (i32) GetWindowSize().y);
//    SASSERT(GetWindowWidth() == 1280);
//    SASSERT(GetWindowHeight() == 720);

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

    // @formatter:off
    f32 vertices[] = {
        // Position  // Color
        0.0f,  0.5f, 1.0f, 0.0f, 0.0f,
       -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f
    };
    // @formatter:on

    u32 indices[] = {
        0, 1, 2
    };

    Shader shader = ShaderLoadFromFiles("../resources/shaders/basic_vertex.glsl",
                                        "../resources/shaders/basic_fragment.glsl");
    VertexArray va = VertexArrayInit();
    VertexBuffer vb = VertexBufferInit(vertices, sizeof(vertices));
    {
        VertexBufferLayout layout = VertexBufferLayoutInit();
        VertexBufferLayoutPushFloat(&layout, 2);
        VertexBufferLayoutPushFloat(&layout, 3);
        VertexArrayAddBuffer(&va, &vb, &layout);
        VertexBufferLayoutDelete(&layout);
    }

    IndexBuffer ib = IndexBufferInit(indices, 3);

    Mat4 proj = MatrixOrthogonal(0.0f, 800.0f, 600.0f, 0.0f, 0.0f, 1.0f);
    // NOTE: Flip Y-axis
    proj = MatrixScale(proj, Vec3{ 1.0f, -1.0f, 1.0f });

    Mat4 view = Matrix4Identity();
    view = MatrixTranslate(view, Vec3{ 0.0f, 0.0f, 0.0f });

    Mat4 model = Matrix4Identity();
    model = MatrixTranslate(model, Vec3{ 400.0f, -300.0f, 0.0f });
    model = MatrixScale(model, Vec3{ 300, 300, 1 });

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
                LOG_DEBUG("'%d' Key is Pressed", KEY_1);
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

        model = MatrixRotate(model, GetFrameTime(), Vec3{ 0.0f, 0.0f, 1.0f });
        Mat4 mvp = proj * view * model;
        ShaderSetMatrix4(&shader, "uMvp", mvp);

        DrawLowLevel(&va, &ib, &shader);

        EndDrawing();
        PollInputEvents();
    }

    ShaderDelete(&shader);
    VertexArrayDelete(&va);
    VertexBufferDelete(&vb);
    IndexBufferDelete(&ib);

    CloseWindow();

    return 0;
}