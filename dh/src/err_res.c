#include "dh/err_res.h"

#if !defined(ERR_RES_NO_HIJACK_MAIN)
#define ERR_RES_MAIN_INCLUDED (1)

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

#endif /* !defined(ERR_RES_NO_HIJACK_MAIN) */
