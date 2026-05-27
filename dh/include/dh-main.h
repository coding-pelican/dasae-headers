/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    main.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-30 (date of creation)
 * @updated 2026-05-27 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  (none)
 *
 * @brief   hijacked main for error handling
 */
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

/*========== Macros =========================================================*/

#if !defined(main_no_hijack)
#define main_no_hijack 0
#endif /* !defined(main_no_hijack) */

#if !defined(main_no_args)
#define main_no_args 0
#endif /* !defined(main_no_args) */

#if !defined(main_no_returns_err)
#define main_no_returns_err 0
#endif /* !defined(main_no_returns_err) */

/*========== Definitions ====================================================*/

#if main_no_hijack
/* No hijack, just call main as usual */
#else /* !main_no_hijack */

$attr(pp_if_(pp_not(main_no_returns_err))(pp_then_($must_check)))
$attr(pp_if_(TEST_comp_enabled)(
    pp_then_($maybe_unused $static),
    pp_else_($extern)
))
fn_((dh_main(pp_if_(pp_not(main_no_args))(
    pp_then_(S$S_const$u8 args),
    pp_else_(void)
)))(pp_if_(pp_not(main_no_returns_err))(pp_then_(E$void), pp_else_(void))));

/*========== Root main ======================================================*/

#ifndef main_root_included
#define main_root_included 1

#if !TEST_comp_enabled
$attr($maybe_unused)
$static fn_((main__runDHMain(pp_if_(pp_not(main_no_args))(
    pp_then_(usize argc, const char* argv[]),
    pp_else_(void)
)))(start_ExitCode)) {
    debug_StackTrace_setupCrashHandler();
    pp_if_(pp_not(main_no_args))((
        let args = local_({
            let ref_args_z0 = P_prefix$((S$P_const$u8)(ptrCast$((P_const$u8*)(argv)))(argc));
            let buf_args_span = u_castS$((S$S_const$u8)(u_allocA(ref_args_z0.len, typeInfo$(S_const$u8))).ref);
            for_(($s(buf_args_span), $s(ref_args_z0))(span, z0)) { *span = mem_spanZ0$u8(*z0); } $end(for);
            local_return_(buf_args_span);
        });
    ));
    pp_if_(pp_not(main_no_returns_err))(
        (let returned),
        (let_ignore)
    ) = dh_main(pp_if_(pp_not(main_no_args))(pp_then_(args)));
    pp_if_(pp_not(main_no_returns_err))(
        (catch_((returned)(err, {
            E_printProgramFailed(&err);
            ETrace_print();
            return $debug_point 1;
        })))
    );
    return 0;
};

#if comp_start_files_linked
fn_((main(pp_if_(pp_not(main_no_args))(
    pp_then_(int argc, const char* argv[]),
    pp_else_(void)
)))(int)) {
    return main__runDHMain(pp_if_(pp_not(main_no_args))(pp_then_(as$(usize)(argc), argv)));
};
#else /* !comp_start_files_linked */
$attr($no_return $maybe_unused)
$static fn_((main__callMainAndExit(P$raw raw_ctx))(void)) {
    pp_if_(main_no_args)((let_ignore = raw_ctx;));
    start_callInitArray();
    let code = pp_if_(main_no_args)(
        (main__runDHMain()),
        (pp_switch_((plat_type)(
            pp_case_((plat_type_linux)(local_({
                let argc_argv_ptr = as$(usize*)(raw_ctx);
                let argc = argc_argv_ptr[0];
                let argv = as$(const char**)(ptrCast$(argc_argv_ptr + 1));
                local_return_(main__runDHMain(argc, argv));
            }))),
            pp_default_(local_({
                claim_assert_static_msg(false, "dh-main.h no-start entry with args is only implemented for linux");
                local_return_(as$(start_ExitCode)(1));
            }))
        )))
    );
    start_callFiniArray();
    start_exit(code);
};

start_emitEntry(main__callMainAndExit);
#endif /* comp_start_files_linked */
#endif /* !TEST_comp_enabled */

#endif /* main_root_included */
#define main dh_main
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* main__included */
