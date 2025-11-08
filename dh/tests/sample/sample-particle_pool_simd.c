#include "dh/Thrd/common.h"
#include "dh/prl.h"
#include "dh/Thrd.h"
#include "dh/atom.h"

// ============================================
// COMPLETE SIMD IMPLEMENTATION
// SSE2, AVX2, NEON support with fallback
// ============================================

#include <math.h>

// ============================================
// SIMD Detection and Configuration
// ============================================

#if defined(__AVX2__)
#include <immintrin.h>
#define SIMD_BACKEND "AVX2"
#define SIMD_WIDTH   8
#define SIMD_ALIGN   32
typedef __m256 simd_f32;
typedef __m256i simd_i32;
#elif defined(__SSE2__) || defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#include <emmintrin.h> // SSE2
#include <xmmintrin.h> // SSE
#define SIMD_BACKEND "SSE2"
#define SIMD_WIDTH   4
#define SIMD_ALIGN   16
typedef __m128 simd_f32;
typedef __m128i simd_i32;
#elif defined(__ARM_NEON) || defined(__aarch64__)
#include <arm_neon.h>
#define SIMD_BACKEND "NEON"
#define SIMD_WIDTH   4
#define SIMD_ALIGN   16
typedef float32x4_t simd_f32;
typedef int32x4_t simd_i32;
#else
#define SIMD_BACKEND "SCALAR"
#define SIMD_WIDTH   1
#define SIMD_ALIGN   4
typedef float simd_f32;
typedef int simd_i32;
#endif

// ============================================
// SIMD Abstraction Layer
// ============================================

// Load/Store operations
#if defined(__AVX2__)
#define simd_load(ptr)     _mm256_loadu_ps(ptr)
#define simd_store(ptr, v) _mm256_storeu_ps(ptr, v)
#define simd_set1(x)       _mm256_set1_ps(x)
#define simd_setzero()     _mm256_setzero_ps()
#elif defined(__SSE2__) || defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#define simd_load(ptr)     _mm_loadu_ps(ptr)
#define simd_store(ptr, v) _mm_storeu_ps(ptr, v)
#define simd_set1(x)       _mm_set1_ps(x)
#define simd_setzero()     _mm_setzero_ps()
#elif defined(__ARM_NEON) || defined(__aarch64__)
#define simd_load(ptr)     vld1q_f32(ptr)
#define simd_store(ptr, v) vst1q_f32(ptr, v)
#define simd_set1(x)       vdupq_n_f32(x)
#define simd_setzero()     vdupq_n_f32(0.0f)
#else
#define simd_load(ptr)     (*(ptr))
#define simd_store(ptr, v) (*(ptr) = (v))
#define simd_set1(x)       (x)
#define simd_setzero()     (0.0f)
#endif

// Arithmetic operations
#if defined(__AVX2__)
#define simd_add(a, b)       _mm256_add_ps(a, b)
#define simd_sub(a, b)       _mm256_sub_ps(a, b)
#define simd_mul(a, b)       _mm256_mul_ps(a, b)
#define simd_div(a, b)       _mm256_div_ps(a, b)
#define simd_sqrt(a)         _mm256_sqrt_ps(a)
#define simd_fmadd(a, b, c)  _mm256_fmadd_ps(a, b, c)  // a*b + c
#define simd_fnmadd(a, b, c) _mm256_fnmadd_ps(a, b, c) // -(a*b) + c
#elif defined(__SSE2__) || defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#define simd_add(a, b)       _mm_add_ps(a, b)
#define simd_sub(a, b)       _mm_sub_ps(a, b)
#define simd_mul(a, b)       _mm_mul_ps(a, b)
#define simd_div(a, b)       _mm_div_ps(a, b)
#define simd_sqrt(a)         _mm_sqrt_ps(a)
// SSE2 doesn't have FMA, emulate it
#define simd_fmadd(a, b, c)  _mm_add_ps(_mm_mul_ps(a, b), c)
#define simd_fnmadd(a, b, c) _mm_sub_ps(c, _mm_mul_ps(a, b))
#elif defined(__ARM_NEON) || defined(__aarch64__)
#define simd_add(a, b)       vaddq_f32(a, b)
#define simd_sub(a, b)       vsubq_f32(a, b)
#define simd_mul(a, b)       vmulq_f32(a, b)
#define simd_div(a, b)       vdivq_f32(a, b)
#define simd_sqrt(a)         vsqrtq_f32(a)
#define simd_fmadd(a, b, c)  vfmaq_f32(c, a, b) // c + a*b
#define simd_fnmadd(a, b, c) vfmsq_f32(c, a, b) // c - a*b
#else
#define simd_add(a, b)       ((a) + (b))
#define simd_sub(a, b)       ((a) - (b))
#define simd_mul(a, b)       ((a) * (b))
#define simd_div(a, b)       ((a) / (b))
#define simd_sqrt(a)         sqrtf(a)
#define simd_fmadd(a, b, c)  ((a) * (b) + (c))
#define simd_fnmadd(a, b, c) ((c) - (a) * (b))
#endif

// Comparison and blend operations
#if defined(__AVX2__)
#define simd_cmp_gt(a, b)      _mm256_cmp_ps(a, b, _CMP_GT_OQ)
#define simd_cmp_lt(a, b)      _mm256_cmp_ps(a, b, _CMP_LT_OQ)
#define simd_blend(a, b, mask) _mm256_blendv_ps(a, b, mask)
#define simd_movemask(a)       _mm256_movemask_ps(a)
#elif defined(__SSE2__) || defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#define simd_cmp_gt(a, b)      _mm_cmpgt_ps(a, b)
#define simd_cmp_lt(a, b)      _mm_cmplt_ps(a, b)
#define simd_blend(a, b, mask) _mm_or_ps(_mm_andnot_ps(mask, a), _mm_and_ps(mask, b))
#define simd_movemask(a)       _mm_movemask_ps(a)
#elif defined(__ARM_NEON) || defined(__aarch64__)
#define simd_cmp_gt(a, b)      vcgtq_f32(a, b)
#define simd_cmp_lt(a, b)      vcltq_f32(a, b)
#define simd_blend(a, b, mask) vbslq_f32(mask, b, a)
// NEON doesn't have movemask, need to check differently
static inline int simd_movemask(simd_f32 a) {
    uint32x4_t mask = vreinterpretq_u32_f32(a);
    uint32_t result = 0;
    result |= (vgetq_lane_u32(mask, 0) >> 31) << 0;
    result |= (vgetq_lane_u32(mask, 1) >> 31) << 1;
    result |= (vgetq_lane_u32(mask, 2) >> 31) << 2;
    result |= (vgetq_lane_u32(mask, 3) >> 31) << 3;
    return result;
}
#else
#define simd_cmp_gt(a, b)      ((a) > (b))
#define simd_cmp_lt(a, b)      ((a) < (b))
#define simd_blend(a, b, mask) ((mask) ? (b) : (a))
#define simd_movemask(a)       ((a) != 0.0f)
#endif

