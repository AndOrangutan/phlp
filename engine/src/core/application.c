#include "application.h"
#include "game_types.h"

#include "core/logger.h"
#include "platform/platform.h"

typedef struct app_state {
    game *game_inst;
    b8 is_running;
    b8 is_suspended;
    plat_state *platform;
    i16 width;
    i16 height;
    f64 last_time;
} app_state;

b8 is_initialized = FALSE;

static app_state state;

b8 app_init(game *game_inst) {

    if (is_initialized) {
        PERROR("Multiple app_init() calls!");
        return FALSE;
    }
    
    // Game instance
    state.game_inst = game_inst;

    // Initialize Subsystems
    logger_init();

    // TODO: REMOVE
    PFATAL("A test error: %2.3f", 69.420f);
    PERROR("A test error: %2.3f", 69.420f);
    PWARN("A test error: %2.3f", 69.420f);
    PINFO("A test error: %2.3f", 69.420f);
    PDEBUG("A test error: %2.3f", 69.420f);
    PTRACE("A test error: %2.3f", 69.420f);

    state.is_running = TRUE;
    state.is_suspended = FALSE;

    state.platform = plat_init(
        game_inst->app_config.name, game_inst->app_config.start_pos_x,
        game_inst->app_config.start_pos_y, game_inst->app_config.start_width,
        game_inst->app_config.start_height);
    if (state.platform == NULL) {
        return FALSE;
    }

    // NOTE: Initialize the game
    if (!state.game_inst->initialize(state.game_inst)) {
        PFATAL("Initialization failed!");
        return FALSE;
    }

    // On Resize handler
    state.game_inst->on_resize(state.game_inst, state.width, state.height);

    is_initialized = TRUE;

    return TRUE;
}

b8 app_run() {

    while (state.is_running) {

        if (!plat_pump_messages(state.platform)) {
            state.is_running = FALSE;
        }

        if (!state.is_suspended) {
            if (!state.game_inst->update(state.game_inst, (f32)0)) {
                PFATAL("Game update failed! Shutting down.");
                break;
            }

            if (!state.game_inst->render(state.game_inst, (f32)0)) {
                PFATAL("Game render failed! Shutting down.");
                break;
            }
        }
    }
    state.is_running = FALSE;

    plat_kill(state.platform);

    return TRUE;
}
