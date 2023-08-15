#include "defines.h"

lv_indev_t * my_indev = NULL;



volatile uint32_t MENUCHANGED_DISABLEBUTTONS = 0;


struct reflow_profile * profiles = NULL;

uint32_t profile_selected = 0;

uint32_t profiles_num;

volatile float internal_temp = 0;
volatile float external_temp = 0;



struct selection *config_list = NULL;

volatile struct reflow_params *reflow_selected_params = NULL;


enum reflow_states reflow_state;
enum parser_state{none, array, profile, label, phase, error};
enum parser_state parser_state_ = none;


esp_err_t init_sdcard_data(){
    char *file_buff = NULL;
    FILE *f = fopen("/sdcard/FOO.TXT", "r");
    if(f == NULL){
        printf("File error!\n");
        return ESP_FAIL;
    }
    printf("File opened\n");
    

    fseek(f, 0, SEEK_END);
    int f_size = ftell(f);
    fseek(f, 0, SEEK_SET); 

    file_buff = (char *)malloc(f_size*sizeof(char));
    fread(file_buff, f_size+1,1,f);
    jsmn_parser p;
    jsmntok_t *t = (jsmntok_t *)malloc(sizeof(jsmntok_t) * 1000);
    int ret;
    int i,j;

    jsmn_init(&p);
    ret = jsmn_parse(&p, file_buff, f_size, t, 1000 );

    if(ret < 0){
        printf("Failed to parse JSON: %d\n", ret);
        goto fail;
    }
    if (ret < 1 || t[0].type != JSMN_ARRAY) {
        printf("Array expected\n");
        goto fail;
    }

    //for(i = 0; i < ret; i++){
    //    printf("%d TYPE: ", i);
    //        switch (t[i].type){
    //        case JSMN_UNDEFINED:
    //            printf(" UNDEFINED ");
    //            break;
    //        case JSMN_OBJECT:
    //            printf(" OBJECT    ");
    //            break;
    //        case JSMN_ARRAY:
    //            printf(" ARRAY     ");
    //            break;
    //        case JSMN_STRING:
    //            printf(" STRING    ");
    //            break;
    //        case JSMN_PRIMITIVE:
    //            printf(" PRIMITIVE ");
    //            break;        
    //        default:
    //            printf("           ");
    //            break;
    //        }
    //    printf(" START: %d END: %d SIZE: %d \n", t[i].start, t[i].end, t[i].size); 
    //}

    if(t[1].type != JSMN_OBJECT){
        printf("Parsing Error 1\n");
        goto fail;
    }

    profiles_num = 0;
    enum reflow_states reflow_state_index = preheat;

    for(i = 0; i < ret; i++){
        switch (parser_state_){
        case none:
            //printf("CASE NONE %d\n", i);
            if(t[i].type == JSMN_ARRAY){
                parser_state_ = array;
                //allocate resouces
                profiles = (struct reflow_profile *)malloc(sizeof(struct reflow_profile)*t[i].size);

                if(profiles == NULL){
                    printf("Cannot allocate resources! %d\n", i);
                    parser_state_ = error;
                }

            }else{
                printf("Expected array! %d\n", i);
                parser_state_ = error;
                goto fail;
            }
            break;
        case array:
            //printf("CASE ARRAY %d\n", i);
            if(t[i].type == JSMN_OBJECT){
                parser_state_ = label;
            }else{
                printf("Expected object! %d\n", i);
                parser_state_ = error;
                goto fail;
            }
            break;
        case profile:
            //printf("CASE PROFILE %d\n", i);
            if(t[i].type == JSMN_OBJECT){
                parser_state_ = label;
            }else{
                printf("Expected object! %d\n", i);
                parser_state_ = error;
                goto fail;
            }
            break;
        case label:
            //printf("CASE LABEL %d\n", i);
            if(i + 1 > ret){
                printf("Out of bounds on tokens array! %d\n", i);
                parser_state_ = error;
                goto fail;
                break;
            }
            if((t[i].type == JSMN_STRING) && (t[i+1].type == JSMN_STRING)){
                file_buff[t[i+1].end] = '\0';
                
                strncpy(profiles[profiles_num].label, &file_buff[t[i+1].start],(t[i+1].end) - t[i+1].start + 1); 
                //printf("%s, %d\n",profiles[profiles_index].label, i);
                i = i + 1;
            }else if((t[i].type == JSMN_STRING) && (t[i+1].type == JSMN_OBJECT)){
                if(strncmp("preheat", &file_buff[t[i].start], t[i].end - t[i].start) == 0){
                    //printf("phase_preheat\n");
                    reflow_state_index = preheat;
                }else if(strncmp("soak", &file_buff[t[i].start], t[i].end - t[i].start) == 0){
                    //printf("phase_soak\n");
                    reflow_state_index = soak;
                }else if(strncmp("reflow", &file_buff[t[i].start], t[i].end - t[i].start) == 0){
                    //printf("phase_reflow\n");
                    reflow_state_index = reflow;
                }else if(strncmp("reflow_ramp", &file_buff[t[i].start], t[i].end - t[i].start) == 0){
                    //printf("phase_reflow_ramp\n");
                    reflow_state_index = reflow_ramp;
                }else if(strncmp("cooldown", &file_buff[t[i].start], t[i].end - t[i].start) == 0){
                    //printf("phase_cooldown\n");
                    reflow_state_index = cooldown;
                }else{
                    printf("Unknown key! %d %c\n", i, file_buff[t[i].start]);
                    parser_state_ = error;
                    break;
                }
                i = i + 1;
                parser_state_ = phase;
            }else if(t[i].type == JSMN_OBJECT){
                parser_state_ = label;
                profiles_num++;
            }else{
                printf("Expected object or strings! %d\n", i);
                parser_state_ = error;
                goto fail;
            }
            break;
        case phase:
            //printf("CASE PHASE %d\n", i);
            if((i + 5 > ret) ){
                printf("Out of bounds on tokens array! %d\n", i);
                parser_state_ = error;
                break;
            }
            if((t[i].type == JSMN_STRING) && (t[i+1].type == JSMN_PRIMITIVE)){
                profiles[profiles_num].phase[reflow_state_index].start_temp = atoi(&file_buff[t[i+1].start]);
                //printf("%d\n",profiles[profiles_num].phase[reflow_state_index].start_temp);
            }
            if((t[i+2].type == JSMN_STRING) && (t[i+3].type == JSMN_PRIMITIVE)){
                profiles[profiles_num].phase[reflow_state_index].finish_temp = atoi(&file_buff[t[i+3].start]);
                //printf("%d\n",profiles[profiles_num].phase[reflow_state_index].finish_temp);
            }
            if((t[i+4].type == JSMN_STRING) && (t[i+5].type == JSMN_PRIMITIVE)){
                profiles[profiles_num].phase[reflow_state_index].time = atoi(&file_buff[t[i+5].start]);
                //printf("%d\n",profiles[profiles_num].phase[reflow_state_index].time);
            }
            i = i + 5;
            parser_state_ = label;
            break;
        case error:
            printf("CASE ERROR %d\n", i);
            goto fail;
            break;
        default:
            goto fail;
            break;
        }
    }
    profiles_num++;
    
    //for(i = 0; i < profiles_num; i ++){
    //    printf("%s\n", profiles[i].label);
    //    for(j = 0; j < 5; j++){
    //       printf("    PREHEAT: %d %d %d\n", profiles[i].phase[j].start_temp, profiles[i].phase[j].finish_temp, profiles[i].phase[j].time);
    //    }  
    //}

    goto end;
    fail:
    if(file_buff != NULL){
        free(file_buff);
    }
    if(f != NULL){
        fclose(f);
        printf("File closed %d\n", f_size);
    }
    if(t != NULL){
        free(t);
    }
    return ESP_FAIL;

    end:
    return ESP_OK;
}

