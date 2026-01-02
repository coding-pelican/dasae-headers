/**
 * @file    example-circ_phys_2d.c
 * @brief   Circle physics 2D example - ported from old engine to dage
 * @details Demonstrates 2D circle collision and physics simulation:
 *          - Elastic ball collisions with momentum conservation
 *          - Drag simulation (rolling friction)
 *          - Screen wrapping
 *          - Mouse interaction (drag and launch balls)
 *
 * @note run with `dh-c run example-circ_phys_2d.c --sample`
 */

#include "dage.h"
#include <dh/main.h>
#include <dh/Rand.h>
#include <dh/heap/Page.h>
#include <dh/ArrList.h>
#include <dh/io/stream.h>

/*========== Circle Collision Geometry ==========*/

typedef struct Circ2f32 {
    var_(center, m_V2f32);
    var_(radius, f32);
} Circ2f32;

$attr($inline_always)
$static fn_((Circ2f32_of(m_V2f32 center, f32 radius))(Circ2f32)) {
    return (Circ2f32){
        .center = center,
        .radius = radius,
    };
};

$attr($inline_always)
$static fn_((Circ2f32_intersects(Circ2f32 self, Circ2f32 other))(bool)) {
    let dist_sq = m_V2f32_distSq(other.center, self.center);
    let sum_radii = self.radius + other.radius;
    return dist_sq <= flt_sq(sum_radii);
};

$attr($inline_always)
$static fn_((Circ2f32_containsPoint(Circ2f32 self, m_V2f32 point))(bool)) {
    let dist_sq = m_V2f32_distSq(point, self.center);
    return dist_sq <= flt_sq(self.radius);
};

/*========== Configuration ==========*/

#define window_width 640
#define window_height 480
#define initial_ball_count 16

/*========== Physics Constants ==========*/

$static let_(Ball_mass_scale_by_radius, f32) = 10.0f;
$static let_(Ball_velocity_tolerance, f32) = 0.01f;
$static let_(Ball_drag_coefficient, f32) = 0.8f;
$static let_(Ball_launch_multiplier, f32) = 5.0f;

/*========== Ball Structure ==========*/

typedef struct Ball {
    var_(xform, Circ2f32);
    var_(vel, m_V2f32);
    var_(acc, m_V2f32);
    var_(mass, f32);
    var_(id, usize);
} Ball;
T_use$((Ball)(P, S));
T_use$((P$Ball)(O));
T_use$((Ball)(
    ArrList,
    ArrList_init,
    ArrList_fini,
    ArrList_clearRetainingCap,
    ArrList_ensureCap,
    ArrList_appendWithin
));

$attr($inline_always)
$static fn_((Ball_of(m_V2f32 center, f32 radius, usize id))(Ball)) {
    return (Ball){
        .xform = Circ2f32_of(center, radius),
        .vel = m_V2f32_zero,
        .acc = m_V2f32_zero,
        .mass = radius * Ball_mass_scale_by_radius,
        .id = id,
    };
};

/*========== Collision Pair ==========*/

typedef struct CollisionPair {
    var_(collider_idx, usize);
    var_(collidee_idx, usize);
} CollisionPair;
T_use$((CollisionPair)(S));
T_use$((CollisionPair)(
    ArrList,
    ArrList_init,
    ArrList_fini,
    ArrList_clearRetainingCap,
    ArrList_appendWithin
));

$attr($inline_always)
$static fn_((CollisionPair_of(usize collider_idx, usize collidee_idx))(CollisionPair)) {
    return (CollisionPair){
        .collider_idx = collider_idx,
        .collidee_idx = collidee_idx,
    };
};

/*========== Ball Manager ==========*/

typedef struct BallManager {
    var_(balls, ArrList$Ball);
    var_(collision_pairs, ArrList$CollisionPair);
    var_(selected_ball, O$P$Ball);
    var_(rng, Rand);
    var_(world_half_size, m_V2f32);
    var_(gpa, mem_Allocator);
} BallManager;
T_use_E$(BallManager);

