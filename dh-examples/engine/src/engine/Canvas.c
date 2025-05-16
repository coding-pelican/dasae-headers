#include "engine/Canvas.h"
#include "dh/math/common.h"
#include "dh/mem/cfg.h"

extern fn_(engine_Canvas_init(const engine_Canvas_Config* config), Err$Ptr$engine_Canvas) $must_check;
extern fn_(engine_Canvas_fini(engine_Canvas* self), void);
extern fn_(engine_Canvas_resize(engine_Canvas* self, u32 width, u32 height), Err$void) $must_check;
extern fn_(engine_Canvas_clear(engine_Canvas* self, Opt$Color color), void);

fn_scope_ext(engine_Canvas_init(const engine_Canvas_Config* config), Err$Ptr$engine_Canvas) {
    debug_assert_nonnull(config);
    debug_assert(0 < config->width);
    debug_assert(0 < config->height);

    let allocator = unwrap(config->allocator);
    let self      = meta_cast$(engine_Canvas*, try_(mem_Allocator_create(allocator, typeInfo$(engine_Canvas))));
    errdefer_(mem_Allocator_destroy(allocator, anyPtr(self)));
    self->allocator = allocator;

    let area   = as$(usize, config->width) * config->height;
    let buffer = meta_cast$(Sli$Color, try_(mem_Allocator_alloc(allocator, typeInfo$(Color), area)));
    errdefer_(mem_Allocator_free(allocator, anySli(buffer)));
    self->buffer = Grid_fromSli$(Grid$Color, buffer, config->width, config->height);

    let type   = unwrap(config->type);
    self->type = type;

    self->default_color = orelse(config->default_color, Color_blank);
    engine_Canvas_clear(self, none$(Opt$Color));

    return_ok(self);
} unscoped_ext;

fn_(engine_Canvas_fini(engine_Canvas* self), void) {
    if (!self) { return; }
    mem_Allocator_free(self->allocator, anySli(self->buffer.items));
    mem_Allocator_destroy(self->allocator, anyPtr(self));
}

fn_scope(engine_Canvas_resize(engine_Canvas* self, u32 width, u32 height), Err$void) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(self->buffer.items.ptr);

    if_(let buffer = self->buffer, width == Grid_width(buffer) && height == Grid_height(buffer)) { return_ok({}); }

    let new_len   = as$(usize, width) * height;
    let new_items = try_(mem_Allocator_realloc(self->allocator, anySli(self->buffer.items), new_len));
    self->buffer  = Grid_fromSli$(Grid$Color, meta_cast$(Sli$Color, new_items), width, height);
    log_debug("canvas resized: %d x %d -> %d x %d", self->buffer.width, self->buffer.height, width, height);

    return_ok({});
} unscoped;

fn_(engine_Canvas_clear(engine_Canvas* self, Opt$Color other_color), void) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(self->buffer.items.ptr);

    let color = orelse(other_color, self->default_color);
    for_slice (self->buffer.items, item) {
        *item = color;
    }
}

$inline_always Color Color_blendAlpha(Color src, Color dst) {
    // Convert [0..255] => [0..1]
    let s_a = as$(f32, src.a) / as$(f32, ColorChannel_max_value);
    let d_a = as$(f32, dst.a) / as$(f32, ColorChannel_max_value);

    // Final alpha = alpha_src + alpha_dst * (1 - alpha_src)
    let out_a = s_a + d_a * (1.0f - s_a);

    // If the result is fully transparent, just return transparent
    if (out_a <= as$(f32, ColorChannel_min_value)) { return Color_blank; }

    // “Source over” for color channels:
    // out.rgb = (src.rgb * src.a) + (dst.rgb * dst.a * (1 - src.a)) / outA
    // (We do the division by outA at the end so that final alpha is correct.)

    let out_r = (as$(f32, src.r) * s_a + as$(f32, dst.r) * d_a * (1.0f - s_a)) / out_a;
    let out_g = (as$(f32, src.g) * s_a + as$(f32, dst.g) * d_a * (1.0f - s_a)) / out_a;
    let out_b = (as$(f32, src.b) * s_a + as$(f32, dst.b) * d_a * (1.0f - s_a)) / out_a;

    // Convert back to [0..255] range
    return literal_Color_from(
        as$(u8, out_r + 0.5f),
        as$(u8, out_g + 0.5f),
        as$(u8, out_b + 0.5f),
        as$(u8, out_a* as$(f32, ColorChannel_max_value) + 0.5f)
    );
}

