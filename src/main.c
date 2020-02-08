/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "dev/rdb-leds.h"
#include "dev/sparkfun-qwiic-twist.h"
#include <applibs/log.h>

int main(void)
{
    Log_Debug("Hello, world!");
    while (1)
    {
        int16_t count = devSparkFunQwiicTwistGetCount();
        devRdbLedSetColor(RDBLED_USER1, (uint16_t)count & 0x7);
        devRdbLedSetColor(RDBLED_USER2, ((uint16_t)count >> 3) & 0x7);
        devRdbLedSetColor(RDBLED_USER3, ((uint16_t)count >> 6) & 0x7);
        devRdbLedSetColor(RDBLED_USER4, ((uint16_t)count >> 9) & 0x7);
    }
}
