#pragma once

#include "defines.h"

#ifdef SNOWFLAKE_RELEASE
#define LOG_TRACE_ENABLED 0
#define LOG_DEBUG_ENABLED 0
#define LOG_INFO_ENABLED 0
#define LOG_WARN_ENABLED 0
#else
#define LOG_TRACE_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_WARN_ENABLED 1
#endif

#define TICK(X) clock_t X = clock()
#define TOC(X) ((double)(clock() - (X)) / CLOCKS_PER_SEC)

#define LOG_FATAL(msg, ...) LogMessage(LOG_LEVEL_FATAL, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) LogMessage(LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)

#define LOG_TIME_BEGIN(timer) TICK(timer)
#define LOG_TIME_END(timer, msg) LogMessage(LOG_LEVEL_INFO, msg "T:%gs", TOC(timer))

#if LOG_TRACE_ENABLED
#define LOG_TRACE(msg, ...) LogMessage(LOG_LEVEL_TRACE, msg, ##__VA_ARGS__)
#else
#define LOG_TRACE
#endif

#if LOG_DEBUG_ENABLED
#define LOG_DEBUG(msg, ...) LogMessage(LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__)
#else
#define LOG_DEBUG
#endif

#if LOG_INFO_ENABLED
#define LOG_INFO(msg, ...) LogMessage(LOG_LEVEL_INFO, msg, ##__VA_ARGS__)
#else
#define LOG_INFO
#endif

#if LOG_WARN_ENABLED
#define LOG_WARN(msg, ...) LogMessage(LOG_LEVEL_WARN, msg, ##__VA_ARGS__)
#else
#define LOG_WARN
#endif

enum SAPI LogLevel {
    LOG_LEVEL_ALL = 0,
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_NONE
};

SAPI void LogMessage(LogLevel level, const char* msg, ...);
SAPI void LoggerSetLevel(LogLevel level);