void engine_Canvas_drawPixel(engine_Canvas* self, i32 x, i32 y, Color color) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(self->buffer.items.ptr);

    // Bounds check
    if (x < 0 || as$(i32, Grid_width(self->buffer)) <= x) { return; }
    if (y < 0 || as$(i32, Grid_height(self->buffer)) <= y) { return; }

    // If source alpha is zero => invisible
    if (color.a == ColorChannel_min_value) { return; }
    // If the source is fully opaque, just overwrite
    if (color.a == ColorChannel_max_value) {
        Grid_setAt(self->buffer, x, y, color);
        return;
    }
    // Otherwise, do alpha blending
    let dst = Grid_getAt(self->buffer, x, y); // Get the destination pixel already in the buffer
    Grid_setAt(self->buffer, x, y, Color_blendAlpha(color, dst));
}

// Helper function to draw a line using Bresenham's algorithm
void engine_Canvas_drawLine(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, Color color) {
    const i32 dx  = abs(x2 - x1);
    const i32 dy  = -abs(y2 - y1);
    const i32 sx  = x1 < x2 ? 1 : -1;
    const i32 sy  = y1 < y2 ? 1 : -1;
    i32       err = dx + dy;

    while (true) {
        engine_Canvas_drawPixel(self, x1, y1, color);

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

void engine_Canvas_drawHLine(engine_Canvas* self, i32 x1, i32 x2, i32 y, Color color) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(self->buffer.items.ptr);

    // Ensure x1 is the leftmost point
    if (x2 < x1) { prim_swap(x1, x2); }

    // Clip to self bounds
    if (y < 0 || as$(i32, Grid_height(self->buffer)) <= y) { return; }
    x1 = prim_max(0, x1);
    x2 = prim_min(as$(i32, Grid_width(self->buffer)) - 1, x2);

    // Draw the horizontal line
    for (i32 x = x1; x <= x2; ++x) {
        engine_Canvas_drawPixel(self, x, y, color);
    }
}

void engine_Canvas_drawVLine(engine_Canvas* self, i32 x, i32 y1, i32 y2, Color color) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(self->buffer.items.ptr);

    // Ensure y1 is the topmost point
    if (y2 < y1) { prim_swap(y1, y2); }

    // Clip to self bounds
    if (x < 0 || as$(i32, Grid_width(self->buffer)) <= x) { return; }
    y1 = prim_max(0, y1);
    y2 = prim_min(as$(i32, Grid_height(self->buffer)) - 1, y2);

    // Draw the vertical line
    for (i32 y = y1; y <= y2; ++y) {
        engine_Canvas_drawPixel(self, x, y, color);
    }
}

void engine_Canvas_drawRect(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, Color color) {
    // Ensure x1 < x2 and y1 < y2
    if (x2 < x1) { prim_swap(x1, x2); }
    if (y2 < y1) { prim_swap(y1, y2); }

    // Top edge (x1,y1) to (x2,y1)
    engine_Canvas_drawHLine(self, x1, x2, y1, color);
    // Bottom edge (x1,y2) to (x2,y2)
    engine_Canvas_drawHLine(self, x1, x2, y2, color);

    // Left edge (x1,y1) to (x1,y2)
    engine_Canvas_drawVLine(self, x1, y1, y2, color);
    // Right edge (x2,y1) to (x2,y2)
    engine_Canvas_drawVLine(self, x2, y1, y2, color);
}

