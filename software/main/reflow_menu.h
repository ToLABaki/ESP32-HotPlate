#ifndef REFLOW_MENU_H
#define REFLOW_MENU_H

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


/* Littlevgl specific */
#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif
#include "lvgl_helpers.h"

#include "defines.h"

#include "styles.h"

#include "adc.h"
#include "thermistor.h"
#include "adc081s.h"

#include "reflow_menu_options.h"
#include "reflow_menu_error.h"
#include "reflow_menu_finished.h"


enum reflow_menu_states{reflowing, finished, options, error} reflow_menu_state;



extern void _reflow_menu(enum states* state);




#endif