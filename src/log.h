/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

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
