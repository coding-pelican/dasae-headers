#include "Self.h"
#include "../time/self/internal.h"
#include "dh/heap/vmem.h"
#include "dh/meta.h"

typedef struct thrd__Start {
    var_(clsr, Clsr$raw*);
    var_(ret_type, TypeInfo);
    var_(gpa, mem_Alctr);
} thrd__Start;
$attr($must_check)
$static fn_((thrd__startAlloc(thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type))(mem_E$u_P$raw));
$static fn_((thrd__startFree(thrd__Start* start))(void));

/*========== Internal Declarations ==========================================*/

pp_if_(pp_true)(pp_then_(
    $attr($inline_always $maybe_unused)
    $static fn_((thrd__unsupported_handle(thrd_Self self))(thrd_Handle));
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((thrd__unsupported_yield(void))(thrd_E$void));
    $attr($inline_always $maybe_unused)
    $static fn_((thrd__unsupported_currId(void))(thrd_Id));
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((thrd__unsupported_cpuCount(void))(thrd_E$usize));
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((thrd__unsupported_getName(thrd_Self self, thrd_NameBuf* buf_ptr))(thrd_E$O$S_const$u8));
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((thrd__unsupported_setName(thrd_Self self, S_const$u8 name))(thrd_E$void));
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((thrd__unsupported_spawn(thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type))(thrd_spawn_E$thrd_Self));
    $attr($inline_always $maybe_unused)
    $static fn_((thrd__unsupported_detach(thrd_Self self))(void));
    $attr($inline_always $maybe_unused)
    $static fn_((thrd__unsupported_join(thrd_Self self))(Clsr$raw*));
));
pp_if_(thrd_use_pthread)(pp_then_(
    $attr($inline_always)
    $static fn_((thrd__pthread_handle(thrd_Self self))(thrd_Handle));
    $attr($inline_always $must_check)
    $static fn_((thrd__pthread_yield(void))(thrd_E$void));
    $attr($inline_always)
    $static fn_((thrd__pthread_currId(void))(thrd_Id));
    $attr($inline_always $must_check)
    $static fn_((thrd__pthread_cpuCount(void))(thrd_E$usize));
    $attr($inline_always $must_check)
    $static fn_((thrd__pthread_getName(thrd_Self self, thrd_NameBuf* buf_ptr))(thrd_E$O$S_const$u8));
    $attr($inline_always $must_check)
    $static fn_((thrd__pthread_setName(thrd_Self self, S_const$u8 name))(thrd_E$void));
    $attr($inline_always $must_check)
    $static fn_((thrd__pthread_spawn(thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type))(thrd_spawn_E$thrd_Self));
    $attr($inline_always)
    $static fn_((thrd__pthread_detach(thrd_Self self))(void));
    $attr($inline_always)
    $static fn_((thrd__pthread_join(thrd_Self self))(Clsr$raw*));
));
pp_if_(plat_is_windows)(pp_then_(
    $attr($inline_always)
    $static fn_((thrd__windows_handle(thrd_Self self))(thrd_Handle));
    $attr($inline_always $must_check)
    $static fn_((thrd__windows_yield(void))(thrd_E$void));
    $attr($inline_always)
    $static fn_((thrd__windows_currId(void))(thrd_Id));
    $attr($inline_always $must_check)
    $static fn_((thrd__windows_cpuCount(void))(thrd_E$usize));
    $attr($inline_always $must_check)
    $static fn_((thrd__windows_getName(thrd_Self self, thrd_NameBuf* buf_ptr))(thrd_E$O$S_const$u8));
    $attr($inline_always $must_check)
    $static fn_((thrd__windows_setName(thrd_Self self, S_const$u8 name))(thrd_E$void));
    $attr($inline_always $must_check)
    $static fn_((thrd__windows_spawn(thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type))(thrd_spawn_E$thrd_Self));
    $attr($inline_always)
    $static fn_((thrd__windows_detach(thrd_Self self))(void));
    $attr($inline_always)
    $static fn_((thrd__windows_join(thrd_Self self))(Clsr$raw*));
));
pp_if_(plat_is_linux)(pp_then_(
    $attr($inline_always)
    $static fn_((thrd__linux_handle(thrd_Self self))(thrd_Handle));
    $attr($inline_always $must_check)
    $static fn_((thrd__linux_yield(void))(thrd_E$void));
    $attr($inline_always)
    $static fn_((thrd__linux_currId(void))(thrd_Id));
    $attr($inline_always $must_check)
    $static fn_((thrd__linux_cpuCount(void))(thrd_E$usize));
    $attr($inline_always $must_check)
    $static fn_((thrd__linux_getName(thrd_Self self, thrd_NameBuf* buf_ptr))(thrd_E$O$S_const$u8));
    $attr($inline_always $must_check)
    $static fn_((thrd__linux_setName(thrd_Self self, S_const$u8 name))(thrd_E$void));
    $attr($inline_always $must_check)
    $static fn_((thrd__linux_spawn(thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type))(thrd_spawn_E$thrd_Self));
    $attr($inline_always)
    $static fn_((thrd__linux_detach(thrd_Self self))(void));
    $attr($inline_always)
    $static fn_((thrd__linux_join(thrd_Self self))(Clsr$raw*));
));
pp_if_(plat_is_wasi)(pp_then_(
    $attr($inline_always)
    $static fn_((thrd__wasi_handle(thrd_Self self))(thrd_Handle));
    $attr($inline_always $must_check)
    $static fn_((thrd__wasi_yield(void))(thrd_E$void));
    $attr($inline_always)
    $static fn_((thrd__wasi_currId(void))(thrd_Id));
    $attr($inline_always $must_check)
    $static fn_((thrd__wasi_cpuCount(void))(thrd_E$usize));
    $attr($inline_always $must_check)
    $static fn_((thrd__wasi_getName(thrd_Self self, thrd_NameBuf* buf_ptr))(thrd_E$O$S_const$u8));
    $attr($inline_always $must_check)
    $static fn_((thrd__wasi_setName(thrd_Self self, S_const$u8 name))(thrd_E$void));
    $attr($inline_always $must_check)
    $static fn_((thrd__wasi_spawn(thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type))(thrd_spawn_E$thrd_Self));
    $attr($inline_always)
    $static fn_((thrd__wasi_detach(thrd_Self self))(void));
    $attr($inline_always)
    $static fn_((thrd__wasi_join(thrd_Self self))(Clsr$raw*));
));

