#pragma once

#include "renderer/renderer_backend.h"

b8 vulkan_renderer_backend_initialize(renderer_backend *backend,
                                      const char *app_name,
                                      struct plat_state *plat_state);
void vulkan_renderer_backed_shutdown(renderer_backend *backend);

void vulkan_renderer_backend_on_resized(renderer_backend *backend,
                                        u16 width,
                                        u16 height);

b8 vulkan_renderer_backend_begin_frame(renderer_backend *backend,
                                       f32 delta_time);
b8 vulkan_renderer_backend_end_frame(renderer_backend *backend, f32 delta_time);
