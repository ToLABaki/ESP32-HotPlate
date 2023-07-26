/* LVGL Example project
 *
 * Basic project to test LVGL on ESP32 based projects.
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */
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
#include "main_menu.h"
#include "config_menu.h"
#include "reflow_menu.h"

#include "adc.h"
#include "thermistor.h"




volatile uint32_t BTN_UP_SCORE       = 0x00;
volatile uint32_t BTN_UP_EVENT       = 0x00;
volatile uint32_t BTN_DOWN_SCORE     = 0x00;
volatile uint32_t BTN_DOWN_EVENT     = 0x00;
volatile uint32_t BTN_SELECT_SCORE   = 0x00;
volatile uint32_t BTN_SELECT_EVENT   = 0x00;

#define BUTTON_THERSHOLD (uint64_t)10000


static void lv_tick_task(void *arg);
static void guiTask(void *pvParameter);
static void menuLogicTask(void *pvParameter);
static void blink(void *pvParameter);

static const char *TAG = "example";

void init_fs(){
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s). Formatting...", esp_err_to_name(ret));
        esp_spiffs_format(conf.partition_label);
        return;
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    // Check consistency of reported partiton size info.
    if (used > total) {
        ESP_LOGW(TAG, "Number of used bytes cannot be larger than total. Performing SPIFFS_check().");
        ret = esp_spiffs_check(conf.partition_label);
        // Could be also used to mend broken files, to clean unreferenced pages, etc.
        // More info at https://github.com/pellepl/spiffs/wiki/FAQ#powerlosses-contd-when-should-i-run-spiffs_check
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
            return;
        } else {
            ESP_LOGI(TAG, "SPIFFS_check() successful");
        }
    }


        // Use POSIX and C standard library functions to work with files.
    // First create a file.
    ESP_LOGI(TAG, "Opening file");
    FILE* f = fopen("/spiffs/hello.txt", "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    fprintf(f, "Hello World!\n");
    fclose(f);
    ESP_LOGI(TAG, "File written");

    // Check if destination file exists before renaming
    struct stat st;
    if (stat("/spiffs/foo.txt", &st) == 0) {
        // Delete it if it exists
        unlink("/spiffs/foo.txt");
    }

    // Rename original file
    ESP_LOGI(TAG, "Renaming file");
    if (rename("/spiffs/hello.txt", "/spiffs/foo.txt") != 0) {
        ESP_LOGE(TAG, "Rename failed");
        return;
    }

    // Open renamed file for reading
    ESP_LOGI(TAG, "Reading file");
    f = fopen("/spiffs/foo.txt", "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    // strip newline
    char* pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);


    esp_vfs_spiffs_unregister(conf.partition_label);
    ESP_LOGI(TAG, "SPIFFS unmounted");
}

void init_gpio(){
    gpio_config_t io_conf_out = {}; gpio_config_t io_conf_in = {};
    io_conf_out.intr_type = GPIO_INTR_DISABLE;
    io_conf_out.mode = GPIO_MODE_OUTPUT;
    io_conf_out.pin_bit_mask = (1ULL<<STATUS_PIN) | (1ULL<<HEATER_PIN) | (1ULL<<FAN_PIN) | (1ULL<<BUZZER_PIN);
    io_conf_out.pull_down_en = 1;
    io_conf_out.pull_up_en = 0;
    gpio_config(&io_conf_out);

    io_conf_in.intr_type = GPIO_INTR_DISABLE;
    io_conf_in.mode = GPIO_MODE_INPUT;
    io_conf_in.pin_bit_mask = (1ULL<<UP_PIN) | (1ULL<<DOWN_PIN) | (1ULL<<SELECT_PIN);
    io_conf_in.pull_down_en = 1;
    io_conf_in.pull_up_en = 0;
    gpio_config(&io_conf_in);
}

