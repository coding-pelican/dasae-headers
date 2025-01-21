#include "State.h"

// Static helper functions
static math_Vec2f State_calculateWorldMouse(State* self, engine_Window* window, Vec2i screen_pos) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(window);

    let client_size = engine_Window_getClientSize(window);

    // Convert to normalized device coordinates
    var mouse = math_Vec2f_from(
        as$(f32, screen_pos.s[0]) * 2.0f / as$(f32, client_size.s[1]),
        as$(f32, screen_pos.s[1]) * 2.0f / as$(f32, client_size.s[1])
    );

    // Apply transformations
    mouse.y -= 1.0f;
    mouse.y *= -1.0f;
    mouse.x -= as$(f32, client_size.s[0]) / as$(f32, client_size.s[1]);

    // Convert to world space
    return math_Vec2f_add(
        math_Vec2f_scale(mouse, self->scale),
        self->pos
    );
}

static void State_drawNode(State* self, const QuadNode* node, usize depth, Color color) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(node);
    unused(depth);

    let half_size = node->quad.size * 0.5f;
    let min       = math_Vec2f_sub(node->quad.center, math_Vec2f_one);
    let max       = math_Vec2f_add(node->quad.center, math_Vec2f_one);

    // Convert world to screen coordinates
    let min_screen = (Vec2i){
        .s = {
            as$(i32, (min.x - half_size) / self->scale),
            as$(i32, (min.y - half_size) / self->scale),
        }
    };

    let max_screen = (Vec2i){
        .s = {
            as$(i32, (max.x + half_size) / self->scale),
            as$(i32, (max.y + half_size) / self->scale),
        }
    };

    engine_Canvas_drawRect(
        self->canvas,
        min_screen.s[0],
        min_screen.s[1],
        max_screen.s[0],
        max_screen.s[1],
        color
    );
}

Err$State State_create(mem_Allocator allocator) {
    reserveReturn(Err$State);
    return_ok((State){
        .pos   = math_Vec2f_zero,
        .scale = 3600.0f,

        .settings_window_open = false,
        .show_bodies          = true,
        .show_quad_tree       = false,

        .depth_range = {
            .min_depth = 0,
            .max_depth = 0,
        },

        .spawn = {
            .body      = null,
            .angle     = null,
            .total     = null,
            .confirmed = null,
        },

        .bodies = typed(ArrList$Body, ArrList_init(typeInfo(Body), allocator)),
        .nodes  = typed(ArrList$QuadNode, ArrList_init(typeInfo(QuadNode), allocator)),

        .allocator = allocator,
        .canvas    = try(engine_Canvas_createWithDefault(800, 600, engine_CanvasType_rgba, Color_transparent)),
    });
}

void State_destroy(State* self) {
    debug_assert_nonnull(self);

    if (self->spawn.body != null) {
        mem_Allocator_free(self->allocator, anyPtr(self->spawn.body));
    }
    if (self->spawn.angle != null) {
        mem_Allocator_free(self->allocator, anyPtr(self->spawn.angle));
    }
    if (self->spawn.total != null) {
        mem_Allocator_free(self->allocator, anyPtr(self->spawn.total));
    }
    if (self->spawn.confirmed != null) {
        mem_Allocator_free(self->allocator, anyPtr(self->spawn.confirmed));
    }

    ArrList_fini(&self->bodies.base);
    ArrList_fini(&self->nodes.base);
    engine_Canvas_destroy(self->canvas);
}

