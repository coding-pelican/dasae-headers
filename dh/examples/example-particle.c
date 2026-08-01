#include "dh-main.h"
#include "dh/atom.h"
#include "dh/clsr.h"
#include "dh/fmt/common.h"
#include "dh/heap/Sys.h"
#include "dh/io/stream.h"
#include "dh/m-math-linalg.h"
#include "dh/mem/Alctr.h"
#include "dh/thrd/Cond.h"
#include "dh/thrd/Mtx.h"
#include "dh/thrd/Self.h"
#include "dh/time.h"

#define particle_example_transient (1 << 0)
#define particle_example_pool (1 << 1)
#define particle_example_pool_opti (1 << 2)
#define particle_example_pool_opti_half (1 << 3)
#define particle_example_pool_opti_simd (1 << 4)
#define particle_example_pool_opti_phased (1 << 5)
#define particle_example_all ( \
    particle_example_transient \
    | particle_example_pool \
    | particle_example_pool_opti \
    | particle_example_pool_opti_half \
    | particle_example_pool_opti_simd \
    | particle_example_pool_opti_phased \
)

#ifndef particle_example
#define particle_example particle_example_pool_opti_phased
#endif

#if (particle_example & ~particle_example_all) != 0
#error "particle_example contains an unknown selector bit"
#elif particle_example == 0 || (particle_example & (particle_example - 1)) != 0
#error "particle_example must contain exactly one selector bit"
#endif

#if particle_example & particle_example_transient
#define particle_example_kind particle_Kind_transient
#define particle_example_title "transient threads"
#define particle_example_use_f32 0
#elif particle_example & particle_example_pool
#define particle_example_kind particle_Kind_pool_static
#define particle_example_title "persistent pool with static partitions"
#define particle_example_use_f32 0
#elif particle_example & particle_example_pool_opti
#define particle_example_kind particle_Kind_pool_dynamic
#define particle_example_title "persistent pool with dynamic work sharing"
#define particle_example_use_f32 0
#elif particle_example & particle_example_pool_opti_half
#define particle_example_kind particle_Kind_pool_half
#define particle_example_title "persistent pool with half the available workers"
#define particle_example_use_f32 1
#elif particle_example & particle_example_pool_opti_simd
#define particle_example_kind particle_Kind_pool_simd
#define particle_example_title "persistent pool with f32 SIMD-backed vectors"
#define particle_example_use_f32 1
#elif particle_example & particle_example_pool_opti_phased
#define particle_example_kind particle_Kind_pool_phased
#define particle_example_title "persistent pool with phased simulation"
#define particle_example_use_f32 0
#endif

#define particle_max_workers usize_(32)
#define particle_count_log2 usize_(20)
#define particle_count (usize_(1) << particle_count_log2)
#define particle_boundary 500.0
#define particle_target_fps 30.0
#define particle_delta_time (1.0 / particle_target_fps)

#if particle_example_use_f32
typedef f32 particle_Flt;
typedef m_V2f32 particle_Vec;
#define particle_Vec_zero m_V2f32_zero
#define particle_Vec_of m_V2f32_of
#define particle_Vec_sincos m_V2f32_sincos
#define particle_Vec_add m_V2f32_add
#define particle_Vec_sub m_V2f32_sub
#define particle_Vec_scal m_V2f32_scal
#define particle_Vec_scalInv m_V2f32_scalInv
#define particle_Vec_len m_V2f32_len
#define particle_Vec_lenSq m_V2f32_lenSq
#define particle_Vec_dot m_V2f32_dot
#else
typedef f64 particle_Flt;
typedef m_V2f64 particle_Vec;
#define particle_Vec_zero m_V2f64_zero
#define particle_Vec_of m_V2f64_of
#define particle_Vec_sincos m_V2f64_sincos
#define particle_Vec_add m_V2f64_add
#define particle_Vec_sub m_V2f64_sub
#define particle_Vec_scal m_V2f64_scal
#define particle_Vec_scalInv m_V2f64_scalInv
#define particle_Vec_len m_V2f64_len
#define particle_Vec_lenSq m_V2f64_lenSq
#define particle_Vec_dot m_V2f64_dot
#endif