$attr($must_check)
$static fn_((BallManager_init(mem_Allocator gpa, m_V2f32 world_half_size))(E$BallManager) $guard) {
    var balls = try_(ArrList_init$Ball(gpa, 32));
    errdefer_($ignore, ArrList_fini$Ball(&balls, gpa));
    var collision_pairs = try_(ArrList_init$CollisionPair(gpa, 64));
    return_ok({
        .balls = balls,
        .collision_pairs = collision_pairs,
        .selected_ball = none(),
        .rng = Rand_init(),
        .world_half_size = world_half_size,
        .gpa = gpa,
    });
} $unguarded_(fn);

$static fn_((BallManager_fini(BallManager* self))(void)) {
    ArrList_fini$CollisionPair(&self->collision_pairs, self->gpa);
    ArrList_fini$Ball(&self->balls, self->gpa);
};

$static fn_((BallManager_replaceAllRandomly(BallManager* self, u32 count))(void)) {
    ArrList_clearRetainingCap$Ball(&self->balls);
    catch_((ArrList_ensureCap$Ball(&self->balls, self->gpa, count))($ignore, claim_unreachable));

    for_(($r(0, count))(i) {
        let center = m_V2f32_of(
            (as$(f32)(Rand_rangeFlt(&self->rng, as$(f64)(-self->world_half_size.x), as$(f64)(self->world_half_size.x)))),
            (as$(f32)(Rand_rangeFlt(&self->rng, as$(f64)(-self->world_half_size.y), as$(f64)(self->world_half_size.y))))
        );
        let radius = as$(f32)(Rand_rangeFlt(&self->rng, as$(f64)(5.0f), as$(f64)(20.0f)));
        ArrList_appendWithin$Ball(&self->balls, Ball_of(center, radius, i));
    });
};

$static fn_((BallManager_updatePhysics(BallManager* self, f32 dt))(void)) {
    for_(($s(self->balls.items))(ball) {
        /* Apply drag to simulate rolling friction */
        ball->acc = m_V2f32_scal(m_V2f32_neg(ball->vel), Ball_drag_coefficient);

        /* Update vel and position */
        m_V2f32_addAsg(&ball->vel, m_V2f32_scal(ball->acc, dt));
        m_V2f32_addAsg(&ball->xform.center, m_V2f32_scal(ball->vel, dt));

        /* Screen wrapping */
        let wrap_min = m_V2f32_neg(self->world_half_size);
        let wrap_max = self->world_half_size;
        let size = m_V2f32_scal(self->world_half_size, 2.0f);

        while (ball->xform.center.x < wrap_min.x) { ball->xform.center.x += size.x; }
        while (ball->xform.center.x > wrap_max.x) { ball->xform.center.x -= size.x; }
        while (ball->xform.center.y < wrap_min.y) { ball->xform.center.y += size.y; }
        while (ball->xform.center.y > wrap_max.y) { ball->xform.center.y -= size.y; }

        /* Clamp very small velocities to zero */
        if (m_V2f32_lenSq(ball->vel) < Ball_velocity_tolerance) {
            ball->vel = m_V2f32_zero;
        }
    });
};

