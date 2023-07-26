#include "reflow_menu.h"


#define TOP_BAR_HEIGHT  90
#define GRAPH_WIDTH     320
#define GRAPH_HEIGHT    150
#define GRAPH_TEMP_MAX  300.0

#define TIMER_CALLBACK_PERIOD_MS    10
#define GRAPH_UPDATE_PERIOD_FREQ    25



lv_obj_t *reflow_canvas = NULL;
lv_obj_t *chart = NULL;
lv_obj_t *temp_line = NULL;
lv_point_t line_points[6];
lv_point_t tracking_line_points[GRAPH_WIDTH+2];


lv_obj_t *reflow_top_bar = NULL;
lv_obj_t *top_bar_label = NULL;

lv_obj_t * reflow_menu_item2_content = NULL;
lv_obj_t * reflow_menu_item2_content2 = NULL;
lv_obj_t * reflow_menu_item3_content = NULL;


volatile uint32_t options_selected_index = 0;


volatile uint32_t reflow_finished_flag = 0;

volatile uint32_t update_graph_flag = 0;
volatile uint32_t update_graph_current_temp = 0;
volatile uint32_t update_graph_target_temp = 0;
volatile uint32_t update_graph_time_ms = 0;

volatile uint32_t killed_timer_flag = 0;

esp_timer_handle_t timer_handler;



void kill_timer(esp_timer_handle_t handler){
    if(killed_timer_flag == 0){
        ESP_ERROR_CHECK(esp_timer_stop(handler));
        ESP_ERROR_CHECK(esp_timer_delete(handler));
        killed_timer_flag = 1;
    }
    
}




#define CHART_DIV_X 60
#define CHART_DIV_Y 3
#define CHART_OFFSET_X 0
#define CHART_OFFSET_Y 0

uint32_t excess(uint32_t a, uint32_t b){
    uint32_t i = 0;
    while(a > i){
        i = i + b;
    }
    return i - a;
}

