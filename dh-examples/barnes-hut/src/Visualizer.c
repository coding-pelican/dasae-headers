#include "Visualizer.h"
#include "QuadTree.h"
#include "dh/ArrList.h"
#include "dh/log.h"
#include "engine/color.h"

/* Color definitions for rendering nodes in the visualizer. */
#define Visualizer_color_body        (literal_Color_fromOpaque(192, 192, 192))
#define Visualizer_color_vel_vec     (literal_Color_fromOpaque(0, 0, 192))
#define Visualizer_color_acc_vec     (literal_Color_fromOpaque(192, 0, 0))
#define Visualizer_min_render_radius (0.5f) // Minimum radius to render as solid
#define Visualizer_alpha_scale       (0.8f) // Alpha multiplier for blending

/* Function to calculate the inverse scale factor for rendering nodes.  */
force_inline f32   Visualizer_invScale(Visualizer* self) { return 1.0f / self->scale; }
/// Unified coordinate system transformations
/// Screen Coordinates          World Coordinates
/// (0,0)  --------- (w-1,0)    (-w/2*scale, +h/2*scale)
///    |               |                ↑ y+
///    |               |                |
///    |    (CENTER)   |                +--→ x+
///    |               |
/// (0,h-1) -------- (w-1,h-1)  (-w/2*scale, -h/2*scale)
force_inline Vec2i Visualizer_worldToScreen(Visualizer* self, Vec2f world_pos) {
    let canvas = self->canvas;
    let width  = canvas->width;
    let height = canvas->height;

    // Convert to camera-relative coordinates
    var relative = math_Vec2f_sub(world_pos, self->pos);
    relative     = math_Vec2f_scale(relative, Visualizer_invScale(self));

    // // Aspect ratio compensation
    // let aspect = as$(f32, width) / as$(f32, height);
    // relative.x *= aspect;

    // Calculate screen center offsets
    let center_x = as$(f32, width - 1) / 2.0f;
    let center_y = as$(f32, height - 1) / 2.0f;

    // Convert to screen coordinates (Y-down)
    return math_Vec2i_from(
        as$(i32, roundf(relative.x + center_x)),
        as$(i32, roundf(center_y - relative.y))
    );
}
/// Unified coordinate system transformations
/// // Example usage in body spawning:
/// if (engine_Mouse_pressed(engine_MouseButton_right)) {
///     let mouse_pos = engine_Mouse_getPosition();
///     let world_mouse = Visualizer_screenToWorld(self, window, mouse_pos);
///
///     // Spawn body at calculated world position
///     self->spawn.body = some(Body_new(world_mouse, math_Vec2f_zero, 1.0f, 1.0f));
/// }
force_inline math_Vec2f Visualizer_screenToWorld(Visualizer* self, engine_Window* window, Vec2i screen_pos) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(window);

    let client_size = engine_Window_getClientSize(window);
    let width       = client_size.x;
    let height      = client_size.y;

    // Calculate screen center offsets (handles even/odd dimensions)
    // let center_x = (width - (~width & 1)) >> 1;
    // let center_y = (height - (~height & 1)) >> 1;
    let center_x = as$(f32, width - 1) / 2.0f;
    let center_y = as$(f32, height - 1) / 2.0f;

    // Convert to centered coordinates (world-space oriented)
    let screen_x = eval(
        var x = as$(f32, screen_pos.x) - center_x;
        // Aspect ratio compensation
        // let aspect = as$(f32, width) / as$(f32, height);
        // x /= aspect;
        eval_return(x);
    );
    let screen_y = center_y - as$(f32, screen_pos.y);

    // Apply inverse scale and camera offset
    return math_Vec2f_add(
        math_Vec2f_scale(math_Vec2f_from(screen_x, screen_y), Visualizer_invScale(self)),
        self->pos
    );
}
force_inline math_Vec2f Visualizer_calculateWorldMouse(Visualizer* self, engine_Window* window, Vec2i screen_pos) {
    return Visualizer_screenToWorld(self, window, screen_pos);
}

