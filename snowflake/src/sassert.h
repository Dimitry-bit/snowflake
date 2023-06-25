#pragma once

#include "defines.h"

#if defined(ASSERTION_ENABLED) && defined(SNOWFLAKE_DEBUG)
#ifdef _MSV_VER
#include <intrin.h>
#define debugbreak() __debugbreak()
#else
#define debugbreak() __builtin_trap()
#endif

#define SASSERT(expr)                                               \
    if (expr) {                                                     \
    } else {                                                        \
        ReportAssertionFailure(#expr, "", __FILE__, __LINE__);      \
        debugbreak();                                               \
    }

#define SASSERT_MSG(expr, msg)                                      \
    if (expr) {                                                     \
    } else {                                                        \
        ReportAssertionFailure(#expr, msg, __FILE__, __LINE__);     \
        debugbreak();                                               \
    }

#else
#define SASSERT(expr)
#define SASSERT_MSG(expr, msg)
#endif

SAPI void ReportAssertionFailure(const char* expr, const char* message, const char* file, int line);