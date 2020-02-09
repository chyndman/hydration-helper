/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "i2c-if.h"
#include <errno.h>
#include <unistd.h>
#include <applibs/log.h>

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

    //Log_Debug("I2CMaster_WriteThenRead (Reg) 0x%08x [%u]\n", pReg->addr, pReg->width);
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

static int devI2cIfReadRegField(const int fd, const I2cRegisterField* pRegField, unsigned long* pVal)
{
    int status = devI2cIfReadReg(fd, pRegField->pReg, pVal);
    if (0 == status && NULL != pVal)
    {
        *pVal &= pRegField->mask;
        *pVal >>= pRegField->shift;
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
            case I2CRES_REGFLD:
                rv = devI2cIfReadRegField(fd, res.pRegField, pVal);
                break;
            default:
                break;
        }
    }
    return rv;
}

static int devI2cIfWriteReg(const int fd, const I2cRegister* pReg, unsigned long val)
{
    int status = -1;
    const I2cDevice* pDev = pReg->pDev;
    uint8_t buf[sizeof(unsigned long) * 2];

    for (unsigned i = 0; i < pDev->regAddrWidth; i++)
    {
        buf[i] = (pReg->addr >> (i * 8)) & 0xff;
    }
    for (unsigned i = 0; i < pReg->width; i++)
    {
        buf[i + pDev->regAddrWidth] = (val >> (i * 8)) & 0xff;
    }

    //Log_Debug("I2CMaster_Write (Reg) 0x%08x <- 0x%08x [%u]\n", pReg->addr, val, pReg->width);
    ssize_t rv = I2CMaster_Write(fd, pDev->addr, buf, pDev->regAddrWidth + pReg->width);
    if (0 <= rv)
    {
        status = 0;
    }

    return status;
}

static int devI2cIfWriteRegField(const int fd, const I2cRegisterField* pRegField, unsigned long val)
{
    unsigned long regVal = 0;
    int status = devI2cIfReadReg(fd, pRegField->pReg, &regVal);
    if (0 == status)
    {
        regVal &= ~( pRegField->mask );
        regVal |= ( val << pRegField->shift ) & pRegField->mask;
        status = devI2cIfWriteReg(fd, pRegField->pReg, regVal);
    }
    return status;
}

int devI2cIfWrite(const I2C_InterfaceId master, const I2cResource res, unsigned long val)
{
    int rv = -EPROTO;
    const int fd = devI2cIfActiveMasterAcquire(master);
    if (0 <= fd)
    {
        switch (res.resType)
        {
            case I2CRES_REG:
                rv = devI2cIfWriteReg(fd, res.pReg, val);
                break;
            case I2CRES_REGFLD:
                rv = devI2cIfWriteRegField(fd, res.pRegField, val);
            default:
                break;
        }
    }
    return rv;
}