$static let thrd__handle = pp_if_(thrd_use_pthread)(
    pp_then_(thrd__pthread_handle),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(thrd__windows_handle),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(thrd__linux_handle),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(thrd__wasi_handle),
                pp_else_(thrd__unsupported_handle)
            ))
        ))
    )));
$static let thrd__yield = pp_if_(thrd_use_pthread)(
    pp_then_(thrd__pthread_yield),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(thrd__windows_yield),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(thrd__linux_yield),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(thrd__wasi_yield),
                pp_else_(thrd__unsupported_yield)
            ))
        ))
    )));
$static let thrd__currId = pp_if_(thrd_use_pthread)(
    pp_then_(thrd__pthread_currId),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(thrd__windows_currId),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(thrd__linux_currId),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(thrd__wasi_currId),
                pp_else_(thrd__unsupported_currId)
            ))
        ))
    )));
$static let thrd__cpuCount = pp_if_(thrd_use_pthread)(
    pp_then_(thrd__pthread_cpuCount),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(thrd__windows_cpuCount),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(thrd__linux_cpuCount),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(thrd__wasi_cpuCount),
                pp_else_(thrd__unsupported_cpuCount)
            ))
        ))
    )));
$static let thrd__getName = pp_if_(thrd_use_pthread)(
    pp_then_(thrd__pthread_getName),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(thrd__windows_getName),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(thrd__linux_getName),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(thrd__wasi_getName),
                pp_else_(thrd__unsupported_getName)
            ))
        ))
    )));
$static let thrd__setName = pp_if_(thrd_use_pthread)(
    pp_then_(thrd__pthread_setName),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(thrd__windows_setName),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(thrd__linux_setName),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(thrd__wasi_setName),
                pp_else_(thrd__unsupported_setName)
            ))
        ))
    )));
$static let thrd__spawn = pp_if_(thrd_use_pthread)(
    pp_then_(thrd__pthread_spawn),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(thrd__windows_spawn),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(thrd__linux_spawn),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(thrd__wasi_spawn),
                pp_else_(thrd__unsupported_spawn)
            ))
        ))
    )));
