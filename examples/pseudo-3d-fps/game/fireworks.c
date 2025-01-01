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
        // Initialize platform with terminal backend
        let window = try(engine_Window_create(
            &(engine_PlatformParams){
                .backend_type = engine_RenderBackendType_vt100,
                .window_title = "Fireworks",
                .width        = 80,
                .height       = 50,
            }
        ));
        defer(engine_Window_destroy(window));
        printf("engine initialized\n");

        // Create canvases
        let game_canvas = catch (engine_Canvas_create(80, 50, engine_CanvasType_rgba), err, {
            defer_return;
            return_err(err);
        });
        defer(engine_Canvas_destroy(game_canvas));
        printf("canvas created\n");

        engine_Canvas_clear(game_canvas, Color_blank);
        printf("canvas cleared\n");

        // Add canvas views
        engine_Window_addCanvasView(window, game_canvas, 0, 0, 80, 50);
        printf("canvas views added\n");

        var heap      = (heap_Classic){};
        var allocator = heap_Classic_allocator(&heap);
        var state     = catch (State_init(allocator, 80, 50), err, {
            defer_return;
            return_err(err);
        });
        defer(State_fini(&state));
        printf("game state created\n");
        ignore getchar();

        var curr_time   = time_SysTime_now();
        var prev_time   = curr_time;
        let target_time = time_Duration_fromSecs_f64(0.016f); // Assume 62.5 FPS for simplicity

        // Game loop
        while (state.is_running || !State_isDead(&state)) {
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
                state.is_running = false;
                break;
            }
            if (engine_Key_pressed(engine_KeyCode_Space)) {
                catch (State_spawnFirework(&state), err, {
                    defer_return;
                    return_err(err);
                });
            }
            catch (State_update(&state, as(f32, dt)), err, {
                defer_return;
                return_err(err);
            });

            // Render all views
            State_render(&state, game_canvas, dt);

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
    if (Particle_isDead(p)) { return; }

    let render_color = Color_fromOpaque(
        as(u8, as(f64, p->color.r) * p->lifetime * dt),
        as(u8, as(f64, p->color.g) * p->lifetime * dt),
        as(u8, as(f64, p->color.b) * p->lifetime * dt)
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

    f->allocator = allocator;
    scope_with(let rocket = meta_castPtr(Particle*, try(mem_Allocator_create(f->allocator, typeInfo(Particle))))) {
        Particle_init(rocket, as(f64, rocket_x), as(f64, rocket_y), 1.0, 3.0, effect_base_color);
        Particle_initWithSpeed(rocket, 0.0, -2.0 - Random_f64() * -1.0);
        Particle_initWithAcceleration(rocket, 0.0, 0.02);
        f->rocket = (Opt$Ptr$Particle)some(rocket);
    }
    f->effects           = catch (ArrayList_initCapacity(typeInfo(Particle), f->allocator, Firework_effects_per_rocket), err, {
        mem_Allocator_destroy(f->allocator, AnyType(unwrap(f->rocket)));
        return_err(err);
    });
    f->effect_base_color = Color_intoHsl(effect_base_color);

    return_ok(f);
}

void Firework_fini(Firework* f) {
    debug_assert_nonnull(f);
    if_some(f->rocket, rocket) {
        mem_Allocator_destroy(f->allocator, AnyType(rocket));
    }
    ArrayList_fini(&f->effects);
}

static bool Firework__deadsAllEffect(const Firework* f) {
    debug_assert_nonnull(f);
    let effects = meta_castSli(Sli$Particle, f->effects.items);
    for_slice(effects, effect) {
        if (!Particle_isDead(&effect)) { return false; }
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
            mem_Allocator_destroy(f->allocator, AnyType(rocket));
            f->rocket = (Opt$Ptr$Particle)none();
        }
    }
    scope_with(let effects = meta_castSli(Sli$Particle, f->effects.items)) {
        for_slice_mut(effects, effect) {
            Particle_update(&effect, dt);
        }
    }
    return_void();
}

void Firework_render(const Firework* f, engine_Canvas* c, f64 dt) {
    debug_assert_nonnull(f);
    debug_assert_nonnull(c);

    if_some(f->rocket, rocket) {
        Particle_render(rocket, c, dt);
    }
    scope_with(let effects = meta_castSli(Sli$Particle, f->effects.items)) {
        for_slice(effects, effect) {
            Particle_render(&effect, c, dt);
        }
    }
}

Err$State State_init(mem_Allocator allocator, u32 width, u32 height) {
    reserveReturn(Err$State);

    var fireworks = (ArrayList$Firework)try(ArrayList_initCapacity(typeInfo(Firework), allocator, Fireworks_max));
    catch (
        ArrayList_append(
            &fireworks,
            meta_ptr(&(Firework){
                .rocket            = (Opt$Ptr$Particle)none(),
                .effects           = (ArrayList$Particle)try(ArrayList_initCapacity(typeInfo(Particle), allocator, Firework_effects_per_rocket)),
                .effect_base_color = Color_intoHsl(Color_white),
            })
        ),
        err,
        {
            ArrayList_fini(&fireworks);
            return_err(err);
        }
    );

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
        Firework_fini(&firework);
    }
    ArrayList_fini(&s->fireworks);
}

bool State_isDead(const State* s) {
    debug_assert_nonnull(s);
    let fireworks = meta_castSli(Sli$Firework, s->fireworks.items);
    for_slice(fireworks, firework) {
        if (!Firework_isDead(&firework)) { return false; }
    }
    return true;
}

Err$void State_update(State* s, f64 dt) {
    reserveReturn(Err$void);
    debug_assert_nonnull(s);

    // Add a new rocket with with 5% chance.
    if (Random_f64() < 0.05) {
        try(State_spawnFirework(s));
    }

    // Update all fireworks.
    scope_with(let fireworks = meta_castSli(Sli$Firework, s->fireworks.items)) {
        for_slice_mut(fireworks, firework) {
            try(Firework_update(&firework, dt));
        }
    }

    // Remove dead fireworks.
    for (usize index = 0; index < ArrayList_len(&s->fireworks); ++index) {
        scope_with(let fireworks = meta_castSli(Sli$Firework, s->fireworks.items)) {
            let firework = &fireworks.ptr[index];
            if (!Firework_isDead(firework)) { continue; }
            let removed = meta_castPtr(Firework*, ArrayList_swapRemove(&s->fireworks, index));
            Firework_fini(removed);
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
            Firework_render(&firework, c, dt);
        }
    }
}

Err$Opt$Ptr$Firework State_spawnFirework(State* s) {
    reserveReturn(Err$Opt$Ptr$Firework);
    debug_assert_nonnull(s);

    if (s->fireworks.items.len <= s->fireworks.capacity) {
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
