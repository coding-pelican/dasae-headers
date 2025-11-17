#include "dh/prl.h"
#include "dh/Thrd.h"

#define mp_max_thrd_count (32)

$static var_(mp__thrd_count, usize) = 3;
$static fn_((mp_setThrdCount(usize thrd_count))(void)) { mp__thrd_count = thrd_count; }
$static fn_((mp_getThrdCount(void))(usize)) { return mp__thrd_count; }

typedef fn_(((*)(usize, u_V$raw))(void) $T) mp_LoopFn;
typedef struct mp_LoopData {
    var_(range, R);
    var_(workerFn, mp_LoopFn);
    var_(params, u_V$raw);
} mp_LoopData;

$static Thrd_fn_(mp_worker, ({ mp_LoopData data; }, Void), ($ignore, args) $scope) {
    let data = args->data;
    for_(((data.range))(i) {
        data.workerFn(i, data.params);
    });
    return_({});
} $unscoped_(Thrd_fn);

$static fn_((mp_parallel_for(R range, mp_LoopFn workerFn, u_V$raw params))(void)) {
    let thrd_count = mp_getThrdCount();

    var_(data_list_buf, A$$(mp_max_thrd_count, mp_LoopData)) = zero$A();
    let data_list = slice$A(data_list_buf, $r(0, thrd_count));
    var_(workers_buf, A$$(mp_max_thrd_count, Thrd_FnCtx$(mp_worker))) = zero$A();
    let workers = slice$A(workers_buf, $r(0, thrd_count));
    var_(threads_buf, A$$(mp_max_thrd_count, Thrd)) = zero$A();
    let threads = slice$A(threads_buf, $r(0, thrd_count));

    let chunk = (range.end - range.begin + thrd_count - 1) / thrd_count;
    for_(($rf(0), $s(workers), $s(data_list))(t, worker, data) {
        data->range.begin = range.begin + chunk * t;
        data->range.end = ((data->range.begin + chunk) <= range.end) ? (data->range.begin + chunk) : range.end;
        data->workerFn = workerFn;
        data->params = params;
        *worker = Thrd_FnCtx_from$((mp_worker)(*data));
    });
    for_(($s(threads), $s(workers))(thread, worker) {
        *thread = catch_((Thrd_spawn(Thrd_SpawnConfig_default, worker->as_raw))(
            $ignore, claim_unreachable
        ));
    });
    for_(($s(threads))(thread) {
        Thrd_join(*thread);
    });
}

// ============================================
// 유틸리티
// ============================================

#include "dh/math/common.h"
#include "dh/math/vec.h"
#include "dh/Rand.h"

typedef struct RandGaussian {
    Rand rng $like_ref;
    O$f64 spare;
} RandGaussian;
$static let_(RandGaussian_default, RandGaussian) = {
    .rng $like_deref = Rand_default,
    .spare = none(),
};
$static fn_((RandGaussian_init(Rand rng))(RandGaussian)) {
    return lit$((RandGaussian){
        .rng $like_deref = rng,
        .spare = none(),
    });
}
$static fn_((RandGaussian_next$f64(RandGaussian* self, f64 mean, f64 std_dev))(f64)) {
    if_some((self->spare)(value)) {
        return mean + std_dev * value;
    }

    f64 u = f64_nan;
    f64 v = f64_nan;
    f64 s = f64_nan;
    do {
        u = (Rand_next$f64(self->rng) * 2.0 - 1.0);
        v = (Rand_next$f64(self->rng) * 2.0 - 1.0);
        s = u * u + v * v;
    } while (s >= 1.0 || s == 0.0);
    s = flt_sqrt(-2.0 * flt_ln(s) / s);
    asg_lit((&self->spare)(some(v * s)));
    return mean + std_dev * u * s;
}

// ============================================
// 시뮬레이션 설정
// ============================================

#define State_particles_log2 (20ull)
#define State_particles      (1ull << State_particles_log2) // 2^20 = 1,048,576
#define State_cell_size              (10.0f)
#define State_grid_width             (200ull)
#define State_grid_height            (200ull)
#define State_max_particles_per_cell (32ull)

