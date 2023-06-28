#include <cstdio>
#include "snowflake.h"

char* LoadFile(const char* filePath)
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
        LOG_INFO("'%s' File loaded successfully", filePath);
    }

    return data;
}

SAPI void UnloadFile(char* data)
{
    free(data);
}