/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    main.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-30 (date of creation)
 * @updated 2025-02-16 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
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

#include "dh/core.h"
#include "dh/prl.h"
#include "dh/Str.h"
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

pp_attr_(pp_if_(pp_not_(main_no_returns_err))(pp_then_($must_check)))
$extern fn_((dh_main(pp_if_(pp_not_(main_no_args))(
    pp_then_(S$S_const$u8 args),
    pp_else_(void)
)))(pp_if_(pp_not_(main_no_returns_err))(pp_else_(E$void), pp_then_(void))));

/*========== Root main ======================================================*/

#ifndef main__root_included
#define main__root_included 1

#if !TEST_comp_enabled

// $static $inline_always
// fn_((dh_main__finiMemTrace(void))(void)) {
// #if defined(MEM_NO_TRACE_ALLOC_AND_FREE) || !debug_comp_enabled
// #else
//     extern fn_((mem_Tracker_finiAndGenerateReport(void))(void));
//     mem_Tracker_finiAndGenerateReport();
// #endif /* defined(MEM_NO_TRACE_ALLOC_AND_FREE) || !debug_comp_enabled */
// }

fn_((main(pp_if_(pp_not_(main_no_args))(
    pp_then_(int argc, const char* argv[]),
    pp_else_(void)
)))(int)) {
#if main_no_args && main_no_returns_err
    dh_main();
#elif main_no_args && !main_no_returns_err
    catch_((dh_main())(err, {
        Err_print(err);
        ErrTrace_print();
        return (debug_break(), 1);
    }));
#elif !main_no_args && main_no_returns_err
    let args_buf = as$((S_const$u8*)(bti_alloca(sizeOf$(S_const$u8) * argc)));
    let args     = ({
        for (i32 i = 0; i < argc; ++i) {
            args_buf[i] = Str_viewZ(as$((const u8*)(argv[i])));
        }
        blk_return_(S_from$(S$S_const$u8, args_buf, argc));
    });
    dh_main(args);
#else /* !main_no_args && !main_no_returns_err */
    let args_buf = as$((S_const$u8*)(bti_alloca(sizeOf$(S_const$u8) * argc)));
    let args     = ({
        for (i32 i = 0; i < argc; ++i) {
            args_buf[i] = Str_viewZ(as$((const u8*)(argv[i])));
        }
        blk_return_(S_from$(S$S_const$u8, args_buf, argc));
    });
    catch_((dh_main(args))(err, {
        Err_print(err);
        ErrTrace_print();
        return (debug_break(), 1);
    }));
#endif
    return 0;
}
#else  /* TEST_comp_enabled */
fn_((main(void))(int)) {
    TEST_Framework_run();
    return 0;
}
#endif /* TEST_comp_enabled */

#endif /* main__root_included */

#endif /* !main_no_hijack */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* main__included */
