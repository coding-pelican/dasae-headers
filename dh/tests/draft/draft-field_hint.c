#include "dh/Thrd/common.h"
#include "dh/prl.h"
#include "dh/Thrd.h"
#include "dh/atom.h"

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
                asg_lit((&maybe_task)(some(atS(pool->tasks, tail % mp_max_task_count))));
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
    mp_ThrdPool pool = ({
        typedef mp_ThrdPool InitType;
        (InitType){
            .workers = ({
                typedef FieldType$(InitType, workers) FieldType;
                (FieldType){};
            }),
            .threads = ({
                typedef FieldType$(InitType, threads) FieldType;
                (FieldType){};
            }),
        };
    });
    let workers = pool.workers;
    let thrds = pool.threads;

    #define $init(/*(_T){_initial...}*/... /*(_T)*/)\
        __op__$init(__op__$init__parseT __VA_ARGS__)
    #define __op__$init(...) __op__$init__emit(__VA_ARGS__)
    #define __op__$init__parseT(_T...) _T,
    #define __op__$init__emit(_T, _initial...) ({ \
        $maybe_unused typedef _T InitType; \
        (InitType) _initial; \
    })

    #define $asg(_val...) , $_asg, (_val)
    #define $field(...) __op__$field(__VA_ARGS__)
    #define __op__$field(...) pp_overload(__op__$field, __VA_ARGS__)(__VA_ARGS__)
    #define __op__$field_1(...) __op__$field_1__emit(__VA_ARGS__)
    #define __op__$field_1__emit(_field_val...) \
        .__op__$field__expand _field_val = ({ \
            $maybe_unused typedef InitType ParentType; \
            $maybe_unused typedef FieldType$(ParentType, __op__$field__expand _field_val) FieldType; \
            _field_val; \
        })
    #define __op__$field_3(...) __op__$field_3__emit(__VA_ARGS__)
    #define __op__$field_3__emit(_field, _op, _val...) \
        .__op__$field__expand _field = ({ \
            $maybe_unused typedef InitType ParentType; \
            $maybe_unused typedef FieldType$(ParentType, __op__$field__expand _field) FieldType; \
            _val; \
        })
    #define __op__$field__expand(...) __VA_ARGS__
    #define $in_field(...) __op__$in_field__emit(__VA_ARGS__)
    #define __op__$in_field(...) pp_overload(__op__$in_field, __VA_ARGS__)(__VA_ARGS__)
    #define __op__$in_field_1(...) __op__$in_field_1__emit(__VA_ARGS__)
    #define __op__$in_field_1__emit(_field_val...) \
        .__op__$in_field__expand _field_val = ({ \
            $maybe_unused typedef FieldType ParentType; \
            $maybe_unused typedef FieldType$(ParentType, __op__$in_field__expand _field_val) FieldType; \
            _field_val; \
        })
    #define __op__$in_field_3(...) __op__$in_field_3__emit(__VA_ARGS__)
    #define __op__$in_field_3__emit(_field, _op, _val...) \
        .__op__$in_field__expand _field = ({ \
            $maybe_unused typedef FieldType ParentType; \
            $maybe_unused typedef FieldType$(ParentType, __op__$in_field__expand _field) FieldType; \
            _val; \
        })
    #define __op__$in_field__expand(...) __VA_ARGS__

    asg_lit((&pool)($init((LitType){
        $field((workers)$asg(u_castS$((FieldType)(
            try_(mem_Allocator_alloc(gpa, typeInfo$(InnerType), thrd_count))
        )))),
        $field((threads)$asg(thrds)),
    })));

    let pool = $init((mp_ThrdPool){
        $field((workers)$asg(u_castS$((FieldType)(
            try_(mem_Allocator_alloc(gpa, typeInfo$(InnerType), thrd_count))
        )))),
        $field((threads)$asg(thrds)),
    });


    // let_(pool, mp_ThrdPool*) = try_(u_castE$((ReturnType)(mem_Allocator_create(gpa, typeInfo$(mp_ThrdPool)))));
    // let_(workers, TypeOf(pool->workers)) = u_castS$((TypeOf(pool->workers))(try_(mem_Allocator_alloc(
    //     gpa, typeInfo$(union Thrd_FnCtx$(mp_ThrdPool_worker)), mp_max_task_count))));
    // let_(threads, TypeOf(pool->threads)) = u_castS$((TypeOf(pool->threads))(try_(mem_Allocator_alloc(
    //     gpa, typeInfo$(Thrd), thrd_count))));
    // let_(tasks, TypeOf(pool->tasks)) = u_castS$((TypeOf(pool->tasks))(try_(mem_Allocator_alloc(
    //     gpa, typeInfo$(mp_ThrdPool_Task), mp_max_task_count))));
    // asg_lit((pool)({
    //     .workers = workers,
    //     .threads = threads,
    //     .tasks = tasks,
    //     .head = atom_V_init(0ull),
    //     .tail = atom_V_init(0ull),
    //     .count = atom_V_init(0ull),
    //     .mutex = Thrd_Mtx_init(),
    //     .cond_has_task = Thrd_Cond_init(),
    //     .cond_all_done = Thrd_Cond_init(),
    //     .active_tasks = atom_V_init(0ull),
    //     .running = atom_V_init(true),
    // }));
    // for_(($s(pool->workers), $s(pool->threads))(worker, thread) {
    //     *worker = Thrd_FnCtx_from$((mp_ThrdPool_worker)(pool));
    //     *thread = catch_((Thrd_spawn(
    //         Thrd_SpawnConfig_default, worker->as_raw
    //     ))($ignore, claim_unreachable));
    // });
    return_ok(pool);
} $unscoped_(fn);
