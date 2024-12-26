cmake_minimum_required(VERSION 3.13)

set(PICO_PLATFORM rp2350-arm-s)
set(PICO_BOARD pimoroni_pico_plus2_rp2350)

set(PICO_COPY_TO_RAM OFF)
set(PIMORONI_PICO_FETCH_FROM_GIT true)

# Change your executable name to something creative!
set(NAME presto-boilerplate) # <-- Name your project/executable here!

# the following enables the system malloc/free to be wrapped
set(SKIP_PICO_MALLOC 1)

# the following enables wrapping in sparkfun_pico builds
set(SFE_PICO_ALLOC_WRAP 1)

# the following enables the system malloc/free to be wrapped during compilation
add_definitions(-DSFE_PICO_ALLOC_WRAP -DSFE_RP2350_XIP_CSI_PIN=47)

include(pimoroni_pico_import.cmake)
include(pico_sdk_import.cmake)

# Gooey boilerplate
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_subdirectory(sparkfun-pico/sparkfun_pico)

add_subdirectory(drivers/st7701)
add_subdirectory(drivers/ft6x36)

set(LV_CONF_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h
    CACHE STRING "" FORCE)

set(LV_LVGL_H_INCLUDE_SIMPLE ON)

include_directories(lvgl)

add_subdirectory(lvgl)

# Download and install SquareLineStudio and create a UI project, 480 x 480 pixel 16 bit
# Specify its path here
#add_subdirectory(SquareLineWorkspace/SquareLine_Project/ui)

# Add your source files
add_executable(${NAME}
    src/main.cpp
    #    src/rp2_psram.c
    #    src/rp2_psram.h
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
    sparkfun_pico
    #ui # <-- Uncomment this if using SquareLineStudio
)

# Enable USB UART output only
pico_enable_stdio_uart(${NAME} 0)
pico_enable_stdio_usb(${NAME} 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})
