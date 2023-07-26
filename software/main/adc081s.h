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

#define HSPI_MISO   12
#define HSPI_CLK    14
#define HSPI_CS     16

esp_err_t adc081s_init();

esp_err_t adc081s_get_val(uint16_t *val);

uint16_t adc081s_val_to_mv(uint16_t val);



#endif