#include "reflow_menu_options.h"


lv_obj_t *options_window = NULL;
lv_obj_t *options_list[OPTIONS_SIZE];

#define OPTIONS_PANEL_SIZE_X    150
#define OPTIONS_PANEL_SIZE_Y    70

void options_make_window(lv_obj_t * parent){
    
    options_window = lv_list_create(parent, NULL);
    lv_obj_add_style(options_window, LV_OBJ_PART_MAIN, &style_no_borders);
    lv_obj_set_size(options_window, OPTIONS_PANEL_SIZE_X, OPTIONS_PANEL_SIZE_Y);
    lv_obj_align(options_window, NULL, LV_ALIGN_CENTER, 0,0);
    //lv_obj_set_pos(options_window, OPTIONS_PANEL_SIZE_X, OPTIONS_PANEL_SIZE_Y);

    options_list[0] = lv_list_add_btn(options_window, LV_SYMBOL_DIRECTORY, "Back");
    lv_obj_add_style(options_list[0], LV_BTN_PART_MAIN, &style_main_config_list_button);

    options_list[1] = lv_list_add_btn(options_window, LV_SYMBOL_DIRECTORY, "Cancel");
    lv_obj_add_style(options_list[1], LV_BTN_PART_MAIN, &style_main_config_list_button);

    lv_obj_set_state(options_list[0], LV_STATE_FOCUSED);
    
}

void options_window_select(uint32_t index){
    lv_obj_set_state(options_list[index], LV_STATE_FOCUSED);
}
void options_window_unselect(uint32_t index){
    lv_obj_set_state(options_list[index], LV_STATE_DEFAULT);
}

void options_delete_window(){
    lv_obj_del(options_window);
}