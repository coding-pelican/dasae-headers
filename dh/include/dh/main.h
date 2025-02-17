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
#include "dh/err_res.h"
#include "dh/variant.h"

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
extern void dh_main(void);
#elif main_no_args && !main_no_returns_err
extern Err$void dh_main(void) must_check;
#elif !main_no_args && main_no_returns_err
extern void dh_main(int argc, const char* argv[]);
#else  /* !main_no_args && !main_no_returns_err */
extern Err$void dh_main(int argc, const char* argv[]) must_check;
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
    catch (dh_main(), err, {
        ignore fprintf(
            stderr,
            "Program failed: [%s] %s(%d)\n",
            Err_domainToCStr(err),
            Err_codeToCStr(err),
            err.ctx
        );
        claim_unreachable;
    });
#elif !main_no_args && main_no_returns_err
    dh_main();
#else  /* !main_no_args && !main_no_returns_err */
    catch (dh_main(argc, argv), err, {
        ignore fprintf(
            stderr,
            "Program failed: [%s] %s(%d)\n",
            Err_domainToCStr(err),
            Err_codeToCStr(err),
            err.ctx
        );
        claim_unreachable;
    });
#endif /* !main_no_args && !main_no_returns_err */
    return 0;
}

#endif /* MAIN_ROOT_INCLUDED */

#endif /* !main_no_hijack */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MAIN_INCLUDED */
