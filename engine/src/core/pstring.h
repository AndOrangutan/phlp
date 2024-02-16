#pragma once

#include "defines.h"

// TODO: Document

PAPI u64 str_length(const char *str);

PAPI char *str_duplicate(const char *str);

/**
 * Case-sensitive string comparison.
 * @return TRUE if same, else FALSE
 */
PAPI b8 str_equals(const char *str0, const char *str1);
