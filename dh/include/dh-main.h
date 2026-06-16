/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    main.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-30 (date of creation)
 * @updated 2026-06-03 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  (none)
 *
 * @brief   hijacked main for error handling
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
/* No hijack, just call main as usual */
#else /* !main_no_hijack */

$attr(pp_if_(pp_not(main_no_return_err))(pp_then_($must_check)))
$attr(pp_if_(TEST_enabled)(
    pp_then_($maybe_unused $static),
    pp_else_($extern)
))
fn_((dh_main(pp_if_(pp_not(main_no_args))(
    pp_then_(S$S_const$u8 args),
    pp_else_(void)
)))(pp_if_(pp_not(main_no_return_err))(pp_then_(E$void), pp_else_(void))));

/*---------- Root main ------------------------------------------------------*/

#ifndef main_root_included
#define main_root_included 1

#if on_comptime
#if !TEST_enabled
#if main_no_args
$attr($maybe_unused)
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
$attr($maybe_unused)
$static fn_((main__runDHMainArgs(S$S_const$u8 args))(start_ExitCode)) {
    debug_StackTrace_setupCrashHandler();
    pp_if_(pp_not(main_no_return_err))(
        (let returned =),
        ($ignore_void)
    ) dh_main(args);
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
$static fn_((main__runDHMainFromArgv(usize argc, const char* argv[]))(start_ExitCode)) {
    let args = local_({
        let ref_args_z0 = P_prefix$((S$P_const$u8)(ptrCast$((P_const$u8*)(argv)))(argc));
        let buf_args_span = u_castS$((S$S_const$u8)(u_allocA(ref_args_z0.len, typeInfo$(S_const$u8))).ref);
        for_(($s(buf_args_span), $s(ref_args_z0))(span, z0)) { *span = mem_spanZ0$u8(*z0); } $end(for);
        local_return_(buf_args_span);
    });
    return main__runDHMainArgs(args);
};

pp_if_(plat_is_windows)(pp_then_(
    $attr($maybe_unused)
    $static fn_((main__runDHMainFromWin32CmdLine(void))(start_ExitCode)) {
        let cmd_z = ptrCast$((const u8*)(GetCommandLineA()));
        let cmd_len = mem_lenZ0$u8(cmd_z);
        let cmd_buf = u_castS$((S$u8)(u_allocA(cmd_len + 1, typeInfo$(u8))).ref);
        let args = u_castS$((S$S_const$u8)(u_allocA(cmd_len + 1, typeInfo$(S_const$u8))).ref);
        var src = cmd_z;
        var dst = cmd_buf.ptr;
        var argc = usize_(0);
        while (*src != 0) {
            while (*src == u8_c(' ') || *src == u8_c('\t')) ++src;
            if (*src == 0) break;
            let arg_start = dst;
            var in_quotes = false;
            while (*src != 0) {
                if (!in_quotes && (*src == u8_c(' ') || *src == u8_c('\t'))) break;
                if (*src == u8_c('"')) {
                    in_quotes = !in_quotes;
                    ++src;
                    continue;
                };
                if (*src == u8_c('\\')) {
                    var slashes = usize_(0);
                    while (*src == u8_c('\\')) {
                        ++slashes;
                        ++src;
                    };
                    if (*src == u8_c('"')) {
                        for (var i = usize_(0); i < slashes / 2; ++i) {
                            *dst++ = u8_c('\\');
                        };
                        if ((slashes & 1) == 0) {
                            in_quotes = !in_quotes;
                        } else {
                            *dst++ = u8_c('"');
                        };
                        ++src;
                        continue;
                    };
                    for (var i = usize_(0); i < slashes; ++i) {
                        *dst++ = u8_c('\\');
                    };
                    continue;
                };
                *dst++ = *src++;
            };
            asg_l((S_at((args)[argc]))({ .ptr = arg_start, .len = as$(usize)(dst - arg_start) }));
            ++argc;
            *dst++ = u8_c(0);
        };
        return main__runDHMainArgs(P_prefix$((S$S_const$u8)(args.ptr)(argc)));
    };
));
#endif /* main_no_args || !main_no_args */

#if comp_start_files_linked
fn_((main(pp_if_(pp_not(main_no_args))(
    pp_then_(int argc, const char* argv[]),
    pp_else_(void)
)))(int)) {
    return pp_if_(main_no_args)(
        (main__runDHMain()),
        (main__runDHMainFromArgv(as$(usize)(argc), argv))
    );
};

#else /* !comp_start_files_linked */
$attr($no_return $maybe_unused)
$static fn_((main__callDHMainAndExit(P$raw raw_ctx))(void)) {
    let_ignore = raw_ctx;
    start_callInitArray();
    let code = pp_if_(main_no_args)(
        (main__runDHMain()),
        (pp_switch_((plat_type)(
            pp_case_((plat_type_linux)(local_({
                let argc_argv_ptr = as$(usize*)(raw_ctx);
                let argc = argc_argv_ptr[0];
                let argv = as$(const char**)(ptrCast$(argc_argv_ptr + 1));
                local_return_(main__runDHMainFromArgv(argc, argv));
            }))),
            pp_case_((plat_type_windows)(local_({
                local_return_(main__runDHMainFromWin32CmdLine());
            }))),
            pp_default_(local_({
                claim_assert_static_msg(false, "dh-main.h no-start entry with args is not implemented for target");
                local_return_(as$(start_ExitCode)(1));
            }))
        )))
    );
    start_callFiniArray();
    start_exit(code);
};

start_emitEntry(main__callDHMainAndExit);
#endif /* comp_start_files_linked || !comp_start_files_linked */
#endif /* !TEST_enabled */
#endif /* on_comptime */

#endif /* main_root_included */
#define main dh_main
#endif /* main_no_hijack || !main_no_hijack */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* main__included */
