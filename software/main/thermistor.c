#include "thermistor.h"



struct mv_to_temp EPCOS_100K_B57560G104F[] = {
    {100.11,     -55},
    {69.56,      -50},
    {48.945,     -45},
    {34.853,     -40},
    {25.102,     -35},
    {18.275,     -30},
    {13.443,     -25},
    {9.9853,     -20},
    {7.4867,     -15},
    {5.6636,     -10},
    {4.3212,     -5},
    {3.324,      0},
    {2.5769,     5},
    {2.0127,     10},
    {1.5834,     15},
    {1.2542,     20},
    {1.0000,     25},
    {0.80239,    30},
    {0.64776,    35},
    {0.52598,    40},
    {0.4295,     45},
    {0.35262,    50},
    {0.291,      55},
    {0.24136,    60},
    {0.20114,    65},
    {0.16841,    70},
    {0.14164,    75},
    {0.11963,    80},
    {0.10147,    85},
    {0.086407,   90},
    {0.073867,   95},
    {0.063383,   100},
    {0.054584,   105},
    {0.04717,    110},
    {0.040901,   115},
    {0.035581,   120},
    {0.03105,    125},
    {0.027179,   130},
    {0.023861,   135},
    {0.021008,   140},
    {0.018548,   145},
    {0.016419,   150},
    {0.014573,   155},
    {0.012967,   160},
    {0.011566,   165},
    {0.010341,   170},
    {0.0092664,  175},
    {0.0083224,  180},
    {0.0074907,  185},
    {0.0067564,  190},
    {0.0061064,  195},
    {0.0055299,  200},
    {0.0050175,  205},
    {0.0045611,  210},
    {0.0041537,  215},
    {0.0037895,  220},
    {0.0034631,  225},
    {0.0031701,  230},
    {0.0029067,  235},
    {0.0026693,  240},
    {0.0024551,  245},
    {0.0022615,  250},
    {0.0020862,  255},
    {0.0019273,  260},
    {0.0017829,  265},
    {0.0016516,  270},
    {0.0015319,  275},
    {0.0014228,  280},
    {0.0013231,  285},
    {0.0012319,  290},
    {0.0011483,  295},
    {0.0010716,  300}
};



#define R1 10000.0
#define VS 3300.0
#define R25 100000.0


float mv_to_temp(uint32_t mv, struct mv_to_temp *table, uint32_t table_size){
    struct mv_to_temp *low_cell, *high_cell;
    float R2 = (R1*(float)mv)/(VS - (float)mv);
    float ratio = R2/R25;

    uint32_t i;
    for(i = 1; i < table_size; i++){
        if(table[i].ratio <= ratio){
            break;
        }
    }
    
    low_cell = &table[i-1];
    high_cell = &table[i];

    float res = ( (low_cell->temp)*(high_cell->ratio - ratio) + (high_cell->temp)*(ratio - low_cell->ratio) )/(high_cell->ratio - low_cell->ratio);
    
    //printf("RES:%f LOW:%f  HIGH:%f RATIO:%f\n", res, low_cell->ratio, high_cell->ratio, ratio);

    return res;

}

float mv_to_temp_EPCOS_100K_B57560G104F(uint32_t mv){
    return mv_to_temp(mv, EPCOS_100K_B57560G104F, sizeof(EPCOS_100K_B57560G104F)/sizeof(struct mv_to_temp));
}


//float thermistor_






