# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.0.0)
project(hydration-helper C)

add_executable(
    hydration-helper
    src/main.c
    src/dev/rdb-leds.c
    src/dev/i2c-if.c
    src/dev/sparkfun-qwiic-twist.c
    src/dev/sparkfun-qwiic-scale.c
)
include_directories(src)
target_link_libraries(${PROJECT_NAME} applibs gcc_s c)

include("${AZURE_SPHERE_MAKE_IMAGE_FILE}")
