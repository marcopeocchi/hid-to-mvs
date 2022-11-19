cmake_minimum_required(VERSION 3.13)

include($ENV{TINYUSB_PATH}/hw/bsp/family_support.cmake)

project(pico-hid-mvs)

# Checks this example is valid for the family and initializes the project
family_initialize_project(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR})

suppress_tinyusb_warnings()
add_executable(${PROJECT_NAME})

# Example source
target_sources(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/board_output.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hid_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
        )

# Example include
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

# Configure compilation flags and libraries for the example... see the corresponding function
# in hw/bsp/FAMILY/family.cmake for details.
family_configure_host_example(${PROJECT_NAME})

# For rp2040, un-comment to enable pico-pio-usb
family_add_pico_pio_usb(${PROJECT_NAME})