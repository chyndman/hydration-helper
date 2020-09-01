cmake_minimum_required(VERSION 3.10)
project(hydration-helper C)

add_executable(
    ${PROJECT_NAME}
    src/main.c
    src/dev/rdb-leds.c
    src/dev/i2c-if.c
    src/dev/sparkfun-qwiic-twist.c
    src/dev/sparkfun-qwiic-scale.c
    src/light-ctrl.c
    src/pomodoro.c
)

include_directories(src)
target_link_libraries(${PROJECT_NAME} applibs gcc_s pthread c)

azsphere_configure_tools(TOOLS_REVISION "20.04")
azsphere_configure_api(TARGET_API_SET "5")
azsphere_target_hardware_definition(${PROJECT_NAME} TARGET_DIRECTORY "contrib/hw/mt3620_rdb" TARGET_DEFINITION "mt3620_rdb.json")

azsphere_target_add_image_package(${PROJECT_NAME})
