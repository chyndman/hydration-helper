#pragma once

typedef enum
{
    LEDCH_NONE = 0
    ,LEDCH_RED      = 0x1
    ,LEDCH_GREEN    = 0x2
    ,LEDCH_BLUE     = 0x4
} LedChannel;

typedef enum
{
    LEDST_NONE
    ,LEDST_OFF
    ,LEDST_ON
} LedState;

typedef enum
{
    LEDDEV_NONE
    ,LEDDEV_SPARKFUN_QWIIC_TWIST
    ,LEDDEV_RDB
} LedDevice;
