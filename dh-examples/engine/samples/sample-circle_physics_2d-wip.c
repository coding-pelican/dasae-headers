#include "dh/main.h"
#include "dh/core.h"
#include "dh/log.h"

#include "dh/meta.h"
#include "dh/mem/Allocator.h"
#include "dh/heap/Page.h"
#include "dh/ArrList.h"

#include "dh/time/Duration.h"
#include "dh/time/Instant.h"

#include "dh/Random.h"

#include "engine.h"

#define window_res_width  (160 * 2)
#define window_res_height (100 * 2)
static const f32 window_res_x     = window_res_width;
static const f32 window_res_y     = window_res_height;
static const f32 window_res_scale = 1.0f;

// Circular collision and physics structures
typedef struct Circ2f {
    Vec2f center;
    f32   radius;
} Circ2f;
$inline_always bool Circ2f_intersects(Circ2f a, Circ2f b) {
    let diff      = math_Vec2f_sub(b.center, a.center);
    let dist_sq   = math_Vec2f_lenSq(diff);
    let sum_radii = a.radius + b.radius;
    return dist_sq <= (sum_radii * sum_radii);
}
$inline_always bool Circ2f_containsPoint(Circ2f c, Vec2f point) {
    let diff    = math_Vec2f_sub(point, c.center);
    let dist_sq = math_Vec2f_lenSq(diff);
    return dist_sq <= (c.radius * c.radius);
}

// Ball entity
typedef struct Ball {
    Circ2f transform;
    Vec2f  vel;
    Vec2f  acc;
    f32    mass;
} Ball;
use_Sli$(Ball);
use_ArrList$(Ball);

static const f32 Ball_mass_scaler_by_radius = 10.0f;
static const f32 Ball_velocity_tolerance    = 0.01f;
static const f32 Ball_drag_coefficient      = 0.8f;

static Ball Ball_init(f32 x, f32 y, f32 radius) {
    return (Ball){
        .transform = {
            .center = { .x = x, .y = y },
            .radius = radius,
        },
        .vel  = math_Vec2f_zero,
        .acc  = math_Vec2f_zero,
        .mass = radius * Ball_mass_scaler_by_radius,
    };
}

// Ball collision record
typedef struct BallCollisionPair {
    usize collider_index;
    usize collidee_index;
} BallCollisionPair;
use_Sli$(BallCollisionPair);
use_ArrList$(BallCollisionPair);

// Ball system manager
typedef struct BallManager {
    ArrList$Ball  balls;
    Ball*         selected_ball;
    mem_Allocator allocator;
} BallManager;
use_Err$(BallManager);

static Err$BallManager BallManager_init(mem_Allocator allocator) {
    scope_reserveReturn(Err$BallManager) {
        return_ok((BallManager){
            .balls         = type$(ArrList$Ball, try(ArrList_initCap(typeInfo$(Ball), allocator, 32))),
            .selected_ball = null,
            .allocator     = allocator,
        });
    }
    scope_returnReserved;
}

static void BallManager_fini(BallManager* self) {
    ArrList_fini(&self->balls.base);
}

static Err$void BallManager_replaceRandomly(BallManager* self, u32 count) {
    reserveReturn(Err$void);

    ArrList_clearRetainingCap(&self->balls.base);
    for (u32 i = 0; i < count; ++i) {
        try(ArrList_append(
            &self->balls.base,
            meta_refPtr(&(Ball){
                .transform = {
                    .center = {
                        .x = Random_range_f64(-window_res_x / 2, window_res_x / 2),
                        .y = Random_range_f64(-window_res_y / 2, window_res_y / 2),
                    },
                    .radius = Random_range_f64(2.0, 7.0),
                },
                .vel  = math_Vec2f_zero,
                .acc  = math_Vec2f_zero,
                .mass = 0.0f, // Will be set based on radius
            })
                        ));
    }

    // Set masses based on radii
    for_slice (self->balls.items, ball) {
        ball->mass = ball->transform.radius * Ball_mass_scaler_by_radius;
    }

    return_void();
}

