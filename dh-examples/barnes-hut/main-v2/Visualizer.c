#include "Visualizer.h"
#include "QuadTree.h"
#include "dh/ArrList.h"
#include "dh/debug/common.h"
#include "dh/log.h"
#include "engine/color.h"

/* Color definitions for rendering nodes in the visualizer. */
#define Visualizer_color_body        (literal_Color_fromOpaque(192, 192, 192))
#define Visualizer_color_vel_vec     (literal_Color_fromOpaque(0, 0, 192))
#define Visualizer_color_acc_vec     (literal_Color_fromOpaque(192, 0, 0))
#define Visualizer_min_render_radius (0.5f) // Minimum radius to render as solid
#define Visualizer_alpha_scale       (0.8f) // Alpha multiplier for blending

/* Function to calculate the inverse scale factor for rendering nodes.  */
$inline_always f32   Visualizer_scale(Visualizer* self) { return self->scale; }
$inline_always f32   Visualizer_scaleInv(Visualizer* self) { return 1.0f / self->scale; }
///////////////////////////////////////////////////////////////////////////////
/// Unified coordinate system transformations:
///
/// Screen Coordinates          World Coordinates
/// (0,0)  --------- (w-1,0)    (-w/2*scale, +h/2*scale)
///    |               |                ↑ y+
///    |               |                |
///    |    (CENTER)   |                +--→ x+
///    |               |
/// (0,h-1) -------- (w-1,h-1)  (-w/2*scale, -h/2*scale)
///
///  We treat (self->pos) as our camera center in "world" coordinates.
///  A larger 'self->scale' means we see more of the world (zoom out).
///
///  Let (cx, cy) = Visualizer_screenCenter(self)   // The 2D center of the screen in pixel coords
///  Let (camx, camy) = self->pos                   // Camera center in world coords
///  Let scale = self->scale                        // Zoom factor
///
///  Then for a world-space point (wx, wy) -> screen-space (sx, sy):
///
///    sx = cx + ( (wx - camx) / scale )
///    sy = cy - ( (wy - camy) / scale )
///
///  Conversely, for a screen-space point (sx, sy) -> world-space (wx, wy):
///
///    wx = camx + ( (sx - cx) * scale )
///    wy = camy - ( (sy - cy) * scale )
///
///////////////////////////////////////////////////////////////////////////////
$inline_always Vec2i Visualizer_screenCenter(Visualizer* self) {
    // Return the middle pixel (handles even/odd dimensions by integer truncation)
    let cx = (as$(i32, self->canvas->width) - 1) / 2;
    let cy = (as$(i32, self->canvas->height) - 1) / 2;
    return math_Vec2i_from(cx, cy);
}
/// @brief Convert a world-space position to screen-space (pixels).
///
/// Formula:
///    screen_x = center_x + ( (world_x - camera_x) / scale )
///    screen_y = center_y - ( (world_y - camera_y) / scale )
///
/// @param self      Pointer to the Visualizer (contains camera pos & scale).
/// @param world_pos The (wx, wy) coordinates in world space.
/// @return          The corresponding (sx, sy) on the screen in pixels.
$inline_always Vec2i Visualizer_worldToScreen(Visualizer* self, Vec2f world_pos) {
    let center      = Visualizer_screenCenter(self);
    let w_minus_cam = math_Vec2f_sub(world_pos, self->pos);
    let divided     = math_Vec2f_scale(w_minus_cam, 1.0f / self->scale);

    let sx = as$(i32, roundf(as$(f32, center.x) + divided.x));
    let sy = as$(i32, roundf(as$(f32, center.y) - divided.y));
    return math_Vec2i_from(sx, sy);
}
/// @brief Convert a screen-space pixel coordinate to world-space.
///
/// Formula:
///    world_x = camera_x + ( (screen_x - center_x) * scale )
///    world_y = camera_y - ( (screen_y - center_y) * scale )
///
/// @param self       Pointer to the Visualizer (contains camera pos & scale).
/// @param screen_pos The (sx, sy) pixel coordinates on the screen.
/// @return           The corresponding (wx, wy) in world space.
$inline_always Vec2f Visualizer_screenToWorld(Visualizer* self, Vec2i screen_pos) {
    let center     = Visualizer_screenCenter(self);
    let dx         = as$(f32, screen_pos.x - center.x);
    let dy         = as$(f32, center.y - screen_pos.y);
    let multiplied = math_Vec2f_scale(math_Vec2f_from(dx, dy), self->scale);

    return math_Vec2f_add(self->pos, multiplied);
}
/// Returns the current mouse position converted to world coords.
$inline_always Vec2f Visualizer_mousePosToWorld(Visualizer* self) {
    return Visualizer_screenToWorld(self, engine_Mouse_getPosition());
}