// ============================================
// Build Flags for Maximum Performance
// ============================================

/*
GCC/Clang:
gcc -O3 -march=native -mavx2 -ffast-math -fopenmp sample-particle-optimized.c

Explanation:
-O3           : Maximum optimization
-march=native : Use all CPU instructions available
-mavx2        : Enable AVX2 SIMD (x86)
-ffast-math   : Fast floating point (relaxed IEEE)
-fopenmp      : OpenMP support (optional)

ARM (NEON):
gcc -O3 -mcpu=native -mfpu=neon -ffast-math

MSVC:
cl /O2 /arch:AVX2 /fp:fast sample-particle-optimized.c
*/

// ============================================
// Usage Example
// ============================================

/*
// In dh_main, print which SIMD backend is being used:
io_stream_println(u8_l("SIMD Backend: " SIMD_BACKEND));
io_stream_println(u8_l("SIMD Width: {:uz} floats"), SIMD_WIDTH);

// Convert AoS to SoA for better SIMD performance:
typedef struct ParticlesSoA {
    S$f32 pos_x;
    S$f32 pos_y;
    S$f32 vel_x;
    S$f32 vel_y;
    S$f32 mass;
} ParticlesSoA;

// Conversion function:
fn_((convertAoStoSoA(Particles aos, ParticlesSoA* soa))(void)) {
    for_(($r(0, aos.len))(i) {
        soa->pos_x.ptr[i] = aos.ptr[i].pos.x;
        soa->pos_y.ptr[i] = aos.ptr[i].pos.y;
        soa->vel_x.ptr[i] = aos.ptr[i].vel.x;
        soa->vel_y.ptr[i] = aos.ptr[i].vel.y;
        soa->mass.ptr[i] = aos.ptr[i].mass;
    }
}

fn_((convertSoAtoAoS(ParticlesSoA* soa, Particles aos))(void)) {
    for_(($r(0, aos.len))(i) {
        aos.ptr[i].pos.x = soa->pos_x.ptr[i];
        aos.ptr[i].pos.y = soa->pos_y.ptr[i];
        aos.ptr[i].vel.x = soa->vel_x.ptr[i];
        aos.ptr[i].vel.y = soa->vel_y.ptr[i];
        aos.ptr[i].mass = soa->mass.ptr[i];
    }
}
*/

// ============================================
// Build Instructions
// ============================================

/*
SSE2 (default on x86_64):
gcc -O3 -msse2 -mfpmath=sse particle.c

AVX2 (better performance):
gcc -O3 -mavx2 -mfma particle.c

Native (use best available):
gcc -O3 -march=native particle.c

ARM NEON:
gcc -O3 -mfpu=neon particle.c

Check what SIMD is used:
gcc -O3 -march=native -dM -E - < /dev/null | grep -E "SSE|AVX|NEON"
*/

#define mp_max_thrd_count (32ull)
#define mp_max_task_count (mp_max_thrd_count << 3ull)

$static var_(mp__thrd_count, usize) = 3;
$maybe_unused
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

$maybe_unused
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

typedef fn_(((*)(R, u_V$raw))(void) $T) mp_ThrdPool_TaskFn;
typedef struct mp_ThrdPool_Task {
    var_(range, R);
    var_(workerFn, mp_ThrdPool_TaskFn);
    var_(params, u_V$raw);
} mp_ThrdPool_Task;
T_use_P$(mp_ThrdPool_Task);

typedef struct mp_ThrdPool {
    // Worker threads (persistent)
    var_(workers, S$$(union Thrd_FnCtx$(mp_ThrdPool_worker)));
    var_(threads, S$$(Thrd));
    // Task queue (ring buffer)
    var_(tasks, S$$(mp_ThrdPool_Task));
    var_(head, atom_V$$(usize));
    var_(tail, atom_V$$(usize));
    var_(count, atom_V$$(usize));
    // Synchronization
    var_(mutex, Thrd_Mtx);
    var_(cond_has_task, Thrd_Cond);
    var_(cond_all_done, Thrd_Cond);
    var_(active_tasks, atom_V$$(usize));
    var_(running, atom_V$$(bool));
} mp_ThrdPool;
T_use_P$(mp_ThrdPool);
T_use_E$(P$mp_ThrdPool);

$static Thrd_fn_(mp_ThrdPool_worker, ({ mp_ThrdPool* pool; }, Void), ($ignore, args)$scope) {
    let pool = args->pool;
    while (atom_V_load(&pool->running, atom_MemOrd_acquire)) {
        O$$(P$mp_ThrdPool_Task) maybe_task = none();
        with_fini_(Thrd_Mtx_lock(&pool->mutex), Thrd_Mtx_unlock(&pool->mutex)) {
            /* clang-format off */
            while (atom_V_load(&pool->count, atom_MemOrd_acquire) == 0
                && atom_V_load(&pool->running, atom_MemOrd_acquire)) {
                Thrd_Cond_wait(&pool->cond_has_task, &pool->mutex);
            }
            /* clang-format on */
            if (atom_V_load(&pool->count, atom_MemOrd_acquire) > 0) {
                let tail = atom_V_load(&pool->tail, atom_MemOrd_acquire);
                asgLit((&maybe_task)(some(atS(pool->tasks, tail % mp_max_task_count))));
                atom_V_store(&pool->tail, tail + 1, atom_MemOrd_release);
                atom_V_fetchSub(&pool->count, 1, atom_MemOrd_release);
                atom_V_fetchAdd(&pool->active_tasks, 1, atom_MemOrd_acq_rel);
            }
        }
        if_some((maybe_task)(task)) {
            task->workerFn(task->range, task->params);
            let remaining = atom_V_fetchSub(&pool->active_tasks, 1, atom_MemOrd_acq_rel) - 1;
            if (remaining == 0 && atom_V_load(&pool->count, atom_MemOrd_acquire) == 0) {
                Thrd_Mtx_lock(&pool->mutex);
                Thrd_Cond_broadcast(&pool->cond_all_done);
                Thrd_Mtx_unlock(&pool->mutex);
            }
        }
    }
    return_({});
} $unscoped_(Thrd_fn);

