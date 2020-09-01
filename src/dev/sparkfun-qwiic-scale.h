#pragma once
#include "i2c-if.h"

int devSparkFunQwiicScaleInitialize(I2C_InterfaceId master);
int devSparkFunQwiicScaleReadingGet(I2C_InterfaceId master, int32_t* pReading);