#define State_boundary_radius    (500.0)
#define State_gravity            (9.81)
#define State_damping            (0.98)
#define State_target_fps         (30.0)
#define State_delta_time         (1.0 / 30.0)

typedef struct Particle {
    m_V2f64 pos;
    m_V2f64 vel;
    f64     mass;
} Particle;
T_use_S$(Particle);

typedef struct Cell {
    var_(count, usize);
    var_(indices, A$$(State_max_particles_per_cell, usize));
} Cell;
T_use_S$(Cell);

typedef struct State {
    S$Particle particles;
    S$Cell grid;
} State;

#define State_enable_randomization 0

$static fn_((State_rng(void))(Rand*));
$static fn_((State_rngGaussian(void))(RandGaussian*));
$static fn_((State_initParticles_worker(usize i, u_V$raw params))(void));
$static fn_((State_initParticles(S$Particle particles, m_V2f64 center, m_V2f64 radius_a_b))(void));
$static fn_((State_init(S$Particle particles, S$Cell grid))(State)) {
    *State_rng() = pp_if_(State_enable_randomization)(
        pp_then_(Rand_init()),
        pp_else_(Rand_default)
    );
    *State_rngGaussian() = RandGaussian_init(*State_rng());
    State_initParticles(particles, m_V2f64_zero, m_V2f64_from(State_boundary_radius, 200.0));
    return lit$((State){
        .particles = particles,
        .grid = grid,
    });
}

// ============================================
// Spatial Grid 구축
// ============================================

$static fn_((State_clearGrid_worker(usize i, u_V$raw params))(void));
$static fn_((State_clearGrid(S$Cell grid))(void));
$static fn_((State_hashPosition(m_V2f64 pos))(usize));
$static fn_((State_buildSpatialGrid(State* self))(void)) {
    State_clearGrid(self->grid);

    for_(($r(0, State_particles),$s(self->particles))(i, particle) {
        let cell_idx = State_hashPosition(particle->pos);

        let pos = atom_fetchAdd(&S_at((self->grid)[cell_idx])->count, 1, atom_MemOrd_release);
        if (pos < State_max_particles_per_cell) {
            *A_at((S_at((self->grid)[cell_idx])->indices)[pos]) = i;
        }
    });
}

// ============================================
// 중력 적용
// ============================================

$static fn_((State_applyGravity_worker(usize i, u_V$raw params))(void));
$static fn_((State_applyGravity(State* self))(void));

// ============================================
// 충돌 처리
// ============================================

$static fn_((State_handleCollisions_worker(usize i, u_V$raw params))(void));
$static fn_((State_handleCollisions(State* self))(void));

// ============================================
// 위치 업데이트 및 경계 처리
// ============================================

$static fn_((State_updatePositions_worker(usize i, u_V$raw params))(void));
$static fn_((State_updatePositions(State* self))(void));

// ============================================
// 시뮬레이션 루프
// ============================================

#include "dh/io/stream.h"
#include "dh/time/Instant.h"

$static fn_((State_simulate(State* self, usize frame_amount))(void)) {
    io_stream_println(u8_l("\nStarting simulation for {:uz} frames at {:.1fl} FPS..."), frame_amount, State_target_fps);
    io_stream_println(u8_l("Using {:uz} threads (MP)"), mp_getThrdCount());

    f64 total_time = 0.0;
    for_(($r(0, frame_amount))(frame) {
        let frame_start = time_Instant_now();
        State_buildSpatialGrid(self);
        State_applyGravity(self);
        State_handleCollisions(self);
        State_updatePositions(self);
        let frame_end = time_Instant_now();
        let frame_time = time_Instant_durationSince(frame_end, frame_start);
        total_time += time_Duration_asSecs$f64(frame_time);
        if (frame % as$(usize)(State_target_fps) == 0) {
            io_stream_println(
                u8_l("Frame {:uz}: {:.2fl} ms ({:.1fl} FPS) | Avg: {:.2fl} ms ({:.1fl} FPS)"),
                frame, time_Duration_asSecs$f64(frame_time) * 1000.0, 1.0 / time_Duration_asSecs$f64(frame_time),
                (total_time / (frame + 1)) * 1000.0, (frame + 1) / total_time
            );
        }
    });

    let avg_fps = as$(f64)(frame_amount) / total_time;
    let avg_spf = total_time / as$(f64)(frame_amount);
    io_stream_println(u8_l("\n=== Simulation Complete ===\n"));
    io_stream_println(u8_l("Total frames: {:uz}"), frame_amount);
    io_stream_println(u8_l("Total time: {:.2fl} seconds"), total_time);
    io_stream_println(u8_l("Average FPS: {:.2fl}"), avg_fps);
    io_stream_println(u8_l("Average frame time: {:.2fl} ms"), avg_spf * 1000.0);
    if (avg_fps >= State_target_fps) {
        io_stream_println(u8_l("✓ SUCCESS: {:.2fl} FPS achieved!"), State_target_fps);
    } else {
        io_stream_println(u8_l("Target {:.2fl} FPS not achieved ({:.2fl} FPS)"), State_target_fps, avg_fps);
    }
}

