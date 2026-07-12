/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Que.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_Que
 * @brief   Endpoint-less OS-thread blocking queue surface.
 */
#pragma once
#ifndef thrd_Que__included
#define thrd_Que__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "chan.h"
#include "Select.h"
#include "../ArrQue.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_Que {
    var_(que, ArrQue);
    var_(closed, bool);
    var_(lock, thrd_Mtx);
    var_(send_waiters, thrd_wait_List);
    var_(recv_waiters, thrd_wait_List);
    var_(type, debug_TypeInfo);
} thrd_Que;
typedef struct thrd_Que_SendOp {
    var_(que, thrd_Que*);
    var_(item, u_P$raw);
} thrd_Que_SendOp;
typedef struct thrd_Que_RecvOp {
    var_(que, thrd_Que*);
    var_(type, debug_TypeInfo);
} thrd_Que_RecvOp;

#define thrd_Que$(_T...) __alias__thrd_Que$(_T)
#define thrd_Que$$(_T...) __anon__thrd_Que$$(_T)
#define T_decl_thrd_Que$(_T...) __gen__T_decl_thrd_Que$(_T)
#define T_impl_thrd_Que$(_T...) __gen__T_impl_thrd_Que$(_T)
#define T_use_thrd_Que$(_T...) __gen__T_use_thrd_Que$(_T)

$extern fn_((thrd_Que_init(u_S$raw buf))(thrd_Que));
#define T_use_thrd_Que_init$(_T...) __gen__T_use_thrd_Que_init$(_T)
$extern fn_((thrd_Que_fini(thrd_Que* self))(void));
#define T_use_thrd_Que_fini$(_T...) __gen__T_use_thrd_Que_fini$(_T)

$extern fn_((thrd_Que_len(thrd_Que* self))(usize));
#define T_use_thrd_Que_len$(_T...) __gen__T_use_thrd_Que_len$(_T)
$extern fn_((thrd_Que_cap(thrd_Que* self))(usize));
#define T_use_thrd_Que_cap$(_T...) __gen__T_use_thrd_Que_cap$(_T)
$extern fn_((thrd_Que_isFull(thrd_Que* self))(bool));
#define T_use_thrd_Que_isFull$(_T...) __gen__T_use_thrd_Que_isFull$(_T)
$extern fn_((thrd_Que_isEmpty(thrd_Que* self))(bool));
#define T_use_thrd_Que_isEmpty$(_T...) __gen__T_use_thrd_Que_isEmpty$(_T)
$extern fn_((thrd_Que_isOpen(thrd_Que* self))(bool));
#define T_use_thrd_Que_isOpen$(_T...) __gen__T_use_thrd_Que_isOpen$(_T)
$extern fn_((thrd_Que_isClosed(thrd_Que* self))(bool));
#define T_use_thrd_Que_isClosed$(_T...) __gen__T_use_thrd_Que_isClosed$(_T)

$extern fn_((thrd_Que_close(thrd_Que* self))(void));
#define T_use_thrd_Que_close$(_T...) __gen__T_use_thrd_Que_close$(_T)

$attr($must_check)
$extern fn_((thrd_Que_trySend(thrd_Que* self, u_V$raw item))(thrd_chan_E$void));
#define T_use_thrd_Que_trySend$(_T...) __gen__T_use_thrd_Que_trySend$(_T)
$attr($must_check)
$extern fn_((thrd_Que_send(thrd_Que* self, u_V$raw item, thrd_wait_Src cancel_src))(thrd_chan_WaitE$void));
#define T_use_thrd_Que_send$(_T...) __gen__T_use_thrd_Que_send$(_T)
$attr($must_check)
$extern fn_((thrd_Que_sendFor(thrd_Que* self, u_V$raw item, thrd_wait_Src cancel_src, time_Dur dur))(thrd_chan_TimedE$void));
#define T_use_thrd_Que_sendFor$(_T...) __gen__T_use_thrd_Que_sendFor$(_T)
$extern fn_((thrd_Que_sendOp(thrd_Que* self, u_P$raw item))(thrd_Que_SendOp));
#define T_use_thrd_Que_sendOp$(_T...) __gen__T_use_thrd_Que_sendOp$(_T)
$extern fn_((thrd_Que_SendOp_op(thrd_Que_SendOp* self))(thrd_Select_Op));

