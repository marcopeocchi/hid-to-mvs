cmake_minimum_required(VERSION 3.13)

include($ENV{TINYUSB_PATH}/hw/bsp/family_support.cmake)

project(pico-hid-mvs)

family_initialize_project(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR})

suppress_tinyusb_warnings()
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/board_output.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hid_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

family_configure_host_example(${PROJECT_NAME})

family_add_pico_pio_usb(${PROJECT_NAME})