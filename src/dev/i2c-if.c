/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "i2c-if.h"
#include <errno.h>
#include <unistd.h>

static int devI2cIfActiveMasterAcquire(const I2C_InterfaceId idNew)
{
    static int fd = -1;
    static I2C_InterfaceId id = -1;

    if (id != idNew)
    {
        if (0 <= fd)
        {
            close(fd);
        }
        fd = -1;
        id = -1;
    }

    if (-1 != idNew && -1 == fd)
    {
        fd = I2CMaster_Open(idNew);
        id = idNew;
    }

    return fd;
}

static int devI2cIfReadReg(const int fd, const I2cRegister* pReg, unsigned long* pVal)
{
    int status = -1;
    const I2cDevice* pDev = pReg->pDev;
    uint8_t bufReg[sizeof(unsigned long)];
    uint8_t bufVal[sizeof(unsigned long)];

    for (unsigned i = 0; i < pDev->regAddrWidth; i++)
    {
        bufReg[i] = (pReg->addr >> (i * 8)) & 0xff;
    }

    ssize_t rv = I2CMaster_WriteThenRead(fd, pDev->addr, bufReg, pDev->regAddrWidth, bufVal, pReg->width);
    if (0 <= rv)
    {
        status = 0;
        if (NULL != pVal)
        {
            *pVal = 0;
            for (unsigned i = 0; i < pReg->width; i++)
            {
                *pVal |= ((unsigned long)bufVal[i] << (i * 8));
            }
        }
    }

    return status;
}

int devI2cIfRead(const I2C_InterfaceId master, const I2cResource res, unsigned long* pVal)
{
    int rv = -EPROTO;
    const int fd = devI2cIfActiveMasterAcquire(master);
    if (0 <= fd)
    {
        switch (res.resType)
        {
            case I2CRES_REG:
                rv = devI2cIfReadReg(fd, res.pReg, pVal);
                break;
            default:
                break;
        }
    }
    return rv;
}