$attr($must_check)
$extern fn_((thrd_Que_tryRecv(thrd_Que* self, u_V$raw ret_mem))(thrd_chan_E$u_V$raw));
#define T_use_thrd_Que_tryRecv$(_T...) __gen__T_use_thrd_Que_tryRecv$(_T)
$attr($must_check)
$extern fn_((thrd_Que_recv(thrd_Que* self, thrd_wait_Src cancel_src, u_V$raw ret_mem))(thrd_chan_WaitE$u_V$raw));
#define T_use_thrd_Que_recv$(_T...) __gen__T_use_thrd_Que_recv$(_T)
$attr($must_check)
$extern fn_((thrd_Que_recvFor(thrd_Que* self, thrd_wait_Src cancel_src, time_Dur dur, u_V$raw ret_mem))(thrd_chan_TimedE$u_V$raw));
#define T_use_thrd_Que_recvFor$(_T...) __gen__T_use_thrd_Que_recvFor$(_T)
$extern fn_((thrd_Que_recvOp(thrd_Que* self, TypeInfo type))(thrd_Que_RecvOp));
#define T_use_thrd_Que_recvOp$(_T...) __gen__T_use_thrd_Que_recvOp$(_T)
$extern fn_((thrd_Que_RecvOp_op(thrd_Que_RecvOp* self))(thrd_Select_Op));

/*========== Macros and Definitions =========================================*/

/* clang-format off */
#define __alias__thrd_Que$(_T...) tpl$(thrd_Que, _T)
#define __anon__thrd_Que$$(_T...) TypeOf(union { \
    T_embed$(struct { \
        var_(que, ArrQue$$(_T)); \
        var_(closed, bool); \
        var_(lock, thrd_Mtx); \
        var_(send_waiters, thrd_wait_List); \
        var_(recv_waiters, thrd_wait_List); \
        var_(type, debug_TypeInfo); \
    }); \
    var_(as_raw, thrd_Que) $like_ref; \
})
#define __gen__T_decl_thrd_Que$(_T...) \
    T_decl_ArrQue$(_T); \
    $maybe_unused typedef union thrd_Que$(_T) thrd_Que$(_T); \
    T_decl_E$($set(thrd_chan_E)(_T)); \
    T_decl_E$($set(thrd_chan_WaitE)(_T)); \
    T_decl_E$($set(thrd_chan_TimedE)(_T))
#define __gen__T_impl_thrd_Que$(_T...) \
    T_impl_ArrQue$(_T); \
    union thrd_Que$(_T) { \
        T_embed$(struct { \
            var_(que, ArrQue$(_T)); \
            var_(closed, bool); \
            var_(lock, thrd_Mtx); \
            var_(send_waiters, thrd_wait_List); \
            var_(recv_waiters, thrd_wait_List); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, thrd_Que) $like_ref; \
    }; \
    T_impl_E$($set(thrd_chan_E)(_T)); \
    T_impl_E$($set(thrd_chan_WaitE)(_T)); \
    T_impl_E$($set(thrd_chan_TimedE)(_T))
#define __gen__T_use_thrd_Que$(_T...) \
    T_decl_thrd_Que$(_T); \
    T_impl_thrd_Que$(_T)

#define __gen__T_use_thrd_Que_init$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Que_init, _T)(S$(_T) buf))(thrd_Que$(_T))) { \
        return type$((thrd_Que$(_T))(thrd_Que_init(u_anyS(buf)))); \
    }
#define __gen__T_use_thrd_Que_fini$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Que_fini, _T)(P$$(thrd_Que$(_T)) self))(void)) { \
        return thrd_Que_fini(self->as_raw); \
    }
