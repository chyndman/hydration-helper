#include "sparkfun-qwiic-twist.h"
#include "i2c-if.h"
#include <stdint.h>

static I2cDevice g_dev = DEF_I2C_DEVICE(0x3F, uint8_t);
static I2cRegister g_regCount = DEF_I2C_REG(&g_dev, 0x5, int16_t);
static I2cResource g_resRegCount = DEF_I2C_RES_REG(&g_regCount);
static I2cRegister g_regRed = DEF_I2C_REG(&g_dev, 0xD, uint8_t);
static I2cResource g_resRegRed = DEF_I2C_RES_REG(&g_regRed);
static I2cRegister g_regGreen = DEF_I2C_REG(&g_dev, 0xE, uint8_t);
static I2cResource g_resRegGreen = DEF_I2C_RES_REG(&g_regGreen);
static I2cRegister g_regBlue = DEF_I2C_REG(&g_dev, 0xF, uint8_t);
static I2cResource g_resRegBlue = DEF_I2C_RES_REG(&g_regBlue);

int16_t devSparkFunQwiicTwistGetCount(const I2C_InterfaceId master)
{
    unsigned long val = 0;
    devI2cIfRead(master, g_resRegCount, &val);
    return (int16_t)(val);
}

void devSparkFunQwiicTwistLedSet(const I2C_InterfaceId master, const LedChannel colorChannel, const LedState st)
{
    I2cResource* pReg = NULL;
    switch (colorChannel)
    {
        case LEDCH_RED:
            pReg = &g_resRegRed;
            break;
        case LEDCH_GREEN:
            pReg = &g_resRegGreen;
            break;
        case LEDCH_BLUE:
            pReg = &g_resRegBlue;
            break;
        default:
            break;
    }
    if (NULL != pReg)
    {
        devI2cIfWrite(master, *pReg, (LEDST_ON == st ? 0xff : 0));
    }
}

void devSparkFunQwiicTwistLedSetColor(const I2C_InterfaceId master, const LedChannel colorMask)
{
    const LedState r = (0 != (LEDCH_RED & colorMask) ? LEDST_ON : LEDST_OFF);
    const LedState g = (0 != (LEDCH_GREEN & colorMask) ? LEDST_ON : LEDST_OFF);
    const LedState b = (0 != (LEDCH_BLUE & colorMask) ? LEDST_ON : LEDST_OFF);
    devSparkFunQwiicTwistLedSet(master, LEDCH_RED, r);
    devSparkFunQwiicTwistLedSet(master, LEDCH_GREEN, g);
    devSparkFunQwiicTwistLedSet(master, LEDCH_BLUE, b);
}

