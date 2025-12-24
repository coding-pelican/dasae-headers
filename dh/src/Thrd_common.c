#include "dh/Thrd/common.h"
#include "dh/time/Duration.h"

fn_((Thrd_sleep(time_Duration duration))(void)) {
    time_sleep(duration);
};

fn_((Thrd_handle(Thrd self))(Thrd_Handle)) {
    claim_assert_nonnull(self.inner);
    return self.inner->handle;
};

#if Thrd__use_pthread
fn_((Thrd_yield(void))(E$void) $scope) {
    if (sched_yield() != 0) {
        return_err(Err_Unspecified()); /* TODO: Replace to specific error */
    }
    return_ok({});
} $unscoped_(fn);

fn_((Thrd_currentId(void))(Thrd_Id)) {
    return pp_switch_((plat_type)(/* clang-format off */
    pp_case_((plat_type_linux)(syscall(SYS_gettid))),
    pp_case_((plat_type_darwin)(blk({
        u64 tid = 0;
        pthread_threadid_np(null, &tid);
        blk_return tid;
    }))),
    pp_default_(as$(Thrd_Id)(pthread_self()))
    )); /* clang-format on */
};

fn_((Thrd_cpuCount(void))(E$usize) $scope) {
    pp_switch_((plat_type)(/* clang-format off */
    pp_case_((plat_type_linux)({
        cpu_set_t cpu_set = 0;
        if (sched_getaffinity(0, sizeOf$(cpu_set_t), &cpu_set) != 0) {
            return_err(Err_Unspecified()); /* TODO: Replace to specific error */
        }
        return_ok(as$(usize)(CPU_COUNT(&cpu_set)));
    })),
    pp_case_((plat_type_darwin)({
        i32 count = 0;
        usize len = sizeOf$(i32);
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
    )); /* clang-format on */
} $unscoped_(fn);

#if defined(PTHREAD_STACK_MIN)
#define Thrd__defined_pthread_stack_min pp_true
#else
#define Thrd__defined_pthread_stack_min pp_false
#endif

$static fn_((Thrd__entry(P$raw arg))(P$raw));
fn_((Thrd_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd) $guard) {
    claim_assert_nonnull(fn_ctx);
    pthread_attr_t attr = 0;
    pthread_attr_init(&attr);
    defer_(pthread_attr_destroy(&attr));
    let_(stack, usize) = prim_max(
        pp_if_(Thrd__defined_pthread_stack_min)(
            pp_then_(PTHREAD_STACK_MIN),
            pp_else_(64 * 1024)
        ),
        cfg.stack_size
    );
    pthread_attr_setstacksize(&attr, stack);
    if (pthread_create(as$(pthread_t*)(&fn_ctx->handle), &attr, Thrd__entry, fn_ctx) != 0) {
        return_err(Err_SystemResources()); /* TODO: Replace to specific error */
    }
    return_ok({ .inner = fn_ctx });
} $unguarded_(fn);

fn_((Thrd__entry(P$raw arg))(P$raw)) {
    let ctx = ensureNonnull(as$(Thrd_FnCtx*)(arg));
    return call((ctx->fn)(ctx));
};

fn_((Thrd_detach(Thrd self))(void)) {
    let_ignore = pthread_detach(self.inner->handle);
};

fn_((Thrd_join(Thrd self))(Thrd_FnCtx*)) {
    var_(ret_val, P$raw) = null;
    let_ignore = pthread_join(self.inner->handle, &ret_val);
    claim_assert(as$(Thrd_FnCtx*)(ret_val) == self.inner);
    return self.inner;
};

#else
#if plat_type == plat_type_windows
#include "dh/os/windows.h"

fn_((Thrd_yield(void))(E$void) $scope) {
    let_ignore = SwitchToThread(); /* TODO: Use NtYieldExecution instead */
    return_ok({});
} $unscoped_(fn);

fn_((Thrd_currentId(void))(Thrd_Id)) {
    return as$(Thrd_Id)(GetCurrentThreadId());
};

fn_((Thrd_cpuCount(void))(E$usize) $scope) {
    SYSTEM_INFO sys_info = cleared();
    GetSystemInfo(&sys_info);
    return_ok(as$(usize)(sys_info.dwNumberOfProcessors));
} $unscoped_(fn);

fn_((Thrd_getName(Thrd self, Thrd_NameBuf* buf_ptr))(E$O$S_const$u8) $scope) {
    let_ignore = self;
    let_ignore = buf_ptr;
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd_setName(Thrd self, S_const$u8 name))(E$void) $scope) {
    let_ignore = self;
    let_ignore = name;
    return_err(Err_Unsupported());
} $unscoped_(fn);

$attr($stdcall)
$static fn_((Thrd__entry(LPVOID lpParameter))(DWORD));
fn_((Thrd_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd) $scope) {
    claim_assert_nonnull(fn_ctx);
    claim_assert_nonnull(fn_ctx->fn);
    let stack_size = prim_max(64 * 1024, cfg.stack_size);
    fn_ctx->handle = CreateThread(
        null,
        stack_size,
        Thrd__entry,
        fn_ctx,
        CREATE_SUSPENDED,
        null
    );
    ResumeThread(fn_ctx->handle);
    if (!fn_ctx->handle) { return_err(Err_Unexpected()); }
    return_ok({ .inner = fn_ctx });
} $unscoped_(fn);

fn_((Thrd__entry(LPVOID lpParameter))(DWORD)) {
    let ctx = ensureNonnull(as$(Thrd_FnCtx*)(lpParameter));
    let_ignore = call((ctx->fn)(ctx));
    return 0;
};

fn_((Thrd_detach(Thrd self))(void)) {
    let fn_ctx = ensureNonnull(self.inner);
    let handle = ensureNonnull(fn_ctx->handle);
    fn_ctx->handle = (claim_assert(CloseHandle(handle)), null);
};

fn_((Thrd_join(Thrd self))(Thrd_FnCtx*)) {
    let fn_ctx = ensureNonnull(self.inner);
    let handle = ensureNonnull(fn_ctx->handle);
    switch (WaitForSingleObject(handle, INFINITE)) {
    case WAIT_OBJECT_0:  break; /* Thread signaled termination */
    case WAIT_ABANDONED: $fallthrough; /* Mutex only, not for threads */
    case WAIT_TIMEOUT:   $fallthrough; /* with INFINITE delay */
    case WAIT_FAILED:    $fallthrough; /* Invalid handle or logic error */
    default:             claim_unreachable;
    }
    fn_ctx->handle = (claim_assert(CloseHandle(handle)), null);
    return fn_ctx;
};

#elif plat_type == plat_type_linux
#include <sys/syscall.h>
#include <sys/mman.h>
#include <linux/sched.h>
#include <linux/futex.h>
#include <unistd.h>
#include <errno.h>

#define Thrd__clone_flags /* clang-format off */ ( \
    CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | \
    CLONE_THREAD | CLONE_SYSVSEM | CLONE_SETTLS | \
    CLONE_PARENT_SETTID | CLONE_CHILD_SETTID | CLONE_CHILD_CLEARTID | CLONE_DETACHED \
) /* clang-format on */

fn_((Thrd_yield(void))(E$void) $scope) {
    if (syscall(SYS_sched_yield) != 0) {
        return_err(Err_Unspecified()); /* TODO: Replace to specific error */
    }
    return_ok({});
} $unscoped_(fn);

fn_((Thrd_currentId(void))(Thrd_Id)) {
    return as$(Thrd_Id)(syscall(SYS_gettid));
};

fn_((Thrd_cpuCount(void))(E$usize) $scope) {
    cpu_set_t cpu_set = 0;
    if (sched_getaffinity(0, sizeOf$(cpu_set_t), &cpu_set) != 0) {
        return_err(Err_Unspecified()); /* TODO: Replace to specific error */
    }
    return_ok(as$(usize)(CPU_COUNT(&cpu_set)));
} $unscoped_(fn);

$static fn_((Thrd__entry(P$raw arg))(i32));
fn_((Thrd_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd) $guard) {
    claim_assert_nonnull(fn_ctx);
    let page_size = as$(usize)(sysconf(_SC_PAGESIZE));
    let stack_size = mem_alignFwd(prim_max(page_size, cfg.stack_size), page_size);

    //  Allocate stack with guard page
    let map_size = stack_size + page_size;
    let stack_base = mmap(null, map_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (stack_base == MAP_FAILED) {
        return_err(Err_SystemResources()); /* TODO: Replace to specific error */
    }
    errdefer_($ignore, munmap(stack_base, map_size));

    mprotect(as$(u8*)(stack_base) + page_size, stack_size, PROT_READ | PROT_WRITE);
    let stack_top = as$(u8*)(stack_base) + map_size;

    // tidptr as the address to be cleared on exit
    let parent_tid = &fn_ctx->handle;
    let child_tid = &fn_ctx->handle;
    // Note: actual clone wrapper might need assembly for stack setup on some platforms,
    // but glibc's clone() takes a function pointer.
    let tid = clone(Thrd__entry, stack_top, Thrd__clone_flags, fn_ctx, parent_tid, null, child_tid);
    if (tid == -1) {
        return_err(Err_SystemResources()); /* TODO: Replace to specific error */
    }
    return_ok({ .inner = fn_ctx });
} $unguarded_(fn);

fn_((Thrd__entry(P$raw arg))(i32)) {
    let ctx = ensureNonnull(as$(Thrd_FnCtx*)(arg));
    let_ignore = call((ctx->fn)(ctx));
    return 0; // `CLONE_CHILD_CLEARTID` makes this location 0 and performs futex wake.
};

fn_((Thrd_detach(Thrd self))(void)) {
    let_ignore = self;
    // Linux clone with CLONE_DETACHED doesn't strictly need join/detach
    // but for consistency we might manage the lifecycle here.
};

fn_((Thrd_join(Thrd self))(Thrd_FnCtx*)) {
    // Wait until the child_tid becomes 0 via futex
    var tid_ptr = as$(volatile i32*)(&self.inner->handle);
    while (true) {
        let tid = *tid_ptr;
        if (tid == 0) break;
        syscall(SYS_futex, tid_ptr, FUTEX_WAIT | FUTEX_PRIVATE_FLAG, tid, null, null, 0);
    }
    return self.inner;
};

#else
fn_((Thrd_yield(void))(E$void)) {
    let_ignore = self;
    let_ignore = buf_ptr;
    return_err(Err_Unsupported());
};

fn_((Thrd_currentId(void))(Thrd_Id)) {
    let_ignore = self;
    let_ignore = buf_ptr;
    return 0;
};

fn_((Thrd_cpuCount(void))(E$usize)) {
    let_ignore = self;
    let_ignore = buf_ptr;
    return_err(Err_Unsupported());
};

fn_((Thrd_getName(Thrd self, Thrd_NameBuf* buf_ptr))(E$O$S_const$u8)) {
    let_ignore = self;
    let_ignore = buf_ptr;
    return_err(Err_Unsupported());
};

fn_((Thrd_setName(Thrd self, S_const$u8 name))(E$void)) {
    let_ignore = self;
    let_ignore = name;
    return_err(Err_Unsupported());
};

fn_((Thrd_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd)) {
    let_ignore = cfg;
    let_ignore = fn_ctx;
    return_err(Err_Unsupported());
};

fn_((Thrd_detach(Thrd self))(void)) {
    let_ignore = self;
};

fn_((Thrd_join(Thrd self))(Thrd_FnCtx*)) {
    return ensureNonnull(self.inner);
};
#endif /* plat_type */
#endif