typedef enum_((particle_Kind $fits($packed))(
    particle_Kind_transient,
    particle_Kind_pool_static,
    particle_Kind_pool_dynamic,
    particle_Kind_pool_phased,
    particle_Kind_pool_half,
    particle_Kind_pool_simd
)) particle_Kind;

typedef struct particle_Item {
    var_(pos, particle_Vec);
    var_(vel, particle_Vec);
    var_(mass, particle_Flt);
} particle_Item;
T_use_S$(particle_Item);
T_use_E$($set(mem_E)(S$particle_Item));
T_use_mem_Alctr_alloc$(particle_Item);
T_use_mem_Alctr_free$(particle_Item);

typedef fn_(((*particle_LoopFn)(usize idx, u_V$raw args))(void));
typedef struct particle_Work {
    var_(range, R);
    var_(loopFn, particle_LoopFn);
    var_(args, u_V$raw);
} particle_Work;

typedef struct particle_TransientCtx {
    var_(gpa, mem_Alctr);
    var_(worker_count, usize);
} particle_TransientCtx;

$static fn_((particle_transientWorker(particle_Work work))(Void));
fn_use_Clsr_((particle_transientWorker)(particle_Work)(Void));
T_use$((Void)(thrd_spawn, thrd_join));

typedef struct particle_Pool particle_Pool;
$static fn_((particle_poolWorker(particle_Pool* pool, usize worker_id))(Void));
fn_use_Clsr_((particle_poolWorker)(particle_Pool*, usize)(Void));

struct particle_Pool {
    var_(gpa, mem_Alctr);
    var_(worker_count, usize);
    var_(dynamic, bool);
    var_(stopping, bool);
    var_(generation, usize);
    var_(pending, usize);
    var_(next, atom_V$usize);
    var_(work, particle_Work);
    var_(lock, thrd_Mtx);
    var_(ready, thrd_Cond);
    var_(done, thrd_Cond);
    var_(workers, A$$(particle_max_workers, Clsr_(particle_poolWorker)));
    var_(threads, A$$(particle_max_workers, thrd_Self));
};
T_use$((particle_Pool)(P));
T_use_E$($set(mem_E)(P$particle_Pool));
T_use$((particle_Pool)(mem_Alctr_create, mem_Alctr_destroy));

typedef struct particle_Executor {
    var_(ctx, P$raw);
    fn_(((*parallelFn)(P$raw ctx, particle_Work work))(void));
} particle_Executor;

typedef struct particle_State {
    var_(particles, S$particle_Item);
    var_(executor, particle_Executor);
} particle_State;

typedef struct particle_ItemsArgs {
    var_(items, S$particle_Item);
} particle_ItemsArgs;

$static fn_((particle_transientRun(P$raw ctx, particle_Work work))(void));
$static fn_((particle_poolRun(P$raw ctx, particle_Work work))(void));
$static fn_((particle_Pool_init(
    particle_Pool* self, mem_Alctr gpa, usize worker_count, bool dynamic
))(E$void));
$static fn_((particle_Pool_fini(particle_Pool* self))(void));
$static fn_((particle_Executor_run(
    particle_Executor self, R range, particle_LoopFn loopFn, u_V$raw args
))(void));

