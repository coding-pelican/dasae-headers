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
#include "dh/opt.h"
#include "dh/err_res.h"
#include "dh/variant.h"

/*========== Macros and Definitions =========================================*/

#ifndef MAIN_RETURNS_ERR_OR_VOID
#define MAIN_RETURNS_ERR_OR_VOID (0)
#endif /* MAIN_RETURNS_ERR_OR_VOID */

/* Error handling root main */
#if !defined(MAIN_NO_HIJACK)
#if !MAIN_RETURNS_ERR_OR_VOID
#undef MAIN_RETURNS_ERR_OR_VOID
#define MAIN_RETURNS_ERR_OR_VOID (1)

#if !defined(MAIN_NO_ARGS)

extern must_check Err$void dh_main(int argc, const char* argv[]);

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

/* int main(int argc, const char* argv[]) {
    scope_defer {
        // Initialize logging to a file
        scope_if(let debug_file = fopen("debug.log", "w"), debug_file) {
            log_initWithFile(debug_file);
            // Configure logging behavior
            log_showTimestamp(true);
            log_showLocation(true);
            log_showLevel(true);
            log_setLevel(log_Level_debug);
        }
        defer(log_fini());

        const Err$void result = dh_main(argc, argv);
        if (!result.is_err) {
            defer_return;
            return 0;
        }

        log_error("Program failed: %s (type: %d)\n", Err_message(result.err), Err_type(result.err));
    }
    scope_deferred;
    return 1;
} */

#else /* defined(MAIN_NO_ARGS) */

extern must_check Err$void dh_main(void);

int main(int argc, const char* argv[]) {
    unused(argc), unused(argv);
    const Err$void result = dh_main();
    if (!result.is_err) { return 0; }
    ignore fprintf(
        stderr,
        "Program failed: %s (type: %d)\n",
        Err_message(result.err),
        Err_type(result.err)
    );
    return 1;
}

#endif /* !defined(MAIN_NO_ARGS) */

#endif /* !MAIN_RETURNS_ERR_OR_VOID */
#endif /* !defined(MAIN_NO_HIJACK) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MAIN_INCLUDED */
