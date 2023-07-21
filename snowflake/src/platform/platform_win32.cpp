#include "platform.h"
#include "core/defines.h"

#if SPLATFORM_WINDOWS

#include <windows.h>

void PlatformConsoleWrite(const char* message, u8 color)
{
    // TRACE,DEBUG,INFO,WARN,ERROR,FATAL
    static u8 levels[] = {0, 8, 3, 2, 6, 4, 64};
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_handle, levels[color]);
    OutputDebugStringA(message);
    size_t length = strlen(message);
    DWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD) length, &number_written, 0);
}

void PlatformConsoleWriteError(const char* message, u8 color)
{
    // TRACE,DEBUG,INFO,WARN,ERROR,FATAL
    static u8 levels[] = {0, 8, 3, 2, 6, 4, 64};
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    SetConsoleTextAttribute(console_handle, levels[color]);
    OutputDebugStringA(message);
    size_t length = strlen(message);
    DWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD) length, &number_written, 0);
}

#endif