/* Core visualizer functions */
Err$Visualizer Visualizer_create(mem_Allocator allocator, engine_Canvas* canvas) {
    reserveReturn(Err$Visualizer);
    return_ok((Visualizer){
        .pos   = math_Vec2f_zero, // camera center pos
        .scale = 2.0f,            // camera zoom (2.0f == x0.5, 0.5f == x2)
        // .scale = 3600,

        .pan_screen_begin = math_Vec2i_zero,
        .pan_cam_begin    = math_Vec2f_zero,

        .zoom_anchor_world = math_Vec2f_zero,

        .shows_bodies         = true,
        .shows_bodies_vel_vec = false,
        .shows_bodies_acc_vec = false,
        .shows_quad_tree      = false,

        .depth_range = {
            .min = 0,
            .max = 0,
        },
        .cache_stack = type$(ArrList$Visualizer_QuadCache, try(ArrList_initCap(typeInfo$(Visualizer_QuadCache), allocator, 12500))),

        .spawn = {
            .body      = none(),
            .angle     = none(),
            .total     = none(),
            .confirmed = none(),
        },

        .bodies = type$(ArrList$Body, ArrList_init(typeInfo$(Body), allocator)),
        .nodes  = type$(ArrList$QuadNode, ArrList_init(typeInfo$(QuadNode), allocator)),

        .allocator = allocator,
        .canvas    = canvas,
    });
}

void Visualizer_destroy(Visualizer* self) {
    debug_assert_nonnull(self);

    ArrList_fini(&self->cache_stack.base);

    ArrList_fini(&self->bodies.base);
    ArrList_fini(&self->nodes.base);
}

typedef struct Control {
    engine_KeyCode key;
    Vec2f          vec;
} Control;
use_Sli$(Control);
static Sli_const$Control Control_list(void) {
    static const Control controls[] = {
        { .key = engine_KeyCode_w, .vec = math_Vec2f_up },
        { .key = engine_KeyCode_a, .vec = math_Vec2f_left },
        { .key = engine_KeyCode_s, .vec = math_Vec2f_down },
        { .key = engine_KeyCode_d, .vec = math_Vec2f_right },
    };
    static const usize controls_len = countOf(controls);
    return (Sli_const$Control){
        .ptr = controls,
        .len = controls_len,
    };
}

