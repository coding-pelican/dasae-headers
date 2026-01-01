#include "dh/Thrd/common.h"
#include "dh/time/Duration.h"

/*========== Internal Declarations ==========================================*/

pp_if_(pp_true)(pp_then_(
    $attr($inline_always $maybe_unused)
    $static fn_((Thrd__unsupported_handle(Thrd self))(Thrd_Handle));
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((Thrd__unsupported_yield(void))(E$void));
    $attr($inline_always $maybe_unused)
    $static fn_((Thrd__unsupported_currentId(void))(Thrd_Id));
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((Thrd__unsupported_cpuCount(void))(E$usize));
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((Thrd__unsupported_getName(Thrd self, Thrd_NameBuf* buf_ptr))(E$O$S_const$u8));
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((Thrd__unsupported_setName(Thrd self, S_const$u8 name))(E$void));
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((Thrd__unsupported_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd));
    $attr($inline_always $maybe_unused)
    $static fn_((Thrd__unsupported_detach(Thrd self))(void));
    $attr($inline_always $maybe_unused)
    $static fn_((Thrd__unsupported_join(Thrd self))(Thrd_FnCtx*));
));
pp_if_(Thrd_use_pthread)(pp_then_(
    $attr($inline_always)
    $static fn_((Thrd__pthread_handle(Thrd self))(Thrd_Handle));
    $attr($inline_always $must_check)
    $static fn_((Thrd__pthread_yield(void))(E$void));
    $attr($inline_always)
    $static fn_((Thrd__pthread_currentId(void))(Thrd_Id));
    $attr($inline_always $must_check)
    $static fn_((Thrd__pthread_cpuCount(void))(E$usize));
    $attr($inline_always $must_check)
    $static fn_((Thrd__pthread_getName(Thrd self, Thrd_NameBuf* buf_ptr))(E$O$S_const$u8));
    $attr($inline_always $must_check)
    $static fn_((Thrd__pthread_setName(Thrd self, S_const$u8 name))(E$void));
    $attr($inline_always $must_check)
    $static fn_((Thrd__pthread_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd));
    $attr($inline_always)
    $static fn_((Thrd__pthread_detach(Thrd self))(void));
    $attr($inline_always)
    $static fn_((Thrd__pthread_join(Thrd self))(Thrd_FnCtx*));
));
pp_if_(plat_is_windows)(pp_then_(
    $attr($inline_always)
    $static fn_((Thrd__windows_handle(Thrd self))(Thrd_Handle));
    $attr($inline_always $must_check)
    $static fn_((Thrd__windows_yield(void))(E$void));
    $attr($inline_always)
    $static fn_((Thrd__windows_currentId(void))(Thrd_Id));
    $attr($inline_always $must_check)
    $static fn_((Thrd__windows_cpuCount(void))(E$usize));
    $attr($inline_always $must_check)
    $static fn_((Thrd__windows_getName(Thrd self, Thrd_NameBuf* buf_ptr))(E$O$S_const$u8));
    $attr($inline_always $must_check)
    $static fn_((Thrd__windows_setName(Thrd self, S_const$u8 name))(E$void));
    $attr($inline_always $must_check)
    $static fn_((Thrd__windows_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd));
    $attr($inline_always)
    $static fn_((Thrd__windows_detach(Thrd self))(void));
    $attr($inline_always)
    $static fn_((Thrd__windows_join(Thrd self))(Thrd_FnCtx*));
));
pp_if_(plat_is_linux)(pp_then_(
    $attr($inline_always)
    $static fn_((Thrd__linux_handle(Thrd self))(Thrd_Handle));
    $attr($inline_always $must_check)
    $static fn_((Thrd__linux_yield(void))(E$void));
    $attr($inline_always)
    $static fn_((Thrd__linux_currentId(void))(Thrd_Id));
    $attr($inline_always $must_check)
    $static fn_((Thrd__linux_cpuCount(void))(E$usize));
    $attr($inline_always $must_check)
    $static fn_((Thrd__linux_getName(Thrd self, Thrd_NameBuf* buf_ptr))(E$O$S_const$u8));
    $attr($inline_always $must_check)
    $static fn_((Thrd__linux_setName(Thrd self, S_const$u8 name))(E$void));
    $attr($inline_always $must_check)
    $static fn_((Thrd__linux_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd));
    $attr($inline_always)
    $static fn_((Thrd__linux_detach(Thrd self))(void));
    $attr($inline_always)
    $static fn_((Thrd__linux_join(Thrd self))(Thrd_FnCtx*));
));
pp_if_(plat_is_wasi)(pp_then_(
    $attr($inline_always)
    $static fn_((Thrd__wasi_handle(Thrd self))(Thrd_Handle));
    $attr($inline_always $must_check)
    $static fn_((Thrd__wasi_yield(void))(E$void));
    $attr($inline_always)
    $static fn_((Thrd__wasi_currentId(void))(Thrd_Id));
    $attr($inline_always $must_check)
    $static fn_((Thrd__wasi_cpuCount(void))(E$usize));
    $attr($inline_always $must_check)
    $static fn_((Thrd__wasi_getName(Thrd self, Thrd_NameBuf* buf_ptr))(E$O$S_const$u8));
    $attr($inline_always $must_check)
    $static fn_((Thrd__wasi_setName(Thrd self, S_const$u8 name))(E$void));
    $attr($inline_always $must_check)
    $static fn_((Thrd__wasi_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd));
    $attr($inline_always)
    $static fn_((Thrd__wasi_detach(Thrd self))(void));
    $attr($inline_always)
    $static fn_((Thrd__wasi_join(Thrd self))(Thrd_FnCtx*));
));