$static fn_((particle_initOne(usize idx, u_V$raw args))(void));
$static fn_((particle_applyGravityOne(usize idx, u_V$raw args))(void));
$static fn_((particle_handleCollisionOne(usize idx, u_V$raw args))(void));
$static fn_((particle_updatePositionOne(usize idx, u_V$raw args))(void));
$static fn_((particle_State_init(particle_State* self))(void));
$static fn_((particle_State_step(particle_State* self))(void));
$static fn_((particle_State_simulate(
    particle_State* self, time_Awake clock, usize frame_count, S_const$u8 title
))(void));
$static fn_((particle_nextUsize(
    proc_Args_Iter* args, S$u8 scratch, usize fallback
))(usize));
$static fn_((particle_runPooled(
    mem_Alctr gpa,
    S$particle_Item particles,
    time_Awake clock,
    usize frame_count,
    particle_Kind kind,
    usize worker_count
))(E$void));
$static fn_((particle_run(proc_Entry entry))(E$void));

fn_((particle_transientWorker(particle_Work work))(Void)) {
    for_(($r(work.range.begin, work.range.end))(idx)) {
        work.loopFn(idx, work.args);
    } $end(for);
    return Void_();
};

fn_((particle_transientRun(P$raw ctx, particle_Work work))(void)) {
    let self = (particle_TransientCtx*)ctx;
    let worker_count = self->worker_count;
    var_(workers, A$$(particle_max_workers, Clsr_(particle_transientWorker))) = {};
    var_(threads, A$$(particle_max_workers, thrd_Self)) = {};
    let workers_used = A_slice((workers)$r(0, worker_count));
    let threads_used = A_slice((threads)$r(0, worker_count));
    let chunk = (R_len(work.range) + worker_count - 1) / worker_count;

    for_(($rf(0), $s(workers_used), $s(threads_used))(worker_id, worker, thread)) {
        let begin = pri_min(work.range.begin + chunk * worker_id, work.range.end);
        let end = pri_min(begin + chunk, work.range.end);
        *worker = clsr_((particle_transientWorker)((particle_Work){
            .range = R_from(begin, end),
            .loopFn = work.loopFn,
            .args = work.args,
        }));
        *thread = catch_((thrd_spawn$Void(
            thrd_SpawnCfg_default(self->gpa),
            worker->as_base
        ))($ignore, claim_unreachable));
    } $end(for);
    for_(($s(threads_used))(thread)) {
        let_ignore = thrd_join$Void(*thread);
    } $end(for);
};

fn_((particle_poolWorker(particle_Pool* pool, usize worker_id))(Void)) {
    var_(observed_generation, usize) = 0;
    while (true) {
        thrd_Mtx_lockProtcd(&pool->lock);
        while (observed_generation == pool->generation && !pool->stopping) {
            thrd_Cond_waitProtcd(&pool->ready, &pool->lock);
        }
        if (pool->stopping) {
            thrd_Mtx_unlock(&pool->lock);
            return Void_();
        }
        observed_generation = pool->generation;
        let work = pool->work;
        let dynamic = pool->dynamic;
        thrd_Mtx_unlock(&pool->lock);

        if (dynamic) {
            while (true) {
                let idx = atom_V_pri_fetchAdd(
                    &pool->next, 1, atom_MemOrd_monotonic
                );
                if (work.range.end <= idx) break;
                work.loopFn(idx, work.args);
            }
        } else {
            let chunk = (R_len(work.range) + pool->worker_count - 1)
                      / pool->worker_count;
            let begin = pri_min(
                work.range.begin + chunk * worker_id,
                work.range.end
            );
            let end = pri_min(begin + chunk, work.range.end);
            for_(($r(begin, end))(idx)) {
                work.loopFn(idx, work.args);
            } $end(for);
        }

        thrd_Mtx_lockProtcd(&pool->lock);
        pool->pending -= 1;
        if (pool->pending == 0) thrd_Cond_signal(&pool->done);
        thrd_Mtx_unlock(&pool->lock);
    }
};

