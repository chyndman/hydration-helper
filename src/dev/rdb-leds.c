/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "rdb-leds.h"
#include <applibs/gpio.h>
#include <hw/mt3620_rdb.h>
#include <unistd.h>

void devRdbLedSet(const RdbLed led, const LedChannel colorChannel, const LedState st)
{
    GPIO_Id id = -1;

    static const struct
    {
        RdbLed led;
        GPIO_Id idr;
        GPIO_Id idg;
        GPIO_Id idb;
    } map[] =
    {
        { RDBLED_USER1, MT3620_RDB_LED1_RED, MT3620_RDB_LED1_GREEN, MT3620_RDB_LED1_BLUE },
        { RDBLED_USER2, MT3620_RDB_LED2_RED, MT3620_RDB_LED2_GREEN, MT3620_RDB_LED2_BLUE },
        { RDBLED_USER3, MT3620_RDB_LED3_RED, MT3620_RDB_LED3_GREEN, MT3620_RDB_LED3_BLUE },
        { RDBLED_USER4, MT3620_RDB_LED4_RED, MT3620_RDB_LED4_GREEN, MT3620_RDB_LED4_BLUE },
        { RDBLED_WIFISTATUS, MT3620_RDB_NETWORKING_LED_RED, MT3620_RDB_NETWORKING_LED_GREEN, MT3620_RDB_NETWORKING_LED_BLUE },
        { RDBLED_APPSTATUS, MT3620_RDB_STATUS_LED_RED, MT3620_RDB_STATUS_LED_GREEN, MT3620_RDB_STATUS_LED_BLUE }
    };

    for (unsigned i = 0; i < sizeof(map) / sizeof(map[0]); i++)
    {
        if (map[i].led == led)
        {
            switch (colorChannel)
            {
                case LEDCH_RED:
                    id = map[i].idr;
                    break;
                case LEDCH_GREEN:
                    id = map[i].idg;
                    break;
                case LEDCH_BLUE:
                    id = map[i].idb;
                    break;
                default:
                    break;
            }
            break;
        }
    }

    if (-1 != id)
    {
        int fd = GPIO_OpenAsOutput(id, GPIO_OutputMode_PushPull, GPIO_Value_High);
        if (0 <= fd)
        {
            GPIO_SetValue(fd, (LEDST_ON == st ? GPIO_Value_Low : GPIO_Value_High));
        }
        close(fd);
    }
}

void devRdbLedSetColor(const RdbLed led, const LedChannel colorMask)
{
    const LedState r = (0 != (LEDCH_RED & colorMask) ? LEDST_ON : LEDST_OFF);
    const LedState g = (0 != (LEDCH_GREEN & colorMask) ? LEDST_ON : LEDST_OFF);
    const LedState b = (0 != (LEDCH_BLUE & colorMask) ? LEDST_ON : LEDST_OFF);
    devRdbLedSet(led, LEDCH_RED, r);
    devRdbLedSet(led, LEDCH_GREEN, g);
    devRdbLedSet(led, LEDCH_BLUE, b);
}