bool get_buttons(lv_indev_drv_t *drv, lv_indev_data_t *data){
    static uint64_t prev_micros = 0;
    static uint64_t counter = 0;
    uint64_t diff = 0;
    uint64_t current_micros = esp_timer_get_time();


    if(counter != 0){
        if(current_micros < prev_micros){
            //overflow occured
            diff = current_micros + (0xFFFFFFFFFFFFFFFF - prev_micros);
        }else{
            diff = current_micros - prev_micros;
        }
    }


    if(gpio_get_level(SELECT_PIN) == 1){
        if(MENUCHANGED_DISABLEBUTTONS == 0){
            if((BTN_SELECT_SCORE < BUTTON_THERSHOLD) ){
                BTN_SELECT_SCORE += diff;
            }else{  
                data->key = LV_KEY_ENTER;
                data->state = LV_INDEV_STATE_PR;
            }
        }
        
    }else if(gpio_get_level(UP_PIN) == 1){
        if(MENUCHANGED_DISABLEBUTTONS == 0){
            if((BTN_UP_SCORE < BUTTON_THERSHOLD) ){
                BTN_UP_SCORE += diff;
            }else{
                data->key = LV_KEY_UP;
                data->state = LV_INDEV_STATE_PR;
            }
        }
    }else if(gpio_get_level(DOWN_PIN) == 1){
        if(MENUCHANGED_DISABLEBUTTONS == 0){
            if((BTN_DOWN_SCORE < BUTTON_THERSHOLD) ){
                BTN_DOWN_SCORE += diff;
            }else{
                data->key = LV_KEY_DOWN;
                data->state = LV_INDEV_STATE_PR;
            }
        }
    }else{
        BTN_DOWN_SCORE = 0;
        BTN_UP_SCORE = 0;
        BTN_SELECT_SCORE = 0;
        data->state = LV_INDEV_STATE_REL;
        data->key = 0;
        MENUCHANGED_DISABLEBUTTONS = 0;
    }

    counter++;
    if(counter == 1000000){
        counter = 1;
    }

    return false; /*No buffering now so no more data read*/
}

void app_main() {
    init_gpio();
    init_data();
    adc_init();
    adc081s_init();
    //init_fs();
    
    xGuiSemaphore = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(blink, "blink", 4096*2, NULL, tskIDLE_PRIORITY, NULL,0);
    xTaskCreatePinnedToCore(guiTask, "gui", 4096*2, NULL, 0, NULL, 1);
    xTaskCreatePinnedToCore(menuLogicTask, "menuLogic", 4096*2, NULL, tskIDLE_PRIORITY, NULL,0);
   
}







extern void _main_menu(enum states* state);

extern void _reflow_menu(enum states* state);

extern void _config_menu(enum states* state);

void(*actions[3])(enum states* state)={_main_menu, _reflow_menu, _config_menu};

void menuLogicTask(void *pvParameter){
    printf("Menu logic task started\n");
    enum states state = main_menu;
    while(1){
        actions[state](&state);
        
    }
    vTaskDelete(NULL);
}

static void guiTask(void *pvParameter) {

    (void) pvParameter;
    

    lv_init();

    /* Initialize SPI or I2C bus used by the drivers */
    lvgl_driver_init();

    lv_color_t* buf1 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1 != NULL);

    /* Use double buffered when not working with monochrome displays */
#ifndef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    lv_color_t* buf2 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2 != NULL);
#else
    static lv_color_t *buf2 = NULL;
#endif

    static lv_disp_buf_t disp_buf;

    uint32_t size_in_px = DISP_BUF_SIZE;

#if defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_IL3820         \
    || defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_JD79653A    \
    || defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_UC8151D     \
    || defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_SSD1306

    /* Actual size in pixels, not bytes. */
    size_in_px *= 8;
#endif

    /* Initialize the working buffer depending on the selected display.
     * NOTE: buf2 == NULL when using monochrome displays. */
    lv_disp_buf_init(&disp_buf, buf1, buf2, size_in_px);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_driver_flush;

#if defined CONFIG_DISPLAY_ORIENTATION_PORTRAIT || defined CONFIG_DISPLAY_ORIENTATION_PORTRAIT_INVERTED
    disp_drv.rotated = 1;
#endif

    /* When using a monochrome display we need to register the callbacks:
     * - rounder_cb
     * - set_px_cb */
#ifdef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    disp_drv.rounder_cb = disp_driver_rounder;
    disp_drv.set_px_cb = disp_driver_set_px;
#endif

    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);




    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);     
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = get_buttons;
    my_indev = lv_indev_drv_register(&indev_drv);


    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    


    init_styles();
    LVGL_SETUP_COMPLETE = 1;
     

    //vTaskDelay(pdMS_TO_TICKS(1000));
    while (1) {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));

        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
       }
    }

    /* A task should NEVER return */
    free(buf1);
#ifndef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    free(buf2);
#endif
    vTaskDelete(NULL);
}

static void lv_tick_task(void *arg) {
    (void) arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}

static void blink(void *pvParameter){
    while(1){
        //printf("TOGGLE\n");
        gpio_set_level(STATUS_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
        gpio_set_level(STATUS_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    vTaskDelete(NULL);
}