esp_err_t init_data(){
    config_list = (struct selection *)malloc(sizeof(struct selection) * (CONFIG_LIST_SIZE+2));
    if(config_list == NULL){
        return ESP_FAIL;
    }
    
    strcpy(config_list[0].label, "Function");
    config_list[0].selected_index = 0;
    config_list[0].value = 0;
    config_list[0].items_array_length = 2;
    config_list[0].items_array = (struct selection_items *)malloc(sizeof(struct selection_items) * config_list[0].items_array_length);
    if(config_list[0].items_array == NULL){
        return ESP_FAIL;
    }

    config_list[0].items_array[0].associated_selection = &(config_list[0]);
    strcpy(config_list[0].items_array[0].label, "Reflow");
    config_list[0].items_array[0].value = 0;

    config_list[0].items_array[1].associated_selection = &(config_list[0]);
    strcpy(config_list[0].items_array[1].label, "Heat");
    config_list[0].items_array[1].value = 1;

   
    strcpy(config_list[1].label, "Profile");
    config_list[1].selected_index = 0;
    config_list[1].value = 1;
    config_list[1].items_array_length = profiles_num;
    config_list[1].items_array = (struct selection_items *)malloc(sizeof(struct selection_items) * config_list[1].items_array_length);
    if(config_list[1].items_array == NULL){
        return ESP_FAIL;
    }

    for(uint32_t i = 0; i < config_list[1].items_array_length; i++){
        config_list[1].items_array[i].associated_selection = &(config_list[1]);
        strcpy(config_list[1].items_array[i].label, profiles[i].label);
        config_list[1].items_array[i].value = i;
        if(profiles == NULL){
            return ESP_FAIL;
        }
        config_list[1].items_array[i].data = &profiles[i];
    }
   

    strcpy(config_list[2].label, "Sensor");
    config_list[2].selected_index = 1;
    config_list[2].value = 2;
    config_list[2].items_array_length = 2;
    config_list[2].items_array = (struct selection_items *)malloc(sizeof(struct selection_items) * config_list[2].items_array_length);
    if(config_list[2].items_array == NULL){
        return ESP_FAIL;
    }
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
    if(config_list[3].items_array == NULL){
        return ESP_FAIL;
    }
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
    if(reflow_selected_params == NULL){
        return ESP_FAIL;
    }
    reflow_selected_params->fans = 4;
    reflow_selected_params->function = 4;
    reflow_selected_params->heat = 4;
    reflow_selected_params->profile = &profiles[profile_selected];
    reflow_selected_params->sensor = 4;
    return ESP_OK;
}

