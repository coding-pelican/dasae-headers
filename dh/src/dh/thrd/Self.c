#include "dh/thrd/Self.h"
#include "dh/heap/VMem.h"
#include "dh/u-meta.h"
#if plat_is_linux
#include "dh/sys/call/linux.h"
#endif /* plat_is_linux */

typedef struct thrd__Start {
    var_(clsr, Clsr$raw*);
    var_(ret_type, TypeInfo);
    var_(gpa, mem_Alctr);
    var_(destroy_clsr, bool);
    var_(owned_clsr, u_P$raw);
} thrd__Start;
$attr($must_check)
$static fn_((thrd__startAlloc(
    thrd_SpawnCfg cfg,
    Clsr$raw* clsr,
    TypeInfo ret_type,
    bool destroy_clsr,
    u_P$raw owned_clsr
))(mem_E$u_P$raw));
$static fn_((thrd__startFree(thrd__Start* start))(void));

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
    $static fn_((thrd__unsupported_spawn(
        thrd_SpawnCfg cfg,
        Clsr$raw* clsr,
        TypeInfo ret_type,
        bool destroy_clsr,
        u_P$raw owned_clsr
    ))(thrd_spawn_E$thrd_Self));
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
    $static fn_((thrd__pthread_spawn(
        thrd_SpawnCfg cfg,
        Clsr$raw* clsr,
        TypeInfo ret_type,
        bool destroy_clsr,
        u_P$raw owned_clsr
    ))(thrd_spawn_E$thrd_Self));
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
    $static fn_((thrd__windows_spawn(
        thrd_SpawnCfg cfg,
        Clsr$raw* clsr,
        TypeInfo ret_type,
        bool destroy_clsr,
        u_P$raw owned_clsr
    ))(thrd_spawn_E$thrd_Self));
    $attr($inline_always)
    $static fn_((thrd__windows_detach(thrd_Self self))(void));
    $attr($inline_always)
    $static fn_((thrd__windows_join(thrd_Self self))(Clsr$raw*));
));
pp_if_(plat_is_linux)(pp_then_(
    $attr($inline_always)
    $static $attr($maybe_unused)
fn_((thrd__linux_handle(thrd_Self self))(thrd_Handle));
    $attr($inline_always $must_check)
    $static $attr($maybe_unused)
fn_((thrd__linux_yield(void))(thrd_E$void));
    $attr($inline_always)
    $static fn_((thrd__linux_currId(void))(thrd_Id));
    $attr($inline_always $must_check)
    $static fn_((thrd__linux_cpuCount(void))(thrd_E$usize));
    $attr($inline_always $must_check)
    $static $attr($maybe_unused)
fn_((thrd__linux_getName(thrd_Self self, thrd_NameBuf* buf_ptr))(thrd_E$O$S_const$u8));
    $attr($inline_always $must_check)
    $static $attr($maybe_unused)
fn_((thrd__linux_setName(thrd_Self self, S_const$u8 name))(thrd_E$void));
    $attr($inline_always $must_check)
    $static $attr($maybe_unused)
fn_((thrd__linux_spawn(
        thrd_SpawnCfg cfg,
        Clsr$raw* clsr,
        TypeInfo ret_type,
        bool destroy_clsr,
        u_P$raw owned_clsr
    ))(thrd_spawn_E$thrd_Self));
    $attr($inline_always)
    $static $attr($maybe_unused)
fn_((thrd__linux_detach(thrd_Self self))(void));
    $attr($inline_always)
    $static $attr($maybe_unused)
fn_((thrd__linux_join(thrd_Self self))(Clsr$raw*));
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
    $static fn_((thrd__wasi_spawn(
        thrd_SpawnCfg cfg,
        Clsr$raw* clsr,
        TypeInfo ret_type,
        bool destroy_clsr,
        u_P$raw owned_clsr
    ))(thrd_spawn_E$thrd_Self));
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

fn_((thrd_spawn(
    thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type
))(thrd_spawn_E$thrd_Self)) {
    return thrd__spawn(cfg, clsr, ret_type, false, u_anyP(null));
};

fn_((thrd_spawnOwned(
    thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type, u_P$raw owned_clsr
))(thrd_spawn_E$thrd_Self)) {
    return thrd__spawn(cfg, clsr, ret_type, true, owned_clsr);
};

fn_((thrd_detach(thrd_Self self))(void)) {
    return thrd__detach(self);
};

fn_((thrd_join(thrd_Self self))(Clsr$raw*)) {
    return thrd__join(self);
};

$attr($maybe_unused)
fn_((thrd__startAlloc(
    thrd_SpawnCfg cfg,
    Clsr$raw* clsr,
    TypeInfo ret_type,
    bool destroy_clsr,
    u_P$raw owned_clsr
))(mem_E$u_P$raw) $scope) {
    let gpa = mem_Alctr_ensureValid(cfg.gpa);
    let raw = try_(mem_Alctr_create($trace gpa, typeInfo$(thrd__Start)));
    let start = u_castP$((thrd__Start*)(raw));
    asg_l((start)({
        .clsr = clsr,
        .ret_type = ret_type,
        .gpa = gpa,
        .destroy_clsr = destroy_clsr,
        .owned_clsr = owned_clsr,
    }));
    return_ok(u_anyP(start));
} $unscoped(fn);

$attr($maybe_unused)
fn_((thrd__startFree(thrd__Start* start))(void)) {
    let gpa = start->gpa;
    mem_Alctr_destroy($trace gpa, u_anyP(start));
};

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

fn_((thrd__unsupported_spawn(
    thrd_SpawnCfg cfg,
    Clsr$raw* clsr,
    TypeInfo ret_type,
    bool destroy_clsr,
    u_P$raw owned_clsr
))(thrd_spawn_E$thrd_Self) $scope) {
    let_ignore = cfg;
    let_ignore = clsr;
    let_ignore = ret_type;
    let_ignore = destroy_clsr;
    let_ignore = owned_clsr;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__unsupported_detach(thrd_Self self))(void)) {
    let_ignore = self;
};

fn_((thrd__unsupported_join(thrd_Self self))(Clsr$raw*)) {
    return ensureNonnull(self.clsr);
};

#if thrd_use_pthread
fn_((thrd__pthread_handle(thrd_Self self))(thrd_Handle)) {
    return self.handle;
};

fn_((thrd__pthread_yield(void))(thrd_E$void) $scope) {
    if (sched_yield() != 0) return_err(E_cause$thrd_SystemResources());
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
        pp_default_(()(as$(thrd_Id)(pthread_self())))
    ));
};

