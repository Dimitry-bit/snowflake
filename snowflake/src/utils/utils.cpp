#include "utils.h"
#include "core/logger.h"
#include "core/smemory.h"

#include <cstdio>
#include <cstring>

char* FileLoad(const char* filePath)
{
    char* data = nullptr;
    FILE* fp = fopen(filePath, "r");

    if (!fp) {
        LOG_ERROR("'%s' File is not valid", filePath);
        return nullptr;
    }

    if (fseek(fp, 0, SEEK_END) == 0) {
        i64 bufSize = -1;
        bufSize = ftell(fp);
        if (bufSize == -1) {
            LOG_ERROR("'%s' Failed to calculate file size", filePath);
            fclose(fp);
            return nullptr;
        }

        data = (char*) SMalloc((bufSize + 1) * sizeof(char), MEMORY_TAG_STRING);
        if (!data) {
            LOG_ERROR("'%s' Failed to allocate memory", filePath);
        }

        if (fseek(fp, 0, SEEK_SET) != 0) {
            LOG_ERROR("'%s' Failed to read file", filePath);
            fclose(fp);
            SFree(data);
            return nullptr;
        }

        u64 len = fread(data, sizeof(char), bufSize, fp);
        if (ferror(fp) != 0) {
            LOG_ERROR("'%s' Failed to read file", filePath);
            fclose(fp);
            SFree(data);
            return nullptr;
        }

        data[len] = '\0';
        fclose(fp);
        LOG_TRACE("'%s' File loaded successfully", filePath);
    }

    return data;
}

u8* FileLoadBinary(const char* filePath)
{
    u8* data = nullptr;
    FILE* fp = fopen(filePath, "rb");

    if (!fp) {
        LOG_ERROR("'%s' File is not valid", filePath);
        return nullptr;
    }

    if (fseek(fp, 0, SEEK_END) == 0) {
        i64 bufSize = -1;
        bufSize = ftell(fp);
        if (bufSize == -1) {
            LOG_ERROR("'%s' Failed to calculate file size", filePath);
            fclose(fp);
            return nullptr;
        }

        data = (u8*) SMalloc(bufSize * sizeof(u8), MEMORY_TAG_ARRAY);
        if (!data) {
            LOG_ERROR("'%s' Failed to allocate memory", filePath);
        }

        if (fseek(fp, 0, SEEK_SET) != 0) {
            LOG_ERROR("'%s' Failed to read file", filePath);
            fclose(fp);
            SFree(data);
            return nullptr;
        }

        fread(data, sizeof(char), bufSize, fp);
        if (ferror(fp) != 0) {
            LOG_ERROR("'%s' Failed to read file", filePath);
            fclose(fp);
            SFree(data);
            return nullptr;
        }

        fclose(fp);
        LOG_TRACE("'%s' File loaded successfully", filePath);
    }

    return data;
}

void FileUnload(void* data)
{
    SFree(data);
}

StringViewer FileGetExtension(const char* filePath)
{
    StringViewer result = { };

    u32 len = strlen(filePath);
    const char* s = filePath + len;
    result.data = s;
    for (; s != filePath; s--) {
        if (*s == '.') {
            result.data = s;
            result.length = (filePath + len) - s;
            break;
        }
    }

    return result;
}

StringViewer FileGetFileName(const char* filePath)
{
    StringViewer result = { };

    u32 len = strlen(filePath);
    const char* s = filePath + len;
    result.data = filePath;
    result.length = len;
    for (; s != filePath; s--) {
        if (*s == '/') {
            result.data = ++s;
            result.length = (filePath + len) - s;
            break;
        }
    }

    return result;
}

StringViewer FileGetFileNameNoExtension(const char* filePath)
{
    StringViewer result = { };

    u32 len = strlen(filePath);
    const char* s = filePath + len;
    result.data = filePath;
    result.length = len;

    u32 extensionLen = 0;
    for (; s != filePath; s--) {
        if (*s == '/') {
            result.data = ++s;
            result.length = (filePath + len) - s - extensionLen;
            break;
        } else if (*s == '.') {
            extensionLen = (filePath + len) - s;
        }
    }

    return result;
}

StringViewer FileGetPath(const char* filePath)
{
    StringViewer result = { };

    u32 len = strlen(filePath);
    const char* s = filePath + len;
    result.data = filePath;
    for (; s != filePath; s--) {
        if (*s == '/') {
            result.length = (s - filePath) + 1;
            break;
        }
    }

    return result;
}