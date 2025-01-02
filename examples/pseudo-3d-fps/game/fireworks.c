#include <stdio.h>

#include "dh/main.h"
#include "dh/mem.h"
#include "dh/heap/Classic.h"
#include "dh/ArrayList.h"
#include "dh/time/Instant.h"
#include "dh/time/Duration.h"
#include "dh/Random.h"
#include "dh/defer.h"

#include "../engine/include/engine.h"
#include "dh/debug.h"
#include "log.h"

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
using_Opt$(Particle);
using_Err$(Particle);
using_Sli$(Particle);
extern Particle  Particle_new(f64 x, f64 y, f64 width, f64 height, Color color);
extern Particle* Particle_init(Particle* p, f64 x, f64 y, f64 width, f64 height, Color color);
extern Particle* Particle_initWithSpeed(Particle* p, f64 x, f64 y);
extern Particle* Particle_initWithAcceleration(Particle* p, f64 x, f64 y);
extern Particle* Particle_initWithFading(Particle* p, f64 fading);
extern bool      Particle_isDead(const Particle* p);
extern void      Particle_update(Particle* p, f64 dt);
extern void      Particle_render(const Particle* p, engine_Canvas* c, f64 dt);

typedef ArrayList ArrayList$Particle;
typedef struct Firework {
    Opt$Ptr$Particle   rocket;
    ArrayList$Particle effects;
    Hsl                effect_base_color;
    mem_Allocator      allocator;
} Firework;
using_Opt$(Firework);
using_Err$(Firework);
using_Err$(Opt$Ptr$Firework);
using_Sli$(Firework);
extern Err$Ptr$Firework Firework_init(Firework* f, mem_Allocator allocator, i64 rocket_x, i64 rocket_y, Color effect_base_color) must_check;
extern void             Firework_fini(Firework* f);
extern bool             Firework_isDead(const Firework* f);
extern Err$void         Firework_update(Firework* f, f64 dt) must_check;
extern void             Firework_render(const Firework* f, engine_Canvas* c, f64 dt);

typedef ArrayList ArrayList$Firework;
typedef struct State {
    ArrayList$Firework fireworks;
    u32                width;
    u32                height;
    mem_Allocator      allocator;
    bool               is_running;
} State;
using_Err$(State);
extern Err$State            State_init(mem_Allocator allocator, u32 width, u32 height) must_check;
extern void                 State_fini(State* s);
extern bool                 State_isDead(const State* s);
extern Err$void             State_update(State* s, f64 dt) must_check;
extern void                 State_render(const State* s, engine_Canvas* c, f64 dt);
extern Err$Opt$Ptr$Firework State_spawnFirework(State* s) must_check;



Err$void dh_main(int argc, const char* argv[]) {
    reserveReturn(Err$void);
    unused(argc), unused(argv);

    Random_init();
    scope_defer {
        // Initialize logging to a file
        scope_if(let debug_file = fopen("debug.log", "w"), debug_file) {
            log_initWithFile(debug_file);
            // Configure logging behavior
            log_showTimestamp(true);
            log_showLocation(true);
            log_showLevel(true);
            log_setLevel(log_Level_debug);
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
            defer_return;
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
            defer_return;
            return_err(err);
        });
        defer(State_fini(&state));
        log_info("game state created\n");
        ignore getchar();

        var curr_time   = time_SysTime_now();
        var prev_time   = curr_time;
        let target_time = time_Duration_fromSecs_f64(0.016f); // Assume 62.5 FPS for simplicity
        log_info("game loop started\n");

        // Game loop
        while (!State_isDead(&state)) {
            // const f64   real_delta_time = elapsed;
            // const usize update_step     = (usize)(Display_deltaTime() / real_delta_time);
            // for (usize step = 0; step < update_step; ++step) {
            //     const f64 delta_time = (f64)step * real_delta_time;

            curr_time        = time_SysTime_now();
            let elapsed_time = time_SysTime_durationSince(curr_time, prev_time);
            let dt           = time_Duration_asSecs_f64(elapsed_time);

            // Process events
            engine_Window_processEvents(window);

            // Update game state
            if (engine_Key_pressed(engine_KeyCode_Esc)) {
                log_debug("pressed esc\n");
                state.is_running = false;
                break;
            }
            if (engine_Key_pressed(engine_KeyCode_Space)) {
                log_debug("pressed space\n");
                catch (State_spawnFirework(&state), err, {
                    log_error("failed to spawn firework: %s\n", Err_message(err));
                    defer_return;
                    return_err(err);
                });
            }
            catch (State_update(&state, 1), err, {
                log_error("failed to update game state: %s\n", Err_message(err));
                defer_return;
                return_err(err);
            });

            // Render all views
            State_render(&state, game_canvas, 1);

            // Present to screen
            engine_Window_present(window);

            // Sleep for the remaining time to maintain FPS
            time_SysTime_sleep(time_Duration_sub(target_time, elapsed_time));
            prev_time = curr_time;
        }
    }
    scope_deferred;

    return_void();
}