// ============================================
// 메인
// ============================================

#include "dh/main.h"
#include "dh/fmt/common.h"
#include "dh/heap/Page.h"
#include "dh/io/common.h"

fn_((dh_main(S$S_const$u8 args))(E$void) $guard) {
    io_stream_println(u8_l("╔═══════════════════════════════════════╗"));
    io_stream_println(u8_l("║  Particle Simulation                  ║"));
    io_stream_println(u8_l("║  Thrd + atom                          ║"));
    io_stream_println(u8_l("╚═══════════════════════════════════════╝\n"));

    io_stream_println(u8_l("Particles: 2^{:uz} = {:uz}"), State_particles_log2, State_particles);
    io_stream_println(u8_l("Target FPS: {:.1f}"), State_target_fps);
    let max_cpu_count = usize_subSat(catch_((Thrd_getCpuCount())($ignore, 2)), 2) + 1;
    let cpu_count = prim_clamp(expr_(usize $scope) if (1 < args.len) {
        $break_(catch_((fmt_parse$usize(*S_at((args)[1]), 10))($ignore, usize_limit_max)));
    } else {
        $break_(usize_limit_max);
    } $unscoped_(expr), 1, max_cpu_count);
    mp_setThrdCount(cpu_count);
    io_stream_println(u8_l("Using {:uz} threads (MP)"), mp_getThrdCount());

    io_stream_println(u8_l("\nAllocating memory..."));
    var page = lit$((heap_Page){});
    let gpa = heap_Page_allocator(&page);
    let particles = try_(u_castE$((E$$(S$Particle))(mem_Allocator_alloc(gpa, typeInfo$(Particle), State_particles))));
    defer_(mem_Allocator_free(gpa, u_anyS(particles)));
    let grid = try_(u_castE$((E$$(S$Cell))(mem_Allocator_alloc(gpa, typeInfo$(Cell), State_grid_width * State_grid_height))));
    defer_(mem_Allocator_free(gpa, u_anyS(grid)));
    io_stream_println(u8_l("Memory allocated: {:.2fl} MB"), as$(f64)(State_particles * typeInfo$(Particle).size) / (1024.0 * 1024.0));

    var state = State_init(particles, grid);
    let frame_amount = expr_(usize $scope) if (2 < args.len) {
        $break_(catch_((fmt_parse$usize(*S_at((args)[2]), 10))($ignore, 1000)));
    } else {
        $break_(1000);
    } $unscoped_(expr);
    io_stream_println(u8_l("Simulating {:uz} frames..."), frame_amount);
    State_simulate(&state, frame_amount);

    io_stream_println(u8_l("\nSample particles:"));
    for_(($r(0, 3), $s(particles))(i, particle){
        io_stream_println(
            u8_l("- Particle {:uz}: pos({:.2fl}, {:.2fl}) vel({:.2fl}, {:.2fl})"),
            i, particle->pos.x, particle->pos.y, particle->vel.x, particle->vel.y
        );
    });
    io_stream_println(u8_l("\n=== Program Complete ==="));
    return_ok({});
} $unguarded_(fn);

// ============================================
// 구현
// ============================================

fn_((State_rng(void))(Rand*)) {
    static $Thrd_local Rand rng = Rand_default;
    return &rng;
}

