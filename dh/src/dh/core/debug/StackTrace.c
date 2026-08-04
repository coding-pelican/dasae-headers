#if defined(__linux__) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif /* defined(__linux__) && !defined(_GNU_SOURCE) */

#include "dh/core/debug/StackTrace.h"
#include "dh/io/stream.h"
#include "dh/thrd/Self.h"

#define debug_StackTrace__unix_enabled pp_and(plat_based_unix, env_libc_linked)

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused)
$static fn_((debug_StackTrace__printPanicHeader(io_Writer writer, S_const$u8 reason, usize code))(void));

pp_if_(pp_true)(pp_then_(
    $attr($inline_always $maybe_unused)
    $static fn_((debug_StackTrace__unsupported_setupCrashHandler(void))(void));
    $attr($inline_always $maybe_unused)
    $static fn_((debug_StackTrace__unsupported_print(void))(void));
));
pp_if_(plat_is_windows)(pp_then_(
    $attr($inline_always)
    $static fn_((debug_StackTrace__windows_setupCrashHandler(void))(void));
    $attr($inline_always)
    $static fn_((debug_StackTrace__windows_print(void))(void));
));
pp_if_(debug_StackTrace__unix_enabled)(pp_then_(
    $attr($inline_always)
    $static fn_((debug_StackTrace__unix_setupCrashHandler(void))(void));
    $attr($inline_always)
    $static fn_((debug_StackTrace__unix_print(void))(void));
));

$attr(pp_if_(pp_not(debug_enabled))(pp_then_($maybe_unused)))
$static let debug_StackTrace__setupCrashHandler = pp_if_(plat_is_windows)(
    pp_then_(debug_StackTrace__windows_setupCrashHandler),
    pp_else_(pp_if_(debug_StackTrace__unix_enabled)(
        pp_then_(debug_StackTrace__unix_setupCrashHandler),
        pp_else_(debug_StackTrace__unsupported_setupCrashHandler)
    )));
$attr(pp_if_(pp_not(debug_enabled))(pp_then_($maybe_unused)))
$static let debug_StackTrace__print = pp_if_(plat_is_windows)(
    pp_then_(debug_StackTrace__windows_print),
    pp_else_(pp_if_(debug_StackTrace__unix_enabled)(
        pp_then_(debug_StackTrace__unix_print),
        pp_else_(debug_StackTrace__unsupported_print)
    )));

#define debug_StackTrace__max_frames pp_expand( \
    pp_switch_ pp_begin(arch_bits_unit)( \
        pp_case_((arch_bits_unit_64bit)(pp_expand( \
            pp_switch_ pp_begin(arch_family_type)( \
                pp_case_((arch_family_type_x86)(64)), \
                pp_case_((arch_family_type_arm)(48)), \
                pp_case_((arch_family_type_riscv)(48)), \
                pp_default_(()(32)) \
            ) pp_end \
        ))), \
        pp_case_((arch_bits_unit_32bit)(pp_expand( \
            pp_switch_ pp_begin(arch_family_type)( \
                pp_case_((arch_family_type_x86)(32)), \
                pp_case_((arch_family_type_arm)(24)), \
                pp_case_((arch_family_type_riscv)(24)), \
                pp_default_(()(16)) \
            ) pp_end \
        ))), \
        pp_default_(()(16)) \
    ) pp_end \
)
#define debug_StackTrace__max_symbol_len 256

/*========== External Definitions ===========================================*/

/* TODO: Using `thrd_LazyLock` */
fn_((debug_StackTrace_setupCrashHandler(void))(void)) { debug_only(
    $static var_(s_setted_up, bool) = false;
    if (!s_setted_up) {
        s_setted_up = (debug_StackTrace__setupCrashHandler(), true);
    }
) };

fn_((debug_StackTrace_print(void))(void)) { debug_only(
    debug_StackTrace__print();
) };

/*========== Internal Definitions ===========================================*/

fn_((debug_StackTrace__printPanicHeader(io_Writer writer, S_const$u8 reason, usize code))(void)) {
    if (code != 0) {
        /* e.g. `panic: access violation (code: 0xC0000005) */
        catch_((io_Writer_println(writer, u8_l("panic: {:s} (code: {:#Xz})"), reason, code))($ignore, $do_nothing));
    } else {
        /* e.g. `panic: segmentation fault` */
        catch_((io_Writer_println(writer, u8_l("panic: {:s}"), reason))($ignore, $do_nothing));
    }
};

/*--- Unsupported ---*/

fn_((debug_StackTrace__unsupported_setupCrashHandler(void))(void)) $do_nothing;
fn_((debug_StackTrace__unsupported_print(void))(void) $guard) {
    let std = catch_((io_std_direct())($ignore, io_std_noop));
    var locked = io_std_lockErr(std);
    defer_(io_Locked_Writer_unlock(&locked));
    let writer = io_Locked_writer(&locked);
    catch_((io_Writer_println(writer, u8_l("Stack Trace: Unsupported Platform")))($ignore, $do_nothing));
} $unguarded(fn);

