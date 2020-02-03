/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "sparkfun-qwiic-twist.h"
#define I2C_STRUCTS_VERSION 1
#include <applibs/i2c.h>
#include <hw/mt3620_rdb.h>
#include <unistd.h>

#define QWIIC_TWIST_ADDR 0x3F

int16_t devSparkFunQwiicTwistGetCount(void)
{
    int16_t count = 0;

    int fd = I2CMaster_Open(MT3620_ISU3_I2C);

    if (0 <= fd)
    {
        uint8_t reg = 0x5;
        I2CMaster_WriteThenRead(fd, QWIIC_TWIST_ADDR, &reg, sizeof(reg), (uint8_t*)&count, sizeof(count));
    }

    close(fd);

    return count;
}
