aux_source_directory(. DIR_TLC_Libs_SRCS)

add_library(TLC_Libs ${DIR_TLC_Libs_SRCS})
target_include_directories(TLC_Libs PUBLIC
        ${PORT_DIR}
        )

target_link_libraries(TLC_Libs PUBLIC pico_stdlib
        hardware_i2c  
        hardware_pio
        pico_stdlib
        cmsis_core
        CMSIS_FREERTOS_FILES
        hardware_dma
        FREERTOS_FILES
        TIMER_FILES
        WIZFI360_FILES
        IOT_SOCKET_FILES
        AWS_SDK_PORT_FILES
)