// Utility to plot the eight symmetrical points
$inline_always void plotCirclePoints(engine_Canvas* self, i32 cx, i32 cy, i32 x, i32 y, Color color) {
    engine_Canvas_drawPixel(self, cx + x, cy + y, color);
    engine_Canvas_drawPixel(self, cx - x, cy + y, color);
    engine_Canvas_drawPixel(self, cx + x, cy - y, color);
    engine_Canvas_drawPixel(self, cx - x, cy - y, color);
    engine_Canvas_drawPixel(self, cx + y, cy + x, color);
    engine_Canvas_drawPixel(self, cx - y, cy + x, color);
    engine_Canvas_drawPixel(self, cx + y, cy - x, color);
    engine_Canvas_drawPixel(self, cx - y, cy - x, color);
}

// Draws an *outline* circle using the midpoint algorithm
void engine_Canvas_drawCircle(engine_Canvas* self, i32 cx, i32 cy, i32 radius, Color color) {
    // if (radius <= 0) { return; }                                                // Zero or Negative radius is invalid
    // if (radius == 1) { return engine_Canvas_drawPixel(self, cx, cy, color); } // Fall-back to a single pixel

    i32 x = 0;
    i32 y = radius;

    // Decision variable p to decide when to move y
    i32 p = 1 - radius;

    // Plot the initial points
    plotCirclePoints(self, cx, cy, x, y, color);

    // Loop over the first octant, then mirror into the rest
    while (x < y) {
        x++;
        if (p < 0) {
            // Midpoint is inside the circle
            p += 2 * x + 1;
        } else {
            // Midpoint is on or outside the circle
            y--;
            p += 2 * (x - y) + 1;
        }

        // Plot the symmetrical points in all octants
        plotCirclePoints(self, cx, cy, x, y, color);
    }
}

void engine_Canvas_drawRing(engine_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, Color color) {
    // Draw outer circle boundary
    engine_Canvas_drawCircle(self, cx, cy, r_outer, color);

    // Draw inner circle boundary
    engine_Canvas_drawCircle(self, cx, cy, r_inner, color);
}

// void engine_Canvas_drawRing(engine_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, Color color) {
//     for (i32 r = r_inner; r <= r_outer; ++r) {
//         // Draw the filled circle of radius r
//         // This effectively layers circles on top of each other
//         // from the smallest to the largest.
//         engine_Canvas_drawCircle(self, cx, cy, r, color);
//     }
//     /* void engine_Canvas_drawRing(
//         engine_Canvas* self,
//         i32            centerX,
//         i32            centerY,
//         f32            innerRadius,
//         f32            outerRadius,
//         f32            start_angle,
//         f32            end_angle,
//         i32            segments,
//         Color          color
//     ) {
//         debug_assert_nonnull(self);
//         debug_assert_nonnull(self->buffer.items.ptr);
//         debug_assert(0 <= innerRadius && innerRadius < outerRadius);
//         debug_assert(0 < segments);

//         // Convert angles from degrees to radians
//         start_angle = (f32)start_angle * (math_f32_pi / 180.0f);
//         end_angle   = (f32)end_angle * (math_f32_pi / 180.0f);

//         const f32 step = (end_angle - start_angle) / (f32)segments;

//         // Initialize previous points
//         i32 prev_x1 = centerX + (i32)(innerRadius * cosf(start_angle));
//         i32 prev_y1 = centerY + (i32)(innerRadius * sinf(start_angle));
//         i32 prev_x2 = centerX + (i32)(outerRadius * cosf(start_angle));
//         i32 prev_y2 = centerY + (i32)(outerRadius * sinf(start_angle));