$static let Thrd__handle = pp_if_(Thrd_use_pthread)(
    pp_then_(Thrd__pthread_handle),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(Thrd__windows_handle),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(Thrd__linux_handle),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(Thrd__wasi_handle),
                pp_else_(Thrd__unsupported_handle)
            ))
        ))
    )));
$static let Thrd__yield = pp_if_(Thrd_use_pthread)(
    pp_then_(Thrd__pthread_yield),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(Thrd__windows_yield),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(Thrd__linux_yield),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(Thrd__wasi_yield),
                pp_else_(Thrd__unsupported_yield)
            ))
        ))
    )));
$static let Thrd__currentId = pp_if_(Thrd_use_pthread)(
    pp_then_(Thrd__pthread_currentId),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(Thrd__windows_currentId),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(Thrd__linux_currentId),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(Thrd__wasi_currentId),
                pp_else_(Thrd__unsupported_currentId)
            ))
        ))
    )));
$static let Thrd__cpuCount = pp_if_(Thrd_use_pthread)(
    pp_then_(Thrd__pthread_cpuCount),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(Thrd__windows_cpuCount),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(Thrd__linux_cpuCount),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(Thrd__wasi_cpuCount),
                pp_else_(Thrd__unsupported_cpuCount)
            ))
        ))
    )));
$static let Thrd__getName = pp_if_(Thrd_use_pthread)(
    pp_then_(Thrd__pthread_getName),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(Thrd__windows_getName),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(Thrd__linux_getName),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(Thrd__wasi_getName),
                pp_else_(Thrd__unsupported_getName)
            ))
        ))
    )));
$static let Thrd__setName = pp_if_(Thrd_use_pthread)(
    pp_then_(Thrd__pthread_setName),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(Thrd__windows_setName),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(Thrd__linux_setName),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(Thrd__wasi_setName),
                pp_else_(Thrd__unsupported_setName)
            ))
        ))
    )));
$static let Thrd__spawn = pp_if_(Thrd_use_pthread)(
    pp_then_(Thrd__pthread_spawn),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(Thrd__windows_spawn),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(Thrd__linux_spawn),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(Thrd__wasi_spawn),
                pp_else_(Thrd__unsupported_spawn)
            ))
        ))
    )));
