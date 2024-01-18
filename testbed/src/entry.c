#include "game.h"

#include <entry.h>

#include <core/pmemory.h>

b8 game_init(game* out_game) {

    // App Configuration
    out_game->app_config.name = "Phlp Engine Testbed";
    out_game->app_config.start_pos_x = 100;
    out_game->app_config.start_pos_y = 100;
    out_game->app_config.start_width = 1280;
    out_game->app_config.start_height = 720;
    out_game->update = game_update;
    out_game->render = game_render;
    out_game->initialize = game_initialize;
    out_game->on_resize = game_on_resize;

    // Create game state
    out_game->state = palloc(sizeof(game_state), MEMORY_TAG_GAME);

    return TRUE;
}
