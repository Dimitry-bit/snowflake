#include "core/smemory.h"
#include "renderer/srenderer_internal.h"
#include "snowflake.h"

#include <cstdio>

static void TestInput();
static void TestPrimitiveShapes();
static void TestTextureDrawing(const Texture2D* texture);

int main()
{
    WindowConfig config = { };
    config.flags = FLAG_WINDOW_RESIZEABLE | FLAG_CONTEXT_OPENGL_3 | FLAG_CONTEXT_OPENGL_CORE_PROFILE;
    config.antialiasingLevel = 4;

    if (!InitWindow("TestWindow", 800, 600, config)) {
        return -1;
    }

    Texture2D tex = TextureLoadFromFile("../resources/RPGpack_sheet.bmp");
    Font font = FontLoadFromFile("../resources/IBMPlexSans-Regular.ttf", 48);
    Text testText = TextCreate(&font);
    TextSetString(&testText, "Potato Man Strikes Again");

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

        TestInput();
//        TestPrimitiveShapes();
        TestTextureDrawing(&tex);

        DrawText(&testText, Vec2{ 100, 100 });

        EndDrawing();
        PollInputEvents();
    }

    TextDelete(&testText);
    TextureUnload(&tex);
    FontUnload(&font);

    CloseWindow();
}

static void TestInput()
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

static void TestPrimitiveShapes()
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
            RectangleShape rect = RectangleCreate(Vec2{ posX, posY }, Vec2{ width, height }, ANGLEBLUE);
            rect.transform.rotation = Vec3{ 0.0f, 0.0f, Sin((f32) GetTime()) };
            rect.transform.origin = Vec3{ 0.5f * rect.width, 0.5f * rect.height, 0.0f };
            DrawRectanglePro(&rect);
        }
    }

    f32 scrWidth = GetWindowWidth();
    f32 scrHeight = GetWindowHeight();

    Vec2 v1 = { scrWidth / 2.0f, scrHeight / 2.0f - 100.0f };
    Vec2 v2 = { scrWidth / 2.0f - 100.0f, scrHeight / 2.0f + 100.0f };
    Vec2 v3 = { scrWidth / 2.0f + 100.0f, scrHeight / 2.0f + 100.0f };

    DrawPixel(Vec2{ scrWidth - 100.0f, 50.0f }, WHITE);
    DrawPixel(Vec2{ scrWidth - 150.0f, 50.0f }, WHITE);
    DrawLine(Vec2{ 500.0f, 50.0f }, Vec2{ 600.0f, 50.0f }, 1.0f, DEEPMAGENTA);

    CircleShape circle = CircleCreate(Vec2{ 0.0f, 0.0f }, 40.0f, GLOSSYCYAN);
    DrawCirclePro(&circle);

    EllipseShape ellipse = EllipseCreate(Vec2{ 100.0f, 0.0f }, 40.0f, 20.0f, GOLD);
    DrawEllipsePro(&ellipse);

    RingShape ring = RingCreate(Vec2{ 200.0f, 0.0f }, 35.0f, 40.0f, COSMICPINK);
    DrawRingPro(&ring);

    DrawTriangle(v1, v2, v3, CANDYPURPLE);
}

static void TestTextureDrawing(const Texture2D* texture)
{
    static char map[] = {
        "WWWWWWWWWWWWWWWWWWW"
        "WWWWGGWGGGGGWGGWWWW"
        "WWWGGGBGGGTGGGGWWWW"
        "WWWGGGTGGGGGGBGGWWW"
        "WWWWGGGGGGGGTGGGWWW"
        "WWWGGGGBGGGGGGGGWWW"
        "WWWWGGGGGGGBGGGWWWW"
        "WWWWWWWWWWWWWWWWWWW"
    };

    SubTexture2D barrel = SubTexture2DCreate(texture, Vec2{ 8, 11 }, Vec2{ 64, 64 }, Vector2One());
    SubTexture2D tree = SubTexture2DCreate(texture, Vec2{ 0, 10 }, Vec2{ 64, 64 }, Vec2{ 1, 2 });
    SubTexture2D ground = SubTexture2DCreate(texture, Vec2{ 1, 1 }, Vec2{ 64, 64 }, Vector2One());
    SubTexture2D water = SubTexture2DCreate(texture, Vec2{ 11, 1 }, Vec2{ 64, 64 }, Vector2One());

    const i32 mapWidth = 19;
    const i32 mapHeight = 7;

    f32 width = (f32) ground.rect.width;
    f32 height = (f32) ground.rect.height;

    for (i32 y = 0; y < mapHeight; y++) {
        for (i32 x = 0; x < mapWidth; x++) {
            Sprite tile = SpriteCreate(Vec2{ x * width, y * height }, nullptr);
            char tileID = map[x + y * mapWidth];
            if (tileID == 'W') {
                SpriteSetTexture(&tile, &water, true);
                DrawSpritePro(&tile);
            } else {
                SpriteSetTexture(&tile, &ground, true);
                DrawSpritePro(&tile);
            }
        }
    }

    for (i32 y = 0; y < mapHeight; y++) {
        for (i32 x = 0; x < mapWidth; x++) {
            Sprite tile = SpriteCreate(Vec2{ x * width, y * height }, nullptr);
            char tileID = map[x + y * mapWidth];
            if (tileID == 'T') {
                SpriteSetTexture(&tile, &tree, true);
                DrawSpritePro(&tile);
            } else if (tileID == 'B') {
                SpriteSetTexture(&tile, &barrel, true);
                DrawSpritePro(&tile);
            }
        }
    }
}