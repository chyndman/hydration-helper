/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

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