$static let thrd__detach = pp_if_(thrd_use_pthread)(
    pp_then_(thrd__pthread_detach),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(thrd__windows_detach),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(thrd__linux_detach),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(thrd__wasi_detach),
                pp_else_(thrd__unsupported_detach)
            ))
        ))
    )));
$static let thrd__join = pp_if_(thrd_use_pthread)(
    pp_then_(thrd__pthread_join),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(thrd__windows_join),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(thrd__linux_join),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(thrd__wasi_join),
                pp_else_(thrd__unsupported_join)
            ))
        ))
    )));

/*========== External Definitions ===========================================*/

fn_((thrd_handle(thrd_Self self))(thrd_Handle)) {
    return thrd__handle(self);
};

fn_((thrd_yield(void))(thrd_E$void)) {
    return thrd__yield();
};

fn_((thrd_currId(void))(thrd_Id)) {
    return thrd__currId();
};

fn_((thrd_cpuCount(void))(thrd_E$usize)) {
    return thrd__cpuCount();
};

fn_((thrd_getName(thrd_Self self, thrd_NameBuf* buf_ptr))(thrd_E$O$S_const$u8)) {
    return thrd__getName(self, buf_ptr);
};

fn_((thrd_setName(thrd_Self self, S_const$u8 name))(thrd_E$void)) {
    return thrd__setName(self, name);
};

fn_((thrd_spawn(thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type))(thrd_spawn_E$thrd_Self)) {
    return thrd__spawn(cfg, clsr, ret_type);
};

fn_((thrd_detach(thrd_Self self))(void)) {
    return thrd__detach(self);
};

fn_((thrd_join(thrd_Self self))(Clsr$raw*)) {
    return thrd__join(self);
};

/*========== Internal Definitions ===========================================*/

fn_((thrd__startAlloc(thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type))(mem_E$u_P$raw) $scope) {
    let gpa = orelse_((cfg.gpa)(return_err(E_cause$Unexpected())));
    let raw = try_(mem_Alctr_create($trace gpa, typeInfo$(thrd__Start)));
    let start = u_castP$((thrd__Start*)(raw));
    asg_l((start)({
        .clsr = clsr,
        .ret_type = ret_type,
        .gpa = gpa,
    }));
    return_ok(u_anyP(start));
} $unscoped(fn);

fn_((thrd__startFree(thrd__Start* start))(void)) {
    let gpa = start->gpa;
    mem_Alctr_destroy($trace gpa, u_anyP(start));
};

/* --- Unsupported --- */

fn_((thrd__unsupported_handle(thrd_Self self))(thrd_Handle)) {
    return self.handle;
};

fn_((thrd__unsupported_yield(void))(thrd_E$void) $scope) {
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__unsupported_currId(void))(thrd_Id)) {
    return 0;
};

fn_((thrd__unsupported_cpuCount(void))(thrd_E$usize) $scope) {
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__unsupported_getName(thrd_Self self, thrd_NameBuf* buf_ptr))(thrd_E$O$S_const$u8) $scope) {
    let_ignore = self;
    let_ignore = buf_ptr;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__unsupported_setName(thrd_Self self, S_const$u8 name))(thrd_E$void) $scope) {
    let_ignore = self;
    let_ignore = name;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__unsupported_spawn(thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type))(thrd_spawn_E$thrd_Self) $scope) {
    let_ignore = cfg;
    let_ignore = clsr;
    let_ignore = ret_type;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__unsupported_detach(thrd_Self self))(void)) {
    let_ignore = self;
};

fn_((thrd__unsupported_join(thrd_Self self))(Clsr$raw*)) {
    return ensureNonnull(self.clsr);
};

/* --- Pthreads --- */

#if thrd_use_pthread
fn_((thrd__pthread_handle(thrd_Self self))(thrd_Handle)) {
    return self.handle;
};

fn_((thrd__pthread_yield(void))(thrd_E$void) $scope) {
    if (sched_yield() != 0) {
        return_err(E_cause$thrd_SystemResources()); /* TODO: Replace to specific error */
    }
    return_ok({});
} $unscoped(fn);

fn_((thrd__pthread_currId(void))(thrd_Id)) {
    return pp_switch_((plat_type)(
        pp_case_((plat_type_linux)(thrd__linux_currId())),
        pp_case_((plat_type_darwin)(local_({
            var_(tid, u64) = 0;
            pthread_threadid_np(null, &tid);
            local_return_(tid);
        }))),
        pp_default_(as$(thrd_Id)(pthread_self()))
    ));
};

