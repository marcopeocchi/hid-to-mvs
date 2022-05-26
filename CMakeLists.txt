cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from submodule
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(mvs-to-usb)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(${PROJECT_NAME}
    src/main.c
    src/hid_app.c
)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(${PROJECT_NAME} pico_stdlib tinyusb_host tinyusb_board)

target_include_directories(${PROJECT_NAME} PRIVATE 
    src
)

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)