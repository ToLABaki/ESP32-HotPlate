#include "reflow_menu_finished.h"


lv_obj_t *finished_window = NULL;

#define FINISHED_PANEL_SIZE_X    150
#define FINISHED_PANEL_SIZE_Y    70

void finished_make_window(lv_obj_t * parent){
    
    finished_window = lv_obj_create(parent, NULL);
    lv_obj_add_style(finished_window, LV_OBJ_PART_MAIN, &style_no_borders);
    lv_obj_add_style(finished_window, LV_OBJ_PART_MAIN, &style_reflow_finished_window);
    lv_obj_set_size(finished_window, FINISHED_PANEL_SIZE_X, FINISHED_PANEL_SIZE_Y);
    lv_obj_align(finished_window, NULL, LV_ALIGN_CENTER, 0,0);
    lv_obj_t * label = lv_label_create(finished_window, NULL);
    lv_obj_set_size(label, FINISHED_PANEL_SIZE_X, FINISHED_PANEL_SIZE_Y);
    lv_label_set_text(label,"Finished");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0,0);
    //lv_obj_set_pos(options_window, OPTIONS_PANEL_SIZE_X, OPTIONS_PANEL_SIZE_Y);
    
    
}


void finished_delete_window(){
    lv_obj_del(finished_window);
}