//         for (i32 i = 1; i <= segments; ++i) {
//             f32 angle = start_angle + (step * (f32)i);
//             f32 cos_a = cosf(angle);
//             f32 sin_a = sinf(angle);

//             // Calculate points for both inner and outer radius
//             i32 x1 = centerX + (i32)(innerRadius * cos_a);
//             i32 y1 = centerY + (i32)(innerRadius * sin_a);
//             i32 x2 = centerX + (i32)(outerRadius * cos_a);
//             i32 y2 = centerY + (i32)(outerRadius * sin_a);

//             // -- Removed the line that connects inner and outer points --
//             // engine_Canvas_drawLine(self, x1, y1, x2, y2, color);

//             // Only connect consecutive points along the inner circle...
//             engine_Canvas_drawLine(self, prev_x1, prev_y1, x1, y1, color);
//             // ...and the outer circle
//             engine_Canvas_drawLine(self, prev_x2, prev_y2, x2, y2, color);

//             prev_x1 = x1;
//             prev_y1 = y1;
//             prev_x2 = x2;
//             prev_y2 = y2;
//         }
//     } */
// }

// Convert degrees to radians
$inline_always f32  degToRad(f32 deg) { return (deg * math_f32_pi) / 180.0f; }
// Check if angle (in radians) is in [start_rad, end_rad]
$inline_always bool isAngleInRange(f32 angle, f32 start_rad, f32 end_rad) {
    // Normalize angle to [0, 2π) for easier comparison
    if (angle < 0) { angle += 2.0f * math_f32_pi; }
    // We assume startRad <= endRad
    return start_rad <= angle && angle <= end_rad;
}
// Plot 8 symmetric points *only if* their angle is in [startRad, endRad].
$inline_always void plotArcPoints(engine_Canvas* self, i32 cx, i32 cy, i32 x, i32 y, f32 start_rad, f32 end_rad, Color color) {
    // All the symmetrical coords around the center
    const i32 coords[8][2] = {
        { cx + x, cy + y },
        { cx - x, cy + y },
        { cx + x, cy - y },
        { cx - x, cy - y },
        { cx + y, cy + x },
        { cx - y, cy + x },
        { cx + y, cy - x },
        { cx - y, cy - x }
    };

    // For each of the 8 points, compute its angle and check if it's in range
    for (i32 i = 0; i < 8; ++i) {
        i32 px = coords[i][0];
        i32 py = coords[i][1];

        // Compute angle in radians using atan2
        f32 angle = atan2f((f32)(py - cy), (f32)(px - cx));

        if (!isAngleInRange(angle, start_rad, end_rad)) { continue; }
        engine_Canvas_drawPixel(self, px, py, color);
    }
}

// Draw an arc from start_angle to end_angle
void engine_Canvas_drawArc(engine_Canvas* self, i32 cx, i32 cy, i32 radius, f32 start_angle, f32 end_angle, Color color) {
    // Convert degrees to radians
    f32 start_rad = degToRad(start_angle);
    f32 end_rad   = degToRad(end_angle);

    i32 x = 0;
    i32 y = radius;

    // Decision variable
    i32 p = 1 - radius;

    // Plot initial octant points if within the angle
    plotArcPoints(self, cx, cy, x, y, start_rad, end_rad, color);

    // Midpoint circle for the first octant
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }

        // Plot symmetrical points *only* if in angle range
        plotArcPoints(self, cx, cy, x, y, start_rad, end_rad, color);
    }
}

void engine_Canvas_drawAngleRing(engine_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, f32 start_angle, f32 end_angle, Color color) {
    // For each radius from inner to outer...
    for (i32 r = r_inner; r <= r_outer; ++r) {
        // ...draw the *sector* of that radius
        engine_Canvas_fillArc(self, cx, cy, r, start_angle, end_angle, color);
    }
}

