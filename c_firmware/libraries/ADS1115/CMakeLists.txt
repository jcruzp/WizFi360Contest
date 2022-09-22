aux_source_directory(. DIR_ADS1115_SRCS)


add_library(ADS1115 ${DIR_ADS1115_SRCS})

target_link_libraries(ADS1115 PUBLIC pico_stdlib
  hardware_i2c
)


