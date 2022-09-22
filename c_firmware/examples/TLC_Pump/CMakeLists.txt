set(TARGET_NAME TLC_Pump)

add_executable(${TARGET_NAME}
        ${TARGET_NAME}.c
        mqtt_thing.c
)

# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(${TARGET_NAME} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

target_include_directories(${TARGET_NAME} PUBLIC
        ${PORT_DIR}
        )

target_link_libraries(${TARGET_NAME} PRIVATE
        pico_stdlib
        cmsis_core
        CMSIS_FREERTOS_FILES
        hardware_dma
        FREERTOS_FILES
        TIMER_FILES
        WIZFI360_FILES
        IOT_SOCKET_FILES
        AWS_SDK_PORT_FILES
        TLC_Libs
)

pico_enable_stdio_usb(${TARGET_NAME} 1)
pico_enable_stdio_uart(${TARGET_NAME} 0)

pico_add_extra_outputs(${TARGET_NAME})

# Additionally generate python and hex pioasm outputs for inclusion in the RP2040 datasheet
add_custom_target(pio_ws2812_datasheet2 DEPENDS ${CMAKE_CURRENT_LIST_DIR}/generated/ws2812.py)
add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/ws2812.py
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio
        COMMAND Pioasm -o python ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio ${CMAKE_CURRENT_LIST_DIR}/generated/ws2812.py
        )
add_dependencies(${TARGET_NAME} pio_ws2812_datasheet2)