fn_((thrd__pthread_cpuCount(void))(thrd_E$usize) $scope) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)(thrd__linux_cpuCount())),
        pp_case_((plat_type_darwin)({
            var_(count, i32) = 0;
            var_(len, usize) = sizeOf$(i32);
            if (sysctlbyname("hw.logicalcpu", &count, &len, null, 0) == 0) {
                return_ok(as$(usize)(count));
            }
            return_err(E_cause$thrd_SystemResources()); /* TODO: Replace to specific error */
        })),
        pp_default_({
            let count = sysconf(_SC_NPROCESSORS_ONLN);
            if (count < 1) {
                return_err(E_cause$thrd_SystemResources()); /* TODO: Replace to specific error */
            }
            return_ok(as$(usize)(count));
        })
    ));
} $unscoped(fn);


#if defined(PTHREAD_STACK_MIN)
#define thrd__pthread_defined_stack_min pp_true
#else
#define thrd__pthread_defined_stack_min pp_false
#endif
#define thrd__pthread_stack_size_min pp_if_(thrd__pthread_defined_stack_min)( \
    pp_then_(PTHREAD_STACK_MIN), \
    pp_else_(64 * 1024) \
)

$static fn_((thrd__pthread_entry(P$raw arg))(P$raw));
fn_((thrd__pthread_spawn(thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type))(thrd_spawn_E$thrd_Self) $guard) {
    let start = u_castP$((thrd__Start*)(try_(thrd__startAlloc(cfg, clsr, ret_type))));
    pthread_attr_t attr = 0;
    pthread_attr_init(&attr);
    defer_(pthread_attr_destroy(&attr));
    let stack_size = as$(usize)(pri_max(thrd__pthread_stack_size_min, cfg.stack_size));
    pthread_attr_setstacksize(&attr, stack_size);
    var_(handle, pthread_t) = 0;
    if (pthread_create(&handle, &attr, thrd__pthread_entry, start) != 0) {
        thrd__startFree(start);
        return_err(E_cause$thrd_SystemResources()); /* TODO: Replace to specific error */
    }
    return_ok({
        .handle = handle,
        .clsr = clsr,
        .inner = null,
    });
} $unguarded(fn);

fn_((thrd__pthread_entry(P$raw arg))(P$raw)) {
    let start = ensureNonnull(as$(thrd__Start*)(arg));
    let clsr = ensureNonnull(start->clsr);
    let ret_type = start->ret_type;
    thrd__startFree(start);
    thrd_invokeToCompletion(clsr, ret_type);
    return null;
};

fn_((thrd__pthread_detach(thrd_Self self))(void)) {
    let_ignore = pthread_detach(self.handle);
};

fn_((thrd__pthread_join(thrd_Self self))(Clsr$raw*)) {
    var_(ret_val, P$raw) = null;
    let_ignore = pthread_join(self.handle, &ret_val);
    return ensureNonnull(self.clsr);
};
#endif /* thrd_use_pthread */

/* --- Windows --- */

#if plat_is_windows
fn_((thrd__windows_handle(thrd_Self self))(thrd_Handle)) {
    return self.handle;
};

fn_((thrd__windows_yield(void))(thrd_E$void) $scope) {
    let_ignore = SwitchToThread(); /* TODO: Use NtYieldExecution instead */
    return_ok({});
} $unscoped(fn);

fn_((thrd__windows_currId(void))(thrd_Id)) {
    return as$(thrd_Id)(GetCurrentThreadId());
};

fn_((thrd__windows_cpuCount(void))(thrd_E$usize) $scope) {
    SYSTEM_INFO sys_info = cleared();
    GetSystemInfo(&sys_info);
    return_ok(as$(usize)(sys_info.dwNumberOfProcessors));
} $unscoped(fn);

