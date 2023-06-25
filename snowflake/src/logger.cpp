#include <cstdio>
#include <cstdarg>

#include "logger.h"
#include "sassert.h"
#include "platform.h"

#define MAX_MESSAGE_LEN 32000

static LogLevel logLevel = LOG_LEVEL_ALL;

void LogMessage(LogLevel level, const char* msg, ...)
{
    const char* levelStrings[] = {"", "[TRACE]: ", "[DEBUG]: ", "[INFO]: ", "[WARN]: ", "[ERROR]: ", "[FATAL]: "};
    bool8 isError = level > LOG_LEVEL_WARN;

    if (level < logLevel) {
        return;
    }

    char tmpBuf[MAX_MESSAGE_LEN] = {};

    va_list argPtr;
    va_start(argPtr, msg);
    vsnprintf(tmpBuf, MAX_MESSAGE_LEN, msg, argPtr);
    va_end(argPtr);

    char outMsg[MAX_MESSAGE_LEN] = {};

    snprintf(outMsg, MAX_MESSAGE_LEN, "%s%s\n", levelStrings[level], tmpBuf);

    // TODO(Tony): Add file logging

    if (isError) {
        PlatformConsoleWriteError(outMsg, level);
    } else {
        PlatformConsoleWrite(outMsg, level);
    }
}

void LoggerSetLevel(LogLevel level)
{
    logLevel = level;
}

void ReportAssertionFailure(const char* expr, const char* message, const char* file, int line)
{
    LogMessage(LOG_LEVEL_FATAL, "ASSERTION FAILURE: %s, message: '%s', in file: %s, line: %d\n",
               expr, message, file, line);
}