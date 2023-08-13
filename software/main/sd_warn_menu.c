#include "sd_warn_menu.h"


void _sd_warn_menu(enum states* state){
    volatile uint8_t LOCK = 1;
    lv_indev_data_t button_data;



    //setup the screen
    GUI_SEMAPHORE_WAIT


    lv_obj_t * canvas = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_add_style(canvas, LV_OBJ_PART_MAIN,&style_no_borders);
    lv_obj_set_style_local_bg_color(canvas, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_size(canvas, 320, 240);


    //main_menu_item1
    lv_obj_t * main_menu_item1 = lv_obj_create(canvas, NULL);
    lv_obj_add_style(main_menu_item1, LV_OBJ_PART_MAIN, &style_menu_item);
    lv_obj_set_size(main_menu_item1, 140, 170);
    lv_obj_set_pos(main_menu_item1, 12, 12);

    lv_obj_t * main_menu_item1_title_bg = lv_obj_create(main_menu_item1, NULL);
    lv_obj_add_style(main_menu_item1_title_bg, LV_OBJ_PART_MAIN, &style_menu_item_title_bg);
    lv_obj_set_size(main_menu_item1_title_bg, lv_obj_get_width(lv_obj_get_parent(main_menu_item1_title_bg)), 25);
    lv_obj_set_pos(main_menu_item1_title_bg, 0, 0);

    lv_obj_t * main_menu_item1_title = lv_label_create(main_menu_item1_title_bg, NULL);
    lv_obj_add_style(main_menu_item1_title, LV_OBJ_PART_MAIN, &style_menu_item_title);
    lv_obj_set_size(main_menu_item1_title, lv_obj_get_width(main_menu_item1_title_bg), lv_obj_get_height(main_menu_item1_title_bg));
    lv_obj_align(main_menu_item1_title, NULL, LV_ALIGN_IN_LEFT_MID, 7,0);
    lv_label_set_text(main_menu_item1_title, "Profile");
    lv_label_set_align(main_menu_item1_title, LV_LABEL_ALIGN_LEFT);

    lv_obj_t * main_menu_item1_content = lv_label_create(main_menu_item1, NULL);
    lv_obj_add_style(main_menu_item1_content, LV_OBJ_PART_MAIN, &style_menu_item_content);
    lv_label_set_long_mode(main_menu_item1_content, LV_LABEL_LONG_BREAK);
    lv_label_set_align(main_menu_item1_content, LV_LABEL_ALIGN_LEFT);
    if(profiles != NULL){
        lv_label_set_text(main_menu_item1_content, profiles[profile_selected].label);
    }
    
    lv_obj_set_size(main_menu_item1_content, lv_obj_get_width(main_menu_item1)-(7*2), lv_obj_get_height(main_menu_item1)-27);
    lv_obj_set_pos(main_menu_item1_content, 7, 27);

    

    

    xSemaphoreGive(xGuiSemaphore);

 

    

    uint16_t temp_val = 0;
    char string_buffer[20];

    while(LOCK == 1){
        
        GUI_SEMAPHORE_WAIT
        printf("SDWARN done\n");
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