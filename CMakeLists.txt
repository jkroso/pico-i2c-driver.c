cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico-i2c-driver.c C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(pico-i2c-driver.c main.c)

pico_set_program_name(pico-i2c-driver.c "pico-i2c-driver.c")
pico_set_program_version(pico-i2c-driver.c "0.1")

pico_enable_stdio_usb(pico-i2c-driver.c 1)
pico_enable_stdio_uart(pico-i2c-driver.c 0)

# Add the standard library to the build
target_link_libraries(pico-i2c-driver.c pico_stdlib hardware_i2c)

pico_add_extra_outputs(pico-i2c-driver.c)