fn_((State_rngGaussian(void))(RandGaussian*)) {
    static $Thrd_local RandGaussian rng = RandGaussian_default;
    return &rng;
}

typedef struct State_InitParticlesArgs {
    S$Particle particles;
    m_V2f64 center;
    m_V2f64 radius_a_b;
} State_InitParticlesArgs;

fn_((State_initParticles_worker(usize i, u_V$raw params))(void)) {
    static $Thrd_local bool c_initialized = false;
    if (!c_initialized) {
        let seed = Thrd_getCurrentId() ^ i;
        *State_rng() = pp_if_(State_enable_randomization)(
            pp_then_(Rand_initSeed(seed)),
            pp_else_(Rand_withSeed(Rand_default, seed))
        );
        *State_rngGaussian() = RandGaussian_init(*State_rng());
        c_initialized = true;
    }
    let args = u_castV$((State_InitParticlesArgs)(params));
    let particles = args.particles;
    let particle_i = S_at((particles)[i]);
    var particle = *particle_i;
    let c = args.center;
    let r_a_b = args.radius_a_b;
    let r_a = r_a_b.x;
    let r_b = r_a_b.y;

    var r = RandGaussian_next$f64(State_rngGaussian(), 0.0, r_b / 2.5);
    if (r > r_b) { r = r_b; }
    if (r < 0) { r = -r; }
    let theta = m_V2f64_sincos(Rand_next$f64(State_rng()) * math_f64_tau);
    var p = m_V2f64_mul(m_V2f64_addScalar(c, r), theta);
    let d = m_V2f64_len(p);
    if (d > r_a) {
        m_V2f64_scaleAsg(&p, r_a / d);
    }
    particle.pos = p;
    particle.vel = m_V2f64_zero;
    particle.mass = 1.0;

    *particle_i = particle;
}

fn_((State_initParticles(S$Particle particles, m_V2f64 center, m_V2f64 radius_a_b))(void)) {
    mp_parallel_for($r(0, State_particles), State_initParticles_worker, u_anyV(lit$((State_InitParticlesArgs){
        .particles = particles,
        .center = center,
        .radius_a_b = radius_a_b,
    })));
}

typedef struct State_ClearGridArgs {
    S$Cell grid;
} State_ClearGridArgs;

fn_((State_clearGrid_worker(usize i, u_V$raw params))(void)) {
    let args = u_castV$((State_ClearGridArgs)(params));
    let grid = args.grid;
    let cell = S_at((grid)[i]);
    atom_store(&cell->count, 0ull, atom_MemOrd_release);
}

fn_((State_clearGrid(S$Cell grid))(void)) {
    mp_parallel_for($r(0, State_grid_width * State_grid_height), State_clearGrid_worker, u_anyV(lit$((State_ClearGridArgs){
        .grid = grid,
    })));
}

fn_((State_hashPosition(m_V2f64 pos))(usize)) {
    let gx_0 = as$(isize)((pos.x + (State_grid_width * State_cell_size) / 2.0f) / State_cell_size);
    let gy_0 = as$(isize)((pos.y + (State_grid_height * State_cell_size) / 2.0f) / State_cell_size);
    let gx_1 = as$(usize)(prim_clamp(gx_0, 0, as$(isize)(State_grid_width) - 1));
    let gy_1 = as$(usize)(prim_clamp(gy_0, 0, as$(isize)(State_grid_height) - 1));
    return gy_1 * State_grid_width + gx_1;
}

typedef struct State_ApplyGravityArgs {
    S$Particle particles;
} State_ApplyGravityArgs;

fn_((State_applyGravity_worker(usize i, u_V$raw params))(void)) {
    let args = u_castV$((State_ApplyGravityArgs)(params));
    let particles = args.particles;
    let particle_i = S_at((particles)[i]);
    var particle = *particle_i;

    let dp = m_V2f64_neg(particle.pos);
    let d = m_V2f64_len(dp);
    let d_sq = d * d;
    if (d > 0.1f) {
        let force = State_gravity / d_sq;
        m_V2f64_addAsg(&particle.vel, m_V2f64_scale(dp, (1.0 / d) * force * State_delta_time));
        // particle->vel.x += (dx / dist) * force * State_delta_time;
        // particle->vel.y += (dy / dist) * force * State_delta_time;
    }
    m_V2f64_scaleAsg(&particle.vel, State_damping);

    *particle_i = particle;
}