void engine_Canvas_drawCapsule(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, Color color) {
    if (x2 < x1) { prim_swap(x1, x2); }
    if (y2 < y1) { prim_swap(y1, y2); }

    i32 height = y2 - y1;
    i32 r      = height / 2;
    i32 y_mid  = y1 + r;

    // 1) Draw the "rectangular" part (horizontal edges)
    // left edge to right edge, at top and bottom
    engine_Canvas_drawHLine(self, x1 + r, x2 - r, y1, color);
    engine_Canvas_drawHLine(self, x1 + r, x2 - r, y2, color);

    // vertical edges (these are just lines from the rectangle part,
    // but if the capsule's corners are purely circular,
    // these lines are "tangent" to the arcs.
    engine_Canvas_drawVLine(self, x1, y1 + 1, y2 - 1, color);
    engine_Canvas_drawVLine(self, x2, y1 + 1, y2 - 1, color);

    // 2) Draw left semicircle
    engine_Canvas_drawCircle(self, x1 + r, y_mid, r, color);
    // 3) Draw right semicircle
    engine_Canvas_drawCircle(self, x2 - r, y_mid, r, color);

    // The circle function needs to be able to handle
    // only the top/bottom arcs if you want a "perfect" capsule outline.
    // Otherwise, a full circle is drawn, some of which overlaps
    // the rectangle edges.
}

// Helper: integer "round" for f32 -> int
$inline_always i32 iround(f32 x) { return (i32)(x + 0.5f); }

// Draw a thick line by drawing multiple offset lines
void engine_Canvas_drawLineThick(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, f32 thickness, Color color) {
    if (thickness <= 1.0f) {
        // Just draw a normal line
        engine_Canvas_drawLine(self, x1, y1, x2, y2, color);
        return;
    }
    // Vector
    f32 dx = (f32)(x2 - x1);
    f32 dy = (f32)(y2 - y1);

    // Length
    f32 length = sqrtf(dx * dx + dy * dy);
    if (length < 1.0f) {
        // Degenerate line; just draw a pixel or small box
        engine_Canvas_drawPixel(self, x1, y1, color);
        return;
    }

    // Unit normal: (-dy, dx) / length
    f32 ndx = -dy / length;
    f32 ndy = dx / length;

    // Half thickness offset
    f32 half_t = (f32)(thickness - 1) / 2.0f;

    // For each offset i in [-half_t ... +half_t], draw a parallel line
    for (f32 i = -half_t; i <= half_t; i += 1.0f) {
        i32 offset_x1 = x1 + iround(ndx * i);
        i32 offset_y1 = y1 + iround(ndy * i);
        i32 offset_x2 = x2 + iround(ndx * i);
        i32 offset_y2 = y2 + iround(ndy * i);

        engine_Canvas_drawLine(self, offset_x1, offset_y1, offset_x2, offset_y2, color);
    }
}

void engine_Canvas_drawRectThick(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, f32 thickness, Color color) {
    if (x2 < x1) { prim_swap(x1, x2); }
    if (y2 < y1) { prim_swap(y1, y2); }

    // Top edge
    engine_Canvas_drawLineThick(self, x1, y1, x2, y1, thickness, color);
    // Bottom edge
    engine_Canvas_drawLineThick(self, x1, y2, x2, y2, thickness, color);
    // Left edge
    engine_Canvas_drawLineThick(self, x1, y1, x1, y2, thickness, color);
    // Right edge
    engine_Canvas_drawLineThick(self, x2, y1, x2, y2, thickness, color);
}

void engine_Canvas_drawRectBorderByCutout(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, i32 thickness, Color inner_color, Color border_color) {
    if (x2 < x1) { prim_swap(x1, x2); }
    if (y2 < y1) { prim_swap(y1, y2); }

    // 1) Fill outer rectangle in the border color
    engine_Canvas_fillRect(self, x1, y1, x2, y2, border_color);

    // 2) Erase the inside with bgColor, leaving a thick frame
    i32 inner_x1 = x1 + thickness;
    i32 inner_y1 = y1 + thickness;
    i32 inner_x2 = x2 - thickness;
    i32 inner_y2 = y2 - thickness;

    // Make sure the inner rectangle is valid (innerX2 >= innerX1, etc.)
    if (inner_x1 < inner_x2 && inner_y1 < inner_y2) {
        engine_Canvas_fillRect(self, inner_x1, inner_y1, inner_x2, inner_y2, inner_color);
    }
}

