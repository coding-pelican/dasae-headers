#include "dh/main.h"
#include "dh/debug.h"
#include "dh/log.h"

#include "dh/mem.h"
#include "dh/heap/Page.h"
#include "dh/ArrList.h"

#include "dh/time.h"
#include "dh/Random.h"

#include "engine-wip.h"

#define window_res_width__320x200  /* template value */ (320)
#define window_res_height__320x200 /* template value */ (200)
#define window_res_width__160x100  /* template value */ (160)
#define window_res_height__160x100 /* template value */ (100)
#define window_res_width__80x50    /* template value */ (80)
#define window_res_height__80x50   /* template value */ (50)
#define window_res_width__40x25    /* template value */ (40)
#define window_res_height__40x25   /* template value */ (25)

#define window_res_width  (window_res_width__160x100)
#define window_res_height (window_res_height__160x100)
#define window_res_size   (as$(usize, window_res_width) * window_res_height)

/* (1.0 / target_fps__62_50) ~16ms => ~60 FPS, Assume 62.5 FPS for simplicity */
#define target_fps__125_0 /* template value */ (125.0)
#define target_fps__62_50 /* template value */ (62.50)
#define target_fps__50_00 /* template value */ (50.00)
#define target_fps__31_25 /* template value */ (31.25)

#define target_fps (target_fps__125_0)
#define target_spf (1.0 / target_fps)


#define Firework_effects_max        (25)
#define Firework_effects_per_rocket (25)
#define Fireworks_max               (16)

typedef struct Particle {
    f64   position[2];
    f64   speed[2];
    f64   acceleration[2];
    f64   fading;
    f64   lifetime;
    Color color;
    u32   dimensions[2];
} Particle;
use_Opt$(Particle);
use_Err$(Particle);
use_Sli$(Particle);
extern fn_(Particle_new(f64 x, f64 y, f64 width, f64 height, Color color), Particle);
extern fn_(Particle_init(Particle* p, f64 x, f64 y, f64 width, f64 height, Color color), Particle*);
extern fn_(Particle_withSpeed(Particle* p, f64 x, f64 y), Particle*);
extern fn_(Particle_withAcceleration(Particle* p, f64 x, f64 y), Particle*);
extern fn_(Particle_withFading(Particle* p, f64 fading), Particle*);
extern fn_(Particle_isDead(const Particle* p), bool);
extern fn_(Particle_update(Particle* p, f64 dt), void);
extern fn_(Particle_render(const Particle* p, engine_Canvas* c, f64 dt), void);

use_ArrList$(Particle);
typedef struct Firework {
    Opt$Ptr$Particle rocket;
    ArrList$Particle effects;
    Hsl              effect_base_color;
    mem_Allocator    allocator;
} Firework;
use_Opt$(Firework);
use_Err$(Firework);
use_Err$(Opt$Ptr$Firework);
use_Sli$(Firework);
extern fn_(Firework_init(Firework* f, mem_Allocator allocator, i64 rocket_x, i64 rocket_y, Color effect_base_color), Err$Ptr$Firework) $must_check;
extern fn_(Firework_fini(Firework* f), void);
extern fn_(Firework_isDead(const Firework* f), bool);
extern fn_(Firework_update(Firework* f, f64 dt), Err$void) $must_check;
extern fn_(Firework_render(const Firework* f, engine_Canvas* c, f64 dt), void);

use_ArrList$(Firework);
typedef struct State {
    mem_Allocator       allocator;
    u32                 width;
    u32                 height;
    const engine_Input* input;
    ArrList$Firework    fireworks;
    bool                is_running;
} State;
use_Err$(State);
extern fn_(State_init(mem_Allocator allocator, u32 width, u32 height, const engine_Input* input), Err$State) $must_check;
extern fn_(State_fini(State* s), void);
extern fn_(State_isDead(const State* s), bool);
extern fn_(State_update(State* s, f64 dt), Err$void) $must_check;
extern fn_(State_render(const State* s, engine_Canvas* c, f64 dt), void);
extern fn_(State_spawnFirework(State* s), Err$Opt$Ptr$Firework) $must_check;



