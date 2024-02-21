#pragma once

#include "defines.h"

/**
 * @typedef event_context
 * @brief Variable event context of 128 bytes.
 */
typedef struct event_context {
    // 128 bytes
    union {
        i64 i64[2];
        u64 u64[2];
        f64 f64[2];

        i32 i32[4];
        u32 u32[4];
        f32 f32[4];

        i16 i16[8];
        u16 u16[8];

        i8 i8[16];
        u8 u8[16];

        char c[16];
    } data;
} event_context;

/**
 * @typedef PFN_on_event
 * Event interface used by listeners that returns TRUE if handled
 */
typedef b8 (*PFN_on_event)(u16 code,
                           void *sender,
                           void *lister_inst,
                           event_context data);

/**
 * @typedef system_event_code
 * @brief System internal event codes. Codes 1-154. Application should use codes
 * > 255.
 *
 */
typedef enum system_event_code {
    // Shutdown application on the next frame.
    EVENT_CODE_APPLICATION_QUIT = 0x01,

    // Keyboard key pressed.
    /* Context usage:
     * u16 key_code = data.data.u16[0];
     */
    EVENT_CODE_KEY_PRESSED = 0x02,

    // Keyboard key released.
    /* Context usage:
     * u16 key_code = data.data.u16[0];
     */
    EVENT_CODE_KEY_RELEASED = 0x03,

    // Mouse button pressed.
    /* Context usage:
     * u16 button = data.data.u16[0];
     */
    EVENT_CODE_BUTTON_PRESSED = 0x04,

    // Mouse button released.
    /* Context usage:
     * u16 button = data.data.u16[0];
     */
    EVENT_CODE_BUTTON_RELEASED = 0x05,

    // Mouse moved.
    /* Context usage:
     * u16 x = data.data.u16[0];
     * u16 y = data.data.u16[1];
     */
    EVENT_CODE_MOUSE_MOVED = 0x06,

    // Mouse moved.
    /* Context usage:
     * u8 z_delta = data.data.u8[0];
     */
    EVENT_CODE_MOUSE_WHEEL = 0x07,

    // Resized/resolution changed from the OS.
    /* Context usage:
     * u16 width = data.data.u16[0];
     * u16 height = data.data.u16[1];
     */
    EVENT_CODE_RESIZED = 0x08,

    MAX_EVENT_CODE = 0xFF

} system_event_code;

b8 event_init();
void event_kill();

/**
 * Register to listen for when events are sent with the provided code. Events
 * withd duplicate listener/callback bombos will not be registered again and
 * will cause this to return FALSE.
 * @param code Event code to listen for.
 * @param listener A ptr to a listner instance. Can be 0/NULL.
 * @param on_event Callback function pointer to be invoced on event code firing.
 * @return TRUE if the event is successfully registered; else FALSE.
 */
PAPI b8 event_register(u16 code, void *listener, PFN_on_event on_event);

/**
 * Unregister from listening for when events are sent with the provided code. If
 * no matching registration is found, this function returns FALSE.
 * @param code Event code to stop listneing for.
 * @param lisener A ptr to a listener instance. Can be 0/NULL.
 * @param on_event The callback function pointer to be unregistered.
 * @return TRUE if the event is successfully unregistered; otherwise FALSE.
 */
PAPI b8 event_unregister(u16 code, void *lisener, PFN_on_event on_event);

/**
 * Fires an event to listeners of the given code. If an event handler returns
 * TRUE, the event is considered handled and is not passed on to any more
 * listeners
 * @param code Event code to fire.
 * @param sender A ptr to the sender instance. Can be 0/NULL.
 * @param data Event context data.
 * @return TRUE if handled; otherwise FALSE.
 */
PAPI b8 event_fire(u16 code, void *sender, event_context context);
