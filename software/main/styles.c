#include "styles.h"

lv_style_t style_no_borders;
lv_style_t style_reflow_chart_line;
lv_style_t style_big_text;
lv_style_t style_list;
lv_style_t style_list_btn;
lv_style_t style_menu_item;
lv_style_t style_menu_item_title;
lv_style_t style_menu_item_title_bg;
lv_style_t style_menu_item_content;
lv_style_t style_menu_bar_bg;
lv_style_t style_menu_bar_tab;
lv_style_t style_main_config_list_button;
lv_style_t style_secondary_config_list;
lv_style_t style_secondary_config_list_button;
lv_style_t style_reflow_top_bar;
lv_style_t style_reflow_top_bar_content;
lv_style_t style_reflow_chart_temp_line;
lv_style_t style_reflow_finished_window;
lv_style_t style_reflow_error_window;

extern lv_font_t inter_extra_bold_20;
extern lv_font_t inter_medium_18;

void init_styles(){

    //no_borders
    lv_style_init(&style_no_borders);
    lv_style_set_radius(&style_no_borders, LV_STATE_DEFAULT, 0);
    lv_style_set_border_width(&style_no_borders, LV_STATE_DEFAULT, 0);
    lv_style_set_shadow_width(&style_no_borders, LV_STATE_DEFAULT, 0);
    lv_style_set_margin_all(&style_no_borders, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_all(&style_no_borders, LV_STATE_DEFAULT, 0);
    

    //reflow_chart_line
    lv_style_init(&style_reflow_chart_line);
    lv_style_set_line_width(&style_reflow_chart_line, LV_STATE_DEFAULT, 3);
    lv_style_set_line_color(&style_reflow_chart_line, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_style_set_line_rounded(&style_reflow_chart_line, LV_STATE_DEFAULT, true);

    lv_style_init(&style_big_text);
    lv_style_set_text_font(&style_big_text, LV_STATE_DEFAULT, &lv_font_montserrat_36);
    lv_style_set_text_color(&style_big_text, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //reflow_chart_temp_line
    lv_style_init(&style_reflow_chart_temp_line);
    lv_style_set_line_width(&style_reflow_chart_temp_line, LV_STATE_DEFAULT, 3);
    lv_style_set_line_color(&style_reflow_chart_temp_line, LV_STATE_DEFAULT, COLOR_YELLOW);
    lv_style_set_line_rounded(&style_reflow_chart_temp_line, LV_STATE_DEFAULT, true);

    //list_style
    lv_style_init(&style_list);
    lv_style_set_radius(&style_list, LV_STATE_DEFAULT, 0);
    lv_style_set_border_width(&style_list, LV_STATE_DEFAULT, 0);
    lv_style_set_shadow_width(&style_list, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&style_list, LV_STATE_DEFAULT, 0);
    lv_style_set_margin_all(&style_list, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_all(&style_list, LV_STATE_DEFAULT, 0);


    lv_style_set_radius(&style_list, LV_STATE_FOCUSED, 0);
    lv_style_set_border_width(&style_list, LV_STATE_FOCUSED, 0);
    lv_style_set_shadow_width(&style_list, LV_STATE_FOCUSED, 0);
    lv_style_set_outline_width(&style_list, LV_STATE_FOCUSED, 0);
    lv_style_set_pad_all(&style_list, LV_STATE_FOCUSED, 0);


    //list_btn
    lv_style_init(&style_list_btn);
    lv_style_set_radius(&style_list_btn, LV_STATE_DEFAULT, 0);
    lv_style_set_border_width(&style_list_btn, LV_STATE_DEFAULT, 0);
    lv_style_set_shadow_width(&style_list_btn, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_color(&style_list, LV_STATE_DEFAULT, LV_COLOR_WHITE );
    lv_style_set_bg_color(&style_list_btn, LV_STATE_DEFAULT,  LV_COLOR_WHITE );
    

    lv_style_set_radius(&style_list_btn, LV_STATE_FOCUSED, 0);
    lv_style_set_border_width(&style_list_btn, LV_STATE_FOCUSED, 0);
    lv_style_set_shadow_width(&style_list_btn, LV_STATE_FOCUSED, 0);
    lv_style_set_outline_width(&style_list_btn, LV_STATE_FOCUSED, 2);
    lv_style_set_outline_color(&style_list_btn, LV_STATE_FOCUSED, lv_color_make(0xD0,0xD0,0xD0) );
    lv_style_set_bg_color(&style_list_btn, LV_STATE_FOCUSED,      lv_color_make(0xD0,0xD0,0xD0));
    
    lv_style_set_radius(&style_list_btn, LV_STATE_PRESSED, 0);
    lv_style_set_border_width(&style_list_btn, LV_STATE_PRESSED, 0);
    lv_style_set_shadow_width(&style_list_btn, LV_STATE_PRESSED, 0);
    lv_style_set_outline_width(&style_list_btn, LV_STATE_PRESSED, 0);
    lv_style_set_bg_color(&style_list_btn, LV_STATE_PRESSED, LV_COLOR_WHITE);
    

    //menu_item
    lv_style_init(&style_menu_item);
    lv_style_set_radius(&style_menu_item, LV_STATE_DEFAULT, 10);
    lv_style_set_border_width(&style_menu_item, LV_STATE_DEFAULT, 0);
    lv_style_set_shadow_width(&style_menu_item, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&style_menu_item, LV_STATE_DEFAULT, 0 );
    lv_style_set_bg_color(&style_menu_item, LV_STATE_DEFAULT,  COLOR_LIGHT_GREY );
    

    //menu_item_title_bg
    lv_style_init(&style_menu_item_title_bg);
    lv_style_set_bg_color(&style_menu_item_title_bg, LV_STATE_DEFAULT,  COLOR_VIOLET_BLUE );
   
    lv_style_set_border_width(&style_menu_item_title_bg, LV_STATE_DEFAULT, 0);
    lv_style_set_shadow_width(&style_menu_item_title_bg, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&style_menu_item_title_bg, LV_STATE_DEFAULT, 0);

    //menu_item_title
    lv_style_init(&style_menu_item_title);
    lv_style_set_border_width(&style_menu_item_title, LV_STATE_DEFAULT, 0);
    lv_style_set_shadow_width(&style_menu_item_title, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&style_menu_item_title, LV_STATE_DEFAULT, 0);
    lv_style_set_text_color(&style_menu_item_title, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_menu_item_title, LV_STATE_DEFAULT, &inter_medium_18);

    //menu_item_content
    lv_style_init(&style_menu_item_content);
    lv_style_set_border_width(&style_menu_item_content, LV_STATE_DEFAULT, 0);
    lv_style_set_shadow_width(&style_menu_item_content, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&style_menu_item_content, LV_STATE_DEFAULT, 0);
    lv_style_set_text_color(&style_menu_item_content, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_style_set_text_font(&style_menu_item_content, LV_STATE_DEFAULT, &inter_extra_bold_20);

    //menu_bar_bg
    lv_style_init(&style_menu_bar_bg);
    lv_style_set_border_width(&style_menu_bar_bg, LV_STATE_DEFAULT, 0);
    lv_style_set_shadow_width(&style_menu_bar_bg, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&style_menu_bar_bg, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&style_menu_bar_bg, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_color(&style_menu_bar_bg, LV_STATE_DEFAULT,  COLOR_VIOLET_BLUE );

    //menu_bar_tab
    lv_style_init(&style_menu_bar_tab);
    lv_style_set_border_width(&style_menu_bar_tab, LV_STATE_DEFAULT, 0);
    lv_style_set_shadow_width(&style_menu_bar_tab, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&style_menu_bar_tab, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&style_menu_bar_tab, LV_STATE_DEFAULT, 0);
    lv_style_set_text_color(&style_menu_bar_tab, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //config_list_button
    lv_style_init(&style_main_config_list_button);

    lv_style_set_radius(&style_main_config_list_button, LV_STATE_DEFAULT, 0);
    lv_style_set_border_width(&style_main_config_list_button, LV_STATE_DEFAULT, 0);
    lv_style_set_shadow_width(&style_main_config_list_button, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&style_main_config_list_button, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_color(&style_main_config_list_button, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_color(&style_main_config_list_button, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_font(&style_main_config_list_button, LV_STATE_DEFAULT, &inter_medium_18);
    lv_style_set_pad_ver(&style_main_config_list_button, LV_STATE_DEFAULT, 4);
    lv_style_set_pad_hor(&style_main_config_list_button, LV_STATE_DEFAULT, 0);


    lv_style_set_radius(&style_main_config_list_button, LV_STATE_FOCUSED, 0);
    lv_style_set_border_width(&style_main_config_list_button, LV_STATE_FOCUSED, 0);
    lv_style_set_shadow_width(&style_main_config_list_button, LV_STATE_FOCUSED, 0);
    lv_style_set_outline_width(&style_main_config_list_button, LV_STATE_FOCUSED, 0);
    lv_style_set_bg_color(&style_main_config_list_button, LV_STATE_FOCUSED, COLOR_VIOLET_BLUE);
    lv_style_set_text_color(&style_main_config_list_button, LV_STATE_FOCUSED, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_main_config_list_button, LV_STATE_FOCUSED, &inter_medium_18);
    lv_style_set_pad_ver(&style_main_config_list_button, LV_STATE_FOCUSED, 4);
    lv_style_set_pad_hor(&style_main_config_list_button, LV_STATE_FOCUSED, 0);
    
    lv_style_set_radius(&style_main_config_list_button, LV_STATE_PRESSED, 0);
    lv_style_set_border_width(&style_main_config_list_button, LV_STATE_PRESSED, 0);
    lv_style_set_shadow_width(&style_main_config_list_button, LV_STATE_PRESSED, 0);
    lv_style_set_outline_width(&style_main_config_list_button, LV_STATE_PRESSED, 0);
    lv_style_set_bg_color(&style_main_config_list_button, LV_STATE_PRESSED, LV_COLOR_RED);
    lv_style_set_text_color(&style_main_config_list_button, LV_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_main_config_list_button, LV_STATE_PRESSED, &inter_medium_18);
    lv_style_set_pad_ver(&style_main_config_list_button, LV_STATE_PRESSED, 4);
    lv_style_set_pad_hor(&style_main_config_list_button, LV_STATE_PRESSED, 0);

    lv_style_set_radius(&style_main_config_list_button, LV_STATE_DISABLED, 0);
    lv_style_set_border_width(&style_main_config_list_button, LV_STATE_DISABLED, 0);
    lv_style_set_shadow_width(&style_main_config_list_button, LV_STATE_DISABLED, 0);
    lv_style_set_outline_width(&style_main_config_list_button, LV_STATE_DISABLED, 0);
    lv_style_set_bg_color(&style_main_config_list_button, LV_STATE_DISABLED, LV_COLOR_GRAY);
    lv_style_set_text_color(&style_main_config_list_button, LV_STATE_DISABLED, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_main_config_list_button, LV_STATE_DISABLED, &inter_medium_18);
    lv_style_set_pad_ver(&style_main_config_list_button, LV_STATE_DISABLED, 4);
    lv_style_set_pad_hor(&style_main_config_list_button, LV_STATE_DISABLED, 0);
    

    
    //secondary_config_list
    lv_style_init(&style_secondary_config_list);
    lv_style_set_bg_color(&style_secondary_config_list, LV_STATE_DEFAULT, COLOR_LIGHT_GREY);

    //secondary_config_list_button
    lv_style_init(&style_secondary_config_list_button);
    lv_style_set_bg_color(&style_secondary_config_list_button, LV_STATE_DEFAULT, COLOR_LIGHT_GREY);

    //reflow_top_bar
    lv_style_init(&style_reflow_top_bar);
    lv_style_set_radius(&style_reflow_top_bar, LV_STATE_DEFAULT, 0);
    lv_style_set_border_width(&style_reflow_top_bar, LV_STATE_DEFAULT, 0);
    lv_style_set_shadow_width(&style_reflow_top_bar, LV_STATE_DEFAULT, 0);
    lv_style_set_margin_all(&style_reflow_top_bar, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_all(&style_reflow_top_bar, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_color(&style_reflow_top_bar, LV_STATE_DEFAULT,  COLOR_VIOLET_BLUE);

    //reflow_top_bar_content
    lv_style_set_text_font(&style_reflow_top_bar_content, LV_STATE_DEFAULT, &inter_medium_18);
    lv_style_set_text_color(&style_reflow_top_bar_content, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //reflow_finished_window
    lv_style_init(&style_reflow_finished_window);
    lv_style_set_bg_color(&style_reflow_finished_window, LV_STATE_DEFAULT, COLOR_VIOLET_BLUE);
    lv_style_set_text_color(&style_reflow_finished_window, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_reflow_finished_window, LV_STATE_DEFAULT, &inter_extra_bold_20);
    lv_style_set_radius(&style_reflow_finished_window, LV_STATE_DEFAULT, 10);

    //reflow_error_window
    lv_style_init(&style_reflow_finished_window);
    lv_style_set_bg_color(&style_reflow_finished_window, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_style_set_text_color(&style_reflow_finished_window, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_reflow_finished_window, LV_STATE_DEFAULT, &inter_extra_bold_20);
    lv_style_set_radius(&style_reflow_finished_window, LV_STATE_DEFAULT, 10);

}

