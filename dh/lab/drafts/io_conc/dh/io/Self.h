#ifndef io_Self__included
#define io_Self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../Sched/base.h"
#include "../exec/base.h"

/*========== Macros and Declarations ========================================*/

typedef enum $packed io_Stream {
    io_Stream_out = 0,
    io_Stream_err,
} io_Stream;

T_alias$((io_Self_VTbl)(struct io_Self_VTbl));
$extern let_(io_VTbl_noop, io_Self_VTbl);
$extern let_(io_VTbl_failing, io_Self_VTbl);

T_alias$((io_Self)(struct io_Self {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(io_Self_VTbl));
}));
T_impl_E$($set(io_direct_E)(io_Self));
$extern let_(io_noop, io_Self);
$extern let_(io_failing, io_Self);

$attr($inline_always)
$static fn_((io_Self_isValid(io_Self self))(bool));
$attr($inline_always)
$static fn_((io_Self_assertValid(P$raw ctx, P_const$$(io_Self_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((io_Self_ensureValid(io_Self self))(io_Self));

$attr($must_check)
$extern fn_((io_direct(void))(io_direct_E$io_Self));
$extern fn_((io_evented(exec_Coop* coop))(io_Self));

struct io_Self_VTbl {
    fn_(((*nlFn)(P$raw ctx, io_Stream stream))(void));
    fn_(((*printVaArgsFn)(P$raw ctx, io_Stream stream, S_const$u8 fmt, va_list va_args))(void));
};
$extern fn_((io_VTbl_noNL(P$raw ctx, io_Stream stream))(void));
$extern fn_((io_VTbl_unreachableNL(P$raw ctx, io_Stream stream))(void));
$extern fn_((io_VTbl_noPrintVaArgs(P$raw ctx, io_Stream stream, S_const$u8 fmt, va_list va_args))(void));
$extern fn_((io_VTbl_unreachablePrintVaArgs(P$raw ctx, io_Stream stream, S_const$u8 fmt, va_list va_args))(void));

/*========== Macros and Definitions =========================================*/

fn_((io_Self_isValid(io_Self self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->nlFn)
        && isNonnull(self.vtbl->printVaArgsFn);
};
fn_((io_Self_assertValid(P$raw ctx, P_const$$(io_Self_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->nlFn);
    claim_assert_nonnull(vtbl->printVaArgsFn);
};
fn_((io_Self_ensureValid(io_Self self))(io_Self)) {
    return io_Self_assertValid(self.ctx, self.vtbl), self;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Self__included */