Err$void State_processInput(State* self, engine_Window* window) {
    scope_reserveReturn(Err$void) {
        debug_assert_nonnull(self);
        debug_assert_nonnull(window);

        // Toggle settings window
        if (engine_Key_pressed(engine_KeyCode_e)) {
            self->settings_window_open = !self->settings_window_open;
        }

        // Handle mouse zooming
        let mouse_pos    = engine_Mouse_getPosition();
        let scroll_delta = engine_Mouse_getScrollDelta();

        if (scroll_delta != 0) {
            const f32 steps = 5.0f;
            let       zoom  = expf(-as$(f32, scroll_delta) / steps);

            math_Vec2f world_mouse = State_calculateWorldMouse(self, window, mouse_pos);

            self->pos = math_Vec2f_add(
                self->pos,
                math_Vec2f_scale(world_mouse, 1.0f - zoom)
            );

            self->scale *= zoom;
        }

        // Handle panning with middle mouse button
        if (engine_Mouse_held(engine_MouseButton_middle)) {
            let client_size = engine_Window_getClientSize(window);
            let mouse_delta = engine_Mouse_getDelta();

            self->pos.x -= as$(f32, mouse_delta.s[0]) / as$(f32, client_size.s[1]) * self->scale * 2.0f;
            self->pos.y += as$(f32, mouse_delta.s[1]) / as$(f32, client_size.s[1]) * self->scale * 2.0f;
        }

        // Handle body spawning
        if (engine_Mouse_pressed(engine_MouseButton_right)) {
            math_Vec2f world_mouse = State_calculateWorldMouse(self, window, mouse_pos);

            if (self->spawn.body == null) {
                self->spawn.body = meta_cast$(TypeOf(self->spawn.body), try(mem_Allocator_create(self->allocator, typeInfo(Body))));
                errdefer(mem_Allocator_free(self->allocator, anyPtr(self->spawn.body)));
            }
            *self->spawn.body = Body_new(world_mouse, math_Vec2f_zero, 1.0f, 1.0f);

            if (self->spawn.angle == null) {
                self->spawn.angle = meta_cast$(TypeOf(self->spawn.angle), try(mem_Allocator_create(self->allocator, typeInfo(f32))));
                errdefer(mem_Allocator_free(self->allocator, anyPtr(self->spawn.body)));
            }
            *self->spawn.angle = 0.0f;

            if (self->spawn.total == null) {
                self->spawn.total = meta_cast$(TypeOf(self->spawn.total), try(mem_Allocator_create(self->allocator, typeInfo(f32))));
                errdefer(mem_Allocator_free(self->allocator, anyPtr(self->spawn.body)));
            }
            *self->spawn.total = 0.0f;

        } else if (engine_Mouse_held(engine_MouseButton_right)) {
            if (self->spawn.body != null) {
                math_Vec2f world_mouse = State_calculateWorldMouse(self, window, mouse_pos);

                let d = math_Vec2f_sub(world_mouse, self->spawn.body->pos);

                if (self->spawn.angle != null) {
                    let angle2    = atan2f(d.y, d.x);
                    let a         = angle2 - *self->spawn.angle;
                    let a_wrapped = fmodf(a + math_f32_pi, math_f32_tau) - math_f32_pi;
                    *self->spawn.total -= a_wrapped;
                    *self->spawn.angle = angle2;

                    self->spawn.body->mass   = exp2f(*self->spawn.total / math_f32_tau);
                    self->spawn.body->radius = cbrtf(self->spawn.body->mass);
                } else {
                    *self->spawn.angle = atan2f(d.y, d.x);
                }

                self->spawn.body->vel = d;
            }

        } else if (engine_Mouse_released(engine_MouseButton_right)) {
            if (self->spawn.body != null) {
                if (self->spawn.confirmed == null) {
                    self->spawn.confirmed = malloc(sizeof(Body));
                }
                *self->spawn.confirmed = *self->spawn.body;

                free(self->spawn.body);
                self->spawn.body = null;

                free(self->spawn.angle);
                self->spawn.angle = null;

                free(self->spawn.total);
                self->spawn.total = null;
            }
        }

        return_ok({});
    }
    scope_returnReserved;
}

Err$void State_update(State* self) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    // Handle spawned body confirmation
    if (self->spawn.confirmed != null) {
        try(ArrList_append(&self->bodies.base, meta_refPtr(self->spawn.confirmed)));

        mem_Allocator_free(self->allocator, anyPtr(self->spawn.confirmed));
        self->spawn.confirmed = null;
    }

    return_ok({});
}