fn_scope_ext(dh_main(Sli$Str_const args), Err$void) {
    $unused(args);
    Random_init();

    // Initialize logging to a file
    try_(log_init("log/debug.log"));
    defer_(log_fini());
    {
        // Configure logging behavior
        log_setLevel(log_Level_debug);
        log_showTimestamp(true);
        log_showLevel(true);
        log_showLocation(false);
        log_showFunction(true);
    }

    // Initialize heap allocator and page pool
    var allocator = heap_Page_allocator(create$(heap_Page));
    log_info("allocator reserved");

    // Create window
    let window = try_(engine_Window_init(create$(engine_WindowConfig,
        .allocator = allocator,
        .title     = Str_l("Fireworks"),
        .rect_size = {
            .x = window_res_width,
            .y = window_res_height,
        },
        .default_color = some(Color_black)
    )));
    defer_(engine_Window_fini(window));
    log_info("window created");

    // Create canvases
    let game_canvas = try_(engine_Canvas_init(
        allocator,
        window_res_width,
        window_res_height,
        engine_CanvasType_rgba,
        none$(Opt$Color)
    ));
    defer_(engine_Canvas_fini(game_canvas));
    {
        log_info("canvas created: %s", nameOf(game_canvas));
        engine_Canvas_clear(game_canvas, none$(Opt$Color));
        log_info("canvas cleared: %s", nameOf(game_canvas));
        engine_Window_appendCanvasView(
            window,
            game_canvas,
            make$(Vec2i, .x = 0, .y = 0),
            make$(Vec2u, .x = window_res_width, .y = window_res_height),
            make$(Vec2f, .x = 1.0f, .y = 1.0f),
            true,
            true,
            true
        );
        log_info("canvas views added: %s", nameOf(game_canvas));
    }

    // Create input system
    let input = try_(engine_Input_init(allocator));
    defer_(engine_Input_fini(input));

    // Bind engine core
    let core = try_(engine_core_Vt100_init(create$(engine_core_Vt100_Config,
        .allocator = allocator,
        .window    = window,
        .input     = input,
    )));
    defer_(engine_core_Vt100_fini(core));
    log_info("engine ready");

    // Create game state
    var state = try_(State_init(allocator, window_res_width, window_res_height, input));
    defer_(State_fini(&state));
    log_info("game state created");

    $ignore engine_utils_getch();

    // Initialize timing variables
    let time_frame_target = time_Duration_fromSecs_f64(target_spf);
    var time_frame_prev   = time_Instant_now();
    log_info("game loop started\n");

    // Game loop
    while (!State_isDead(&state)) {
        // 1) Capture the start of the frame (and capture the end-of-frame time of prev iteration’s dt includes sleep)
        let time_frame_curr = time_Instant_now();

        // 2) Compute how long since last frame (purely for your dt usage)
        let time_elapsed = time_Instant_durationSince(time_frame_curr, time_frame_prev);
        let time_dt      = time_Duration_asSecs_f64(time_elapsed);

        // 3) Process input/events
        try_(engine_Window_update(window));

        // 4) Update game state
        try_(State_update(&state, time_dt));

        // 5) Render all views
        // engine_Canvas_clear(game_canvas, none$(Opt$Color));
        State_render(&state, game_canvas, time_dt);
        engine_Window_present(window);

        // 6) (Optional) Display instantaneous FPS
        {
            const f64 time_fps = (0.0 < time_dt) ? (1.0 / time_dt) : 9999.0;
            printf("\033[H\033[40;37m"); // Move cursor to top left
            printf("\rFPS: %6.2f", time_fps);
            debug_only({ // log frame every 1s
                static f64 total_game_time_for_timestamp = 0.0;
                static f64 logging_after_duration        = 0.0;
                total_game_time_for_timestamp += time_dt;
                logging_after_duration += time_dt;
                if (1.0 < logging_after_duration) {
                    logging_after_duration = 0.0;
                    log_debug("[t=%6.2f] dt: %6.2f, fps %6.2f\n", total_game_time_for_timestamp, time_dt, 1.0 / time_dt);
                }
            });
        }

        // 7) Measure how long the update+render actually took
        let time_now        = time_Instant_now();
        let time_frame_used = time_Instant_durationSince(time_now, time_frame_curr);

        // 8) Subtract from our target
        if_some(time_Duration_chkdSub(time_frame_target, time_frame_used), leftover) {
            time_sleep(leftover);
        }
        time_frame_prev = time_frame_curr;
    }
    return_ok({});
} unscoped_ext;



