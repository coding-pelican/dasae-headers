#include "dh/main.h"
#include "dh/debug.h"
#include "dh/log.h"

#include "dh/mem.h"
#include "dh/heap/Classic.h"
#include "dh/ArrList.h"

#include "dh/time.h"
#include "dh/Random.h"

#include "engine.h"

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
extern Particle  Particle_new(f64 x, f64 y, f64 width, f64 height, Color color);
extern Particle* Particle_init(Particle* p, f64 x, f64 y, f64 width, f64 height, Color color);
extern Particle* Particle_initWithSpeed(Particle* p, f64 x, f64 y);
extern Particle* Particle_initWithAcceleration(Particle* p, f64 x, f64 y);
extern Particle* Particle_initWithFading(Particle* p, f64 fading);
extern bool      Particle_isDead(const Particle* p);
extern void      Particle_update(Particle* p, f64 dt);
extern void      Particle_render(const Particle* p, engine_Canvas* c, f64 dt);

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
extern Err$Ptr$Firework Firework_init(Firework* f, mem_Allocator allocator, i64 rocket_x, i64 rocket_y, Color effect_base_color) must_check;
extern void             Firework_fini(Firework* f);
extern bool             Firework_isDead(const Firework* f);
extern Err$void         Firework_update(Firework* f, f64 dt) must_check;
extern void             Firework_render(const Firework* f, engine_Canvas* c, f64 dt);

use_ArrList$(Firework);
typedef struct State {
    ArrList$Firework fireworks;
    u32              width;
    u32              height;
    mem_Allocator    allocator;
    bool             is_running;
} State;
use_Err$(State);
extern Err$State            State_init(mem_Allocator allocator, u32 width, u32 height) must_check;
extern void                 State_fini(State* s);
extern bool                 State_isDead(const State* s);
extern Err$void             State_update(State* s, f64 dt) must_check;
extern void                 State_render(const State* s, engine_Canvas* c, f64 dt);
extern Err$Opt$Ptr$Firework State_spawnFirework(State* s) must_check;



Err$void dh_main(int argc, const char* argv[]) {
    unused(argc), unused(argv);
    scope_reserveReturn(Err$void) {
        Random_init();

        // Initialize logging to a file
        scope_if(let debug_file = fopen("main-debug.log", "w"), debug_file) {
            log_initWithFile(debug_file);
            // Configure logging behavior
            log_setLevel(log_Level_debug);
            log_showTimestamp(true);
            log_showLevel(true);
            log_showLocation(false);
            log_showFunction(true);
        }
        defer(log_fini());

        // Initialize platform with terminal backend
        let window = try(engine_Window_create(
            &(engine_PlatformParams){
                .backend_type = engine_RenderBackendType_vt100,
                .window_title = "Fireworks",
                .width        = 80 * 2,
                .height       = 50 * 2,
            }
        ));
        defer(engine_Window_destroy(window));
        log_info("engine initialized\n");

        // Create canvases
        let game_canvas = catch (engine_Canvas_create(80 * 2, 50 * 2, engine_CanvasType_rgba), err, {
            log_error("Failed to create canvas: %s\n", err);
            return_err(err);
        });
        defer(engine_Canvas_destroy(game_canvas));
        log_info("canvas created\n");

        engine_Canvas_clear(game_canvas, Color_blank);
        log_info("canvas cleared\n");

        // Add canvas views
        engine_Window_addCanvasView(window, game_canvas, 0, 0, 80 * 2, 50 * 2);
        log_info("canvas views added\n");

        var heap      = (heap_Classic){};
        var allocator = heap_Classic_allocator(&heap);
        var state     = catch (State_init(allocator, 80 * 2, 50 * 2), err, {
            log_error("Failed to create game state: %s\n", err);
            return_err(err);
        });
        defer(State_fini(&state));
        log_info("game state created\n");
        ignore getchar();

        var curr_time   = time_Instant_now();
        var prev_time   = curr_time;
        let target_time = time_Duration_fromSecs_f64(0.016f); // Assume 62.5 FPS for simplicity
        log_info("game loop started\n");

        // Game loop
        while (!State_isDead(&state)) {
            // const f64   real_delta_time = elapsed;
            // const usize update_step     = (usize)(Display_deltaTime() / real_delta_time);
            // for (usize step = 0; step < update_step; ++step) {
            //     const f64 delta_time = (f64)step * real_delta_time;

            curr_time        = time_Instant_now();
            let elapsed_time = time_Instant_durationSince(curr_time, prev_time);
            let dt           = time_Duration_asSecs_f64(elapsed_time);

            // Process events
            try(engine_Window_processEvents(window));

            // Update game state
            try(State_update(&state, dt));

            // Render all views
            State_render(&state, game_canvas, dt);

            // Present to screen
            engine_Window_present(window);

            // Sleep for the remaining time to maintain FPS
            time_sleep(time_Duration_sub(target_time, elapsed_time));
            prev_time = curr_time;
        }
        return_void();
    }
    scope_returnReserved;
}



