#ifndef DEFINES_H
#define DEFINES_H


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>      /* wint_t */

#include <sys/unistd.h>
#include <sys/stat.h>

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
#include "esp_timer.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"




/* Littlevgl specific */
#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif
#include "lvgl_helpers.h"



#define GUI_SEMAPHORE_WAIT while(xSemaphoreTake(xGuiSemaphore, portMAX_DELAY) == pdFALSE){taskYIELD();}
#define LV_TICK_PERIOD_MS 1

#define STATUS_PIN      GPIO_NUM_2
#define HEATER_PIN      GPIO_NUM_27
#define FAN_PIN         GPIO_NUM_33

#define BUZZER_PIN      GPIO_NUM_22
#define UP_PIN          GPIO_NUM_35
#define DOWN_PIN        GPIO_NUM_32
#define SELECT_PIN      GPIO_NUM_34

#define HSPI_MISO   GPIO_NUM_12
#define HSPI_CLK    GPIO_NUM_14
#define HSPI_MOSI    GPIO_NUM_13

#define ADC_SAMPLES_AVG_ARRAY_SIZE 20



enum states{main_menu, reflow_menu, select_menu} state;



SemaphoreHandle_t xGuiSemaphore;

extern lv_indev_t * my_indev ;

extern volatile uint32_t LVGL_SETUP_COMPLETE;

extern volatile uint32_t MENUCHANGED_DISABLEBUTTONS;

struct reflow_phase{
    uint16_t start_temp;
    uint16_t finish_temp;
    uint32_t time;
};
struct reflow_profile{
    struct reflow_phase preheat;
    struct reflow_phase soak;
    struct reflow_phase reflow_ramp;;
    struct reflow_phase reflow;
    struct reflow_phase cooldown;
  
    char label[55];
};

extern struct reflow_profile * profiles;

extern uint32_t profile_selected;

extern uint32_t profiles_num;

extern volatile float internal_temp;
extern volatile float external_temp;


struct selection{
    char label[20];
    uint32_t value;

    lv_obj_t * associated_button;
    struct selection_items *items_array;
    uint32_t items_array_length;
    uint32_t selected_index;
};

struct selection_items{
    char label[45];
    uint32_t value;
    void * data;

    lv_obj_t *associated_button;
    struct selection *associated_selection;

};

struct reflow_params{
    uint32_t function;
    uint32_t sensor;
    uint32_t fans;
    //function == 0 REFLOW
    uint32_t heat;
    //function == 1 HEATING
    struct reflow_profile *profile;
};

extern volatile struct reflow_params *reflow_selected_params;

#define CONFIG_LIST_SIZE 6
extern struct selection *config_list;

extern void init_data();

extern float current_temp;
extern float target_temp;
extern uint32_t total_time_ms;
extern uint32_t elapsed_time_ms;


#endif