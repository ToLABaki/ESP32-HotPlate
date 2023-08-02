#include "adc081s.h"


spi_device_handle_t handle;



spi_device_interface_config_t adc_cfg1={
    .command_bits       = 0,
    .address_bits       = 0,
    .dummy_bits         = 0,
    .clock_speed_hz     = 1000000,
    .duty_cycle_pos     = 128,        //50% duty cycle
    .mode               = 2,
    .spics_io_num       = ADC1_CS,
    .cs_ena_posttrans   = 1,        //Keep the CS low 3 cycles after transaction, to stop slave from missing the last bit when CS has less propagation delay than CLK
    .queue_size         = 1
};

spi_device_interface_config_t adc_cfg2={
    .command_bits       = 0,
    .address_bits       = 0,
    .dummy_bits         = 0,
    .clock_speed_hz     = 1000000,
    .duty_cycle_pos     = 128,        //50% duty cycle
    .mode               = 2,
    .spics_io_num       = ADC2_CS,
    .cs_ena_posttrans   = 1,        //Keep the CS low 3 cycles after transaction, to stop slave from missing the last bit when CS has less propagation delay than CLK
    .queue_size         = 1
};

esp_err_t adc081s_init(){
    esp_err_t ret = ESP_OK;
    ret = spi_bus_add_device(HSPI_HOST, &adc_cfg1, &handle);
    if(ret != ESP_OK){
        printf("SPI ERR\n");
        return ret;
    }
    printf("ADC081s DONE\n");
    return ret;
}


esp_err_t adc081s_get_val(uint16_t *val){
    uint8_t receive_data[2];
    spi_transaction_t t= {
        .tx_buffer = NULL,
        .rx_buffer = receive_data,
        .length = 2*8,
        .rxlength = 2*8
    };
    if(spi_device_transmit(handle, &t) != ESP_OK){
        printf("SPI ERR\n");
        return ESP_FAIL;
    }
    

    //change the bit shift values if a genuine part is used, according to the datasheet
    *val = (receive_data[0]<<4) | (receive_data[1]>>4);

    

    return ESP_OK;
}

uint16_t adc081s_val_to_mv(uint16_t val){
    return ((float)((float)((float)val*3.3))/255)*1000;
}