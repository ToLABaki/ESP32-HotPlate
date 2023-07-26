#include "defines.h"




lv_indev_t * my_indev = NULL;

volatile uint32_t LVGL_SETUP_COMPLETE = 0;

volatile uint32_t MENUCHANGED_DISABLEBUTTONS = 0;


struct reflow_profile * profiles = NULL;

uint32_t profile_selected = 2;

uint32_t profiles_num = 4;

volatile float internal_temp = 0;
volatile float external_temp = 0;



struct selection *config_list = NULL;

volatile struct reflow_params *reflow_selected_params = NULL;


void init_data(){
    profiles = (struct reflow_profile *)malloc(sizeof(struct reflow_profile) * profiles_num);
    strcpy(profiles[0].label, "Chip Quik Sn63/Pb37");
    strcpy(profiles[1].label, "Chip Quik Sn96.5/Ag3/Cu0.5");
    strcpy(profiles[2].label, "Chip Quik TEST");
    strcpy(profiles[3].label, "Chip Quik TEST2");
    

    profiles[0].preheat.start_temp = 0;
    profiles[0].preheat.finish_temp = 150;
    profiles[0].preheat.time = 90;

    profiles[0].soak.start_temp = 150;
    profiles[0].soak.finish_temp = 150;
    profiles[0].soak.time = 90;

    profiles[0].reflow_ramp.start_temp = 150;
    profiles[0].reflow_ramp.finish_temp = 260;
    profiles[0].reflow_ramp.time = 60;

    profiles[0].reflow.start_temp = 260;
    profiles[0].reflow.finish_temp = 260;
    profiles[0].reflow.time = 30;

    profiles[0].cooldown.start_temp = 260;
    profiles[0].cooldown.finish_temp = 0;
    profiles[0].cooldown.time = 90;



    profiles[1].preheat.start_temp = 0;
    profiles[1].preheat.finish_temp = 150;
    profiles[1].preheat.time = 60;

    profiles[1].soak.start_temp = 150;
    profiles[1].soak.finish_temp = 150;
    profiles[1].soak.time = 60;

    profiles[1].reflow_ramp.start_temp = 150;
    profiles[1].reflow_ramp.finish_temp = 250;
    profiles[1].reflow_ramp.time = 60;

    profiles[1].reflow.start_temp = 250;
    profiles[1].reflow.finish_temp = 250;
    profiles[1].reflow.time = 30;

    profiles[1].cooldown.start_temp = 250;
    profiles[1].cooldown.finish_temp = 0;
    profiles[1].cooldown.time = 60;



    profiles[2].preheat.start_temp = 21;
    profiles[2].preheat.finish_temp = 250;
    profiles[2].preheat.time = 20;

    profiles[2].soak.start_temp = 250;
    profiles[2].soak.finish_temp = 300;
    profiles[2].soak.time = 20;

    profiles[2].reflow_ramp.start_temp = 300;
    profiles[2].reflow_ramp.finish_temp = 300;
    profiles[2].reflow_ramp.time = 20;

    profiles[2].reflow.start_temp = 300;
    profiles[2].reflow.finish_temp = 250;
    profiles[2].reflow.time = 20;

    profiles[2].cooldown.start_temp = 250;
    profiles[2].cooldown.finish_temp = 0;
    profiles[2].cooldown.time = 50;



    profiles[3].preheat.start_temp = 0;
    profiles[3].preheat.finish_temp = 150;
    profiles[3].preheat.time = 10;

    profiles[3].soak.start_temp = 150;
    profiles[3].soak.finish_temp = 200;
    profiles[3].soak.time = 10;

    profiles[3].reflow_ramp.start_temp = 200;
    profiles[3].reflow_ramp.finish_temp = 200;
    profiles[3].reflow_ramp.time = 10;

    profiles[3].reflow.start_temp = 200;
    profiles[3].reflow.finish_temp = 250;
    profiles[3].reflow.time = 10;

    profiles[3].cooldown.start_temp = 250;
    profiles[3].cooldown.finish_temp = 0;
    profiles[3].cooldown.time = 10;


    config_list = (struct selection *)malloc(sizeof(struct selection) * (CONFIG_LIST_SIZE+2));
    

    
    strcpy(config_list[0].label, "Function");
    config_list[0].selected_index = 0;
    config_list[0].value = 0;
    config_list[0].items_array_length = 2;
    config_list[0].items_array = (struct selection_items *)malloc(sizeof(struct selection_items) * config_list[0].items_array_length);

    config_list[0].items_array[0].associated_selection = &(config_list[0]);
    strcpy(config_list[0].items_array[0].label, "Reflow");
    config_list[0].items_array[0].value = 0;

    config_list[0].items_array[1].associated_selection = &(config_list[0]);
    strcpy(config_list[0].items_array[1].label, "Heat");
    config_list[0].items_array[1].value = 1;

   



    strcpy(config_list[1].label, "Profile");
    config_list[1].selected_index = 1;
    config_list[1].value = 1;
    config_list[1].items_array_length = profiles_num;
    config_list[1].items_array = (struct selection_items *)malloc(sizeof(struct selection_items) * config_list[1].items_array_length);

    for(uint32_t i = 0; i < config_list[1].items_array_length; i++){
        config_list[1].items_array[i].associated_selection = &(config_list[1]);
        strcpy(config_list[1].items_array[i].label, profiles[i].label);
        config_list[1].items_array[i].value = i;
        config_list[1].items_array[i].data = &profiles[i];
    }
   

    strcpy(config_list[2].label, "Sensor");
    config_list[2].selected_index = 1;
    config_list[2].value = 2;
    config_list[2].items_array_length = 2;
        config_list[2].items_array = (struct selection_items *)malloc(sizeof(struct selection_items) * config_list[2].items_array_length);

    config_list[2].items_array[0].associated_selection = &(config_list[2]);
    strcpy(config_list[2].items_array[0].label, "Internal");
    config_list[2].items_array[0].value = 0;

    config_list[2].items_array[1].associated_selection = &(config_list[2]);
    strcpy(config_list[2].items_array[1].label, "External");
    config_list[2].items_array[1].value = 1;

 
    strcpy(config_list[3].label, "Fans");
    config_list[3].selected_index = 0;
    config_list[3].value = 3;
    config_list[3].items_array_length = 2;
    config_list[3].items_array = (struct selection_items *)malloc(sizeof(struct selection_items) * config_list[3].items_array_length);

    config_list[3].items_array[0].associated_selection = &(config_list[3]);
    strcpy(config_list[3].items_array[0].label, "On");
    config_list[3].items_array[0].value = 0;

    config_list[3].items_array[1].associated_selection = &(config_list[3]);
    strcpy(config_list[3].items_array[1].label, "Off");
    config_list[3].items_array[1].value = 1;



    strcpy(config_list[4].label, "Start");
    config_list[4].selected_index = 0;
    config_list[4].value = 0;
    config_list[4].items_array_length = 0;
    config_list[4].items_array = NULL;

    strcpy(config_list[5].label, "Back");
    config_list[5].selected_index = 0;
    config_list[5].value = 0;
    config_list[5].items_array_length = 0;
    config_list[5].items_array = NULL;

    reflow_selected_params = (struct reflow_params *)malloc(sizeof(struct reflow_params));
    reflow_selected_params->fans = 4;
    reflow_selected_params->function = 4;
    reflow_selected_params->heat = 4;
    reflow_selected_params->profile = NULL;
    reflow_selected_params->sensor = 4;

}


float current_temp = 300.0;
float target_temp = 200.0;
uint32_t total_time_ms = 0;
uint32_t elapsed_time_ms = 0;