$static fn_((BallManager_detectAndSeparateCollisions(BallManager* self))(void)) {
    /* Clear collision pairs from previous frame */
    ArrList_clearRetainingCap$CollisionPair(&self->collision_pairs);

    /* Detect collisions and separate balls (O(n^2)) */
    for (usize i = 0; i < self->balls.items.len; ++i) {
        let ball = &self->balls.items.ptr[i];

        for (usize j = i + 1; j < self->balls.items.len; ++j) {
            let target = &self->balls.items.ptr[j];

            if (!Circ2f32_intersects(ball->xform, target->xform)) {
                continue;
            }

            /* Distance between ball centers */
            let diff = m_V2f32_sub(ball->xform.center, target->xform.center);
            let distance = m_V2f32_len(diff);
            /* Prevent division by zero */
            if (distance < 0.0001f) {
                continue;
            }

            /* Record this collision pair for vel resolution */
            ArrList_appendWithin$CollisionPair(
                &self->collision_pairs,
                CollisionPair_of(i, j)
            );

            /* Calculate overlap (negative when intersecting) */
            let overlap = 0.5f * (distance - ball->xform.radius - target->xform.radius);

            /* Separation direction: from target to ball (normalized diff) */
            let sep_dir = m_V2f32_scal(diff, 1.0f / distance);

            /* Displace balls away from each other
             * overlap is negative, so:
             * - ball moves in sep_dir direction (away from target)
             * - target moves opposite (away from ball)
             */
            m_V2f32_subAsg(&ball->xform.center, m_V2f32_scal(sep_dir, overlap));
            m_V2f32_addAsg(&target->xform.center, m_V2f32_scal(sep_dir, overlap));
        }
    }
};

$static fn_((BallManager_resolveCollisionVelocities(BallManager* self))(void)) {
    /* Process all collision pairs and update velocities */
    for_(($s(self->collision_pairs.items))(pair) {
        let collider = S_at((self->balls.items)[pair->collider_idx]);
        let collidee = S_at((self->balls.items)[pair->collidee_idx]);

        /* Distance vector between balls (from collider to collidee) */
        let dist_vec = m_V2f32_sub(collidee->xform.center, collider->xform.center);
        let distance = m_V2f32_len(dist_vec);
        /* Prevent division by zero */
        if (distance < 0.0001f) {
            continue;
        }

        /* Normal vector (from collider toward collidee) */
        let normal = m_V2f32_scal(dist_vec, 1.0f / distance);
        /* Tangent vector (perpendicular to normal) */
        let tangent = m_V2f32_of(-normal.y, normal.x);

        /* Project velocities onto tangent (preserved) and normal (exchanged) */
        let dp_tan_collider = m_V2f32_dot(collider->vel, tangent);
        let dp_tan_collidee = m_V2f32_dot(collidee->vel, tangent);
        let dp_norm_collider = m_V2f32_dot(collider->vel, normal);
        let dp_norm_collidee = m_V2f32_dot(collidee->vel, normal);

        /* Conservation of momentum in 1D (elastic collision) */
        let m1 = collider->mass;
        let m2 = collidee->mass;
        let momentum_collider = (dp_norm_collider * (m1 - m2) + 2.0f * m2 * dp_norm_collidee) / (m1 + m2);
        let momentum_collidee = (dp_norm_collidee * (m2 - m1) + 2.0f * m1 * dp_norm_collider) / (m1 + m2);

        /* Update velocities: tangent component unchanged, normal component exchanged */
        collider->vel = m_V2f32_add(
            m_V2f32_scal(tangent, dp_tan_collider),
            m_V2f32_scal(normal, momentum_collider)
        );
        collidee->vel = m_V2f32_add(
            m_V2f32_scal(tangent, dp_tan_collidee),
            m_V2f32_scal(normal, momentum_collidee)
        );
    });
};

/*========== Coordinate Conversion ==========*/

/// World: Y-up, origin at center
/// Screen: Y-down, origin at top-left
$attr($inline_always)
$static fn_((worldToScreen(m_V2f32 world_pos, m_V2f32 screen_center))(m_V2f32)) {
    return m_V2f32_of(
        world_pos.x + screen_center.x,
        screen_center.y - world_pos.y
    );
};

/// Screen: Y-down, origin at top-left
/// World: Y-up, origin at center
$attr($inline_always)
$static fn_((screenToWorld(m_V2i32 screen_pos, m_V2f32 screen_center))(m_V2f32)) {
    return m_V2f32_of(
        (as$(f32)(screen_pos.x)) - screen_center.x,
        screen_center.y - (as$(f32)(screen_pos.y))
    );
};

/*========== Input Processing ==========*/

