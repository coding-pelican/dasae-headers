#include "dh/prl.h"
#include "dh/meta.h"
#include "dh/atom.h"
#include "dh/Thrd.h"

#include "dh/io/stream.h"
#include "dh/time/Instant.h"

#define mp_max_thrd_count (32ull)
#define mp_max_task_count (mp_max_thrd_count << 3ull)

$static var_(mp__thrd_count, usize) = 3;
$attr($maybe_unused)
$static fn_((mp_setThrdCount(usize thrd_count))(void)) { mp__thrd_count = thrd_count; }
$static fn_((mp_getThrdCount(void))(usize)) { return mp__thrd_count; }

typedef fn_(((*)(usize, u_V$raw))(void) $T) mp_LoopFn;
typedef struct mp_LoopData {
    var_(range, R);
    var_(workerFn, mp_LoopFn);
    var_(params, u_V$raw);
} mp_LoopData;

$static Thrd_fn_(mp_worker, ({ mp_LoopData data; }, Void), ($ignore, args)$scope) {
    let data = args->data;
    for_(((data.range))(i) {
        data.workerFn(i, data.params);
    });
    return_({});
} $unscoped_(Thrd_fn);

$attr($maybe_unused)
$static fn_((mp_parallel_for(R range, mp_LoopFn workerFn, u_V$raw params))(void)) {
    let thrd_count = mp_getThrdCount();

    var_(data_list_buf, A$$(mp_max_thrd_count, mp_LoopData)) = zero$A();
    let data_list = A_slice((data_list_buf)$r(0, thrd_count));
    var_(workers_buf, A$$(mp_max_thrd_count, Thrd_FnCtx$(mp_worker))) = zero$A();
    let workers = A_slice((workers_buf)$r(0, thrd_count));
    var_(threads_buf, A$$(mp_max_thrd_count, Thrd)) = zero$A();
    let threads = A_slice((threads_buf)$r(0, thrd_count));

    let chunk = (range.end - range.begin + thrd_count - 1) / thrd_count;
    for_(($rf(0), $s(workers), $s(data_list))(t, worker, data) {
        data->range.begin = range.begin + chunk * t;
        data->range.end = ((data->range.begin + chunk) <= range.end) ? (data->range.begin + chunk) : range.end;
        data->workerFn = workerFn;
        data->params = params;
        *worker = Thrd_FnCtx_from$((mp_worker)(*data));
    });
    for_(($s(threads), $s(workers))(thread, worker) {
        *thread = catch_((Thrd_spawn(Thrd_SpawnCfg_default, worker->as_raw))(
            $ignore, claim_unreachable
        ));
    });
    for_(($s(threads))(thread) {
        Thrd_join(*thread);
    });
};

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

// ============================================
// ThreadPool 성능 측정 추가
// ============================================

typedef struct mp_ThrdPool_Stats {
    atom_V$$(f64) total_work_time;
    atom_V$$(f64) total_idle_time;
    atom_V$$(f64) total_sync_time;
    atom_V$$(usize) task_count;
    atom_V$$(usize) wait_count;
    atom_V$$(usize) loop_iterations;
    atom_V$$(usize) cond_wait_count;
    atom_V$$(usize) empty_wakeups;
} mp_ThrdPool_Stats;

$static mp_ThrdPool_Stats mp__worker_stats[mp_max_thrd_count];
$static atom_V$$(bool) mp__stats_enabled = atom_V_init(false);

$static fn_((mp_ThrdPool_enableStats(void))(void)) {
    atom_V_store(&mp__stats_enabled, true, atom_MemOrd_release);
    for_(($r(0, mp_max_thrd_count))(i) {
        atom_V_store(&mp__worker_stats[i].total_work_time, 0.0, atom_MemOrd_monotonic);
        atom_V_store(&mp__worker_stats[i].total_idle_time, 0.0, atom_MemOrd_monotonic);
        atom_V_store(&mp__worker_stats[i].total_sync_time, 0.0, atom_MemOrd_monotonic);
        atom_V_store(&mp__worker_stats[i].task_count, 0ull, atom_MemOrd_monotonic);
        atom_V_store(&mp__worker_stats[i].wait_count, 0ull, atom_MemOrd_monotonic);
        atom_V_store(&mp__worker_stats[i].loop_iterations, 0ull, atom_MemOrd_monotonic);
        atom_V_store(&mp__worker_stats[i].cond_wait_count, 0ull, atom_MemOrd_monotonic);
        atom_V_store(&mp__worker_stats[i].empty_wakeups, 0ull, atom_MemOrd_monotonic);
    });
};