void engine_Canvas_fillRect(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, Color color) {
    // Ensure x1 < x2 and y1 < y2
    if (x2 < x1) { prim_swap(x1, x2); }
    if (y2 < y1) { prim_swap(y1, y2); }

    for (i32 y = y1; y <= y2; ++y) {
        engine_Canvas_drawHLine(self, x1, x2, y, color);
    }
}

// Draws a *filled* circle using the midpoint algorithm
void engine_Canvas_fillCircle(engine_Canvas* self, i32 cx, i32 cy, i32 radius, Color color) {
    // if (radius <= 0) { return; }                                                // Zero or Negative radius is invalid
    // if (radius == 1) { return engine_Canvas_drawPixel(self, cx, cy, color); } // Fall-back to a single pixel

    i32 x = 0;
    i32 y = radius;

    i32 p = 1 - radius;

    // We'll fill the very top and bottom line first.
    // Horizontal line from (cx - 0) to (cx + 0) is just one pixel,
    // but we plot from -y to +y in x-direction for the initial top/bottom.
    engine_Canvas_drawHLine(self, cx - radius, cx + radius, cy, color);

    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }

        // For each x, y on the boundary, fill horizontally
        // "Top" portion
        engine_Canvas_drawHLine(self, cx - x, cx + x, cy + y, color);
        engine_Canvas_drawHLine(self, cx - y, cx + y, cy + x, color);

        // "Bottom" portion (mirror in y)
        engine_Canvas_drawHLine(self, cx - x, cx + x, cy - y, color);
        engine_Canvas_drawHLine(self, cx - y, cx + y, cy - x, color);
    }
}

void engine_Canvas_fillRingByCutout(engine_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, Color inner_color, Color outer_color) {
    // 1) Fill the outer circle in ringColor
    engine_Canvas_fillCircle(self, cx, cy, r_outer, outer_color);

    // 2) Fill the inner circle in bgColor to "cut out" the center
    engine_Canvas_fillCircle(self, cx, cy, r_inner, inner_color);
}

void engine_Canvas_fillRingByScanlines(engine_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, Color color) {
    // For each row from cy - r_outer to cy + r_outer
    for (i32 y = cy - r_outer; y <= cy + r_outer; ++y) {
        // Figure out how far x extends for the outer radius
        // and the inner radius at this y
        i32 dy = y - cy;

        // The maximum x for outer radius = sqrt(r_outer^2 - dy^2)
        // The maximum x for inner radius = sqrt(r_inner^2 - dy^2)
        // (if inside the circle)
        f32 outer2     = (f32)(r_outer * r_outer - dy * dy);
        f32 inner2     = (f32)(r_inner * r_inner - dy * dy);
        f32 outer_span = sqrtf(fabsf(outer2));
        f32 inner_span = sqrtf(fabsf(inner2));
        outer_span     = outer_span == f32_nan ? 1 : outer_span;
        inner_span     = inner_span == f32_nan ? 1 : inner_span;

        i32 x_outer = (i32)floorf(outer_span);
        i32 x_inner = (i32)floorf(inner_span);

        // So we draw from cx - x_outer to cx - x_inner, and from cx + x_inner to cx + x_outer
        // That covers the ring region horizontally at this y.

        // Left side of ring
        engine_Canvas_drawHLine(self, cx - x_outer, cx - x_inner, y, color);

        // Right side of ring
        engine_Canvas_drawHLine(self, cx + x_inner, cx + x_outer, y, color);
    }
}