$inline_always void VisualizerInput_resetPos(Visualizer* self) {
    self->pos = math_Vec2f_zero;
}
$inline_always void VisualizerInput_toggleShowingBodies(Visualizer* self) {
    self->shows_bodies = !self->shows_bodies;
}
$inline_always void VisualizerInput_toggleVisualizationBodiesVelVec(Visualizer* self) {
    self->shows_bodies_vel_vec = !self->shows_bodies_vel_vec;
}
$inline_always void VisualizerInput_toggleVisualizationBodiesAccVec(Visualizer* self) {
    self->shows_bodies_acc_vec = !self->shows_bodies_acc_vec;
}
$inline_always void VisualizerInput_toggleVisualizationQuadTree(Visualizer* self) {
    self->shows_quad_tree = !self->shows_quad_tree;
}
// Call this when the middle mouse button is first pressed:
$inline_always void VisualizerInput_onPanBegin(Visualizer* self) {
    if (self->is_panning) { return; }
    // Store screen coordinates & camera pos
    self->pan_screen_begin = engine_Mouse_getPosition();
    self->pan_cam_begin    = self->pos;
    self->is_panning       = true;
}
// Call this each frame while the middle mouse is held:
$inline_always void VisualizerInput_handlePan(Visualizer* self) {
    // Current mouse position in SCREEN space
    let mouse_now_screen = engine_Mouse_getPosition();

    // Convert BOTH the original "pan begin" screen coords
    // and the new screen coords into world space
    let old_mouse_world = Visualizer_screenToWorld(self, self->pan_screen_begin);
    let new_mouse_world = Visualizer_screenToWorld(self, mouse_now_screen);

    // The mouse has moved by (old_mouse_world - new_mouse_world) in world coords
    // So shift the camera by that same difference (so the "under-mouse" point remains under mouse)
    let diff_world = math_Vec2f_sub(old_mouse_world, new_mouse_world);

    // Restore the camera to its position at the moment panning began,
    // then offset it by diff_world
    self->pos = math_Vec2f_add(self->pan_cam_begin, diff_world);
}
// Call this when the middle mouse button is released:
$inline_always void VisualizerInput_onPanEnd(Visualizer* self) {
    self->is_panning = false;
    // // Reset pan screen & cam begin to zero
    // self->pan_screen_begin = math_Vec2f_zero;
    // self->pan_cam_begin    = math_Vec2f_zero;
}
$inline_always void VisualizerInput_handleZoom(Visualizer* self, i32 scroll_delta) {
    if (scroll_delta == 0) { return; }

    // 1) Find the world coords under the mouse BEFORE changing scale
    let mouse_screen       = engine_Mouse_getPosition();
    let mouse_world_before = Visualizer_screenToWorld(self, mouse_screen);

    // 2) Choose a zoom factor
    //    e.g. scrolling up => zoom in => "scale goes down" if you want
    //    or scrolling up => zoom out => "scale goes up" - your choice
    //    (Below code: scroll_delta > 0 => bigger scale => zoom out)
    let zoom_sensitivity = 0.1f;
    let zoom_factor      = ({
        var factor = 1.0f + as$(f32, -scroll_delta) * zoom_sensitivity;
        if (factor < 0.1f) { factor = 0.1f; }
        eval_return factor;
    });

    // Apply the scale change
    self->scale *= zoom_factor;

    // 3) Find the world coords under the mouse AFTER changing scale
    let mouse_world_after = Visualizer_screenToWorld(self, mouse_screen);

    // 4) Shift the camera so the "mouse_world_before" is still under the mouse
    //    That is: new camera pos = old camera pos + (before - after)
    //    Because (mouse_world_before - camera_pos) should remain the same
    let diff  = math_Vec2f_sub(mouse_world_before, mouse_world_after);
    self->pos = math_Vec2f_add(self->pos, diff);
}
Err$void Visualizer_processInput(Visualizer* self, engine_Window* window) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);
    $unused(window);

    // Handle resets
    if (engine_Key_pressed(engine_KeyCode_r)) {
        log_debug("pressed 'r' to reset position\n");
        VisualizerInput_resetPos(self);
    }

    // Handle toggles
    if (engine_Key_pressed(engine_KeyCode_n1)) {
        log_debug("pressed '1' to toggle bodies showing\n");
        VisualizerInput_toggleShowingBodies(self);
    }
    if (engine_Key_pressed(engine_KeyCode_n2)) {
        log_debug("pressed '2' to toggle bodies velocity vector visualization\n");
        VisualizerInput_toggleVisualizationBodiesVelVec(self);
    }
    if (engine_Key_pressed(engine_KeyCode_n3)) {
        log_debug("pressed '3' to toggle bodies acceleration vector visualization\n");
        VisualizerInput_toggleVisualizationBodiesAccVec(self);
    }
    if (engine_Key_pressed(engine_KeyCode_q)) {
        log_debug("pressed 'q' to toggle quad-tree visualization\n");
        VisualizerInput_toggleVisualizationQuadTree(self);
    }

    // Handle moving
    let controls = Control_list();
    for_slice (controls, control) {
        if (engine_Key_held(control->key)) {
            log_debug("pressed '%c' to move\n", control->key);
            self->pos = math_Vec2f_add(self->pos, math_Vec2f_scale(control->vec, 5.0f * Visualizer_scaleInv(self)));
        }
    }

    // Handle panning
    if (engine_Mouse_pressed(engine_MouseButton_middle)) {
        log_debug("mouse wheel down: pan begin\n");
        VisualizerInput_onPanBegin(self);
    }
    if (engine_Mouse_held(engine_MouseButton_middle)) {
        VisualizerInput_handlePan(self);
    }
    if (engine_Mouse_released(engine_MouseButton_middle)) {
        log_debug("mouse wheel up: pan end\n");
        VisualizerInput_onPanEnd(self);
    }

    // Handle zooming
    scope_if(let scroll_delta = engine_Mouse_getScrollDelta(), scroll_delta != 0) {
        log_debug("mouse wheel scroll: zoom begin\n");
        VisualizerInput_handleZoom(self, scroll_delta);
    }

    // FIXME: Handle body spawning
    if (engine_Mouse_pressed(engine_MouseButton_right)) {
        log_debug("right mouse button pressed");
        let world_mouse = Visualizer_mousePosToWorld(self);

        toSome(self->spawn.body, Body_new(world_mouse, math_Vec2f_zero, 1.0f, 1.0f));
        toSome(self->spawn.angle, 0.0f);
        toSome(self->spawn.total, 0.0f);

    } else if (engine_Mouse_held(engine_MouseButton_right)) {
        log_debug("right mouse button held");
        if_some_mut(self->spawn.body, body) {
            let world_mouse = Visualizer_mousePosToWorld(self);
            let d           = math_Vec2f_sub(world_mouse, body->pos);
            if_some_mut(self->spawn.angle, angle) {
                let angle2    = atan2f(d.y, d.x);
                let a         = angle2 - *angle;
                let a_wrapped = fmodf(a + math_f32_pi, math_f32_tau) - math_f32_pi;

                self->spawn.total.value -= a_wrapped;
                *angle = angle2;

                body->mass   = exp2f(self->spawn.total.value / math_f32_tau);
                body->radius = cbrtf(body->mass);
            }
            else {
                toSome(self->spawn.angle, atan2f(d.y, d.x));
            }
            body->vel = d;
        }

    } else if (engine_Mouse_released(engine_MouseButton_right)) {
        log_debug("right mouse button released");
        if_some_mut(self->spawn.body, body) {
            if_none(self->spawn.confirmed) {
                toSome(self->spawn.confirmed, *body);
            }
            toNone(self->spawn.body);
            toNone(self->spawn.angle);
            toNone(self->spawn.total);
        }
    }
    return_void();
}

