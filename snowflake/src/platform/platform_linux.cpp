#include "platform.h"
#include "core/defines.h"

#if SPLATFORM_LINUX

#include <cstdio>

void PlatformConsoleWrite(const char* msg, u8 color)
{
    // TRACE,DEBUG,INFO,WARN,ERROR,FATAL
    const char* colorStrs[] = { 0, "1;30", "1;36", "1;32", "1;33", "1;31", "0;41"};
    fprintf(stdout, "\033[%sm%s\033[0m", colorStrs[color], msg);
}

void PlatformConsoleWriteError(const char* msg, u8 color)
{
    // TRACE,DEBUG,INFO,WARN,ERROR,FATAL
    const char* colorStrs[] = { 0, "1;30", "1;36", "1;32", "1;33", "1;31", "0;41"};
    fprintf(stderr, "\033[%sm%s\033[0m", colorStrs[color], msg);
}

#endif