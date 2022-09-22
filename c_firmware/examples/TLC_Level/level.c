/*****************************************************************************
* | File      	:   level.c
* | Author      :   Jose A. Cruz P.
* | Function    :
* | Info        :   ADS1115 for read voltaje from level control module
*----------------
* |	This version:   V1.0
* | Date        :   2022-09-01
* | Info        :
#******************************************************************************
/*
 * Copyright (c) 2021-2022 Antonio González
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* ADS1115 hello_ads1115 example

A differential signal is measured between pins A0 and A3. The measured value will be printed to stdout (raw value and voltage).
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "../libraries/ADS1115/ads1115.h"

#define I2C_PORT i2c0           // I2C0 Port
#define I2C_FREQ 400000         // I2C Frequency
#define ADS1115_I2C_ADDR 0x48   // ADS1115 address
#define SDA_PIN 8               // GP8
#define SCL_PIN 9               // GP8

struct ads1115_adc adc;

void init_level(void){
    
    // Initialise I2C
    i2c_init(I2C_PORT, I2C_FREQ);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C); 
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C); 
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Initialise ADC
    ads1115_init(I2C_PORT, ADS1115_I2C_ADDR, &adc);
    
    // Measured pin A0
    // Full-scale voltage range is set to +/- 6.144 V
    // 128 Samples per second
    ads1115_set_input_mux(ADS1115_MUX_SINGLE_0, &adc);
    ads1115_set_pga(ADS1115_PGA_6_144, &adc);
    ads1115_set_data_rate(ADS1115_RATE_128_SPS , &adc);

    // Write the configuration for this to have an effect.
    ads1115_write_config(&adc);  
    printf("TLC_Level ADS1115 init ...\r\n");  
}

float get_level(void) {
    // Data containers
    uint16_t adc_value;

    ads1115_read_adc(&adc_value, &adc);
    return ads1115_raw_to_volts(adc_value, &adc);
}

