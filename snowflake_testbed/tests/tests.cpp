#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_session.hpp"

#include "snowflake.h"

int main(int argc, char* argv[])
{
    int result = Catch::Session().run(argc, argv);
    return result;
}

TEST_CASE("Window Functions", "[CORE]")
{
    WindowConfig config = { };
    config.flags = FLAG_WINDOW_RESIZEABLE | FLAG_CONTEXT_OPENGL_3 | FLAG_CONTEXT_OPENGL_CORE_PROFILE;

    REQUIRE(InitWindow("TestWindow", 800, 600, config));
    REQUIRE(IsWindowState(config.flags) == true);

    LOG_INFO("Screen Size: %dx%d", (i32) GetWindowSize().x, (i32) GetWindowSize().y);

    REQUIRE(IsWindowHidden() == false);
    REQUIRE(IsWindowFullscreen() == false);

    EnableVsync();
    REQUIRE(IsVsyncEnabled() == true);
    DisableVsync();
    REQUIRE(IsVsyncEnabled() == false);

    SetWindowSize(800, 600);

    SetWindowPosition(100, 100);
    MaximizeWindow();
    MinimizeWindow();

    HideCursor();
    REQUIRE(IsCursorHidden() == true);
    ShowCursor();
    REQUIRE(IsCursorHidden() == false);
    DisableCursor();
    EnableCursor();

    CloseWindow();
}

TEST_CASE("File Utils", "[UTILS]")
{
    StringViewer fn = FileGetFileName("../resources/wall.bmp");
    StringViewer fe = FileGetExtension("../resources/wall.bmp");
    StringViewer fne = FileGetFileNameNoExtension("../resources/wall.bmp");
    StringViewer path = FileGetPath("../resources/wall.bmp");

    REQUIRE((strncmp(fn.data, "wall.bmp", fn.length) == 0 && fn.length == 8));
    REQUIRE(((strncmp(fe.data, ".bmp", fe.length) == 0 && fe.length == 4)));
    REQUIRE(((strncmp(fne.data, "wall", fne.length) == 0 && fne.length == 4)));
    REQUIRE(((strncmp(path.data, "../resources/", path.length) == 0 && path.length == 13)));
    LOG_DEBUG("FileName '%.*s', Extension '%.*s', FileNameOnly '%.*s', Path '%.*s'",
              fn.length, fn.data, fe.length, fe.data, fne.length, fne.data, path.length, path.data);
}

TEST_CASE("Math Functions", "[MATH]")
{
    REQUIRE(Min(5, 10) == 5);
    REQUIRE(Min(5.0f, 10.0f) == 5.0f);
    REQUIRE(Max(5, 10) == 10);
    REQUIRE(Max(5.0f, 10.0f) == 10.0f);
    REQUIRE(Clamp(2, 0, 1) == 1);
    REQUIRE(Clamp(2.0f, 0.0f, 1.0f) == 1.0f);
    REQUIRE(Clamp(-2, 0, 1) == 0);
    REQUIRE(Clamp(-2.0f, 0.0f, 0.0f) == 0.0f);
    REQUIRE(Abs(-2) == 2);
    REQUIRE(Abs(-2.0f) == 2.0f);
    REQUIRE(Square(-2) == 4);
    REQUIRE(Square(-2.0f) == 4.0f);
}