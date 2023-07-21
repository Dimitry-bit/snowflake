#pragma once

#include "core/defines.h"

SAPI u8* SImageLoad(const char* filePath, i32* width, i32* height, i32* nrChannels);
SAPI void SImageUnload(void* data);