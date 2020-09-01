#pragma once

#define I2C_STRUCTS_VERSION 1
#include <applibs/i2c.h>
#include <stdint.h>

typedef struct
{
    I2C_DeviceAddress addr;
    unsigned regAddrWidth;
} I2cDevice;
#define DEF_I2C_DEVICE(A, RAW_INTTYPE) { .addr = A, .regAddrWidth = sizeof(RAW_INTTYPE) }

typedef struct
{
    I2cDevice* pDev;
    unsigned long addr;
    unsigned width;
} I2cRegister;
#define DEF_I2C_REG(D, A, W_INTTYPE) { .pDev = D, .addr = A, .width = sizeof(W_INTTYPE) }

typedef struct
{
    I2cRegister* pReg;
    unsigned long mask;
    unsigned shift;
} I2cRegisterField;
#define DEF_I2C_REGFLD(R, M, S) { .pReg = R, .mask = M, .shift = S }

typedef enum
{
    I2CRES_NONE = 0
    ,I2CRES_REG
    ,I2CRES_REGFLD
} I2cResourceType;

typedef struct
{
    I2cResourceType resType;
    union
    {
        I2cRegister* pReg;
        I2cRegisterField* pRegField;
    };
} I2cResource;

#define DEF_I2C_RES_REG(R)          { .resType = I2CRES_REG,    .pReg = R }
#define DEF_I2C_RES_REGFLD(R)       { .resType = I2CRES_REGFLD, .pRegField = R }

int devI2cIfRead(const I2C_InterfaceId master, const I2cResource res, unsigned long* pVal);
int devI2cIfWrite(const I2C_InterfaceId master, const I2cResource res, unsigned long val);
