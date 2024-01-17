#pragma once

#include "defines.h"

typedef struct app_config {
    char* name;
    i16 start_pos_x;
    i16 start_pos_y;
    i16 start_width;
    i16 start_height;
} app_config;

PAPI b8 app_init(app_config* config);

PAPI b8 app_run();
