/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    dh-main.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-30 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  (none)
 *
 * @brief   hijacked main with explicit process startup context
 */
#pragma once
#ifndef main__included
#define main__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/start.h"
#include "dh/TEST.h"
#include "dh/heap/Sys.h"
#include "dh/proc/Direct.h"
#include "dh/proc/Entry.h"

/*========== Macros and Declarations ========================================*/

/*---------- Configuration Flags --------------------------------------------*/

#if !defined(main_no_hijack)
#define main_no_hijack __comp_bool__main_no_hijack
#endif /* !defined(main_no_hijack) */
#define main__no_hijack_default __comp_flag__main__no_hijack_default

#if !defined(main_no_entry)
#define main_no_entry __comp_bool__main_no_entry
#endif /* !defined(main_no_entry) */
#define main__no_entry_default __comp_flag__main__no_entry_default

#if !defined(main_no_return_err)
#define main_no_return_err __comp_bool__main_no_return_err
#endif /* !defined(main_no_return_err) */
#define main__no_return_err_default __comp_flag__main__no_return_err_default

#if !defined(main_no_print_err)
#define main_no_print_err __comp_bool__main_no_print_err
#endif /* !defined(main_no_print_err) */
#define main__no_print_err_default __comp_flag__main__no_print_err_default

/*========== Macros and Definitions =========================================*/

/*---------- Configuration Flags --------------------------------------------*/

#define __comp_bool__main_no_hijack main__no_hijack_default
#define __comp_flag__main__no_hijack_default pp_false
#define __comp_bool__main_no_entry main__no_entry_default
#define __comp_flag__main__no_entry_default main_no_hijack
#define __comp_bool__main_no_return_err main__no_return_err_default
#define __comp_flag__main__no_return_err_default main_no_hijack
#define __comp_bool__main_no_print_err main__no_print_err_default
#define __comp_flag__main__no_print_err_default main_no_return_err

#if defined(MAIN_NO_PRINT_ERR)
#undef __comp_flag__main__no_print_err_default
#define __comp_flag__main__no_print_err_default pp_true
#endif /* defined(MAIN_NO_PRINT_ERR) */

/*---------- Hijack Main ----------------------------------------------------*/

#if main_no_hijack
/* No hijack, just call main as usual. */
#else /* !main_no_hijack */

$attr(pp_if_(pp_not(main_no_return_err))(pp_then_($must_check)))
$attr(pp_if_(TEST_enabled)(
    pp_then_($maybe_unused $static),
    pp_else_($extern)
))
fn_((dh_main(pp_if_(pp_not(main_no_entry))(
    pp_then_(proc_Entry entry),
    pp_else_(void)
)))(pp_if_(pp_not(main_no_return_err))(pp_then_(E$void), pp_else_(void))));

/*---------- Root main ------------------------------------------------------*/

#ifndef main__root_included
#define main__root_included 1

#if in_comptime
#if !TEST_enabled
#if main_no_entry
$attr($maybe_unused $inline)
$static fn_((main__runDHMain(void))(start_ExitCode)) {
    debug_StackTrace_setupCrashHandler();
    pp_if_(pp_not(main_no_return_err))(
        (let returned =),
        ($ignore_void)
    ) dh_main();
    pp_if_(pp_not(main_no_return_err))((catch_((returned)(err, {
        pp_if_(main_no_print_err)(
            (let_ignore = err),
            (E_printProgramFailed(&err), ETrace_print())
        );
        return $debug_point 1;
    }))));
    return 0;
};

#else /* !main_no_entry */
$attr($maybe_unused $inline)
$static fn_((main__runDHMain(proc_Entry entry))(start_ExitCode)) {
    debug_StackTrace_setupCrashHandler();
    pp_if_(pp_not(main_no_return_err))(
        (let returned =),
        ($ignore_void)
    ) dh_main(entry);
    pp_if_(pp_not(main_no_return_err))((catch_((returned)(err, {
        pp_if_(main_no_print_err)(
            (let_ignore = err),
            (E_printProgramFailed(&err), ETrace_print())
        );
        return $debug_point 1;
    }))));
    return 0;
};

