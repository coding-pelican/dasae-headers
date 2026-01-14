#include "dh/core/debug/StkTrace.h"
#include "dh/io/stream.h"
#include "dh/Thrd.h"

/*========== Internal Declarations ==========================================*/

$static fn_((debug_StkTrace__printPanicHeader(S_const$u8 reason, usize code))(void));
$attr($on_load)
$static fn_((debug_StkTrace__setupCrashHandler(void))(void));

pp_if_(pp_true)(pp_then_(
    $attr($inline_always $maybe_unused)
    $static fn_((debug_StkTrace__unsupported__setupCrashHandler(void))(void));
    $attr($inline_always $maybe_unused)
    $static fn_((debug_StkTrace__unsupported_print(void))(void));
));
pp_if_(plat_is_windows)(pp_then_(
    $attr($inline_always)
    $static fn_((debug_StkTrace__windows__setupCrashHandler(void))(void));
    $attr($inline_always)
    $static fn_((debug_StkTrace__windows_print(void))(void));
));
pp_if_(plat_based_unix)(pp_then_(
    $attr($inline_always)
    $static fn_((debug_StkTrace__unix__setupCrashHandler(void))(void));
    $attr($inline_always)
    $static fn_((debug_StkTrace__unix_print(void))(void));
));

$static let debug_StkTrace___setupCrashHandler = pp_if_(plat_is_windows)(
    pp_then_(debug_StkTrace__windows__setupCrashHandler),
    pp_else_(pp_if_(plat_based_unix)(
        pp_then_(debug_StkTrace__unix__setupCrashHandler),
        pp_else_(debug_StkTrace__unsupported__setupCrashHandler)
    )));
$static let debug_StkTrace__print = pp_if_(plat_is_windows)(
    pp_then_(debug_StkTrace__windows_print),
    pp_else_(pp_if_(plat_based_unix)(
        pp_then_(debug_StkTrace__unix_print),
        pp_else_(debug_StkTrace__unsupported_print)
    )));

#define debug_StkTrace__max_frames 64
#define debug_StkTrace__max_symbol_len 256

/*========== External Definitions ===========================================*/

fn_((debug_StkTrace_print(void))(void)) {
    debug_StkTrace__print();
};

/*========== Internal Definitions ===========================================*/

fn_((debug_StkTrace__printPanicHeader(S_const$u8 reason, usize code))(void)) {
    if (code != 0) {
        /* e.g. `panic: access violation (code: 0xC0000005) */
        io_stream_eprintln(u8_l("panic: {:s} (code: {:#Xz})"), reason, code);
    } else {
        /* e.g. `panic: segmentation fault` */
        io_stream_eprintln(u8_l("panic: {:s}"), reason);
    }
};

fn_((debug_StkTrace__setupCrashHandler(void))(void)) {
    debug_StkTrace___setupCrashHandler();
};

/* --- Unsupported --- */

fn_((debug_StkTrace__unsupported__setupCrashHandler(void))(void)) $do_nothing;
fn_((debug_StkTrace__unsupported_print(void))(void)) {
    io_stream_eprintln(u8_l("Stack Trace: Unsupported Platform"));
};

/* --- Windows --- */

#if plat_is_windows
#include "dh/os/windows/base.h"
#include "dh/os/windows/proc.h"
#include "dh/os/windows/pp-def/guard.h"
#include <winver.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#include "dh/os/windows/pp-def/unguarded.h"

$attr($stdcall)
$static fn_((debug_StkTrace__windows__handleException(EXCEPTION_POINTERS* ExceptionInfo))(LONG)) {
    let code = ExceptionInfo->ExceptionRecord->ExceptionCode;
    let reason = expr_(S_const$u8 $scope)(switch (code) {
        case EXCEPTION_ACCESS_VIOLATION:      $break_(u8_l("access violation"));
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: $break_(u8_l("array bounds exceeded"));
        case EXCEPTION_DATATYPE_MISALIGNMENT: $break_(u8_l("datatype misalignment"));
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:    $break_(u8_l("float divide by zero"));
        case EXCEPTION_INT_DIVIDE_BY_ZERO:    $break_(u8_l("integer divide by zero"));
        case EXCEPTION_ILLEGAL_INSTRUCTION:   $break_(u8_l("illegal instruction"));
        case EXCEPTION_STACK_OVERFLOW:        $break_(u8_l("stack overflow"));
        default:                              $break_(u8_l("unknown exception"));
    }) $unscoped_(expr);
    debug_StkTrace__printPanicHeader(reason, as$(usize)(code));
    debug_StkTrace_print();
    return EXCEPTION_EXECUTE_HANDLER; /* Process termination via OS */
};

fn_((debug_StkTrace__windows__setupCrashHandler(void))(void)) {
    SetUnhandledExceptionFilter(debug_StkTrace__windows__handleException);
};

