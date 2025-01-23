#include "Visualizer.h"
#include "QuadTree.h"
#include "dh/ArrList.h"
#include "dh/log.h"
#include "engine/color.h"

#define color_body    (literal_Color_fromOpaque(192, 192, 192))
#define color_vel_vec (literal_Color_fromOpaque(0, 0, 192))
#define color_acc_vec (literal_Color_fromOpaque(192, 0, 0))

// Static helper functions
force_inline f32  Visualizer_invScale(Visualizer* self) { return 1.0f / self->scale; }
static Vec2f      Visualizer_screenToWorld(Visualizer* self, engine_Window* window, Vec2i screen_pos);
static Vec2i      Visualizer_worldToScreen(Visualizer* self, Vec2f world_pos);
static math_Vec2f Visualizer_calculateWorldMouse(Visualizer* self, engine_Window* window, Vec2i screen_pos);
static void       Visualizer_drawNode(Visualizer* self, Vec2f min, Vec2f max, Color color);

Err$Visualizer Visualizer_create(mem_Allocator allocator, engine_Canvas* canvas) {
    reserveReturn(Err$Visualizer);
    return_ok((Visualizer){
        .pos   = math_Vec2f_zero,
        .scale = (f32)canvas->width / (f32)canvas->height,
        // .scale = 3600,

        .shows_bodies         = true,
        .shows_bodies_vel_vec = false,
        .shows_bodies_acc_vec = false,
        .shows_quad_tree      = false,

        .body_color = color_body,

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

force_inline void VisualizerInput_toggleShowingBodies(Visualizer* self) {
    if (engine_Key_pressed(engine_KeyCode_n1)) {
        log_debug("pressed '1' to toggle bodies showing\n");
        self->shows_bodies = !self->shows_bodies;
    }
}

force_inline void VisualizerInput_toggleVisualizationBodiesVelVec(Visualizer* self) {
    if (engine_Key_pressed(engine_KeyCode_n2)) {
        log_debug("pressed '2' to toggle bodies velocity vector visualization\n");
        self->shows_bodies_vel_vec = !self->shows_bodies_vel_vec;
    }
}

force_inline void VisualizerInput_toggleVisualizationBodiesAccVec(Visualizer* self) {
    if (engine_Key_pressed(engine_KeyCode_n3)) {
        log_debug("pressed '3' to toggle bodies acceleration vector visualization\n");
        self->shows_bodies_acc_vec = !self->shows_bodies_acc_vec;
    }
}

force_inline void VisualizerInput_toggleVisualizationQuadTree(Visualizer* self) {
    if (engine_Key_pressed(engine_KeyCode_q)) {
        log_debug("pressed 'q' to toggle quad-tree visualization\n");
        self->shows_quad_tree = !self->shows_quad_tree;
    }
}

Err$void Visualizer_processInput(Visualizer* self, engine_Window* window) {
    scope_reserveReturn(Err$void) {
        debug_assert_nonnull(self);
        debug_assert_nonnull(window);

        VisualizerInput_toggleShowingBodies(self);
        VisualizerInput_toggleVisualizationBodiesVelVec(self);
        VisualizerInput_toggleVisualizationBodiesAccVec(self);
        VisualizerInput_toggleVisualizationQuadTree(self);

        // Handle zooming
        let scroll_delta = engine_Mouse_getScrollDelta();
        if (scroll_delta != 0) {
            const f32 steps = 5.0f;
            let       zoom  = expf(-as$(f32, scroll_delta) / steps);

            let mouse_pos   = engine_Mouse_getPosition();
            let world_mouse = Visualizer_calculateWorldMouse(self, window, mouse_pos);

            self->pos = math_Vec2f_add(
                self->pos,
                math_Vec2f_scale(world_mouse, 1.0f - zoom)
            );

            self->scale *= zoom;
        }

        // Handle panning
        if (engine_Mouse_held(engine_MouseButton_middle)) {
            let mouse_delta = engine_Mouse_getDelta();
            let client_size = engine_Window_getClientSize(window);

            self->pos.x -= as$(f32, mouse_delta.s[0]) / as$(f32, client_size.s[0]) * self->scale * 2.0f;
            self->pos.y -= as$(f32, mouse_delta.s[1]) / as$(f32, client_size.s[1]) * self->scale * 2.0f;
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

        return_ok({});
    }
    scope_returnReserved;
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

force_inline void Visualizer_renderBodies(Visualizer* self) {
    for_slice(self->bodies.items, body) {
        let screen_pos    = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = as$(i32, body->radius * Visualizer_invScale(self));
        engine_Canvas_fillCircle(
            self->canvas,
            screen_pos.s[0],
            screen_pos.s[1],
            screen_radius,
            self->body_color
        );
    }
}

force_inline void Visualizer_renderBodiesWithVelVec(Visualizer* self) {
    for_slice(self->bodies.items, body) {
        let screen_pos    = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = as$(i32, body->radius * Visualizer_invScale(self));
        engine_Canvas_fillCircle(
            self->canvas,
            screen_pos.s[0],
            screen_pos.s[1],
            screen_radius,
            self->body_color
        );

        // Draw velocity vector
        if (0.0f < math_Vec2f_lenSq(body->vel)) {
            let vel_end_world  = math_Vec2f_add(body->pos, body->vel);
            let vel_end_screen = Visualizer_worldToScreen(self, vel_end_world);
            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], vel_end_screen.s[0], vel_end_screen.s[1], color_vel_vec);
        }
    }
}

force_inline void Visualizer_renderBodiesWithAccVec(Visualizer* self) {
    for_slice(self->bodies.items, body) {
        let screen_pos    = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = as$(i32, body->radius * Visualizer_invScale(self));
        engine_Canvas_fillCircle(self->canvas, screen_pos.s[0], screen_pos.s[1], screen_radius, self->body_color);

        // Draw acceleration vector
        if (0.0f < math_Vec2f_lenSq(body->acc)) {
            const Vec2f acc_end_world  = math_Vec2f_add(body->pos, body->acc);
            const Vec2i acc_end_screen = Visualizer_worldToScreen(self, acc_end_world);
            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], acc_end_screen.s[0], acc_end_screen.s[1], color_acc_vec);
        }
    }
}

force_inline void Visualizer_renderBodiesWithVelAccVec(Visualizer* self) {
    debug_assert_nonnull(self);
    for_slice(self->bodies.items, body) {
        let screen_pos    = Visualizer_worldToScreen(self, body->pos);
        let screen_radius = as$(i32, body->radius * Visualizer_invScale(self));
        engine_Canvas_fillCircle(self->canvas, screen_pos.s[0], screen_pos.s[1], screen_radius, self->body_color);

        // Velocity vector
        if (0.0f < math_Vec2f_lenSq(body->vel)) {
            const Vec2f vel_end_world  = math_Vec2f_add(body->pos, body->vel);
            const Vec2i vel_end_screen = Visualizer_worldToScreen(self, vel_end_world);
            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], vel_end_screen.s[0], vel_end_screen.s[1], color_vel_vec);
        }

        // Acceleration vector
        if (0.0f < math_Vec2f_lenSq(body->acc)) {
            const Vec2f acc_end_world  = math_Vec2f_add(body->pos, body->acc);
            const Vec2i acc_end_screen = Visualizer_worldToScreen(self, acc_end_world);
            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], acc_end_screen.s[0], acc_end_screen.s[1], color_acc_vec);
        }
    }
}

