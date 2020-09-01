#pragma once

#include "dev/led.h"

typedef enum
{
    LPAT_NONE
    ,LPAT_SOLID
    ,LPAT_BLINK_SLOW
    ,LPAT_STROBE
} LightPattern;

void lightCtrlSet(const LedDevice dev, const int led, const LedChannel color, const LightPattern pat, const unsigned count);