esp_err_t init_data_generic(){
    profiles = (struct reflow_profile *)malloc(sizeof(struct reflow_profile)*1);
    if(profiles == NULL){
        return ESP_FAIL;
    }
    strcpy(profiles[0].label, "Generic");
    profiles[0].phase[preheat].start_temp = 0;
    profiles[0].phase[preheat].finish_temp = 150;
    profiles[0].phase[preheat].time = 60;

    profiles[0].phase[soak].start_temp = 150;
    profiles[0].phase[soak].finish_temp = 150;
    profiles[0].phase[soak].time = 60;

    profiles[0].phase[reflow_ramp].start_temp = 150;
    profiles[0].phase[reflow_ramp].finish_temp = 250;
    profiles[0].phase[reflow_ramp].time = 30;

    profiles[0].phase[reflow].start_temp = 250;
    profiles[0].phase[reflow].finish_temp = 250;
    profiles[0].phase[reflow].time = 30;

    profiles[0].phase[cooldown].start_temp = 250;
    profiles[0].phase[cooldown].finish_temp = 0;
    profiles[0].phase[cooldown].time = 90;

    profiles_num = 1;


    return ESP_OK;
}

float current_temp = 300.0;
float target_temp = 200.0;
uint32_t total_time_ms = 0;
uint32_t elapsed_time_ms = 0;