#include "smemory.h"
#include "logger.h"
#include "sassert.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

struct AllocMetaData {
    u32 size;
    MemoryTags tag;
};

struct MemoryContext {
    u64 totalAllocated;
    u64 taggedAllocations[MEMORY_TAG_MAX_TAGS];
    std::unordered_map<void*, AllocMetaData> allocTable;
};

static MemoryContext memContext;

void MemoryStartup()
{
    SASSERT_MSG(memContext.totalAllocated == 0 && memContext.allocTable.empty(),
                "MemoryStart() must be called before any allocations");

    memContext.totalAllocated = 0;
    memset(memContext.taggedAllocations, 0, MEMORY_TAG_MAX_TAGS);
    memContext.allocTable.clear();
}

void MemoryShutdown()
{
    if (memContext.totalAllocated != 0 || !memContext.allocTable.empty()) {
        LOG_FATAL(SMemUsage());

        memContext.totalAllocated = 0;
        memset(memContext.taggedAllocations, 0, MEMORY_TAG_MAX_TAGS);
        memContext.allocTable.clear();

        SASSERT_MSG(false, "Memory might be lost (Memory leak)");
    }
}

/*
    Returns a pointer to a beginning of a zeroed block with specified size
*/
void* SMalloc(u32 size, MemoryTags tag)
{
#ifdef SNOWFLAKE_MEM_DEBUG
    if (tag == MEMORY_TAG_UNKNOWN) {
        LOG_WARN("SMalloc called using MEMORY_TAG_UNKNOWN. Re-class this allocation");
    }

    memContext.totalAllocated += size;
    memContext.taggedAllocations[tag] += size;
#endif

    void* block = malloc(size);
    if (!block) {
        SASSERT_MSG(!block, "[FATAL]: Out Of Memory");
    }

    memset(block, 0, size);

#ifdef SNOWFLAKE_MEM_DEBUG
    memContext.allocTable.emplace(block, AllocMetaData{ size, tag });
#endif

    return block;
}

void* SRealloc(void* block, u32 size, MemoryTags tag)
{
    void* newBlk = realloc(block, size);

#ifdef SNOWFLAKE_MEM_DEBUG
    if (newBlk) {
        if (memContext.allocTable.count(block)) {
            AllocMetaData old = memContext.allocTable.at(block);
            memContext.totalAllocated -= old.size;
            memContext.taggedAllocations[old.tag] -= old.size;
            memContext.allocTable.erase(block);

            memContext.totalAllocated += size;
            memContext.taggedAllocations[old.tag] += size;
            memContext.allocTable.emplace(newBlk, AllocMetaData{ size, old.tag });
        } else {
            memContext.totalAllocated += size;
            memContext.taggedAllocations[tag] += size;
            memContext.allocTable.emplace(newBlk, AllocMetaData{ size, tag });
        }
    } else {
        LOG_ERROR("SRealloc '%p' failed to allocate block", block);
    }
#endif

    return newBlk;
}

void SFree(void* block)
{
#ifdef SNOWFLAKE_MEM_DEBUG
    if (memContext.allocTable.count(block)) {
        AllocMetaData metaData = memContext.allocTable.at(block);
        memContext.allocTable.erase(block);

        memContext.totalAllocated -= metaData.size;
        memContext.taggedAllocations[metaData.tag] -= metaData.size;
    } else if (block) {
        LOG_WARN("SFree '%p' allocation is not recorded in AllocTable", block);
    }
#endif

    free(block);
}

void SMemZero(void* block, u32 size)
{
    memset(block, 0, size);
}

void SMemSet(void* dst, i32 value, u32 size)
{
    memset(dst, value, size);
}

void SMemCopy(void* dst, const void* src, u32 size)
{
    memcpy(dst, src, size);
}

void SMemMove(void* dst, const void* src, u32 size)
{
    memmove(dst, src, size);
}
char* SMemUsage()
{
    static const char* memoryTagStr[MEMORY_TAG_MAX_TAGS] = {
        "UNKNOWN    ",
        "ARRAY      ",
        "STRING     ",
        "APPLICATION",
        "TEXTURE    ",
        "IMAGE      ",
        "RENDERER   ",
        "FONT       ",
    };

    const u64 gib = 1024 * 1024 * 1024;
    const u64 mib = 1024 * 1024;
    const u64 kib = 1024;

    char buffer[8000] = "System Memory usage (tagged):\n";
    u64 offset = strlen(buffer);
    for (u32 i = 0; i < MEMORY_TAG_MAX_TAGS; ++i) {
        char unit[4] = "xiB";
        f32 amount = 1.0f;

        if (memContext.taggedAllocations[i] >= gib) {
            unit[0] = 'G';
            amount = (f32) memContext.taggedAllocations[i] / (f32) gib;
        } else if (memContext.taggedAllocations[i] >= mib) {
            unit[0] = 'M';
            amount = (f32) memContext.taggedAllocations[i] / (f32) mib;
        } else if (memContext.taggedAllocations[i] >= kib) {
            unit[0] = 'K';
            amount = (f32) memContext.taggedAllocations[i] / (f32) kib;
        } else {
            unit[0] = 'B';
            unit[1] = '\0';
            amount = (f32) memContext.taggedAllocations[i];
        }

        i32 len = snprintf(buffer + offset, 8000 - offset, " %s: %.2f%s\n", memoryTagStr[i], amount, unit);
        offset += len;
    }

    char* out = (char*) calloc((offset + 1), sizeof(char));
    memcpy(out, buffer, offset);
    out[offset] = '\0';

    return out;
}