Particle Particle_new(f64 x, f64 y, f64 width, f64 height, Color color) {
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

Particle* Particle_init(Particle* p, f64 x, f64 y, f64 width, f64 height, Color color) {
    debug_assert_nonnull(p);
    *p = Particle_new(x, y, width, height, color);
    return p;
}

Particle* Particle_initWithSpeed(Particle* p, f64 x, f64 y) {
    debug_assert_nonnull(p);
    p->speed[0] = x;
    p->speed[1] = y;
    return p;
}

Particle* Particle_initWithAcceleration(Particle* p, f64 x, f64 y) {
    debug_assert_nonnull(p);
    p->acceleration[0] = x;
    p->acceleration[1] = y;
    return p;
}

Particle* Particle_initWithFading(Particle* p, f64 fading) {
    debug_assert_nonnull(p);
    p->fading = fading;
    return p;
}

bool Particle_isDead(const Particle* p) {
    debug_assert_nonnull(p);
    return p->lifetime <= 0.0;
}

void Particle_update(Particle* p, f64 dt) {
    debug_assert_nonnull(p);
    unused(dt);
    if (Particle_isDead(p)) { return; }

    p->speed[0] += p->acceleration[0];
    p->speed[1] += p->acceleration[1];

    p->position[0] += p->speed[0];
    p->position[1] += p->speed[1];

    p->lifetime -= p->fading;
}

void Particle_render(const Particle* p, engine_Canvas* c, f64 dt) {
    debug_assert_nonnull(p);
    debug_assert_nonnull(c);
    unused(dt);
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

Err$Ptr$Firework Firework_init(Firework* f, mem_Allocator allocator, i64 rocket_x, i64 rocket_y, Color effect_base_color) {
    scope_reserveReturn(Err$Ptr$Firework) {
        debug_assert_nonnull(f);

        log_debug("Initializing firework(%p) at (%d, %d)\n", f, rocket_x, rocket_y);
        f->allocator = allocator;

        scope_with(let rocket = meta_castPtr$(Particle*, try(mem_Allocator_create(f->allocator, typeInfo(Particle))))) {
            errdefer(mem_Allocator_destroy(f->allocator, anyPtr(rocket)));
            Particle_init(rocket, as$(f64, rocket_x), as$(f64, rocket_y), 1.0, 3.0, effect_base_color);
            Particle_initWithSpeed(rocket, 0.0, -2.0 - Random_f64() * -1.0);
            Particle_initWithAcceleration(rocket, 0.0, 0.02);
            f->rocket = (Opt$Ptr$Particle)some(rocket);
        }

        f->effects = typed(ArrList$Particle, try(ArrList_initCap(typeInfo(Particle), f->allocator, Firework_effects_per_rocket)));
        errdefer(ArrList_fini(&f->effects.base));

        f->effect_base_color = Color_intoHsl(effect_base_color);

        return_ok(f);
    }
    scope_returnReserved;
}

void Firework_fini(Firework* f) {
    debug_assert_nonnull(f);

    log_debug("Destroying firework(%p)\n", f);

    if_some(f->rocket, rocket) {
        log_debug("Destroying rocket(%p)\n", rocket);
        mem_Allocator_destroy(f->allocator, anyPtr(rocket));
        log_debug("rocket destroyed\n");
    }

    log_debug("Destroying effects(%p)\n", f->effects.items);
    ArrList_fini(&f->effects.base);
    log_debug("effects destroyed\n");

    log_debug("firework destroyed\n");
}

static bool Firework__deadsAllEffect(const Firework* f) {
    debug_assert_nonnull(f);

    for_slice(f->effects.items, effect) {
        if (Particle_isDead(effect)) { continue; }
        return false;
    }
    return true;
}

bool Firework_isDead(const Firework* f) {
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

Err$void Firework_update(Firework* f, f64 dt) {
    reserveReturn(Err$void);
    debug_assert_nonnull(f);
    unused(dt);

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
                scope_with(let particle = meta_castPtr$(Particle*, try(ArrList_addBackOne(&f->effects.base)))) {
                    let x      = rocket->position[0];
                    let y      = rocket->position[1];
                    let width  = 1.0;
                    let height = 1.0;
                    let color  = Hsl_intoColorOpaque(Hsl_from(
                        f->effect_base_color.h,
                        f->effect_base_color.s + (Random_f64() - 0.5) * 20.0,
                        f->effect_base_color.l + (Random_f64() - 0.5) * 40.0
                    ));
                    Particle_init(particle, x, y, width, height, color);
                    Particle_initWithSpeed(particle, (Random_f64() - 0.5) * 1.0, (Random_f64() - 0.9) * 1.0);
                    Particle_initWithAcceleration(particle, 0.0, 0.02);
                    Particle_initWithFading(particle, 0.01);
                }
            }
            log_debug("destroying rocket(%p)\n", rocket);
            mem_Allocator_destroy(f->allocator, anyPtr(rocket));
            f->rocket = (Opt$Ptr$Particle)none();
            // log_debug("rocket destroyed\n");
        }
    }
    for_slice(f->effects.items, effect) {
        Particle_update(effect, dt);
    }

    return_void();
}