static Err$void BallManager_resolveCollisions(BallManager* self) {
    reserveReturn(Err$void);

    // Quick n^2 collision check between all balls
    for (usize i = 0; i < self->balls.items.len; ++i) {
        let ball = &self->balls.items.ptr[i];

        for (usize j = i + 1; j < self->balls.items.len; ++j) {
            let target = &self->balls.items.ptr[j];

            if (!Circ2f_intersects(ball->transform, target->transform)) {
                continue;
            }

            // Calculate collision response
            Vec2f diff = math_Vec2f_sub(target->transform.center, ball->transform.center);
            f32   dist = math_Vec2f_len(diff);

            // Prevent division by zero
            if (dist <= 0.0001f) {
                continue;
            }

            // Calculate overlap
            f32 overlap = 0.5f * (dist - ball->transform.radius - target->transform.radius);

            // Separate balls
            Vec2f sep_dir = math_Vec2f_scale(diff, 1.0f / dist);
            Vec2f sep     = math_Vec2f_scale(sep_dir, overlap);

            math_Vec2f_subTo(&ball->transform.center, sep);
            math_Vec2f_addTo(&target->transform.center, sep);

            // Calculate collision impulse
            Vec2f normal  = sep_dir;
            Vec2f tangent = { .x = -normal.y, .y = normal.x };

            // Project velocities
            f32 dpTanBall    = math_Vec2f_dot(ball->vel, tangent);
            f32 dpTanTarget  = math_Vec2f_dot(target->vel, tangent);
            f32 dpNormBall   = math_Vec2f_dot(ball->vel, normal);
            f32 dpNormTarget = math_Vec2f_dot(target->vel, normal);

            // Conservation of momentum
            f32 m1              = ball->mass;
            f32 m2              = target->mass;
            f32 newDpNormBall   = (dpNormBall * (m1 - m2) + 2.0f * m2 * dpNormTarget) / (m1 + m2);
            f32 newDpNormTarget = (dpNormTarget * (m2 - m1) + 2.0f * m1 * dpNormBall) / (m1 + m2);

            // Apply new velocities
            ball->vel = math_Vec2f_add(
                math_Vec2f_scale(tangent, dpTanBall),
                math_Vec2f_scale(normal, newDpNormBall)
            );
            target->vel = math_Vec2f_add(
                math_Vec2f_scale(tangent, dpTanTarget),
                math_Vec2f_scale(normal, newDpNormTarget)
            );
        }
    }

    return_void();
}

static void BallManager_update(BallManager* self, f32 dt) {
    // Update physics
    for_slice (self->balls.items, ball) {
        // Apply drag
        ball->acc = math_Vec2f_scale(math_Vec2f_neg(ball->vel), Ball_drag_coefficient);

        // Update velocity and position
        math_Vec2f_addTo(&ball->vel, math_Vec2f_scale(ball->acc, dt));
        math_Vec2f_addTo(&ball->transform.center, math_Vec2f_scale(ball->vel, dt));

        // Screen wrapping
        let wrap_min = math_Vec2f_from(-window_res_x / 2, -window_res_y / 2);
        let wrap_max = math_Vec2f_from(window_res_x / 2, window_res_y / 2);
        let size     = math_Vec2f_sub(wrap_max, wrap_min);

        while (ball->transform.center.x < wrap_min.x) {
            ball->transform.center.x += size.x;
        }
        while (ball->transform.center.x > wrap_max.x) {
            ball->transform.center.x -= size.x;
        }
        while (ball->transform.center.y < wrap_min.y) {
            ball->transform.center.y += size.y;
        }
        while (ball->transform.center.y > wrap_max.y) {
            ball->transform.center.y -= size.y;
        }

        // Damping for very small velocities
        if (math_Vec2f_lenSq(ball->vel) < Ball_velocity_tolerance) {
            ball->vel = math_Vec2f_zero;
        }
    }
}

// Convert world space to screen space for a ball
$inline_always Vec2f worldToScreen(Vec2f pos, engine_Canvas* canvas) {
    let scale  = window_res_scale;
    let center = (Vec2f){
        .x = as$(f32, canvas->width) * 0.5f,
        .y = as$(f32, canvas->height) * 0.5f
    };
    var screen_pos = math_Vec2f_scale(pos, scale);
    screen_pos.y   = -screen_pos.y;
    return math_Vec2f_add(screen_pos, center);
};
static void BallManager_render(BallManager* self, engine_Canvas* canvas) {
    // Draw all balls
    for_slice (self->balls.items, ball) {
        let pos    = worldToScreen(ball->transform.center, canvas);
        let radius = ball->transform.radius * window_res_scale;
        engine_Canvas_drawCircle(canvas, as$(i32, pos.x), as$(i32, pos.y), as$(i32, radius), Color_white);
    }

    // Draw selected ball highlight and cue line
    if (self->selected_ball) {
        let pos       = worldToScreen(self->selected_ball->transform.center, canvas);
        let mouse_pos = math_Vec_as$(Vec2f, engine_Mouse_getPosition());
        engine_Canvas_drawLine(canvas, as$(i32, pos.x), as$(i32, pos.y), as$(i32, mouse_pos.x), as$(i32, mouse_pos.y), Color_blue);
    }
}

