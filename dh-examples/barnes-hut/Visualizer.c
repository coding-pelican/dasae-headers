#include "Visualizer.h"
#include "dh/log.h"
#include "engine/color.h"

// Static helper functions
static math_Vec2f Visualizer_calculateWorldMouse(Visualizer* self, engine_Window* window, Vec2i screen_pos);
static void       Visualizer_drawNode(Visualizer* self, const QuadNode* node, usize depth, Color color);

Err$Visualizer Visualizer_create(mem_Allocator allocator, engine_Canvas* canvas) {
    reserveReturn(Err$Visualizer);
    return_ok((Visualizer){
        .pos   = math_Vec2f_zero,
        .scale = (f32)canvas->width / (f32)canvas->height,

        .shows_bodies         = true,
        .shows_bodies_vel_vec = false,
        .shows_bodies_acc_vec = false,
        .shows_quad_tree      = false,

        .body_color = Color_fromOpaque(192, 192, 192),

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
        .canvas    = canvas,
    });
}

void Visualizer_destroy(Visualizer* self) {
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
}

force_inline void Input_toggleShowingBodies(Visualizer* self) {
    if (engine_Key_pressed(engine_KeyCode_n1)) {
        log_debug("pressed '1' to toggle bodies showing\n");
        self->shows_bodies = !self->shows_bodies;
    }
}

force_inline void Input_toggleVisualizationBodiesVelVec(Visualizer* self) {
    if (engine_Key_pressed(engine_KeyCode_n2)) {
        log_debug("pressed '2' to toggle bodies velocity vector visualization\n");
        self->shows_bodies_vel_vec = !self->shows_bodies_vel_vec;
    }
}

force_inline void Input_toggleVisualizationBodiesAccVec(Visualizer* self) {
    if (engine_Key_pressed(engine_KeyCode_n3)) {
        log_debug("pressed '3' to toggle bodies acceleration vector visualization\n");
        self->shows_bodies_acc_vec = !self->shows_bodies_acc_vec;
    }
}

force_inline void Input_toggleVisualizationQuadTree(Visualizer* self) {
    if (engine_Key_pressed(engine_KeyCode_n0)) {
        log_debug("pressed '0' to toggle quad-tree visualization\n");
        self->shows_quad_tree = !self->shows_quad_tree;
    }
}

