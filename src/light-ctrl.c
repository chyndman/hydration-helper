/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "light-ctrl.h"
#include <pthread.h>
#include <time.h>
#include "dev/led.h"
#include "dev/rdb-leds.h"
#include "dev/sparkfun-qwiic-twist.h"

#define NS_PER_MS 1000000

static void thdDo(void* (*pFun)(void*), void* pData)
{
    static pthread_t thd;
    static void* (*pCurrFun)(void*) = NULL;
    if (NULL != pCurrFun)
    {
        pthread_cancel(thd);
        pthread_join(thd, NULL);
    }
    pCurrFun = pFun;
    if (NULL != pFun)
    {
        pthread_create(&thd, NULL, pFun, pData);
    }
}

static void ledUpdate(const LedDevice dev, const int led, const LedChannel color)
{
    switch (dev)
    {
        case LEDDEV_SPARKFUN_QWIIC_TWIST:
            devSparkFunQwiicTwistLedSetColor(led, color);
            break;
        case LEDDEV_RDB:
            devRdbLedSetColor(led, color);
            break;
        default:
            break;
    }
}

static struct
{
    LedDevice dev;
    int led;
    LedChannel color;
} g_blinkContext;

static void* thdBlinkSlow(void* pData)
{
    unsigned count = (unsigned)pData;
    int infinite = (count == 0 ? 1 : 0);

    struct timespec t =
    {
        1,
        0
    };

    while (infinite || 0 < count--)
    {
        ledUpdate(g_blinkContext.dev, g_blinkContext.led, g_blinkContext.color);
        nanosleep(&t, NULL);
        ledUpdate(g_blinkContext.dev, g_blinkContext.led, 0);
        nanosleep(&t, NULL);
    }

    return NULL;
}

static void* thdStrobe(void* pData)
{
    unsigned count = (unsigned)pData;
    int infinite = (count == 0 ? 1 : 0);

    struct timespec tOn =
    {
        0,
        40 * NS_PER_MS
    };

    struct timespec tOff =
    {
        0,
        (500 * NS_PER_MS) - (tOn.tv_nsec)
    };

    while (infinite || 0 < count--)
    {
        ledUpdate(g_blinkContext.dev, g_blinkContext.led, g_blinkContext.color);
        nanosleep(&tOn, NULL);
        ledUpdate(g_blinkContext.dev, g_blinkContext.led, 0);
        nanosleep(&tOff, NULL);
    }

    return NULL;
}

void lightCtrlSet(const LedDevice dev, const int led, const LedChannel color, const LightPattern pat, const unsigned count)
{
    switch (pat)
    {
        case LPAT_SOLID:
            thdDo(NULL, NULL);
            ledUpdate(dev, led, color);
            break;
        case LPAT_BLINK_SLOW:
            g_blinkContext.dev = dev;
            g_blinkContext.led = led;
            g_blinkContext.color = color;
            thdDo(thdBlinkSlow, (void*)count);
            break;
        case LPAT_STROBE:
            g_blinkContext.dev = dev;
            g_blinkContext.led = led;
            g_blinkContext.color = color;
            thdDo(thdStrobe, (void*)count);
        default:
            break;
    }
}
