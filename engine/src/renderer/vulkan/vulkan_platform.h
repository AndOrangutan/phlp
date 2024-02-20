#pragma once

#include "defines.h"

struct plat_state;
struct vulkan_context;

b8 plat_create_vulkan_surface(struct plat_state *plat_state,
                              struct vulkan_context *contxet);

/**
 * Appends the names of required extensions for this platform to the
 * names_darray, which should be created and passed in.
 * @param names_darray darray to extend
 */
void plat_get_required_extension_names(const char ***names_darray);
