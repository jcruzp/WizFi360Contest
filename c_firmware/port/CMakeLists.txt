# ioLibrary_Driver


# timer
add_library(TIMER_FILES STATIC)

target_sources(TIMER_FILES PUBLIC
        ${PORT_DIR}/timer/timer.c
        )

target_include_directories(TIMER_FILES PUBLIC
        ${PORT_DIR}/timer
        )

target_link_libraries(TIMER_FILES PRIVATE
        pico_stdlib      
        )

# WizFi360
add_library(WIZFI360_FILES STATIC)

target_sources(WIZFI360_FILES PUBLIC
        ${PORT_DIR}/WizFi360/src/UART_RP2040.c
        )

target_include_directories(WIZFI360_FILES PUBLIC
        ${CMSIS_5_DIR}/CMSIS/Driver/Include
        ${PORT_DIR}/WizFi360/inc
        ${PORT_DIR}/RTE/inc
        )

target_link_libraries(WIZFI360_FILES PRIVATE
        pico_stdlib
        cmsis_core
        hardware_uart
        CMSIS_FREERTOS_FILES
        WIZFI360_DRIVER_FILES
        FREERTOS_FILES
        )

# aws-iot-device-sdk-embedded-C
add_library(AWS_SDK_PORT_FILES STATIC)

target_sources(AWS_SDK_PORT_FILES PUBLIC
        #${PORT_DIR}/aws-iot-device-sdk-embedded-C/src/http_transport_interface.c
        ${PORT_DIR}/aws-iot-device-sdk-embedded-C/src/mqtt_transport_interface.c
        ${PORT_DIR}/aws-iot-device-sdk-embedded-C/src/ssl_transport_interface.c
        ${PORT_DIR}/aws-iot-device-sdk-embedded-C/src/timer_interface.c
        ${PORT_DIR}/aws-iot-device-sdk-embedded-C/src/util.c
        )

target_include_directories(AWS_SDK_PORT_FILES PUBLIC
        ${PORT_DIR}/aws-iot-device-sdk-embedded-C/inc
        ${CMSIS_5_DIR}/CMSIS/Driver/Include
        ${MBEDTLS_DIR}/include
        )

target_link_libraries(AWS_SDK_PORT_FILES PUBLIC
        pico_stdlib
        cmsis_core
        AWS_SDK_FILES
        mbedcrypto
        mbedx509
        mbedtls
        WIZFI360_FILES
        IOT_SOCKET_FILES
        )