fn_(Particle_new(f64 x, f64 y, f64 width, f64 height, Color color), Particle) {
    return (Particle){
        .position     = { x, y },
        .speed        = { 0.0, 0.0 },
        .acceleration = { 0.0, 0.0 },
        .fading       = 0.0,
        .lifetime     = 1.0,
        .color        = color,
        .dimensions   = { as$(i64, width), as$(i64, height) }
    };
}
fn_(Particle_init(Particle* p, f64 x, f64 y, f64 width, f64 height, Color color), Particle*) {
    debug_assert_nonnull(p);
    *p = Particle_new(x, y, width, height, color);
    return p;
}
fn_(Particle_withSpeed(Particle* p, f64 x, f64 y), Particle*) {
    debug_assert_nonnull(p);
    p->speed[0] = x;
    p->speed[1] = y;
    return p;
}
fn_(Particle_withAcceleration(Particle* p, f64 x, f64 y), Particle*) {
    debug_assert_nonnull(p);
    p->acceleration[0] = x;
    p->acceleration[1] = y;
    return p;
}
fn_(Particle_withFading(Particle* p, f64 fading), Particle*) {
    debug_assert_nonnull(p);
    p->fading = fading;
    return p;
}
fn_(Particle_isDead(const Particle* p), bool) {
    debug_assert_nonnull(p);
    return p->lifetime <= 0.0;
}
fn_(Particle_update(Particle* p, f64 dt), void) {
    debug_assert_nonnull(p);
    $ignore dt;
    if (Particle_isDead(p)) { return; }

    p->speed[0] += p->acceleration[0];
    p->speed[1] += p->acceleration[1];

    p->position[0] += p->speed[0];
    p->position[1] += p->speed[1];

    p->lifetime -= p->fading;
}
fn_(Particle_render(const Particle* p, engine_Canvas* c, f64 dt), void) {
    debug_assert_nonnull(p);
    debug_assert_nonnull(c);
    $ignore dt;
    if (Particle_isDead(p)) { return; }

    let render_color = Color_fromOpaque(
        as$(u8, as$(f64, p->color.r) * p->lifetime),
        as$(u8, as$(f64, p->color.g) * p->lifetime),
        as$(u8, as$(f64, p->color.b) * p->lifetime)
    );
    engine_Canvas_fillRect(
        c,
        as$(i64, p->position[0]),
        as$(i64, p->position[1]),
        as$(u32, p->position[0] + (p->dimensions[0] - 1)),
        as$(u32, p->position[1] + (p->dimensions[1] - 1)),
        render_color
    );
}

