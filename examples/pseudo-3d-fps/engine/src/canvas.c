#include "../include/engine/canvas.h"
#include "dh/claim/unreachable.h"

#include <math.h>

// canvas.c
ResErr_Ptr_engine_Canvas engine_Canvas_create(u32 width, u32 height, engine_CanvasType type) {
    engine_Canvas* const canvas = (engine_Canvas*)malloc(sizeof(engine_Canvas));
    if (!canvas) {
        return ResErr_Ptr_engine_Canvas_err(Err_OutOfMemory);
    }

    const Ptr_engine_Canvas self = Ptr_engine_Canvas_from(canvas);
    canvas->width                = width;
    canvas->height               = height;
    canvas->type                 = type;

    // Calculate buffer size based on type
    usize pixel_size = 0;
    switch (type) {
    case engine_CanvasType_rgba:
        pixel_size = sizeof(engine_ColorRgba);
        break;
    case engine_CanvasType_indexed8:
    case engine_CanvasType_indexed16:
    case engine_CanvasType_indexed256:
        pixel_size = sizeof(engine_ColorIndexed);
        break;
    default:
        free(canvas);
        return ResErr_Ptr_engine_Canvas_err(Err_InvalidArgument);
    }

    canvas->buffer_size = (usize)(width * height) * pixel_size;
    canvas->buffer      = malloc(canvas->buffer_size);
    if (!canvas->buffer) {
        free(canvas);
        return ResErr_Ptr_engine_Canvas_err(Err_OutOfMemory);
    }

    // Initialize conversion functions based on type
    canvas->pixel_to_color = null; // Would be implemented based on type
    canvas->color_to_pixel = null; // Would be implemented based on type

    return ResErr_Ptr_engine_Canvas_ok(self);
}

void engine_Canvas_destroy(Ptr_engine_Canvas canvas) {
    if (!canvas.addr) { return; }

    if (canvas.addr->buffer) {
        free(canvas.addr->buffer);
    }
    free(canvas.addr);
}

void engine_Canvas_clear(Ptr_engine_Canvas canvas, engine_ColorValue color) {
    debug_assert_nonnull(canvas.addr);
    debug_assert_nonnull(canvas.addr->buffer);

    const u32 pixel_count = canvas.addr->width * canvas.addr->height;

    switch (canvas.addr->type) {
    case engine_CanvasType_rgba: {
        engine_ColorRgba* const buffer = canvas.addr->buffer;
        for (u32 i = 0; i < pixel_count; ++i) {
            buffer[i] = color.rgba;
        }
    } break;

    case engine_CanvasType_indexed8:
    case engine_CanvasType_indexed16:
    case engine_CanvasType_indexed256: {
        engine_ColorIndexed* const buffer = canvas.addr->buffer;
        for (u32 i = 0; i < pixel_count; ++i) {
            buffer[i] = color.indexed;
        }
    } break;

    default: {
        claim_unreachable_msg("Invalid canvas type");
    } break;
    }
}

void engine_Canvas_drawPixel(Ptr_engine_Canvas canvas, i32 x, i32 y, engine_ColorValue color) {
    debug_assert_nonnull(canvas.addr);
    debug_assert_nonnull(canvas.addr->buffer);

    if (x < 0 || y < 0 || x >= as(i32, canvas.addr->width) || y >= as(i32, canvas.addr->height)) {
        return;
    }

    const u32 index = as(u32, y) * canvas.addr->width + as(u32, x);

    switch (canvas.addr->type) {
    case engine_CanvasType_rgba: {
        engine_ColorRgba* const buffer = canvas.addr->buffer;
        buffer[index]                  = color.rgba;
    } break;

    case engine_CanvasType_indexed8:
    case engine_CanvasType_indexed16:
    case engine_CanvasType_indexed256: {
        engine_ColorIndexed* const buffer = canvas.addr->buffer;
        buffer[index]                     = color.indexed;
    } break;

    default: {
        claim_unreachable_msg("Invalid canvas type");
    } break;
    }
}

void engine_Canvas_resize(Ptr_engine_Canvas canvas, u32 width, u32 height) {
    debug_assert_nonnull(canvas.addr);
    debug_assert_nonnull(canvas.addr->buffer);

    const usize  new_size   = (usize)width * (usize)height * (canvas.addr->type == engine_CanvasType_rgba ? sizeof(engine_ColorRgba) : sizeof(engine_ColorIndexed));
    anyptr const new_buffer = realloc(canvas.addr->buffer, new_size);
    if (new_buffer) {
        canvas.addr->buffer      = new_buffer;
        canvas.addr->width       = width;
        canvas.addr->height      = height;
        canvas.addr->buffer_size = new_size;
    }
}

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