$static let Thrd__detach = pp_if_(Thrd_use_pthread)(
    pp_then_(Thrd__pthread_detach),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(Thrd__windows_detach),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(Thrd__linux_detach),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(Thrd__wasi_detach),
                pp_else_(Thrd__unsupported_detach)
            ))
        ))
    )));
$static let Thrd__join = pp_if_(Thrd_use_pthread)(
    pp_then_(Thrd__pthread_join),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(Thrd__windows_join),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(Thrd__linux_join),
            pp_else_(pp_if_(plat_is_wasi)(
                pp_then_(Thrd__wasi_join),
                pp_else_(Thrd__unsupported_join)
            ))
        ))
    )));

/*========== External Definitions ===========================================*/

fn_((Thrd_sleep(time_Duration duration))(void)) {
    time_sleep(duration);
};

fn_((Thrd_handle(Thrd self))(Thrd_Handle)) {
    return Thrd__handle(self);
};

fn_((Thrd_yield(void))(E$void)) {
    return Thrd__yield();
};

fn_((Thrd_currentId(void))(Thrd_Id)) {
    return Thrd__currentId();
};

fn_((Thrd_cpuCount(void))(E$usize)) {
    return Thrd__cpuCount();
};

fn_((Thrd_getName(Thrd self, Thrd_NameBuf* buf_ptr))(E$O$S_const$u8)) {
    return Thrd__getName(self, buf_ptr);
};

fn_((Thrd_setName(Thrd self, S_const$u8 name))(E$void)) {
    return Thrd__setName(self, name);
};

fn_((Thrd_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd)) {
    return Thrd__spawn(cfg, fn_ctx);
};

fn_((Thrd_detach(Thrd self))(void)) {
    return Thrd__detach(self);
};

fn_((Thrd_join(Thrd self))(Thrd_FnCtx*)) {
    return Thrd__join(self);
};

/*========== Internal Definitions ===========================================*/

/* --- Unsupported --- */

fn_((Thrd__unsupported_handle(Thrd self))(Thrd_Handle)) {
    return ensureNonnull(self.inner)->handle;
};

fn_((Thrd__unsupported_yield(void))(E$void) $scope) {
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd__unsupported_currentId(void))(Thrd_Id)) {
    return 0;
};

fn_((Thrd__unsupported_cpuCount(void))(E$usize) $scope) {
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd__unsupported_getName(Thrd self, Thrd_NameBuf* buf_ptr))(E$O$S_const$u8) $scope) {
    let_ignore = self;
    let_ignore = buf_ptr;
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd__unsupported_setName(Thrd self, S_const$u8 name))(E$void) $scope) {
    let_ignore = self;
    let_ignore = name;
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd__unsupported_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd) $scope) {
    let_ignore = cfg;
    let_ignore = fn_ctx;
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd__unsupported_detach(Thrd self))(void)) {
    let_ignore = self;
};

fn_((Thrd__unsupported_join(Thrd self))(Thrd_FnCtx*)) {
    return ensureNonnull(self.inner);
};

/* --- Pthreads --- */

#if Thrd_use_pthread
fn_((Thrd__pthread_handle(Thrd self))(Thrd_Handle)) {
    return ensureNonnull(self.inner)->handle;
};

fn_((Thrd__pthread_yield(void))(E$void) $scope) {
    if (sched_yield() != 0) {
        return_err(Err_Unspecified()); /* TODO: Replace to specific error */
    }
    return_ok({});
} $unscoped_(fn);

fn_((Thrd__pthread_currentId(void))(Thrd_Id)) {
    return pp_switch_((plat_type)(
        pp_case_((plat_type_linux)(Thrd__linux_currentId())),
        pp_case_((plat_type_darwin)(blk({
            var_(tid, u64) = 0;
            pthread_threadid_np(null, &tid);
            blk_return_(tid);
        }))),
        pp_default_(as$(Thrd_Id)(pthread_self()))
    ));
};

