/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "light-ctrl.h"
#include <pthread.h>
#include <time.h>
#include "dev/led.h"
#include "dev/rdb-leds.h"
#include "dev/sparkfun-qwiic-twist.h"

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
    struct timespec t =
    {
        1,
        0
    };

    while (1)
    {
        ledUpdate(g_blinkContext.dev, g_blinkContext.led, g_blinkContext.color);
        nanosleep(&t, NULL);
        ledUpdate(g_blinkContext.dev, g_blinkContext.led, 0);
        nanosleep(&t, NULL);
    }

    return NULL;
}

void lightCtrlSet(const LedDevice dev, const int led, const LedChannel color, const LightPattern pat)
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
            thdDo(thdBlinkSlow, NULL);
            break;
        default:
            break;
    }
}