void Firework_render(const Firework* f, engine_Canvas* c, f64 dt) {
    debug_assert_nonnull(f);
    debug_assert_nonnull(c);

    if_some(f->rocket, rocket) {
        debug_assert_fmt(!Particle_isDead(rocket), "rocket must be alive");
        Particle_render(rocket, c, dt);
    }
    for_slice(f->effects.items, effect) {
        Particle_render(effect, c, dt);
    }
}

Err$State State_init(mem_Allocator allocator, u32 width, u32 height) {
    reserveReturn(Err$State);

    var fireworks = typed(ArrList$Firework, try(ArrList_initCap(typeInfo(Firework), allocator, Fireworks_max)));
    return_ok({
        .fireworks  = fireworks,
        .width      = width,
        .height     = height,
        .allocator  = allocator,
        .is_running = true,
    });
}

void State_fini(State* s) {
    debug_assert_nonnull(s);

    for_slice(s->fireworks.items, firework) {
        Firework_fini(firework);
    }
    ArrList_fini(&s->fireworks.base);
}

bool State_isDead(const State* s) {
    debug_assert_nonnull(s);
    return !s->is_running;
}

Err$void State_update(State* s, f64 dt) {
    reserveReturn(Err$void);
    debug_assert_nonnull(s);

    /* Add a new rocket with with 5% chance */
    if (Random_f64() < 0.05) {
        /* NOTE: Rule #0 - Don't put code that causes side effects in debug code like assertions
            What a stupid mistake...
            This explains why the fireworks were automatically triggered in debug mode,
            but not in release mode.
         */
        // debug_only(
        if_some(try(State_spawnFirework(s)), firework) {
            let rocket = unwrap(firework->rocket);
            log_debug("Spawning rocket at (%.2f, %.2f)", rocket->position[0], rocket->position[1]);
        }
        // );
    }

    /* Update all fireworks */
    for_slice(s->fireworks.items, firework) {
        try(Firework_update(firework, dt));
    }

    // Input handling
    if (engine_Key_pressed(engine_KeyCode_esc)) {
        log_debug("pressed esc\n");
        s->is_running = false;
        return_void();
    }

    if (engine_Key_pressed(engine_KeyCode_space)) {
        log_debug("pressed space\n");
        let maybe_firework = catch (State_spawnFirework(s), err, {
            log_error("failed to spawn firework: %s\n", Err_message(err));
            return_err(err);
        });
        if_some(maybe_firework, firework) {
            let rocket = unwrap(firework->rocket);
            log_debug("Spawning rocket at (%.2f, %.2f)", rocket->position[0], rocket->position[1]);
        }
    }

    return_void();
}

void State_render(const State* s, engine_Canvas* c, f64 dt) {
    debug_assert_nonnull(s);
    debug_assert_nonnull(c);

    engine_Canvas_clear(c, Color_black);
    for_slice(s->fireworks.items, firework) {
        Firework_render(firework, c, dt);
    }
}

Err$Opt$Ptr$Firework State_spawnFirework(State* s) {
    reserveReturn(Err$Opt$Ptr$Firework);
    debug_assert_nonnull(s);

    log_debug("Spawning new firework...");
    log_debug("%d fireworks remaining: Removing dead fireworks...", s->fireworks.items.len);
    // Remove dead fireworks.
    for (usize index = 0; index < s->fireworks.items.len; ++index) {
        let firework = &s->fireworks.items.ptr[index];
        if (!Firework_isDead(firework)) { continue; }
        let removed = meta_castPtr$(Firework*, ArrList_removeSwap(&s->fireworks.base, index--));
        Firework_fini(removed);
    }
    log_debug("Removed dead fireworks: %d fireworks remaining", s->fireworks.items.len);

    if (s->fireworks.cap <= s->fireworks.items.len) {
        return_ok(none());
    }

    let firework = meta_castPtr$(Firework*, try(ArrList_addBackOne(&s->fireworks.base)));
    return_ok(some(try(Firework_init(
        firework,
        s->allocator,
        as$(i64, Random_u32() % s->width),
        s->height,
        Color_fromOpaque(Random_u8(), Random_u8(), Random_u8())
    ))));
}
