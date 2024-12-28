#include "../include/engine/canvas.h"

#include <math.h>

// canvas.c
Err$Ptr$engine_Canvas engine_Canvas_create(u32 width, u32 height, engine_CanvasType type) {
    reserveReturn(Err$Ptr$engine_Canvas);

    let canvas = (engine_Canvas*)malloc(sizeof(engine_Canvas));
    if (!canvas) { return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory)); }

    canvas->width  = width;
    canvas->height = height;
    canvas->type   = type;

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
        claim_assert_fmt(false, "Invalid canvas type");
    }

    canvas->buffer_size = (usize)(width * height) * pixel_size;
    canvas->buffer      = malloc(canvas->buffer_size);
    if (!canvas->buffer) {
        free(canvas);
        return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
    }

    // Initialize conversion functions based on type
    canvas->pixel_to_color = null; // Would be implemented based on type
    canvas->color_to_pixel = null; // Would be implemented based on type

    return_ok(canvas);
}

void engine_Canvas_destroy(engine_Canvas* canvas) {
    if (!canvas) { return; }
    if (canvas->buffer) {
        free(canvas->buffer);
    }
    free(canvas);
}

void engine_Canvas_clear(engine_Canvas* canvas, engine_ColorValue color) {
    debug_assert_nonnull(canvas);
    debug_assert_nonnull(canvas->buffer);

    const u32 pixel_count = canvas->width * canvas->height;

    switch (canvas->type) {
    case engine_CanvasType_rgba: {
        engine_ColorRgba* const buffer = canvas->buffer;
        for (u32 i = 0; i < pixel_count; ++i) {
            buffer[i] = color.rgba;
        }
    } break;

    case engine_CanvasType_indexed8:
    case engine_CanvasType_indexed16:
    case engine_CanvasType_indexed256: {
        engine_ColorIndexed* const buffer = canvas->buffer;
        for (u32 i = 0; i < pixel_count; ++i) {
            buffer[i] = color.indexed;
        }
    } break;

    default: {
        claim_unreachable_msg("Invalid canvas type");
    } break;
    }
}

void engine_Canvas_drawPixel(engine_Canvas* canvas, i32 x, i32 y, engine_ColorValue color) {
    debug_assert_nonnull(canvas);
    debug_assert_nonnull(canvas->buffer);

    if (x < 0 || y < 0 || x >= as(i32, canvas->width) || y >= as(i32, canvas->height)) {
        return;
    }

    const u32 index = as(u32, y) * canvas->width + as(u32, x);

    switch (canvas->type) {
    case engine_CanvasType_rgba: {
        engine_ColorRgba* const buffer = canvas->buffer;
        buffer[index]                  = color.rgba;
    } break;

    case engine_CanvasType_indexed8:
    case engine_CanvasType_indexed16:
    case engine_CanvasType_indexed256: {
        engine_ColorIndexed* const buffer = canvas->buffer;
        buffer[index]                     = color.indexed;
    } break;

    default: {
        claim_unreachable_msg("Invalid canvas type");
    } break;
    }
}

void engine_Canvas_resize(engine_Canvas* canvas, u32 width, u32 height) {
    debug_assert_nonnull(canvas);
    debug_assert_nonnull(canvas->buffer);

    const usize  new_size   = (usize)width * (usize)height * (canvas->type == engine_CanvasType_rgba ? sizeof(engine_ColorRgba) : sizeof(engine_ColorIndexed));
    anyptr const new_buffer = realloc(canvas->buffer, new_size);
    if (new_buffer) {
        canvas->buffer      = new_buffer;
        canvas->width       = width;
        canvas->height      = height;
        canvas->buffer_size = new_size;
    }
}

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

void engine_Canvas_blit(engine_Canvas* dst, const engine_Canvas* src, i32 x, i32 y) {
    debug_assert_nonnull(dst);
    debug_assert_nonnull(src);
    debug_assert_nonnull(dst->buffer);
    debug_assert_nonnull(src->buffer);

    // Calculate intersection rectangle
    const i32 src_right  = min(x + as(i32, src->width), as(i32, dst->width));
    const i32 src_bottom = min(y + as(i32, src->height), as(i32, dst->height));
    const i32 start_x    = max(0, x);
    const i32 start_y    = max(0, y);

    // Handle same type direct copy
    if (dst->type == src->type) {
        for (i32 py = start_y; py < src_bottom; ++py) {
            const i32   src_y          = py - y;
            const usize src_row_offset = as(usize, src_y) * src->width;
            const usize dst_row_offset = as(usize, py) * dst->width;

            const usize row_size = as(usize, src_right - start_x) * (dst->type == engine_CanvasType_rgba ? sizeof(engine_ColorRgba) : sizeof(engine_ColorIndexed));

            memcpy(
                (u8*)dst->buffer + (dst_row_offset + as(usize, start_x)) * (dst->type == engine_CanvasType_rgba ? sizeof(engine_ColorRgba) : sizeof(engine_ColorIndexed)),
                (u8*)src->buffer + (src_row_offset + as(usize, start_x - x)) * (src->type == engine_CanvasType_rgba ? sizeof(engine_ColorRgba) : sizeof(engine_ColorIndexed)),
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
            const u32 src_idx = as(u32, src_y) * src->width + as(u32, src_x);

            engine_ColorValue color = { 0 };
            switch (src->type) {
            case engine_CanvasType_rgba:
                color.rgba = ((engine_ColorRgba*)src->buffer)[src_idx];
                break;
            default:
                color.indexed = ((engine_ColorIndexed*)src->buffer)[src_idx];
                break;
            }
            engine_Canvas_drawPixel(dst, px, py, color);
        }
    }
}

void engine_Canvas_blitScaled(engine_Canvas* dst, const engine_Canvas* src, i32 x, i32 y, f32 scale) {
    debug_assert_nonnull(dst);
    debug_assert_nonnull(src);
    debug_assert_nonnull(dst->buffer);
    debug_assert_nonnull(src->buffer);
    debug_assert(scale > 0);

    const i32 scaled_width  = (i32)((f32)src->width * scale);
    const i32 scaled_height = (i32)((f32)src->height * scale);

    // Calculate destination bounds
    const i32 dst_right  = min(x + scaled_width, as(i32, dst->width));
    const i32 dst_bottom = min(y + scaled_height, as(i32, dst->height));
    const i32 start_x    = max(0, x);
    const i32 start_y    = max(0, y);

    for (i32 dy = start_y; dy < dst_bottom; dy++) {
        for (i32 dx = start_x; dx < dst_right; dx++) {
            // Calculate source pixel
            const i32 src_x = (i32)((f32)(dx - x) / scale);
            const i32 src_y = (i32)((f32)(dy - y) / scale);

            if (src_x >= as(i32, src->width) || src_y >= as(i32, src->height)) {
                continue;
            }

            const u32 src_idx = as(u32, src_y) * src->width + as(u32, src_x);

            engine_ColorValue color;
            switch (src->type) {
            case engine_CanvasType_rgba:
                color.rgba = ((engine_ColorRgba*)src->buffer)[src_idx];
                break;
            default:
                color.indexed = ((engine_ColorIndexed*)src->buffer)[src_idx];
                break;
            }

            engine_Canvas_drawPixel(dst, dx, dy, color);
        }
    }
}
