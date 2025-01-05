#include "../include/engine/canvas.h"
#include "dh/defer.h"

#include <math.h>

Err$Ptr$engine_Canvas engine_Canvas_create(u32 width, u32 height, engine_CanvasType type) {
    reserveReturn(Err$Ptr$engine_Canvas);
    scope_defer {
        let canvas = (engine_Canvas*)malloc(sizeof(engine_Canvas));
        if (!canvas) {
            defer_return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
        }
        errdefer(free(canvas));

        let len = as(usize, width) * as(usize, height);
        let ptr = (Color*)malloc(len * sizeof(Color));
        if (!ptr) {
            defer_return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
        }
        errdefer(free(ptr));

        canvas->buffer.len = len;
        canvas->buffer.ptr = ptr;
        canvas->width      = width;
        canvas->height     = height;
        canvas->type       = type;

        // Initialize conversion functions based on type
        canvas->pixelToColor = null; // Would be implemented based on type
        canvas->colorToPixel = null; // Would be implemented based on type

        engine_Canvas_clear(canvas, Color_blank);
        defer_return_ok(canvas);
    }
    return_deferred;
}

void engine_Canvas_destroy(engine_Canvas* canvas) {
    if (!canvas) { return; }
    if (canvas->buffer.ptr) {
        free(canvas->buffer.ptr);
    }
    free(canvas);
}

void engine_Canvas_clear(engine_Canvas* canvas, Color color) {
    debug_assert_nonnull(canvas);
    debug_assert_nonnull(canvas->buffer.ptr);

    switch (canvas->type) {
    case engine_CanvasType_rgba: {
        let buffer = canvas->buffer;
        for (usize i = 0; i < buffer.len; ++i) {
            buffer.ptr[i] = color;
        }
    } break;
    default: {
        claim_unreachable_msg("Invalid canvas type");
    } break;
    }
}

void engine_Canvas_drawPixel(engine_Canvas* canvas, i32 x, i32 y, Color color) {
    debug_assert_nonnull(canvas);
    debug_assert_nonnull(canvas->buffer.ptr);
    if (x < 0 || as(i32, canvas->width) <= x) { return; }
    if (y < 0 || as(i32, canvas->height) <= y) { return; }

    const usize index = as(usize, x) + (as(usize, y) * canvas->width);
    switch (canvas->type) {
    case engine_CanvasType_rgba: {
        let buffer        = canvas->buffer;
        buffer.ptr[index] = color;
    } break;
    default: {
        claim_unreachable_msg("Invalid canvas type");
    } break;
    }
}

