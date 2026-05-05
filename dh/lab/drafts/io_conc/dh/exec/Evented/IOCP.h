#ifndef exec_Evented_IOCP__included
#define exec_Evented_IOCP__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "self.h"

/*========== Macros and Declarations ========================================*/

#define exec_Evented_IOCP_supported __bool__exec_Evented_IOCP_supported
#if plat_is_windows
#define __bool__exec_Evented_IOCP_supported pp_true
#else
#define __bool__exec_Evented_IOCP_supported pp_false
#endif

T_alias$((exec_Evented_IOCP)(struct exec_Evented_IOCP {
    var_(port, exec_Evented_Handle);
}));
T_use_E$($set(exec_Evented_E)(exec_Evented_IOCP));

$attr($must_check)
$extern fn_((exec_Evented_IOCP_init(void))(exec_Evented_E$exec_Evented_IOCP));
$extern fn_((exec_Evented_IOCP_fini(exec_Evented_IOCP* self))(void));
$extern fn_((exec_Evented_IOCP_evented(exec_Evented_IOCP* self))(exec_Evented));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Evented_IOCP__included */
