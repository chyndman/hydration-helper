/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "dev/rdb-leds.h"
#include "dev/sparkfun-qwiic-twist.h"
#include "dev/sparkfun-qwiic-scale.h"
#include <applibs/log.h>
#include <hw/mt3620_rdb.h>
#include <time.h>

int main(void)
{
    Log_Debug("Hello, world!");
    int status = devSparkFunQwiicScaleInitialize(MT3620_ISU3_I2C);
    Log_Debug("status %d", status);
    while (1)
    {
        int32_t reading = 0;
        if (0 == devSparkFunQwiicScaleReadingGet(MT3620_ISU3_I2C, &reading))
        {
            devRdbLedSetColor(RDBLED_USER1, (uint32_t)reading & 0x7);
            devRdbLedSetColor(RDBLED_USER2, ((uint32_t)reading >> 3) & 0x7);
            devRdbLedSetColor(RDBLED_USER3, ((uint32_t)reading >> 6) & 0x7);
            devRdbLedSetColor(RDBLED_USER4, ((uint32_t)reading >> 9) & 0x7);
        }
    }
}