#define __gen__T_use_thrd_Que_len$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Que_len, _T)(P$$(thrd_Que$(_T)) self))(usize)) { \
        return thrd_Que_len(self->as_raw); \
    }
#define __gen__T_use_thrd_Que_cap$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Que_cap, _T)(P$$(thrd_Que$(_T)) self))(usize)) { \
        return thrd_Que_cap(self->as_raw); \
    }
#define __gen__T_use_thrd_Que_isFull$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Que_isFull, _T)(P$$(thrd_Que$(_T)) self))(bool)) { \
        return thrd_Que_isFull(self->as_raw); \
    }
#define __gen__T_use_thrd_Que_isEmpty$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Que_isEmpty, _T)(P$$(thrd_Que$(_T)) self))(bool)) { \
        return thrd_Que_isEmpty(self->as_raw); \
    }
#define __gen__T_use_thrd_Que_isOpen$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Que_isOpen, _T)(P$$(thrd_Que$(_T)) self))(bool)) { \
        return thrd_Que_isOpen(self->as_raw); \
    }
#define __gen__T_use_thrd_Que_close$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Que_close, _T)(P$$(thrd_Que$(_T)) self))(void)) { \
        return thrd_Que_close(self->as_raw); \
    }
#define __gen__T_use_thrd_Que_isClosed$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Que_isClosed, _T)(P$$(thrd_Que$(_T)) self))(bool)) { \
        return thrd_Que_isClosed(self->as_raw); \
    }
#define __gen__T_use_thrd_Que_trySend$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Que_trySend, _T)(P$$(thrd_Que$(_T)) self, _T item))(thrd_chan_E$void)) { \
        return thrd_Que_trySend(self->as_raw, u_anyV(item)); \
    }
#define __gen__T_use_thrd_Que_send$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Que_send, _T)(P$$(thrd_Que$(_T)) self, _T item, thrd_wait_Src cancel_src))(thrd_chan_WaitE$void)) { \
        return thrd_Que_send(self->as_raw, u_anyV(item), cancel_src); \
    }
#define __gen__T_use_thrd_Que_sendFor$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Que_sendFor, _T)(P$$(thrd_Que$(_T)) self, _T item, thrd_wait_Src cancel_src, time_Dur dur))(thrd_chan_TimedE$void)) { \
        return thrd_Que_sendFor(self->as_raw, u_anyV(item), cancel_src, dur); \
    }
#define __gen__T_use_thrd_Que_sendOp$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Que_sendOp, _T)(P$$(thrd_Que$(_T)) self, _T* item))(thrd_Que_SendOp)) { \
        return thrd_Que_sendOp(self->as_raw, u_anyP(item)); \
    }
#define __gen__T_use_thrd_Que_tryRecv$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Que_tryRecv, _T)(P$$(thrd_Que$(_T)) self))(E$($set(thrd_chan_E)(_T))) $scope) { \
        return_(u_castE$((ReturnType)(thrd_Que_tryRecv(self->as_raw, u_retV$(_T))))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_Que_recv$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Que_recv, _T)(P$$(thrd_Que$(_T)) self, thrd_wait_Src cancel_src))(E$($set(thrd_chan_WaitE)(_T))) $scope) { \
        return_(u_castE$((ReturnType)(thrd_Que_recv(self->as_raw, cancel_src, u_retV$(_T))))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_Que_recvFor$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Que_recvFor, _T)(P$$(thrd_Que$(_T)) self, thrd_wait_Src cancel_src, time_Dur dur))(E$($set(thrd_chan_TimedE)(_T))) $scope) { \
        return_(u_castE$((ReturnType)(thrd_Que_recvFor(self->as_raw, cancel_src, dur, u_retV$(_T))))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_Que_recvOp$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Que_recvOp, _T)(P$$(thrd_Que$(_T)) self))(thrd_Que_RecvOp)) { \
        return thrd_Que_recvOp(self->as_raw, typeInfo$(_T)); \
    }
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Que__included */
