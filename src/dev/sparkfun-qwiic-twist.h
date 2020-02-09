/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once
#include <stdint.h>
#include "i2c-if.h"

int16_t devSparkFunQwiicTwistGetCount(I2C_InterfaceId master);
