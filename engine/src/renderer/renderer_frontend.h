#pragma once

#include "renderer_types.inl"

struct static_mesh_data;
struct plat_state;

b8 renderer_initialize(const char *app_name, struct plat_state *plat_state);
void renderer_shutdown();

void renderer_on_resize(u64 width, u64 height);

b8 renderer_draw_frame(render_packet *packet);