// Helper function to draw a line using Bresenham's algorithm
void engine_Canvas_drawLine(engine_Canvas* canvas, i32 x1, i32 y1, i32 x2, i32 y2, Color color) {
    const i32 dx  = abs(x2 - x1);
    const i32 dy  = -abs(y2 - y1);
    const i32 sx  = x1 < x2 ? 1 : -1;
    const i32 sy  = y1 < y2 ? 1 : -1;
    i32       err = dx + dy;

    while (true) {
        engine_Canvas_drawPixel(canvas, x1, y1, color);

        if (x1 == x2 && y1 == y2) { break; }

        let e2 = 2 * err;
        if (dy <= e2) {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void engine_Canvas_fillRect(engine_Canvas* canvas, i32 x, i32 y, i32 w, i32 h, Color color) {
    debug_assert_nonnull(canvas);
    debug_assert_nonnull(canvas->buffer.ptr);
    if (x < 0 || as(i32, canvas->width) <= x) { return; }
    if (y < 0 || as(i32, canvas->height) <= y) { return; }
    if (w <= 0 || h <= 0) { return; }

    if (as(i32, canvas->width) < x + w) { w = as(i32, canvas->width) - x; }
    if (as(i32, canvas->height) < y + h) { h = as(i32, canvas->height) - y; }

    for (i32 py = y; py < (y + h); ++py) {
        for (i32 px = x; px < (x + w); ++px) {
            engine_Canvas_drawPixel(canvas, px, py, color);
        }
    }
}

void engine_Canvas_drawRing(
    engine_Canvas* canvas,
    i32            centerX,
    i32            centerY,
    f32            innerRadius,
    f32            outerRadius,
    f32            startAngle,
    f32            endAngle,
    i32            segments,
    Color          color
) {
    debug_assert_nonnull(canvas);
    debug_assert_nonnull(canvas->buffer.ptr);
    debug_assert(0 <= innerRadius && innerRadius < outerRadius);
    debug_assert(0 < segments);

    const f32 step = (endAngle - startAngle) / (f32)segments;

    // Convert angles from degrees to radians
    startAngle = (f32)startAngle * ((f32)PI / 180.0f);
    endAngle   = (f32)endAngle * ((f32)PI / 180.0f);

    // Initialize previous points
    i32 prev_x1 = centerX + (i32)(innerRadius * cosf(startAngle));
    i32 prev_y1 = centerY + (i32)(innerRadius * sinf(startAngle));
    i32 prev_x2 = centerX + (i32)(outerRadius * cosf(startAngle));
    i32 prev_y2 = centerY + (i32)(outerRadius * sinf(startAngle));

    for (i32 i = 1; i <= segments; ++i) {
        f32 angle = startAngle + (step * (f32)i);
        f32 cos_a = cosf(angle);
        f32 sin_a = sinf(angle);

        // Calculate points for both inner and outer radius
        i32 x1 = centerX + (i32)(innerRadius * cos_a);
        i32 y1 = centerY + (i32)(innerRadius * sin_a);
        i32 x2 = centerX + (i32)(outerRadius * cos_a);
        i32 y2 = centerY + (i32)(outerRadius * sin_a);

        // Draw lines between inner and outer points
        engine_Canvas_drawLine(canvas, x1, y1, x2, y2, color);

        // Connect to previous points
        engine_Canvas_drawLine(canvas, prev_x1, prev_y1, x1, y1, color);
        engine_Canvas_drawLine(canvas, prev_x2, prev_y2, x2, y2, color);

        prev_x1 = x1;
        prev_y1 = y1;
        prev_x2 = x2;
        prev_y2 = y2;
    }
}

void engine_Canvas_resize(engine_Canvas* canvas, u32 width, u32 height) {
    debug_assert_nonnull(canvas);
    debug_assert_nonnull(canvas->buffer.ptr);

    let new_len   = as(usize, width) * as(usize, height);
    let new_items = (Color*)realloc(canvas->buffer.ptr, new_len * sizeof(Color));
    if (!new_items) { return; }

    canvas->buffer.len = new_len;
    canvas->buffer.ptr = new_items;
    canvas->width      = width;
    canvas->height     = height;
}

void engine_Canvas_blit(engine_Canvas* dst, const engine_Canvas* src, i32 x, i32 y) {
    debug_assert_nonnull(dst);
    debug_assert_nonnull(src);
    debug_assert_nonnull(dst->buffer.ptr);
    debug_assert_nonnull(src->buffer.ptr);

    // Calculate intersection rectangle
    const i32 src_right  = prim_min(x + as(i32, src->width), as(i32, dst->width));
    const i32 src_bottom = prim_min(y + as(i32, src->height), as(i32, dst->height));
    const i32 start_x    = prim_max(0, x);
    const i32 start_y    = prim_max(0, y);

    const bool same_rgba_type = (dst->type == engine_CanvasType_rgba && src->type == engine_CanvasType_rgba);
    if (!same_rgba_type) {
        // Fallback for other types
        for (i32 py = start_y; py < src_bottom; ++py) {
            for (i32 px = start_x; px < src_right; ++px) {
                const i32   src_x     = px - x;
                const i32   src_y     = py - y;
                const Color src_color = src->buffer.ptr[src_x + (src_y * src->width)];
                engine_Canvas_drawPixel(dst, px, py, src_color);
            }
        }
        return;
    }

    // Handle alpha blending for RGBA type
    for (i32 py = start_y; py < src_bottom; ++py) {
        for (i32 px = start_x; px < src_right; ++px) {
            const i32   src_x     = px - x;
            const i32   src_y     = py - y;
            const Color src_color = src->buffer.ptr[src_x + (src_y * src->width)];

            // Skip fully transparent pixels
            if (src_color.a == 0) { continue; }

            // Fast path for fully opaque pixels
            if (src_color.a == ColorChannel_max_value) {
                dst->buffer.ptr[px + (py * dst->width)] = src_color;
                continue;
            }

            // Alpha blending
            const Color dst_color = dst->buffer.ptr[px + (py * dst->width)];
            const f32   src_alpha = as(f32, src_color.a) / ColorChannel_max_value;
            const f32   dst_alpha = as(f32, dst_color.a) / ColorChannel_max_value;
            const f32   out_alpha = src_alpha + (dst_alpha * (1.0f - src_alpha));

            Color blended = Color_blank;
            for (usize i = 0; i < 3; ++i) {
                const f32 src_component     = as(f32, src_color.rgb.channels[i]) / ColorChannel_max_value;
                const f32 dst_component     = as(f32, dst_color.rgb.channels[i]) / ColorChannel_max_value;
                const f32 blended_component = ((src_component * src_alpha) + ((dst_component * dst_alpha) * (1.0f - src_alpha))) / out_alpha;
                blended.rgb.channels[i]     = as(u8, blended_component * ColorChannel_max_value);
            }
            blended.a = as(u8, out_alpha * ColorChannel_max_value);

            dst->buffer.ptr[px + (py * dst->width)] = blended;
        }
    }
}

void engine_Canvas_blitScaled(engine_Canvas* dst, const engine_Canvas* src, i32 x, i32 y, f32 scale) {
    debug_assert_nonnull(dst);
    debug_assert_nonnull(src);
    debug_assert_nonnull(dst->buffer.ptr);
    debug_assert_nonnull(src->buffer.ptr);
    debug_assert(0 < scale);

    const i32 scaled_width  = as(i32, as(f32, src->width) * scale);
    const i32 scaled_height = as(i32, as(f32, src->height) * scale);

    // Calculate destination bounds
    const i32 dst_right  = prim_min(x + scaled_width, as(i32, dst->width));
    const i32 dst_bottom = prim_min(y + scaled_height, as(i32, dst->height));
    const i32 start_x    = prim_max(0, x);
    const i32 start_y    = prim_max(0, y);

    for (i32 dy = start_y; dy < dst_bottom; ++dy) {
        for (i32 dx = start_x; dx < dst_right; ++dx) {
            // Calculate source pixel
            const i32 src_x = as(i32, as(f32, dx - x) / scale);
            const i32 src_y = as(i32, as(f32, dy - y) / scale);

            if (as(i32, src->width) <= src_x || as(i32, src->height) <= src_y) { continue; }
            const usize src_idx = as(usize, src_x) + (as(usize, src_y) * src->width);

            Color color = cleared();
            switch (src->type) {
            case engine_CanvasType_rgba:
                color = src->buffer.ptr[src_idx];
                break;
            default:
                // color.indexed = ((engine_ColorIndexed*)src->buffer)[src_idx];
                break;
            }
            engine_Canvas_drawPixel(dst, dx, dy, color);
        }
    }
}