$static fn_((BallManager_processInput(BallManager* self, dage_Window* win, m_V2f32 screen_center))(void)) {
    let input = &win->input;
    let isMousePressed = dage_InputState_isMousePressed;
    let isMouseHeld = dage_InputState_isMouseHeld;
    let isMouseReleased = dage_InputState_isMouseReleased;
    let Button_left = dage_MouseButton_left;
    let Button_right = dage_MouseButton_right;

    let mouse_pos = dage_Window_getMousePos(win);
    let world_mouse = screenToWorld(mouse_pos, screen_center);

    /* Ball selection on mouse press */
    if (isMousePressed(input, Button_left) || isMousePressed(input, Button_right)) {
        asg_lit((&self->selected_ball)(none()));
        for_(($s(self->balls.items))(ball) {
            if (Circ2f32_containsPoint(ball->xform, world_mouse)) {
                asg_lit((&self->selected_ball)(some(ball)));
                break;
            }
        });
    }

    /* Drag ball with left mouse */
    if (isMouseHeld(input, Button_left)) {
        if_some((self->selected_ball)(selected)) {
            selected->xform.center = world_mouse;
            selected->vel = m_V2f32_zero;
        }
    }

    /* Release left mouse - just deselect */
    if (isMouseReleased(input, Button_left)) {
        asg_lit((&self->selected_ball)(none()));
    }

    /* Release right mouse - launch ball */
    if (isMouseReleased(input, Button_right)) {
        if_some((self->selected_ball)(selected)) {
            let launch_dir = m_V2f32_sub(selected->xform.center, world_mouse);
            selected->vel = m_V2f32_scal(launch_dir, Ball_launch_multiplier);
            asg_lit((&self->selected_ball)(none()));
        }
    }
};

/*========== Rendering ==========*/

$static fn_((BallManager_render(const BallManager* self, dage_Canvas* canvas, m_V2f32 screen_center, m_V2i32 mouse_pos))(void)) {
    /* Draw all balls */
    for_(($s(self->balls.items))(ball) {
        let pos = worldToScreen(ball->xform.center, screen_center);
        let radius = as$(i32)(ball->xform.radius);
        dage_Canvas_drawCircle(canvas, as$(i32)(pos.x), as$(i32)(pos.y), radius, color_RGBA_white);
    });

    /* Draw collision lines */
    for_(($s(self->collision_pairs.items))(pair) {
        let collider = S_at((self->balls.items)[pair->collider_idx]);
        let collidee = S_at((self->balls.items)[pair->collidee_idx]);
        let pos1 = worldToScreen(collider->xform.center, screen_center);
        let pos2 = worldToScreen(collidee->xform.center, screen_center);
        dage_Canvas_drawLine(canvas, as$(i32)(pos1.x), as$(i32)(pos1.y), as$(i32)(pos2.x), as$(i32)(pos2.y), color_RGBA_red);
    });

    /* Draw cue line for selected ball */
    if_some((self->selected_ball)(selected)) {
        let pos = worldToScreen(selected->xform.center, screen_center);
        dage_Canvas_drawLine(canvas, as$(i32)(pos.x), as$(i32)(pos.y), mouse_pos.x, mouse_pos.y, color_RGBA_blue);
    }
};

/*========== Event Handling ==========*/

$static fn_((handleEvents(dage_Window* win, bool* running))(void)) {
    while_some(dage_Window_pollEvent(win), event) {
        match_(event) {
        case_((dage_Event_close_request)) {
            *running = false;
        } $end(case);

        pattern_((dage_Event_key_down)(on_key_down)) {
            if (on_key_down->key == dage_KeyCode_esc) {
                *running = false;
            }
        } $end(pattern);

        default_() {
            /* Ignore other events */
        } $end(default);
        } $end(match);
    };
};