fn_scope_ext(Firework_init(Firework* f, mem_Allocator allocator, i64 rocket_x, i64 rocket_y, Color effect_base_color), Err$Ptr$Firework) {
    debug_assert_nonnull(f);
    log_debug("Initializing firework(%p) at (%d, %d)\n", f, rocket_x, rocket_y);
    f->allocator = allocator;

    with_(let rocket = meta_cast$(Particle*, try_(mem_Allocator_create(f->allocator, typeInfo$(Particle))))) {
        errdefer_(mem_Allocator_destroy(f->allocator, anyPtr(rocket)));
        toSome(&f->rocket, pipe(rocket,
            (Particle_init,(as$(f64, rocket_x), as$(f64, rocket_y), 1.0, 3.0, effect_base_color)),
            (Particle_withSpeed,(0.0, -2.0 - Random_f64() * -1.0)),
            (Particle_withAcceleration,(0.0, 0.02))
        ));
    }

    f->effects = type$(ArrList$Particle, try_(ArrList_initCap(typeInfo$(Particle), f->allocator, Firework_effects_per_rocket)) );
    errdefer_(ArrList_fini(f->effects.base));

    f->effect_base_color = Color_intoHsl(effect_base_color);

    return_ok(f);
} unscoped_ext;
fn_(Firework_fini(Firework* f), void) {
    debug_assert_nonnull(f);
    log_debug("Destroying firework(%p)\n", f);

    if_some(f->rocket, rocket) {
        log_debug("Destroying rocket(%p)\n", rocket);
        mem_Allocator_destroy(f->allocator, anyPtr(rocket));
        log_debug("rocket destroyed\n");
    }

    log_debug("Destroying effects(%p)\n", f->effects.items);
    ArrList_fini(f->effects.base);
    log_debug("effects destroyed\n");

    log_debug("firework destroyed\n");
}
$inline fn_(Firework__deadsAllEffect(const Firework* f), bool) {
    debug_assert_nonnull(f);
    for_slice (f->effects.items, effect) {
        if (Particle_isDead(effect)) { continue; }
        return false;
    }
    return true;
}
fn_(Firework_isDead(const Firework* f), bool) {
    debug_assert_nonnull(f);
    if (f->rocket.has_value) { return false; }
    /* NOTE: Why isn't dead firework removed? (deallocate memory)
        Looking at the log, when fireworks are removed:
            1. Dead fireworks should be removed in `State_spawnFirework`
            2. The log shows firework count reaching and staying at 16 (max capacity)
            3. Even after destroying rockets and effects, the firework objects remain
        The issue is in State_spawnFirework.
        `Firework_isDead` returns false when it should return true.
        The condition `f->effects.items.len != 0` prevents removal even when all effects are dead.
        It should only check if the effects are dead through `Firework__deadsAllEffect`, not the array length.
        This is because effects are being used like a memory pool, not reallocated each time, and are never destroyed.
     */
    // if (f->effects.items.len != 0) { return false; }
    if (!Firework__deadsAllEffect(f)) { return false; }
    return true;
}
fn_scope(Firework_update(Firework* f, f64 dt), Err$void) {
    debug_assert_nonnull(f);
    $ignore dt;
    if_some(f->rocket, rocket) {
        Particle_update(rocket, dt);
        if (-0.2 <= rocket->speed[1]) {
            log_debug(
                "Spawning %d effects at (%.2f, %.2f)",
                Firework_effects_per_rocket,
                rocket->position[0],
                rocket->position[1]
            );
            for (i64 i = 0; i < Firework_effects_per_rocket; ++i) {
                if (Firework_effects_max <= f->effects.items.len) { break; }
                with_(let particle = meta_cast$(Particle*, try_(ArrList_addBackOne(f->effects.base)))) {
                    let x      = rocket->position[0];
                    let y      = rocket->position[1];
                    let width  = 1.0;
                    let height = 1.0;
                    let color  = Hsl_intoColorOpaque(Hsl_from(
                        f->effect_base_color.h,
                        f->effect_base_color.s + (Random_f64() - 0.5) * 20.0,
                        f->effect_base_color.l + (Random_f64() - 0.5) * 40.0
                    ));
                    pipe(particle,
                        (Particle_init,(x, y, width, height, color)),
                        (Particle_withSpeed,((Random_f64() - 0.5) * 1.0, (Random_f64() - 0.9) * 1.0)),
                        (Particle_withAcceleration,(0.0, 0.02)),
                        (Particle_withFading,(0.01))
                    );
                }
            }
            log_debug("destroying rocket(%p)\n", rocket);
            mem_Allocator_destroy(f->allocator, anyPtr(rocket));
            toNone(&f->rocket);
            log_debug("rocket destroyed\n");
        }
    }
    for_slice (f->effects.items, effect) {
        Particle_update(effect, dt);
    }
    return_ok({});
} unscoped;
fn_(Firework_render(const Firework* f, engine_Canvas* c, f64 dt), void) {
    debug_assert_nonnull(f);
    debug_assert_nonnull(c);

    if_some(f->rocket, rocket) {
        debug_assert_fmt(!Particle_isDead(rocket), "rocket must be alive");
        Particle_render(rocket, c, dt);
    }
    for_slice (f->effects.items, effect) {
        Particle_render(effect, c, dt);
    }
}

