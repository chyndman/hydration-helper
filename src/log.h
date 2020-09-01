#pragma once

#include <applibs/log.h>

typedef enum
{
    LL_TRACE
    ,LL_DETAIL
    ,LL_INFO
    ,LL_WARN
    ,LL_ERROR
    ,LL_FATAL
} LogLevel;

#define LOG(LL, FMT, ...) Log_Debug("%s " FMT, logLevelStr(LL), __VA_ARGS__)

static const char* logLevelStr(const LogLevel ll)
{
    static const char* lvlStrs[] =
    {
        "[      ]",
        "[     \\]",
        "[    \\\\]",
        "[   \\\\\\]",
        "[  \\\\\\\\]",
        "[ \\\\\\\\\\]"
    };
    return lvlStrs[ll];
}
