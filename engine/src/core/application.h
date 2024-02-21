#pragma once

#include "defines.h"

struct game;

/**
 * @typedef app_config
 * @brief Configuration used to start applications
 *
 */
typedef struct app_config {
    char *name;
    i16 start_pos_x;
    i16 start_pos_y;
    i16 start_width;
    i16 start_height;
} app_config;

/**
 * Initialize the app
 * @return TRUE if successful
 */
PAPI b8 app_init(struct game *game_inst);

/**
 * Run app and handle shutdown
 * @return TRUE on successful shutdown
 */
PAPI b8 app_run();