Particle Particle_new(f64 x, f64 y, f64 width, f64 height, Color color) {
    return make(
        Particle,
        .position     = { x, y },
        .speed        = { 0.0, 0.0 },
        .acceleration = { 0.0, 0.0 },
        .fading       = 0.0,
        .lifetime     = 1.0,
        .color        = color,
        .dimensions   = { width, height }
    );
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
    if (Particle_isDead(p)) { return; }

    p->speed[0] += p->acceleration[0] * dt;
    p->speed[1] += p->acceleration[1] * dt;

    p->position[0] += p->speed[0] * dt;
    p->position[1] += p->speed[1] * dt;

    p->lifetime -= p->fading * dt;
}

void Particle_render(const Particle* p, engine_Canvas* c, f64 dt) {
    debug_assert_nonnull(p);
    debug_assert_nonnull(c);
    unused(dt);
    if (Particle_isDead(p)) { return; }

    let render_color = Color_fromOpaque(
        as(u8, as(f64, p->color.r) * p->lifetime),
        as(u8, as(f64, p->color.g) * p->lifetime),
        as(u8, as(f64, p->color.b) * p->lifetime)
    );
    engine_Canvas_fillRect(
        c,
        as(i64, p->position[0]),
        as(i64, p->position[1]),
        as(u32, p->dimensions[0]),
        as(u32, p->dimensions[1]),
        render_color
    );
}

Err$Ptr$Firework Firework_init(Firework* f, mem_Allocator allocator, i64 rocket_x, i64 rocket_y, Color effect_base_color) {
    reserveReturn(Err$Ptr$Firework);
    debug_assert_nonnull(f);

    log_debug("Initializing firework(%p) at (%d, %d)\n", f, rocket_x, rocket_y);
    f->allocator = allocator;
    scope_with(let rocket = meta_castPtr(Particle*, try(mem_Allocator_create(f->allocator, typeInfo(Particle))))) {
        Particle_init(rocket, as(f64, rocket_x), as(f64, rocket_y), 1.0, 3.0, effect_base_color);
        Particle_initWithSpeed(rocket, 0.0, -2.0 - Random_f64() * -1.0);
        Particle_initWithAcceleration(rocket, 0.0, 0.02);
        f->rocket = (Opt$Ptr$Particle)some(rocket);
    }
    f->effects           = catch (ArrayList_initCapacity(typeInfo(Particle), f->allocator, Firework_effects_per_rocket), err, {
        log_error("failed to create effects array: %s\n", Err_message(err));
        let rocket = unwrap(f->rocket);
        mem_Allocator_destroy(f->allocator, (AnyType){ .ctx = (void*)&rocket, .type = typeInfo(Particle) });
        return_err(err);
    });
    f->effect_base_color = Color_intoHsl(effect_base_color);

    return_ok(f);
}

void Firework_fini(Firework* f) {
    debug_assert_nonnull(f);
    log_debug("Destroying firework(%p)\n", f);
    if_some(f->rocket, rocket) {
        log_debug("Destroying rocket(%p)\n", rocket);
        mem_Allocator_destroy(f->allocator, (AnyType){ .ctx = (void*)&rocket, .type = typeInfo(Particle) });
    }
    ArrayList_fini(&f->effects);
}

static bool Firework__deadsAllEffect(const Firework* f) {
    debug_assert_nonnull(f);
    let effects = meta_castSli(Sli$Particle, f->effects.items);
    for_slice(effects, effect) {
        if (Particle_isDead(effect)) { continue; }
        return false;
    }
    return true;
}

