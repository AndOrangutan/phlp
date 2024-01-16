#pragma once

#include "defines.h"


/**
 * @typedef plat_state
 * @brief Platfrom specific state
 *
 */
typedef struct plat_state plat_state;

/**
 * @brief Platform specific initializer
 *
 * @param application_name Name dispalyed in the window
 * @param x Location of the window
 * @param y Location of the window
 */
PAPI plat_state* plat_init(const char* application_name, i32 x, i32 y, i32 width, i32 height);

/**
 * @brief Platfrom shutdown
 *
 * @param plat_state Platform to shutdown
 */
PAPI void plat_kill(plat_state* state);

/**
 * @brief Clear window message queue. Must be called in the main game loop!!!
 *
 * @param plat_state Platform to pump
 */
PAPI b8 plat_pump_messages(plat_state* state);


/**
 * @brief Platform specific malloc()
 *
 * @param size Size of the memory block
 * @param aligned [TODO:parameter] fill in later
 * @return Allowcation memory block
 */
PAPI void* plat_alloc(u64 size, b8 aligned);

/**
 * @brief Platform specific free()
 *
 * @param block Block of memory to free
 * @param aligned [TODO:parameter]
 */
PAPI void plat_free(void* block, b8 aligned);

/**
 * @brief Platform specific zero memory
 *
 * @param block Block of memory to zero
 * @param size Size of block
 * @return Zeroed memory block
 */
PAPI void* plat_zero_memory(void* block, u64 size);

/**
* @brief Platform specific memcpy()
 *
* @return Destination memory block
 */
PAPI void* plat_copy_memory(void* dest, const void* source, u64 size);

/**
 * @brief Platform specific memset()
 *
 * @return Destination memory block
 */
PAPI void* plat_set_memory(void* dest, i32 value, u64 size);


/**
 * @brief Platform specific console write
 */
PAPI void plat_console_write(const char* msg, u8 color);

/**
 * @brief Write colored error to a platform specific console
 */

/**
 * @brief Platform specific console write colored error
 */
PAPI void plat_console_write_error(const char* msg, u8 color);


/**
 * @brief Platform absolute time
 */
PAPI f64 plat_get_absolute_time();

/**
 * @brief Sleep on the thread for the provided ms. This blocks the main thread. Not exported.
 *
 * @param ms Time to sleep in miliseconds
 */

/**
 * @brief Platform specific sleep on thread for provided ms. Main thread blocking
 */
PAPI void plat_sleep(u64 ms);
