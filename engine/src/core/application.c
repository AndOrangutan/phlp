#include "application.h"

#include "core/logger.h"
#include "platform/platform.h"

typedef struct app_state {
    b8 is_running;
    b8 is_suspended;
    plat_state *platform;
    i16 width;
    i16 height;
    f64 last_time;
} app_state;

b8 is_initialized = FALSE;
static app_state state;

b8 app_init(app_config *config) {

    if (is_initialized) {
        PERROR("Multiple app_init() calls!");
        return FALSE;
    }

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

    state.platform =
        plat_init(config->name, config->start_pos_x, config->start_pos_y,
                  config->start_width, config->start_height);

    if (state.platform == NULL) {

        return FALSE;
    }

    is_initialized = TRUE;
    return TRUE;
}

b8 app_run() {

    while (state.is_running) {

        if (!plat_pump_messages(state.platform)) {
            state.is_running = FALSE;
        }
    }
    state.is_running = FALSE;

    plat_kill(state.platform);

    return TRUE;
}
