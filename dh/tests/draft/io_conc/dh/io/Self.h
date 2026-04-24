#ifndef io_Self__included
#define io_Self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "../Sched/base.h"
#include "../exec/base.h"

/* ========= Macros and Declarations ======================================== */

typedef enum $packed io_Stream {
    io_Stream_out = 0,
    io_Stream_err,
} io_Stream;

T_alias$((io_Self_Vtbl)(struct io_Self_Vtbl {
    fn_(((*nlFn)(P$raw ctx, io_Stream stream))(void));
    fn_(((*printVaArgsFn)(P$raw ctx, io_Stream stream, S_const$u8 fmt, va_list va_args))(void));
}));
T_alias$((io_Self)(struct io_Self {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(io_Self_Vtbl));
}));
$extern fn_((io_direct(void))(io_Self));
$extern fn_((io_evented(exec_Lane* lane))(io_Self));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Self__included */
