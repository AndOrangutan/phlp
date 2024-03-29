#include "application.h"
#include "game_types.h"

#include "core/clock.h"
#include "core/event.h"
#include "core/input.h"
#include "core/logger.h"
#include "core/pmemory.h"
#include "platform/platform.h"

#include "renderer/renderer_frontend.h"

typedef struct app_state {
    game *game_inst;
    b8 is_running;
    b8 is_suspended;
    plat_state *platform;
    i16 width;
    i16 height;
    clock clock;
    f64 last_time;
} app_state;

b8 is_initialized = FALSE;

static app_state state;

// Event handlers
b8 application_on_event(u16 code, void *sender, void *sender_inst,
                        event_context context);
b8 application_on_key(u16 code, void *sender, void *sender_inst,
                      event_context context);

b8 app_init(game *game_inst) {

    if (is_initialized) {
        PERROR("Multiple app_init() calls!");
        return FALSE;
    }

    // Game instance
    state.game_inst = game_inst;

    // Initialize Subsystems
    logger_init();
    input_init();

    // TODO: REMOVE
    PFATAL("A test error: %2.3f", 69.420f);
    PERROR("A test error: %2.3f", 69.420f);
    PWARN("A test error: %2.3f", 69.420f);
    PINFO("A test error: %2.3f", 69.420f);
    PDEBUG("A test error: %2.3f", 69.420f);
    PTRACE("A test error: %2.3f", 69.420f);

    state.is_running = TRUE;
    state.is_suspended = FALSE;

    if (!event_init()) {
        PERROR("Event system failed init! Application cannot contine.");
        return FALSE;
    }

    event_register(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_register(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_register(EVENT_CODE_KEY_RELEASED, 0, application_on_key);

    // Initialize events
    state.platform = plat_init(
        game_inst->app_config.name, game_inst->app_config.start_pos_x,
        game_inst->app_config.start_pos_y, game_inst->app_config.start_width,
        game_inst->app_config.start_height);
    if (state.platform == NULL) {
        return FALSE;
    }

    // Renderer startup
    if (!renderer_initialize(game_inst->app_config.name, state.platform)) {
        PFATAL("Failed to initialize renderer! Aborting application.");
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
    clock_start(&state.clock);
    clock_update(&state.clock);
    state.last_time = state.clock.elapsed;
    f64 running_time = 0;
    u8 frame_count = 0;
    f64 target_frame_seconds = 1.0f / 60;

    PINFO(memory_usage_str());

    while (state.is_running) {

        if (!plat_pump_messages(state.platform)) {
            state.is_running = FALSE;
        }

        if (!state.is_suspended) {
            // Update clock and get delta
            clock_update(&state.clock);
            f64 current_time = state.clock.elapsed;
            f64 delta = (current_time - state.last_time);
            f64 frame_start_time = plat_get_absolute_time();

            if (!state.game_inst->update(state.game_inst, (f32)delta)) {
                PFATAL("Game update failed! Shutting down.");
                break;
            }

            // Game's render routine
            if (!state.game_inst->render(state.game_inst, (f32)delta)) {
                PFATAL("Game render failed! Shutting down.");
                break;
            }

            // TODO: Rewrite
            render_packet packet;
            packet.delta_time = delta;

            renderer_draw_frame(&packet);

            // Determine how long frame took and if below
            f64 frame_end_time = plat_get_absolute_time();
            f64 frame_elapsed_time = frame_end_time - frame_start_time;
            running_time += frame_elapsed_time;
            f64 remaining_seconds = target_frame_seconds - frame_elapsed_time;

            if (remaining_seconds > 0) {
                u64 remaining_ms = (remaining_seconds * 1000);

                // If there is time left, give it back to the OS.
                b8 limit_frames = FALSE;
                if (remaining_ms > 0 && limit_frames) {
                    plat_sleep(remaining_ms - 1);
                }

                frame_count++;
            }

            // NOTE: Input update/state copying should always be handled
            // after any input should be recorded; I.E. before this line.
            // As a safety, input is the last thing to be updated before
            // this frame ends.
            input_update(delta);

            // Update last time
            state.last_time = current_time;
        }
    }
    state.is_running = FALSE;

    event_unregister(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_unregister(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_unregister(EVENT_CODE_KEY_RELEASED, 0, application_on_key);

    event_kill();
    input_kill();

    renderer_shutdown();

    plat_kill(state.platform);

    return TRUE;
}

b8 application_on_event(u16 code, void *sender, void *sender_inst,
                        event_context context) {
    switch (code) {
    case EVENT_CODE_APPLICATION_QUIT: {
        PINFO("EVENT_CODE_APPLICATION_QUIT creceived, shutting down.\n");
        state.is_running = FALSE;
        return TRUE;
    }
    }
    return FALSE;
}

b8 application_on_key(u16 code, void *sender, void *sender_inst,
                      event_context context) {
    if (code == EVENT_CODE_KEY_PRESSED) {
        u16 key_code = context.data.u16[0];
        if (key_code == KEY_ESCAPE) {
            // NOTE: Techinically firing an event for itself, but there may be
            // other listeners
            event_context data = {};
            event_fire(EVENT_CODE_APPLICATION_QUIT, 0, data);

            return TRUE;
        } else if (key_code == KEY_A) {
            // example of checking for [a] key
            PDEBUG("Explicity - A key pressed!");
        } else {
            PDEBUG("'%c' key pressed in window.", key_code);
        }
    } else if (code == EVENT_CODE_KEY_RELEASED) {
        u16 key_code = context.data.u16[0];
        if (key_code == KEY_B) {
            // Example of checking for [b] key
            PDEBUG("Explicit - B key releaded!");
        } else {
            PDEBUG("'%c' key released in window.", key_code);
        }
    }
    return FALSE;
}