force_inline Err$void Visualizer_renderQuadTree(Visualizer* self) { // NOLINT
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

Err$void Visualizer_render(Visualizer* self) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    // Clear canvas
    engine_Canvas_clearDefault(self->canvas);

    // Draw bodies
    if (self->shows_bodies) {
        let render_bodies_type = self->shows_bodies_vel_vec | self->shows_bodies_acc_vec << 1;
        switch (render_bodies_type) {
        case (0 | 0 << 1):
            Visualizer_renderBodies(self);
            break;
        case (1 | 0 << 1):
            Visualizer_renderBodiesWithVelVec(self);
            break;
        case (0 | 1 << 1):
            Visualizer_renderBodiesWithAccVec(self);
            break;
        case (1 | 1 << 1):
            Visualizer_renderBodiesWithVelAccVec(self);
            break;
        default:
            claim_unreachable;
            break;
        }
    }

    // Draw quad-tree
    if (self->shows_quad_tree) {
        try(Visualizer_renderQuadTree(self));
    }

    return_void();
}

static math_Vec2f Visualizer_screenToWorld(Visualizer* self, engine_Window* window, Vec2i screen_pos) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(window);

    let client_size = engine_Window_getClientSize(window);
    if (client_size.s[0] <= 0 || client_size.s[1] <= 0) { return math_Vec2f_zero; }

    var ndc_x = (2.0f * (f32)screen_pos.s[0] / (f32)client_size.s[0]) - 1.0f;
    let ndc_y = 1.0f - (2.0f * (f32)screen_pos.s[1] / (f32)client_size.s[1]);

    // Adjust for aspect ratio to maintain square aspect
    let aspect_ratio = (f32)client_size.s[0] / (f32)client_size.s[1];
    ndc_x *= aspect_ratio;

    let ndc = math_Vec2f_from(ndc_x, ndc_y);
    return math_Vec2f_add(math_Vec2f_scale(ndc, self->scale), self->pos);
}

static math_Vec2f Visualizer_calculateWorldMouse(Visualizer* self, engine_Window* window, Vec2i screen_pos) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(window);
    return Visualizer_screenToWorld(self, window, screen_pos);
}

static Vec2i Visualizer_worldToScreen(Visualizer* self, Vec2f world_pos) {
    debug_assert_nonnull(self);

    var translated = math_Vec2f_sub(world_pos, self->pos);
    let inv_scale  = Visualizer_invScale(self);

    // Adjust for aspect ratio
    let aspect_ratio = (f32)self->canvas->width / (f32)self->canvas->height;
    translated.x *= inv_scale / aspect_ratio;
    translated.y *= inv_scale;

    let canvas_half_w = as$(f32, self->canvas->width) / 2.0f;
    let canvas_half_h = as$(f32, self->canvas->height) / 2.0f;
    let screen_x      = as$(i32, translated.x * canvas_half_w + canvas_half_w);
    let screen_y      = as$(i32, translated.y * canvas_half_h + canvas_half_h);

    return math_Vec2i_from(screen_x, screen_y);
}

static void Visualizer_drawNode(Visualizer* self, Vec2f min, Vec2f max, Color color) {
    debug_assert_nonnull(self);

    // Convert world min/max to screen coordinates
    let screen_min = Visualizer_worldToScreen(self, min);
    let screen_max = Visualizer_worldToScreen(self, max);

    engine_Canvas_drawRect(
        self->canvas,
        screen_min.s[0],
        screen_min.s[1],
        screen_max.s[0],
        screen_max.s[1],
        color
    );
}
