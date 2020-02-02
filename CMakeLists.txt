# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.0.0)
project(hydration-helper C)

add_executable(hydration-helper main.c)
target_link_libraries(${PROJECT_NAME} applibs gcc_s c)