fn_((Thrd__pthread_cpuCount(void))(E$usize) $scope) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)(Thrd__linux_cpuCount())),
        pp_case_((plat_type_darwin)({
            var_(count, i32) = 0;
            var_(len, usize) = sizeOf$(i32);
            if (sysctlbyname("hw.logicalcpu", &count, &len, null, 0) == 0) {
                return_ok(as$(usize)(count));
            }
            return_err(Err_Unexpected()); /* TODO: Replace to specific error */
        })),
        pp_default_({
            let count = sysconf(_SC_NPROCESSORS_ONLN);
            if (count < 1) {
                return_err(Err_Unexpected()); /* TODO: Replace to specific error */
            }
            return_ok(as$(usize)(count));
        })
    ));
} $unscoped_(fn);


#if defined(PTHREAD_STACK_MIN)
#define Thrd__pthread_defined_stack_min pp_true
#else
#define Thrd__pthread_defined_stack_min pp_false
#endif
#define Thrd__pthread_stack_size_min pp_if_(Thrd__pthread_defined_stack_min)( \
    pp_then_(PTHREAD_STACK_MIN), \
    pp_else_(64 * 1024) \
)

$static fn_((Thrd__pthread_entry(P$raw arg))(P$raw));
fn_((Thrd__pthread_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd) $guard) {
    claim_assert_nonnull(fn_ctx);
    pthread_attr_t attr = 0;
    pthread_attr_init(&attr);
    defer_(pthread_attr_destroy(&attr));
    let stack_size = as$(usize)(prim_max(Thrd__pthread_stack_size_min, cfg.stack_size));
    pthread_attr_setstacksize(&attr, stack_size);
    if (pthread_create(as$(pthread_t*)(&fn_ctx->handle), &attr, Thrd__pthread_entry, fn_ctx) != 0) {
        return_err(Err_SystemResources()); /* TODO: Replace to specific error */
    }
    return_ok({ .inner = fn_ctx });
} $unguarded_(fn);

fn_((Thrd__pthread_entry(P$raw arg))(P$raw)) {
    let ctx = ensureNonnull(as$(Thrd_FnCtx*)(arg));
    return call((ctx->fn)(ctx));
};

fn_((Thrd__pthread_detach(Thrd self))(void)) {
    let_ignore = pthread_detach(self.inner->handle);
};

fn_((Thrd__pthread_join(Thrd self))(Thrd_FnCtx*)) {
    var_(ret_val, P$raw) = null;
    let_ignore = pthread_join(self.inner->handle, &ret_val);
    claim_assert(as$(Thrd_FnCtx*)(ret_val) == self.inner);
    return self.inner;
};
#endif /* Thrd_use_pthread */

/* --- Windows --- */

#if plat_is_windows
fn_((Thrd__windows_handle(Thrd self))(Thrd_Handle)) {
    return ensureNonnull(self.inner)->handle;
};

fn_((Thrd__windows_yield(void))(E$void) $scope) {
    let_ignore = SwitchToThread(); /* TODO: Use NtYieldExecution instead */
    return_ok({});
} $unscoped_(fn);

fn_((Thrd__windows_currentId(void))(Thrd_Id)) {
    return as$(Thrd_Id)(GetCurrentThreadId());
};

fn_((Thrd__windows_cpuCount(void))(E$usize) $scope) {
    SYSTEM_INFO sys_info = cleared();
    GetSystemInfo(&sys_info);
    return_ok(as$(usize)(sys_info.dwNumberOfProcessors));
} $unscoped_(fn);

fn_((Thrd__windows_getName(Thrd self, Thrd_NameBuf* buf_ptr))(E$O$S_const$u8) $scope) {
    let_ignore = self;
    let_ignore = buf_ptr;
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd__windows_setName(Thrd self, S_const$u8 name))(E$void) $scope) {
    let_ignore = self;
    let_ignore = name;
    return_err(Err_Unsupported());
} $unscoped_(fn);

