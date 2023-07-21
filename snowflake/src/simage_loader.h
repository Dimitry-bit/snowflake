#pragma once

#include "defines.h"

SAPI u8* SImageLoad(const char* filePath, i32* width, i32* height, i32* nrChannels);
SAPI void SImageUnload(void* data);