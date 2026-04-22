/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Co.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-28 (date of creation)
 * @updated 2026-04-22 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  Co
 *
 * @brief   Stackless coroutine primitives
 * @details Duff's device based state machine coroutines.
 *          Pure scheduling-agnostic primitives — no knowledge of Io.
 *          suspend_data field enables communication with external runners.
 */
#ifndef Co__included
#define Co__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Definitions =========================================*/

/*--- Coroutine's Control ---*/
T_alias$((Co_Count)(u32));
T_alias$((Co_State)(enum_((Co_State $fits($packed))(
    Co_State_undefined = 0,
    Co_State_pending,
    Co_State_suspended,
    Co_State_ready
))));
claim_assert_static(eqlType$(Co_State, u8));
T_alias$((Co_CtrlPacked)(Co_Count));
T_alias$((Co_Ctrl)(union Co_Ctrl {
    T_embed$(struct {
        var_(state, Co_CtrlPacked) : 2;
        var_(count, Co_CtrlPacked) : 30;
    });
    var_(packed, Co_CtrlPacked);
}));
claim_assert_static(eqlType$(Co_State, u8));
/*--- Coroutine's Context ---*/
#define Co_Ctx$(_T...) __alias__Co_Ctx$(_T)
T_alias$((Co_Ctx$raw)(struct Co_Ctx$raw {
    var_(ctrl, Co_Ctrl);
    var_(suspended_data, P$raw);
    var_(ret_, V$raw) $flexible;
    var_(args_, V$raw) $flexible;
    var_(data_, V$raw) $flexible;
}));
#define T_use_Co_Ctx$(_T...) __stmt__T_use_Co_Ctx$(_T)
#define T_decl_Co_Ctx$(_T...) __stmt__T_decl_Co_Ctx$(_T)
#define T_impl_Co_Ctx$(_T...) __stmt__T_impl_Co_Ctx$(_T)
/*--- Coroutine's Routine ---*/
#define Co_Rtn$(_T...) __alias__Co_Rtn$(_T)
T_alias$((Co_Rtn)(fn_(((*)(P$$(Co_Ctx$raw) ctx))(P$$(Co_Ctx$raw)) $T)));
#define T_use_Co_Rtn$(_T...) __stmt__T_use_Co_Rtn$(_T)
/*--- Coroutine's Frame ---*/
#define Co_Frame$(_T...) __alias__Co_Frame$(_T)
T_alias$((Co_Frame$raw)(struct Co_Frame$raw {
    var_(rtn, Co_Rtn);
    var_(ctx_, Co_Ctx$raw) $flexible;
}));
#define T_use_Co_Frame$(_T...) __stmt__T_use_Co_Frame$(_T)
#define T_decl_Co_Frame$(_T...) __stmt__T_decl_Co_Frame$(_T)
#define T_impl_Co_Frame$(_T...) __stmt__T_impl_Co_Frame$(_T)
/*--- Coroutine ---*/
#define co_fn_(_fnName, _Args, _Ret_Type...) \
    __stmt__co_fn_(_fnName, _Args, _Ret_Type)
#define co_fn_scope(_fnName, _Locals, _LocalsMut, _Suspended...) \
    __stmt__co_fn_scope(_fnName, _Locals, _LocalsMut, _Suspended)
#define $co_frame() __expr__$co_frame()
#define $co_arg(_ident...) __expr__$co_arg(_ident)
#define $co(_ident...) __expr__$co(_ident)
#define $co_mut(_ident...) __expr__$co_mut(_ident)
#define co_let_(_ident, _expr...) __expr__co_let_(_ident, _expr)
#define co_var_(_ident, _expr...) __expr__co_var_(_ident, _expr)
#define suspend_(_ident, _expr...) __stmt__suspend_(_ident, _expr)
#define resume_(_p_frame...) __expr__resume_(_p_frame)
#define co_return_(_expr...) __expr__co_return_(_expr)
#if UNUSED_CODE
#define co_returned(...) __expr__co_returned(__VA_ARGS__)
#define co_call_(...) __expr__co_call_(__VA_ARGS__)
#define co_called(...) __stmt__co_called(__VA_ARGS__)
#endif /* UNUSED_CODE */

/*========== Macros and Definitions =========================================*/

