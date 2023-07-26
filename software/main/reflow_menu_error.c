#include "reflow_menu_error.h"

lv_obj_t *error_window = NULL;

#define ERROR_PANEL_SIZE_X    150
#define ERROR_PANEL_SIZE_Y    70

void error_make_window(lv_obj_t * parent, char * text){
    
    error_window = lv_obj_create(parent, NULL);
    lv_obj_add_style(error_window, LV_OBJ_PART_MAIN, &style_no_borders);
    lv_obj_add_style(error_window, LV_OBJ_PART_MAIN, &style_reflow_error_window);

    lv_obj_set_size(error_window, ERROR_PANEL_SIZE_X, ERROR_PANEL_SIZE_Y);
    lv_obj_align(error_window, NULL, LV_ALIGN_CENTER, 0,0);
    lv_obj_t * label = lv_label_create(error_window, NULL);
    lv_obj_set_size(label, ERROR_PANEL_SIZE_X, ERROR_PANEL_SIZE_Y);
    lv_label_set_text(label,text);
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0,0);
    //lv_obj_set_pos(options_window, OPTIONS_PANEL_SIZE_X, OPTIONS_PANEL_SIZE_Y);
    

    
}


void error_delete_window(){
    lv_obj_del(error_window);
}