$static fn_((mp_ThrdPool_printStats(mp_ThrdPool* pool))(void)) {
    if (!atom_V_load(&mp__stats_enabled, atom_MemOrd_acquire)) { return; }
    io_stream_println(u8_l("\n=== ThreadPool Performance Stats ==="));

    f64 total_work = 0.0;
    f64 total_idle = 0.0;
    f64 total_sync = 0.0;
    usize total_tasks = 0;
    for_(($r(0, pool->threads.len))(i) {
        let work = atom_V_load(&mp__worker_stats[i].total_work_time, atom_MemOrd_monotonic);
        let idle = atom_V_load(&mp__worker_stats[i].total_idle_time, atom_MemOrd_monotonic);
        let sync = atom_V_load(&mp__worker_stats[i].total_sync_time, atom_MemOrd_monotonic);
        let tasks = atom_V_load(&mp__worker_stats[i].task_count, atom_MemOrd_monotonic);
        let loops = atom_V_load(&mp__worker_stats[i].loop_iterations, atom_MemOrd_monotonic);
        let waits = atom_V_load(&mp__worker_stats[i].cond_wait_count, atom_MemOrd_monotonic);
        let empty = atom_V_load(&mp__worker_stats[i].empty_wakeups, atom_MemOrd_monotonic);

        let total = work + idle + sync;
        let efficiency = (total > 0.0) ? (work / total * 100.0) : 0.0;

        io_stream_println(
            u8_l("Worker {:uz}: {:.1fl}% eff | Tasks: {:uz} | Loops: {:uz} | Waits: {:uz} | Empty: {:uz}"),
            i, efficiency, tasks, loops, waits, empty
        );

        total_work += work;
        total_idle += idle;
        total_sync += sync;
        total_tasks += tasks;
    });

    let total = total_work + total_idle + total_sync;
    let avg_efficiency = (total > 0.0) ? (total_work / total * 100.0) : 0.0;

    io_stream_println(u8_l("\n--- Summary ---"));
    io_stream_println(u8_l("Average Efficiency: {:.1fl}%"), avg_efficiency);
    io_stream_println(u8_l("Total Work Time: {:.2fl}s"), total_work);
    io_stream_println(u8_l("Total Idle Time: {:.2fl}s ({:.1fl}%)"), total_idle, (total_idle / total * 100.0));
    io_stream_println(u8_l("Total Sync Time: {:.2fl}s ({:.1fl}%)"), total_sync, (total_sync / total * 100.0));
    io_stream_println(u8_l("Total Tasks: {:uz}"), total_tasks);
    io_stream_println(u8_l("Avg Tasks per Worker: {:.1fl}"), as$(f64)(total_tasks) / as$(f64)(pool->threads.len));
};

