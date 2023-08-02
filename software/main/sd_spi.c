#include "sd_spi.h"

sdmmc_host_t host = SDSPI_HOST_DEFAULT();

esp_vfs_fat_sdmmc_mount_config_t mount_config = {
    .format_if_mount_failed = false,
    .max_files = 5,
    .allocation_unit_size = 16 * 1024
};
sdmmc_card_t *card;


sdspi_device_config_t slot_config = {\
    .host_id   = HSPI_HOST, \
    .gpio_cs   = SD_CS, \
    .gpio_cd   = SDSPI_SLOT_NO_CD, \
    .gpio_wp   = SDSPI_SLOT_NO_WP, \
    .gpio_int  = GPIO_NUM_NC, \
};

esp_err_t sd_init(){
    //sdspi_host_set_card_clk(handle, 1000);
    esp_err_t ret = esp_vfs_fat_sdspi_mount("/sdcard", &host, &slot_config, &mount_config, &card);
    if(ret != ESP_OK){
        printf("SD SPI ERR\n");
        return ret;
    }
    sdmmc_card_print_info(stdout, card);
    return ret;

}


esp_err_t sd_deinit(){
    //sdspi_host_set_card_clk(handle, 1000);
    esp_err_t ret = esp_vfs_fat_sdcard_unmount("/sdcard", card);
    if(ret != ESP_OK){
        printf("SD SPI RM ERR\n");
        return ret;
    }
    return ret;

}