#define Thrd__windows_stack_size_min (64 * 1024)

$attr($stdcall)
$static fn_((Thrd__windows_entry(LPVOID lpParameter))(DWORD));
fn_((Thrd__windows_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd) $scope) {
    claim_assert_nonnull(fn_ctx);
    claim_assert_nonnull(fn_ctx->fn);
    let stack_size = as$(usize)(prim_max(Thrd__windows_stack_size_min, cfg.stack_size));
    fn_ctx->handle = CreateThread(
        null, stack_size,
        Thrd__windows_entry, fn_ctx,
        CREATE_SUSPENDED, null
    );
    ResumeThread(fn_ctx->handle);
    if (!fn_ctx->handle) { return_err(Err_Unexpected()); }
    return_ok({ .inner = fn_ctx });
} $unscoped_(fn);

fn_((Thrd__windows_entry(LPVOID lpParameter))(DWORD)) {
    let ctx = ensureNonnull(as$(Thrd_FnCtx*)(lpParameter));
    let_ignore = call((ctx->fn)(ctx));
    return 0;
};

fn_((Thrd__windows_detach(Thrd self))(void)) {
    let fn_ctx = ensureNonnull(self.inner);
    let handle = ensureNonnull(fn_ctx->handle);
    fn_ctx->handle = (claim_assert(CloseHandle(handle)), null);
};