/* Core visualizer functions */

Err$Visualizer Visualizer_create(mem_Allocator allocator, engine_Canvas* canvas) {
    reserveReturn(Err$Visualizer);
    return_ok((Visualizer){
        .pos   = math_Vec2f_zero,
        .scale = 2.0f,
        // .scale = 3600,

        .shows_bodies         = true,
        .shows_bodies_vel_vec = false,
        .shows_bodies_acc_vec = false,
        .shows_quad_tree      = false,

        .depth_range = {
            .min = 0,
            .max = 0,
        },
        .stack = {
            .nodes  = typed(ArrList$usize, try(ArrList_initCap(typeInfo(usize), allocator, 512))),
            .depths = typed(ArrList$usize, try(ArrList_initCap(typeInfo(usize), allocator, 512))),
            .len    = 0,
        },

        .spawn = {
            .body      = none(),
            .angle     = none(),
            .total     = none(),
            .confirmed = none(),
        },

        .bodies = typed(ArrList$Body, ArrList_init(typeInfo(Body), allocator)),
        .nodes  = typed(ArrList$QuadNode, ArrList_init(typeInfo(QuadNode), allocator)),

        .allocator = allocator,
        .canvas    = canvas,
    });
}

void Visualizer_destroy(Visualizer* self) {
    debug_assert_nonnull(self);

    ArrList_fini(&self->stack.pair[0].base);
    ArrList_fini(&self->stack.pair[1].base);

    ArrList_fini(&self->bodies.base);
    ArrList_fini(&self->nodes.base);
}

