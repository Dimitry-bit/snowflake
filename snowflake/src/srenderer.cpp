#include "GL/glew.h"

#include "srenderer.h"
#include "srenderer_internal.h"

void ClearBackground(u8 r, u8 g, u8 b, u8 a)
{
    f32 outR = (f32) r / 255.0f;
    f32 outG = (f32) g / 255.0f;
    f32 outB = (f32) b / 255.0f;
    f32 outA = (f32) a / 255.0f;

    GLCall(glClearColor(outR, outG, outB, outA));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void ClearBackground(Color color)
{
    ClearBackground(color.r, color.g, color.b, color.a);
}