bool Firework_isDead(const Firework* f) {
    debug_assert_nonnull(f);
    if (f->rocket.has_value) { return false; }
    if (f->effects.items.len != 0) { return false; }
    if (!Firework__deadsAllEffect(f)) { return false; }
    return true;
}

Err$void Firework_update(Firework* f, f64 dt) {
    reserveReturn(Err$void);
    debug_assert_nonnull(f);

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
                scope_with(let particle = meta_castPtr(Particle*, try(ArrayList_addOne(&f->effects)))) {
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
            mem_Allocator_destroy(f->allocator, (AnyType){ .ctx = (void*)&rocket, .type = typeInfo(Particle) });
            log_debug("rocket destroyed\n");
            f->rocket = (Opt$Ptr$Particle)none();
        }
    }
    scope_with(let effects = meta_castSli(Sli$Particle, f->effects.items)) {
        for_slice_mut(effects, effect) {
            Particle_update(effect, dt);
        }
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
    scope_with(let effects = meta_castSli(Sli$Particle, f->effects.items)) {
        for_slice(effects, effect) {
            Particle_render(effect, c, dt);
        }
    }
}

Err$State State_init(mem_Allocator allocator, u32 width, u32 height) {
    reserveReturn(Err$State);

    var fireworks = (ArrayList$Firework)try(ArrayList_initCapacity(typeInfo(Firework), allocator, Fireworks_max));
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

    let fireworks = meta_castSli(Sli$Firework, s->fireworks.items);
    for_slice_mut(fireworks, firework) {
        Firework_fini(firework);
    }
    ArrayList_fini(&s->fireworks);
}

bool State_isDead(const State* s) {
    debug_assert_nonnull(s);
    // let fireworks = meta_castSli(Sli$Firework, s->fireworks.items);
    // for_slice(fireworks, firework) {
    //     if (!Firework_isDead(firework)) { return false; }
    // }
    // return true;
    return !s->is_running;
}

Err$void State_update(State* s, f64 dt) {
    reserveReturn(Err$void);
    debug_assert_nonnull(s);

    // Add a new rocket with with 5% chance.
    if (Random_f64() < 0.05) {
        debug_only(
            if_some(try(State_spawnFirework(s)), firework) {
                let rocket = unwrap(firework->rocket);
                log_debug("Spawning rocket at (%.2f, %.2f)", rocket->position[0], rocket->position[1]);
            }
        );
    }

    // Update all fireworks.
    scope_with(let fireworks = meta_castSli(Sli$Firework, s->fireworks.items)) {
        for_slice_mut(fireworks, firework) {
            try(Firework_update(firework, dt));
        }
    }

    return_void();
}

void State_render(const State* s, engine_Canvas* c, f64 dt) {
    debug_assert_nonnull(s);
    debug_assert_nonnull(c);

    engine_Canvas_clear(c, Color_black);
    scope_with(let fireworks = meta_castSli(Sli$Firework, s->fireworks.items)) {
        for_slice(fireworks, firework) {
            Firework_render(firework, c, dt);
        }
    }
}

Err$Opt$Ptr$Firework State_spawnFirework(State* s) {
    reserveReturn(Err$Opt$Ptr$Firework);
    debug_assert_nonnull(s);

    log_debug("Spawning new firework...");
    log_debug("%d fireworks remaining: Removing dead fireworks...", s->fireworks.items.len);
    // Remove dead fireworks.
    for (usize index = 0; index < ArrayList_len(&s->fireworks); ++index) {
        scope_with(let fireworks = meta_castSli(Sli$Firework, s->fireworks.items)) {
            let firework = &fireworks.ptr[index];
            if (!Firework_isDead(firework)) { continue; }
            let removed = meta_castPtr(Firework*, ArrayList_swapRemove(&s->fireworks, index--));
            Firework_fini(removed);
        }
    }
    log_debug("Removed dead fireworks: %d fireworks remaining", s->fireworks.items.len);

    if (s->fireworks.capacity <= s->fireworks.items.len) {
        return_ok(none());
    }

    let firework = meta_castPtr(Firework*, try(ArrayList_addOne(&s->fireworks)));
    return_ok(some(try(Firework_init(
        firework,
        s->allocator,
        as(i64, Random_u32() % s->width),
        s->height,
        Color_fromOpaque(Random_u8(), Random_u8(), Random_u8())
    ))));
}
