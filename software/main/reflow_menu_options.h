#ifndef REFLOW_MENU_OPTIONS_H
#define REFLOW_MENU_OPTIONS_H

#include "defines.h"
#include "styles.h"


#define OPTIONS_SIZE 2

extern void options_make_window(lv_obj_t * parent);

extern void options_window_select(uint32_t index);

extern void options_window_unselect(uint32_t index);


extern void options_delete_window();

#endif 