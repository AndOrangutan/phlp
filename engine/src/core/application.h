#pragma once

#include "defines.h"

/**
 * @typedef app_config
 * @brief Configuration used to start applications
 *
 */
typedef struct app_config {
    char* name;
    i16 start_pos_x;
    i16 start_pos_y;
    i16 start_width;
    i16 start_height;
} app_config;

/**
 * @brief Initialize the app
 *
 * @return TRUE if successful
 */
PAPI b8 app_init(app_config* config);

/**
 * @brief Run app and handle shutdown
 *
 * @return TRUE on successful shutdown
 */
PAPI b8 app_run();