force_inline void VisualizerInput_resetPos(Visualizer* self) {
    self->pos = math_Vec2f_zero;
}
force_inline void VisualizerInput_toggleShowingBodies(Visualizer* self) {
    self->shows_bodies = !self->shows_bodies;
}
force_inline void VisualizerInput_toggleVisualizationBodiesVelVec(Visualizer* self) {
    self->shows_bodies_vel_vec = !self->shows_bodies_vel_vec;
}
force_inline void VisualizerInput_toggleVisualizationBodiesAccVec(Visualizer* self) {
    self->shows_bodies_acc_vec = !self->shows_bodies_acc_vec;
}
force_inline void VisualizerInput_toggleVisualizationQuadTree(Visualizer* self) {
    self->shows_quad_tree = !self->shows_quad_tree;
}
force_inline void VisualizerInput_handleZoom(Visualizer* self, engine_Window* window, i32 scroll_delta) {
    if (scroll_delta == 0) { return; }

    const f32 zoom_sensitivity = 0.1f;

    var zoom = 1.0f - (f32)scroll_delta * zoom_sensitivity;
    zoom     = fmaxf(zoom, zoom_sensitivity);

    let mouse_pos   = engine_Mouse_getPosition();
    let world_mouse = Visualizer_calculateWorldMouse(self, window, mouse_pos);

    // Maintain focal point during zoom
    self->pos = math_Vec2f_add(
        self->pos,
        math_Vec2f_scale(world_mouse, 1.0f - zoom)
    );
    self->scale *= zoom;
}
force_inline void VisualizerInput_handlePan(Visualizer* self, engine_Window* window) {
    let mouse_delta = engine_Mouse_getDelta();
    if (mouse_delta.s[0] == 0 && mouse_delta.s[1] == 0) { return; }

    let client_size = engine_Window_getClientSize(window);
    if (client_size.s[0] <= 0 || client_size.s[1] <= 0) { return; }

    // Use double-precision for critical position calculations
    let aspect_ratio = (f32)client_size.s[0] / (f32)client_size.s[1];

    // Calculate movement in screen pixels
    f64 world_per_pixel_x = (f64)self->scale * 2.0 / (f64)client_size.s[0];
    f64 world_per_pixel_y = (f64)self->scale * 2.0 / (f64)client_size.s[1];

    // Apply aspect ratio compensation only to horizontal axis
    world_per_pixel_x /= aspect_ratio;

    // Convert to world delta using proper coordinate system
    math_Vec2d world_delta = {
        .x = -(f64)mouse_delta.s[0] * world_per_pixel_x,
        .y = (f64)mouse_delta.s[1] * world_per_pixel_y // Natural Y movement
    };

    // Update position with double-precision
    self->pos.x += (f32)world_delta.x;
    self->pos.y += (f32)world_delta.y;
}
Err$void Visualizer_processInput(Visualizer* self, engine_Window* window) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);
    debug_assert_nonnull(window);

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

    // Handle zooming
    scope_if(let scroll_delta = engine_Mouse_getScrollDelta(), scroll_delta != 0) {
        VisualizerInput_handleZoom(self, window, scroll_delta);
    }
    // Handle panning
    if (engine_Mouse_held(engine_MouseButton_middle)) {
        VisualizerInput_handlePan(self, window);
    }

    // Handle body spawning
    if (engine_Mouse_pressed(engine_MouseButton_right)) {
        log_debug("right mouse button pressed");
        let mouse_pos   = engine_Mouse_getPosition();
        let world_mouse = Visualizer_calculateWorldMouse(self, window, mouse_pos);


        self->spawn.body  = (TypeOf(self->spawn.body))some(Body_new(world_mouse, math_Vec2f_zero, 1.0f, 1.0f));
        self->spawn.angle = (TypeOf(self->spawn.angle))some(0.0f);
        self->spawn.total = (TypeOf(self->spawn.total))some(0.0f);

    } else if (engine_Mouse_held(engine_MouseButton_right)) {
        log_debug("right mouse button held");
        if_some_mut(self->spawn.body, body) {
            let mouse_pos   = engine_Mouse_getPosition();
            let world_mouse = Visualizer_calculateWorldMouse(self, window, mouse_pos);
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
                self->spawn.angle = (TypeOf(self->spawn.angle))some(atan2f(d.y, d.x));
            }
            body->vel = d;
        }

    } else if (engine_Mouse_released(engine_MouseButton_right)) {
        log_debug("right mouse button released");
        if_some_mut(self->spawn.body, body) {
            if_none(self->spawn.confirmed) {
                self->spawn.confirmed = (TypeOf(self->spawn.confirmed))some(*body);
            }
            self->spawn.body  = (TypeOf(self->spawn.body))none();
            self->spawn.angle = (TypeOf(self->spawn.angle))none();
            self->spawn.total = (TypeOf(self->spawn.total))none();
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
        self->spawn.confirmed = (TypeOf(self->spawn.confirmed))none();
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

// Modified circle rendering with alpha blending
force_inline void Visualizer_drawCircle(Visualizer* self, Vec2i screen_pos, f32 screen_radius, Color color) {
    if (Visualizer_min_render_radius <= screen_radius) {
        return engine_Canvas_fillCircle(
            self->canvas,
            screen_pos.s[0],
            screen_pos.s[1],
            as$(i32, screen_radius),
            Visualizer_color_body
        );
    }
    // Calculate alpha based on coverage area
    let coverage = screen_radius * 2.0f;
    color.a      = as$(u8, 255.0f * coverage * Visualizer_alpha_scale);
    engine_Canvas_drawPixel(self->canvas, screen_pos.s[0], screen_pos.s[1], color);
}
force_inline void Visualizer_drawBodiesOnly(Visualizer* self) {
    for_slice(self->bodies.items, body) {
        let screen_pos    = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = body->radius * Visualizer_invScale(self);
        // if (1.0f <= screen_radius) {
        //     engine_Canvas_fillCircle(
        //         self->canvas,
        //         screen_pos.s[0],
        //         screen_pos.s[1],
        //         as$(i32, screen_radius),
        //         Visualizer_color_body
        //     );
        // } else {
        //     Visualizer_drawCircle(self, screen_pos, screen_radius, Visualizer_color_body);
        // }
        Visualizer_drawCircle(self, screen_pos, screen_radius, Visualizer_color_body);
    }
}
force_inline void Visualizer_drawBodiesWithVelVec(Visualizer* self) {
    for_slice(self->bodies.items, body) {
        let screen_pos    = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = body->radius * Visualizer_invScale(self);
        Visualizer_drawCircle(self, screen_pos, screen_radius, Visualizer_color_body);

        // Draw velocity vector
        if (0.0f < math_Vec2f_lenSq(body->vel)) {
            let vel_end_world  = math_Vec2f_add(body->pos, math_Vec2f_scale(body->vel, Visualizer_invScale(self)));
            let vel_end_screen = Visualizer_worldToScreen(self, vel_end_world);
            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], vel_end_screen.s[0], vel_end_screen.s[1], Visualizer_color_vel_vec);
        }
    }
}
force_inline void Visualizer_drawBodiesWithAccVec(Visualizer* self) {
    for_slice(self->bodies.items, body) {
        let screen_pos    = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = body->radius * Visualizer_invScale(self);
        Visualizer_drawCircle(self, screen_pos, screen_radius, Visualizer_color_body);

        // Draw acceleration vector
        if (0.0f < math_Vec2f_lenSq(body->acc)) {
            const Vec2f acc_end_world  = math_Vec2f_add(body->pos, math_Vec2f_scale(body->acc, Visualizer_invScale(self)));
            const Vec2i acc_end_screen = Visualizer_worldToScreen(self, acc_end_world);
            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], acc_end_screen.s[0], acc_end_screen.s[1], Visualizer_color_acc_vec);
        }
    }
}
force_inline void Visualizer_drawBodiesWithVelAccVec(Visualizer* self) {
    debug_assert_nonnull(self);
    for_slice(self->bodies.items, body) {
        let screen_pos    = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = body->radius * Visualizer_invScale(self);
        Visualizer_drawCircle(self, screen_pos, screen_radius, Visualizer_color_body);

        // Velocity vector
        if (0.0f < math_Vec2f_lenSq(body->vel)) {
            const Vec2f vel_end_world  = math_Vec2f_add(body->pos, math_Vec2f_scale(body->vel, Visualizer_invScale(self)));
            const Vec2i vel_end_screen = Visualizer_worldToScreen(self, vel_end_world);
            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], vel_end_screen.s[0], vel_end_screen.s[1], Visualizer_color_vel_vec);
        }

        // Acceleration vector
        if (0.0f < math_Vec2f_lenSq(body->acc)) {
            const Vec2f acc_end_world  = math_Vec2f_add(body->pos, math_Vec2f_scale(body->acc, Visualizer_invScale(self)));
            const Vec2i acc_end_screen = Visualizer_worldToScreen(self, acc_end_world);
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
        break;
    }
}

