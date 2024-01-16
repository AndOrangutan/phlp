#pragma once

#include "defines.h"


/**
 * @typedef p_state
 * @brief Platfrom specific state
 *
 */
typedef struct p_state p_state;

/**
 * @brief Platform specific initializer
 *
 * @param application_name Name dispalyed in the window
 * @param x Location of the window
 * @param y Location of the window
 */
PAPI p_state* p_init(const char* application_name, i32 x, i32 y, i32 width, i32 height);

/**
 * @brief Platfrom shutdown
 *
 * @param plat_state Platform to shutdown
 */
PAPI void p_kill(p_state* plat_state);

/**
 * @brief Clear window message queue. Must be called in the main game loop!!!
 *
 * @param plat_state Platform to pump
 */
PAPI b8 p_pump_messages(p_state* plat_state);


/**
 * @brief Platform specific malloc()
 *
 * @param size Size of the memory block
 * @param aligned [TODO:parameter] fill in later
 * @return Allowcation memory block
 */
PAPI void* p_alloc(u64 size, b8 aligned);

/**
 * @brief Platform specific free()
 *
 * @param block Block of memory to free
 * @param aligned [TODO:parameter]
 */
PAPI void p_free(void* block, b8 aligned);

/**
 * @brief Zero a block of memory
 */

/**
 * @brief Platform specific zero memory
 *
 * @param block Block of memory to zero
 * @param size Size of block
 * @return Zeroed memory block
 */
PAPI void* p_zero_memory(void* block, u64 size);

/**
 * @brief Copy a block of memory
 */

/**
* @brief Platform specific memcpy()
 *
* @return Destination memory block
 */
PAPI void* p_copy_memory(void* dest, const void* source, u64 size);

/**
 * @brief Set value of platform specific memory
 */

/**
 * @brief Platform specific memset()
 *
 * @return Destination memory block
 */
PAPI void* p_set_memory(void* dest, i32 value, u64 size);

/**
 * @brief Write colored text to a platform specific console
 */

/**
 * @brief Platform specific console write
 */
PAPI void p_console_write(const char* msg, u8 color);

/**
 * @brief Write colored error to a platform specific console
 */

/**
 * @brief Platform specific console write colored error
 */
PAPI void p_console_write_error(const char* msg, u8 color);


/**
 * @brief Platform absolute time
 */
PAPI f64 p_get_absolute_time();

/**
 * @brief Sleep on the thread for the provided ms. This blocks the main thread. Not exported.
 *
 * @param ms Time to sleep in miliseconds
 */

/**
 * @brief Platform specific sleep
 */
PAPI void p_sleep(u64 ms);
