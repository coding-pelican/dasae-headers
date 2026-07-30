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
#include "dh/mem/common.h"
#include "dh/start.h"
#include "dh/proc.h"
#include "dh/io/self.h"
#include "dh/TEST.h"

/*========== Macros and Declarations ========================================*/

/*---------- Configuration Flags --------------------------------------------*/

#if !defined(main_no_hijack)
#define main_no_hijack __comp_bool__main_no_hijack
#endif /* !defined(main_no_hijack) */
#define main__no_hijack_default __comp_flag__main__no_hijack_default

#if !defined(main_no_args)
#define main_no_args __comp_bool__main_no_args
#endif /* !defined(main_no_args) */
#define main__no_args_default __comp_flag__main__no_args_default

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
#define __comp_bool__main_no_args main__no_args_default
#define __comp_flag__main__no_args_default main_no_hijack
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
fn_((dh_main(pp_if_(pp_not(main_no_args))(
    pp_then_(proc_Self self),
    pp_else_(void)
)))(pp_if_(pp_not(main_no_return_err))(pp_then_(E$void), pp_else_(void))));

/*---------- Root main ------------------------------------------------------*/

#ifndef main__root_included
#define main__root_included 1

#if in_comptime
#if !TEST_enabled
#if main_no_args
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

#else /* !main_no_args */
$attr($maybe_unused $inline)
$static fn_((main__runDHMain(proc_Self self))(start_ExitCode)) {
    debug_StackTrace_setupCrashHandler();
    pp_if_(pp_not(main_no_return_err))(
        (let returned =),
        ($ignore_void)
    ) dh_main(self);
    pp_if_(pp_not(main_no_return_err))((catch_((returned)(err, {
        pp_if_(main_no_print_err)(
            (let_ignore = err),
            (E_printProgramFailed(&err), ETrace_print())
        );
        return $debug_point 1;
    }))));
    return 0;
};

$attr($maybe_unused $inline_always)
$static fn_((main__fillZ0Spans(usize count, P_const$raw z0s_raw, S$S_const$u8 out))(void)) {
    claim_assert(count == out.len);
    if (count == 0) return;
    let z0s = as$(const char* const*)(z0s_raw);
    for (var_(i, usize) = 0; i < count; ++i) {
        out.ptr[i] = mem_spanZ0$u8(as$(const u8*)(z0s[i]));
    };
};

$attr($maybe_unused $inline_always)
$static fn_((main__preopens(proc_Preopen out[3]))(proc_Preopens)) {
#if plat_is_windows || plat_is_linux || plat_is_darwin
    out[0] = (proc_Preopen){
        .name = u8_l("stdin"),
        .tag = proc_Preopen_Tag_file,
        .file = io_handleStdIn(),
    };
    out[1] = (proc_Preopen){
        .name = u8_l("stdout"),
        .tag = proc_Preopen_Tag_file,
        .file = io_handleStdOut(),
    };
    out[2] = (proc_Preopen){
        .name = u8_l("stderr"),
        .tag = proc_Preopen_Tag_file,
        .file = io_handleStdErr(),
    };
    return (proc_Preopens){
        .items = P_prefix$((S_const$proc_Preopen)(out)(3)),
    };
#else
    let_ignore = out;
    return (proc_Preopens){
        .items = { .ptr = ptrFromInt$((P$raw)(mem_emptyAddr(0))), .len = 0 },
    };
#endif /* platform standard resources */
};

$attr($maybe_unused $inline)
$static fn_((main__runDHMainFromStart(start_Info info))(start_ExitCode)) {
    let arg_capacity = info.argc == 0 ? usize_(1) : info.argc;
    let env_capacity = info.envc == 0 ? usize_(1) : info.envc;
    let args_buf = u_castS$((S$S_const$u8)(u_allocA(arg_capacity, typeInfo$(S_const$u8))).ref);
    let env_buf = u_castS$((S$S_const$u8)(u_allocA(env_capacity, typeInfo$(S_const$u8))).ref);
    let args = P_prefix$((S$S_const$u8)(args_buf.ptr)(info.argc));
    let environ = P_prefix$((S$S_const$u8)(env_buf.ptr)(info.envc));
    main__fillZ0Spans(info.argc, info.argv_raw, args);
    main__fillZ0Spans(info.envc, info.envp_raw, environ);

    var_(preopen_buf, proc_Preopen)[3] = cleared();
    let self = (proc_Self){
        .args = { .items = P_prefix$((S_const$S_const$u8)(args.ptr)(args.len)) },
        .environ = { .block = P_prefix$((S_const$S_const$u8)(environ.ptr)(environ.len)) },
        .preopens = main__preopens(preopen_buf),
    };
    return main__runDHMain(self);
};