fn_((thrd__windows_getName(thrd_Self self, thrd_NameBuf* buf_ptr))(thrd_E$O$S_const$u8) $scope) {
    let_ignore = self;
    let_ignore = buf_ptr;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__windows_setName(thrd_Self self, S_const$u8 name))(thrd_E$void) $scope) {
    let_ignore = self;
    let_ignore = name;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

#define thrd__windows_stack_size_min (64 * 1024)

$attr($callconv_stdcall)
$static fn_((thrd__windows_entry(LPVOID lpParameter))(DWORD));
fn_((thrd__windows_spawn(thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type))(thrd_spawn_E$thrd_Self) $scope) {
    let start = u_castP$((thrd__Start*)(try_(thrd__startAlloc(cfg, clsr, ret_type))));
    let stack_size = as$(usize)(pri_max(thrd__windows_stack_size_min, cfg.stack_size));
    let handle = CreateThread(
        null, stack_size,
        thrd__windows_entry, start,
        CREATE_SUSPENDED | STACK_SIZE_PARAM_IS_A_RESERVATION, null
    );
    if (!handle) {
        thrd__startFree(start);
        return_err(E_cause$thrd_SystemResources()); /* TODO: Replace to specific error */
    }
    ResumeThread(handle);
    return_ok({
        .handle = handle,
        .clsr = clsr,
        .inner = null,
    });
} $unscoped(fn);

fn_((thrd__windows_entry(LPVOID lpParameter))(DWORD)) {
    let start = ensureNonnull(as$(thrd__Start*)(lpParameter));
    let clsr = ensureNonnull(start->clsr);
    let ret_type = start->ret_type;
    thrd__startFree(start);
    clsr_invokeToComplete(clsr, ret_type);
    return 0;
};

fn_((thrd__windows_detach(thrd_Self self))(void)) {
    let handle = ensureNonnull(self.handle);
    claim_assert(CloseHandle(handle));
};

fn_((thrd__windows_join(thrd_Self self))(Clsr$raw*)) {
    let handle = ensureNonnull(self.handle);
    switch (WaitForSingleObject(handle, INFINITE)) {
    case WAIT_OBJECT_0: break; /* Thread signaled termination */
    case WAIT_ABANDONED: $fallthrough; /* Mutex only, not for threads */
    case WAIT_TIMEOUT: $fallthrough; /* with INFINITE delay */
    case WAIT_FAILED: $fallthrough; /* Invalid handle or logic error */
    default_() claim_unreachable $end(default);
    }
    claim_assert(CloseHandle(handle));
    return ensureNonnull(self.clsr);
};
#endif /* plat_is_windows */

/* --- Linux --- */

#if plat_is_linux
#include <sys/syscall.h>
#include <sys/mman.h>
#include <linux/sched.h>
#include <linux/futex.h>
#include <unistd.h>
#include <errno.h>

typedef enum_((thrd__linux_Completion $fits($packed))(
    thrd__linux_Completion_running = 0,
    thrd__linux_Completion_detached = 1,
    thrd__linux_Completion_completed = 2
)) thrd__linux_Completion;
T_use_atom_V$(thrd__linux_Completion);
T_use_atom_V$(i32);

/// Linux-specific thread metadata, placed at the top of mmap'd memory.
///
/// Memory layout:
///   [guard page][stack grows down ...][thrd__linux_Meta]
///                                      ^-- stack_top points here
///
/// This struct is internal to Linux impl.
typedef struct thrd__linux_Meta {
    var_(clsr, Clsr$raw*);
    var_(ret_type, TypeInfo);
    var_(map, S$raw); // mmap'd memory region
    var_(completion, atom_V$thrd__linux_Completion); // State machine: running/detached/completed
    var_(parent_tid, i32); // Set by CLONE_PARENT_SETTID
    var_(child_tid, atom_V$i32); // Cleared by CLONE_CHILD_CLEARTID, used for futex
} thrd__linux_Meta;

/// Get thread handle (tid).
/// Returns `parent_tid` from metadata.
fn_((thrd__linux_handle(thrd_Self self))(thrd_Handle)) {
    return self.handle;
};

fn_((thrd__linux_yield(void))(thrd_E$void) $scope) {
    if (syscall(SYS_sched_yield) != 0) {
        return_err(E_cause$thrd_SystemResources()); /* TODO: Replace to specific error */
    }
    return_ok({});
} $unscoped(fn);

fn_((thrd__linux_currId(void))(thrd_Id)) {
    return as$(thrd_Id)(syscall(SYS_gettid));
};

fn_((thrd__linux_cpuCount(void))(thrd_E$usize) $scope) {
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    if (sched_getaffinity(0, sizeOf$(TypeOf(cpu_set)), &cpu_set) != 0) {
        return_err(E_cause$thrd_SystemResources()); /* TODO: Replace to specific error */
    }
    return_ok(as$(usize)(CPU_COUNT(&cpu_set)));
} $unscoped(fn);

fn_((thrd__linux_getName(thrd_Self self, thrd_NameBuf* buf_ptr))(thrd_E$O$S_const$u8) $scope) {
#if UNUSED_CODE
    let meta = as$(thrd__linux_Meta*)(self.inner);
    // Use /proc/self/task/{tid}/comm
    char path[64];
    snprintf(path, sizeOf$(path), "/proc/self/task/%d/comm", meta->parent_tid);
    let fd = open(path, O_RDONLY);
    if (fd < 0) {
        return_err(E_cause$thrd_SystemResources()); /* TODO: Replace to specific error */
    }
    defer_(close(fd));
    let n = read(fd, buf_ptr->buf, thrd_max_name_len);
    if (n < 0) {
        return_err(E_cause$thrd_SystemResources()); /* TODO: Replace to specific error */
    }
    // Remove trailing newline
    if (n > 0 && buf_ptr->buf[n - 1] == '\n') {
        n--;
    }
    buf_ptr->buf[n] = '\0';
    return_ok(some(S_const$u8, ((S_const$u8){ .ptr = buf_ptr->buf, .len = as$(usize)(n) })));
#endif /* UNUSED_CODE */
    /* TODO: Implement */
    let_ignore = self;
    let_ignore = buf_ptr;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__linux_setName(thrd_Self self, S_const$u8 name))(thrd_E$void) $scope) {
#if UNUSED_CODE
    let meta = as$(thrd__linux_Meta*)(self.inner);
    if (name.len > thrd_max_name_len) {
        return_err(E_cause$thrd_InvalidArg());
    }
    // Use /proc/self/task/{tid}/comm
    char path[64];
    snprintf(path, sizeOf$(path), "/proc/self/task/%d/comm", meta->parent_tid);
    let fd = open(path, O_WRONLY);
    if (fd < 0) {
        return_err(E_cause$thrd_SystemResources()); /* TODO: Replace to specific error */
    }
    defer_(close(fd));
    if (write(fd, name.ptr, name.len) < 0) {
        return_err(E_cause$thrd_SystemResources()); /* TODO: Replace to specific error */
    }
    return_ok({});
#endif /* UNUSED_CODE */
    /* TODO: Implement */
    let_ignore = self;
    let_ignore = name;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

#define thrd__linux_clone_flags \
    /* Note: CLONE_DETACHED is deprecated and ignored on modern kernels */ \
    /* clang-format off */ ( \
    CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM \
    | CLONE_SETTLS | CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID \
) /* clang-format on */

/// Thread entry point.
///
/// After user function returns, atomically swap completion state:
/// - If was 'running', becomes 'completed' -> return normally (join will cleanup)
/// - If was 'detached', thread must self-cleanup via freeAndExit()
$static fn_((thrd__linux_entry(P$raw arg))(i32));
fn_((thrd__linux_spawn(thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type))(thrd_spawn_E$thrd_Self) $guard) {
    claim_assert_nonnull(clsr);
    let_ignore = ret_type;
    let page_size = as$(usize)(sysconf(_SC_PAGESIZE));
    let stack_size = mem_alignFwd(pri_max(page_size, cfg.stack_size), page_size);

    /*
     * Memory layout:
     *   [guard page][stack area][thrd__linux_Meta]
     *   |<- map_base            |<- meta_offset  |<- map_base + map_size
     *
     * Stack grows downward from meta_offset.
     */
    let meta_size = mem_alignFwd(sizeOf$(thrd__linux_Meta), alignOf$(thrd__linux_Meta));
    let map_size = page_size + stack_size + meta_size;

    // Reserve entire region as inaccessible first
    let map_base = orelse_((heap_vmem_reserve(null, map_size))(null));
    if (map_base == null) {
        return_err(E_cause$thrd_SystemResources()); /* TODO: Replace to specific error */
    }
    errdefer_(let_ignore = heap_vmem_release(map_base, map_size));

    // Commit stack + meta area (keep guard page reserved/inaccessible)
    let stack_start = as$(u8*)(map_base) + page_size;
    if (!heap_vmem_commit(stack_start, stack_size + meta_size)) {
        return_err(E_cause$thrd_SystemResources()); /* TODO: Replace to specific error */
    }

    // Initialize metadata at the top of mapped region
    let meta = as$(thrd__linux_Meta*)(as$(u8*)(map_base) + page_size + stack_size);
    *meta = (thrd__linux_Meta){
        .clsr = clsr,
        .ret_type = ret_type,
        .map = {
            .ptr = map_base,
            .len = map_size,
        },
        .completion = atom_V_init(thrd__linux_Completion_running),
        .parent_tid = 0,
        .child_tid = atom_V_init(1), // Non-zero initial value
    };

    // Stack top is just below the meta struct
    let stack_top = as$(P$raw)(meta);
    // Clone the thread
    let tid = clone(
        thrd__linux_entry, stack_top,
        thrd__linux_clone_flags, meta, // arg to entry function
        &meta->parent_tid, // parent_tid (CLONE_PARENT_SETTID)
        null, // tls (CLONE_SETTLS) - could add TLS support
        &meta->child_tid.value // child_tid (CLONE_CHILD_CLEARTID)
    );
    if (tid == -1) {
        return_err(E_cause$thrd_SystemResources()); /* TODO: Replace to specific error */
    }

    return_ok({
        .handle = as$(thrd_Handle)(meta->parent_tid),
        .clsr = clsr,
        .inner = meta,
    });
} $unguarded(fn);

$attr($no_return)
$static fn_((thrd__linux_freeAndExit(thrd__linux_Meta* meta))(void));
fn_((thrd__linux_entry(P$raw arg))(i32)) {
    let meta = ensureNonnull(as$(thrd__linux_Meta*)(arg));
    thrd_invokeToCompletion(ensureNonnull(meta->clsr), meta->ret_type);
    // Atomic state transition
    let prev = atom_V_fetchXchg(&meta->completion, thrd__linux_Completion_completed, memory_order_seq_cst);
    switch (prev) {
    case_((thrd__linux_Completion_running)) /* Normal case */
        return 0 $end(case); /* join() will handle cleanup */
    case_((thrd__linux_Completion_detached)) /* Thread was detached while running */
        thrd__linux_freeAndExit(meta) $end(case); /* must self-cleanup, noreturn */
    case thrd__linux_Completion_completed: $fallthrough;
    default_() claim_unreachable $end(default);
    }
    return 0;
};

/// Detach a thread.
///
/// Atomically swap completion state to 'detached':
/// - If was 'running', thread will self-cleanup when done
/// - If was 'completed', we must cleanup now (same as join)
///
fn_((thrd__linux_detach(thrd_Self self))(void)) {
    let meta = as$(thrd__linux_Meta*)(self.inner);
    let prev = atom_V_fetchXchg(&meta->completion, thrd__linux_Completion_detached, memory_order_seq_cst);
    switch (prev) {
    case_((thrd__linux_Completion_running)) /* Thread still running */
        break $end(case); /* it will self-cleanup */
    case_((thrd__linux_Completion_completed)) /* Thread already finished */
        let_ignore = heap_vmem_release(meta->map.ptr, meta->map.len);
        break $end(case); /* self-cleanup (like join but discard result) */
    case_((thrd__linux_Completion_detached)) $fallthrough;
    default_() claim_unreachable $end(default);
    }
};

/// Join a thread (wait for completion and cleanup).
///
/// Uses futex to wait for child_tid to become 0 (set by CLONE_CHILD_CLEARTID).
fn_((thrd__linux_join(thrd_Self self))(Clsr$raw*)) {
    let meta = as$(thrd__linux_Meta*)(self.inner);
    /* Wait for thread to exit (child_tid becomes 0) */
    while (true) {
        let tid = atom_V_load(&meta->child_tid, memory_order_seq_cst);
        if (tid == 0) { break; }
        // Futex wait - will wake when kernel clears child_tid
        syscall(SYS_futex, &meta->child_tid.value, FUTEX_WAIT | FUTEX_PRIVATE_FLAG, tid, null, null, 0);
        // Ignore return value - spurious wakeups are fine, we'll check tid again
    }
    // Thread has exited, safe to unmap
    let_ignore = heap_vmem_release(meta->map.ptr, meta->map.len);
    return ensureNonnull(self.clsr);
};

fn_((thrd__linux_freeAndExit(thrd__linux_Meta* meta))(void)) pp_switch_((arch_family_type)(
    pp_case_((arch_family_type_x86)(pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_(map_base, P$raw) $reg(rdi) = meta->map.ptr;
            asm_var_(map_size, usize) $reg(rsi) = meta->map.len;
            asm_volatile(
                "movl $11, %%eax\n\t" // SYS_munmap
                "syscall\n\t"
                "movl $60, %%eax\n\t" // SYS_exit
                "xor %%rdi, %%rdi\n\t" // exit code 0
                "syscall" : : "r"(map_base),
                "r"(map_size) : "memory", "rax"
            );
            claim_unreachable;
        })),
        pp_case_((arch_type_x86)({
            asm_var_(map_base, P$raw) $reg(ebx) = meta->map.ptr;
            asm_var_(map_size, usize) $reg(ecx) = meta->map.len;
            asm_volatile(
                "movl $91, %%eax\n\t" // SYS_munmap
                "int $0x80\n\t"
                "movl $1, %%eax\n\t" // SYS_exit
                "movl $0, %%ebx\n\t"
                "int $0x80" : : "r"(map_base),
                "r"(map_size) : "memory", "eax"
            );
            claim_unreachable;
        }))
    )))),
    pp_case_((arch_family_type_arm)(pp_switch_((arch_type)(
        pp_case_((arch_type_aarch64)({
            asm_var_(map_base, P$raw) $reg(x0) = meta->map.ptr;
            asm_var_(map_size, usize) $reg(x1) = meta->map.len;
            asm_volatile(
                "mov x8, #215\n\t" // SYS_munmap
                "svc 0\n\t"
                "mov x8, #93\n\t" // SYS_exit
                "mov x0, #0\n\t"
                "svc 0" : : "r"(map_base),
                "r"(map_size) : "memory", "x8"
            );
            claim_unreachable;
        })),
        pp_case_((arch_type_arm)({
            asm_var_(map_base, P$raw) $reg(r0) = meta->map.ptr;
            asm_var_(map_size, usize) $reg(r1) = meta->map.len;
            asm_volatile(
                "mov r7, #91\n\t" // SYS_munmap
                "svc 0\n\t"
                "mov r7, #1\n\t" // SYS_exit
                "mov r0, #0\n\t"
                "svc 0" : : "r"(map_base),
                "r"(map_size) : "memory", "r7"
            );
            claim_unreachable;
        }))
    )))),
    pp_case_((arch_family_type_riscv)({
        asm_var_(map_base, P$raw) $reg(a0) = meta->map.ptr;
        asm_var_(map_size, usize) $reg(a1) = meta->map.len;
        asm_volatile(
            "li a7, 215\n\t" // SYS_munmap
            "ecall\n\t"
            "li a7, 93\n\t" // SYS_exit
            "li a0, 0\n\t"
            "ecall" : : "r"(map_base),
            "r"(map_size) : "memory", "a7"
        );
        claim_unreachable;
    }))
));
#endif /* plat_is_linux */

