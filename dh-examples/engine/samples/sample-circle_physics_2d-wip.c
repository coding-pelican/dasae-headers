#include "dh/main.h"
#include "dh/log.h"

#include "dh/heap/Page.h"
#include "dh/ArrList.h"

#include "dh/time/Duration.h"
#include "dh/time/Instant.h"
#include "dh/Rand.h"

#include "engine.h"
#include "engine/Color.h"
#include "engine/Input.h"

#define window_res_width (160 / 2)
#define window_res_height (100 / 2)
static const f32 window_res_x = window_res_width;
static const f32 window_res_y = window_res_height;
static const f32 window_res_scale = 1.0f;

/* FIXME: Correct collision detection and resolution logic */

// Circular collision and physics structures
typedef struct Circ2f32 {
    m_V2f32 center;
    f32 radius;
} Circ2f32;
$attr($inline_always)
$static fn_((Circ2f32_intersects(Circ2f32 a, Circ2f32 b))(bool)) {
    let diff = m_V2f32_sub(b.center, a.center);
    let dist_sq = m_V2f32_lenSq(diff);
    let sum_radii = a.radius + b.radius;
    return dist_sq <= (sum_radii * sum_radii);
};
$attr($inline_always)
$static fn_((Circ2f32_containsPoint(Circ2f32 c, m_V2f32 point))(bool)) {
    let diff = m_V2f32_sub(point, c.center);
    let dist_sq = m_V2f32_lenSq(diff);
    return dist_sq <= (c.radius * c.radius);
};

typedef struct Ball {
    var_(transform, Circ2f32);
    var_(vel, m_V2f32);
    var_(acc, m_V2f32);
    var_(mass, f32);
} Ball;
T_use$((Ball)(P, S));
T_use$((P$Ball)(O));
$static let_(Ball_mass_scaler_by_radius, f32) = 10.0f;
$static let_(Ball_velocity_tolerance, f32) = 0.01f;
$static let_(Ball_drag_coefficient, f32) = 0.8f;
$static fn_((Ball_init(f32 x, f32 y, f32 radius))(Ball)) {
    return (Ball){
        .transform = {
            .center = { .x = x, .y = y },
            .radius = radius,
        },
        .vel = m_V2f32_zero,
        .acc = m_V2f32_zero,
        .mass = radius * Ball_mass_scaler_by_radius,
    };
};

T_use$((Ball)(
    ArrList,
    ArrList_init,
    ArrList_fini,
    ArrList_clearRetainingCap,
    ArrList_ensureCap,
    ArrList_appendWithin
));
typedef struct BallManager {
    var_(balls, ArrList$Ball);
    var_(selected_ball, O$P$Ball);
    var_(rng, Rand*);
    var_(gpa, mem_Allocator);
} BallManager;
T_use_E$(BallManager);
$attr($must_check)
$static fn_((BallManager_init(mem_Allocator gpa, Rand* rng))(E$BallManager) $scope) {
    return_ok({
        .balls = try_(ArrList_init$Ball(gpa, 32)),
        .selected_ball = none(),
        .rng = rng,
        .gpa = gpa,
    });
} $unscoped_(fn);
$static fn_((BallManager_fini(BallManager* self))(void)) {
    ArrList_fini$Ball(&self->balls, self->gpa);
};
$static fn_((BallManager_replaceAllRandomly(BallManager* self, u32 count))(void)) {
    ArrList_clearRetainingCap$Ball(&self->balls);
    catch_((ArrList_ensureCap$Ball(&self->balls, self->gpa, count))($ignore, claim_unreachable));
    for_(($r(0, count))($ignore) {
        ArrList_appendWithin$Ball(
            &self->balls,
            Ball_init(
                Rand_rangeFlt(self->rng, -window_res_x / 2, window_res_x / 2),
                Rand_rangeFlt(self->rng, -window_res_y / 2, window_res_y / 2),
                Rand_rangeFlt(self->rng, 2.0, 7.0)
            )
        );
    });
    // Set masses based on radii
    for_(($s(self->balls.items))(ball) {
        ball->mass = ball->transform.radius * Ball_mass_scaler_by_radius;
    });
};

