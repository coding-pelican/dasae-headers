#include "dh/start.h"
#if plat_is_windows
#include "dh/sys/api/windows/proc.h"
#elif plat_is_linux
#include "dh/sys/call/linux.h"
#endif /* plat_is_linux */

/*========== Internal Declarations ==========================================*/

#if !env_start_files_linked
#if plat_is_windows
$extern var_(__xi_a, start_InitFn);
$extern var_(__xi_z, start_InitFn);
$extern var_(__xc_a, start_Fn);
$extern var_(__xc_z, start_Fn);
$extern var_(__xp_a, start_Fn);
$extern var_(__xp_z, start_Fn);
$extern var_(__xt_a, start_Fn);
$extern var_(__xt_z, start_Fn);
$extern var_(__CTOR_LIST__, start_Fn)[];
$extern var_(__DTOR_LIST__, start_Fn)[];
$static fn_((start__win32_callInitFns(P$$(start_InitFn) first, P$$(start_InitFn) last))(void));
$static fn_((start__win32_callFnsForward(P$$(start_Fn) first, P$$(start_Fn) last))(void));
$static fn_((start__win32_callCtorList(void))(void));
$static fn_((start__win32_callDtorList(void))(void));
#elif plat_is_linux
$extern var_(__preinit_array_start, start_Fn)[];
$extern var_(__preinit_array_end, start_Fn)[];
$extern var_(__init_array_start, start_Fn)[];
$extern var_(__init_array_end, start_Fn)[];
$extern var_(__fini_array_start, start_Fn)[];
$extern var_(__fini_array_end, start_Fn)[];
$static fn_((start__linux_callPreInitArray(void))(void));
$static fn_((start__linux_callInitArray(void))(void));
$static fn_((start__linux_callFiniArray(void))(void));
#endif /* plat_is_linux */
#endif /* !env_start_files_linked */

/*========== External Definitions ===========================================*/

fn_((start_exit(start_ExitCode status))(void)) {
#if plat_is_windows
    ExitProcess(as$(UINT)(status));
#elif plat_is_linux
    sys_call_linux_exit_group(status);
#endif /* plat_is_linux */
    claim_unreachable;
};

#if !env_start_files_linked
fn_((start_callInitArray(void))(void)) {
#if plat_is_windows
    start__win32_callInitFns(&__xi_a, &__xi_z);
    start__win32_callFnsForward(&__xc_a, &__xc_z);
    start__win32_callCtorList();
#elif plat_is_linux
    start__linux_callPreInitArray();
    start__linux_callInitArray();
#endif /* plat_is_linux */
};
fn_((start_callFiniArray(void))(void)) {
#if plat_is_windows
    start__win32_callDtorList();
    start__win32_callFnsForward(&__xp_a, &__xp_z);
    start__win32_callFnsForward(&__xt_a, &__xt_z);
#elif plat_is_linux
    start__linux_callFiniArray();
#endif /* plat_is_linux */
};
#endif /* !env_start_files_linked */

/*========== Internal Definitions ===========================================*/

#if !env_start_files_linked
#if plat_is_windows
fn_((start__win32_callInitFns(P$$(start_InitFn) first, P$$(start_InitFn) last))(void)) {
    for (var it = first + 1; it != last; ++it) {
        let fn = *it;
        if (fn == null) continue;
        let code = call((fn)());
        if (code != 0) start_exit(code);
    };
};
fn_((start__win32_callFnsForward(P$$(start_Fn) first, P$$(start_Fn) last))(void)) {
    for (var it = first + 1; it != last; ++it) {
        let fn = *it;
        if (fn != null) call((fn)());
    };
};
fn_((start__win32_callCtorList(void))(void)) {
    let count_or_marker = as$(usize)(__CTOR_LIST__[0]);
    if (count_or_marker != usize_limit_max) {
        for (var i = count_or_marker; i != 0; --i) {
            let fn = __CTOR_LIST__[i];
            if (fn != null) call((fn)());
        };
        return;
    };
    var_(count, usize) = 0;
    while (__CTOR_LIST__[count + 1] != null) ++count;
    while (count != 0) {
        let fn = __CTOR_LIST__[count];
        if (fn != null) call((fn)());
        --count;
    };
};
fn_((start__win32_callDtorList(void))(void)) {
    let count_or_marker = as$(usize)(__DTOR_LIST__[0]);
    if (count_or_marker != usize_limit_max) {
        for (var_(i, usize) = 1; i <= count_or_marker; ++i) {
            let fn = __DTOR_LIST__[i];
            if (fn != null) call((fn)());
        };
        return;
    };
    for (var_(i, usize) = 1; __DTOR_LIST__[i] != null; ++i) {
        let fn = __DTOR_LIST__[i];
        if (fn != null) call((fn)());
    };
};
#elif plat_is_linux
fn_((start__linux_callPreInitArray(void))(void)) {
    for (var it = __preinit_array_start; it != __preinit_array_end; ++it) {
        call((*it)());
    };
};
fn_((start__linux_callInitArray(void))(void)) {
    for (var it = __init_array_start; it != __init_array_end; ++it) {
        call((*it)());
    };
};
fn_((start__linux_callFiniArray(void))(void)) {
    for (var it = __fini_array_end; it-- != __fini_array_start;) {
        call((*it)());
    };
};
#endif /* plat_is_linux */
#endif /* !env_start_files_linked */
