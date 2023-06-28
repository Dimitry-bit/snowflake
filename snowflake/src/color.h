#pragma once

#include "snowflake.h"

union SAPI Color {
    u8 e[4];

    struct {
        u8 r, g, b, a;
    };
};

#define SNOWWHITE (Color) {243, 246, 251, 255}
#define ANGLEBLUE (Color) {150, 174, 208, 255}
#define GOLD (Color) {255, 215, 0, 255}
#define ROYALTEAL (Color) {17, 108, 110, 255}
#define EARTHGREEN (Color) {24, 94, 63, 255}
#define CALMINGYELLOW (Color) {245, 225, 162, 225}
#define SKYORANGE (Color) {247, 152, 2, 255}
#define CANDYRED (Color) {181, 33, 11, 255}
#define COSMICPINK (Color) {235, 148, 183, 255}
#define CANDYPURPLE (Color) {158, 58, 195, 255}
#define SIMPLEGREY (Color) {103, 103, 101, 255}
#define GLOSSYCYAN (Color) {19, 218, 233, 255}
#define DEEPMAGENTA (Color) {197, 25, 193, 255}
#define OLDBLACK (Color) {41, 40, 39, 255}

#define WHITE (Color) {255, 255, 255, 255}
#define BLACK (Color) {0, 0, 0, 255}