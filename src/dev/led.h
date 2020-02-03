/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

typedef enum
{
    LEDCH_NONE = 0
    ,LEDCH_RED      = 0x1
    ,LEDCH_GREEN    = 0x2
    ,LEDCH_BLUE     = 0x4
} LedChannel;

typedef enum
{
    LEDST_NONE
    ,LEDST_OFF
    ,LEDST_ON
} LedState;
