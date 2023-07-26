#ifndef STYLES_H
#define STYLES_H

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#include "lvgl_helpers.h"

#include "defines.h"



#define COLOR_ORANGE            lv_color_make(0XFB, 0x76, 0x1C)
#define COLOR_DARK_ORANGE       lv_color_make(0XBD, 0x53, 0x25)
#define COLOR_LIGHT_GREY        lv_color_make(0XE9, 0xE9, 0xE9)
#define COLOR_VIOLET_BLUE       lv_color_make(0X43, 0X56, 0xFF)
#define COLOR_VIOLET_BLUE_DARK  lv_color_make(0X27, 0X33, 0xA1)
#define COLOR_GREEN             lv_color_make(0X4D, 0xB5, 0x44)
#define COLOR_YELLOW            lv_color_make(0XFF, 0x99, 0x00)


extern lv_style_t style_no_borders;
extern lv_style_t style_reflow_chart_line;
extern lv_style_t style_big_text;
extern lv_style_t style_list;
extern lv_style_t style_list_btn;
extern lv_style_t style_menu_item;
extern lv_style_t style_menu_item_title;
extern lv_style_t style_menu_item_title_bg;
extern lv_style_t style_menu_item_content;
extern lv_style_t style_menu_bar_bg;
extern lv_style_t style_menu_bar_tab;
extern lv_style_t style_main_config_list_button;
extern lv_style_t style_secondary_config_list;
extern lv_style_t style_secondary_config_list_button;
extern lv_style_t style_reflow_top_bar;
extern lv_style_t style_reflow_top_bar_content;
extern lv_style_t style_reflow_chart_temp_line;
extern lv_style_t style_reflow_finished_window;
extern lv_style_t style_reflow_error_window;



void init_styles();



#endif