#include "sd_spi.h"


sdmmc_host_t host_config = {\
    .flags = SDMMC_HOST_FLAG_SPI | SDMMC_HOST_FLAG_DEINIT_ARG, \
    .slot = SDSPI_DEFAULT_HOST, \
    .max_freq_khz = 500, \
    .io_voltage = 3.3f, \
    .init = &sdspi_host_init, \
    .set_bus_width = NULL, \
    .get_bus_width = NULL, \
    .set_bus_ddr_mode = NULL, \
    .set_card_clk = &sdspi_host_set_card_clk, \
    .do_transaction = &sdspi_host_do_transaction, \
    .deinit_p = &sdspi_host_remove_device, \
    .io_int_enable = &sdspi_host_io_int_enable, \
    .io_int_wait = &sdspi_host_io_int_wait, \
    .command_timeout_ms = 0, \
};


sdspi_dev_handle_t handle;
sdmmc_card_t *sdcard_info;
esp_vfs_fat_mount_config_t mount_config;

sdspi_device_config_t slot_config = {\
    .host_id   = SDSPI_DEFAULT_HOST, \
    .gpio_cs   = SD_CS, \
    .gpio_cd   = SDSPI_SLOT_NO_CD, \
    .gpio_wp   = SDSPI_SLOT_NO_WP, \
    .gpio_int  = GPIO_NUM_NC, \
};



esp_err_t sd_init(){
    esp_err_t ret = esp_vfs_fat_sdspi_mount("/basepath", &host_config, &slot_config, &mount_config, &sdcard_info);
    if(ret != ESP_OK){
        printf("SD SPI ERR\n");
        
    }
   
    return ret;

}