fn_((particle_Pool_init(
    particle_Pool* self, mem_Alctr gpa, usize worker_count, bool dynamic
))(E$void) $guard) {
    claim_assert_nonnull(self);
    claim_assert(worker_count != 0 && worker_count <= particle_max_workers);
    *self = (particle_Pool){
        .gpa = mem_Alctr_ensureValid(gpa),
        .worker_count = worker_count,
        .dynamic = dynamic,
        .stopping = false,
        .generation = 0,
        .pending = 0,
        .next = atom_V_init(0),
        .work = {
            .range = R_from(0, 0),
            .loopFn = particle_initOne,
            .args = u_anyV(Void_()),
        },
        .lock = thrd_Mtx_init(),
        .ready = thrd_Cond_init(),
        .done = thrd_Cond_init(),
        .workers = {},
        .threads = {},
    };
    errdefer_($ignore, {
        thrd_Cond_fini(&self->done);
        thrd_Cond_fini(&self->ready);
        thrd_Mtx_fini(&self->lock);
    });

    let workers = A_slice((self->workers)$r(0, worker_count));
    let threads = A_slice((self->threads)$r(0, worker_count));
    var_(spawned, usize) = 0;
    errdefer_($ignore, {
        thrd_Mtx_lockProtcd(&self->lock);
        self->stopping = true;
        thrd_Cond_broadcast(&self->ready);
        thrd_Mtx_unlock(&self->lock);
        for_(($r(0, spawned), $s(threads))(idx, thread)) {
            let_ignore = idx;
            let_ignore = thrd_join$Void(*thread);
        } $end(for);
    });
    for_(($rf(0), $s(workers), $s(threads))(worker_id, worker, thread)) {
        *worker = clsr_((particle_poolWorker)(self, worker_id));
        *thread = try_(thrd_spawn$Void(
            thrd_SpawnCfg_default(gpa),
            worker->as_base
        ));
        spawned += 1;
    } $end(for);
    return_ok({});
} $unguarded(fn);

fn_((particle_Pool_fini(particle_Pool* self))(void)) {
    thrd_Mtx_lockProtcd(&self->lock);
    self->stopping = true;
    thrd_Cond_broadcast(&self->ready);
    thrd_Mtx_unlock(&self->lock);
    let threads = A_slice((self->threads)$r(0, self->worker_count));
    for_(($s(threads))(thread)) {
        let_ignore = thrd_join$Void(*thread);
    } $end(for);
    thrd_Cond_fini(&self->done);
    thrd_Cond_fini(&self->ready);
    thrd_Mtx_fini(&self->lock);
};

fn_((particle_poolRun(P$raw ctx, particle_Work work))(void)) {
    let self = (particle_Pool*)ctx;
    if (R_len(work.range) == 0) return;
    thrd_Mtx_lockProtcd(&self->lock);
    self->work = work;
    self->pending = self->worker_count;
    atom_V_store(&self->next, work.range.begin, atom_MemOrd_monotonic);
    self->generation += 1;
    thrd_Cond_broadcast(&self->ready);
    while (self->pending != 0) {
        thrd_Cond_waitProtcd(&self->done, &self->lock);
    }
    thrd_Mtx_unlock(&self->lock);
};

fn_((particle_Executor_run(
    particle_Executor self, R range, particle_LoopFn loopFn, u_V$raw args
))(void)) {
    self.parallelFn(self.ctx, (particle_Work){
                                  .range = R_ensureValid(range),
                                  .loopFn = loopFn,
                                  .args = args,
                              });
};

fn_((particle_initOne(usize idx, u_V$raw args))(void)) {
    let cfg = u_castV$((particle_ItemsArgs)(args));
    let item = S_at((cfg.items)[idx]);
    let phase = as$(particle_Flt)(idx % 4096) / as$(particle_Flt)(4096);
    let radius = as$(particle_Flt)(particle_boundary)
              * (as$(particle_Flt)(0.15) + as$(particle_Flt)(0.8) * phase);
    let angle = phase * as$(particle_Flt)(math_f64_tau)
              + as$(particle_Flt)(idx / 4096) * as$(particle_Flt)(0.013);
    *item = (particle_Item){
        .pos = particle_Vec_scal(particle_Vec_sincos(angle), radius),
        .vel = particle_Vec_zero,
        .mass = as$(particle_Flt)(1),
    };
};

