#include "config_menu.h"


uint32_t main_config_list_selection_index = 0;

lv_obj_t * main_config_list = NULL;
lv_obj_t * secondary_config_list = NULL;

lv_group_t * main_list_group = NULL;
lv_group_t * secondary_list_group = NULL;

lv_obj_t * canvas = NULL;

uint32_t list_selected = 0;



#define CONFIG_LISTS_SPLIT 324/3
        



void redraw_secondary_list(){
    uint32_t i = 0;
    secondary_config_list = lv_list_create(canvas, NULL);
    lv_obj_set_size(secondary_config_list,  2*324/3, 240);
    lv_obj_set_pos(secondary_config_list,   324/3, 0);
    lv_obj_add_style(secondary_config_list, LV_OBJ_PART_MAIN,&style_no_borders);
    lv_obj_add_style(secondary_config_list, LV_OBJ_PART_MAIN,&style_secondary_config_list);
    if(main_config_list_selection_index != CONFIG_LIST_SIZE - 2){
        for(i = 0; i < config_list[main_config_list_selection_index].items_array_length; i++){
            config_list[main_config_list_selection_index].items_array[i].associated_button = lv_list_add_btn(secondary_config_list, LV_SYMBOL_DIRECTORY, config_list[main_config_list_selection_index].items_array[i].label);
            lv_obj_add_style(config_list[main_config_list_selection_index].items_array[i].associated_button, LV_BTN_PART_MAIN, &style_main_config_list_button);
            lv_obj_add_style(config_list[main_config_list_selection_index].items_array[i].associated_button, LV_BTN_PART_MAIN, &style_secondary_config_list_button);
            
            //lv_obj_set_event_cb(config_list[main_config_list_selection_index].items_array[i].associated_button, event_handler_secondary_config_list);
            
            if(i == config_list[main_config_list_selection_index].selected_index){
                lv_obj_set_state(config_list[main_config_list_selection_index].items_array[i].associated_button, LV_STATE_DISABLED);
                lv_list_focus_btn(secondary_config_list, config_list[main_config_list_selection_index].items_array[i].associated_button);
            }
        }
    }else{
        for(i = 0; i < CONFIG_LIST_SIZE - 2; i++){
            lv_obj_t *lol = lv_list_add_btn(secondary_config_list, LV_SYMBOL_DIRECTORY, config_list[i].items_array[config_list[i].selected_index].label);
            lv_obj_add_style(lol, LV_BTN_PART_MAIN, &style_main_config_list_button);
            lv_obj_add_style(lol, LV_BTN_PART_MAIN, &style_secondary_config_list_button);
        }
    }
    
}

