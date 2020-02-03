/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <dev/led.h>

typedef enum
{
    RDBLED_NONE
    ,RDBLED_USER1
    ,RDBLED_USER2
    ,RDBLED_USER3
    ,RDBLED_USER4
    ,RDBLED_WIFISTATUS
    ,RDBLED_APPSTATUS
    ,RDBLED_MIN = RDBLED_USER1
    ,RDBLED_MAX = RDBLED_APPSTATUS
} RdbLed;

void devRdbLedSet(const RdbLed led, const LedChannel colorChannel, const LedState st);
void devRdbLedSetColor(const RdbLed led, const LedChannel colorMask);
