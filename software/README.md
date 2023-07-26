Based on the LVGL port for ESP-IDF 4.4 demo project: https://github.com/lvgl/lv_port_esp32

The current adc081s files are temporarely meant to work with clones of this IC that shift their data in a different way. If genuine ICs are to be used, the ```adc081s_get_val(uint16_t *val)``` function needs to be modified. 