// Worker 함수 수정 (측정 추가)
$static Thrd_fn_(mp_ThrdPool_worker, ({ mp_ThrdPool* pool; usize worker_id; }, Void), ($ignore, args)$scope) {
    let pool = args->pool;
    let worker_id = args->worker_id;

    while (atom_V_load(&pool->running, atom_MemOrd_acquire)) {
        // Track loop iterations
        atom_V_fetchAdd(&mp__worker_stats[worker_id].loop_iterations, 1, atom_MemOrd_monotonic);

        let idle_start = time_Instant_now();

        O$$(P$mp_ThrdPool_Task) maybe_task = none();

        let sync_start = time_Instant_now();
        with_fini_(Thrd_Mtx_lock(&pool->mutex), Thrd_Mtx_unlock(&pool->mutex)) {
            while (atom_V_load(&pool->count, atom_MemOrd_acquire) == 0
                   && atom_V_load(&pool->running, atom_MemOrd_acquire)) {
                atom_V_fetchAdd(&mp__worker_stats[worker_id].cond_wait_count, 1, atom_MemOrd_monotonic);
                Thrd_Cond_wait(&pool->cond_has_task, &pool->mutex);
            }

            if (atom_V_load(&pool->count, atom_MemOrd_monotonic) > 0) {
                let tail = atom_V_load(&pool->tail, atom_MemOrd_monotonic);
                asg_lit((&maybe_task)(some(S_at((pool->tasks)[tail % mp_max_task_count]))));
                atom_V_store(&pool->tail, tail + 1, atom_MemOrd_monotonic);
                atom_V_fetchSub(&pool->count, 1, atom_MemOrd_monotonic);
                atom_V_fetchAdd(&pool->active_tasks, 1, atom_MemOrd_monotonic);
            } else {
                // Woke up but no task available
                atom_V_fetchAdd(&mp__worker_stats[worker_id].empty_wakeups, 1, atom_MemOrd_monotonic);
            }
        }
        let sync_end = time_Instant_now();

        if_some((maybe_task)(task)) {
            let work_start = time_Instant_now();
            task->workerFn(task->range, task->params);
            let work_end = time_Instant_now();

            let stats_enabled = atom_V_load(&mp__stats_enabled, atom_MemOrd_acquire);
            if (stats_enabled) {
                let idle_time = time_Duration_asSecs$f64(time_Instant_durationSince(sync_start, idle_start));
                let sync_time = time_Duration_asSecs$f64(time_Instant_durationSince(sync_end, sync_start));
                let work_time = time_Duration_asSecs$f64(time_Instant_durationSince(work_end, work_start));

                atom_V_fetchAdd(&mp__worker_stats[worker_id].total_idle_time, idle_time, atom_MemOrd_monotonic);
                atom_V_fetchAdd(&mp__worker_stats[worker_id].total_sync_time, sync_time, atom_MemOrd_monotonic);
                atom_V_fetchAdd(&mp__worker_stats[worker_id].total_work_time, work_time, atom_MemOrd_monotonic);
                atom_V_fetchAdd(&mp__worker_stats[worker_id].task_count, 1, atom_MemOrd_monotonic);
            }

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

// ThreadPool 초기화 수정 (worker_id 전달)
$attr($must_check)
$static fn_((mp_ThrdPool_init(mem_Allocator gpa, usize thrd_count))(E$P$mp_ThrdPool) $guard) {
    let_(pool, mp_ThrdPool*) = u_castP$((mp_ThrdPool*)(try_((mem_Allocator_create(gpa, typeInfo$(InnerType))))));
    let field_types = typeInfos$(S_InnerT$(FieldType$(mp_ThrdPool, workers)), S_InnerT$(FieldType$(mp_ThrdPool, threads)));
    let field_mem = try_(mem_Allocator_create(gpa, u_typeInfoRecordN(thrd_count, field_types)));
    errdefer_($ignore, mem_Allocator_destroy(gpa, field_mem));
    let fields = u_fieldSlisMut(field_mem, thrd_count, field_types, A_ref$((S$u_S$raw)((A$$(2, u_S$raw)){})));
    let workers = u_castS$((FieldType$(mp_ThrdPool, workers))(*S_at((fields)[0])));
    let threads = u_castS$((FieldType$(mp_ThrdPool, threads))(*S_at((fields)[1])));
    asg_lit((pool)($init((LitType){
        $field((workers)),
        $field((threads)),
        $field((tasks)$asg(u_castS$((FieldType)(try_(mem_Allocator_alloc(
            gpa, typeInfo$(InnerType), mp_max_task_count
        )))))),
        .head = atom_V_init(0ull),
        .tail = atom_V_init(0ull),
        .count = atom_V_init(0ull),
        .mutex = Thrd_Mtx_init(),
        .cond_has_task = Thrd_Cond_init(),
        .cond_all_done = Thrd_Cond_init(),
        .active_tasks = atom_V_init(0ull),
        .running = atom_V_init(true),
    })));
    claim_assert_fmt(pool->workers.len == thrd_count, "workers.len(%zu) != thrd_count(%zu)", pool->workers.len, thrd_count);
    claim_assert_fmt(pool->threads.len == thrd_count, "threads.len(%zu) != thrd_count(%zu)", pool->threads.len, thrd_count);
    for_(($s(pool->workers), $s(pool->threads), $r(0, thrd_count))(worker, thread, i) {
        *worker = Thrd_FnCtx_from$((mp_ThrdPool_worker)(pool, i));
        *thread = try_(Thrd_spawn(Thrd_SpawnCfg_default, worker->as_raw));
    });
    return_ok(pool);
} $unguarded_(fn);

$static fn_((mp_ThrdPool_shutdown(mp_ThrdPool* self))(void)) {
    atom_V_store(&self->running, false, atom_MemOrd_release);
    Thrd_Mtx_lock(&self->mutex);
    Thrd_Cond_broadcast(&self->cond_has_task);
    Thrd_Mtx_unlock(&self->mutex);
    for_(($s(self->threads))(thread) { Thrd_join(*thread); });
    Thrd_Mtx_fini(&self->mutex);
    Thrd_Cond_fini(&self->cond_has_task);
    Thrd_Cond_fini(&self->cond_all_done);
};

$static fn_((mp_ThrdPool_fini(mp_ThrdPool** self, mem_Allocator gpa))(void)) {
    mp_ThrdPool_shutdown(*self);
    mem_Allocator_free(gpa, u_anyS((*self)->tasks));
    // Free the contiguous field memory block (workers array is at the start of the block)
    let field_types = typeInfos$(S_InnerT$(FieldType$(mp_ThrdPool, workers)), S_InnerT$(FieldType$(mp_ThrdPool, threads)));
    let field_mem = u_recordNPtrMut(u_anyS((*self)->workers), (*self)->workers.len, field_types, 0);
    mem_Allocator_destroy(gpa, field_mem);
    mem_Allocator_destroy(gpa, u_anyP(*self));
    *self = null;
};

$attr($maybe_unused)
$static fn_((mp_ThrdPool_submit(mp_ThrdPool* self, R range, mp_ThrdPool_TaskFn fn, u_V$raw params))(void)) {
    Thrd_Mtx_lock(&self->mutex);
    let head = atom_V_load(&self->head, atom_MemOrd_acquire);
    let task = S_at((self->tasks)[head % mp_max_task_count]);
    task->range = range;
    task->workerFn = fn;
    task->params = params;
    atom_V_store(&self->head, head + 1, atom_MemOrd_release);
    atom_V_fetchAdd(&self->count, 1, atom_MemOrd_release);
    Thrd_Cond_signal(&self->cond_has_task);
    Thrd_Mtx_unlock(&self->mutex);
};

$static fn_((mp_ThrdPool_submitBatch(mp_ThrdPool* self, mp_ThrdPool_Task* tasks, usize task_count))(void)) {
    Thrd_Mtx_lock(&self->mutex); // Lock ONCE for all tasks
    for_(($r(0, task_count))(i) {
        let task = tasks + i;
        let head = atom_V_load(&self->head, atom_MemOrd_monotonic); // relaxed inside lock
        *S_at((self->tasks)[head % mp_max_task_count]) = *task;
        atom_V_store(&self->head, head + 1, atom_MemOrd_monotonic);
    });
    atom_V_fetchAdd(&self->count, task_count, atom_MemOrd_release);
    Thrd_Cond_broadcast(&self->cond_has_task); // Wake all threads at once
    Thrd_Mtx_unlock(&self->mutex);
};

$static fn_((mp_ThrdPool_waitAll(mp_ThrdPool* self))(void)) {
    Thrd_Mtx_lock(&self->mutex);
    while (atom_V_load(&self->count, atom_MemOrd_acquire) > 0 || atom_V_load(&self->active_tasks, atom_MemOrd_acquire) > 0) {
        Thrd_Cond_wait(&self->cond_all_done, &self->mutex);
    }
    Thrd_Mtx_unlock(&self->mutex);
};

$static fn_((mp_parallel_for_pooled(mp_ThrdPool* pool, R range, mp_ThrdPool_TaskFn fn, u_V$raw params))(void)) {
    let chunk = (range.end - range.begin + pool->threads.len - 1) / pool->threads.len;
    var_(task_buf, A$$(mp_max_thrd_count, mp_ThrdPool_Task));
    usize task_count = 0;

    for_(($r(0, pool->threads.len))(t) {
        let begin = range.begin + chunk * t;
        let end = ((begin + chunk) <= range.end) ? (begin + chunk) : range.end;
        if (begin < end) {
            asg_lit((A_at((task_buf)[task_count++]))({
                .range = $r(begin, end),
                .workerFn = fn,
                .params = params,
            }));
        }
    });
    mp_ThrdPool_submitBatch(pool, task_buf.val, task_count);
    mp_ThrdPool_waitAll(pool);
};

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
};
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
};

// ============================================
// 시뮬레이션 설정
// ============================================

#include "dh/math/vec.h"

#define State_particles_log2 (23ull)
#define State_particles (1ull << State_particles_log2) // 2^20 = 1,048,576
// #define State_cell_size (20.0)                         // Was 10.0
// #define State_grid_width (100ull)                      // Was 200
// #define State_grid_height (100ull)                     // Was 200
// #define State_max_particles_per_cell (31ull)           // Was 32
#define State_cell_size (10.0) // 20.0 → 10.0
#define State_grid_width (200ull) // 100 → 200
#define State_grid_height (200ull) // 100 → 200
#define State_max_particles_per_cell (3ull) // 16 → 63

#define State_boundary_radius (500.0)
#define State_gravity (9.81)
#define State_damping (0.98)
#define State_target_fps (30.0)
#define State_delta_time (1.0 / 30.0)

typedef struct Particle {
    m_V2f64 pos;
    m_V2f64 vel;
    f64 mass;
} Particle;
T_use_S$(Particle);

typedef struct Cell {
    var_(count, usize);
    var_(indices, A$$(State_max_particles_per_cell, usize));
} Cell;
T_use_S$(Cell);

typedef struct LocalGrid {
    var_(cells, A$$(State_grid_width * State_grid_height, Cell));
} LocalGrid;

typedef enum SimPhase {
    SimPhase_idle = 0,
    SimPhase_build_grid,
    SimPhase_update_positions,
    SimPhase_handle_collisions,
    SimPhase_shutdown,
} SimPhase;

typedef struct State {
    mp_ThrdPool* pool; // 비교용으로 유지
    S$Particle particles;
    S$Cell grid;
} State;

#define State_enable_randomization 0

$static fn_((State_rng(void))(Rand*));
$static fn_((State_rngGaussian(void))(RandGaussian*));
$static fn_((State_initParticles_worker(R range, u_V$raw params))(void));
$static fn_((State_initParticles(mp_ThrdPool* pool, S$Particle particles, m_V2f64 center, m_V2f64 radius_a_b))(void));
$static fn_((State_init(mp_ThrdPool* pool, S$Particle particles, S$Cell grid))(State)) {
    State_initParticles(pool, particles, m_V2f64_zero, m_V2f64_of(State_boundary_radius, 200.0));
    return lit$((State){
        .pool = pool,
        .particles = particles,
        .grid = grid,
    });
};

// ============================================
// Spatial Grid 구축
// ============================================

$static fn_((State_clearGrid_worker(R range, u_V$raw params))(void));
$static fn_((State_clearGrid(mp_ThrdPool* pool, S$Cell grid))(void));
$static fn_((State_hashPosition(m_V2f64 pos))(usize));
$static fn_((State_buildSpatialGrid_worker(R range, u_V$raw params))(void));
$static fn_((State_buildSpatialGrid(State* self))(void));

// ============================================
// 중력 적용 및 위치 업데이트
// ============================================

$static fn_((State_applyGravityAndUpdate_worker(R range, u_V$raw params))(void));
$static fn_((State_applyGravityAndUpdate(State* self))(void));

// ============================================
// 충돌 처리
// ============================================

$static fn_((State_handleCollisions_worker(R range, u_V$raw params))(void));
$static fn_((State_handleCollisions(State* self))(void));

// ============================================
// 시뮬레이션 루프
// ============================================

#define enable_print_frame_stats 1

$static fn_((State_simulate(State* self, mp_ThrdPool* pool, usize frame_amount))(void)) {
    io_stream_println(u8_l("\nStarting simulation for {:uz} frames at {:.1fl} FPS..."), frame_amount, State_target_fps);
    io_stream_println(u8_l("Using {:uz} threads (Pool)"), pool->threads.len);

    f64 total_time = 0.0;
    f64 build_grid_time = 0.0;
    f64 gravity_update_time = 0.0;
    f64 collision_time = 0.0;
    for_(($r(0, frame_amount))(frame) {
        let frame_start = time_Instant_now();
        let t0 = time_Instant_now();
        State_buildSpatialGrid(self);
        let t1 = time_Instant_now();
        build_grid_time += time_Duration_asSecs$f64(time_Instant_durationSince(t1, t0));
        State_applyGravityAndUpdate(self);
        let t2 = time_Instant_now();
        gravity_update_time += time_Duration_asSecs$f64(time_Instant_durationSince(t2, t1));
        State_handleCollisions(self);
        let t3 = time_Instant_now();
        collision_time += time_Duration_asSecs$f64(time_Instant_durationSince(t3, t2));
        let frame_end = time_Instant_now();
        let frame_time = time_Instant_durationSince(frame_end, frame_start);
        total_time += time_Duration_asSecs$f64(frame_time);
        if (frame % as$(usize)(State_target_fps) == 0) {
            pp_if_(enable_print_frame_stats)(pp_then_(io_stream_println), pp_else_(pp_ignore))(
                u8_l("Frame {:uz}: {:.2fl} ms ({:.1fl} FPS) | Avg: {:.2fl} ms ({:.1fl} FPS)"),
                frame, time_Duration_asSecs$f64(frame_time) * 1000.0, 1.0 / time_Duration_asSecs$f64(frame_time),
                (total_time / intToFlt$((f64)(frame + 1))) * 1000.0, intToFlt$((f64)(frame + 1)) / total_time
            );
        }
    });

    io_stream_println(
        u8_l("\n=== Performance Breakdown ===")
    );
    io_stream_println(
        u8_l("Build Grid:     {:.2fl} ms ({:.1fl}%)"),
        (build_grid_time / as$(f64)(frame_amount)) * 1000.0,
        (build_grid_time / total_time) * 100.0
    );
    io_stream_println(
        u8_l("Gravity+Update: {:.2fl} ms ({:.1fl}%)"),
        (gravity_update_time / as$(f64)(frame_amount)) * 1000.0,
        (gravity_update_time / total_time) * 100.0
    );
    io_stream_println(
        u8_l("Collisions:     {:.2fl} ms ({:.1fl}%)"),
        (collision_time / as$(f64)(frame_amount)) * 1000.0,
        (collision_time / total_time) * 100.0
    );
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
};

// ============================================
// 메인
// ============================================

#include "dh/main.h"
#include "dh/fmt/common.h"
#include "dh/heap/Page.h"
#include "dh/io/common.h"

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    io_stream_println(u8_l("╔═══════════════════════════════════════╗"));
    io_stream_println(u8_l("║  Particle Simulation                  ║"));
    io_stream_println(u8_l("║  Thrd + pool + atom (opt)             ║"));
    io_stream_println(u8_l("╚═══════════════════════════════════════╝\n"));

    io_stream_println(u8_l("Particles: 2^{:uz} = {:uz}"), State_particles_log2, State_particles);
    io_stream_println(u8_l("Target FPS: {:.1f}"), State_target_fps);
    io_stream_println(u8_l("Cell Size: {:.1fl}"), State_cell_size);
    io_stream_println(u8_l("Grid Width: {:uz}"), State_grid_width);
    io_stream_println(u8_l("Grid Height: {:uz}"), State_grid_height);
    io_stream_println(u8_l("Max Particles per Cell: {:uz}"), State_max_particles_per_cell);
    io_stream_println(u8_l("Boundary Radius: {:.1fl}"), State_boundary_radius);
    io_stream_println(u8_l("Gravity: {:.2fl}"), State_gravity);
    io_stream_println(u8_l("Damping: {:.2fl}"), State_damping);
    io_stream_println(u8_l("Delta Time: {:.2fl}"), State_delta_time);
    let max_cpu_count = usize_subSat(catch_((Thrd_cpuCount())($ignore, 2)), 2) + 1;
    let cpu_count = prim_clamp(
        expr_(usize $scope)(if (1 < args.len) {
            $break_(catch_((fmt_parse$usize(*S_at((args)[1]), 10))($ignore, usize_limit_max)));
        }) expr_(else)({
            $break_(usize_limit_max);
        }) $unscoped_(expr),
        as$(usize)(1), max_cpu_count
    );
    var gpa = heap_Page_allocator(&(heap_Page){});
    var pool = try_(mp_ThrdPool_init(gpa, cpu_count));
    defer_(mp_ThrdPool_fini(&pool, gpa));
    mp_ThrdPool_enableStats();
    defer_(mp_ThrdPool_printStats(pool));
    io_stream_println(u8_l("Using Thread Pool with {:uz} workers\n"), pool->threads.len);

    io_stream_println(u8_l("Allocating memory..."));
    let particles = try_(u_castE$((E$$(S$Particle))(mem_Allocator_alloc(gpa, typeInfo$(Particle), State_particles))));
    defer_(mem_Allocator_free(gpa, u_anyS(particles)));
    let grid = try_(u_castE$((E$$(S$Cell))(mem_Allocator_alloc(gpa, typeInfo$(Cell), State_grid_width * State_grid_height))));
    defer_(mem_Allocator_free(gpa, u_anyS(grid)));
    io_stream_println(u8_l("Memory allocated: {:.2fl} MB"), as$(f64)(State_particles * typeInfo$(Particle).size) / (1024.0 * 1024.0));

    var state = State_init(pool, particles, grid);
    let frame_amount = expr_(usize $scope)(if (2 < args.len) {
        $break_(catch_((fmt_parse$usize(*S_at((args)[2]), 10))($ignore, 1000)));
    }) expr_(else)({
        $break_(1000);
    }) $unscoped_(expr);
    io_stream_println(u8_l("Simulating {:uz} frames..."), frame_amount);
    State_simulate(&state, pool, frame_amount);

    io_stream_println(u8_l("\nSample particles:"));
    for_(($r(0, 3), $s(particles))(i, particle) {
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
};

fn_((State_rngGaussian(void))(RandGaussian*)) {
    static $Thrd_local RandGaussian rng = RandGaussian_default;
    return &rng;
};

typedef struct State_InitParticlesArgs {
    S$Particle particles;
    m_V2f64 center;
    m_V2f64 radius_a_b;
} State_InitParticlesArgs;

fn_((State_initParticles_worker(R range, u_V$raw params))(void)) {
    static $Thrd_local bool c_initialized = false;
    if (!c_initialized) {
        let seed = Thrd_currentId() ^ range.begin;
        *State_rng() = pp_if_(State_enable_randomization)(
            pp_then_(Rand_initSeed(seed)),
            pp_else_(Rand_withSeed(Rand_default, seed)));
        *State_rngGaussian() = RandGaussian_init(*State_rng());
        c_initialized = true;
    }

    let args = u_castV$((State_InitParticlesArgs)(params));
    let particles = args.particles;
    let targets = slice$S(particles, range);
    let c = args.center;
    let r_a_b = args.radius_a_b;
    let r_a = r_a_b.x;
    let r_b = r_a_b.y;

    for_(($s(targets))(particle) {
        var self_i = *particle;
        var r = RandGaussian_next$f64(State_rngGaussian(), 0.0, r_b / 2.5);
        if (r > r_b) { r = r_b; }
        if (r < 0) { r = -r; }
        let theta = m_V2f64_sincos(Rand_next$f64(State_rng()) * math_f64_tau);
        var p = m_V2f64_mul(m_V2f64_addScalar(c, r), theta);
        let d = m_V2f64_len(p);
        if (d > r_a) {
            m_V2f64_scalAsg(&p, r_a / d);
        }
        self_i.pos = p;
        self_i.vel = m_V2f64_zero;
        self_i.mass = 1.0;
        *particle = self_i;
    });
};

fn_((State_initParticles(mp_ThrdPool* pool, S$Particle particles, m_V2f64 center, m_V2f64 radius_a_b))(void)) {
    mp_parallel_for_pooled(
        pool, $r(0, State_particles), State_initParticles_worker,
        u_anyV(lit$((State_InitParticlesArgs){
            .particles = particles,
            .center = center,
            .radius_a_b = radius_a_b,
        }))
    );
};

typedef struct State_ClearGridArgs {
    S$Cell grid;
} State_ClearGridArgs;

fn_((State_clearGrid_worker(R range, u_V$raw params))(void)) {
    let args = u_castV$((State_ClearGridArgs)(params));
    let grid = sliceS(args.grid, range);
    for_(($s(grid))(cell) {
        cell->count = 0;
    });
};

fn_((State_clearGrid(mp_ThrdPool* pool, S$Cell grid))(void)) {
    mp_parallel_for_pooled(
        pool, $r(0, State_grid_width * State_grid_height), State_clearGrid_worker,
        u_anyV(lit$((State_ClearGridArgs){
            .grid = grid,
        }))
    );
};

fn_((State_hashPosition(m_V2f64 pos))(usize)) {
    let gx_0 = as$(isize)((pos.x + (State_grid_width * State_cell_size) / 2.0f) / State_cell_size);
    let gy_0 = as$(isize)((pos.y + (State_grid_height * State_cell_size) / 2.0f) / State_cell_size);
    let gx_1 = as$(usize)(prim_clamp(gx_0, 0, as$(isize)(State_grid_width)-1));
    let gy_1 = as$(usize)(prim_clamp(gy_0, 0, as$(isize)(State_grid_height)-1));
    return gy_1 * State_grid_width + gx_1;
};

typedef struct State_BuildGridArgs {
    S$Particle particles;
    S$Cell grid; // shared global grid
} State_BuildGridArgs;

fn_((State_buildSpatialGrid_worker(R range, u_V$raw params))(void)) {
    typedef struct LocalGrid {
        var_(cells, A$$(State_grid_width * State_grid_height, Cell));
    } LocalGrid;
    $static $Thrd_local LocalGrid local_grid = {};
    mem_setBytes0(mem_asBytes(&local_grid));
    let args = u_castV$((State_BuildGridArgs)(params));
    let particles = args.particles;
    let grid = args.grid;

    for_(((range), $s(particles))(i, particle) {
        let cell_idx = State_hashPosition(particle->pos);
        let local_cell = A_at((local_grid.cells)[cell_idx]);
        if (local_cell->count < State_max_particles_per_cell) {
            *A_at((local_cell->indices)[local_cell->count++]) = i;
        }
    });

    for_(($r(0, State_grid_width * State_grid_height))(cell_idx) {
        let local_cell = A_at((local_grid.cells)[cell_idx]);
        if (local_cell->count == 0) { continue; }
        let global_cell = S_at((grid)[cell_idx]);
        // 전역 그리드에 공간 예약 (atomic 1회)
        let global_pos = atom_fetchAdd(&global_cell->count, local_cell->count, atom_MemOrd_acq_rel);
        // 예약된 공간에 로컬 데이터 복사 (atomic 불필요)
        let copy_count = prim_min(local_cell->count, State_max_particles_per_cell - global_pos);
        for_(($r(0, copy_count))(k) {
            let src_idx = *A_at((local_cell->indices)[k]);
            let dst_pos = global_pos + k;
            if (dst_pos < State_max_particles_per_cell) {
                *A_at((global_cell->indices)[dst_pos]) = src_idx;
            }
        });
    });
};

fn_((State_buildSpatialGrid(State* self))(void)) {
    State_clearGrid(self->pool, self->grid);
    mp_parallel_for_pooled(
        self->pool, $r(0, State_particles), State_buildSpatialGrid_worker,
        u_anyV(lit$((State_BuildGridArgs){
            .particles = self->particles,
            .grid = self->grid,
        }))
    );
};

typedef struct State_ApplyGravityAndUpdateArgs {
    S$Particle particles;
} State_ApplyGravityAndUpdateArgs;

fn_((State_applyGravityAndUpdate_worker(R range, u_V$raw params))(void)) {
    let args = u_castV$((State_ApplyGravityAndUpdateArgs)(params));
    let particles = args.particles;
    let targets = slice$S(particles, range);

    let gravity_dt = State_gravity * State_delta_time;
    let dt = State_delta_time;

    for_(($s(targets))(particle) {
        var self_i = *particle;

        // 중력 적용
        let px = self_i.pos.x;
        let py = self_i.pos.y;
        let d_sq = px * px + py * py;
        let inv_d = (d_sq > 0.01) ? (1.0 / flt_sqrt(d_sq)) : 0.0;
        let force = gravity_dt / (d_sq + 0.01);

        self_i.vel.x = (self_i.vel.x - px * inv_d * force) * State_damping;
        self_i.vel.y = (self_i.vel.y - py * inv_d * force) * State_damping;

        // 즉시 위치 업데이트 (메모리 접근 1회로 감소)
        self_i.pos.x += self_i.vel.x * dt;
        self_i.pos.y += self_i.vel.y * dt;

        // 경계 처리
        let d = flt_sqrt(self_i.pos.x * self_i.pos.x + self_i.pos.y * self_i.pos.y);
        if (d > State_boundary_radius) {
            let inv_d = 1.0 / d;
            let nx = self_i.pos.x * inv_d;
            let ny = self_i.pos.y * inv_d;

            self_i.pos.x = nx * State_boundary_radius;
            self_i.pos.y = ny * State_boundary_radius;

            let dot = self_i.vel.x * nx + self_i.vel.y * ny;
            self_i.vel.x -= nx * dot * 2.0;
            self_i.vel.y -= ny * dot * 2.0;
            self_i.vel.x *= 0.8;
            self_i.vel.y *= 0.8;
        }

        *particle = self_i;
    });
};

fn_((State_applyGravityAndUpdate(State* self))(void)) {
    mp_parallel_for_pooled(
        self->pool, $r(0, State_particles), State_applyGravityAndUpdate_worker,
        u_anyV(lit$((State_ApplyGravityAndUpdateArgs){
            .particles = self->particles,
        }))
    );
};

typedef struct State_HandleCollisionsArgs {
    S$Particle particles;
    S$Cell grid;
} State_HandleCollisionsArgs;

fn_((State_handleCollisions_worker(R range, u_V$raw params))(void)) {
    let args = u_castV$((State_HandleCollisionsArgs)(params));
    let particles = args.particles;
    let targets = slice$S(particles, range);
    let grid = args.grid;
    let collision_radius = 0.5;
    let collision_radius_sq = collision_radius * collision_radius;
    let check_radius_sq = (collision_radius * 3.0) * (collision_radius * 3.0); // 셀 범위

    for_(((range), $s(targets))(i, particle) {
        var self_i = *particle;
        let cell_idx = State_hashPosition(self_i.pos);
        let gx = cell_idx % State_grid_width;
        let gy = cell_idx / State_grid_width;

        for_(($r(0, 3))(dy_offset) {
            for_(($r(0, 3))(dx_offset) {
                let dx = as$(isize)(dx_offset)-1;
                let dy = as$(isize)(dy_offset)-1;
                let nx = as$(isize)(gx) + dx;
                let ny = as$(isize)(gy) + dy;

                if ((nx < 0 || State_grid_width <= as$(usize)(nx)) || (ny < 0 || State_grid_height <= as$(usize)(ny))) {
                    continue;
                }

                let neighbor_idx = as$(usize)(ny)*State_grid_width + as$(usize)(nx);
                let neighbor = S_at((grid)[neighbor_idx]);

                // Relaxed load (충돌 단계에서는 grid가 변하지 않음)
                let count = neighbor->count;
                if (count == 0) { continue; }

                let limit = prim_min(count, State_max_particles_per_cell);

                // 셀 중심까지의 거리로 빠른 컬링
                let cell_center_x = (as$(f64)(nx)-100.0) * State_cell_size + State_cell_size * 0.5;
                let cell_center_y = (as$(f64)(ny)-100.0) * State_cell_size + State_cell_size * 0.5;
                let dcx = cell_center_x - self_i.pos.x;
                let dcy = cell_center_y - self_i.pos.y;
                let dist_to_cell_sq = dcx * dcx + dcy * dcy;

                // 셀이 너무 멀면 스킵
                if (dist_to_cell_sq > check_radius_sq) { continue; }

                for (usize k = 0; k < limit; ++k) {
                    let j = *A_at((neighbor->indices)[k]);
                    if (j <= i) { continue; }

                    let other = *S_at((particles)[j]);
                    let dpx = other.pos.x - self_i.pos.x;
                    let dpy = other.pos.y - self_i.pos.y;
                    let d_sq = dpx * dpx + dpy * dpy;

                    // 상수로 비교 (min_d 계산 제거)
                    let min_d_sq = collision_radius_sq * 4.0; // (2*r)^2

                    if (d_sq < min_d_sq && d_sq > 0.001) {
                        let inv_d = 1.0 / flt_sqrt(d_sq);
                        let d = d_sq * inv_d;
                        let overlap = (collision_radius * 2.0) - d;
                        let nx = dpx * inv_d;
                        let ny = dpy * inv_d;
                        let separation = overlap * 0.5;
                        self_i.pos.x -= nx * separation;
                        self_i.pos.y -= ny * separation;
                    }
                }
            });
        });
        *particle = self_i;
    });
};

fn_((State_handleCollisions(State* self))(void)) {
    mp_parallel_for_pooled(
        self->pool, $r(0, State_particles), State_handleCollisions_worker,
        u_anyV(lit$((State_HandleCollisionsArgs){
            .particles = self->particles,
            .grid = self->grid,
        }))
    );
};

/*
$ dh-c run optimize sample-particle_pool_opt.c
Building sample-particle_pool_opt with config 'optimize'...
Build successful!
Running sample-particle_pool_opt...
╔═══════════════════════════════════════╗
║  Particle Simulation                  ║
║  Thrd + pool + atom (opt)             ║
╚═══════════════════════════════════════╝

Particles: 2^23 = 8388608
Target FPS: 30.0
Cell Size: 10.0
Grid Width: 200
Grid Height: 200
Max Particles per Cell: 3
Boundary Radius: 500.0
Gravity: 9.81
Damping: 0.98
Delta Time: 0.03
Using Thread Pool with 15 workers

Allocating memory...
Memory allocated: 320.00 MB
Simulating 1000 frames...

Starting simulation for 1000 frames at 30.0 FPS...
Using 15 threads (Pool)
Frame 0: 67.89 ms (14.7 FPS) | Avg: 67.89 ms (14.7 FPS)
Frame 30: 70.93 ms (14.1 FPS) | Avg: 69.44 ms (14.4 FPS)
Frame 60: 71.43 ms (13.0 FPS) | Avg: 69.88 ms (14.3 FPS)
Frame 90: 68.83 ms (14.5 FPS) | Avg: 70.03 ms (14.3 FPS)
Frame 120: 71.04 ms (14.1 FPS) | Avg: 70.28 ms (14.2 FPS)
Frame 150: 77.98 ms (12.8 FPS) | Avg: 70.84 ms (14.1 FPS)
Frame 180: 67.70 ms (14.8 FPS) | Avg: 70.60 ms (14.2 FPS)
Frame 210: 68.72 ms (14.6 FPS) | Avg: 70.47 ms (14.2 FPS)
Frame 240: 74.21 ms (13.5 FPS) | Avg: 70.77 ms (14.1 FPS)
Frame 270: 68.07 ms (14.7 FPS) | Avg: 70.91 ms (14.1 FPS)
Frame 300: 72.80 ms (13.7 FPS) | Avg: 70.94 ms (14.1 FPS)
Frame 330: 69.23 ms (14.4 FPS) | Avg: 70.96 ms (14.1 FPS)
Frame 360: 71.64 ms (13.0 FPS) | Avg: 70.87 ms (14.1 FPS)
Frame 390: 70.22 ms (14.2 FPS) | Avg: 70.85 ms (14.1 FPS)
Frame 420: 67.97 ms (14.7 FPS) | Avg: 70.80 ms (14.1 FPS)
Frame 450: 67.11 ms (14.9 FPS) | Avg: 70.73 ms (14.1 FPS)
Frame 480: 71.14 ms (14.1 FPS) | Avg: 70.69 ms (14.1 FPS)
Frame 510: 66.95 ms (14.9 FPS) | Avg: 70.67 ms (14.1 FPS)
Frame 540: 70.12 ms (14.3 FPS) | Avg: 70.66 ms (14.2 FPS)
Frame 570: 67.96 ms (14.7 FPS) | Avg: 70.62 ms (14.2 FPS)
Frame 600: 71.30 ms (14.0 FPS) | Avg: 70.57 ms (14.2 FPS)
Frame 630: 71.16 ms (14.1 FPS) | Avg: 70.53 ms (14.2 FPS)
Frame 660: 71.28 ms (14.0 FPS) | Avg: 70.51 ms (14.2 FPS)
Frame 690: 72.08 ms (13.9 FPS) | Avg: 70.51 ms (14.2 FPS)
Frame 720: 66.16 ms (15.1 FPS) | Avg: 70.54 ms (14.2 FPS)
Frame 750: 72.67 ms (13.8 FPS) | Avg: 70.54 ms (14.2 FPS)
Frame 780: 69.12 ms (14.5 FPS) | Avg: 70.53 ms (14.2 FPS)
Frame 810: 69.60 ms (14.4 FPS) | Avg: 70.54 ms (14.2 FPS)
Frame 840: 75.93 ms (13.2 FPS) | Avg: 70.52 ms (14.2 FPS)
Frame 870: 72.82 ms (13.7 FPS) | Avg: 70.53 ms (14.2 FPS)
Frame 900: 72.62 ms (13.8 FPS) | Avg: 70.53 ms (14.2 FPS)
Frame 930: 70.39 ms (14.2 FPS) | Avg: 70.51 ms (14.2 FPS)
Frame 960: 71.82 ms (13.9 FPS) | Avg: 70.51 ms (14.2 FPS)
Frame 990: 70.93 ms (14.1 FPS) | Avg: 70.57 ms (14.2 FPS)

=== Performance Breakdown ===
Build Grid:     5.72 ms (8.1%)
Gravity+Update: 29.38 ms (41.6%)
Collisions:     35.49 ms (50.3%)

=== Simulation Complete ===

Total frames: 1000
Total time: 70.58 seconds
Average FPS: 14.17
Average frame time: 70.58 ms
Target 30.00 FPS not achieved (14.17 FPS)

Sample particles:
- Particle 0: pos(158.45, 0.00) vel(-0.00, -0.00)
- Particle 1: pos(-89.19, -54.33) vel(0.00, 0.00)
- Particle 2: pos(-83.64, -62.54) vel(0.00, 0.00)

=== Program Complete ===

=== ThreadPool Performance Stats ===
Worker 0: 80.6% eff | Tasks: 4046 | Loops: 4047 | Waits: 3828 | Empty: 0
Worker 1: 80.1% eff | Tasks: 3974 | Loops: 3975 | Waits: 3812 | Empty: 0
Worker 2: 80.3% eff | Tasks: 3968 | Loops: 3969 | Waits: 3824 | Empty: 0
Worker 3: 80.6% eff | Tasks: 4001 | Loops: 4002 | Waits: 3833 | Empty: 0
Worker 4: 80.1% eff | Tasks: 3955 | Loops: 3956 | Waits: 3810 | Empty: 0
Worker 5: 81.3% eff | Tasks: 4019 | Loops: 4020 | Waits: 3828 | Empty: 0
Worker 6: 81.2% eff | Tasks: 3984 | Loops: 3985 | Waits: 3820 | Empty: 0
Worker 7: 80.6% eff | Tasks: 4039 | Loops: 4040 | Waits: 3827 | Empty: 0
Worker 8: 80.3% eff | Tasks: 4035 | Loops: 4036 | Waits: 3821 | Empty: 0
Worker 9: 80.4% eff | Tasks: 4017 | Loops: 4018 | Waits: 3831 | Empty: 0
Worker 10: 80.1% eff | Tasks: 3990 | Loops: 3991 | Waits: 3816 | Empty: 0
Worker 11: 80.4% eff | Tasks: 4013 | Loops: 4014 | Waits: 3820 | Empty: 0
Worker 12: 80.5% eff | Tasks: 4007 | Loops: 4008 | Waits: 3833 | Empty: 0
Worker 13: 81.3% eff | Tasks: 3961 | Loops: 3962 | Waits: 3817 | Empty: 0
Worker 14: 81.2% eff | Tasks: 4006 | Loops: 4007 | Waits: 3811 | Empty: 0

--- Summary ---
Average Efficiency: 80.6%
Total Work Time: 854.48s
Total Idle Time: 0.00s (0.0%)
Total Sync Time: 205.67s (19.4%)
Total Tasks: 60015
Avg Tasks per Worker: 4001.0
*/