Err$void Visualizer_update(Visualizer* self) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    // Handle spawned body confirmation
    if_some_mut(self->spawn.confirmed, confirmed) {
        try(ArrList_append(&self->bodies.base, meta_refPtr(confirmed)));
        toNone(self->spawn.confirmed);
    }

    return_void();
}

static void     Visualizer_renderBodies(Visualizer* self);
static Err$void Visualizer_renderQuadTree(Visualizer* self);
Err$void        Visualizer_render(Visualizer* self) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    // Clear canvas
    engine_Canvas_clearDefault(self->canvas);
    // Draw bodies
    if (self->shows_bodies) {
        Visualizer_renderBodies(self);
    }
    // Draw quad-tree
    if (self->shows_quad_tree) {
        try(Visualizer_renderQuadTree(self));
    }

    return_void();
}

$inline_always void Visualizer_drawCircle(Visualizer* self, Vec2i screen_pos, f32 screen_radius, Color color) {
    if (1.0f < screen_radius) {
        return engine_Canvas_fillCircle(self->canvas, screen_pos.s[0], screen_pos.s[1], as$(i32, screen_radius), color);
    }
    if (Visualizer_min_render_radius < screen_radius) {
        return engine_Canvas_drawPixel(self->canvas, screen_pos.s[0], screen_pos.s[1], color);
    }
    // Calculate alpha based on coverage area
    let coverage = screen_radius * 2.0f;
    color.a      = as$(u8, 255.0f * coverage * Visualizer_alpha_scale);
    return engine_Canvas_drawPixel(self->canvas, screen_pos.s[0], screen_pos.s[1], color);
}
$inline_always void Visualizer_drawBodiesOnly(Visualizer* self) {
    let view_min = math_Vec2f_from(
        self->pos.x - 0.5f * (as$(f32, self->canvas->width) * self->scale),
        self->pos.y - 0.5f * (as$(f32, self->canvas->height) * self->scale)
    );
    let view_max = math_Vec2f_from(
        self->pos.x + 0.5f * (as$(f32, self->canvas->width) * self->scale),
        self->pos.y + 0.5f * (as$(f32, self->canvas->height) * self->scale)
    );
    for_slice (self->bodies.items, body) {
        let left   = body->pos.x - body->radius;
        let right  = body->pos.x + body->radius;
        let bottom = body->pos.y - body->radius;
        let top    = body->pos.y + body->radius;

        // If outside camera view, skip
        if (right < view_min.x || view_max.x < left || top < view_min.y || view_max.y < bottom) { continue; }

        let screen_pos    = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = body->radius * Visualizer_scaleInv(self);
        Visualizer_drawCircle(self, screen_pos, screen_radius, Visualizer_color_body);
    }
}
$inline_always void Visualizer_drawBodiesWithVelVec(Visualizer* self) {
    let view_min = math_Vec2f_from(
        self->pos.x - 0.5f * (as$(f32, self->canvas->width) * self->scale),
        self->pos.y - 0.5f * (as$(f32, self->canvas->height) * self->scale)
    );
    let view_max = math_Vec2f_from(
        self->pos.x + 0.5f * (as$(f32, self->canvas->width) * self->scale),
        self->pos.y + 0.5f * (as$(f32, self->canvas->height) * self->scale)
    );
    for_slice (self->bodies.items, body) {
        let left   = body->pos.x - body->radius;
        let right  = body->pos.x + body->radius;
        let bottom = body->pos.y - body->radius;
        let top    = body->pos.y + body->radius;

        // If outside camera view, skip
        if (right < view_min.x || view_max.x < left || top < view_min.y || view_max.y < bottom) { continue; }

        let screen_pos    = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = body->radius * Visualizer_scaleInv(self);
        Visualizer_drawCircle(self, screen_pos, screen_radius, Visualizer_color_body);

        // Draw velocity vector
        if (0.0f < math_Vec2f_lenSq(body->vel)) {
            let vel_end_world  = math_Vec2f_add(body->pos, math_Vec2f_scale(body->vel, Visualizer_scaleInv(self)));
            let vel_end_screen = Visualizer_worldToScreen(self, vel_end_world);
            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], vel_end_screen.s[0], vel_end_screen.s[1], Visualizer_color_vel_vec);
        }
    }
}
$inline_always void Visualizer_drawBodiesWithAccVec(Visualizer* self) {
    let view_min = math_Vec2f_from(
        self->pos.x - 0.5f * (as$(f32, self->canvas->width) * self->scale),
        self->pos.y - 0.5f * (as$(f32, self->canvas->height) * self->scale)
    );
    let view_max = math_Vec2f_from(
        self->pos.x + 0.5f * (as$(f32, self->canvas->width) * self->scale),
        self->pos.y + 0.5f * (as$(f32, self->canvas->height) * self->scale)
    );
    for_slice (self->bodies.items, body) {
        let left   = body->pos.x - body->radius;
        let right  = body->pos.x + body->radius;
        let bottom = body->pos.y - body->radius;
        let top    = body->pos.y + body->radius;

        // If outside camera view, skip
        if (right < view_min.x || view_max.x < left || top < view_min.y || view_max.y < bottom) { continue; }

        let screen_pos    = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = body->radius * Visualizer_scaleInv(self);
        Visualizer_drawCircle(self, screen_pos, screen_radius, Visualizer_color_body);

        // Draw acceleration vector
        if (0.0f < math_Vec2f_lenSq(body->acc)) {
            let acc_end_world  = math_Vec2f_add(body->pos, math_Vec2f_scale(body->acc, Visualizer_scaleInv(self)));
            let acc_end_screen = Visualizer_worldToScreen(self, acc_end_world);
            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], acc_end_screen.s[0], acc_end_screen.s[1], Visualizer_color_acc_vec);
        }
    }
}
$inline_always void Visualizer_drawBodiesWithVelAccVec(Visualizer* self) {
    let view_min = math_Vec2f_from(
        self->pos.x - 0.5f * (as$(f32, self->canvas->width) * self->scale),
        self->pos.y - 0.5f * (as$(f32, self->canvas->height) * self->scale)
    );
    let view_max = math_Vec2f_from(
        self->pos.x + 0.5f * (as$(f32, self->canvas->width) * self->scale),
        self->pos.y + 0.5f * (as$(f32, self->canvas->height) * self->scale)
    );
    for_slice (self->bodies.items, body) {
        let left   = body->pos.x - body->radius;
        let right  = body->pos.x + body->radius;
        let bottom = body->pos.y - body->radius;
        let top    = body->pos.y + body->radius;

        // If outside camera view, skip
        if (right < view_min.x || view_max.x < left || top < view_min.y || view_max.y < bottom) { continue; }

        let screen_pos    = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = body->radius * Visualizer_scaleInv(self);
        Visualizer_drawCircle(self, screen_pos, screen_radius, Visualizer_color_body);

        // Velocity vector
        if (0.0f < math_Vec2f_lenSq(body->vel)) {
            let vel_end_world  = math_Vec2f_add(body->pos, math_Vec2f_scale(body->vel, Visualizer_scaleInv(self)));
            let vel_end_screen = Visualizer_worldToScreen(self, vel_end_world);
            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], vel_end_screen.s[0], vel_end_screen.s[1], Visualizer_color_vel_vec);
        }

        // Acceleration vector
        if (0.0f < math_Vec2f_lenSq(body->acc)) {
            let acc_end_world  = math_Vec2f_add(body->pos, math_Vec2f_scale(body->acc, Visualizer_scaleInv(self)));
            let acc_end_screen = Visualizer_worldToScreen(self, acc_end_world);
            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], acc_end_screen.s[0], acc_end_screen.s[1], Visualizer_color_acc_vec);
        }
    }
}
static void Visualizer_renderBodies(Visualizer* self) {
    debug_assert_nonnull(self);

    let render_bodies_type = self->shows_bodies_vel_vec | self->shows_bodies_acc_vec << 1;
    switch (render_bodies_type) {
    case (0 | 0 << 1):
        Visualizer_drawBodiesOnly(self);
        break;
    case (1 | 0 << 1):
        Visualizer_drawBodiesWithVelVec(self);
        break;
    case (0 | 1 << 1):
        Visualizer_drawBodiesWithAccVec(self);
        break;
    case (1 | 1 << 1):
        Visualizer_drawBodiesWithVelAccVec(self);
        break;
    default:
        claim_unreachable;
    }
}

