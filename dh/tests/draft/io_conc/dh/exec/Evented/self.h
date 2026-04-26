#ifndef exec_Evented_self__included
#define exec_Evented_self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../base.h"
#include "../../time/Dur.h"
#include "dh/posix.h"

/*========== Macros and Declarations ========================================*/

errset_((exec_Evented_E)(
    Unsupported,
    OpenFailed,
    AssociateFailed,
    PollFailed,
    PostFailed,
    Canceled
));
T_decl_E$($set(exec_Evented_E)(exec_Evented));

T_alias$((exec_Evented_Completion)(struct exec_Evented_Completion {
    var_(key, usize);
    var_(op, P$raw);
    var_(bytes, usize);
    var_(err, Err);
}));
T_use_O$(exec_Evented_Completion);
T_use_E$(O$exec_Evented_Completion);

T_alias$((exec_Evented_Handle)(posix_fd_t));
T_alias$((exec_Evented_VTbl)(struct exec_Evented_VTbl));
struct exec_Evented {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(exec_Evented_VTbl));
};
T_impl_E$($set(exec_Evented_E)(exec_Evented));
$extern let_(exec_Evented_noop, exec_Evented);

$extern fn_((exec_Evented_fini(exec_Evented* self))(void));
$attr($must_check)
$extern fn_((exec_Evented_associate(exec_Evented self, exec_Evented_Handle handle, usize key))(E$void));
$attr($must_check)
$extern fn_((exec_Evented_poll(exec_Evented self, time_Dur timeout))(E$O$exec_Evented_Completion));
$attr($must_check)
$extern fn_((exec_Evented_post(exec_Evented self, exec_Evented_Completion completion))(E$void));
$attr($must_check)
$extern fn_((exec_Evented_cancel(exec_Evented self, exec_Evented_Handle handle, P$raw op))(E$void));

struct exec_Evented_VTbl {
    fn_(((*finiFn)(P$raw ctx))(void));
    fn_(((*associateFn)(P$raw ctx, exec_Evented_Handle handle, usize key))(E$void));
    fn_(((*pollFn)(P$raw ctx, time_Dur timeout))(E$O$exec_Evented_Completion));
    fn_(((*postFn)(P$raw ctx, exec_Evented_Completion completion))(E$void));
    fn_(((*cancelFn)(P$raw ctx, exec_Evented_Handle handle, P$raw op))(E$void));
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Evented_self__included */