fn_((particle_applyGravityOne(usize idx, u_V$raw args))(void)) {
    let cfg = u_castV$((particle_ItemsArgs)(args));
    let item = S_at((cfg.items)[idx]);
    let toward_center = particle_Vec_scal(item->pos, as$(particle_Flt)(-1));
    let distance = particle_Vec_len(toward_center);
    if (as$(particle_Flt)(0.1) < distance) {
        let acceleration = as$(particle_Flt)(9.81) / (distance * distance);
        item->vel = particle_Vec_add(
            item->vel,
            particle_Vec_scal(
                particle_Vec_scalInv(toward_center, distance),
                acceleration * as$(particle_Flt)(particle_delta_time)
            )
        );
    }
    item->vel = particle_Vec_scal(item->vel, as$(particle_Flt)(0.98));
};

fn_((particle_handleCollisionOne(usize idx, u_V$raw args))(void)) {
    let cfg = u_castV$((particle_ItemsArgs)(args));
    let item = S_at((cfg.items)[idx]);
    let neighbor = S_at((cfg.items)[(idx + 1) % cfg.items.len]);
    let delta = particle_Vec_sub(neighbor->pos, item->pos);
    let distance_sq = particle_Vec_lenSq(delta);
    let min_distance = as$(particle_Flt)(1);
    if (as$(particle_Flt)(0.000001) < distance_sq
        && distance_sq < min_distance * min_distance) {
        let distance = flt_sqrt(distance_sq);
        let normal = particle_Vec_scalInv(delta, distance);
        item->vel = particle_Vec_sub(
            item->vel,
            particle_Vec_scal(normal, (min_distance - distance) * as$(particle_Flt)(0.5))
        );
    }
};

fn_((particle_updatePositionOne(usize idx, u_V$raw args))(void)) {
    let cfg = u_castV$((particle_ItemsArgs)(args));
    let item = S_at((cfg.items)[idx]);
    item->pos = particle_Vec_add(
        item->pos,
        particle_Vec_scal(item->vel, as$(particle_Flt)(particle_delta_time))
    );
    let distance = particle_Vec_len(item->pos);
    if (as$(particle_Flt)(particle_boundary) < distance) {
        let normal = particle_Vec_scalInv(item->pos, distance);
        item->pos = particle_Vec_scal(normal, as$(particle_Flt)(particle_boundary));
        item->vel = particle_Vec_scal(
            particle_Vec_sub(
                item->vel,
                particle_Vec_scal(normal, as$(particle_Flt)(2) * particle_Vec_dot(item->vel, normal))
            ),
            as$(particle_Flt)(0.8)
        );
    }
};

fn_((particle_State_init(particle_State* self))(void)) {
    particle_Executor_run(
        self->executor,
        R_from(0, self->particles.len),
        particle_initOne,
        u_anyV((particle_ItemsArgs){ .items = self->particles })
    );
};

fn_((particle_State_step(particle_State* self))(void)) {
    let args = u_anyV((particle_ItemsArgs){ .items = self->particles });
    let range = R_from(0, self->particles.len);
    particle_Executor_run(self->executor, range, particle_applyGravityOne, args);
    particle_Executor_run(self->executor, range, particle_handleCollisionOne, args);
    particle_Executor_run(self->executor, range, particle_updatePositionOne, args);
};