void engine_Canvas_blit(Ptr_engine_Canvas dst, const Ptr_engine_Canvas src, i32 x, i32 y) {
    debug_assert_nonnull(dst.addr);
    debug_assert_nonnull(src.addr);
    debug_assert_nonnull(dst.addr->buffer);
    debug_assert_nonnull(src.addr->buffer);

    // Calculate intersection rectangle
    const i32 src_right  = min(x + as(i32, src.addr->width), as(i32, dst.addr->width));
    const i32 src_bottom = min(y + as(i32, src.addr->height), as(i32, dst.addr->height));
    const i32 start_x    = max(0, x);
    const i32 start_y    = max(0, y);

    // Handle same type direct copy
    if (dst.addr->type == src.addr->type) {
        for (i32 py = start_y; py < src_bottom; ++py) {
            const i32   src_y          = py - y;
            const usize src_row_offset = as(usize, src_y) * src.addr->width;
            const usize dst_row_offset = as(usize, py) * dst.addr->width;

            const usize row_size = as(usize, src_right - start_x) * (dst.addr->type == engine_CanvasType_rgba ? sizeof(engine_ColorRgba) : sizeof(engine_ColorIndexed));

            memcpy(
                (u8*)dst.addr->buffer + (dst_row_offset + as(usize, start_x)) * (dst.addr->type == engine_CanvasType_rgba ? sizeof(engine_ColorRgba) : sizeof(engine_ColorIndexed)),
                (u8*)src.addr->buffer + (src_row_offset + as(usize, start_x - x)) * (src.addr->type == engine_CanvasType_rgba ? sizeof(engine_ColorRgba) : sizeof(engine_ColorIndexed)),
                row_size
            );
        }
        return;
    }

    // Different types need pixel format conversion
    for (i32 py = start_y; py < src_bottom; ++py) {
        for (i32 px = start_x; px < src_right; ++px) {
            const i32 src_x   = px - x;
            const i32 src_y   = py - y;
            const u32 src_idx = as(u32, src_y) * src.addr->width + as(u32, src_x);

            engine_ColorValue color = { 0 };
            switch (src.addr->type) {
            case engine_CanvasType_rgba:
                color.rgba = ((engine_ColorRgba*)src.addr->buffer)[src_idx];
                break;
            default:
                color.indexed = ((engine_ColorIndexed*)src.addr->buffer)[src_idx];
                break;
            }
            engine_Canvas_drawPixel(dst, px, py, color);
        }
    }
}

void engine_Canvas_blitScaled(Ptr_engine_Canvas dst, const Ptr_engine_Canvas src, i32 x, i32 y, f32 scale) {
    debug_assert_nonnull(dst.addr);
    debug_assert_nonnull(src.addr);
    debug_assert_nonnull(dst.addr->buffer);
    debug_assert_nonnull(src.addr->buffer);
    debug_assert(scale > 0);

    const i32 scaled_width  = (i32)((f32)src.addr->width * scale);
    const i32 scaled_height = (i32)((f32)src.addr->height * scale);

    // Calculate destination bounds
    const i32 dst_right  = min(x + scaled_width, as(i32, dst.addr->width));
    const i32 dst_bottom = min(y + scaled_height, as(i32, dst.addr->height));
    const i32 start_x    = max(0, x);
    const i32 start_y    = max(0, y);

    for (i32 dy = start_y; dy < dst_bottom; dy++) {
        for (i32 dx = start_x; dx < dst_right; dx++) {
            // Calculate source pixel
            const i32 src_x = (i32)((f32)(dx - x) / scale);
            const i32 src_y = (i32)((f32)(dy - y) / scale);

            if (src_x >= as(i32, src.addr->width) || src_y >= as(i32, src.addr->height)) {
                continue;
            }

            const u32 src_idx = as(u32, src_y) * src.addr->width + as(u32, src_x);

            engine_ColorValue color;
            switch (src.addr->type) {
            case engine_CanvasType_rgba:
                color.rgba = ((engine_ColorRgba*)src.addr->buffer)[src_idx];
                break;
            default:
                color.indexed = ((engine_ColorIndexed*)src.addr->buffer)[src_idx];
                break;
            }

            engine_Canvas_drawPixel(dst, dx, dy, color);
        }
    }
}
