/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once
#include "i2c-if.h"

int devSparkFunQwiicScaleInitialize(I2C_InterfaceId master);
int devSparkFunQwiicScaleReadingGet(I2C_InterfaceId master, int32_t* pReading);
