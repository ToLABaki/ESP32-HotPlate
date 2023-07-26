#include "adc.h"

static esp_adc_cal_characteristics_t adc1_chars;

esp_err_t adc_init(){
    esp_err_t ret;

    //calibrate adc
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 0, &adc1_chars);

    //set attenuation
    ret = adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    if(ret == ESP_FAIL){
        return ret;
    }

    
    //set bit width
    ret = adc1_config_width(ADC_WIDTH_BIT_12);
    if(ret == ESP_FAIL){
        return ret;
    }
    
    return ret;
}


uint32_t adc_get_mv(){
    return esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_0), &adc1_chars);
    
}


