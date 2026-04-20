/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    main.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-30 (date of creation)
 * @updated 2026-02-22 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  (none)
 *
 * @brief   hijacked main for error handling
 * @details Provides a hijacked main function for error handling.
 */
#ifndef main__included
#define main__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/TEST.h"
#include "dh/mem/common.h"

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
$extern fn_((dh_main(pp_if_(pp_not(main_no_args))(
    pp_then_(S$S_const$u8 args),
    pp_else_(void)
)))(pp_if_(pp_not(main_no_returns_err))(pp_then_(E$void), pp_else_(void))));

/*========== Root main ======================================================*/

#ifndef main_root_included
#define main_root_included 1
#if TEST_comp_enabled
fn_((main(void))(int)) {
    return TEST_Framework_run(), 0;
};
#else /* !TEST_comp_enabled */
fn_((main(pp_if_(pp_not(main_no_args))(
    pp_then_(int argc, const char* argv[]),
    pp_else_(void)
)))(int)) {
    debug_StkTrace_setupCrashHandler();
    pp_if_(pp_not(main_no_args))((
        let arg_count = as$(usize)(argc);
        let args = local_({
            let ref_args_z0 = P_prefix$((S$P_const$u8)(ptrCast$((P_const$u8*)(argv)))(arg_count));
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
            Err_print(err);
            ErrTrace_print();
            return $debug_point 1;
        })))
    );
    return 0;
};
#endif
#endif /* da_main__root_included */
#define main dh_main
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* main__included */
