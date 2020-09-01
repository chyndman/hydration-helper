#pragma once
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
