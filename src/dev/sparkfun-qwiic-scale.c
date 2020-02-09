/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "sparkfun-qwiic-scale.h"
#include "i2c-if.h"
#include <time.h>
#include <applibs/log.h>

static I2cDevice g_dev =                    DEF_I2C_DEVICE(0x2A, uint8_t);
static I2cRegister g_regPuCtrl =                DEF_I2C_REG(&g_dev, 0x00, uint8_t);
static I2cRegisterField g_rfRegReset =              DEF_I2C_REGFLD(&g_regPuCtrl, 0x01, 0);
static I2cRegisterField g_rfPwrUpDigital =          DEF_I2C_REGFLD(&g_regPuCtrl, 0x02, 1);
static I2cRegisterField g_rfPwrUpAnalog =           DEF_I2C_REGFLD(&g_regPuCtrl, 0x04, 2);
static I2cRegisterField g_rfPwrUpReady =            DEF_I2C_REGFLD(&g_regPuCtrl, 0x08, 3);
static I2cRegister g_regCtrl1 =                 DEF_I2C_REG(&g_dev, 0x01, uint8_t);
static I2cRegisterField g_rfCtrlLdoVoltage =        DEF_I2C_REGFLD(&g_regCtrl1, 0x38, 3);
static I2cRegisterField g_rfCtrlGainSelect =        DEF_I2C_REGFLD(&g_regCtrl1, 0x07, 0);
static I2cRegister g_regCtrl2 =                 DEF_I2C_REG(&g_dev, 0x02, uint8_t);
static I2cRegisterField g_rfCtrlRateSelect =        DEF_I2C_REGFLD(&g_regCtrl2, 0x70, 4);
static I2cRegisterField g_rfCtrlCal =               DEF_I2C_REGFLD(&g_regCtrl2, 0x04, 2);
static I2cRegisterField g_rfCtrlCalErr =            DEF_I2C_REGFLD(&g_regCtrl2, 0x08, 3);
static I2cRegister g_regAdc =                   DEF_I2C_REG(&g_dev, 0x15, uint8_t);
static I2cRegister g_regPwrCtrl =               DEF_I2C_REG(&g_dev, 0x1c, uint8_t);
static I2cRegisterField g_rfPwrPgaCapEnable =       DEF_I2C_REGFLD(&g_regPwrCtrl, 0x80, 7);

static I2cResource g_resRfRegReset =                    DEF_I2C_RES_REGFLD(&g_rfRegReset);
static I2cResource g_resRfPwrUpDigital =                DEF_I2C_RES_REGFLD(&g_rfPwrUpDigital);
static I2cResource g_resRfPwrUpAnalog =                 DEF_I2C_RES_REGFLD(&g_rfPwrUpAnalog);
static I2cResource g_resRfPwrUpReady =                  DEF_I2C_RES_REGFLD(&g_rfPwrUpReady);
static I2cResource g_resRfCtrlLdoVoltage =              DEF_I2C_RES_REGFLD(&g_rfCtrlLdoVoltage);
static I2cResource g_resRfCtrlGainSelect =              DEF_I2C_RES_REGFLD(&g_rfCtrlGainSelect);
static I2cResource g_resRfCtrlRateSelect =              DEF_I2C_RES_REGFLD(&g_rfCtrlRateSelect);
static I2cResource g_resRegAdc =                        DEF_I2C_RES_REG(&g_regAdc);
static I2cResource g_resRfPwrPgaCapEnable =             DEF_I2C_RES_REGFLD(&g_rfPwrPgaCapEnable);
static I2cResource g_resRfCtrlCal =                     DEF_I2C_RES_REGFLD(&g_rfCtrlCal);
static I2cResource g_resRfCtrlCalErr =                  DEF_I2C_RES_REGFLD(&g_rfCtrlCalErr);

#define LDO_VOLTAGE_3PT3    0x4
#define GAIN_SELECT_X128    0x7
#define RATE_SELECT_80HZ    0x3
#define ADC_QWIIC_DEFAULT   0x30

static int devSparkFunQwiicScaleAfeCalibrate(I2C_InterfaceId master)
{
    int status = devI2cIfWrite(master, g_resRfCtrlCal, 1);
    if (0 == status)
    {
        unsigned long cal = 1;
        do
        {
            status = devI2cIfRead(master, g_resRfCtrlCal, &cal);
        } while (0 == status && 1 == cal);
    }
    if (0 == status)
    {
        unsigned long calErr = 0;
        status = devI2cIfRead(master, g_resRfCtrlCalErr, &calErr);
        if (0 == status && 0 != calErr)
        {
            status = -1;
        }
    }
    return status;
}

int devSparkFunQwiicScaleInitialize(I2C_InterfaceId master)
{
    int status = devI2cIfWrite(master, g_resRfRegReset, 1);
    if (0 == status)
    {
        const struct timespec resetWaitTime = { 0, 1000000 };
        nanosleep(&resetWaitTime, NULL);
        status = devI2cIfWrite(master, g_resRfRegReset, 0);
    }
    if (0 == status)
    {
        status = devI2cIfWrite(master, g_resRfPwrUpDigital, 1);
    }
    if (0 == status)
    {
        status = devI2cIfWrite(master, g_resRfPwrUpAnalog, 1);
    }
    if (0 == status)
    {
        unsigned long rdy = 0;
        do
        {
            status = devI2cIfRead(master, g_resRfPwrUpReady, &rdy);
        } while (0 == status && 0 == rdy);
    }
    if (0 == status)
    {
        status = devI2cIfWrite(master, g_resRfCtrlLdoVoltage, LDO_VOLTAGE_3PT3);
    }
    if (0 == status)
    {
        status = devI2cIfWrite(master, g_resRfCtrlGainSelect, GAIN_SELECT_X128);
    }
    if (0 == status)
    {
        status = devI2cIfWrite(master, g_resRfCtrlRateSelect, RATE_SELECT_80HZ);
    }
    if (0 == status)
    {
        status = devI2cIfWrite(master, g_resRegAdc, ADC_QWIIC_DEFAULT);
    }
    if (0 == status)
    {
        status = devI2cIfWrite(master, g_resRfPwrPgaCapEnable, 1);
    }
    if (0 == status)
    {
        status = devSparkFunQwiicScaleAfeCalibrate(master);
    }

    return status;
}
