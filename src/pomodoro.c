/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "pomodoro.h"
#include "light-ctrl.h"
#include "dev/sparkfun-qwiic-twist.h"
#include <hw/mt3620_rdb.h>
#include <unistd.h>
#include <pthread.h>

#define POMODORO_WORK_MINUTES       25
#define POMODORO_BREAK_MINUTES      5

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

static void doPomState(const unsigned minutes, const LedChannel color)
{
    lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, color, LPAT_SOLID, 0);
    sleep(5);
    lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, 0, LPAT_SOLID, 0);
    sleep((minutes * 60) - 35);
    lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, color, LPAT_BLINK_SLOW, 0);
    sleep(30);
    lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, color, LPAT_STROBE, 0);
}

static void* thdPomStateWork(void* pData)
{
    doPomState(POMODORO_WORK_MINUTES, LEDCH_BLUE);
    return NULL;
}

static void* thdPomStateBreak(void* pData)
{
    doPomState(POMODORO_BREAK_MINUTES, LEDCH_GREEN);
    return NULL;
}

static void* thdPomStateGame(void* pData)
{
    unsigned long quanta = 0;
    const unsigned quantaLenMinutes = 5;
    const LedChannel color = LEDCH_RED | LEDCH_GREEN;

    while (1)
    {
        lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, color, LPAT_SOLID, 0);
        sleep(5);
        lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, 0, LPAT_SOLID, 0);
        sleep(1);
        lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, color, LPAT_STROBE, ++quanta);
        sleep((60 * quantaLenMinutes) - 6);
    }

    return NULL;
}

static void* thdPomStateNone(void* pData)
{
    lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, LEDCH_RED, LPAT_SOLID, 0);
    sleep(5);
    lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, 0, LPAT_SOLID, 0);
    return NULL;
}

void pomodoroMain(void)
{
    struct
    {
        PomodoroState state;
        void* (*pThdFunc)(void*);
    } map[] =
    {
        { POMST_NONE, &thdPomStateNone },
        { POMST_WORK, &thdPomStateWork },
        { POMST_BREAK, &thdPomStateBreak },
        { POMST_GAME, &thdPomStateGame },
    };

    lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, LEDCH_RED | LEDCH_GREEN | LEDCH_BLUE, LPAT_BLINK_SLOW, 0);

    int16_t prevCount = devSparkFunQwiicTwistGetCount(MT3620_ISU3_I2C);
    while (1)
    {
        int16_t count = devSparkFunQwiicTwistGetCount(MT3620_ISU3_I2C);
        if (count != prevCount)
        {
            prevCount = count;
            const unsigned i = count % (sizeof(map) / sizeof(map[0]));
            thdDo(map[i].pThdFunc, NULL);
        }
    }
}