$inline_always void drawHLineAngleClipped(engine_Canvas* self, i32 x1, i32 x2, i32 y, i32 cx, i32 cy, f32 start_rad, f32 end_rad, Color color) {
    if (x2 < x1) { prim_swap(x1, x2); }

    for (i32 x = x1; x <= x2; ++x) {
        // Compute angle
        f32 angle = atan2f((f32)(y - cy), (f32)(x - cx));

        // Check if angle is in range
        if (angle < 0) {
            angle += 2.0f * math_f32_pi;
        }
        if (start_rad <= angle && angle <= end_rad) {
            engine_Canvas_drawPixel(self, x, y, color);
        }
    }
}

$inline_always void drawVLineAngleClipped(engine_Canvas* self, i32 y1, i32 y2, i32 x, i32 cx, i32 cy, f32 start_rad, f32 end_rad, Color color) {
    if (y2 < y1) { prim_swap(y1, y2); }

    for (i32 y = y1; y <= y2; ++y) {
        // Compute angle
        f32 angle = atan2f((f32)(y - cy), (f32)(x - cx));

        // Check if angle is in range
        if (angle < 0) {
            angle += 2.0f * math_f32_pi;
        }
        if (start_rad <= angle && angle <= end_rad) {
            engine_Canvas_drawPixel(self, x, y, color);
        }
    }
}

void engine_Canvas_fillArc(engine_Canvas* self, i32 cx, i32 cy, i32 radius, f32 start_angle, f32 end_angle, Color color) {
    // Convert to radians
    f32 start_rad = degToRad(start_angle);
    f32 end_rad   = degToRad(end_angle);

    i32 x = 0;
    i32 y = radius;
    i32 p = 1 - radius;

    // Fill the center row (y=cy) from (cx - radius) to (cx + radius)
    drawHLineAngleClipped(self, cx - radius, cx + radius, cy, cx, cy, start_rad, end_rad, color);

    // Midpoint logic
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }

        // We now have boundary points (x, y). We fill horizontally:
        // "Top" rows:
        drawHLineAngleClipped(self, cx - x, cx + x, cy + y, cx, cy, start_rad, end_rad, color);
        drawHLineAngleClipped(self, cx - y, cx + y, cy + x, cx, cy, start_rad, end_rad, color);

        // "Bottom" rows (mirror)
        drawHLineAngleClipped(self, cx - x, cx + x, cy - y, cx, cy, start_rad, end_rad, color);
        drawHLineAngleClipped(self, cx - y, cx + y, cy - x, cx, cy, start_rad, end_rad, color);
    }
}

void engine_Canvas_fillAngleRingByCutout(engine_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, f32 start_angle, f32 end_angle, Color inner_color, Color outer_color) {
    // 1) Fill outer
    engine_Canvas_fillArc(self, cx, cy, r_outer, start_angle, end_angle, outer_color);

    // 2) "Erase" inner
    engine_Canvas_fillArc(self, cx, cy, r_inner, start_angle, end_angle, inner_color);
}

void engine_Canvas_fillCapsule(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, Color color) {
    if (x2 < x1) { prim_swap(x1, x2); }
    if (y2 < y1) { prim_swap(y1, y2); }

    i32 height = y2 - y1;
    i32 r      = height / 2;
    i32 y_mid  = y1 + r;

    // 1) Fill the central rectangle portion
    engine_Canvas_fillRect(self, x1 + r, y1, x2 - r, y2, color);

    // 2) Fill left semicircle
    //    (A "filled" circle drawing function,
    //     then you only fill the half that’s relevant.)
    engine_Canvas_fillCircle(self, x1 + r, y_mid, r, color);

    // 3) Fill right semicircle
    engine_Canvas_fillCircle(self, x2 - r, y_mid, r, color);
}