$inline_always void Visualizer_drawNode(Visualizer* self, Vec2f min, Vec2f max, Color color) {
    // Convert world min/max to screen coordinates
    let screen_min = Visualizer_worldToScreen(self, min);
    let screen_max = Visualizer_worldToScreen(self, max);

    engine_Canvas_fillRect(
        self->canvas,
        screen_min.s[0],
        screen_min.s[1],
        screen_max.s[0],
        screen_max.s[1],
        color
    );
}
static Err$void Visualizer_renderQuadTree(Visualizer* self) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    let depth_range = &self->depth_range;
    if (depth_range->max <= depth_range->min) {
        let stack = eval({
            let cache = &self->cache_stack;
            ArrList_clearRetainingCap(&cache->base);
            eval_return cache;
        });
        try(ArrList_append(
            &stack->base,
            meta_refPtr(create$(Visualizer_QuadCache, .node_idx = QuadTree_root, .depth = 0))
            )
        );

        var min_depth = usize_limit;
        var max_depth = 0ull;
        while_some(ArrList_popOrNull(&stack->base), capture) {
            let item = *meta_cast$(Visualizer_QuadCache*, capture);
            let node = Sli_at(self->nodes.items, item.node_idx);

            if (QuadNode_isLeaf(node)) {
                min_depth = prim_min(min_depth, item.depth);
                max_depth = prim_max(max_depth, item.depth);
                continue;
            }
            for (usize i = 0; i < 4; ++i) {
                try(ArrList_append(
                    &stack->base,
                    meta_refPtr(create$(Visualizer_QuadCache, .node_idx = node->children + i, .depth = item.depth + 1))
                    )
                );
            }
        }
        *depth_range = make$(TypeOf(*depth_range), .min = min_depth, .max = max_depth);
    }
    let min_depth = depth_range->min;
    let max_depth = depth_range->max;

    let stack = eval({
        let cache = &self->cache_stack;
        ArrList_clearRetainingCap(&cache->base);
        eval_return cache;
    });
    try(ArrList_append(
        &stack->base,
        meta_refPtr(create$(Visualizer_QuadCache, .node_idx = QuadTree_root, .depth = 0))
        )
    );
    while_some(ArrList_popOrNull(&stack->base), capture) {
        let item = *meta_cast$(Visualizer_QuadCache*, capture);
        let node = Sli_at(self->nodes.items, item.node_idx);

        if (QuadNode_isBranch(node) && item.depth < max_depth) {
            for (usize i = 0; i < 4; ++i) {
                try(ArrList_append(
                    &stack->base,
                    meta_refPtr(create$(Visualizer_QuadCache, .node_idx = node->children + i, .depth = item.depth + 1))
                    )
                );
            }
            continue;
        }
        if (item.depth < min_depth) { continue; }

        let quad = node->quad;
        let half = math_Vec2f_scale(math_Vec2f_scale(math_Vec2f_one, 0.5f), quad.size);
        let min  = math_Vec2f_sub(quad.center, half);
        let max  = math_Vec2f_add(quad.center, half);

        let t = as$(f32, item.depth - min_depth + as$(usize, !QuadNode_isEmpty(node)))
              / as$(f32, max_depth - min_depth + 1);

        let start_h = -100.0f;
        let end_h   = 80.0f;
        let h       = start_h + (end_h - start_h) * t;
        let s       = 100.0f;
        let l       = t * 100.0f;

        // TODO: Apply `HSLuv` color space
        let hsl   = Hsl_from(h, s, l);
        let color = Hsl_intoColor(hsl, 192);
        Visualizer_drawNode(self, min, max, color);
    }

    return_void();
}