Err$void Visualizer_processInput(Visualizer* self, engine_Window* window) {
    scope_reserveReturn(Err$void) {
        debug_assert_nonnull(self);
        debug_assert_nonnull(window);

        Input_toggleShowingBodies(self);
        Input_toggleVisualizationBodiesVelVec(self);
        Input_toggleVisualizationBodiesAccVec(self);
        Input_toggleVisualizationQuadTree(self);

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
            log_debug("right mouse button held");
            if (self->spawn.body != null) {
                let mouse_pos   = engine_Mouse_getPosition();
                let world_mouse = Visualizer_calculateWorldMouse(self, window, mouse_pos);

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
            log_debug("right mouse button released");
            if (self->spawn.body != null) {
                if (self->spawn.confirmed == null) {
                    self->spawn.confirmed = meta_cast$(TypeOf(self->spawn.confirmed), try(mem_Allocator_create(self->allocator, typeInfo(Body))));
                    errdefer(mem_Allocator_free(self->allocator, anyPtr(self->spawn.confirmed)));
                }
                *self->spawn.confirmed = *self->spawn.body;

                if (self->spawn.body != null) {
                    mem_Allocator_free(self->allocator, anyPtr(self->spawn.body));
                    self->spawn.body = null;
                }
                if (self->spawn.angle != null) {
                    mem_Allocator_free(self->allocator, anyPtr(self->spawn.angle));
                    self->spawn.angle = null;
                }
                if (self->spawn.total != null) {
                    mem_Allocator_free(self->allocator, anyPtr(self->spawn.total));
                    self->spawn.total = null;
                }
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
    if (self->spawn.confirmed != null) {
        try(ArrList_append(&self->bodies.base, meta_refPtr(self->spawn.confirmed)));

        mem_Allocator_free(self->allocator, anyPtr(self->spawn.confirmed));
        self->spawn.confirmed = null;
    }

    return_ok({});
}

Err$void Visualizer_render(Visualizer* self) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    // Clear canvas
    engine_Canvas_clearDefault(self->canvas);

    // Draw bodies
    if (!self->shows_bodies_vel_vec) {
        for_slice(self->bodies.items, body) {
            let screen_pos = math_Vec2f_scale(
                math_Vec2f_sub(body->pos, self->pos),
                1.0f / self->scale
            );

            engine_Canvas_fillCircle(
                self->canvas,
                as$(i32, screen_pos.x * as$(f32, self->canvas->width) / 2.0f + as$(i32, self->canvas->width) / 2.0f),
                as$(i32, screen_pos.y * as$(f32, self->canvas->height) / 2.0f + as$(i32, self->canvas->height) / 2.0f),
                as$(i32, body->radius / self->scale),
                self->body_color
            );
        }
        // In the vector bodies rendering block (else clause):
    } else {
        for_slice(self->bodies.items, body) {
            // Convert world coordinates to screen coordinates correctly
            let screen_pos_ndc = math_Vec2f_scale(math_Vec2f_sub(body->pos, self->pos), 1.0f / self->scale);
            i32 screen_x       = as$(i32, screen_pos_ndc.x * (as$(f32, self->canvas->width) / 2.0f) + (as$(f32, self->canvas->width) / 2.0f));
            i32 screen_y       = as$(i32, screen_pos_ndc.y * (as$(f32, self->canvas->height) / 2.0f) + (as$(f32, self->canvas->height) / 2.0f));
            let screen_radius  = as$(i32, body->radius / self->scale);

            engine_Canvas_fillCircle(self->canvas, screen_x, screen_y, screen_radius, self->body_color);

            // Draw velocity vector
            if (0.0f < math_Vec2f_lenSq(body->vel)) {
                let vel_end     = math_Vec2f_add(body->pos, body->vel);
                let vel_end_ndc = math_Vec2f_scale(math_Vec2f_sub(vel_end, self->pos), 1.0f / self->scale);
                i32 vel_end_x   = as$(i32, vel_end_ndc.x * (as$(f32, self->canvas->width) / 2.0f) + (as$(f32, self->canvas->width) / 2.0f));
                i32 vel_end_y   = as$(i32, vel_end_ndc.y * (as$(f32, self->canvas->height) / 2.0f) + (as$(f32, self->canvas->height) / 2.0f));

                engine_Canvas_drawLine(self->canvas, screen_x, screen_y, vel_end_x, vel_end_y, self->body_color);
            }
        }
    }

    // In the spawn preview section:
    if (self->spawn.body != null) {
        // Convert world coordinates to screen coordinates correctly
        let spawn_pos_ndc  = math_Vec2f_scale(math_Vec2f_sub(self->spawn.body->pos, self->pos), 1.0f / self->scale);
        i32 spawn_screen_x = as$(i32, spawn_pos_ndc.x * (as$(f32, self->canvas->width) / 2.0f) + (as$(f32, self->canvas->width) / 2.0f));
        i32 spawn_screen_y = as$(i32, spawn_pos_ndc.y * (as$(f32, self->canvas->height) / 2.0f) + (as$(f32, self->canvas->height) / 2.0f));
        let screen_radius  = as$(i32, self->spawn.body->radius / self->scale);

        engine_Canvas_fillCircle(self->canvas, spawn_screen_x, spawn_screen_y, screen_radius, self->body_color);

        // Draw velocity vector correctly
        if (math_Vec2f_lenSq(self->spawn.body->vel) > 0.0f) {
            let vel_end     = math_Vec2f_add(self->spawn.body->pos, self->spawn.body->vel);
            let vel_end_ndc = math_Vec2f_scale(math_Vec2f_sub(vel_end, self->pos), 1.0f / self->scale);
            i32 vel_end_x   = as$(i32, vel_end_ndc.x * (as$(f32, self->canvas->width) / 2.0f) + (as$(f32, self->canvas->width) / 2.0f));
            i32 vel_end_y   = as$(i32, vel_end_ndc.y * (as$(f32, self->canvas->height) / 2.0f) + (as$(f32, self->canvas->height) / 2.0f));

            engine_Canvas_drawLine(self->canvas, spawn_screen_x, spawn_screen_y, vel_end_x, vel_end_y, self->body_color);
        }
    }

    // Draw quad tree
    if (self->shows_quad_tree) {
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

                Visualizer_drawNode(self, node, depth, color);
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

static math_Vec2f Visualizer_calculateWorldMouse(Visualizer* self, engine_Window* window, Vec2i screen_pos) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(window);

    let client_size = engine_Window_getClientSize(window);

    // Convert to normalized device coordinates
    var mouse = math_Vec2f_from(
        as$(f32, screen_pos.s[0]) * 2.0f / as$(f32, client_size.s[0]) - 1.0f,
        1.0f - as$(f32, screen_pos.s[1]) * 2.0f / as$(f32, client_size.s[1])
    );

    // Convert to world space
    return math_Vec2f_add(
        math_Vec2f_scale(mouse, self->scale),
        self->pos
    );
}

static void Visualizer_drawNode(Visualizer* self, const QuadNode* node, usize depth, Color color) {
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