/* --- WASI --- */

#if plat_is_wasi
fn_((thrd__wasi_handle(thrd_Self self))(thrd_Handle)) {
    /* TODO: Implement */
    return self.handle;
};

fn_((thrd__wasi_yield(void))(thrd_E$void) $scope) {
    /* TODO: Implement */
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__wasi_currId(void))(thrd_Id)) {
    /* TODO: Implement */
    return 0;
};

fn_((thrd__wasi_cpuCount(void))(thrd_E$usize) $scope) {
    /* TODO: Implement */
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__wasi_getName(thrd_Self self, thrd_NameBuf* buf_ptr))(thrd_E$O$S_const$u8) $scope) {
    /* TODO: Implement */
    let_ignore = self;
    let_ignore = buf_ptr;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__wasi_setName(thrd_Self self, S_const$u8 name))(thrd_E$void) $scope) {
    /* TODO: Implement */
    let_ignore = self;
    let_ignore = name;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__wasi_spawn(thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type))(thrd_spawn_E$thrd_Self) $scope) {
    /* TODO: Implement */
    let_ignore = cfg;
    let_ignore = clsr;
    let_ignore = ret_type;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__wasi_detach(thrd_Self self))(void)) {
    /* TODO: Implement */
    let_ignore = self;
};

fn_((thrd__wasi_join(thrd_Self self))(Clsr$raw*)) {
    /* TODO: Implement */
    return ensureNonnull(self.clsr);
};
#endif /* plat_is_wasi */