// Ball collision record
typedef struct BallCollisionPair {
    usize collider_index;
    usize collidee_index;
} BallCollisionPair;
T_use_S$(BallCollisionPair);
$static fn_((BallManager_resolveCollisions(BallManager* self))(void)) {
    // Quick n^2 collision check between all balls
    for (usize i = 0; i < self->balls.items.len; ++i) {
        let ball = &self->balls.items.ptr[i];

        for (usize j = i + 1; j < self->balls.items.len; ++j) {
            let target = &self->balls.items.ptr[j];

            if (!Circ2f32_intersects(ball->transform, target->transform)) {
                continue;
            }

            // Calculate collision response
            m_V2f32 diff = m_V2f32_sub(target->transform.center, ball->transform.center);
            f32 dist = m_V2f32_len(diff);

            // Prevent division by zero
            if (dist <= 0.0001f) {
                continue;
            }

            // Calculate overlap
            f32 overlap = 0.5f * (dist - ball->transform.radius - target->transform.radius);

            // Separate balls
            m_V2f32 sep_dir = m_V2f32_scale(diff, 1.0f / dist);
            m_V2f32 sep = m_V2f32_scale(sep_dir, overlap);

            m_V2f32_subAsg(&ball->transform.center, sep);
            m_V2f32_addAsg(&target->transform.center, sep);

            // Calculate collision impulse
            m_V2f32 normal = sep_dir;
            m_V2f32 tangent = { .x = -normal.y, .y = normal.x };

            // Project velocities
            f32 dpTanBall = m_V2f32_dot(ball->vel, tangent);
            f32 dpTanTarget = m_V2f32_dot(target->vel, tangent);
            f32 dpNormBall = m_V2f32_dot(ball->vel, normal);
            f32 dpNormTarget = m_V2f32_dot(target->vel, normal);

            // Conservation of momentum
            f32 m1 = ball->mass;
            f32 m2 = target->mass;
            f32 newDpNormBall = (dpNormBall * (m1 - m2) + 2.0f * m2 * dpNormTarget) / (m1 + m2);
            f32 newDpNormTarget = (dpNormTarget * (m2 - m1) + 2.0f * m1 * dpNormBall) / (m1 + m2);

            // Apply new velocities
            ball->vel = m_V2f32_add(
                m_V2f32_scale(tangent, dpTanBall),
                m_V2f32_scale(normal, newDpNormBall)
            );
            target->vel = m_V2f32_add(
                m_V2f32_scale(tangent, dpTanTarget),
                m_V2f32_scale(normal, newDpNormTarget)
            );
        }
    }
};

$static fn_((BallManager_update(BallManager* self, f32 dt))(void)) {
    // Update physics
    for_(($s(self->balls.items))(ball) {
        // Apply drag
        ball->acc = m_V2f32_scale(m_V2f32_neg(ball->vel), Ball_drag_coefficient);

        // Update velocity and position
        m_V2f32_addAsg(&ball->vel, m_V2f32_scale(ball->acc, dt));
        m_V2f32_addAsg(&ball->transform.center, m_V2f32_scale(ball->vel, dt));

        // Screen wrapping
        let wrap_min = m_V2f32_from(-window_res_x / 2, -window_res_y / 2);
        let wrap_max = m_V2f32_from(window_res_x / 2, window_res_y / 2);
        let size = m_V2f32_sub(wrap_max, wrap_min);

        while (ball->transform.center.x < wrap_min.x) { ball->transform.center.x += size.x; }
        while (ball->transform.center.x > wrap_max.x) { ball->transform.center.x -= size.x; }
        while (ball->transform.center.y < wrap_min.y) { ball->transform.center.y += size.y; }
        while (ball->transform.center.y > wrap_max.y) { ball->transform.center.y -= size.y; }

        // Damping for very small velocities
        if (m_V2f32_lenSq(ball->vel) < Ball_velocity_tolerance) { ball->vel = m_V2f32_zero; }
    });
};

// Convert world space to screen space for a ball
$attr($inline_always)
$static fn_((worldToScreen(m_V2f32 pos, engine_Canvas* canvas))(m_V2f32)) {
    let scale = window_res_scale;
    let center = (m_V2f32){
        .x = as$(f32)(canvas->buffer.width) * 0.5f,
        .y = as$(f32)(canvas->buffer.height) * 0.5f
    };
    var screen_pos = m_V2f32_scale(pos, scale);
    screen_pos.y = -screen_pos.y;
    return m_V2f32_add(screen_pos, center);
};
$static fn_((BallManager_render(BallManager* self, engine_Canvas* canvas, const engine_Input* input))(void)) {
    // Draw all balls
    for_(($s(self->balls.items))(ball) {
        let pos = worldToScreen(ball->transform.center, canvas);
        let radius = ball->transform.radius * window_res_scale;
        engine_Canvas_drawCircle(canvas, as$(i32)(pos.x), as$(i32)(pos.y), as$(i32)(radius), Color_white);
    });

    let mouse_pos = engine_Mouse_getPos(input->mouse);
    engine_Canvas_drawPixel(canvas, mouse_pos.x, mouse_pos.y, Color_yellow);

    // Draw selected ball highlight and cue line
    if_some((self->selected_ball)(selected_ball)) {
        let pos = worldToScreen(selected_ball->transform.center, canvas);
        let mouse_pos = m_V_as$(m_V2f32, engine_Mouse_getPos(input->mouse));
        engine_Canvas_drawLine(canvas, as$(i32)(pos.x), as$(i32)(pos.y), as$(i32)(mouse_pos.x), as$(i32)(mouse_pos.y), Color_blue);
    }
};

