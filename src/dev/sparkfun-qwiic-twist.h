#pragma once
#include <stdint.h>
#include "i2c-if.h"
#include "led.h"

int16_t devSparkFunQwiicTwistGetCount(const I2C_InterfaceId master);
void devSparkFunQwiicTwistLedSet(const I2C_InterfaceId master, const LedChannel colorChannel, const LedState st);
void devSparkFunQwiicTwistLedSetColor(const I2C_InterfaceId master, const LedChannel colorMask);