Err$void State_render(State* self) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    // Clear canvas
    engine_Canvas_clearDefault(self->canvas);

    // Draw bodies
    for_slice(self->bodies.items, body) {
        let screen_pos = (Vec2i){
            .s = {
                as$(i32, body->pos.x / self->scale),
                as$(i32, body->pos.y / self->scale) }
        };

        let screen_radius = as$(i32, body->radius / self->scale);

        engine_Canvas_fillCircle(self->canvas, screen_pos.s[0], screen_pos.s[1], screen_radius, Color_white);

        // Draw velocity vector
        if (math_Vec2f_lenSq(body->vel) > 0.0f) {
            let vel_end        = math_Vec2f_add(body->pos, body->vel);
            let screen_vel_end = (Vec2i){
                .s = {
                    as$(i32, vel_end.x / self->scale),
                    as$(i32, vel_end.y / self->scale) }
            };

            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], screen_vel_end.s[0], screen_vel_end.s[1], Color_white);
        }
    }

    // Draw spawn preview
    if (self->spawn.body != null) {
        let screen_pos = (Vec2i){
            .s = {
                as$(i32, self->spawn.body->pos.x / self->scale),
                as$(i32, self->spawn.body->pos.y / self->scale) }
        };

        let screen_radius = as$(i32, self->spawn.body->radius / self->scale);

        engine_Canvas_fillCircle(self->canvas, screen_pos.s[0], screen_pos.s[1], screen_radius, Color_white);

        if (math_Vec2f_lenSq(self->spawn.body->vel) > 0.0f) {
            let vel_end        = math_Vec2f_add(self->spawn.body->pos, self->spawn.body->vel);
            let screen_vel_end = (Vec2i){
                .s = {
                    as$(i32, vel_end.x / self->scale),
                    as$(i32, vel_end.y / self->scale) }
            };

            engine_Canvas_drawLine(self->canvas, screen_pos.s[0], screen_pos.s[1], screen_vel_end.s[0], screen_vel_end.s[1], Color_white);
        }
    }

    if (self->show_quad_tree) {
        // If depth range needs to be recalculated
        if (self->depth_range.min_depth >= self->depth_range.max_depth) {
            var min_depth = usize_limit_max;
            var max_depth = 0;

            usize stack[256]  = cleared();
            u8    depths[256] = cleared();
            usize stack_size  = 1;

            stack[0]  = QuadTree_s_root;
            depths[0] = 0;

            while (stack_size > 0) {
                let node_idx = stack[--stack_size];
                let depth    = depths[stack_size];

                let node = Sli_at(self->nodes.items, node_idx);

                if (QuadNode_isLeaf(node)) {
                    min_depth = prim_min(min_depth, depth);
                    max_depth = prim_max(max_depth, depth);
                } else {
                    // Push children onto stack
                    for (usize i = 0; i < 4; ++i) {
                        let child_idx      = node->children + i;
                        stack[stack_size]  = child_idx;
                        depths[stack_size] = depth + 1;
                        stack_size++;
                    }
                }
            }

            self->depth_range.min_depth = min_depth;
            self->depth_range.max_depth = max_depth;
        }

        // Draw quadtree nodes
        usize stack[256]  = cleared();
        u8    depths[256] = cleared();
        usize stack_size  = 1;

        stack[0]  = QuadTree_s_root;
        depths[0] = 0;

        while (stack_size > 0) {
            let node_idx = stack[--stack_size];
            let depth    = depths[stack_size];
            let node     = Sli_at(self->nodes.items, node_idx);

            bool is_leaf_or_max_depth = QuadNode_isLeaf(node) || depth >= self->depth_range.max_depth;

            if ((depth >= self->depth_range.min_depth) && is_leaf_or_max_depth) {
                // Calculate node color based on depth and occupancy
                let t = as$(f32, depth - self->depth_range.min_depth + !QuadNode_isEmpty(node)) / as$(f32, self->depth_range.max_depth - self->depth_range.min_depth + 1);

                let h = fminf(-100.0f + 180.0f * t, 80.0f);
                let s = 100.0f;
                let l = t * 100.0f;

                let hsl   = Hsl_from(h, s, l);
                let color = Hsl_intoColorOpaque(hsl);

                State_drawNode(self, node, depth, color);
            }

            if (!is_leaf_or_max_depth) {
                // Push children onto stack
                for (usize i = 0; i < 4; ++i) {
                    let child_idx      = node->children + i;
                    stack[stack_size]  = child_idx;
                    depths[stack_size] = depth + 1;
                    stack_size++;
                }
            }
        }
    }

    return_void();
}
