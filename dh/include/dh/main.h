/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    main.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-30 (date of creation)
 * @updated 2024-12-30 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief
 * @details
 */

#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"

/*========== Macros and Definitions =========================================*/

extern must_check Err$void dh_main(int argc, const char* argv[]);

#ifndef ERR_RES_MAIN_INCLUDED
#define ERR_RES_MAIN_INCLUDED (0)
#endif /* ERR_RES_MAIN_INCLUDED */

/* Error handling root main */
#if !defined(ERR_RES_MAIN_NO_HIJACK)
#if !ERR_RES_MAIN_INCLUDED
#undef ERR_RES_MAIN_INCLUDED
#define ERR_RES_MAIN_INCLUDED (1)

// extern int main(int argc, const char* argv[]);
int main(int argc, const char* argv[]) {
    const Err$void result = dh_main(argc, argv);
    if (!result.is_err) { return 0; }
    ignore fprintf(
        stderr,
        "Program failed: %s (type: %d)\n",
        Err_message(result.err),
        Err_type(result.err)
    );
    return 1;
}

#endif /* !ERR_RES_MAIN_INCLUDED */
#endif /* !defined(ERR_RES_NO_HIJACK_MAIN) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MAIN_INCLUDED */