fn_((debug_StkTrace__windows_print(void))(void) $guard) {
    let process = GetCurrentProcess();
    SymInitialize(process, null, true);
    defer_(SymCleanup(process));

    var_(stack, A$$(debug_StkTrace__max_frames, P$raw)) = A_zero();
    let frames = RtlCaptureStackBackTrace(0, debug_StkTrace__max_frames, A_ptr(stack), null);
    let tid = as$(u64)(Thrd_currentId());

    /* [Standard Info] Added TID */
    io_stream_eprintln(u8_l("stack backtrace (tid: {:ul}):"), tid);

    /* Buffer for Symbol Info */
    var_(symbol_buf, A$$(sizeOf$(SYMBOL_INFO) + debug_StkTrace__max_symbol_len, u8)) = A_zero();
    let symbol = ptrAlignCast$((SYMBOL_INFO*)(A_ptr(symbol_buf)));
    symbol->SizeOfStruct = sizeOf$(SYMBOL_INFO);
    symbol->MaxNameLen = debug_StkTrace__max_symbol_len;

    /* Buffer for Module Info (Standard Info: DLL/EXE name) */
    var_(mod_info, IMAGEHLP_MODULE64) = { .SizeOfStruct = sizeOf$(TypeOf(mod_info)) };
    for_(($r(0, frames))(frame) {
        let addr = ptrToInt(*A_at((stack)[frame]));
        var_(displacement, u64) = 0;
        /* 1. Get Module Name (e.g., KERNEL32, MyApp) */
        let has_mod = SymGetModuleInfo64(process, addr, &mod_info);
        let mod_name = has_mod ? mod_info.ModuleName : "unknown";
        /* 2. Get Symbol Name */
        /* Output: "  0: 0xAddress in Symbol + Offset (Module)" */
        if (SymFromAddr(process, addr, &displacement, symbol)) {
            io_stream_eprintln(
                u8_l("  {:uz}: {:#P} in {:z} + {:#Xz} ({:z})"),
                frame, addr, symbol->Name, displacement, mod_name
            );
        } else {
            io_stream_eprintln(
                u8_l("  {:uz}: {:#P} in <unknown> ({:z})"),
                frame, addr, mod_name
            );
        }
    });
} $unguarded_(fn);
#endif /* plat_is_windows */

/* --- Unix --- */

#if plat_based_unix
#include <execinfo.h>
#include <dlfcn.h>
#include <signal.h>
#include <unistd.h>

$static fn_((debug_StkTrace__unix__handleSignal(i32 sig))(void)) {
    let reason = expr_(S_const$u8 $scope)(switch (sig) {
        case SIGSEGV: $break_(u8_l("segmentation fault"));
        case SIGABRT: $break_(u8_l("aborted"));
        case SIGFPE:  $break_(u8_l("floating point exception"));
        case SIGILL:  $break_(u8_l("illegal instruction"));
        case SIGBUS:  $break_(u8_l("bus error"));
        default:      $break_(u8_l("unknown signal"));
    }) $unscoped_(expr);
    debug_StkTrace__printPanicHeader(reason, 0); /* Signals usually don't have a code */
    debug_StkTrace_print();
    _exit(1); /* Syscall exit */
};

fn_((debug_StkTrace__unix__setupCrashHandler(void))(void)) {
    struct sigaction sa = {
        .sa_handler = debug_StkTrace__unix__handleSignal,
        .sa_flags = 0,
    };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, null);
    sigaction(SIGABRT, &sa, null);
    sigaction(SIGFPE, &sa, null);
    sigaction(SIGILL, &sa, null);
    sigaction(SIGBUS, &sa, null);
};

fn_((debug_StkTrace__unix_print(void))(void)) {
    var_(stack, A$$(debug_StkTrace__max_frames, P$raw)) = A_zero();
    let frames = backtrace(A_ptr(stack), debug_StkTrace__max_frames);
    let tid = as$(u64)(Thrd_currentId());

    io_stream_eprintln(u8_l("stack backtrace (tid: {:ul}):"), tid);

    for_(($r(0, frames))(frame) {
        let addr = *A_at((stack)[frame]);
        var_(info, Dl_info) = cleared();
        if (dladdr(addr, &info) && info.dli_sname) {
            let offset = ptrToInt(as$(u8*)(addr) - as$(u8*)(info.dli_saddr));
            io_stream_eprintln(
                u8_l("  {:uz}: {:#P} in {:z} + {:#Xz} ({:z})"),
                frame, addr, info.dli_sname, offset, info.dli_fname ? info.dli_fname : "???"
            );
        } else {
            let fname = (dladdr(addr, &info) && info.dli_fname) ? info.dli_fname : "???";
            io_stream_eprintln(
                u8_l("  {:uz}: {:#P} in <unknown> ({:z})"),
                frame, addr, fname
            );
        }
    });
};
#endif /* plat_based_unix */