void draw_graph(lv_obj_t * chart, lv_obj_t * line, struct reflow_profile *profile){
    lv_obj_set_size(chart, 320, GRAPH_HEIGHT);
    lv_obj_set_pos(chart, 0,TOP_BAR_HEIGHT); 
    lv_obj_add_style(chart , LV_STATE_DEFAULT,&style_no_borders);
    //lv_obj_set_style_local_bg_color(chart,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_AQUA);
    uint32_t total_reflow_time = profile->preheat.time + profile->soak.time + profile->reflow_ramp.time + profile->reflow.time + profile->cooldown.time;
    uint32_t div_lines = (total_reflow_time / CHART_DIV_X) - 1;
    if((total_reflow_time % CHART_DIV_X) != 0){
        div_lines++;
    }
    lv_chart_set_div_line_count(chart, 2, div_lines);
    lv_chart_refresh(chart);

    float scaler_x;
    float scaler_y;
 
    if(excess(total_reflow_time, CHART_DIV_X) == 0){
        scaler_x = GRAPH_WIDTH/ (float)total_reflow_time;
    }else{
        scaler_x = GRAPH_WIDTH/ (float)(total_reflow_time + excess(total_reflow_time, CHART_DIV_X));
    }

    scaler_y = (float)(GRAPH_HEIGHT ) / GRAPH_TEMP_MAX;


    line_points[0].x = CHART_OFFSET_X;
    line_points[0].y = CHART_OFFSET_Y + ((float)(profile->preheat.start_temp) * scaler_y); 
    line_points[1].x = CHART_OFFSET_X + (profile->preheat.time * scaler_x);
    line_points[1].y = CHART_OFFSET_Y + ((float)(profile->preheat.finish_temp) * scaler_y);
    line_points[2].x = CHART_OFFSET_X + ((profile->preheat.time + profile->soak.time) * scaler_x);
    line_points[2].y = CHART_OFFSET_Y + ((float)(profile->soak.finish_temp) * scaler_y);
    line_points[3].x = CHART_OFFSET_X + ((profile->preheat.time + profile->soak.time + profile->reflow_ramp.time) * scaler_x);
    line_points[3].y = CHART_OFFSET_Y + ((float)(profile->reflow_ramp.finish_temp) * scaler_y);
    line_points[4].x = CHART_OFFSET_X + ((profile->preheat.time + profile->soak.time + profile->reflow_ramp.time + profile->reflow.time) * scaler_x);
    line_points[4].y = CHART_OFFSET_Y + ((float)(profile->reflow.finish_temp) * scaler_y); 
    line_points[5].x = CHART_OFFSET_X + ((profile->preheat.time + profile->soak.time + profile->reflow_ramp.time + profile->reflow.time + profile->cooldown.time) * scaler_x);
    line_points[5].y = CHART_OFFSET_Y + ((float)(profile->cooldown.finish_temp) * scaler_y);
    
    lv_line_set_points(line, line_points, 6);
    lv_obj_add_style(line, LV_LINE_PART_MAIN, &style_reflow_chart_line);
    lv_line_set_y_invert(line ,1);
    lv_obj_align(line, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

}

void update_graph(lv_obj_t * line){
    static int32_t prev_pixel = -1;
    uint32_t i;
    static float scaler_y = (float)(GRAPH_HEIGHT) / GRAPH_TEMP_MAX;
    if(((total_time_ms/1000)%CHART_DIV_X) != 0){
        i = (total_time_ms/1000/CHART_DIV_X) + 1;
    }else{
        i = (total_time_ms/1000/CHART_DIV_X);
    }
    int32_t current_pixel = (GRAPH_WIDTH*(float)elapsed_time_ms)/(float)(i*1000*CHART_DIV_X);
    tracking_line_points[0].x = 0;
    tracking_line_points[0].y = update_graph_current_temp * scaler_y;
    if(current_pixel != prev_pixel){
        tracking_line_points[current_pixel].x = current_pixel;
        tracking_line_points[current_pixel].y = update_graph_current_temp * scaler_y;
        lv_line_set_points(line, tracking_line_points, current_pixel);
        printf("i: %d %d %d %d\n",i ,current_pixel, tracking_line_points[current_pixel].x , tracking_line_points[current_pixel].y);

        prev_pixel = current_pixel;
    }


}
#undef CHART_DIV_X
#undef CHART_DIV_Y
#undef CHART_OFFSET_X
#undef CHART_OFFSET_Y




float calculate_target_temp(uint32_t current_ms, struct reflow_profile *profile){
    uint32_t i;
    
    struct reflow_phase *phases[5] = {&(profile->preheat), &(profile->soak), &(profile->reflow_ramp), &(profile->reflow), &(profile->cooldown)};
    uint32_t time_high = 0;
    uint32_t time_low = 0;
    float ret;
    struct reflow_phase *current_phase = NULL;
    for(i = 0; i < 5; i++){
        time_low = time_high;
        time_high = time_high + phases[i]->time;
        if((time_high*1000) > current_ms){
            break;
        }
    }
    if(i>=5){i=4;}
    time_high = time_high * 1000;
    time_low = time_low * 1000;
    ret = (float)((phases[i]->start_temp)*(time_high - current_ms) + (phases[i]->finish_temp)*(current_ms - time_low))/(float)(time_high - time_low);
    if(ret > 300.0){ret = 0.0;}
    return ret;
}


void timer_callback(void *param){
    static uint32_t vals_array[ADC_SAMPLES_AVG_ARRAY_SIZE];
    uint16_t tmp;
    uint32_t i;
    uint32_t avg;
    static uint32_t times = 0;
    if(times == GRAPH_UPDATE_PERIOD_FREQ){
        times = 0;
        update_graph_flag = 1;
        update_graph_current_temp = current_temp;
        update_graph_time_ms = elapsed_time_ms;
        update_graph_target_temp = target_temp;
   
    }else{times++;}

    if(elapsed_time_ms > total_time_ms){
        //reflow finished
        kill_timer(timer_handler);
        reflow_menu_state = finished;
        GUI_SEMAPHORE_WAIT
        finished_make_window(reflow_canvas);
        xSemaphoreGive(xGuiSemaphore);
        
        printf("DONE\n");
        update_graph_flag = 1;
        update_graph_current_temp = current_temp;
        update_graph_time_ms = elapsed_time_ms;
        update_graph_target_temp = 0.0;
    }else{
        //reflow ongoing
        elapsed_time_ms = elapsed_time_ms + TIMER_CALLBACK_PERIOD_MS;
        target_temp = calculate_target_temp(elapsed_time_ms, reflow_selected_params->profile);
        if(adc081s_get_val(&tmp) == ESP_OK){

        }
        

        for(i = 0; i <ADC_SAMPLES_AVG_ARRAY_SIZE - 1; i++){
            vals_array[i] = vals_array[i+1];
        }
        vals_array[ADC_SAMPLES_AVG_ARRAY_SIZE-1] = tmp;

        avg = 0;
        for(i = 0; i < ADC_SAMPLES_AVG_ARRAY_SIZE; i++){
            avg = avg + vals_array[i];
        }
        avg = avg/ADC_SAMPLES_AVG_ARRAY_SIZE;
        current_temp = mv_to_temp_EPCOS_100K_B57560G104F(adc081s_val_to_mv(avg));
        if(current_temp >= 300.0){
            
        }
    }
    
    
}


void draw_main_screen(){
    GUI_SEMAPHORE_WAIT
    reflow_canvas = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_add_style(reflow_canvas, LV_OBJ_PART_MAIN,&style_no_borders);
    lv_obj_set_style_local_bg_color(reflow_canvas, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_size(reflow_canvas, 320, 240);

    chart = lv_chart_create(reflow_canvas, NULL);
    lv_obj_t *line = lv_line_create(chart, NULL);
    draw_graph(chart, line, reflow_selected_params->profile);

    temp_line = lv_line_create(chart, NULL);
    lv_obj_add_style(temp_line, LV_LINE_PART_MAIN, &style_reflow_chart_temp_line);
    lv_line_set_auto_size(temp_line, false);
    lv_obj_set_size(temp_line, GRAPH_WIDTH, GRAPH_HEIGHT);
    lv_line_set_y_invert(temp_line ,1);
    lv_obj_align(temp_line, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    //lv_obj_set_pos(temp_line, 0,140),
    xSemaphoreGive(xGuiSemaphore);

    GUI_SEMAPHORE_WAIT
    //reflow_menu_item2
    lv_obj_t * reflow_menu_item2 = lv_obj_create(reflow_canvas, NULL);
    lv_obj_add_style(reflow_menu_item2, LV_OBJ_PART_MAIN, &style_menu_item);
    lv_obj_set_size(reflow_menu_item2, 170, 57);
    lv_obj_set_pos(reflow_menu_item2, 12, 12);

    lv_obj_t * reflow_menu_item2_title_bg = lv_obj_create(reflow_menu_item2, NULL);
    lv_obj_add_style(reflow_menu_item2_title_bg, LV_OBJ_PART_MAIN, &style_menu_item_title_bg);
    lv_obj_set_size(reflow_menu_item2_title_bg, lv_obj_get_width(lv_obj_get_parent(reflow_menu_item2_title_bg)), 25);
    lv_obj_set_pos(reflow_menu_item2_title_bg, 0, 0);

    lv_obj_t * reflow_menu_item2_title = lv_label_create(reflow_menu_item2_title_bg, NULL);
    lv_obj_add_style(reflow_menu_item2_title, LV_OBJ_PART_MAIN, &style_menu_item_title);
    lv_obj_set_size(reflow_menu_item2_title, lv_obj_get_width(reflow_menu_item2_title_bg), lv_obj_get_height(reflow_menu_item2_title_bg));
    lv_obj_align(reflow_menu_item2_title, NULL, LV_ALIGN_IN_LEFT_MID, 7,0);
    lv_label_set_text(reflow_menu_item2_title, "Current  |  Target");
    lv_label_set_align(reflow_menu_item2_title, LV_LABEL_ALIGN_LEFT);
    xSemaphoreGive(xGuiSemaphore);
    GUI_SEMAPHORE_WAIT
    
    reflow_menu_item2_content = lv_label_create(reflow_menu_item2, NULL);
    lv_obj_add_style(reflow_menu_item2_content, LV_OBJ_PART_MAIN, &style_menu_item_content);
    lv_label_set_long_mode(reflow_menu_item2_content, LV_LABEL_LONG_BREAK);
    lv_label_set_align(reflow_menu_item2_content, LV_LABEL_ALIGN_LEFT);
    lv_label_set_text(reflow_menu_item2_content, "245°");
    lv_obj_set_size(reflow_menu_item2_content, lv_obj_get_width(reflow_menu_item2)-(7*2), lv_obj_get_height(reflow_menu_item2)-27);
    lv_obj_set_pos(reflow_menu_item2_content, 7, 27);

    reflow_menu_item2_content2 = lv_label_create(reflow_menu_item2, NULL);
    lv_obj_add_style(reflow_menu_item2_content2, LV_OBJ_PART_MAIN, &style_menu_item_content);
    lv_label_set_long_mode(reflow_menu_item2_content2, LV_LABEL_LONG_BREAK);
    lv_label_set_align(reflow_menu_item2_content2, LV_LABEL_ALIGN_LEFT);
    lv_label_set_text(reflow_menu_item2_content2, "245°");
    lv_obj_set_size(reflow_menu_item2_content2, lv_obj_get_width(reflow_menu_item2)-(7*2), lv_obj_get_height(reflow_menu_item2)-27);
    lv_obj_set_pos(reflow_menu_item2_content2, 92, 27);
    xSemaphoreGive(xGuiSemaphore);

    GUI_SEMAPHORE_WAIT
    //reflow_menu_item3
    lv_obj_t * reflow_menu_item3 = lv_obj_create(reflow_canvas, NULL);
    lv_obj_add_style(reflow_menu_item3, LV_OBJ_PART_MAIN, &style_menu_item);
    lv_obj_set_size(reflow_menu_item3, 110, 57);
    lv_obj_set_pos(reflow_menu_item3, 320-12-lv_obj_get_width(reflow_menu_item3), 12);

    lv_obj_t * reflow_menu_item3_title_bg = lv_obj_create(reflow_menu_item3, NULL);
    lv_obj_add_style(reflow_menu_item3_title_bg, LV_OBJ_PART_MAIN, &style_menu_item_title_bg);
    lv_obj_set_size(reflow_menu_item3_title_bg, lv_obj_get_width(lv_obj_get_parent(reflow_menu_item3_title_bg)), 25);
    lv_obj_set_pos(reflow_menu_item3_title_bg, 0, 0);
    xSemaphoreGive(xGuiSemaphore);

    GUI_SEMAPHORE_WAIT
    lv_obj_t * reflow_menu_item3_title = lv_label_create(reflow_menu_item3_title_bg, NULL);
    lv_obj_add_style(reflow_menu_item3_title, LV_OBJ_PART_MAIN, &style_menu_item_title);
    lv_obj_set_size(reflow_menu_item3_title, lv_obj_get_width(reflow_menu_item3_title_bg), lv_obj_get_height(reflow_menu_item3_title_bg));
    lv_obj_align(reflow_menu_item3_title, NULL, LV_ALIGN_IN_LEFT_MID, 7,0);
    lv_label_set_text(reflow_menu_item3_title, "Elapsed");
    lv_label_set_align(reflow_menu_item3_title, LV_LABEL_ALIGN_LEFT);

    reflow_menu_item3_content = lv_label_create(reflow_menu_item3, NULL);
    lv_obj_add_style(reflow_menu_item3_content, LV_OBJ_PART_MAIN, &style_menu_item_content);
    lv_label_set_long_mode(reflow_menu_item3_content, LV_LABEL_LONG_BREAK);
    lv_label_set_align(reflow_menu_item3_content, LV_LABEL_ALIGN_LEFT);
    lv_label_set_text(reflow_menu_item3_content, "245°C");
    lv_obj_set_size(reflow_menu_item3_content, lv_obj_get_width(reflow_menu_item3)-(7*2), lv_obj_get_height(reflow_menu_item3)-27);
    lv_obj_set_pos(reflow_menu_item3_content, 7, 27);

    xSemaphoreGive(xGuiSemaphore);
}

void _reflow_menu(enum states* state){
    char str_tmp[30];
    volatile uint8_t LOCK = 1;
    lv_indev_data_t button_data;
    printf("REFLOWING:\n");
    printf("    Fans:%d\n",reflow_selected_params->fans);
    printf("    Function:%d\n",reflow_selected_params->function);
    printf("    Profile:%s\n",reflow_selected_params->profile->label);
    printf("    Sensor:%d\n",reflow_selected_params->sensor);

    total_time_ms = (reflow_selected_params->profile->preheat.time +
                    reflow_selected_params->profile->soak.time +
                    reflow_selected_params->profile->reflow_ramp.time +
                    reflow_selected_params->profile->reflow.time +
                    reflow_selected_params->profile->cooldown.time)*1000;
    elapsed_time_ms = 0;


    //wait for LVGL setup to finish
    while(LVGL_SETUP_COMPLETE == 0);
    

    //draw the screen
    draw_main_screen();



    const esp_timer_create_args_t my_timer_args = {
        .callback = &timer_callback,
        .name = "My Timer"
    };

    
    ESP_ERROR_CHECK(esp_timer_create(&my_timer_args, &timer_handler));
    ESP_ERROR_CHECK(esp_timer_start_periodic(timer_handler, TIMER_CALLBACK_PERIOD_MS * 1000));
    reflow_menu_state = reflowing;
    killed_timer_flag = 0;
    
    
    uint32_t last_butn = 0;
    while(LOCK == 1){
        GUI_SEMAPHORE_WAIT
        _lv_indev_read(my_indev, &button_data);
        
        if(update_graph_flag == 1){
            sprintf(str_tmp, "%.1f°",current_temp);
            lv_label_set_text(reflow_menu_item2_content, str_tmp);
            sprintf(str_tmp, "%.1f°",target_temp);
            lv_label_set_text(reflow_menu_item2_content2, str_tmp);
            sprintf(str_tmp, "%d",elapsed_time_ms/1000);
            lv_label_set_text(reflow_menu_item3_content, str_tmp);
            update_graph(temp_line);
            update_graph_flag = 0;
        }
        if(button_data.state == LV_INDEV_STATE_PR){
            gpio_set_level(STATUS_PIN, 1);
            if(last_butn != button_data.key){
                switch(button_data.key){
                    case LV_KEY_DOWN:
                        switch(reflow_menu_state){
                            case reflowing:
                                break;
                            case finished:
                                break;
                            case options:
                                if(options_selected_index < (OPTIONS_SIZE - 1)){
                                    options_window_unselect(options_selected_index);
                                    options_selected_index++;
                                    options_window_select(options_selected_index);
                                }
                                break;
                            case error:
                                break;
                            default:
                                break;
                        }
                        break;
                    case LV_KEY_UP:
                        switch(reflow_menu_state){
                            case reflowing:
                                break;
                            case finished:
                                break;
                            case options:
                                if(options_selected_index > 0){
                                    options_window_unselect(options_selected_index);
                                    options_selected_index--;
                                    options_window_select(options_selected_index);
                                }
                                break;
                            case error:
                                break;
                            default:
                                break;
                        }
                        break;
                    case LV_KEY_ENTER:
                        switch(reflow_menu_state){
                            case reflowing:
                                options_selected_index = 0;
                                options_make_window(reflow_canvas);
                                reflow_menu_state = options;
                                break;
                            case finished:
                                finished_delete_window();
                                *state = main_menu; 
                                kill_timer(timer_handler);
                                LOCK = 0; 
                                break;
                            case options:
                                if(options_selected_index == 0){
                                    options_delete_window();
                                }else if(options_selected_index == 1){
                                    options_delete_window();
                                    *state = main_menu; 
                                    kill_timer(timer_handler);
                                    LOCK = 0; 
                                }
                                reflow_menu_state = reflowing;
                                break;
                            case error:
                                break;
                            default:
                                break;
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
    kill_timer(timer_handler);

    GUI_SEMAPHORE_WAIT
    lv_obj_del(reflow_canvas);
    xSemaphoreGive(xGuiSemaphore);

    MENUCHANGED_DISABLEBUTTONS = 1;
}



#undef TOP_BAR_HEIGHT