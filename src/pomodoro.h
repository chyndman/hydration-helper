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
