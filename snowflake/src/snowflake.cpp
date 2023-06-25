#include <cstdio>

#include "snowflake.h"
#include "sassert.h"
#include "logger.h"

bool SnowflakeInit()
{
    LOG_TRACE("Snowflake %d", 1);
    LOG_DEBUG("Snowflake %d", 2);
    LOG_INFO("Snowflake %d", 3);
    LOG_WARN("Snowflake %d", 4);
    LOG_ERROR("Snowflake %d", 5);
    LOG_FATAL("Snowflake %d", 6);

    getchar();

    return true;
}