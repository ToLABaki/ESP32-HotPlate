#ifndef ADC081S_H
#define ADC081S_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "driver/spi_master.h"

#include "defines.h"



#define ADC1_CS     16
#define ADC2_CS     17

extern spi_device_interface_config_t adc_cfg1;
extern spi_device_interface_config_t adc_cfg2;

extern spi_device_handle_t handle1;
extern spi_device_handle_t handle2;

esp_err_t adc081s_init(spi_device_interface_config_t *adc_cfg , spi_device_handle_t *handle);

esp_err_t adc081s_get_val(spi_device_handle_t *handle, uint16_t *val);

uint16_t adc081s_val_to_mv(uint16_t val);

uint32_t adc081s_test(spi_device_handle_t *handle);



#endif