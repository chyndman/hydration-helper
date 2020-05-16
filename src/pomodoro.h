/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "light-ctrl.h"
#include "dev/rdb-leds.h"
#include <hw/mt3620_rdb.h>
#include <time.h>

typedef enum
{
    POMST_NONE
    ,POMST_WORK
    ,POMST_BREAK
    ,POMST_GAME
} PomodoroState;

void pomodoroMain(void);
