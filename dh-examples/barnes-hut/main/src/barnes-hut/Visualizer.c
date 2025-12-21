#include "barnes-hut/Visualizer.h"
#include "barnes-hut/QuadTree.h"

#include "dh/ArrList.h"
#include "dh/debug/common.h"
#include "dh/log.h"
#include "dh/time/Duration.h"
#include "dh/time/Instant.h"
#include "dh/pipe.h"

#include "engine/Color.h"
#include "engine/Mouse.h"

/* Color definitions for rendering nodes in the visualizer. */
#define Visualizer_color_body (literal_Color_fromOpaque(192, 192, 192))
#define Visualizer_color_vel_vec (literal_Color_fromOpaque(0, 0, 192))
#define Visualizer_color_acc_vec (literal_Color_fromOpaque(192, 0, 0))
#define Visualizer_min_render_radius (0.5f) // Minimum radius to render as solid
#define Visualizer_alpha_scale (0.8f) // Alpha multiplier for blending

/* Function to calculate the inverse scale factor for rendering nodes.  */
$maybe_unused
$static $inline_always
fn_(Visualizer_scale(Visualizer* self), f32) { return self->scale; }
$maybe_unused
$static $inline_always
fn_(Visualizer_scaleInv(Visualizer* self), f32) { return 1.0f / self->scale; }

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
$static $inline_always
fn_(Visualizer_screenCenter(Visualizer* self), m_V2i32) {
    // Return the middle pixel (handles even/odd dimensions by integer truncation)
    let cx = (as$(i32, self->canvas->buffer.width) - 1) / 2;
    let cy = (as$(i32, self->canvas->buffer.height) - 1) / 2;
    return m_V2i32_from(cx, cy);
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
static $inline_always
fn_(Visualizer_worldToScreen(Visualizer* self, m_V2f32 world_pos), m_V2i32) {
    let center = Visualizer_screenCenter(self);
    let w_minus_cam = m_V2f32_sub(world_pos, self->pos);
    let divided = m_V2f32_scale(w_minus_cam, 1.0f / self->scale);

    let sx = as$(i32, roundf(as$(f32, center.x) + divided.x));
    let sy = as$(i32, roundf(as$(f32, center.y) - divided.y));
    return m_V2i32_from(sx, sy);
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
static $inline_always
fn_(Visualizer_screenToWorld(Visualizer* self, m_V2i32 screen_pos), m_V2f32) {
    let center = Visualizer_screenCenter(self);
    let dx = as$(f32, screen_pos.x - center.x);
    let dy = as$(f32, center.y - screen_pos.y);
    let multiplied = m_V2f32_scale(m_V2f32_from(dx, dy), self->scale);

    return m_V2f32_add(self->pos, multiplied);
}
/// Returns the current mouse position converted to world coords.
static $inline_always
fn_(Visualizer_mousePosToWorld(Visualizer* self, engine_Mouse* mouse), m_V2f32) {
    return Visualizer_screenToWorld(self, engine_Mouse_getPos(mouse));
}

/* Core visualizer functions */
fn_(Visualizer_create(mem_Allocator allocator, engine_Canvas* canvas), E$Visualizer $scope) {
    return_ok({
        .pos = m_V2f32_zero, // camera center pos
        .scale = 2.0f, // camera zoom (2.0f == x0.5, 0.5f == x2)
        // .scale = 3600,

        .pan_screen_begin = m_V2i32_zero,
        .pan_cam_begin = m_V2f32_zero,

        .zoom_anchor_world = m_V2f32_zero,

        .shows_bodies = true,
        .shows_bodies_vel_vec = false,
        .shows_bodies_acc_vec = false,
        .shows_quad_tree = false,

        .depth_range = {
            .min = 0,
            .max = 0,
        },
        .cache_stack = type$(ArrList$Visualizer_QuadCache, try_(ArrList_initCap(typeInfo$(Visualizer_QuadCache), allocator, 12500))),

        .spawn = {
            .body = none(),
            .angle = none(),
            .total = none(),
            .confirmed = none(),
        },

        .bodies = type$(ArrList$Body, ArrList_init(typeInfo$(Body), allocator)),
        .nodes = type$(ArrList$QuadNode, ArrList_init(typeInfo$(QuadNode), allocator)),

        .allocator = allocator,
        .canvas = canvas,
    });
}
$unscoped;

fn_(Visualizer_destroy(Visualizer* self), void) {
    debug_assert_nonnull(self);

    ArrList_fini(self->cache_stack.base);

    ArrList_fini(self->bodies.base);
    ArrList_fini(self->nodes.base);
}

typedef struct Control {
    engine_KeyCode key;
    m_V2f32 vec;
} Control;
use_S$(Control);
static fn_(Control_list(void), S_const$Control) {
    static const Control controls[] = {
        { .key = engine_KeyCode_w, .vec = m_V2f32_up },
        { .key = engine_KeyCode_a, .vec = m_V2f32_left },
        { .key = engine_KeyCode_s, .vec = m_V2f32_down },
        { .key = engine_KeyCode_d, .vec = m_V2f32_right },
    };
    static const usize controls_len = countOf(controls);
    return (S_const$Control){
        .ptr = controls,
        .len = controls_len,
    };
}

static $inline_always
fn_(VisualizerInput_resetPos(Visualizer* self), void) {
    self->pos = m_V2f32_zero;
}
static $inline_always
fn_(VisualizerInput_toggleShowingBodies(Visualizer* self), void) {
    self->shows_bodies = !self->shows_bodies;
}
static $inline_always
fn_(VisualizerInput_toggleVisualizationBodiesVelVec(Visualizer* self), void) {
    self->shows_bodies_vel_vec = !self->shows_bodies_vel_vec;
}
static $inline_always
fn_(VisualizerInput_toggleVisualizationBodiesAccVec(Visualizer* self), void) {
    self->shows_bodies_acc_vec = !self->shows_bodies_acc_vec;
}
static $inline_always
fn_(VisualizerInput_toggleVisualizationQuadTree(Visualizer* self), void) {
    self->shows_quad_tree = !self->shows_quad_tree;
}
// Call this when the middle mouse button is first pressed:
static $inline_always
fn_(VisualizerInput_onPanBegin(Visualizer* self, engine_Mouse* mouse), void) {
    if (self->is_panning) { return; }
    // Store screen coordinates & camera pos
    self->pan_screen_begin = engine_Mouse_getPos(mouse);
    self->pan_cam_begin = self->pos;
    self->is_panning = true;
}
// Call this each frame while the middle mouse is held:
static $inline_always
fn_(VisualizerInput_handlePan(Visualizer* self, engine_Mouse* mouse), void) {
    // Current mouse position in SCREEN space
    let mouse_now_screen = engine_Mouse_getPos(mouse);

    // Convert BOTH the original "pan begin" screen coords
    // and the new screen coords into world space
    let old_mouse_world = Visualizer_screenToWorld(self, self->pan_screen_begin);
    let new_mouse_world = Visualizer_screenToWorld(self, mouse_now_screen);

    // The mouse has moved by (old_mouse_world - new_mouse_world) in world coords
    // So shift the camera by that same difference (so the "under-mouse" point remains under mouse)
    let diff_world = m_V2f32_sub(old_mouse_world, new_mouse_world);

    // Restore the camera to its position at the moment panning began,
    // then offset it by diff_world
    self->pos = m_V2f32_add(self->pan_cam_begin, diff_world);
}
// Call this when the middle mouse button is released:
static $inline_always
fn_(VisualizerInput_onPanEnd(Visualizer* self, engine_Mouse* mouse), void) {
    let_ignore = mouse;
    self->is_panning = false;
    // // Reset pan screen & cam begin to zero
    // self->pan_screen_begin = m_V2f32_zero;
    // self->pan_cam_begin    = m_V2f32_zero;
}
static $inline_always
fn_(VisualizerInput_handleZoom(Visualizer* self, i32 scroll_delta, engine_Mouse* mouse), void) {
    if (scroll_delta == 0) { return; }

    // 1) Find the world coords under the mouse BEFORE changing scale
    let mouse_screen = engine_Mouse_getPos(mouse);
    let mouse_world_before = Visualizer_screenToWorld(self, mouse_screen);

    // 2) Choose a zoom factor
    //    e.g. scrolling up => zoom in => "scale goes down" if you want
    //    or scrolling up => zoom out => "scale goes up" - your choice
    //    (Below code: scroll_delta > 0 => bigger scale => zoom out)
    let zoom_sensitivity = 0.1f;
    let zoom_factor = ({
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
    let diff = m_V2f32_sub(mouse_world_before, mouse_world_after);
    self->pos = m_V2f32_add(self->pos, diff);
}
fn_(Visualizer_processInput(Visualizer* self, engine_Window* window, engine_Input* input), E$void $scope) {
    debug_assert_nonnull(self);
    let_ignore = window;

    let keyboard = input->keyboard;
    let mouse = input->mouse;
    // Handle resets
    if (engine_Keyboard_pressed(keyboard, engine_KeyCode_r)) {
        log_debug("pressed 'r' to reset position\n");
        VisualizerInput_resetPos(self);
    }

    // Handle toggles
    if (engine_Keyboard_pressed(keyboard, engine_KeyCode_n1)) {
        log_debug("pressed '1' to toggle bodies showing\n");
        VisualizerInput_toggleShowingBodies(self);
    }
    if (engine_Keyboard_pressed(keyboard, engine_KeyCode_n2)) {
        log_debug("pressed '2' to toggle bodies velocity vector visualization\n");
        VisualizerInput_toggleVisualizationBodiesVelVec(self);
    }
    if (engine_Keyboard_pressed(keyboard, engine_KeyCode_n3)) {
        log_debug("pressed '3' to toggle bodies acceleration vector visualization\n");
        VisualizerInput_toggleVisualizationBodiesAccVec(self);
    }
    if (engine_Keyboard_pressed(keyboard, engine_KeyCode_q)) {
        log_debug("pressed 'q' to toggle quad-tree visualization\n");
        VisualizerInput_toggleVisualizationQuadTree(self);
    }

    // Handle moving
    let controls = Control_list();
    for_slice(controls, control) {
        if (engine_Keyboard_held(keyboard, control->key)) {
            log_debug("pressed '%c' to move\n", control->key);
            pipe_(&self->pos, (m_V2f32_addAsg, (pipe_(control->vec, (m_V2f32_scale, (5.0f * Visualizer_scaleInv(self)))))));
        }
    }

    // Handle panning
    if (engine_Mouse_pressed(mouse, engine_MouseButton_middle)) {
        log_debug("mouse wheel down: pan begin\n");
        VisualizerInput_onPanBegin(self, mouse);
    }
    if (engine_Mouse_held(mouse, engine_MouseButton_middle)) {
        VisualizerInput_handlePan(self, mouse);
    }
    if (engine_Mouse_released(mouse, engine_MouseButton_middle)) {
        log_debug("mouse wheel up: pan end\n");
        VisualizerInput_onPanEnd(self, mouse);
    }

    // Handle zooming
    // FIXME: Correct handling of zooming start/stop and speed control properly based on mouse wheel input
    if_(const m_V2f32 wheel_scroll_delta
        = engine_Mouse_getWheelScrollDelta(mouse),
        wheel_scroll_delta.y != 0) {
        log_debug("mouse wheel scroll: zoom begin\n");
        VisualizerInput_handleZoom(self, as$(i32, wheel_scroll_delta.y), mouse);
    }

    // FIXME: Correct handling of body spawning
    let spawn = &self->spawn;
    if (engine_Mouse_pressed(mouse, engine_MouseButton_right)) {
        log_debug("right mouse button pressed");
        let world_mouse = Visualizer_mousePosToWorld(self, mouse);

        O_asg(&spawn->body, some(Body_new(world_mouse, m_V2f32_zero, 1.0f, 1.0f)));
        O_asg(&spawn->angle, some(0.0f));
        O_asg(&spawn->total, some(0.0f));

    } else if (engine_Mouse_held(mouse, engine_MouseButton_right)) {
        log_debug("right mouse button held");
        if_some(O_asPtr(&spawn->body), body) {
            let world_mouse = Visualizer_mousePosToWorld(self, mouse);
            let d = m_V2f32_sub(world_mouse, body->pos);
            if_some(O_asPtr(&spawn->angle), angle) {
                let angle2 = atan2f(d.y, d.x);
                let a = angle2 - *angle;
                let a_wrapped = fmodf(a + math_f32_pi, math_f32_tau) - math_f32_pi;

                spawn->total.value -= a_wrapped;
                *angle = angle2;

                body->mass = exp2f(spawn->total.value / math_f32_tau);
                body->radius = cbrtf(body->mass);
            }
            else {
                O_asg(&spawn->angle, some(atan2f(d.y, d.x)));
            }
            body->vel = d;
        }

    } else if (engine_Mouse_released(mouse, engine_MouseButton_right)) {
        log_debug("right mouse button released");
        if_some(spawn->body, body) {
            if_none(spawn->confirmed) {
                O_asg(&spawn->confirmed, some(body));
            }
            O_asg(&spawn->body, none());
            O_asg(&spawn->angle, none());
            O_asg(&spawn->total, none());
        }
    }
    return_ok({});
}
$unscoped;

fn_(Visualizer_update(Visualizer* self), E$void $scope) {
    debug_assert_nonnull(self);
    // Handle spawned body confirmation
    let spawn = &self->spawn;
    if_some(O_asPtr(&spawn->confirmed), confirmed) {
        try_(ArrList_append(self->bodies.base, meta_refPtr(confirmed)));
        O_asg(&spawn->confirmed, none());
    }
    return_ok({});
}
$unscoped;

static fn_(Visualizer_renderBodies(Visualizer* self), void);
static fn_(Visualizer_renderQuadTree(Visualizer* self), E$void);
fn_(Visualizer_render(Visualizer* self), E$void $scope) {
    debug_assert_nonnull(self);

    /* let begin_instant = time_Instant_now(); */
    // Clear canvas
    engine_Canvas_clear(self->canvas, (O$Color)none());
    // Draw bodies
    if (self->shows_bodies) {
        Visualizer_renderBodies(self);
    }
    // Draw quad-tree
    if (self->shows_quad_tree) {
        try_(Visualizer_renderQuadTree(self));
    }
    /* let end_duration = time_Instant_elapsed(begin_instant);
    log_info("Render time: %lfms", time_Duration_asSecs$f64(end_duration)); */

    return_ok({});
}
$unscoped;

static $inline_always
fn_(Visualizer_drawCircle(Visualizer* self, m_V2i32 screen_pos, f32 screen_radius, Color color), void) {
    if (1.0f < screen_radius) {
        return engine_Canvas_fillCircle(self->canvas, screen_pos.s[0], screen_pos.s[1], as$(i32, screen_radius), color);
    }
    if (Visualizer_min_render_radius < screen_radius) {
        return engine_Canvas_drawPixel(self->canvas, screen_pos.s[0], screen_pos.s[1], color);
    }
    // Calculate alpha based on coverage area
    let coverage = screen_radius * 2.0f;
    color.a = as$(u8, 255.0f * coverage * Visualizer_alpha_scale);
    return engine_Canvas_drawPixel(self->canvas, screen_pos.s[0], screen_pos.s[1], color);
}
static $inline_always
fn_(Visualizer_drawBodiesOnly(Visualizer* self), void) {
    let view_min = m_V2f32_from(
        self->pos.x - 0.5f * (as$(f32, self->canvas->buffer.width) * self->scale),
        self->pos.y - 0.5f * (as$(f32, self->canvas->buffer.height) * self->scale)
    );
    let view_max = m_V2f32_from(
        self->pos.x + 0.5f * (as$(f32, self->canvas->buffer.width) * self->scale),
        self->pos.y + 0.5f * (as$(f32, self->canvas->buffer.height) * self->scale)
    );
    for_slice(self->bodies.items, body) {
        let left = body->pos.x - body->radius;
        let right = body->pos.x + body->radius;
        let bottom = body->pos.y - body->radius;
        let top = body->pos.y + body->radius;

        // If outside camera view, skip
        if (right < view_min.x || view_max.x < left || top < view_min.y || view_max.y < bottom) { continue; }

        let screen_pos = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = body->radius * Visualizer_scaleInv(self);
        Visualizer_drawCircle(self, screen_pos, screen_radius, Visualizer_color_body);
    }
}
static $inline_always
fn_(Visualizer_drawBodiesWithVelVec(Visualizer* self), void) {
    let view_min = m_V2f32_from(
        self->pos.x - 0.5f * (as$(f32, self->canvas->buffer.width) * self->scale),
        self->pos.y - 0.5f * (as$(f32, self->canvas->buffer.height) * self->scale)
    );
    let view_max = m_V2f32_from(
        self->pos.x + 0.5f * (as$(f32, self->canvas->buffer.width) * self->scale),
        self->pos.y + 0.5f * (as$(f32, self->canvas->buffer.height) * self->scale)
    );
    for_slice(self->bodies.items, body) {
        let left = body->pos.x - body->radius;
        let right = body->pos.x + body->radius;
        let bottom = body->pos.y - body->radius;
        let top = body->pos.y + body->radius;

        // If outside camera view, skip
        if (right < view_min.x || view_max.x < left || top < view_min.y || view_max.y < bottom) { continue; }

        let screen_pos = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = body->radius * Visualizer_scaleInv(self);
        Visualizer_drawCircle(self, screen_pos, screen_radius, Visualizer_color_body);

        // Draw velocity vector
        if (0.0f < m_V2f32_lenSq(body->vel)) {
            let vel_end_world = m_V2f32_add(body->pos, m_V2f32_scale(body->vel, Visualizer_scaleInv(self)));
            let vel_end_screen = Visualizer_worldToScreen(self, vel_end_world);
            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], vel_end_screen.s[0], vel_end_screen.s[1], Visualizer_color_vel_vec);
        }
    }
}
static $inline_always
fn_(Visualizer_drawBodiesWithAccVec(Visualizer* self), void) {
    let view_min = m_V2f32_from(
        self->pos.x - 0.5f * (as$(f32, self->canvas->buffer.width) * self->scale),
        self->pos.y - 0.5f * (as$(f32, self->canvas->buffer.height) * self->scale)
    );
    let view_max = m_V2f32_from(
        self->pos.x + 0.5f * (as$(f32, self->canvas->buffer.width) * self->scale),
        self->pos.y + 0.5f * (as$(f32, self->canvas->buffer.height) * self->scale)
    );
    for_slice(self->bodies.items, body) {
        let left = body->pos.x - body->radius;
        let right = body->pos.x + body->radius;
        let bottom = body->pos.y - body->radius;
        let top = body->pos.y + body->radius;

        // If outside camera view, skip
        if (right < view_min.x || view_max.x < left || top < view_min.y || view_max.y < bottom) { continue; }

        let screen_pos = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = body->radius * Visualizer_scaleInv(self);
        Visualizer_drawCircle(self, screen_pos, screen_radius, Visualizer_color_body);

        // Draw acceleration vector
        if (0.0f < m_V2f32_lenSq(body->acc)) {
            let acc_end_world = m_V2f32_add(body->pos, m_V2f32_scale(body->acc, Visualizer_scaleInv(self)));
            let acc_end_screen = Visualizer_worldToScreen(self, acc_end_world);
            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], acc_end_screen.s[0], acc_end_screen.s[1], Visualizer_color_acc_vec);
        }
    }
}
static $inline_always
fn_(Visualizer_drawBodiesWithVelAccVec(Visualizer* self), void) {
    let view_min = m_V2f32_from(
        self->pos.x - 0.5f * (as$(f32, self->canvas->buffer.width) * self->scale),
        self->pos.y - 0.5f * (as$(f32, self->canvas->buffer.height) * self->scale)
    );
    let view_max = m_V2f32_from(
        self->pos.x + 0.5f * (as$(f32, self->canvas->buffer.width) * self->scale),
        self->pos.y + 0.5f * (as$(f32, self->canvas->buffer.height) * self->scale)
    );
    for_slice(self->bodies.items, body) {
        let left = body->pos.x - body->radius;
        let right = body->pos.x + body->radius;
        let bottom = body->pos.y - body->radius;
        let top = body->pos.y + body->radius;

        // If outside camera view, skip
        if (right < view_min.x || view_max.x < left || top < view_min.y || view_max.y < bottom) { continue; }

        let screen_pos = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = body->radius * Visualizer_scaleInv(self);
        Visualizer_drawCircle(self, screen_pos, screen_radius, Visualizer_color_body);

        // Velocity vector
        if (0.0f < m_V2f32_lenSq(body->vel)) {
            let vel_end_world = m_V2f32_add(body->pos, m_V2f32_scale(body->vel, Visualizer_scaleInv(self)));
            let vel_end_screen = Visualizer_worldToScreen(self, vel_end_world);
            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], vel_end_screen.s[0], vel_end_screen.s[1], Visualizer_color_vel_vec);
        }

        // Acceleration vector
        if (0.0f < m_V2f32_lenSq(body->acc)) {
            let acc_end_world = m_V2f32_add(body->pos, m_V2f32_scale(body->acc, Visualizer_scaleInv(self)));
            let acc_end_screen = Visualizer_worldToScreen(self, acc_end_world);
            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], acc_end_screen.s[0], acc_end_screen.s[1], Visualizer_color_acc_vec);
        }
    }
}
fn_(Visualizer_renderBodies(Visualizer* self), void) {
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

static $inline_always
fn_(Visualizer_drawNode(Visualizer* self, m_V2f32 min, m_V2f32 max, Color color), void) {
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
fn_(Visualizer_renderQuadTree(Visualizer* self), E$void $scope) {
    debug_assert_nonnull(self);

    let depth_range = &self->depth_range;
    if (depth_range->max <= depth_range->min) {
        let stack = eval({
            let cache = &self->cache_stack;
            ArrList_clearRetainingCap(cache->base);
            eval_return cache;
        });
        try_(ArrList_append(
            stack->base,
            meta_refPtr(create$(Visualizer_QuadCache, .node_idx = QuadTree_root, .depth = 0))
        ));

        var min_depth = usize_limit;
        var max_depth = 0ull;
        while_some(ArrList_popOrNull(stack->base), capture) {
            let item = *meta_cast$(Visualizer_QuadCache*, capture);
            let node = Sli_at(self->nodes.items, item.node_idx);

            if (QuadNode_isLeaf(node)) {
                min_depth = prim_min(min_depth, item.depth);
                max_depth = prim_max(max_depth, item.depth);
                continue;
            }
            for (usize i = 0; i < 4; ++i) {
                try_(ArrList_append(
                    stack->base,
                    meta_refPtr(create$(Visualizer_QuadCache, .node_idx = node->children + i, .depth = item.depth + 1))
                ));
            }
        }
        *depth_range = make$(TypeOf(*depth_range), .min = min_depth, .max = max_depth);
    }
    let min_depth = depth_range->min;
    let max_depth = depth_range->max;

    let stack = eval({
        let cache = &self->cache_stack;
        ArrList_clearRetainingCap(cache->base);
        eval_return cache;
    });
    try_(ArrList_append(
        stack->base,
        meta_refPtr(create$(Visualizer_QuadCache, .node_idx = QuadTree_root, .depth = 0))
    ));
    while_some(ArrList_popOrNull(stack->base), capture) {
        let item = *meta_cast$(Visualizer_QuadCache*, capture);
        let node = Sli_at(self->nodes.items, item.node_idx);

        if (QuadNode_isBranch(node) && item.depth < max_depth) {
            for (usize i = 0; i < 4; ++i) {
                try_(ArrList_append(
                    stack->base,
                    meta_refPtr(create$(Visualizer_QuadCache, .node_idx = node->children + i, .depth = item.depth + 1))
                ));
            }
            continue;
        }
        if (item.depth < min_depth) { continue; }

        let quad = node->quad;
        let half = m_V2f32_scale(m_V2f32_scale(m_V2f32_one, 0.5f), quad.size);
        let min = m_V2f32_sub(quad.center, half);
        let max = m_V2f32_add(quad.center, half);

        let t = as$(f32, item.depth - min_depth + as$(usize, !QuadNode_isEmpty(node)))
              / as$(f32, max_depth - min_depth + 1);

        let start_h = -100.0f;
        let end_h = 80.0f;
        let h = start_h + (end_h - start_h) * t;
        let s = 100.0f;
        let l = t * 100.0f;

        // TODO: Apply `HSLuv` color space
        let hsl = Hsl_from(h, s, l);
        let color = Hsl_intoColor(hsl, 192);
        Visualizer_drawNode(self, min, max, color);
    }

    return_ok({});
}
$unscoped;
