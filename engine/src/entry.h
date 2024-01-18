#pragma once

#include "core/application.h"
#include "core/logger.h"
#include "core/pmemory.h"
#include "game_types.h"

extern b8 game_init(game* out_game);

int main(int argc, char *argv[]) {
    /* ("Phlp Engine Testbed", 100, 100, 1280, 720) */

    memory_init();

    // Request game inst
    game game_inst;

    if (!game_init(&game_inst)) {
        PFATAL("Could not create game!");
        return -1;
    }
    
    // Ensure function pointers existance
    if (!game_inst.render || !game_inst.update || !game_inst.initialize || !game_inst.on_resize) {
        PFATAL("All game function pointers must be assingmed!");
        return -2;
    }

    // Initialization
    if (!app_init(&game_inst)) {
        PINFO("Application failed to create!\n");
        return 1;
    }

    // Application configuration
    if(!app_run()) {
        PINFO("Application did not shutdown gracefully");
        return 2;
    }

    app_run();

    memory_kill();

    return 0;
}
