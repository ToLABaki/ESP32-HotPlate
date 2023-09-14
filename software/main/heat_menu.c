#include "heat_menu.h"

void _heat_menu(enum states* state){
     volatile uint8_t LOCK = 1;
    lv_indev_data_t button_data;

    //setup the screen
    GUI_SEMAPHORE_WAIT


    lv_obj_t * canvas = lv_obj_create(lv_scr_act(), NULL);
    
    lv_obj_add_style(canvas, LV_OBJ_PART_MAIN,&style_no_borders);
    lv_obj_set_style_local_bg_color(canvas, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_obj_set_size(canvas, 320, 240);
    //main_menu_item1

    

    xSemaphoreGive(xGuiSemaphore);

    while(LOCK == 1){
        
        GUI_SEMAPHORE_WAIT
        
        //get the buttons
        _lv_indev_read(my_indev, &button_data);
        xSemaphoreGive(xGuiSemaphore);
        
        if(button_data.state == LV_INDEV_STATE_PR){
            
           gpio_set_level(STATUS_PIN, 1); 
            if(button_data.key == LV_KEY_ENTER){
                LOCK = 0;
                *state = main_menu;

            }
        }else{
            gpio_set_level(STATUS_PIN, 0);
        }


       vTaskDelay(pdMS_TO_TICKS(10));
    }

    GUI_SEMAPHORE_WAIT
    lv_obj_del(canvas);
    xSemaphoreGive(xGuiSemaphore);

    MENUCHANGED_DISABLEBUTTONS = 1;
}