$attr($inline_always)
$static fn_((screenToWorld(m_V2i32 screen_pos))(m_V2f32)) {
    var pos = (m_V2f32){
        .x = as$(f32)(screen_pos.x),
        .y = as$(f32)(screen_pos.y)
    };
    let scale = 1.0f / window_res_scale;
    let center = (m_V2f32){
        .x = window_res_x * 0.5f, // Hardcoded canvas size for now
        .y = window_res_y * 0.5f
    };
    pos = m_V2f32_sub(pos, center);
    pos.y = -pos.y;
    return m_V2f32_scale(pos, scale);
};
$static fn_((BallManager_processInput(BallManager* self, const engine_Input* input))(void)) {
    // Ball selection
    if (engine_Mouse_pressed(input->mouse, engine_MouseButton_left) || engine_Mouse_pressed(input->mouse, engine_MouseButton_right)) {
        let world_pos = screenToWorld(engine_Mouse_getPos(input->mouse));
        let selected_ball = eval_(O$P$Ball $scope)(for_(($s(self->balls.items))(ball) {
            if (Circ2f32_containsPoint(ball->transform, world_pos)) { $break_(some(ball)); }
        })) eval_(else)( $break_(none())) $unscoped_(eval);
        asg(&self->selected_ball, selected_ball);
    }

    // Ball dragging
    if (engine_Mouse_held(input->mouse, engine_MouseButton_left)) {
        if_some((self->selected_ball)(selected_ball)) {
            selected_ball->transform.center = screenToWorld(engine_Mouse_getPos(input->mouse));
            selected_ball->vel = m_V2f32_zero;
        }
    }

    // Ball launching
    if (engine_Mouse_released(input->mouse, engine_MouseButton_right)) {
        if_some((self->selected_ball)(selected_ball)) {
            let world_pos = screenToWorld(engine_Mouse_getPos(input->mouse));
            let launch_dir = m_V2f32_sub(selected_ball->transform.center, world_pos);
            selected_ball->vel = m_V2f32_scale(launch_dir, 5.0f);
            asg_lit((&self->selected_ball)(none()));
        }
    }
};

fn_((dh_main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    // Initialize logging
    if_(let debug_file = fopen("sample-circle_physics_2d-debug.log", "w"), debug_file) {
        log_initWithFile(debug_file);
        log_setLevel(log_Level_debug);
        log_showTimestamp(true);
        log_showLevel(true);
        log_showLocation(false);
        log_showFunction(true);
    }
    defer_(log_fini());

    // Prepare heap gpa
    var page = lit$((heap_Page){});
    let gpa = heap_Page_allocator(&page);

    // Initialize engine
    let window = try_(engine_Window_init((engine_Window_Config){
        .gpa = some(gpa),
        .rect_size = { .x = window_res_width, .y = window_res_height },
        .default_color = some(Color_black),
        .title = some(u8_l("Circle Physics 2D")),
    }));
    defer_(engine_Window_fini(window));
    let canvas = try_(engine_Canvas_init((engine_Canvas_Config){
        .gpa = some(gpa),
        .width = window_res_width,
        .height = window_res_height,
        .default_color = some(Color_black),
        .type = some(engine_CanvasType_rgba),
    }));
    defer_(engine_Canvas_fini(canvas));
    engine_Window_appendView(
        window,
        (engine_CanvasView_Config){
            .canvas = canvas,
            .pos = { .x = 0, .y = 0 },
            .size = { .x = window_res_width, .y = window_res_height },
            .scale = { .x = 1.0f, .y = 1.0f },
            .resizable_x = true,
            .resizable_y = true,
            .visible = true,
        }
    );
    let input = try_(engine_Input_init(gpa));
    defer_(engine_Input_fini(input));
    let core = try_(engine_core_VT100_init((engine_core_VT100_Config){
        .gpa = some(gpa),
        .window = window,
        .input = input,
    }));

    // Initialize ball system
    var manager = try_(BallManager_init(gpa, &copy(Rand_default)));
    defer_(BallManager_fini(&manager));
    BallManager_replaceAllRandomly(&manager, 16);

    // Game loop timing setup
    let target_time = time_Duration_fromSecs$f64(0.016);
    var prev_time = time_Instant_now();
    var curr_time = prev_time;
    bool is_running = true;
    while (is_running) {
        curr_time = time_Instant_now();
        let elapsed_time = time_Instant_durationSince(curr_time, prev_time);
        let dt = time_Duration_asSecs$f64(elapsed_time);

        // Process events
        try_(engine_Window_update(window));
        if (engine_Keyboard_pressed(input->keyboard, engine_KeyCode_esc)) {
            is_running = false;
        }

        // Update
        BallManager_processInput(&manager, input);
        BallManager_resolveCollisions(&manager);
        BallManager_update(&manager, as$(f32)(dt));

        // Render
        engine_Canvas_clear(canvas, none$((O$Color)));
        BallManager_render(&manager, canvas, input);
        engine_Window_present(window);

        // Frame timing
        if_some((time_Duration_subChkd(target_time, elapsed_time))(sleep_time)) {
            time_sleep(sleep_time);
        }
        prev_time = curr_time;
    }
    return_ok({});
} $unguarded_(fn);
