/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    main.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-30 (date of creation)
 * @updated 2025-02-16 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   hijacked main for error handling
 * @details
 */

#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/scope.h"
#include "dh/opt.h"
#include "dh/Err.h"
#include "dh/ErrTrace.h"
#include "dh/err_res.h"
#include "dh/variant.h"
#include "dh/union_enum.h"
#include "dh/Str.h"
#include "dh/fn.h"

/*========== Macros =========================================================*/

#if !defined(main_no_hijack)
#define main_no_hijack (0)
#endif /* !defined(main_no_hijack) */

#if !defined(main_no_args)
#define main_no_args (0)
#endif /* !defined(main_no_args) */

#if !defined(main_no_returns_err)
#define main_no_returns_err (0)
#endif /* !defined(main_no_returns_err) */

/*========== Definitions ====================================================*/

#if main_no_hijack
/* No hijack, just call main as usual */
#else /* !main_no_hijack */

#if main_no_args && main_no_returns_err
pub fn_(dh_main(void), void);
#elif main_no_args && !main_no_returns_err
pub fn_(dh_main(void), must_check Err$void);
#elif !main_no_args && main_no_returns_err
use_Sli$(Str_const);
pub fn_(dh_main(Sli$Str_const args), void);
#else  /* !main_no_args && !main_no_returns_err */
use_Sli(Str_const);
pub fn_(dh_main(Sli$Str_const args), must_check Err$void);
#endif /* !main_no_args && !main_no_returns_err */

/*========== Root main ======================================================*/

#ifndef MAIN_ROOT_INCLUDED
#define MAIN_ROOT_INCLUDED (1)

int main(
#if main_no_args && main_no_returns_err
    void
#elif main_no_args && !main_no_returns_err
    void
#elif !main_no_args && main_no_returns_err
    int argc, const char* argv[]
#else  /* !main_no_args && !main_no_returns_err */
    int argc, const char* argv[]
#endif /* !main_no_args && !main_no_returns_err */
) {
#if main_no_args && main_no_returns_err
    dh_main();
#elif main_no_args && !main_no_returns_err
    catch_from(dh_main(), err, eval({
        Err_print(err);
        ErrTrace_print();
        claim_unreachable;
    }));
#elif !main_no_args && main_no_returns_err
    let args_buf = as$(Str_const*, bti_alloca(sizeOf$(Str_const) * argc));
    let args     = eval({
        for (i32 i = 0; i < argc; ++i) {
            args_buf[i] = Str_viewZ(as$(const u8*, argv[i]));
        }
        eval_return make$(Sli$Str_const, .ptr = args_buf, .len = argc);
    });
    dh_main(args);
#else  /* !main_no_args && !main_no_returns_err */
    let args_buf = as$(Str_const*, bti_alloca(sizeOf$(Str_const) * argc));
    let args     = eval({
        for (i32 i = 0; i < argc; ++i) {
            args_buf[i] = Str_viewZ(as$(const u8*, argv[i]));
        }
        eval_return make$(Sli$Str_const, .ptr = args_buf, .len = argc);
    });
    catch_from(dh_main(args), err, eval({
        Err_print(err);
        ErrTrace_print();
        claim_unreachable;
    }));
#endif /* !main_no_args && !main_no_returns_err */
    return 0;
}

#endif /* MAIN_ROOT_INCLUDED */

#endif /* !main_no_hijack */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MAIN_INCLUDED */