force_inline void Visualizer_drawNode(Visualizer* self, Vec2f min, Vec2f max, Color color) {
    debug_assert_nonnull(self);

    // Convert world min/max to screen coordinates
    var screen_min = Visualizer_worldToScreen(self, min);
    var screen_max = Visualizer_worldToScreen(self, max);

    engine_Canvas_drawRect(
        self->canvas,
        screen_min.s[0],
        screen_min.s[1],
        screen_max.s[0],
        screen_max.s[1],
        color
    );
}
static Err$void Visualizer_renderQuadTree(Visualizer* self) { // NOLINT
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    if (0 < self->nodes.items.len) {
        var depth_range = self->depth_range;
        if (depth_range.max <= depth_range.min) {
            /* init */ var stack = eval(
                let stack = &self->stack;
                ArrList_clearRetainingCap(&stack->pair[0].base);
                ArrList_clearRetainingCap(&stack->pair[1].base);
                stack->len = 0;
                eval_return(stack);
            );
            /* push */ stack->len += eval(
                try(ArrList_append(&stack->nodes.base, meta_refPtr(create$(usize, QuadTree_s_root))));
                try(ArrList_append(&stack->depths.base, meta_refPtr(create$(usize, 0))));
                eval_return 1;
            );

            var min_depth = usize_limit;
            var max_depth = 0ull;
            while (0 < stack->len) {
                if_some(
                    /* pop */ eval(
                        let opt_node  = ArrList_popOrNull(&stack->nodes.base);
                        let opt_depth = ArrList_popOrNull(&stack->depths.base);
                        stack->len--;
                        eval_return make$(
                            Opt$(struct {
                                usize node_idx;
                                usize depth;
                            }),
                            .has_value = opt_node.has_value && opt_depth.has_value,
                            .value     = {
                                    .node_idx = *meta_cast$(usize*, opt_node.value),
                                    .depth    = *meta_cast$(usize*, opt_depth.value),
                            }
                        );
                    ),
                    item
                ) {
                    let node = Sli_at(self->nodes.items, item.node_idx);
                    if (QuadNode_isLeaf(node)) {
                        min_depth = prim_min(min_depth, item.depth);
                        max_depth = prim_max(max_depth, item.depth);
                    } else {
                        for (isize i = 0; i < 4; ++i) {
                            /* push */ stack->len += eval(
                                try(ArrList_append(&stack->nodes.base, meta_refPtr(create$(usize, node->children + i))));
                                try(ArrList_append(&stack->depths.base, meta_refPtr(create$(usize, item.depth + 1))));
                                eval_return 1;
                            );
                        }
                    }
                }
            }
            depth_range = make$(TypeOf(depth_range), .min = min_depth, .max = max_depth);
        }
        let min_depth = depth_range.min;
        let max_depth = depth_range.max;

        /* init */ var stack = eval(
            let stack = &self->stack;
            ArrList_clearRetainingCap(&stack->pair[0].base);
            ArrList_clearRetainingCap(&stack->pair[1].base);
            stack->len = 0;
            eval_return(stack);
        );
        /* push */ stack->len += eval(
            try(ArrList_append(&stack->nodes.base, meta_refPtr(create$(usize, QuadTree_s_root))));
            try(ArrList_append(&stack->depths.base, meta_refPtr(create$(usize, 0))));
            eval_return 1;
        );
        while (0 < stack->len) {
            if_some(
                /* pop */ eval(
                    let opt_node_idx = ArrList_popOrNull(&stack->nodes.base);
                    let opt_depth    = ArrList_popOrNull(&stack->depths.base);
                    stack->len--;
                    eval_return make$(
                        Opt$(struct {
                            usize node_idx;
                            usize depth;
                        }),
                        .has_value = opt_node_idx.has_value && opt_depth.has_value,
                        .value     = {
                                .node_idx = *meta_cast$(usize*, opt_node_idx.value),
                                .depth    = *meta_cast$(usize*, opt_depth.value),
                        }
                    );
                ),
                item
            ) {
                let node = Sli_at(self->nodes.items, item.node_idx);
                if (QuadNode_isBranch(node) && item.depth < max_depth) {
                    for (isize i = 0; i < 4; ++i) {
                        /* push */ stack->len += eval(
                            try(ArrList_append(&stack->nodes.base, meta_refPtr(create$(usize, node->children + i))));
                            try(ArrList_append(&stack->depths.base, meta_refPtr(create$(usize, item.depth + 1))));
                            eval_return 1;
                        );
                    }
                } else if (min_depth <= item.depth) {
                    let quad = node->quad;
                    let half = math_Vec2f_scale(math_Vec2f_scale(math_Vec2f_one, 0.5f), quad.size);
                    let min  = math_Vec2f_sub(quad.center, half);
                    let max  = math_Vec2f_add(quad.center, half);

                    let t = as$(f32, item.depth - depth_range.min + as$(usize, !QuadNode_isEmpty(node))) / as$(f32, depth_range.max - depth_range.min + 1);

                    let start_h = -100.0f;
                    let end_h   = 80.0f;
                    let h       = start_h + (end_h - start_h) * t;
                    let s       = 100.0f;
                    let l       = t * 100.0f;

                    let hsl   = Hsl_from(h, s, l);
                    let color = Hsl_intoColorOpaque(hsl);
                    Visualizer_drawNode(self, min, max, color);
                }
            }
        }
    }

    return_void();
}
