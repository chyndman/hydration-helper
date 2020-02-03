/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <stdio.h>
#include <time.h>
#include "dev/rdb-leds.h"

int main(void) {
    printf("Hello, world!\n");
    while (1)
    {
        const struct timespec sleepTime = {0, 80000000};
        for (LedChannel c = 0; c <= 7; c++)
        {
            for (RdbLed led = RDBLED_MIN; led <= RDBLED_MAX; led++)
            {
                devRdbLedSetColor(led, c);
                nanosleep(&sleepTime, NULL);
                devRdbLedSetColor(led, 0);
            }
        }
    }
    while (1)
    {
        // Empty
    }
}