$attr($maybe_unused)
$static fn_((main__runDHMainWithIo(
    start_Invoc* start,
    heap_Sys* heap,
    proc_std_Direct* std_direct,
    io_Direct* io_direct
))(start_ExitCode) $guard) {
    claim_assert_nonnull(start);
    claim_assert_nonnull(heap);
    claim_assert_nonnull(std_direct);
    claim_assert_nonnull(io_direct);
    defer_(io_Direct_fini(io_direct));
    var mem_direct = proc_mem_Direct_init(heap_Geom_default());
    let io = io_Direct_self(io_direct);
    let std = proc_std_Direct_self(std_direct);
    let mem = proc_mem_Direct_self(&mem_direct);
    let env = start_Invoc_env(start);
    let gpa = heap_Sys_alctr(heap);
    var direct = proc_Direct_init(env, gpa, std);
    let proc = catch_((proc_Direct_self(&direct))(err, {
        pp_if_(main_no_print_err)(
            (let_ignore = err),
            (E_printProgramFailed(&err.any), ETrace_print())
        );
        return_(($debug_point 1));
    }));
    return_(main__runDHMain((proc_Entry){
        .proc = proc,
        .gpa = gpa,
        .io = io,
        .std = std,
        .mem = mem,
        .args = start_Invoc_args(start),
        .env = env,
        .preopens = proc_Preopens_fromStd(std),
    }));
} $unguarded(fn);

$attr($maybe_unused)
$static fn_((main__runDHMainWithHeap(
    start_Invoc* start,
    heap_Sys* heap
))(start_ExitCode) $guard) {
    claim_assert_nonnull(start);
    claim_assert_nonnull(heap);
    defer_(heap_Sys_fini(heap));
    var std_direct = proc_std_Direct_initNative();
    var io_direct = io_Direct_init();
    return_(main__runDHMainWithIo(start, heap, &std_direct, &io_direct));
} $unguarded(fn);

$attr($maybe_unused)
$static fn_((main__runDHMainStart(start_Invoc* start))(start_ExitCode) $guard) {
    claim_assert_nonnull(start);
    defer_(start_Invoc_fini(start));
    var heap = catch_((heap_Sys_init())(err, {
        pp_if_(main_no_print_err)(
            (let_ignore = err),
            (E_printProgramFailed(&err.any), ETrace_print())
        );
        return_(($debug_point 1));
    }));
    return_(main__runDHMainWithHeap(start, &heap));
} $unguarded(fn);
#endif /* main_no_entry, !main_no_entry */

#if env_start_files_linked
#if main_no_entry
fn_((main(void))(int)) {
    return main__runDHMain();
};
#else /* !main_no_entry */
#if plat_is_windows
fn_((main(int argc, const char* argv[]))(int)) {
    let_ignore = argc;
    let_ignore = argv;
    var_(start, start_Invoc) $undefined;
    start_Invoc_initWin32(&start);
    return main__runDHMainStart(&start);
};
#elif plat_is_linux || plat_is_darwin
fn_((main(int argc, const char* argv[], const char* envp[]))(int)) {
    var_(start, start_Invoc) $undefined;
    start_Invoc_initPosix(
        &start,
        as$(usize)(argc),
        as$(P_const$P_const$u8)(argv),
        as$(P_const$P_const$u8)(envp)
    );
    return main__runDHMainStart(&start);
};
#else
fn_((main(int argc, const char* argv[]))(int)) {
    var_(start, start_Invoc) $undefined;
    start_Invoc_initClassic(
        &start,
        as$(usize)(argc),
        as$(P_const$P_const$u8)(argv)
    );
    return main__runDHMainStart(&start);
};
#endif /* platform main entry */
#endif /* main_no_entry || !main_no_entry */

#else /* !env_start_files_linked */
#if plat_is_windows
$attr($maybe_unused $no_return)
$static fn_((main__callDHMainAndExit(void))(void)) {
    start_callInitArray();
#if main_no_entry
    let code = main__runDHMain();
#else
    var_(start, start_Invoc) $undefined;
    start_Invoc_initWin32(&start);
    let code = main__runDHMainStart(&start);
#endif /* main_no_entry */
    start_callFiniArray();
    start_exit(code);
};
#elif plat_is_linux
$attr($maybe_unused $no_return)
$static fn_((main__callDHMainAndExit(P$raw initial_stack))(void)) {
    start_callInitArray();
#if main_no_entry
    let_ignore = initial_stack;
    let code = main__runDHMain();
#else
    var_(start, start_Invoc) $undefined;
    start_Invoc_initLinux(&start, initial_stack);
    let code = main__runDHMainStart(&start);
#endif /* main_no_entry */
    start_callFiniArray();
    start_exit(code);
};
#else
claim_assert_static_msg(false, "dh-main.h no-start process context is not implemented for target");
#endif /* no-start platform */

start_emitEntry(main__callDHMainAndExit);
#endif /* env_start_files_linked, !env_start_files_linked */
#endif /* !TEST_enabled */
#endif /* in_comptime */

#endif /* main__root_included */
#define main dh_main
#endif /* main_no_hijack || !main_no_hijack */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* main__included */
