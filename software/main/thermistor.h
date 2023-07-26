#ifndef THERMISTOR_H
#define THERMISTOR_H


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
#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "defines.h"

struct mv_to_temp{
    float      ratio;
    int32_t     temp;
};

extern float mv_to_temp(uint32_t mv, struct mv_to_temp *table, uint32_t table_size);

extern float mv_to_temp_EPCOS_100K_B57560G104F(uint32_t mv);





#endif