/*--- Windows ---*/

#if plat_is_windows
#include "dh/sys/api/windows/base.h"
#include "dh/sys/api/windows/proc.h"
#include "dh/sys/api/windows/pp-def/guard.h"
#include <winver.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#include "dh/sys/api/windows/pp-def/unguarded.h"

$attr($callconv_stdcall)
$static fn_((debug_StackTrace__windows__handleException(EXCEPTION_POINTERS* ExceptionInfo))(LONG) $guard) {
    let std = catch_((io_std_direct())($ignore, io_std_noop));
    var locked = io_std_lockErr(std);
    defer_(io_Locked_Writer_unlock(&locked));
    {
        let code = ExceptionInfo->ExceptionRecord->ExceptionCode;
        let reason = expr_(S_const$u8 $scope)(switch (code) {
            case_((EXCEPTION_ACCESS_VIOLATION)) $break_(u8_l("access violation")) $end(case);
            case_((EXCEPTION_ARRAY_BOUNDS_EXCEEDED)) $break_(u8_l("array bounds exceeded")) $end(case);
            case_((EXCEPTION_DATATYPE_MISALIGNMENT)) $break_(u8_l("datatype misalignment")) $end(case);
            case_((EXCEPTION_FLT_DIVIDE_BY_ZERO)) $break_(u8_l("float divide by zero")) $end(case);
            case_((EXCEPTION_INT_DIVIDE_BY_ZERO)) $break_(u8_l("integer divide by zero")) $end(case);
            case_((EXCEPTION_ILLEGAL_INSTRUCTION)) $break_(u8_l("illegal instruction")) $end(case);
            case_((EXCEPTION_STACK_OVERFLOW)) $break_(u8_l("stack overflow")) $end(case);
            default_() $break_(u8_l("unknown exception")) $end(default);
        }) $unscoped(expr);
        debug_StackTrace__printPanicHeader(io_Locked_writer(&locked), reason, as$(usize)(code));
        debug_StackTrace_print();
    }
    return_(EXCEPTION_EXECUTE_HANDLER); /* Process termination via OS */
} $unguarded(fn);

fn_((debug_StackTrace__windows_setupCrashHandler(void))(void)) {
    SetUnhandledExceptionFilter(debug_StackTrace__windows__handleException);
};