#define Co__Ret_(_rtn...) __alias__Co__Ret_(_rtn)
#define co_use_Co__Ret_(_rtn...) __stmt__co_use_Co__Ret_(_rtn)
#define Co__Args_(_rtn...) __alias__Co__Args_(_rtn)
#define co_use_Co__Args_(_rtn...) __stmt__co_use_Co__Args_(_rtn)
#define Co__Suspended_(_rtn...) __alias__Co__Suspended_(_rtn)
#define co_use_Co__Suspended_(_rtn...) __stmt__co_use_Co__Suspended_(_rtn)
#define Co__Locals_(_rtn...) __alias__Co__Locals_(_rtn)
#define co_use_Co__Locals_(_rtn...) __stmt__co_use_Co__Locals_(_rtn)
#define Co__LocalsMut_(_rtn...) __alias__Co__LocalsMut_(_rtn)
#define co_use_Co__LocalsMut_(_rtn...) __stmt__co_use_Co__LocalsMut_(_rtn)
#define Co__Data_(_rtn...) __alias__Co__Data_(_rtn)
#define co_use_Co__Data_(_rtn...) __stmt__co_use_Co__Data_(_rtn)
#define Co__Ctx_(_rtn...) __alias__Co__Ctx_(_rtn)
#define co_use_Co__Ctx_(_rtn...) __stmt__co_use_Co__Ctx_(_rtn)
#define Co__CtxBase_(_rtn...) __alias__Co__CtxBase_(_rtn)
#define co_use_Co__CtxBase_(_rtn...) __stmt__co_use_Co__CtxBase_(_rtn)
#define Co__Rtn_(_rtn...) __alias__Co__Rtn_(_rtn)
#define co_use_Co__Rtn_(_rtn...) __stmt__co_use_Co__Rtn_(_rtn)
#define Co__rtn_(_rtn...) __alias__Co__rtn_(_rtn)
#define co_use_Co__rtn_(_rtn...) __stmt__co_use_Co__rtn_(_rtn)
#define Co__Frame_(_rtn...) __alias__Co__Frame_(_rtn)
#define co_use_Co__Frame_(_rtn...) __stmt__co_use_Co__Frame_(_rtn)
#define Co__FrameBase_(_rtn...) __alias__Co__FrameBase_(_rtn)
#define co_use_Co__FrameBase_(_rtn...) __stmt__co_use_Co__FrameBase_(_rtn)

#define __alias__Co__Ret_(_rtn...) pp_join(_, Co__Ret, _rtn)
#define __alias__Co__Args_(_rtn...) pp_join(_, Co__Args, _rtn)
#define __alias__Co__Suspended_(_rtn...) pp_join(_, Co__Suspended, _rtn)
#define __alias__Co__Locals_(_rtn...) pp_join(_, Co__Locals, _rtn)
#define __alias__Co__LocalsMut_(_rtn...) pp_join(_, Co__Locals_mut, _rtn)
#define __alias__Co__Data_(_rtn...) pp_join(_, Co__Data, _rtn)
#define __alias__Co__Ctx_(_rtn...) pp_join(_, Co__Ctx, _rtn)
#define __alias__Co__CtxBase_(_rtn...) pp_join(_, Co__CtxBase, _rtn)
#define __alias__Co__Rtn_(_rtn...) pp_join(_, Co__Rtn, _rtn)
#define __alias__Co__rtn_(_rtn...) pp_join(_, Co__rtn, _rtn)
#define __alias__Co__Frame_(_rtn...) pp_join(_, Co__Frame, _rtn)
#define __alias__Co__FrameBase_(_rtn...) pp_join(_, Co__FrameBase, _rtn)

#define __stmt__co_fn_(_fnName, Args, T_Return...) \
    T_alias$((Co__FrameBase_(_fnName))(Co_Frame$(T_Return))); \
    T_alias$((Co__Frame_(_fnName))(union Co__Frame_(_fnName))); \
    T_alias$((Co__CtxBase_(_fnName))(Co_Ctx$(T_Return))); \
    T_alias$((Co__Ctx_(_fnName))(union Co__Ctx_(_fnName))); \
    T_alias$((Co__Rtn_(_fnName))(fn_(((*)(P$$(Co__Ctx_(_fnName)) ctx))(P$$(Co__Ctx_(_fnName)) $T)))); \
    T_alias$((Co__Ret_(_fnName))(T_Return)); \
    T_alias$((Co__Args_(_fnName))(struct Co__Args_(_fnName) { \
        pp_Tuple_unwrap Args; \
    })); \
    fn_((_fnName(P$$(Co__Ctx_(_fnName)) ctx))($P$$(Co__Ctx_(_fnName))))