fn_scope(State_init(mem_Allocator allocator, u32 width, u32 height, const engine_Input* input), Err$State) {
    var fireworks = type$(ArrList$Firework, try_(ArrList_initCap(typeInfo$(Firework), allocator, Fireworks_max)));
    return_ok({
        .allocator  = allocator,
        .width      = width,
        .height     = height,
        .input      = input,
        .fireworks  = fireworks,
        .is_running = true,
    });
} unscoped;
fn_(State_fini(State* s), void) {
    debug_assert_nonnull(s);

    for_slice (s->fireworks.items, firework) {
        Firework_fini(firework);
    }
    ArrList_fini(s->fireworks.base);
}
fn_(State_isDead(const State* s), bool) {
    debug_assert_nonnull(s);
    return !s->is_running;
}
fn_scope(State_update(State* s, f64 dt), Err$void) {
    debug_assert_nonnull(s);

    /* Add a new rocket with with 5% chance */
    if (Random_f64() < 0.05) {
        /* NOTE: Rule #0 - Don't put code that causes side effects in debug code like assertions
            What a stupid mistake...
            This explains why the fireworks were automatically triggered in debug mode,
            but not in release mode.
         */
        // debug_only(
        if_some(try_(State_spawnFirework(s)), firework) {
            let rocket = unwrap(firework->rocket);
            log_debug("Spawning rocket at (%.2f, %.2f)", rocket->position[0], rocket->position[1]);
        }
        // );
    }

    /* Update all fireworks */
    for_slice (s->fireworks.items, firework) {
        try_(Firework_update(firework, dt));
    }

    // Input handling
    if (engine_Keyboard_pressed(&s->input->keyboard, engine_KeyCode_esc)) {
        log_debug("pressed esc\n");
        s->is_running = false;
        return_ok({});
    }

    if (engine_Keyboard_pressed(&s->input->keyboard, engine_KeyCode_space)) {
        log_debug("pressed space\n");
        let maybe_firework = catch_from(State_spawnFirework(s), err, ({
            log_error("failed to spawn firework: %s\n", Err_codeToCStr(err));
            return_err(err);
        }));
        if_some(maybe_firework, firework) {
            let rocket = unwrap(firework->rocket);
            log_debug("Spawning rocket at (%.2f, %.2f)", rocket->position[0], rocket->position[1]);
        }
    }

    if (engine_Mouse_pressed(&s->input->mouse, engine_MouseButton_left)) {
        log_debug("pressed left mouse button\n");
        let maybe_firework = catch_from(State_spawnFirework(s), err, ({
            log_error("failed to spawn firework: %s\n", Err_codeToCStr(err));
            return_err(err);
        }));
        if_some(maybe_firework, firework) {
            let rocket = unwrap(firework->rocket);
            log_debug("Spawning rocket at (%.2f, %.2f)", rocket->position[0], rocket->position[1]);
            let mouse_pos = engine_Mouse_getPos(&s->input->mouse);
            pipe(rocket,
                (Particle_init,(mouse_pos.x, mouse_pos.y, 1, 1, rocket->color)),
                (Particle_withFading,(0.0))
            );
            try_(Firework_update(firework, 0.0));
        }
    }

    return_({});
} unscoped;
fn_(State_render(const State* s, engine_Canvas* c, f64 dt), void) {
    debug_assert_nonnull(s);
    debug_assert_nonnull(c);

    engine_Canvas_clear(c, some$(Opt$Color, Color_black));
    for_slice (s->fireworks.items, firework) {
        Firework_render(firework, c, dt);
    }
}
fn_scope(State_spawnFirework(State* s), Err$Opt$Ptr$Firework) {
    debug_assert_nonnull(s);
    log_debug("Spawning new firework...");
    log_debug("%d fireworks remaining: Removing dead fireworks...", s->fireworks.items.len);
    // Remove dead fireworks.
    for (usize index = 0; index < s->fireworks.items.len; ++index) {
        let firework = &s->fireworks.items.ptr[index];
        if (!Firework_isDead(firework)) { continue; }
        let removed = meta_cast$(Firework*, ArrList_removeSwap(s->fireworks.base, index--));
        Firework_fini(removed);
    }
    log_debug("Removed dead fireworks: %d fireworks remaining", s->fireworks.items.len);

    if (s->fireworks.cap <= s->fireworks.items.len) {
        return_ok(none());
    }

    let firework = meta_cast$(Firework*, try_(ArrList_addBackOne(s->fireworks.base)));
    return_ok(some(try_(Firework_init(
        firework,
        s->allocator,
        as$(i64, Random_u32() % s->width),
        s->height,
        Color_fromOpaque(Random_u8(), Random_u8(), Random_u8())
    ))));
} unscoped;
