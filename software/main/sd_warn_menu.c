#include "sd_warn_menu.h"


LV_IMG_DECLARE(warning);

void _sd_warn_menu(enum states* state){
    volatile uint8_t LOCK = 1;
    lv_indev_data_t button_data;



    //setup the screen
    GUI_SEMAPHORE_WAIT


    lv_obj_t * canvas = lv_obj_create(lv_scr_act(), NULL);
    
    lv_obj_add_style(canvas, LV_OBJ_PART_MAIN,&style_no_borders);
    lv_obj_set_style_local_bg_color(canvas, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_obj_set_size(canvas, 320, 240);


    


    //main_menu_item1
    

    
    lv_obj_t * img1 = lv_img_create(canvas, NULL);
    lv_img_set_src(img1, &warning);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);
    lv_obj_add_style(img1, LV_IMG_PART_MAIN, &style_sd_warn_menu_image);
   
        
    lv_obj_t * text = lv_label_create(canvas, NULL);
    lv_obj_add_style(text, LV_IMG_PART_MAIN, &style_sd_warn_menu_text);
    lv_label_set_text(text, "No SD CARD detected!\nDefaults will be loaded");
    lv_label_set_align(text, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(text, img1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
  

    

    xSemaphoreGive(xGuiSemaphore);

 

    

    uint16_t temp_val = 0;
    char string_buffer[20];

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