void engine_Canvas_blit(engine_Canvas* dst, const engine_Canvas* src, i32 x, i32 y) {
    debug_assert_nonnull(dst);
    debug_assert_nonnull(src);
    debug_assert_nonnull(dst->buffer.items.ptr);
    debug_assert_nonnull(src->buffer.items.ptr);

    // Calculate intersection rectangle
    const i32 src_right  = prim_min(x + as$(i32, Grid_width(src->buffer)), as$(i32, Grid_width(dst->buffer)));
    const i32 src_bottom = prim_min(y + as$(i32, Grid_height(src->buffer)), as$(i32, Grid_height(dst->buffer)));
    const i32 start_x    = prim_max(0, x);
    const i32 start_y    = prim_max(0, y);

    // Handle alpha blending for RGBA type
    for (i32 py = start_y; py < src_bottom; ++py) {
        for (i32 px = start_x; px < src_right; ++px) {
            const i32   src_x     = px - x;
            const i32   src_y     = py - y;
            const Color src_color = src->buffer.items.ptr[src_x + (src_y * Grid_width(src->buffer))];
            const Color dst_color = dst->buffer.items.ptr[px + (py * Grid_width(dst->buffer))];

            // Alpha blending
            dst->buffer.items.ptr[px + (py * Grid_width(dst->buffer))] = Color_blendAlpha(src_color, dst_color);
        }
    }

    /* const bool same_rgba_type = (dst->type == engine_CanvasType_rgba && src->type == engine_CanvasType_rgba);
    if (!same_rgba_type) {
        // Fallback for other types
        for (i32 py = start_y; py < src_bottom; ++py) {
            for (i32 px = start_x; px < src_right; ++px) {
                const i32   src_x     = px - x;
                const i32   src_y     = py - y;
                const Color src_color = src->buffer.items.ptr[src_x + (src_y * Grid_width(src->buffer))];
                engine_Canvas_drawPixel(dst, px, py, src_color);
            }
        }
        return;
    } */
}

void engine_Canvas_blitScaled(engine_Canvas* dst, const engine_Canvas* src, i32 x, i32 y, f32 scale) {
    debug_assert_nonnull(dst);
    debug_assert_nonnull(src);
    debug_assert_nonnull(dst->buffer.items.ptr);
    debug_assert_nonnull(src->buffer.items.ptr);
    debug_assert(0 < scale);

    const i32 scaled_width  = as$(i32, as$(f32, Grid_width(src->buffer)) * scale);
    const i32 scaled_height = as$(i32, as$(f32, Grid_height(src->buffer)) * scale);

    // Calculate destination bounds
    const i32 dst_right  = prim_min(x + scaled_width, as$(i32, Grid_width(dst->buffer)));
    const i32 dst_bottom = prim_min(y + scaled_height, as$(i32, Grid_height(dst->buffer)));
    const i32 start_x    = prim_max(0, x);
    const i32 start_y    = prim_max(0, y);

    for (i32 dy = start_y; dy < dst_bottom; ++dy) {
        for (i32 dx = start_x; dx < dst_right; ++dx) {
            // Calculate source pixel
            const i32 src_x = as$(i32, as$(f32, dx - x) / scale);
            const i32 src_y = as$(i32, as$(f32, dy - y) / scale);

            if (as$(i32, Grid_width(src->buffer)) <= src_x || as$(i32, Grid_height(src->buffer)) <= src_y) { continue; }
            const usize src_idx = as$(usize, src_x) + (as$(usize, src_y) * Grid_width(src->buffer));

            Color color = cleared();
            color       = src->buffer.items.ptr[src_idx];
            /* switch (src->type) {
            case engine_CanvasType_rgba:
                color = src->buffer.items.ptr[src_idx];
                break;
            default:
                // color.indexed = ((engine_ColorIndexed*)src->buffer)[src_idx];
                break;
            } */
            engine_Canvas_drawPixel(dst, dx, dy, color);
        }
    }
}
