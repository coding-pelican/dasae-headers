#ifndef heap_common__included
#define heap_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

$attr($inline_always)
$static fn_((heap_alignPage(usize size))(usize));

/*========== Macros and Definitions =========================================*/

fn_((heap_alignPage(usize size))(usize)) {
    return mem_alignFwd(size != 0 ? size : heap_page_size, heap_page_size);
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_common__included */
