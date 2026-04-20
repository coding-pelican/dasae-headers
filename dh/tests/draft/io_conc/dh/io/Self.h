#ifndef io_Self__included
#define io_Self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/* ========= Macros and Declarations ======================================== */

T_alias$((io_Self)(struct io_Self{}));
$extern fn_((io_seq(void))(io_Self));
$extern fn_((io_para(void))(io_Self));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Self__included */