fn_((Thrd__windows_join(Thrd self))(Thrd_FnCtx*)) {
    let fn_ctx = ensureNonnull(self.inner);
    let handle = ensureNonnull(fn_ctx->handle);
    switch (WaitForSingleObject(handle, INFINITE)) {
    case WAIT_OBJECT_0:  break; /* Thread signaled termination */
    case WAIT_ABANDONED: $fallthrough; /* Mutex only, not for threads */
    case WAIT_TIMEOUT:   $fallthrough; /* with INFINITE delay */
    case WAIT_FAILED:    $fallthrough; /* Invalid handle or logic error */
    default_() claim_unreachable $end(default);
    }
    fn_ctx->handle = (claim_assert(CloseHandle(handle)), null);
    return fn_ctx;
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

typedef enum_(Thrd__linux_Completion $bits(8)) {
    Thrd__linux_Completion_running = 0,
    Thrd__linux_Completion_detached = 1,
    Thrd__linux_Completion_completed = 2
} Thrd__linux_Completion;
T_use_atom_V$(Thrd__linux_Completion);
T_use_atom_V$(i32);

/// Linux-specific thread metadata, placed at the top of mmap'd memory.
///
/// Memory layout:
///   [guard page][stack grows down ...][Thrd__linux_Meta]
///                                      ^-- stack_top points here
///
/// This struct is NOT part of Thrd_FnCtx - it's internal to Linux impl.
typedef struct Thrd__linux_Meta {
    var_(fn_ctx, Thrd_FnCtx*); // User-provided context (allocated separately)
    var_(map, S$raw); // mmap'd memory region
    var_(completion, atom_V$Thrd__linux_Completion); // State machine: running/detached/completed
    var_(parent_tid, i32); // Set by CLONE_PARENT_SETTID
    var_(child_tid, atom_V$i32); // Cleared by CLONE_CHILD_CLEARTID, used for futex
} Thrd__linux_Meta;

/// Get thread handle (tid).
/// Returns `parent_tid` from metadata.
fn_((Thrd__linux_handle(Thrd self))(Thrd_Handle)) {
    let meta = ensureNonnull(as$(Thrd__linux_Meta*)(self.inner));
    return as$(Thrd_Handle)(meta->parent_tid);
};

fn_((Thrd__linux_yield(void))(E$void) $scope) {
    if (syscall(SYS_sched_yield) != 0) {
        return_err(Err_SystemResources());
    }
    return_ok({});
} $unscoped_(fn);

fn_((Thrd__linux_currentId(void))(Thrd_Id)) {
    return as$(Thrd_Id)(syscall(SYS_gettid));
};

fn_((Thrd__linux_cpuCount(void))(E$usize) $scope) {
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    if (sched_getaffinity(0, sizeOf$(TypeOf(cpu_set)), &cpu_set) != 0) {
        return_err(Err_SystemResources());
    }
    return_ok(as$(usize)(CPU_COUNT(&cpu_set)));
} $unscoped_(fn);

fn_((Thrd__linux_getName(Thrd self, Thrd_NameBuf* buf_ptr))(E$O$S_const$u8) $scope) {
#if UNUSED_CODE
    let meta = as$(Thrd__linux_Meta*)(self.inner);
    // Use /proc/self/task/{tid}/comm
    char path[64];
    snprintf(path, sizeOf$(path), "/proc/self/task/%d/comm", meta->parent_tid);
    let fd = open(path, O_RDONLY);
    if (fd < 0) {
        return_err(Err_SystemResources());
    }
    defer_(close(fd));
    let n = read(fd, buf_ptr->buf, Thrd_max_name_len);
    if (n < 0) {
        return_err(Err_SystemResources());
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
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd__linux_setName(Thrd self, S_const$u8 name))(E$void) $scope) {
#if UNUSED_CODE
    let meta = as$(Thrd__linux_Meta*)(self.inner);
    if (name.len > Thrd_max_name_len) {
        return_err(Err_InvalidArgument());
    }
    // Use /proc/self/task/{tid}/comm
    char path[64];
    snprintf(path, sizeOf$(path), "/proc/self/task/%d/comm", meta->parent_tid);
    let fd = open(path, O_WRONLY);
    if (fd < 0) {
        return_err(Err_SystemResources());
    }
    defer_(close(fd));
    if (write(fd, name.ptr, name.len) < 0) {
        return_err(Err_SystemResources());
    }
    return_ok({});
#endif /* UNUSED_CODE */
    /* TODO: Implement */
    let_ignore = self;
    let_ignore = name;
    return_err(Err_Unsupported());
} $unscoped_(fn);

#define Thrd__linux_clone_flags \
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
$static fn_((Thrd__linux_entry(P$raw arg))(i32));
fn_((Thrd__linux_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd) $guard) {
    claim_assert_nonnull(fn_ctx);
    claim_assert_nonnull(fn_ctx->fn);
    let page_size = as$(usize)(sysconf(_SC_PAGESIZE));
    let stack_size = mem_alignFwd(prim_max(page_size, cfg.stack_size), page_size);

    /*
     * Memory layout:
     *   [guard page][stack area][Thrd__linux_Meta]
     *   |<- map_base            |<- meta_offset  |<- map_base + map_size
     *
     * Stack grows downward from meta_offset.
     */
    let meta_size = mem_alignFwd(sizeOf$(Thrd__linux_Meta), alignOf$(Thrd__linux_Meta));
    let map_size = page_size + stack_size + meta_size;

    // Map entire region as PROT_NONE first
    let map_base = mmap(null, map_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (map_base == MAP_FAILED) {
        return_err(Err_SystemResources());
    }
    errdefer_(munmap(map_base, map_size));

    // Make stack + meta area readable/writable (keep guard page as PROT_NONE)
    let stack_start = as$(u8*)(map_base) + page_size;
    if (mprotect(stack_start, stack_size + meta_size, PROT_READ | PROT_WRITE) != 0) {
        return_err(Err_SystemResources());
    }

    // Initialize metadata at the top of mapped region
    let meta = as$(Thrd__linux_Meta*)(as$(u8*)(map_base) + page_size + stack_size);
    *meta = (Thrd__linux_Meta){
        .fn_ctx = fn_ctx,
        .map = {
            .ptr = map_base,
            .len = map_size,
        },
        .completion = atom_V_init(Thrd__linux_Completion_running),
        .parent_tid = 0,
        .child_tid = atom_V_init(1), // Non-zero initial value
    };

    // Stack top is just below the meta struct
    let stack_top = as$(P$raw)(meta);
    // Clone the thread
    let tid = clone(
        Thrd__linux_entry, stack_top,
        Thrd__linux_clone_flags, meta, // arg to entry function
        &meta->parent_tid, // parent_tid (CLONE_PARENT_SETTID)
        null, // tls (CLONE_SETTLS) - could add TLS support
        &meta->child_tid.value // child_tid (CLONE_CHILD_CLEARTID)
    );
    if (tid == -1) {
        return_err(Err_SystemResources());
    }

    // Return Thrd with inner pointing to metadata
    // Note: This changes the semantics - inner is now Thrd__linux_Meta*, not Thrd_FnCtx*
    return_ok({ .inner = as$(Thrd_FnCtx*)(meta) });
} $unguarded_(fn);

$attr($return_never)
$static fn_((Thrd__linux_freeAndExit(Thrd__linux_Meta* meta))(void));
fn_((Thrd__linux_entry(P$raw arg))(i32)) {
    let meta = ensureNonnull(as$(Thrd__linux_Meta*)(arg));
    let fn_ctx = ensureNonnull(meta->fn_ctx);
    // Execute user function
    let_ignore = call((fn_ctx->fn)(fn_ctx));
    // Atomic state transition
    let prev = atom_V_fetchXchg(&meta->completion, Thrd__linux_Completion_completed, memory_order_seq_cst);
    switch (prev) {
    case_((Thrd__linux_Completion_running)) /* Normal case */
        return 0 $end(case); /* join() will handle cleanup */
    case_((Thrd__linux_Completion_detached)) /* Thread was detached while running */
        Thrd__linux_freeAndExit(meta) $end(case); /* must self-cleanup, noreturn */
    case Thrd__linux_Completion_completed: $fallthrough;
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
fn_((Thrd__linux_detach(Thrd self))(void)) {
    let meta = as$(Thrd__linux_Meta*)(self.inner);
    let prev = atom_V_fetchXchg(&meta->completion, Thrd__linux_Completion_detached, memory_order_seq_cst);
    switch (prev) {
    case_((Thrd__Completion_running)) /* Thread still running */
        break $end(case); /* it will self-cleanup */
    case_((Thrd__Completion_completed)) /* Thread already finished */
        munmap(meta->map.ptr, meta->map.len);
        break $end(case); /* self-cleanup (like join but discard result) */
    case_((Thrd__Completion_detached)) $fallthrough;
    default_() claim_unreachable $end(default);
    }
};

/// Join a thread (wait for completion and cleanup).
///
/// Uses futex to wait for child_tid to become 0 (set by CLONE_CHILD_CLEARTID).
fn_((Thrd__linux_join(Thrd self))(Thrd_FnCtx*)) {
    let meta = as$(Thrd__linux_Meta*)(self.inner);
    let fn_ctx = meta->fn_ctx;
    /* Wait for thread to exit (child_tid becomes 0) */
    while (true) {
        let tid = atom_V_load(&meta->child_tid, memory_order_seq_cst);
        if (tid == 0) { break; }
        // Futex wait - will wake when kernel clears child_tid
        syscall(SYS_futex, &meta->child_tid.value, FUTEX_WAIT | FUTEX_PRIVATE_FLAG, tid, null, null, 0);
        // Ignore return value - spurious wakeups are fine, we'll check tid again
    }
    // Thread has exited, safe to unmap
    munmap(meta->map.ptr, meta->map.len);
    return fn_ctx;
};

fn_((Thrd__linux_freeAndExit(Thrd__linux_Meta* meta))(void)) pp_switch_((arch_family_type)(
    pp_case_((arch_family_type_x86)(pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            register P$raw map_base asm("rdi") = meta->map.ptr;
            register usize map_size asm("rsi") = meta->map.len;
            __asm__ __volatile__(
                "movl $11, %%eax\n\t" // SYS_munmap
                "syscall\n\t"
                "movl $60, %%eax\n\t" // SYS_exit
                "xor %%rdi, %%rdi\n\t" // exit code 0
                "syscall"
                :
                : "r"(map_base), "r"(map_size)
                : "memory", "rax"
            );
            claim_unreachable;
        })),
        pp_case_((arch_type_x86)({
            register P$raw map_base asm("ebx") = meta->map.ptr;
            register usize map_size asm("ecx") = meta->map.len;
            __asm__ __volatile__(
                "movl $91, %%eax\n\t" // SYS_munmap
                "int $0x80\n\t"
                "movl $1, %%eax\n\t" // SYS_exit
                "movl $0, %%ebx\n\t"
                "int $0x80"
                :
                : "r"(map_base), "r"(map_size)
                : "memory", "eax"
            );
            claim_unreachable;
        }))
    )))),
    pp_case_((arch_family_type_arm)(pp_switch_((arch_type)(
        pp_case_((arch_type_aarch64)({
            register P$raw map_base asm("x0") = meta->map.ptr;
            register usize map_size asm("x1") = meta->map.len;
            __asm__ __volatile__(
                "mov x8, #215\n\t" // SYS_munmap
                "svc 0\n\t"
                "mov x8, #93\n\t" // SYS_exit
                "mov x0, #0\n\t"
                "svc 0"
                :
                : "r"(map_base), "r"(map_size)
                : "memory", "x8"
            );
            claim_unreachable;
        })),
        pp_case_((arch_type_arm)({
            register P$raw map_base asm("r0") = meta->map.ptr;
            register usize map_size asm("r1") = meta->map.len;
            __asm__ __volatile__(
                "mov r7, #91\n\t" // SYS_munmap
                "svc 0\n\t"
                "mov r7, #1\n\t" // SYS_exit
                "mov r0, #0\n\t"
                "svc 0"
                :
                : "r"(map_base), "r"(map_size)
                : "memory", "r7"
            );
            claim_unreachable;
        }))
    )))),
    pp_case_((arch_family_type_riscv)({
        register P$raw map_base asm("a0") = meta->map.ptr;
        register usize map_size asm("a1") = meta->map.len;
        __asm__ __volatile__(
            "li a7, 215\n\t" // SYS_munmap
            "ecall\n\t"
            "li a7, 93\n\t" // SYS_exit
            "li a0, 0\n\t"
            "ecall"
            :
            : "r"(map_base), "r"(map_size)
            : "memory", "a7"
        );
        claim_unreachable;
    }))
));
#endif /* plat_is_linux */

