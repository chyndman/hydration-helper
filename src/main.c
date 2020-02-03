/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <stdio.h>
#include <time.h>
#include "dev/rdb-leds.h"
#include "dev/sparkfun-qwiic-twist.h"
#include <applibs/log.h>

int main(void) {
    Log_Debug("Hello, world!");
    while (1)
    {
        // devSparkFunQwiicTwistGetCount()
        Log_Debug("Count %i", devSparkFunQwiicTwistGetCount());
        const struct timespec sleepTime = {1, 0};
        nanosleep(&sleepTime, NULL);
    }
}