$must_check
$static fn_((mp_ThrdPool_init(mem_Allocator gpa, usize thrd_count))(E$P$mp_ThrdPool) $scope) {
    let_(pool, mp_ThrdPool*) = u_castP$((P$$(mp_ThrdPool))(try_(mem_Allocator_create(gpa, typeInfo$(mp_ThrdPool)))));
    let_(workers, TypeOf(pool->workers)) = u_castS$((TypeOf(pool->workers))(try_(mem_Allocator_alloc(
        gpa, typeInfo$(union Thrd_FnCtx$(mp_ThrdPool_worker)), mp_max_task_count))));
    let_(threads, TypeOf(pool->threads)) = u_castS$((TypeOf(pool->threads))(try_(mem_Allocator_alloc(
        gpa, typeInfo$(Thrd), thrd_count))));
    let_(tasks, TypeOf(pool->tasks)) = u_castS$((TypeOf(pool->tasks))(try_(mem_Allocator_alloc(
        gpa, typeInfo$(mp_ThrdPool_Task), mp_max_task_count))));
    asgLit((pool)({
        .workers = workers,
        .threads = threads,
        .tasks = tasks,
        .head = atom_V_init(0ull),
        .tail = atom_V_init(0ull),
        .count = atom_V_init(0ull),
        .mutex = Thrd_Mtx_init(),
        .cond_has_task = Thrd_Cond_init(),
        .cond_all_done = Thrd_Cond_init(),
        .active_tasks = atom_V_init(0ull),
        .running = atom_V_init(true),
    }));
    for_(($s(pool->workers), $s(pool->threads))(worker, thread) {
        *worker = Thrd_FnCtx_from$((mp_ThrdPool_worker)(pool));
        *thread = catch_((Thrd_spawn(
            Thrd_SpawnConfig_default, worker->as_raw
        ))($ignore, claim_unreachable));
    });
    return_ok(pool);
} $unscoped_(fn);

$static fn_((mp_ThrdPool_submit(mp_ThrdPool* self, R range, mp_ThrdPool_TaskFn fn, u_V$raw params))(void)) {
    Thrd_Mtx_lock(&self->mutex);
    let head = atom_V_load(&self->head, atom_MemOrd_acquire);
    let task = atS(self->tasks, head % mp_max_task_count);
    task->range = range;
    task->workerFn = fn;
    task->params = params;
    atom_V_store(&self->head, head + 1, atom_MemOrd_release);
    atom_V_fetchAdd(&self->count, 1, atom_MemOrd_release);
    Thrd_Cond_signal(&self->cond_has_task);
    Thrd_Mtx_unlock(&self->mutex);
}

$static fn_((mp_ThrdPool_waitAll(mp_ThrdPool* self))(void)) {
    Thrd_Mtx_lock(&self->mutex);
    while (atom_V_load(&self->count, atom_MemOrd_acquire) > 0 ||
           atom_V_load(&self->active_tasks, atom_MemOrd_acquire) > 0) {
        Thrd_Cond_wait(&self->cond_all_done, &self->mutex);
    }
    Thrd_Mtx_unlock(&self->mutex);
}

$static fn_((mp_ThrdPool_shutdown(mp_ThrdPool* self))(void)) {
    atom_V_store(&self->running, false, atom_MemOrd_release);
    Thrd_Mtx_lock(&self->mutex);
    Thrd_Cond_broadcast(&self->cond_has_task);
    Thrd_Mtx_unlock(&self->mutex);
    for_(($s(self->threads))(thread) { Thrd_join(*thread); });
    Thrd_Mtx_fini(&self->mutex);
    Thrd_Cond_fini(&self->cond_has_task);
    Thrd_Cond_fini(&self->cond_all_done);
}

$static fn_((mp_ThrdPool_fini(mp_ThrdPool* self, mem_Allocator gpa))(void)) {
    mp_ThrdPool_shutdown(self);
    mem_Allocator_free(gpa, u_anyS(self->workers));
    mem_Allocator_free(gpa, u_anyS(self->threads));
    mem_Allocator_free(gpa, u_anyS(self->tasks));
    mem_Allocator_destroy(gpa, u_anyP(self));
}

$static fn_((mp_parallel_for_pooled(mp_ThrdPool* pool, R range, mp_ThrdPool_TaskFn fn, u_V$raw params))(void)) {
    let chunk = (range.end - range.begin + pool->threads.len - 1) / pool->threads.len;
    for_(($r(0, pool->threads.len))(t) {
        let begin = range.begin + chunk * t;
        let end = ((begin + chunk) <= range.end) ? (begin + chunk) : range.end;
        if (begin < end) {
            mp_ThrdPool_submit(pool, $r(begin, end), fn, params);
        }
    });
    mp_ThrdPool_waitAll(pool);
}

#include "dh/math/common.h"
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
    s = sqrt(-2.0 * log(s) / s);
    asgLit((&self->spare)(some(v * s)));
    return mean + std_dev * u * s;
}

// ============================================
// 시뮬레이션 설정
// ============================================

#include "dh/math/vec.h"

#define State_particles_log2 (23ull)
#define State_particles      (1ull << State_particles_log2) // 2^20 = 1,048,576
#define State_cell_size              (20.0)   // Was 10.0
#define State_grid_width             (100ull)  // Was 200
#define State_grid_height            (100ull)  // Was 200
#define State_max_particles_per_cell (16ull)   // Was 32

#define State_boundary_radius    (500.0)
#define State_gravity            (9.81)
#define State_damping            (0.98)
#define State_target_fps         (30.0)
#define State_delta_time         (1.0 / 30.0)

