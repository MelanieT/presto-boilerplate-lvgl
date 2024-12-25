cmake_minimum_required(VERSION 3.13)

set(PICO_PLATFORM rp2350-arm-s)
set(PICO_BOARD pimoroni_pico_plus2_rp2350)

set(PIMORONI_PICO_FETCH_FROM_GIT true)

# Change your executable name to something creative!
set(NAME presto-boilerplate) # <-- Name your project/executable here!

include(pimoroni_pico_import.cmake)
include(pico_sdk_import.cmake)

# Gooey boilerplate
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_subdirectory(drivers/st7701)
add_subdirectory(drivers/ft6x36)

set(LV_CONF_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h
    CACHE STRING "" FORCE)

set(LV_LVGL_H_INCLUDE_SIMPLE ON)

include_directories(lvgl)

add_subdirectory(lvgl)
add_subdirectory(SquareLineWorkspace/SquareLine_Project/ui)

# Add your source files
add_executable(${NAME}
    src/main.cpp # <-- Add source files here!
)

# Include required libraries
# This assumes `pimoroni-pico` is stored alongside your project
include(common/pimoroni_i2c)
include(common/pimoroni_bus)

# Don't forget to link the libraries you need!
target_link_libraries(${NAME}
    st7701_presto
    ft6x36
    pico_stdlib
    pico_multicore
    pimoroni_i2c
    hardware_interp
    lvgl::lvgl
    ui
)

# Enable USB UART output only
pico_enable_stdio_uart(${NAME} 0)
pico_enable_stdio_usb(${NAME} 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})
