#pragma once

SAPI char* FileLoad(const char* filePath);
SAPI u8* FileLoadBinary(const char* filePath);
SAPI void FileUnload(void* data);