typedef struct Particles {
    S$f32 pos_x;
    S$f32 pos_y;
    S$f32 vel_x;
    S$f32 vel_y;
    S$f32 mass;
} Particles;
T_use_S$(Particles);

typedef struct Particle {
    m_V2f32 pos;
    m_V2f32 vel;
    f32     mass;
} Particle;
T_use_S$(Particle);

fn_((Particles_atSoA(const Particles* self, usize idx))(Particle)) {
    return lit$((Particle){
        .pos = m_V2f32_from(*at$S(self->pos_x, idx), *at$S(self->pos_y, idx)),
        .vel = m_V2f32_from(*at$S(self->vel_x, idx), *at$S(self->vel_y, idx)),
        .mass = *at$S(self->mass, idx),
    });
}

typedef struct Cell {
    var_(count, usize);
    var_(indices, A$$(State_max_particles_per_cell, usize));
} Cell;
T_use_S$(Cell);

typedef struct State {
    mp_ThrdPool* pool;
    Particles particles;
    S$Cell grid;
} State;

#define State_enable_randomization 0

$static fn_((State_rng(void))(Rand*));
$static fn_((State_rngGaussian(void))(RandGaussian*));
$static fn_((State_initParticles_worker(R range, u_V$raw params))(void));
$static fn_((State_initParticles(mp_ThrdPool* pool, Particles particles, m_V2f64 center, m_V2f64 radius_a_b))(void));
$static fn_((State_init(mp_ThrdPool* pool, Particles particles, S$Cell grid))(State)) {
    State_initParticles(pool, particles, m_V2f64_zero, m_V2f64_from(State_boundary_radius, 200.0));
    return lit$((State){
        .pool = pool,
        .particles = particles,
        .grid = grid,
    });
}

// ============================================
// Spatial Grid 구축
// ============================================

$static fn_((State_clearGrid_worker(R range, u_V$raw params))(void));
$static fn_((State_clearGrid(mp_ThrdPool* pool, S$Cell grid))(void));
$static fn_((State_hashPosition(m_V2f32 pos))(usize));
$static fn_((State_buildSpatialGrid(State* self))(void)) {
    State_clearGrid(self->pool, self->grid);
    for_(($r(0, State_particles))(i) {
        let particle = Particles_atSoA(&self->particles, i);
        let cell_idx = State_hashPosition(particle.pos);
        let pos = atom_fetchAdd(&at$S(self->grid, cell_idx)->count, 1, atom_MemOrd_release);
        if (pos < State_max_particles_per_cell) {
            *atA(atS(self->grid, cell_idx)->indices, pos) = i;
        }
    });
}

// ============================================
// 중력 적용
// ============================================

$static fn_((State_applyGravity_worker(R range, u_V$raw params))(void));
$static fn_((State_applyGravity(State* self))(void));

// ============================================
// 충돌 처리
// ============================================

$static fn_((State_handleCollisions_worker(R range, u_V$raw params))(void));
$static fn_((State_handleCollisions(State* self))(void));

// ============================================
// 위치 업데이트 및 경계 처리
// ============================================

$static fn_((State_updatePositions_worker(R range, u_V$raw params))(void));
$static fn_((State_updatePositions(State* self))(void));

// ============================================
// 시뮬레이션 루프
// ============================================

#include "dh/io/stream.h"
#include "dh/time/Instant.h"