#if plat_is_windows
$attr($maybe_unused $inline)
$static fn_((main__runDHMainFromWin32(void))(start_ExitCode)) {
    let cmd_z = ptrCast$((const u8*)(GetCommandLineA()));
    let cmd_len = mem_lenZ0$u8(cmd_z);
    let cmd_buf = u_castS$((S$u8)(u_allocA(cmd_len + 1, typeInfo$(u8))).ref);
    let args_buf = u_castS$((S$S_const$u8)(u_allocA(cmd_len + 1, typeInfo$(S_const$u8))).ref);
    var src = cmd_z;
    var dst = cmd_buf.ptr;
    var_(argc, usize) = 0;
    while (*src != 0) {
        while (*src == u8_c(' ') || *src == u8_c('\t')) ++src;
        if (*src == 0) break;
        let arg_start = dst;
        var_(in_quotes, bool) = false;
        while (*src != 0) {
            if (!in_quotes && (*src == u8_c(' ') || *src == u8_c('\t'))) break;
            if (*src == u8_c('"')) {
                in_quotes = !in_quotes;
                ++src;
                continue;
            };
            if (*src == u8_c('\\')) {
                var_(slashes, usize) = 0;
                while (*src == u8_c('\\')) {
                    ++slashes;
                    ++src;
                };
                if (*src == u8_c('"')) {
                    for (var_(i, usize) = 0; i < slashes / 2; ++i) *dst++ = u8_c('\\');
                    if ((slashes & 1) == 0) in_quotes = !in_quotes;
                    else *dst++ = u8_c('"');
                    ++src;
                    continue;
                };
                for (var_(i, usize) = 0; i < slashes; ++i) *dst++ = u8_c('\\');
                continue;
            };
            *dst++ = *src++;
        };
        args_buf.ptr[argc++] = (S_const$u8){
            .ptr = arg_start,
            .len = as$(usize)(dst - arg_start),
        };
        *dst++ = u8_c(0);
    };

    let env_z = GetEnvironmentStringsA();
    var_(envc, usize) = 0;
    if (env_z != null) {
        var env_it = as$(const u8*)(env_z);
        while (*env_it != 0) {
            ++envc;
            env_it += mem_lenZ0$u8(env_it) + 1;
        };
    };
    let env_capacity = envc == 0 ? usize_(1) : envc;
    let env_buf = u_castS$((S$S_const$u8)(u_allocA(env_capacity, typeInfo$(S_const$u8))).ref);
    if (env_z != null) {
        var env_it = as$(const u8*)(env_z);
        for (var_(i, usize) = 0; i < envc; ++i) {
            env_buf.ptr[i] = mem_spanZ0$u8(env_it);
            env_it += env_buf.ptr[i].len + 1;
        };
    };

    var_(preopen_buf, proc_Preopen)[3] = cleared();
    let self = (proc_Self){
        .args = { .items = P_prefix$((S_const$S_const$u8)(args_buf.ptr)(argc)) },
        .environ = { .block = P_prefix$((S_const$S_const$u8)(env_buf.ptr)(envc)) },
        .preopens = main__preopens(preopen_buf),
    };
    let code = main__runDHMain(self);
    if (env_z != null) claim_assert(FreeEnvironmentStringsA(env_z));
    return code;
};
#endif /* plat_is_windows */
#endif /* main_no_args, !main_no_args */

#if env_start_files_linked
#if main_no_args
fn_((main(void))(int)) {
    return main__runDHMain();
};
#else /* !main_no_args */
#if plat_is_windows
fn_((main(int argc, const char* argv[]))(int)) {
    let_ignore = argc;
    let_ignore = argv;
    return main__runDHMainFromWin32();
};
#elif plat_is_linux || plat_is_darwin
fn_((main(int argc, const char* argv[], const char* envp[]))(int)) {
    return main__runDHMainFromStart(start_Info_fromMain(
        as$(usize)(argc),
        as$(P_const$raw)(argv),
        as$(P_const$raw)(envp)));
};
#else
fn_((main(int argc, const char* argv[]))(int)) {
    return main__runDHMainFromStart(start_Info_fromMain(
        as$(usize)(argc),
        as$(P_const$raw)(argv), null));
};
#endif /* platform main entry */
#endif /* main_no_args || !main_no_args */

#else /* !env_start_files_linked */
$attr($maybe_unused $no_return)
$static fn_((main__callDHMainAndExit(P$raw raw_ctx))(void)) {
    start_callInitArray();
    let code = pp_if_(main_no_args)(
        (main__runDHMain()),
        (pp_switch_((plat_type)(
            pp_case_((plat_type_linux)(main__runDHMainFromStart(start_Info_fromRaw(raw_ctx)))),
            pp_case_((plat_type_windows)(main__runDHMainFromWin32())),
            pp_default_(local_({
                claim_assert_static_msg(false, "dh-main.h no-start process context is not implemented for target");
                local_return_(as$(start_ExitCode)(1));
            }))
        )))
    );
    start_callFiniArray();
    start_exit(code);
};

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
