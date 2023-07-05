#include <cstdio>
#include "snowflake.h"

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

        data = (char*) malloc((bufSize + 1) * sizeof(char));
        if (!data) {
            LOG_ERROR("'%s' Failed to allocate memory", filePath);
        }

        if (fseek(fp, 0, SEEK_SET) != 0) {
            LOG_ERROR("'%s' Failed to read file", filePath);
            fclose(fp);
            free(data);
            return nullptr;
        }

        u64 len = fread(data, sizeof(char), bufSize, fp);
        if (ferror(fp) != 0) {
            LOG_ERROR("'%s' Failed to read file", filePath);
            fclose(fp);
            free(data);
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

        data = (u8*) malloc(bufSize * sizeof(u8));
        if (!data) {
            LOG_ERROR("'%s' Failed to allocate memory", filePath);
        }

        if (fseek(fp, 0, SEEK_SET) != 0) {
            LOG_ERROR("'%s' Failed to read file", filePath);
            fclose(fp);
            free(data);
            return nullptr;
        }

        u64 len = fread(data, sizeof(char), bufSize, fp);
        if (ferror(fp) != 0) {
            LOG_ERROR("'%s' Failed to read file", filePath);
            fclose(fp);
            free(data);
            return nullptr;
        }

        fclose(fp);
        LOG_TRACE("'%s' File loaded successfully", filePath);
    }

    return data;
}

void FileUnload(void* data)
{
    free(data);
}