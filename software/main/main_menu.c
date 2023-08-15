#include "main_menu.h"

void _main_menu(enum states* state){
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
        printf("PROFILE SELECTED %s", reflow_selected_params->profile->label);
    }
    
    lv_obj_set_size(main_menu_item1_content, lv_obj_get_width(main_menu_item1)-(7*2), lv_obj_get_height(main_menu_item1)-27);
    lv_obj_set_pos(main_menu_item1_content, 7, 27);

    xSemaphoreGive(xGuiSemaphore);

    GUI_SEMAPHORE_WAIT

    //main_menu_item2
    lv_obj_t * main_menu_item2 = lv_obj_create(canvas, NULL);
    lv_obj_add_style(main_menu_item2, LV_OBJ_PART_MAIN, &style_menu_item);
    lv_obj_set_size(main_menu_item2, 140, 57);
    lv_obj_set_pos(main_menu_item2, 168, 12);

    lv_obj_t * main_menu_item2_title_bg = lv_obj_create(main_menu_item2, NULL);
    lv_obj_add_style(main_menu_item2_title_bg, LV_OBJ_PART_MAIN, &style_menu_item_title_bg);
    lv_obj_set_size(main_menu_item2_title_bg, lv_obj_get_width(lv_obj_get_parent(main_menu_item2_title_bg)), 25);
    lv_obj_set_pos(main_menu_item2_title_bg, 0, 0);

    lv_obj_t * main_menu_item2_title = lv_label_create(main_menu_item2_title_bg, NULL);
    lv_obj_add_style(main_menu_item2_title, LV_OBJ_PART_MAIN, &style_menu_item_title);
    lv_obj_set_size(main_menu_item2_title, lv_obj_get_width(main_menu_item2_title_bg), lv_obj_get_height(main_menu_item2_title_bg));
    lv_obj_align(main_menu_item2_title, NULL, LV_ALIGN_IN_LEFT_MID, 7,0);
    lv_label_set_text(main_menu_item2_title, "Plate");
    lv_label_set_align(main_menu_item2_title, LV_LABEL_ALIGN_LEFT);

    lv_obj_t * main_menu_item2_content = lv_label_create(main_menu_item2, NULL);
    lv_obj_add_style(main_menu_item2_content, LV_OBJ_PART_MAIN, &style_menu_item_content);
    lv_label_set_long_mode(main_menu_item2_content, LV_LABEL_LONG_BREAK);
    lv_label_set_align(main_menu_item2_content, LV_LABEL_ALIGN_LEFT);
    lv_label_set_text(main_menu_item2_content, "245°C");
    lv_obj_set_size(main_menu_item2_content, lv_obj_get_width(main_menu_item2)-(7*2), lv_obj_get_height(main_menu_item2)-27);
    lv_obj_set_pos(main_menu_item2_content, 7, 27);

    xSemaphoreGive(xGuiSemaphore);

    GUI_SEMAPHORE_WAIT

    //main_menu_item3
    lv_obj_t * main_menu_item3 = lv_obj_create(canvas, NULL);
    lv_obj_add_style(main_menu_item3, LV_OBJ_PART_MAIN, &style_menu_item);
    lv_obj_set_size(main_menu_item3, 140, 57);
    lv_obj_set_pos(main_menu_item3, 168, 85);

    lv_obj_t * main_menu_item3_title_bg = lv_obj_create(main_menu_item3, NULL);
    lv_obj_add_style(main_menu_item3_title_bg, LV_OBJ_PART_MAIN, &style_menu_item_title_bg);
    lv_obj_set_size(main_menu_item3_title_bg, lv_obj_get_width(lv_obj_get_parent(main_menu_item3_title_bg)), 25);
    lv_obj_set_pos(main_menu_item3_title_bg, 0, 0);

    lv_obj_t * main_menu_item3_title = lv_label_create(main_menu_item3_title_bg, NULL);
    lv_obj_add_style(main_menu_item3_title, LV_OBJ_PART_MAIN, &style_menu_item_title);
    lv_obj_set_size(main_menu_item3_title, lv_obj_get_width(main_menu_item2_title_bg), lv_obj_get_height(main_menu_item3_title_bg));
    lv_obj_align(main_menu_item3_title, NULL, LV_ALIGN_IN_LEFT_MID, 7,0);
    lv_label_set_text(main_menu_item3_title, "External");
    lv_label_set_align(main_menu_item3_title, LV_LABEL_ALIGN_RIGHT);

    lv_obj_t * main_menu_item3_content = lv_label_create(main_menu_item3, NULL);
    lv_obj_add_style(main_menu_item3_content, LV_OBJ_PART_MAIN, &style_menu_item_content);
    lv_label_set_long_mode(main_menu_item3_content, LV_LABEL_LONG_BREAK);
    lv_label_set_align(main_menu_item3_content, LV_LABEL_ALIGN_LEFT);
    lv_label_set_text(main_menu_item3_content, "-----");
    lv_obj_set_size(main_menu_item3_content, lv_obj_get_width(main_menu_item3)-(7*2), lv_obj_get_height(main_menu_item3)-27);
    lv_obj_set_pos(main_menu_item3_content, 7, 27);

    xSemaphoreGive(xGuiSemaphore);

    GUI_SEMAPHORE_WAIT
    //main_menu_bar
    lv_obj_t * main_menu_bar = lv_obj_create(canvas, NULL);
    lv_obj_add_style(main_menu_bar, LV_OBJ_PART_MAIN, &style_menu_bar_bg);
    lv_obj_set_size(main_menu_bar, lv_obj_get_width(canvas), 35);
    lv_obj_set_pos(main_menu_bar, 0, 240-35);

    lv_obj_t * main_menu_bar_tab1 = lv_label_create(main_menu_bar, NULL);
    lv_obj_add_style(main_menu_bar_tab1, LV_OBJ_PART_MAIN, &style_menu_bar_tab);
    lv_label_set_align(main_menu_bar_tab1, LV_LABEL_ALIGN_CENTER);
    lv_label_set_text(main_menu_bar_tab1, LV_SYMBOL_PLAY);
    lv_obj_set_size(main_menu_bar_tab1, 80, lv_obj_get_height(main_menu_bar));
    lv_obj_align(main_menu_bar_tab1, NULL, LV_ALIGN_CENTER, 0,0);
    




    xSemaphoreGive(xGuiSemaphore);

    printf("Menu done\n");

    uint16_t temp_val = 0;
    char string_buffer[20];

    while(LOCK == 1){
        GUI_SEMAPHORE_WAIT
        //get the buttons
        _lv_indev_read(my_indev, &button_data);
    

       
        
        if(adc081s_get_val(&temp_val) == ESP_OK){
            if(temp_val == 0xFF){
            }
            sprintf(string_buffer, "%.1f °c",  mv_to_temp_EPCOS_100K_B57560G104F(adc081s_val_to_mv(temp_val)));
            lv_label_set_text(main_menu_item2_content, string_buffer);
                //printf("%s\n",string_buffer );
        }else{
            sprintf(string_buffer, "ADC ERROR");
        }
        
      

        
        xSemaphoreGive(xGuiSemaphore);
        
        if(button_data.state == LV_INDEV_STATE_PR){
            
           gpio_set_level(STATUS_PIN, 1); 
           printf("%d\n",button_data.key );
            if(button_data.key == LV_KEY_ENTER){
                LOCK = 0;
                *state = select_menu;

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
