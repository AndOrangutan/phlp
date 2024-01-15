#pragma once

#include "defines.h"


/**
 * @class platform_state
 * @brief Platform specific state
 *
 */
typedef struct platform_state {
    void* internal_state;
} platform_state;


/**
 * @brief Platform initializer
 *
 * @param plat_state State to set
 * @param application_name For windows?
 * @return // TODO: SET
 */
PAPI b8 platform_init (
    platform_state* plat_state,
    const char* application_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height);

/**
 * @brief Shutdown platform
 *
 * @param plat_state State of platform to shutdown
 */
PAPI void platform_shutdown(platform_state* plat_state);

/**
 * @brief Must be called in the main game loop
 */
PAPI b8 platform_pump_messages(platform_state* plat_state);


/**
 * @brief Basically platform specific malloc()
 */
PAPI void* platform_alloc(u64 size, b8 aligned);

/**
 * @brief Corresponding free to platform_alloc()
 */
PAPI void platform_free(void* block, b8 aligned);

/**
 * @brief Zero a block of memory
 */
PAPI void* platform_zero_memory(void* block, u64 size);

/**
 * @brief Copy a block of memory
 */
PAPI void* platform_copy_memory(void* dest, const void* source, u64 size);

/**
 * @brief Set value of platform specific memory
 */
PAPI void* platform_set_memory(void* dest, i32 value, u64 size);

/**
 * @brief Write colored text to a platform specific console
 */
PAPI void platform_console_write(const char* msg, u8 color);

/**
 * @brief Write colored error to a platform specific console
 */
PAPI void platform_console_write_error(const char* msg, u8 color);


/**
 * @brief Get the absolute time of platform
 *
 * @return [TODO:return]
 */
PAPI f64 platform_get_absolute_time();

/**
 * @brief Sleep on the thread for the provided ms. This blocks the main thread. Not exported.
 *
 * @param ms Time to sleep in miliseconds
 */
PAPI void platform_sleep(u64 ms);