fn_((thrd__pthread_cpuCount(void))(thrd_E$usize) $scope) {
#if plat_is_linux
    return thrd__linux_cpuCount();
#elif plat_is_darwin
    var_(count, i32) = 0;
    var_(len, usize) = sizeOf$(i32);
    if (sysctlbyname("hw.logicalcpu", &count, &len, null, 0) == 0) return_ok(as$(usize)(count));
    return_err(E_cause$thrd_SystemResources());
#else
    let count = sysconf(_SC_NPROCESSORS_ONLN);
    if (count < 1) return_err(E_cause$thrd_SystemResources());
    return_ok(as$(usize)(count));
#endif
} $unscoped(fn);

fn_((thrd__pthread_getName(thrd_Self self, thrd_NameBuf* buf_ptr))(thrd_E$O$S_const$u8) $scope) {
    let_ignore = self;
    let_ignore = buf_ptr;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__pthread_setName(thrd_Self self, S_const$u8 name))(thrd_E$void) $scope) {
    let_ignore = self;
    let_ignore = name;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

#if defined(PTHREAD_STACK_MIN)
#define thrd__pthread_defined_stack_min pp_true
#else
#define thrd__pthread_defined_stack_min pp_false
#endif
#define thrd__pthread_stack_size_min pp_if_(thrd__pthread_defined_stack_min)(pp_then_(PTHREAD_STACK_MIN), pp_else_(64 * 1024))

$static fn_((thrd__pthread_entry(P$raw arg))(P$raw));
fn_((thrd__pthread_spawn(
    thrd_SpawnCfg cfg,
    Clsr$raw* clsr,
    TypeInfo ret_type,
    bool destroy_clsr,
    u_P$raw owned_clsr
))(thrd_spawn_E$thrd_Self) $guard) {
    let start = u_castP$((thrd__Start*)(try_(thrd__startAlloc(
        cfg, clsr, ret_type, destroy_clsr, owned_clsr
    ))));
    pthread_attr_t attr = cleared();
    pthread_attr_init(&attr);
    defer_(pthread_attr_destroy(&attr));
    let stack_size_min = as$(usize)(thrd__pthread_stack_size_min);
    let stack_size = cfg.stack_size > stack_size_min ? cfg.stack_size : stack_size_min;
    pthread_attr_setstacksize(&attr, stack_size);
    var_(handle, pthread_t) = 0;
    if (pthread_create(&handle, &attr, thrd__pthread_entry, start) != 0) {
        thrd__startFree(start);
        return_err(E_cause$thrd_SystemResources());
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
    let gpa = start->gpa;
    let destroy_clsr = start->destroy_clsr;
    let owned_clsr = start->owned_clsr;
    thrd__startFree(start);
    clsr_invokeToComplete(clsr, ret_type);
    if (destroy_clsr) {
        mem_Alctr_destroy($trace gpa, owned_clsr);
    }
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
#endif

#if plat_is_windows
fn_((thrd__windows_handle(thrd_Self self))(thrd_Handle)) {
    return self.handle;
};

fn_((thrd__windows_yield(void))(thrd_E$void) $scope) {
    let_ignore = SwitchToThread();
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
fn_((thrd__windows_spawn(
    thrd_SpawnCfg cfg,
    Clsr$raw* clsr,
    TypeInfo ret_type,
    bool destroy_clsr,
    u_P$raw owned_clsr
))(thrd_spawn_E$thrd_Self) $scope) {
    let start = u_castP$((thrd__Start*)(try_(thrd__startAlloc(
        cfg, clsr, ret_type, destroy_clsr, owned_clsr
    ))));
    let stack_size = pri_max(usize_(thrd__windows_stack_size_min), cfg.stack_size);
    let handle = CreateThread(
        null, stack_size,
        thrd__windows_entry, start,
        CREATE_SUSPENDED | STACK_SIZE_PARAM_IS_A_RESERVATION, null
    );
    if (!handle) {
        thrd__startFree(start);
        return_err(E_cause$thrd_SystemResources());
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
    let gpa = start->gpa;
    let destroy_clsr = start->destroy_clsr;
    let owned_clsr = start->owned_clsr;
    thrd__startFree(start);
    clsr_invokeToComplete(clsr, ret_type);
    if (destroy_clsr) {
        mem_Alctr_destroy($trace gpa, owned_clsr);
    }
    return 0;
};

fn_((thrd__windows_detach(thrd_Self self))(void)) {
    let handle = ensureNonnull(self.handle);
    claim_assert(CloseHandle(handle));
};

fn_((thrd__windows_join(thrd_Self self))(Clsr$raw*)) {
    let handle = ensureNonnull(self.handle);
    switch (WaitForSingleObject(handle, INFINITE)) {
    case WAIT_OBJECT_0: break;
    case WAIT_ABANDONED: $fallthrough;
    case WAIT_TIMEOUT: $fallthrough;
    case WAIT_FAILED: $fallthrough;
    default_() claim_unreachable $end(default);
    }
    claim_assert(CloseHandle(handle));
    return ensureNonnull(self.clsr);
};
#endif

#if plat_is_linux
typedef struct thrd__linux_CpuSet {
    usize words[128 / arch_bits_per_word];
} thrd__linux_CpuSet;

$attr($inline_always)
$static fn_((thrd__linux_cpuSetZero(thrd__linux_CpuSet* self))(void)) {
    *self = (thrd__linux_CpuSet){ 0 };
};

$attr($inline_always)
$static fn_((thrd__linux_cpuSetCount(const thrd__linux_CpuSet* self))(usize)) {
    usize count = 0;
    for (usize i = 0; i < 128 / arch_bits_per_word; ++i) {
        count += raw_countOnesSize(self->words[i]);
    }
    return count;
};

typedef enum_((thrd__linux_Completion $fits($packed))(
    thrd__linux_Completion_running = 0,
    thrd__linux_Completion_detached = 1,
    thrd__linux_Completion_completed = 2
)) thrd__linux_Completion;
T_use_atom_V$(thrd__linux_Completion);
T_use_atom_V$(i32);

typedef struct thrd__linux_Meta {
    var_(clsr, Clsr$raw*);
    var_(ret_type, TypeInfo);
    var_(gpa, mem_Alctr);
    var_(destroy_clsr, bool);
    var_(owned_clsr, u_P$raw);
    var_(map, S$raw);
    var_(completion, atom_V$thrd__linux_Completion);
    var_(parent_tid, i32);
    var_(child_tid, atom_V$i32);
} thrd__linux_Meta;

$attr($maybe_unused)
fn_((thrd__linux_handle(thrd_Self self))(thrd_Handle)) {
    return self.handle;
};

$attr($maybe_unused)
fn_((thrd__linux_yield(void))(thrd_E$void) $scope) {
    if (sys_call_linux_sched_yield() != 0) return_err(E_cause$thrd_SystemResources());
    return_ok({});
} $unscoped(fn);

fn_((thrd__linux_currId(void))(thrd_Id)) {
    return as$(thrd_Id)(sys_call_linux_gettid());
};

fn_((thrd__linux_cpuCount(void))(thrd_E$usize) $scope) {
    var_(cpu_set, thrd__linux_CpuSet) = cleared();
    thrd__linux_cpuSetZero(&cpu_set);
    if (sys_call_linux_sched_getaffinity(0, sizeOf$(TypeOf(cpu_set)), &cpu_set) != 0) return_err(E_cause$thrd_SystemResources());
    return_ok(thrd__linux_cpuSetCount(&cpu_set));
} $unscoped(fn);

$attr($maybe_unused)
fn_((thrd__linux_getName(thrd_Self self, thrd_NameBuf* buf_ptr))(thrd_E$O$S_const$u8) $scope) {
    let_ignore = self;
    let_ignore = buf_ptr;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

$attr($maybe_unused)
fn_((thrd__linux_setName(thrd_Self self, S_const$u8 name))(thrd_E$void) $scope) {
    let_ignore = self;
    let_ignore = name;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

#define thrd__linux_clone_flags ( \
    sys_call_linux_CLONE_VM | sys_call_linux_CLONE_FS | sys_call_linux_CLONE_FILES | sys_call_linux_CLONE_SIGHAND \
    | sys_call_linux_CLONE_THREAD | sys_call_linux_CLONE_SYSVSEM \
    | sys_call_linux_CLONE_PARENT_SETTID | sys_call_linux_CLONE_CHILD_CLEARTID \
)

$static $attr($maybe_unused)
fn_((thrd__linux_entry(P$raw arg))(i32));
$attr($maybe_unused)
fn_((thrd__linux_spawn(
    thrd_SpawnCfg cfg,
    Clsr$raw* clsr,
    TypeInfo ret_type,
    bool destroy_clsr,
    u_P$raw owned_clsr
))(thrd_spawn_E$thrd_Self) $guard) {
    claim_assert_nonnull(clsr);
    cfg.gpa = mem_Alctr_ensureValid(cfg.gpa);
    let page_size = mem_page_size;
    let stack_size = mem_alignFwd(pri_max(page_size, cfg.stack_size), page_size);
    let meta_size = mem_alignFwd(sizeOf$(thrd__linux_Meta), alignOf$(thrd__linux_Meta));
    let map_size = page_size + stack_size + meta_size;
    let vmem = catch_((heap_VMem_system())($ignore, return_err(E_cause$thrd_Unsupported())));
    let map_base = orelse_((heap_VMem_reserve(vmem, none$((O$P$raw)), map_size))(null));
    if (map_base == null) return_err(E_cause$thrd_SystemResources());
    errdefer_($ignore, let_ignore = heap_VMem_release(vmem, map_base, map_size));
    let stack_start = as$(u8*)(map_base) + page_size;
    if (!heap_VMem_commit(vmem, stack_start, stack_size + meta_size)) return_err(E_cause$thrd_SystemResources());
    let meta = intToPtr$((thrd__linux_Meta*)(ptrToInt(map_base) + page_size + stack_size));
    *meta = (thrd__linux_Meta){
        .clsr = clsr,
        .ret_type = ret_type,
        .gpa = cfg.gpa,
        .destroy_clsr = destroy_clsr,
        .owned_clsr = owned_clsr,
        .map = P_prefix$((S$raw)(map_base)(map_size)),
        .completion = atom_V_init(thrd__linux_Completion_running),
        .parent_tid = 0,
        .child_tid = atom_V_init(1),
    };
    let stack_top = as$(P$raw)(meta);
    let tid = sys_call_linux_clone_thread(
        thrd__linux_entry, stack_top,
        thrd__linux_clone_flags, meta,
        &meta->parent_tid,
        null,
        ptrQualCast$((i32*)(&meta->child_tid.raw))
    );
    if (tid == -1) return_err(E_cause$thrd_SystemResources());
    return_ok({
        .handle = as$(thrd_Handle)(meta->parent_tid),
        .clsr = clsr,
        .inner = meta,
    });
} $unguarded(fn);

$attr($no_return)
$static $attr($maybe_unused)
fn_((thrd__linux_freeAndExit(thrd__linux_Meta* meta))(void));
$attr($maybe_unused)
fn_((thrd__linux_entry(P$raw arg))(i32)) {
    let meta = ensureNonnull(as$(thrd__linux_Meta*)(arg));
    clsr_invokeToComplete(ensureNonnull(meta->clsr), meta->ret_type);
    if (meta->destroy_clsr) {
        mem_Alctr_destroy($trace meta->gpa, meta->owned_clsr);
    }
    let prev = atom_V_fetchXchg(&meta->completion, thrd__linux_Completion_completed, atom_MemOrd_seq_cst);
    switch (prev) {
    case_((thrd__linux_Completion_running)) return 0 $end(case);
    case_((thrd__linux_Completion_detached)) thrd__linux_freeAndExit(meta) $end(case);
    case_((thrd__linux_Completion_completed)) claim_unreachable $end(case);
    default_() claim_unreachable $end(default);
    }
    return 0;
};

$attr($maybe_unused)
fn_((thrd__linux_detach(thrd_Self self))(void)) {
    let meta = as$(thrd__linux_Meta*)(self.inner);
    let prev = atom_V_fetchXchg(&meta->completion, thrd__linux_Completion_detached, atom_MemOrd_seq_cst);
    switch (prev) {
    case_((thrd__linux_Completion_running)) $do_nothing $end(case);
    case_((thrd__linux_Completion_completed)) {
        let vmem = catch_((heap_VMem_system())($ignore, claim_unreachable));
        let_ignore = heap_VMem_release(vmem, meta->map.ptr, meta->map.len);
    } $end(case);
    case_((thrd__linux_Completion_detached)) claim_unreachable $end(case);
    default_() claim_unreachable $end(default);
    }
};

$attr($maybe_unused)
fn_((thrd__linux_join(thrd_Self self))(Clsr$raw*)) {
    let meta = as$(thrd__linux_Meta*)(self.inner);
    while (true) {
        let tid = atom_V_load(&meta->child_tid, atom_MemOrd_seq_cst);
        if (tid == 0) break;
        let_ignore = sys_call_linux_futex(ptrQualCast$((P$raw)(&meta->child_tid.raw)), sys_call_linux_FUTEX_WAIT, tid, null, null, 0);
    }
    let vmem = catch_((heap_VMem_system())($ignore, claim_unreachable));
    let_ignore = heap_VMem_release(vmem, meta->map.ptr, meta->map.len);
    return ensureNonnull(self.clsr);
};

$attr($maybe_unused)
fn_((thrd__linux_freeAndExit(thrd__linux_Meta* meta))(void)) {
    sys_call_linux_munmap_exit(meta->map.ptr, meta->map.len, 0);
};
#endif

#if plat_is_wasi
fn_((thrd__wasi_handle(thrd_Self self))(thrd_Handle)) {
    return self.handle;
};

fn_((thrd__wasi_yield(void))(thrd_E$void) $scope) {
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__wasi_currId(void))(thrd_Id)) {
    return 0;
};

fn_((thrd__wasi_cpuCount(void))(thrd_E$usize) $scope) {
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__wasi_getName(thrd_Self self, thrd_NameBuf* buf_ptr))(thrd_E$O$S_const$u8) $scope) {
    let_ignore = self;
    let_ignore = buf_ptr;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__wasi_setName(thrd_Self self, S_const$u8 name))(thrd_E$void) $scope) {
    let_ignore = self;
    let_ignore = name;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__wasi_spawn(
    thrd_SpawnCfg cfg,
    Clsr$raw* clsr,
    TypeInfo ret_type,
    bool destroy_clsr,
    u_P$raw owned_clsr
))(thrd_spawn_E$thrd_Self) $scope) {
    let_ignore = cfg;
    let_ignore = clsr;
    let_ignore = ret_type;
    let_ignore = destroy_clsr;
    let_ignore = owned_clsr;
    return_err(E_cause$thrd_Unsupported());
} $unscoped(fn);

fn_((thrd__wasi_detach(thrd_Self self))(void)) {
    let_ignore = self;
};

fn_((thrd__wasi_join(thrd_Self self))(Clsr$raw*)) {
    return ensureNonnull(self.clsr);
};
#endif
