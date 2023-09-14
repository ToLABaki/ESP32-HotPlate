#include "adc081s.h"


spi_device_handle_t handle1;
spi_device_handle_t handle2;


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

esp_err_t adc081s_init(spi_device_interface_config_t *adc_cfg , spi_device_handle_t *handle){
    esp_err_t ret = ESP_OK;
    ret = spi_bus_add_device(HSPI_HOST, adc_cfg, handle);
    if(ret != ESP_OK){
        printf("SPI ERR\n");
        return ret;
    }
    printf("ADC081s DONE\n");
    return ret;
}


esp_err_t adc081s_get_val(spi_device_handle_t *handle, uint16_t *val){
    uint8_t receive_data[2];
    spi_transaction_t t= {
        .tx_buffer = NULL,
        .rx_buffer = receive_data,
        .length = 2*8,
        .rxlength = 2*8
    };
    if(spi_device_transmit(*handle, &t) != ESP_OK){
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

//returns:
//0 = OK
//1 = THERMISTOR ERROR
//2 = ADC ERROR
//3 = SPI ERROR
uint32_t adc081s_test(spi_device_handle_t *handle){
    uint8_t receive_data[2];
    spi_transaction_t t= {
        .tx_buffer = NULL,
        .rx_buffer = receive_data,
        .length = 2*8,
        .rxlength = 2*8
    };
    if(spi_device_transmit(*handle, &t) != ESP_OK){
        return 3;
    }
    //change the bit shift values if a genuine part is used, according to the datasheet
    uint32_t val = (receive_data[0]<<4) | (receive_data[1]>>4);
    if(val == 255){
        return 1;
    }else if(val == 4095){
        return 2;
    }
    return 0;
}