fn_((State_applyGravity(State* self))(void)) {
    mp_parallel_for($r(0, State_particles), State_applyGravity_worker, u_anyV(lit$((State_ApplyGravityArgs){
        .particles = self->particles,
    })));
}

typedef struct State_HandleCollisionsArgs {
    S$Particle particles;
    S$Cell grid;
} State_HandleCollisionsArgs;

fn_((State_handleCollisions_worker(usize i, u_V$raw params))(void)) {
    let args = u_castV$((State_HandleCollisionsArgs)(params));
    let particles = args.particles;
    let particle_i = S_at((particles)[i]);
    var particle = *particle_i;
    let grid = args.grid;
    let collision_radius = 0.5;

    let cell_idx = State_hashPosition(particle.pos);
    let gx = cell_idx % State_grid_width;
    let gy = cell_idx / State_grid_width;

    for_(($r(0, 3))(dy_offset){  // 0, 1, 2
        for_(($r(0, 3))(dx_offset){  // 0, 1, 2
            let dx = as$(isize)(dx_offset) - 1;  // Convert to -1, 0, 1
            let dy = as$(isize)(dy_offset) - 1;

            let nx = as$(isize)(gx) + dx;
            let ny = as$(isize)(gy) + dy;

            if ((nx < 0 || State_grid_width <= as$(usize)(nx)) ||
                (ny < 0 || State_grid_height <= as$(usize)(ny))) {
                continue;
            }

            let neighbor_idx = as$(usize)(ny) * State_grid_width + as$(usize)(nx);
            let neighbor = S_at((grid)[neighbor_idx]);
            let count = atom_load(&neighbor->count, atom_MemOrd_acquire);

            for (usize k = 0; k < count && k < State_max_particles_per_cell; ++k) {
                let j = *A_at((neighbor->indices)[k]);
                if (j <= i) { continue; }

                let self = &particle;
                let other = *S_at((particles)[j]);
                let dp_col = m_V2f64_sub(other.pos, self->pos);
                let d_sq = m_V2f64_lenSq(dp_col);
                let min_d = collision_radius * 2.0f;
                let min_d_sq = min_d * min_d;
                if (d_sq < min_d_sq && 0.001f < d_sq) {
                    let d = flt_sqrt(d_sq);
                    let overlap = min_d - d;
                    let n_norm = m_V2f64_scaleInv(dp_col, d);
                    let separation = overlap * 0.5f;
                    m_V2f64_subAsg(&self->pos, m_V2f64_scale(n_norm, separation));
                }
            }
        });
    });

    *particle_i = particle;
}

fn_((State_handleCollisions(State* self))(void)) {
    mp_parallel_for($r(0, State_particles), State_handleCollisions_worker, u_anyV(lit$((State_HandleCollisionsArgs){
        .particles = self->particles,
        .grid = self->grid,
    })));
}

typedef struct State_UpdatePositionsArgs {
    S$Particle particles;
} State_UpdatePositionsArgs;

fn_((State_updatePositions_worker(usize i, u_V$raw params))(void)) {
    let args = u_castV$((State_UpdatePositionsArgs)(params));
    let particles = args.particles;
    let particle_i = S_at((particles)[i]);
    var particle = *particle_i;

    m_V2f64_addAsg(&particle.pos, m_V2f64_scale(particle.vel, State_delta_time));
    let d = m_V2f64_len(particle.pos);
    if (d > State_boundary_radius) {
        let n = m_V2f64_scaleInv(particle.pos, d);
        particle.pos = m_V2f64_scale(n, State_boundary_radius);
        let dot = m_V2f64_dot(particle.vel, n);
        m_V2f64_subAsg(&particle.vel, m_V2f64_scale(n, dot * 2.0f));
        m_V2f64_scaleAsg(&particle.vel, 0.8);
    }

    *particle_i = particle;
}

fn_((State_updatePositions(State* self))(void)) {
    mp_parallel_for($r(0, State_particles), State_updatePositions_worker, u_anyV(lit$((State_UpdatePositionsArgs){
        .particles = self->particles,
    })));
}