fn_((debug_StackTrace__windows_print(void))(void) $guard) {
    let std = catch_((io_std_direct())($ignore, io_std_noop));
    var locked = io_std_lockErr(std);
    defer_(io_Locked_Writer_unlock(&locked));
    using_(let writer = io_Locked_writer(&locked)) {
        let process = GetCurrentProcess();
        SymSetOptions(SymGetOptions() | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
        SymInitialize(process, null, true);
        defer_(SymCleanup(process));

        $static var_(stack, A$$(debug_StackTrace__max_frames, P$raw)) $undefined_static;
        let frames = RtlCaptureStackBackTrace(0, debug_StackTrace__max_frames, A_ptr(stack), null);
        let tid = as$(u64)(thrd_currId());

        /* [Standard Info] Added TID */
        catch_((io_Writer_println(writer, u8_l("stack backtrace (tid: {:ul}):"), tid))($ignore, $do_nothing));

        /* Buffer for Symbol Info */
        $static var_(symbol_buf, A$$(sizeOf$(SYMBOL_INFO) + debug_StackTrace__max_symbol_len, u8) $align(alignOf$(SYMBOL_INFO))) $undefined_static;
        let symbol = ptrAlignCast$((SYMBOL_INFO*)(A_ptr(symbol_buf)));
        symbol->SizeOfStruct = sizeOf$(SYMBOL_INFO);
        symbol->MaxNameLen = debug_StackTrace__max_symbol_len;

        /* Buffer for Module Info (Standard Info: DLL/EXE name) */
        var_(mod_info, IMAGEHLP_MODULE64) = { .SizeOfStruct = sizeOf$(TypeOf(mod_info)) };
        for_(($r(0, frames))(frame)) {
            let addr = ptrToInt(*A_at((stack)[frame]));
            var_(displacement, u64) $undefined;
            /* 1. Get Module Name (e.g., KERNEL32, MyApp) */
            let has_mod = SymGetModuleInfo64(process, addr, &mod_info);
            let mod_name = has_mod ? mod_info.ModuleName : "unknown";
            var_(line, IMAGEHLP_LINE64) = { .SizeOfStruct = sizeOf$(TypeOf(line)) };
            var_(line_displacement, DWORD) = 0;
            let has_line = SymGetLineFromAddr64(process, addr, &line_displacement, &line);
            /* 2. Get Symbol Name */
            /* Output: "  0: 0xAddress in Symbol + Offset (Module)" */
            if (SymFromAddr(process, addr, &displacement, symbol)) {
                catch_((io_Writer_println(
                    writer, u8_l("  {:uz}: {:#P} in {:z} + {:#Xz} ({:z})"),
                    frame, addr, symbol->Name, displacement, mod_name
                ))($ignore, $do_nothing));
            } else {
                catch_((io_Writer_println(
                    writer, u8_l("  {:uz}: {:#P} in <unknown> ({:z})"),
                    frame, addr, mod_name
                ))($ignore, $do_nothing));
            }
            if (has_line) {
                catch_((io_Writer_println(
                    writer, u8_l("      at {:z}:{:uz}"),
                    line.FileName, as$(usize)(line.LineNumber)
                ))($ignore, $do_nothing));
            }
        } $end(for);
    }
} $unguarded(fn);
#endif /* plat_is_windows */

/*--- Unix ---*/

#if debug_StackTrace__unix_enabled
#include <execinfo.h>
#include <dlfcn.h>
#include <unistd.h>
#include "dh/sys/posix.h"

$static fn_((debug_StackTrace__unix__handleSignal(i32 sig))(void) $guard) {
    using_(let std = catch_((io_std_direct())($ignore, io_std_noop))) blk_defer {
        var locked = io_std_lockErr(std);
        defer_(io_Locked_Writer_unlock(&locked));
        using_(let writer = io_Locked_writer(&locked)) {
            let reason = expr_(S_const$u8 $scope)(switch (sig) {
                case_((sys_posix_SIGSEGV)) $break_(u8_l("segmentation fault")) $end(case);
                case_((sys_posix_SIGABRT)) $break_(u8_l("aborted")) $end(case);
                case_((sys_posix_SIGFPE)) $break_(u8_l("floating point exception")) $end(case);
                case_((sys_posix_SIGILL)) $break_(u8_l("illegal instruction")) $end(case);
                case_((sys_posix_SIGBUS)) $break_(u8_l("bus error")) $end(case);
                default_() $break_(u8_l("unknown signal")) $end(default);
            }) $unscoped(expr);
            debug_StackTrace__printPanicHeader(writer, reason, 0); /* Signals usually don't have a code */
            debug_StackTrace_print();
        }
    } blk_deferral;
    _exit(1); /* Syscall exit */
} $unguarded(fn);

fn_((debug_StackTrace__unix_setupCrashHandler(void))(void)) {
    sys_posix_sigaction sa = {
        .sa_handler = debug_StackTrace__unix__handleSignal,
        .sa_flags = 0,
    };
    sys_posix_sigemptyset(&sa.sa_mask);
    let_ignore = sys_posix_sigaction_set(sys_posix_SIGSEGV, some$((O$P$raw)(&sa)), none$((O$P$raw)));
    let_ignore = sys_posix_sigaction_set(sys_posix_SIGABRT, some$((O$P$raw)(&sa)), none$((O$P$raw)));
    let_ignore = sys_posix_sigaction_set(sys_posix_SIGFPE, some$((O$P$raw)(&sa)), none$((O$P$raw)));
    let_ignore = sys_posix_sigaction_set(sys_posix_SIGILL, some$((O$P$raw)(&sa)), none$((O$P$raw)));
    let_ignore = sys_posix_sigaction_set(sys_posix_SIGBUS, some$((O$P$raw)(&sa)), none$((O$P$raw)));
};

fn_((debug_StackTrace__unix_print(void))(void) $guard) {
    let std = catch_((io_std_direct())($ignore, io_std_noop));
    var locked = io_std_lockErr(std);
    defer_(io_Locked_Writer_unlock(&locked));
    using_(let writer = io_Locked_writer(&locked)) {
        $static var_(stack, A$$(debug_StackTrace__max_frames, P$raw)) $undefined_static;
        let frames = as$(usize)(backtrace(A_ptr(stack), debug_StackTrace__max_frames));
        let tid = as$(u64)(thrd_currId());

        catch_((io_Writer_println(writer, u8_l("stack backtrace (tid: {:ul}):"), tid))($ignore, $do_nothing));

        for_(($r(0, frames))(frame)) {
            let addr = *A_at((stack)[frame]);
            var_(info, Dl_info) $undefined;
            if (dladdr(addr, &info) && info.dli_sname) {
                let offset = ptrToInt(as$(u8*)(addr) - as$(u8*)(info.dli_saddr));
                catch_((io_Writer_println(
                    writer, u8_l("  {:uz}: {:#P} in {:z} + {:#Xz} ({:z})"),
                    frame, addr, info.dli_sname, offset, info.dli_fname ? info.dli_fname : "???"
                ))($ignore, $do_nothing));
            } else {
                let fname = (dladdr(addr, &info) && info.dli_fname) ? info.dli_fname : "???";
                catch_((io_Writer_println(
                    writer, u8_l("  {:uz}: {:#P} in <unknown> ({:z})"),
                    frame, addr, fname
                ))($ignore, $do_nothing));
            }
        } $end(for);
    }
} $unguarded(fn);
#endif /* debug_StackTrace__unix_enabled */