/*========== Main ==========*/

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    /* Setup allocator */
    var page = (heap_Page){};
    let gpa = heap_Page_allocator(&page);

    /* Create WSI backend */
    var wsi = try_(dage_core_WSI_init((dage_core_WSI_Cfg){
        .gpa = gpa,
    }));
    defer_(dage_core_WSI_fini(&wsi));
    let backend = dage_core_WSI_backend(wsi);
    io_stream_println(u8_l("Backend created"));

    /* Create runtime */
    var runtime = try_(dage_Runtime_init((dage_Runtime_Cfg){
        .gpa = gpa,
        .backend = backend,
    }));
    defer_(dage_Runtime_fini(&runtime));
    io_stream_println(u8_l("Runtime created"));

    /* Create window */
    let win_id = try_(dage_Runtime_createWindow(
        &runtime,
        (dage_Runtime_WindowCfg){
            .size = { .x = window_width, .y = window_height },
            .clear_color = some(color_RGBA_black),
            .title = some(u8_l("Circle Physics 2D")),
            .scale = 1.0f,
            .resizable = true,
        }
    ));
    let win = unwrap_(dage_Runtime_getWindow(&runtime, win_id));
    io_stream_println(u8_l("Window created"));

    /* Create game canvas */
    var canvas = try_(dage_Canvas_init((dage_Canvas_Cfg){
        .gpa = gpa,
        .width = window_width,
        .height = window_height,
        .type = dage_CanvasType_rgba,
    }));
    defer_(dage_Canvas_fini(&canvas, gpa));
    io_stream_println(u8_l("Canvas created"));

    /* Add viewport */
    let_ignore = unwrap_(dage_Window_addViewport(
        win,
        (dage_Viewport_Cfg){
            .canvas = &canvas,
            .dst_rect = {
                .pos = { .x = 0, .y = 0 },
                .size = { .x = window_width, .y = window_height },
            },
            .fit = dage_Viewport_Fit_stretch,
            .visible = true,
            .z_order = 0,
        }
    ));
    io_stream_println(u8_l("Viewport added"));

    /* Initialize ball manager */
    let screen_center = m_V2f32_of(
        (as$(f32)(window_width)) * 0.5f,
        (as$(f32)(window_height)) * 0.5f
    );
    let world_half_size = screen_center;

    var manager = try_(BallManager_init(gpa, world_half_size));
    defer_(BallManager_fini(&manager));
    BallManager_replaceAllRandomly(&manager, initial_ball_count);
    io_stream_println(u8_l("Ball manager initialized"));

    /* Game loop */
    let target_fps = 60.0f;
    let frame_time = 1.0f / target_fps;
    bool running = true;
    var prev_time = time_Instant_now();
    io_stream_println(u8_l("Game loop started"));
    while (running && !dage_Runtime_shouldQuit(&runtime)) {
        /* Calculate delta time */
        let curr_time = time_Instant_now();
        let elapsed = time_Instant_durationSince(curr_time, prev_time);
        let dt = time_Duration_asSecs$f32(elapsed);
        prev_time = curr_time;

        /* Process events */
        dage_Runtime_processEvents(&runtime);
        handleEvents(win, &running);

        /* Input */
        BallManager_processInput(&manager, win, screen_center);

        /* Physics update */
        BallManager_updatePhysics(&manager, dt);
        BallManager_detectAndSeparateCollisions(&manager);
        BallManager_resolveCollisionVelocities(&manager);

        /* Render */
        dage_Canvas_clear(&canvas, some$((O$color_RGBA)color_RGBA_black));
        let mouse_pos = dage_Window_getMousePos(win);
        BallManager_render(&manager, &canvas, screen_center, mouse_pos);

        /* Present */
        dage_Runtime_presentAll(&runtime);
        dage_Runtime_endFrame(&runtime);

        /* Frame timing */
        let target_time = time_Duration_fromSecs$f64(as$(f64)(frame_time));
        if_some((time_Duration_subChkd(target_time, elapsed))(sleep_time)) {
            time_sleep(sleep_time);
        }
    }

    io_stream_println(u8_l("Circle Physics 2D exited cleanly"));
    return_ok({});
} $unguarded_(fn);