/* --- WASI --- */

#if plat_is_wasi
fn_((Thrd__wasi_handle(Thrd self))(Thrd_Handle)) {
    /* TODO: Implement */
    return ensureNonnull(self.inner)->handle;
};

fn_((Thrd__wasi_yield(void))(E$void) $scope) {
    /* TODO: Implement */
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd__wasi_currentId(void))(Thrd_Id)) {
    /* TODO: Implement */
    return 0;
};

fn_((Thrd__wasi_cpuCount(void))(E$usize) $scope) {
    /* TODO: Implement */
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd__wasi_getName(Thrd self, Thrd_NameBuf* buf_ptr))(E$O$S_const$u8) $scope) {
    /* TODO: Implement */
    let_ignore = self;
    let_ignore = buf_ptr;
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd__wasi_setName(Thrd self, S_const$u8 name))(E$void) $scope) {
    /* TODO: Implement */
    let_ignore = self;
    let_ignore = name;
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd__wasi_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd) $scope) {
    /* TODO: Implement */
    let_ignore = cfg;
    let_ignore = fn_ctx;
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd__wasi_detach(Thrd self))(void)) {
    /* TODO: Implement */
    let_ignore = self;
};

fn_((Thrd__wasi_join(Thrd self))(Thrd_FnCtx*)) {
    /* TODO: Implement */
    return ensureNonnull(self.inner);
};
#endif /* plat_is_wasi */