#define __stmt__co_fn_scope(_fnName, _Locals, _LocalsMut, _Suspended...) \
    T_alias$((Co__Suspended_(_fnName))(union Co__Suspended_(_fnName) { \
        pp_Tuple_unwrap _Suspended; \
    })); \
    T_alias$((Co__Locals_(_fnName))(struct Co__Locals_(_fnName) { \
        pp_Tuple_unwrap _Locals; \
    })); \
    T_alias$((Co__LocalsMut_(_fnName))(struct Co__LocalsMut_(_fnName) { \
        pp_Tuple_unwrap _LocalsMut; \
    })); \
    T_alias$((Co__Data_(_fnName))(struct Co__Data_(_fnName) { \
        var_(suspended, Co__Suspended_(_fnName)); \
        var_(locals, Co__Locals_(_fnName)); \
        var_(locals_mut, Co__LocalsMut_(_fnName)); \
    })); \
    union Co__Ctx_(_fnName) { \
        T_embed$(struct { \
            var_(ctrl, Co_Ctrl); \
            var_(suspended_data, P$raw); \
            T_embed$(union { \
                var_(ret, Co__Ret_(_fnName)); \
                var_(ret_, Co__Ret_(_fnName)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(args, Co__Args_(_fnName)); \
                var_(args_, Co__Args_(_fnName)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(data, Co__Data_(_fnName)); \
                var_(data_, Co__Data_(_fnName)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Co_Ctx$raw) $flexible; \
        var_(as_base, Co__CtxBase_(_fnName)) $flexible; \
    }; \
    union Co__Frame_(_fnName) { \
        T_embed$(struct { \
            var_(rtn, Co__Rtn_(_fnName)); \
            T_embed$(union { \
                var_(ctx, Co__Ctx_(_fnName)); \
                var_(ctx_, Co__Ctx_(_fnName)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Co_Frame$raw) $flexible; \
        var_(as_base, Co__FrameBase_(_fnName)) $flexible; \
    }; \
    fn_((_fnName($P$(Co__Ctx_(_fnName)) ctx))($P$(Co__Ctx_(_fnName)))) { /* clang-format off */ \
        let __ctx = _ctx; \
        $attr($maybe_unused) \
        let __frame = recordPtr(__ctx, Co__FrameBase_(_fnName), ctx); \
        let __reserved_return = &__ctx->ret; \
        $maybe_unused typedef TypeOf(*__reserved_return) ReturnType; \
        $maybe_unused typedef ReturnType ReturnT; \
        $attr($maybe_unused) \
        let __args = &__ctx->args; \
        $attr($maybe_unused) \
        let __suspended = &__ctx->data.suspended; \
        $attr($maybe_unused) \
        let_const __locals = &__ctx->data.locals; \
        $attr($maybe_unused) \
        let __locals_mut = &__ctx->data.locals_mut; \
        var_(__scope_counter, fn__ScopeCounter) = { \
            .is_returning = __ctx->ctrl.state == Co_State_ready, \
            .current_line = __ctx->ctrl.count \
        }; \
        if (false) { __step_return: \
            __scope_counter.is_returning = true; \
            __ctx->ctrl.state = Co_State_ready; \
            goto __step_unscope; \
        } \
        switch (__scope_counter.current_line) { \
        default: { \
            goto __step_unscope; \
        } break; \
        case 0: \
            __scope_counter.current_line--;
/* clang-format on */
/* clang-format off */
#define inline__$unscoped_co_fn() $unscoped_co_fn
#define $unscoped_co_fn \
        break; \
    } \
    if (false) { \
__step_suspend: \
__step_unscope: \
        return __ctx; \
    } \
}
/* clang-format on */

/*========== Return / Suspend / Resume ======================================*/

#define __expr__$co_frame() (*__frame)
#define __expr__$co_arg(_ident...) (__args->_ident)
#define __expr__$co(_ident...) (__locals->_ident)
#define __expr__$co_mut(_ident...) (__locals_mut->_ident)
#define __expr__co_let_(_ident, _expr...) __ctx->data.locals._ident = _expr
#define __expr__co_var_(_ident, _expr...) __locals_mut->_ident = _expr

#define __stmt__suspend_(_ident, _expr...) __inline__suspend_(pp_uniqTok(suspended_data), _ident, _expr)
#define __inline__suspend_(__suspended_data, _ident, _expr...) \
    do { \
        let __suspended_data = &__suspended->_ident; \
        *__suspended_data = _expr; \
        __ctx->suspended_data = __suspended_data; \
        __ctx->ctrl.state = Co_State_suspended; \
        __ctx->ctrl.count = __LINE__; \
        goto __step_suspend; \
    case __LINE__:; \
        claim_assert(__ctx->ctrl.state == Co_State_suspended); \
        __ctx->ctrl.state = Co_State_pending; \
    } while (false)

#define __expr__resume_(_p_frame...) __inline__resume_(pp_uniqTok(p_frame), _p_frame)
#define __inline__resume_(__p_frame, _p_frame...) local_({ \
    let __p_frame = ensureNonnull(_p_frame); \
    claim_assert(__p_frame->ctx_->ctrl.state != Co_State_none); \
    local_return_(__p_frame->rtn(__p_frame->ctx_)->suspend_data); \
})

#define __expr__co_return_(_expr...) return_(_expr)
#define __expr__co_returned(...)
#define __expr__co_call_(_ctx_co, _fnCo_and_Args...)
#define __stmt__co_called(...)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Co__included */