$inline_always Vec2f screenToWorld(Vec2i screen_pos) {
    var pos = (Vec2f){
        .x = as$(f32, screen_pos.x),
        .y = as$(f32, screen_pos.y)
    };
    let scale  = 1.0f / window_res_scale;
    let center = (Vec2f){
        .x = window_res_x * 0.5f, // Hardcoded canvas size for now
        .y = window_res_y * 0.5f
    };
    pos   = math_Vec2f_sub(pos, center);
    pos.y = -pos.y;
    return math_Vec2f_scale(pos, scale);
}
static void BallManager_processInput(BallManager* self) {
    // Ball selection
    if (engine_Mouse_pressed(engine_MouseButton_left) || engine_Mouse_pressed(engine_MouseButton_right)) {
        self->selected_ball = null;
        let world_pos       = screenToWorld(engine_Mouse_getPosition());
        for_slice (self->balls.items, ball) {
            if (Circ2f_containsPoint(ball->transform, world_pos)) {
                self->selected_ball = ball;
                break;
            }
        }
    }

    // Ball dragging
    if (engine_Mouse_held(engine_MouseButton_left)) {
        if (self->selected_ball) {
            self->selected_ball->transform.center = screenToWorld(engine_Mouse_getPosition());
            self->selected_ball->vel              = math_Vec2f_zero;
        }
    }

    // Ball launching
    if (engine_Mouse_released(engine_MouseButton_right)) {
        if (self->selected_ball) {
            let world_pos  = screenToWorld(engine_Mouse_getPosition());
            let launch_dir = math_Vec2f_sub(
                self->selected_ball->transform.center,
                world_pos
            );
            self->selected_ball->vel = math_Vec2f_scale(launch_dir, 5.0f);
            self->selected_ball      = null;
        }
    }
}

Err$void dh_main(int argc, const char* argv[]) {
    $unused(argc), $unused(argv);
    scope_reserveReturn(Err$void) {
        // Initialize logging
        scope_if(let debug_file = fopen("sample-circle_physics_2d-debug.log", "w"), debug_file) {
            log_initWithFile(debug_file);
            log_setLevel(log_Level_debug);
            log_showTimestamp(true);
            log_showLevel(true);
            log_showLocation(false);
            log_showFunction(true);
        }
        defer(log_fini());

        // Initialize platform
        let window = try(engine_Window_create(
            &(engine_PlatformParams){
                .backend_type  = engine_RenderBackendType_vt100,
                .window_title  = "Circle Physics 2D",
                .width         = window_res_width,
                .height        = window_res_height,
                .default_color = Color_black,
            }
        ));
        defer(engine_Window_destroy(window));

        let canvas = try(engine_Canvas_create(window_res_width, window_res_height, engine_CanvasType_rgba));
        defer(engine_Canvas_destroy(canvas));

        engine_Window_addCanvasView(window, canvas, 0, 0, window_res_width, window_res_height);

        // Initialize ball system
        var allocator = heap_Page_allocator(&(heap_Page){});
        try(heap_Page_init(allocator));
        defer(heap_Page_fini(allocator));

        var manager = try(BallManager_init(allocator));
        defer(BallManager_fini(&manager));
        try(BallManager_replaceRandomly(&manager, 16));

        // Game loop timing setup
        let  target_time = time_Duration_fromSecs_f64(0.016);
        var  prev_time   = time_Instant_now();
        var  curr_time   = prev_time;
        bool is_running  = true;
        while (is_running) {
            curr_time        = time_Instant_now();
            let elapsed_time = time_Instant_durationSince(curr_time, prev_time);
            let dt           = time_Duration_asSecs_f64(elapsed_time);

            // Process events
            try(engine_Window_processEvents(window));
            if (engine_Key_pressed(engine_KeyCode_esc)) {
                is_running = false;
            }

            // Update
            BallManager_processInput(&manager);
            try(BallManager_resolveCollisions(&manager));
            BallManager_update(&manager, as$(f32, dt));

            // Render
            engine_Canvas_clear(canvas, Color_black);
            BallManager_render(&manager, canvas);
            engine_Window_present(window);

            // Frame timing
            if_some(time_Duration_chkdSub(target_time, elapsed_time), sleep_time) {
                time_sleep(sleep_time);
            }
            prev_time = curr_time;
        }
        return_void();
    }
    scope_returnReserved;
}
