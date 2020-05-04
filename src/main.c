/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "light-ctrl.h"
#include "dev/rdb-leds.h"
#include <hw/mt3620_rdb.h>
#include <applibs/log.h>
#include <time.h>

int main(void)
{
    Log_Debug("Hello, world!");
    lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, LEDCH_BLUE, LPAT_BLINK_SLOW);
    while (1)
    {
    }
}