fn_((particle_State_simulate(
    particle_State* self, time_Awake clock, usize frame_count, S_const$u8 title
))(void)) {
    io_stream_println(
        u8_l("Particle simulation: {:s}\nparticles: 2^{:uz} = {:uz}, frames: {:uz}"),
        title, particle_count_log2, particle_count, frame_count
    );
    var_(total_secs, f64) = 0;
    for_(($r(0, frame_count))(frame)) {
        let started = time_Awake_now(clock);
        particle_State_step(self);
        let elapsed = time_Awake_Inst_elapsed(started, clock);
        let frame_secs = time_Dur_asSecs$f64(elapsed);
        total_secs += frame_secs;
        if (frame % as$(usize)(particle_target_fps) == 0) {
            io_stream_println(
                u8_l("frame {:uz}: {:.2fl} ms, average {:.1fl} FPS"),
                frame,
                frame_secs * 1000.0,
                as$(f64)(frame + 1) / total_secs
            );
        }
    } $end(for);
    io_stream_println(
        u8_l("average: {:.2fl} ms, {:.1fl} FPS"),
        total_secs * 1000.0 / as$(f64)(frame_count),
        as$(f64)(frame_count) / total_secs
    );
};

fn_((particle_nextUsize(
    proc_Args_Iter* args, S$u8 scratch, usize fallback
))(usize)) {
    let value = catch_((proc_Args_Iter_next(args, scratch))(
        $ignore, return fallback
    ));
    let text = orelse_((value)(return fallback));
    return catch_((fmt_parse$usize(text, 10))($ignore, fallback));
};

fn_((particle_runPooled(
    mem_Alctr gpa,
    S$particle_Item particles,
    time_Awake clock,
    usize frame_count,
    particle_Kind kind,
    usize worker_count
))(E$void) $guard) {
    let dynamic = kind == particle_Kind_pool_dynamic
               || kind == particle_Kind_pool_phased
               || kind == particle_Kind_pool_simd;
    let pool = try_(mem_Alctr_create$particle_Pool($trace gpa));
    defer_(mem_Alctr_destroy$particle_Pool($trace gpa, pool));
    try_(particle_Pool_init(pool, gpa, worker_count, dynamic));
    defer_(particle_Pool_fini(pool));
    var state = (particle_State){
        .particles = particles,
        .executor = {
            .ctx = pool,
            .parallelFn = particle_poolRun,
        },
    };
    particle_State_init(&state);
    particle_State_simulate(
        &state, clock, frame_count, u8_l(particle_example_title)
    );
    return_ok({});
} $unguarded(fn);

fn_((particle_run(proc_Entry entry))(E$void) $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    let available_workers = pri_max(
        usize_(1),
        catch_((thrd_cpuCount())($ignore, usize_(1)))
    );
    var_(arg_mem, A$$(64, u8)) = {};
    var args = proc_Args_iter(entry.args);
    let_ignore = catch_((proc_Args_Iter_skip(&args))($ignore, false));
    var worker_count = pri_clamp(
        particle_nextUsize(&args, A_ref$((S$u8)(arg_mem)), available_workers),
        usize_(1),
        particle_max_workers
    );
    let frame_count = particle_nextUsize(
        &args, A_ref$((S$u8)(arg_mem)), usize_(1000)
    );

    let kind = particle_example_kind;
    if (kind == particle_Kind_pool_half) {
        worker_count = pri_max(usize_(1), worker_count / 2);
    }
    let particles = try_(mem_Alctr_alloc$particle_Item(
        $trace gpa,
        particle_count
    ));
    defer_(mem_Alctr_free$particle_Item($trace gpa, particles));
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));

    if (kind == particle_Kind_transient) {
        var transient = (particle_TransientCtx){
            .gpa = gpa,
            .worker_count = worker_count,
        };
        var state = (particle_State){
            .particles = particles,
            .executor = {
                .ctx = &transient,
                .parallelFn = particle_transientRun,
            },
        };
        particle_State_init(&state);
        particle_State_simulate(
            &state, clock, frame_count, u8_l(particle_example_title)
        );
    } else {
        try_(particle_runPooled(
            gpa, particles, clock, frame_count, kind, worker_count
        ));
    }
    return_ok({});
} $unguarded(fn);

fn_((main(proc_Entry entry))(E$void)) {
    return particle_run(entry);
};
