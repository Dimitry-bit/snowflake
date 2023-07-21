#pragma once

#include "defines.h"
#include "utils.h"

struct SAPI StringViewer {
    const char* data;
    u32 length;
};

SAPI char* FileLoad(const char* filePath);
SAPI u8* FileLoadBinary(const char* filePath);
SAPI void FileUnload(void* data);

SAPI StringViewer FileGetExtension(const char* filePath);
SAPI StringViewer FileGetFileName(const char* filePath);
SAPI StringViewer FileGetFileNameNoExtension(const char* filePath);
SAPI StringViewer FileGetPath(const char* filePath);