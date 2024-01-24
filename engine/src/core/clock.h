#pragma once

#include "defines.h"

/**
 * @class clock
 * @brief Engine clock
 *
 */
typedef struct clock {
    f64 start_time;
    f64 elapsed;
} clock;

/**
 * Update the provided clock. Should be called just before checking elapsed
 * time.
 * Has no effect on non-started clocks.
 */
void clock_update(clock *clock);

/**
 * Start the provided clock. Reset elapsed time.
 */
void clock_start(clock *clock);

/**
 * Stop the provided clock. Does not reset elapsed time.
 */
void clock_stop(clock *clock);
