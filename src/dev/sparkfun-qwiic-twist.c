/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "sparkfun-qwiic-twist.h"
#include "i2c-if.h"

static I2cDevice g_dev = DEF_I2C_DEVICE(0x3F, uint8_t);
static I2cRegister g_regCount = DEF_I2C_REG(&g_dev, 0x5, int16_t);
static I2cResource g_resRegCount = DEF_I2C_RES_REG(&g_regCount);

int16_t devSparkFunQwiicTwistGetCount(I2C_InterfaceId master)
{
    unsigned long val = 0;
    devI2cIfRead(master, g_resRegCount, &val);
    return (int16_t)(val);
}
