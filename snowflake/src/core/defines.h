#pragma once

#include <cstdint>

#define ARRAYCOUNT(arr) (sizeof((arr)) / sizeof((arr)[0]))
#define KILOBYTES(n) ((n) * 1024ull)
#define MEGABYTES(n) (Kilobytes(n) * 1024ull)
#define GIGABYTES(n) (Megabytes(n) * 1024ull)

#define STATIC_ASSERT(exp) static_assert(exp)
#define STATIC_ASSERT_MSG(exp, msg) static_assert(exp, msg)

#define SPLATFORM_WINDOWS 0
#define SPLATFORM_LINUX 0

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#undef SPLATFORM_WINDOWS
#define SPLATFORM_WINDOWS 1
#if !(defined(_WIN64) || defined(WIN64))
#error "64-bit is required on Windows!"
#endif
#elif defined(__linux__) || defined(__gnu__linux__)
#undef SPLATFORM_LINUX
#define SPLATFORM_LINUX 1
#endif

#ifdef SNOWFLAKE_EXPORT
#ifdef _MSC_VER
#define SAPI __declspec(dllexport)
#else
#define SAPI __attribute__((visibility("default")))
#endif
#else
#ifdef _MSC_VER
#define SAPI __declspec(dllimport)
#else
#define SAPI
#endif
#endif

// Unsigned int types
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Signed int types
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// Floating point types
typedef float f32;
typedef double f64;

// Boolean types
typedef bool bool8;
typedef i32 bool32;

STATIC_ASSERT(sizeof(bool8) == 1);