/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2020 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef XENIA_GPU_GPU_FLAGS_H_
#define XENIA_GPU_GPU_FLAGS_H_
#include "xenia/base/cvar.h"

DECLARE_path(trace_gpu_prefix);
DECLARE_bool(trace_gpu_stream);

DECLARE_path(dump_shaders);

DECLARE_bool(use_50Hz_mode);

DECLARE_bool(vsync);

DECLARE_uint64(framerate_limit);

DECLARE_bool(guest_display_refresh_cap);

void SetGuestDisplayRefreshCap(bool value);
void SetFramerateLimit(uint32_t value);

DECLARE_bool(gpu_allow_invalid_fetch_constants);

DECLARE_bool(gpu_allow_invalid_upload_range);

DECLARE_bool(non_seamless_cube_map);

DECLARE_bool(vulkan_precise_interpolation);

DECLARE_bool(half_pixel_offset);

DECLARE_string(occlusion_query);

DECLARE_int32(occlusion_query_fake_lower_threshold);

DECLARE_int32(occlusion_query_fake_upper_threshold);

DECLARE_bool(occlusion_query_log);

DECLARE_bool(occlusion_query_fast_trust_report);

DECLARE_int32(occlusion_query_querybatch_range);

DECLARE_double(occlusion_query_saturation);

// Returns the guest vblank rate in Hz (50 for PAL, 60 for NTSC).
// Based on use_50Hz_mode cvar.
uint32_t GetGuestVblankRateHz();

DECLARE_int32(anisotropic_override);

DECLARE_bool(disassemble_pm4);

DECLARE_bool(gpu_debug_markers);

// Returns true if GPU debug markers should be enabled.
// Checks the CVAR and also detects if RenderDoc is attached.
// Result is cached after first call for efficiency.
bool IsGpuDebugMarkersEnabled();

DECLARE_string(render_target_path);

DECLARE_bool(no_discard_stencil_in_transfer_pipelines);

DECLARE_bool(submit_on_primary_buffer_end);

DECLARE_bool(force_convert_triangle_strips_to_lists);

DECLARE_bool(async_shader_compilation);

DECLARE_bool(readback_resolve_half_pixel_offset);

DECLARE_bool(gpu_3d_to_2d_texture);

DECLARE_bool(metal_shader_disk_cache);
DECLARE_bool(metal_pipeline_binary_archive);
DECLARE_int32(metal_draw_ring_count);
DECLARE_bool(metal_use_heaps);
DECLARE_int32(metal_heap_min_bytes);
DECLARE_bool(metal_texture_cache_use_private);
DECLARE_bool(metal_texture_upload_via_blit);
DECLARE_bool(metal_constant_payload_cache);
DECLARE_string(metal_residency_sets);
DECLARE_bool(metal_backend_telemetry);
DECLARE_int32(metal_backend_telemetry_interval);
DECLARE_bool(metal_root_rebuild_detail_telemetry);

DECLARE_bool(force_depth_clamp);

#define XE_GPU_FINE_GRAINED_DRAW_SCOPES 1

#endif  // XENIA_GPU_GPU_FLAGS_H_
