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
    lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, color, LPAT_SOLID);
    sleep(5);
    lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, 0, LPAT_SOLID);
    sleep((minutes * 60) - 35);
    lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, color, LPAT_BLINK_SLOW);
    sleep(30);
    lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, color, LPAT_STROBE);
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

static void* thdDoPomStateNone(void* pData)
{
    lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, LEDCH_RED, LPAT_SOLID);
    sleep(5);
    lightCtrlSet(LEDDEV_SPARKFUN_QWIIC_TWIST, MT3620_ISU3_I2C, 0, LPAT_SOLID);
    return NULL;
}

void pomodoroMain(void)
{
    PomodoroState states[] =
    {
        POMST_WORK,
        POMST_BREAK,
        POMST_NONE,
    };
    const unsigned numStates = sizeof(states) / sizeof(states[0]);
    int16_t prevCount = 0;
    while (1)
    {
        int16_t count = devSparkFunQwiicTwistGetCount(MT3620_ISU3_I2C);
        if (count != prevCount)
        {
            prevCount = count;
            const PomodoroState state = states[count % numStates];
            switch (state)
            {
                case POMST_WORK:
                    thdDo(thdPomStateWork, NULL);
                    break;
                case POMST_BREAK:
                    thdDo(thdPomStateBreak, NULL);
                    break;
                case POMST_NONE:
                    thdDo(thdDoPomStateNone, NULL);
                    break;
                default:
                    break;
            }
        }
    }
}