void draw_main_list(){
    canvas = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_add_style(canvas, LV_OBJ_PART_MAIN,&style_no_borders);
    lv_obj_set_style_local_bg_color(canvas, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_size(canvas, 320, 240);

    redraw_secondary_list();

    main_config_list = lv_list_create(canvas, NULL);
    lv_obj_set_size(main_config_list, (324/3)+4, 240);
    lv_obj_set_pos(main_config_list, -2, 0);

    uint32_t i;

    for(i = 0; i < CONFIG_LIST_SIZE; i++){
        config_list[i].associated_button = lv_list_add_btn(main_config_list, LV_SYMBOL_DIRECTORY, config_list[i].label);
        lv_obj_add_style(config_list[i].associated_button, LV_BTN_PART_MAIN, &style_main_config_list_button);
        //lv_obj_set_event_cb(config_list[i].associated_button, event_handler_main_config_list);
        if(i == main_config_list_selection_index){
            lv_list_focus_btn(main_config_list, config_list[i].associated_button);
        }
    }

    lv_obj_add_style(main_config_list, LV_OBJ_PART_MAIN, &style_list);
    lv_list_set_scroll_propagation(main_config_list, false);
}


void _config_menu(enum states* state){
    volatile uint8_t LOCK = 1;
    lv_indev_data_t button_data;

   

    //setup the screen
    GUI_SEMAPHORE_WAIT

    reflow_selected_params->function = config_list[0].items_array[config_list[0].selected_index].value;
    reflow_selected_params->profile = (struct reflow_profile*)config_list[1].items_array[config_list[1].selected_index].data;
    reflow_selected_params->sensor = config_list[2].items_array[config_list[2].selected_index].value;
    reflow_selected_params->fans = config_list[3].items_array[config_list[3].selected_index].value;


    main_config_list_selection_index = 4;
    draw_main_list();

    xSemaphoreGive(xGuiSemaphore);
    uint32_t last_butn = 0;
    while(LOCK == 1){
        GUI_SEMAPHORE_WAIT
        _lv_indev_read(my_indev, &button_data);
        if(button_data.state == LV_INDEV_STATE_PR){
            gpio_set_level(STATUS_PIN, 1);
            if(last_butn != button_data.key){
                switch(button_data.key){
                    case LV_KEY_DOWN:
                        if(list_selected == 0){
                            if(main_config_list_selection_index < CONFIG_LIST_SIZE-1){
                                main_config_list_selection_index++; 
                                lv_list_focus_btn(main_config_list, config_list[main_config_list_selection_index].associated_button);
                                lv_obj_del(secondary_config_list);
                                redraw_secondary_list();
                            }
                        }else if(list_selected == 1){
                            if(config_list[main_config_list_selection_index].selected_index < config_list[main_config_list_selection_index].items_array_length-1){
                                config_list[main_config_list_selection_index].selected_index++;
                                lv_list_focus_btn(secondary_config_list, config_list[main_config_list_selection_index].items_array[config_list[main_config_list_selection_index].selected_index].associated_button);
                            }

                        }
                        break;
                    case LV_KEY_UP:
                        if(list_selected == 0){
                            if(main_config_list_selection_index > 0){
                                main_config_list_selection_index--;
                                lv_list_focus_btn(main_config_list, config_list[main_config_list_selection_index].associated_button);
                                lv_obj_del(secondary_config_list);
                                redraw_secondary_list();
                            }
                        }else if(list_selected == 1){
                            if(config_list[main_config_list_selection_index].selected_index > 0){
                                config_list[main_config_list_selection_index].selected_index--;
                                lv_list_focus_btn(secondary_config_list, config_list[main_config_list_selection_index].items_array[config_list[main_config_list_selection_index].selected_index].associated_button);
                            }
                        }
                        break;
                    case LV_KEY_ENTER:
                        if(list_selected == 0){
                            //lv_event_send(config_list[main_config_list_selection_index].associated_button, LV_EVENT_CLICKED, NULL);
                            if(main_config_list_selection_index < CONFIG_LIST_SIZE -2){
                                list_selected = 1;
                                lv_obj_set_state(config_list[main_config_list_selection_index].items_array[config_list[main_config_list_selection_index].selected_index].associated_button, LV_STATE_FOCUSED);
                                lv_obj_set_state(config_list[main_config_list_selection_index].associated_button, LV_STATE_DISABLED);
                            }else{
                                
                            }
                            switch(main_config_list_selection_index){
                                case (CONFIG_LIST_SIZE-2):
                                    *state = reflow_menu;
                                    LOCK = 0;
                                    break;
                                case (CONFIG_LIST_SIZE-1):
                                    *state = main_menu;
                                    LOCK = 0;
                                    break;
                                default:
                                    break;
                            }
                        }else if(list_selected == 1){
                            //lv_event_send(config_list[main_config_list_selection_index].items_array[config_list[main_config_list_selection_index].selected_index].associated_button, LV_EVENT_CLICKED, NULL);
                            list_selected = 0;
                            lv_obj_set_state(config_list[main_config_list_selection_index].associated_button, LV_STATE_FOCUSED);
                            lv_obj_set_state(config_list[main_config_list_selection_index].items_array[config_list[main_config_list_selection_index].selected_index].associated_button, LV_STATE_DISABLED);
                            switch(main_config_list_selection_index){
                                case (0):
                                    printf("Function\n");
                                    reflow_selected_params->function = config_list[0].items_array[config_list[0].selected_index].value;
                                    break;
                                case (1):
                                    printf("Profile\n");
                                    reflow_selected_params->profile = (struct reflow_profile*)config_list[1].items_array[config_list[1].selected_index].data;
                                    profile_selected = config_list[1].selected_index;
                                    break;
                                case (2):
                                    printf("Sensor\n");
                                    reflow_selected_params->sensor = config_list[2].items_array[config_list[2].selected_index].value;
                                    break;
                                case (3):
                                    printf("Fans\n");
                                    reflow_selected_params->fans = config_list[3].items_array[config_list[3].selected_index].value;
                                    break;
                                default:
                                    break;
                            }
                        }
                        break;
                    default:
                        break;
                }

            }

            last_butn = button_data.key;
        }else{
            last_butn = 0xFF;
            gpio_set_level(STATUS_PIN, 0);
        }
        xSemaphoreGive(xGuiSemaphore);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    GUI_SEMAPHORE_WAIT
    lv_obj_del(canvas);
    xSemaphoreGive(xGuiSemaphore);

    MENUCHANGED_DISABLEBUTTONS = 1;
}


#undef CONFIG_LISTS_SPLIT