$static fn_((State_simulate(State* self, mp_ThrdPool* pool, usize frame_amount))(void)) {
    io_stream_println(u8_l("\nStarting simulation for {:uz} frames at {:.1fl} FPS..."), frame_amount, State_target_fps);
    io_stream_println(u8_l("Using {:uz} threads (Pool)"), pool->threads.len);

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
        if (frame % as$((usize)State_target_fps) == 0) {
            io_stream_println(
                u8_l("Frame {:uz}: {:.2fl} ms ({:.1fl} FPS) | Avg: {:.2fl} ms ({:.1fl} FPS)"),
                frame, time_Duration_asSecs$f64(frame_time) * 1000.0, 1.0 / time_Duration_asSecs$f64(frame_time),
                (total_time / (frame + 1)) * 1000.0, (frame + 1) / total_time
            );
        }
    });

    let avg_fps = as$((f64)(frame_amount)) / total_time;
    let avg_spf = total_time / as$((f64)(frame_amount));
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
    io_stream_println(u8_l("║  Particles Simulation                  ║"));
    io_stream_println(u8_l("║  Thrd + pool + atom +simd             ║"));
    io_stream_println(u8_l("╚═══════════════════════════════════════╝\n"));

    io_stream_println(u8_l("Particles: 2^{:uz} = {:uz}"), State_particles_log2, State_particles);
    io_stream_println(u8_l("Target FPS: {:.1f}"), State_target_fps);
    let max_cpu_count = usize_subSat(catch_((Thrd_getCpuCount())($ignore, 2)), 2) + 1;
    let cpu_count = prim_clamp(expr_(usize $scope) if (1 < args.len) {
        $break_(catch_((fmt_parse$usize(*at$S(args, 1), 10))($ignore, usize_limit_max)));
    } else {
        $break_(usize_limit_max);
    } $unscoped_(expr), 1, max_cpu_count);
    var gpa = heap_Page_allocator(&(heap_Page){});
    var pool = try_(mp_ThrdPool_init(gpa, cpu_count));
    defer_(mp_ThrdPool_fini(pool, gpa));
    io_stream_println(u8_l("Using Thread Pool with {:uz} workers\n"), pool->threads.len);

    io_stream_println(u8_l("\nAllocating memory..."));
    let all_components = try_(u_castE$((E$$(S$f32))(mem_Allocator_alloc(gpa, typeInfo$(f32), State_particles * 5))));
    defer_(mem_Allocator_free(gpa, u_anyS(all_components)));
    let pos_x = sliceS(all_components, $r(0, State_particles));
    let pos_y = sliceS(all_components, $r(State_particles, State_particles * 2));
    let vel_x = sliceS(all_components, $r(State_particles * 2, State_particles * 3));
    let vel_y = sliceS(all_components, $r(State_particles * 3, State_particles * 4));
    let mass = sliceS(all_components, $r(State_particles * 4, State_particles * 5));
    Particles particles = { .pos_x = pos_x, .pos_y = pos_y, .vel_x = vel_x, .vel_y = vel_y, .mass = mass };
    let grid = try_(u_castE$((E$$(S$Cell))(mem_Allocator_alloc(gpa, typeInfo$(Cell), State_grid_width * State_grid_height))));
    defer_(mem_Allocator_free(gpa, u_anyS(grid)));
    io_stream_println(u8_l("Memory allocated: {:.2fl} MB"), as$((f64)(sizeOf$(f32) * State_particles * 5)) / (1024.0 * 1024.0));

    var state = State_init(pool, particles, grid);
    let frame_amount = expr_(usize $scope) if (2 < args.len) {
        $break_(catch_((fmt_parse$usize(*at$S(args, 2), 10))($ignore, 1000)));
    } else {
        $break_(1000);
    } $unscoped_(expr);
    io_stream_println(u8_l("Simulating {:uz} frames..."), frame_amount);
    State_simulate(&state, pool, frame_amount);

    io_stream_println(u8_l("\nSample particles:"));
    for_(($r(0, 3))(i) {
        let particle = Particles_atSoA(&state.particles, i);
        io_stream_println(
            u8_l("- Particles {:uz}: pos({:.2fl}, {:.2fl}) vel({:.2fl}, {:.2fl})"),
            i, particle.pos.x, particle.pos.y, particle.vel.x, particle.vel.y
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
    S$f32 pos_x;
    S$f32 pos_y;
    S$f32 vel_x;
    S$f32 vel_y;
    S$f32 mass;
    f32 center_x;
    f32 center_y;
    f32 radius_a;
    f32 radius_b;
} State_InitParticlesArgs;

fn_((State_initParticles_worker(R range, u_V$raw params))(void)) {
    static $Thrd_local bool c_initialized = false;
    if (!c_initialized) {
        let seed = Thrd_getCurrentId() ^ range.begin;
        *State_rng() = pp_if_(State_enable_randomization)(
            pp_then_(Rand_initSeed(seed)),
            pp_else_(Rand_withSeed(Rand_default, seed))
        );
        *State_rngGaussian() = RandGaussian_init(*State_rng());
        c_initialized = true;
    }

    let args = u_castV$((State_InitParticlesArgs)(params));

    let center_x = args.center_x;
    let center_y = args.center_y;
    let radius_a = args.radius_a;
    let radius_b = args.radius_b;

    let radius_a_vec = simd_set1(radius_a);
    let center_x_vec = simd_set1(center_x);
    let center_y_vec = simd_set1(center_y);
    let zero_vec = simd_setzero();

    // Process SIMD_WIDTH particles at once
    usize i = range.begin;
    for (; i + SIMD_WIDTH <= range.end; i += SIMD_WIDTH) {
        // Generate random values (scalar - RNG is inherently sequential)
        f32 r_values[SIMD_WIDTH] = {};
        f32 theta_values[SIMD_WIDTH] = {};
        for (usize k = 0; k < SIMD_WIDTH; ++k) {
            var r = as$((f32)RandGaussian_next$f64(State_rngGaussian(), 0.0, radius_b / 2.5));
            theta_values[k] = as$((f32)Rand_next$f64(State_rng())) * math_f64_tau;
            if (r > radius_b) { r = radius_b; }
            if (r < 0) { r = -r; }
            r_values[k] = r;
        }
        // Load into SIMD registers
        simd_f32 r_vec = simd_load(r_values);
        // VECTORIZED: Calculate positions for SIMD_WIDTH particles simultaneously
        // px = center_x + r * cos(theta)
        // py = center_y + r * sin(theta)
        // Note: cos/sin are not directly vectorizable in SSE2/AVX2
        // We need to compute them scalar or use approximations
        f32 cos_values[SIMD_WIDTH] = {};
        f32 sin_values[SIMD_WIDTH] = {};
        for (usize k = 0; k < SIMD_WIDTH; ++k) {
            cos_values[k] = cosf(theta_values[k]);
            sin_values[k] = sinf(theta_values[k]);
        }
        simd_f32 cos_vec = simd_load(cos_values);
        simd_f32 sin_vec = simd_load(sin_values);
        // SIMD: px = center_x + r * cos(theta)
        simd_f32 px = simd_fmadd(r_vec, cos_vec, center_x_vec);
        // SIMD: py = center_y + r * sin(theta)
        simd_f32 py = simd_fmadd(r_vec, sin_vec, center_y_vec);
        // SIMD: Clamp to circular boundary
        // dist = sqrt(px^2 + py^2)
        simd_f32 dist_sq = simd_fmadd(px, px, simd_mul(py, py));
        simd_f32 dist = simd_sqrt(dist_sq);
        // SIMD: Check if outside radius_a
        simd_f32 outside_mask = simd_cmp_gt(dist, radius_a_vec);
        if (simd_movemask(outside_mask)) {
            // SIMD: scale = radius_a / dist
            simd_f32 scale = simd_div(radius_a_vec, dist);
            // SIMD: Clamp position
            simd_f32 clamped_px = simd_mul(px, scale);
            simd_f32 clamped_py = simd_mul(py, scale);
            px = simd_blend(px, clamped_px, outside_mask);
            py = simd_blend(py, clamped_py, outside_mask);
        }
        // SIMD: Store positions
        simd_store(&args.pos_x.ptr[i], px);
        simd_store(&args.pos_y.ptr[i], py);
        // SIMD: Zero velocity and set mass
        simd_store(&args.vel_x.ptr[i], zero_vec);
        simd_store(&args.vel_y.ptr[i], zero_vec);
        simd_store(&args.mass.ptr[i], simd_set1(1.0f));
    }
// Handle remaining particles (scalar)
    for (; i < range.end; ++i) {
        var r = as$((f32)RandGaussian_next$f64(State_rngGaussian(), 0.0, radius_b / 2.5));
        let theta = as$((f32)Rand_next$f64(State_rng())) * math_f32_tau;
        if (r > radius_b) { r = radius_b; }
        if (r < 0) { r = -r; }
        var px = center_x + r * cosf(theta);
        var py = center_y + r * sinf(theta);
        let dist = sqrtf(px * px + py * py);
        if (dist > radius_a) {
            let scale = radius_a / dist;
            px *= scale;
            py *= scale;
        }
        args.pos_x.ptr[i] = px;
        args.pos_y.ptr[i] = py;
        args.vel_x.ptr[i] = 0.0f;
        args.vel_y.ptr[i] = 0.0f;
        args.mass.ptr[i] = 1.0f;
    }
}

fn_((State_initParticles(mp_ThrdPool* pool, Particles particles, m_V2f64 center, m_V2f64 radius_a_b))(void)) {
    mp_parallel_for_pooled(pool, $r(0, State_particles), State_initParticles_worker, u_anyV(lit$((State_InitParticlesArgs){
        .pos_x = particles.pos_x,
        .pos_y = particles.pos_y,
        .vel_x = particles.vel_x,
        .vel_y = particles.vel_y,
        .mass = particles.mass,
        .center_x = center.x,
        .center_y = center.y,
        .radius_a = radius_a_b.x,
        .radius_b = radius_a_b.y,
    })));
}

typedef struct State_ClearGridArgs {
    S$Cell grid;
} State_ClearGridArgs;

fn_((State_clearGrid_worker(R range, u_V$raw params))(void)) {
    let args = u_castV$((State_ClearGridArgs)(params));
    let grid = sliceS(args.grid, range);
    for_(($s(grid))(cell) {
        atom_store(&cell->count, 0ull, atom_MemOrd_release);
    });
}

fn_((State_clearGrid(mp_ThrdPool* pool, S$Cell grid))(void)) {
    mp_parallel_for_pooled(pool, $r(0, State_grid_width * State_grid_height), State_clearGrid_worker, u_anyV(lit$((State_ClearGridArgs){
        .grid = grid,
    })));
}

fn_((State_hashPosition(m_V2f32 pos))(usize)) {
    let gx_0 = as$((isize)((pos.x + (State_grid_width * State_cell_size) / 2.0f) / State_cell_size));
    let gy_0 = as$((isize)((pos.y + (State_grid_height * State_cell_size) / 2.0f) / State_cell_size));
    let gx_1 = as$((usize)prim_clamp(gx_0, 0, as$((isize)State_grid_width) - 1));
    let gy_1 = as$((usize)prim_clamp(gy_0, 0, as$((isize)State_grid_height) - 1));
    return gy_1 * State_grid_width + gx_1;
}

typedef struct State_ApplyGravityArgs {
    S$f32 pos_x;
    S$f32 pos_y;
    S$f32 vel_x;
    S$f32 vel_y;
} State_ApplyGravityArgs;

fn_((State_applyGravity_worker(R range, u_V$raw params))(void)) {
    let args = u_castV$((State_ApplyGravityArgs)(params));

    let dt = as$((f32)State_delta_time);
    let gravity = as$((f32)State_gravity);
    let damping = as$((f32)State_damping);
    let epsilon = 0.1f;

    let dt_vec = simd_set1(dt);
    let gravity_vec = simd_set1(gravity);
    let damping_vec = simd_set1(damping);
    let epsilon_vec = simd_set1(epsilon);
    let zero_vec = simd_setzero();

    usize i = range.begin;
    for (; i + SIMD_WIDTH <= range.end; i += SIMD_WIDTH) {
        simd_f32 px = simd_load(&args.pos_x.ptr[i]);
        simd_f32 py = simd_load(&args.pos_y.ptr[i]);
        simd_f32 vx = simd_load(&args.vel_x.ptr[i]);
        simd_f32 vy = simd_load(&args.vel_y.ptr[i]);
        // Direction toward origin: -pos
        simd_f32 dx = simd_sub(zero_vec, px);
        simd_f32 dy = simd_sub(zero_vec, py);
        // Distance
        simd_f32 dist_sq = simd_fmadd(dx, dx, simd_mul(dy, dy));
        simd_f32 dist = simd_sqrt(dist_sq);
        // Avoid division by zero
        simd_f32 valid_mask = simd_cmp_gt(dist, epsilon_vec);
        // Force = gravity / dist^2
        simd_f32 inv_dist = simd_div(simd_set1(1.0f), dist);
        simd_f32 force = simd_div(gravity_vec, dist_sq);
        // Acceleration = (direction / dist) * force
        simd_f32 ax = simd_mul(simd_mul(dx, inv_dist), force);
        simd_f32 ay = simd_mul(simd_mul(dy, inv_dist), force);
        // Update velocity
        vx = simd_blend(vx, simd_fmadd(ax, dt_vec, vx), valid_mask);
        vy = simd_blend(vy, simd_fmadd(ay, dt_vec, vy), valid_mask);
        // Apply damping
        vx = simd_mul(vx, damping_vec);
        vy = simd_mul(vy, damping_vec);
        simd_store(&args.vel_x.ptr[i], vx);
        simd_store(&args.vel_y.ptr[i], vy);
    }
    // Handle remaining particles
    for (; i < range.end; ++i) {
        f32 px = args.pos_x.ptr[i];
        f32 py = args.pos_y.ptr[i];
        f32 vx = args.vel_x.ptr[i];
        f32 vy = args.vel_y.ptr[i];
        f32 dx = -px;
        f32 dy = -py;
        f32 dist = sqrtf(dx * dx + dy * dy);
        if (dist > epsilon) {
            f32 force = gravity / (dist * dist);
            f32 ax = (dx / dist) * force;
            f32 ay = (dy / dist) * force;
            vx += ax * dt;
            vy += ay * dt;
        }
        vx *= damping;
        vy *= damping;
        args.vel_x.ptr[i] = vx;
        args.vel_y.ptr[i] = vy;
    }
}

fn_((State_applyGravity(State* self))(void)) {
    mp_parallel_for_pooled(self->pool, $r(0, State_particles), State_applyGravity_worker, u_anyV(lit$((State_ApplyGravityArgs){
        .pos_x = self->particles.pos_x,
        .pos_y = self->particles.pos_y,
        .vel_x = self->particles.vel_x,
        .vel_y = self->particles.vel_y,
    })));
}

typedef struct State_HandleCollisionsArgs {
    S$f32 pos_x;
    S$f32 pos_y;
    S$Cell grid;
} State_HandleCollisionsArgs;

// Helper: SIMD distance calculation for batch of particles
fn_((simd_compute_distances(
    f32 px, f32 py,  // Reference particle (scalar)
    f32* other_px, f32* other_py,  // Neighbor particles (array)
    f32* out_dx, f32* out_dy, f32* out_dist,
    usize count
))(void)) {
    let px_vec = simd_set1(px);
    let py_vec = simd_set1(py);
    for (usize i = 0; i < count; i += SIMD_WIDTH) {
        if (i + SIMD_WIDTH > count) { break; }
        // Load neighbor positions
        simd_f32 other_px_vec = simd_load(&other_px[i]);
        simd_f32 other_py_vec = simd_load(&other_py[i]);
        // Compute deltas
        simd_f32 dx = simd_sub(other_px_vec, px_vec);
        simd_f32 dy = simd_sub(other_py_vec, py_vec);
        // Compute distance
        simd_f32 dist_sq = simd_fmadd(dx, dx, simd_mul(dy, dy));
        simd_f32 dist = simd_sqrt(dist_sq);
        // Store results
        simd_store(&out_dx[i], dx);
        simd_store(&out_dy[i], dy);
        simd_store(&out_dist[i], dist);
    }
    // Handle remainder
    for (usize i = (count / SIMD_WIDTH) * SIMD_WIDTH; i < count; ++i) {
        out_dx[i] = other_px[i] - px;
        out_dy[i] = other_py[i] - py;
        out_dist[i] = sqrtf(out_dx[i] * out_dx[i] + out_dy[i] * out_dy[i]);
    }
}

fn_((State_handleCollisions_worker(R range, u_V$raw params))(void)) {
    let args = u_castV$((State_HandleCollisionsArgs)(params));
    let grid = args.grid;
    let collision_radius = 0.5f;
    let min_dist = collision_radius * 2.0f;
    let min_dist_sq = min_dist * min_dist;

    // Temporary buffers for SIMD batch processing
    f32 neighbor_px[State_max_particles_per_cell] = {};
    f32 neighbor_py[State_max_particles_per_cell] = {};
    f32 dx_buf[State_max_particles_per_cell] = {};
    f32 dy_buf[State_max_particles_per_cell] = {};
    f32 dist_buf[State_max_particles_per_cell] = {};
    usize neighbor_indices[State_max_particles_per_cell];
    for (usize i = range.begin; i < range.end; ++i) {
        // Load reference particle (scalar)
        f32 px = args.pos_x.ptr[i];
        f32 py = args.pos_y.ptr[i];
        // Get grid cell
        let cell_idx = State_hashPosition((m_V2f32){ .x = px, .y = py });
        let gx = cell_idx % State_grid_width;
        let gy = cell_idx / State_grid_width;
        // Check neighboring cells
        for (isize dy = -1; dy <= 1; ++dy) {
            for (isize dx = -1; dx <= 1; ++dx) {
                let nx = as$((isize)(gx)) + dx;
                let ny = as$((isize)(gy)) + dy;
                if (nx < 0 || as$((usize)(nx)) >= State_grid_width || ny < 0 || as$((usize)(ny)) >= State_grid_height) {
                    continue;
                }
                let neighbor_idx = ny * State_grid_width + nx;
                let neighbor = at$S(grid, neighbor_idx);
                let count = atom_load(&neighbor->count, atom_MemOrd_acquire);
                let safe_count = (count < State_max_particles_per_cell)
                    ? count : State_max_particles_per_cell;
                // Gather neighbor particles (prepare for SIMD)
                usize valid_count = 0;
                for (usize k = 0; k < safe_count; ++k) {
                    let j = *atA(neighbor->indices, k);
                    if (j <= i) { continue; }  // Avoid double-processing
                    neighbor_indices[valid_count] = j;
                    neighbor_px[valid_count] = args.pos_x.ptr[j];
                    neighbor_py[valid_count] = args.pos_y.ptr[j];
                    valid_count++;
                }
                if (valid_count == 0) { continue; }
                // SIMD: Batch distance calculation
                simd_compute_distances(
                    px, py,
                    neighbor_px, neighbor_py,
                    dx_buf, dy_buf, dist_buf,
                    valid_count
                );
                // Scalar: Process collisions (irregular updates)
                for (usize k = 0; k < valid_count; ++k) {
                    let dist_sq = dx_buf[k] * dx_buf[k] + dy_buf[k] * dy_buf[k];
                    if (dist_sq < min_dist_sq && dist_sq > 0.001f) {
                        let dist = dist_buf[k];
                        let overlap = min_dist - dist;
                        let inv_dist = 1.0f / dist;
                        let nx = dx_buf[k] * inv_dist;
                        let ny = dy_buf[k] * inv_dist;
                        let separation = overlap * 0.5f;
                        // Update only reference particle (scalar write)
                        px -= nx * separation;
                        py -= ny * separation;
                    }
                }
            }
        }
        // Write back updated position
        args.pos_x.ptr[i] = px;
        args.pos_y.ptr[i] = py;
    }
}

fn_((State_handleCollisions(State* self))(void)) {
    mp_parallel_for_pooled(self->pool, $r(0, State_particles), State_handleCollisions_worker, u_anyV(lit$((State_HandleCollisionsArgs){
        .pos_x = self->particles.pos_x,
        .pos_y = self->particles.pos_y,
        .grid = self->grid,
    })));
}

typedef struct State_UpdatePositionsArgs {
    S$f32 pos_x;
    S$f32 pos_y;
    S$f32 vel_x;
    S$f32 vel_y;
} State_UpdatePositionsArgs;

fn_((State_updatePositions_worker(R range, u_V$raw params))(void)) {
    let args = u_castV$((State_UpdatePositionsArgs)(params));
    let pos_x = args.pos_x;
    let pos_y = args.pos_y;
    let vel_x = args.vel_x;
    let vel_y = args.vel_y;

    let dt = State_delta_time;
    let radius = State_boundary_radius;
    let damping = 0.8f;

    let dt_vec = simd_set1(dt);
    let radius_vec = simd_set1(radius);
    let damping_vec = simd_set1(damping);

    // Process SIMD_WIDTH particles at a time
    usize i = range.begin;
    for (; (i + SIMD_WIDTH) <= range.end; i += SIMD_WIDTH) {
        // Load positions and velocities
        simd_f32 px = simd_load(atS(pos_x, i));
        simd_f32 py = simd_load(atS(pos_y, i));
        simd_f32 vx = simd_load(atS(vel_x, i));
        simd_f32 vy = simd_load(atS(vel_y, i));
        // Update position: pos += vel * dt
        px = simd_fmadd(vx, dt_vec, px);
        py = simd_fmadd(vy, dt_vec, py);
        // Calculate distance from origin
        simd_f32 d_sq = simd_fmadd(px, px, simd_mul(py, py));
        simd_f32 d = simd_sqrt(d_sq);
        // Check if outside boundary
        simd_f32 outside_mask = simd_cmp_gt(d, radius_vec);
        if (simd_movemask(outside_mask)) {
            // Normalize direction
            simd_f32 inv_dist = simd_div(radius_vec, d);
            simd_f32 nx = simd_mul(px, inv_dist);
            simd_f32 ny = simd_mul(py, inv_dist);
            // Clamp position to boundary
            simd_f32 clamped_px = simd_mul(nx, radius_vec);
            simd_f32 clamped_py = simd_mul(ny, radius_vec);
            px = simd_blend(px, clamped_px, outside_mask);
            py = simd_blend(py, clamped_py, outside_mask);
            // Reflect velocity: vel -= 2 * dot(vel, normal) * normal
            simd_f32 dot = simd_fmadd(vx, nx, simd_mul(vy, ny));
            simd_f32 dot_2 = simd_add(dot, dot);
            simd_f32 reflected_vx = simd_fnmadd(dot_2, nx, vx);
            simd_f32 reflected_vy = simd_fnmadd(dot_2, ny, vy);
            // Apply damping to reflected velocity
            reflected_vx = simd_mul(reflected_vx, damping_vec);
            reflected_vy = simd_mul(reflected_vy, damping_vec);
            vx = simd_blend(vx, reflected_vx, outside_mask);
            vy = simd_blend(vy, reflected_vy, outside_mask);
        }
        // Store back
        simd_store(&args.pos_x.ptr[i], px);
        simd_store(&args.pos_y.ptr[i], py);
        simd_store(&args.vel_x.ptr[i], vx);
        simd_store(&args.vel_y.ptr[i], vy);
    }
    for (; i < range.end; ++i) {
        f32 px = *atS(args.pos_x, i);
        f32 py = *atS(args.pos_y, i);
        f32 vx = *atS(args.vel_x, i);
        f32 vy = *atS(args.vel_y, i);
        px += vx * as$((f32)State_delta_time);
        py += vy * as$((f32)State_delta_time);
        f32 dist = sqrtf(px * px + py * py);
        if (dist > State_boundary_radius) {
            f32 inv_dist = as$((f32)State_boundary_radius) / dist;
            f32 nx = px * inv_dist;
            f32 ny = py * inv_dist;
            px = nx * as$((f32)State_boundary_radius);
            py = ny * as$((f32)State_boundary_radius);
            f32 dot = vx * nx + vy * ny;
            vx -= 2.0f * dot * nx;
            vy -= 2.0f * dot * ny;
            vx *= 0.8f;
            vy *= 0.8f;
        }
        args.pos_x.ptr[i] = px;
        args.pos_y.ptr[i] = py;
        args.vel_x.ptr[i] = vx;
        args.vel_y.ptr[i] = vy;
    }
}

fn_((State_updatePositions(State* self))(void)) {
    mp_parallel_for_pooled(self->pool, $r(0, State_particles), State_updatePositions_worker, u_anyV(lit$((State_UpdatePositionsArgs) {
        .pos_x = self->particles.pos_x,
        .pos_y = self->particles.pos_y,
        .vel_x = self->particles.vel_x,
        .vel_y = self->particles.vel_y,
    })));
}

/* // For completeness, here's a naive SIMD attempt (NOT RECOMMENDED):
fn_((State_buildSpatialGrid_worker_naive_simd(usize start, usize end, u_V$raw params))(void)) {
    let args = u_castV$((State_BuildGridArgs)(params));

    // This is NOT efficient, just showing why SIMD doesn't help
    for (usize i = start; i < end; i++) {
        // Can't vectorize: each particle needs separate hash + atomic
        let particle = at$S(args.particles, i);
        let cell_idx = State_hashPosition(particle->pos);

        // Atomic operation - inherently scalar
        let pos = atom_fetchAdd(&at$S(args.grid, cell_idx)->count, 1, atom_MemOrd_release);

        if (pos < State_max_particles_per_cell) {
            *atA(atS(args.grid, cell_idx)->indices, pos) = i;
        }
    }
} */

/* // RECOMMENDED ALTERNATIVE: Thread-Local Buffers
typedef struct ThreadLocalGridBuffer {
    usize counts[State_grid_width * State_grid_height];
    usize indices[State_grid_width * State_grid_height][State_max_particles_per_cell];
} ThreadLocalGridBuffer;

fn_((State_buildSpatialGrid_worker_buffered(usize start, usize end, u_V$raw params))(void)) {
    let args = u_castV$((State_BuildGridArgs)(params));

    // Thread-local buffer (NO atomics, NO contention!)
    ThreadLocalGridBuffer local = {};

    // Build local grid
    for (usize i = start; i < end; i++) {
        let particle = at$S(args.particles, i);
        let cell_idx = State_hashPosition(particle->pos);

        let pos = local.counts[cell_idx]++;
        if (pos < State_max_particles_per_cell) {
            local.indices[cell_idx][pos] = i;
        }
    }

    // Merge into global grid (only at end, reduces contention)
    for (usize cell = 0; cell < State_grid_width * State_grid_height; cell++) {
        if (local.counts[cell] > 0) {
            let global_cell = at$S(args.grid, cell);
            let base_pos = atom_fetchAdd(&global_cell->count, local.counts[cell], atom_MemOrd_release);

            for (usize k = 0; k < local.counts[cell] && base_pos + k < State_max_particles_per_cell; k++) {
                *atA(global_cell->indices, base_pos + k) = local.indices[cell][k];
            }
        }
    }
} */
