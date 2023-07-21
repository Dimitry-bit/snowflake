#pragma once

#include "defines.h"

#define SAlloca(size) alloca(size)

enum SAPI MemoryTags {
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_STRING,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_TEXTURE,
    MEMORY_TAG_IMAGE,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_FONT,

    MEMORY_TAG_MAX_TAGS,
};

SAPI void MemoryStartup();
SAPI void MemoryShutdown();

SAPI void* SMalloc(u32 size, MemoryTags tag);
SAPI void SFree(void* block);
SAPI void SMemZero(void* block, u32 size);
SAPI void SMemSet(void* dst, i32 value, u32 size);
SAPI void SMemCopy(void* dst, const void* src, u32 size);
SAPI void SMemMove(void* dst, const void* src, u32 size);

SAPI char* SMemUsage();