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
 *          suspended_data field enables communication with external runners.
 */
#ifndef Co_included
#define Co_included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Co/Fiber.h"

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
/*--- Coroutine's Field ---*/
#define Co_Ret_(_rtn...) __alias__Co_Ret_(_rtn)
// #define co_use_Co_Ret_(_rtn...) __stmt__co_use_Co_Ret_(_rtn)
#define Co_Args_(_rtn...) __alias__Co_Args_(_rtn)
// #define co_use_Co_Args_(_rtn...) __stmt__co_use_Co_Args_(_rtn)
#define Co_Suspended_(_rtn...) __alias__Co_Suspended_(_rtn)
// #define co_use_Co_Suspended_(_rtn...) __stmt__co_use_Co_Suspended_(_rtn)
#define Co_Locals_(_rtn...) __alias__Co_Locals_(_rtn)
// #define co_use_Co_Locals_(_rtn...) __stmt__co_use_Co_Locals_(_rtn)
#define Co_LocalsMut_(_rtn...) __alias__Co_LocalsMut_(_rtn)
// #define co_use_Co_LocalsMut_(_rtn...) __stmt__co_use_Co_LocalsMut_(_rtn)
#define Co_Data_(_rtn...) __alias__Co_Data_(_rtn)
// #define co_use_Co_Data_(_rtn...) __stmt__co_use_Co_Data_(_rtn)
#define Co_Ctx_(_rtn...) __alias__Co_Ctx_(_rtn)
// #define co_use_Co_Ctx_(_rtn...) __stmt__co_use_Co_Ctx_(_rtn)
#define Co_CtxBase_(_rtn...) __alias__Co_CtxBase_(_rtn)
// #define co_use_Co_CtxBase_(_rtn...) __stmt__co_use_Co_CtxBase_(_rtn)
#define Co_Rtn_(_rtn...) __alias__Co_Rtn_(_rtn)
// #define co_use_Co_Rtn_(_rtn...) __stmt__co_use_Co_Rtn_(_rtn)
#define Co_rtn_(_rtn...) __alias__Co_rtn_(_rtn)
// #define co_use_Co_rtn_(_rtn...) __stmt__co_use_Co_rtn_(_rtn)
#define Co_Frame_(_rtn...) __alias__Co_Frame_(_rtn)
// #define co_use_Co_Frame_(_rtn...) __stmt__co_use_Co_Frame_(_rtn)
#define Co_FrameBase_(_rtn...) __alias__Co_FrameBase_(_rtn)
// #define co_use_Co_FrameBase_(_rtn...) __stmt__co_use_Co_FrameBase_(_rtn)
/*--- Coroutine ---*/
#define co_fn_(_fnName, _Args, _Ret_Type...) \
    __stmt__co_fn_(_fnName, _Args, _Ret_Type)
#define co_fn_scope(_fnName, _Locals, _LocalsMut, _Suspended...) \
    __stmt__co_fn_scope(_fnName, _Locals, _LocalsMut, _Suspended)
#define $unscoped_co_fn \
    __stmt__$unscoped_co_fn()
#define co_locals_mut_(...) \
    __stmt__$co_locals_mut(__VA_ARGS__)
#define co_locals_(...) \
    __stmt__$co_locals(__VA_ARGS__)
#define co_suspended_(...) \
    __stmt__$co_suspended(__VA_ARGS__)
#define $co_frame(/*void) -> (P(Co_Frame)*/) __expr__$co_frame()
#define $co_arg(_ident...) __expr__$co_arg(_ident)
#define $co(_ident...) __expr__$co(_ident)
#define $co_mut(_ident...) __expr__$co_mut(_ident)
#define co_let_(_ident, _T...) __stmt__co_let_(_ident, _T)
#define co_var_(_ident, _T...) __stmt__co_var_(_ident, _T)
#define suspend_(/*(_ident)(_expr...)*/... /*-> (void)*/) __stmt__suspend_(__VA_ARGS__)
#define resume_(_p_frame /*: P(Co_Frame)*/... /*-> (P(Co_Frame))*/) __expr__resume_(_p_frame)
#define co_return_(_expr...) __expr__co_return_(_expr)
#if UNUSED_CODE
#define co_returned(...) __expr__co_returned(__VA_ARGS__)
#define co_call_(...) __expr__co_call_(__VA_ARGS__)
#define co_called(...) __stmt__co_called(__VA_ARGS__)
#endif /* UNUSED_CODE */

/*========== Macros and Definitions =========================================*/

/*--- Coroutine's Context ---*/
#define __alias__Co_Ctx$(_T...) tpl$(Co_Ctx, _T)
#define __stmt__T_use_Co_Ctx$(_T...) \
    T_decl_Co_Ctx$(_T); \
    T_impl_Co_Ctx$(_T)
#define __stmt__T_decl_Co_Ctx$(_T...) \
    T_alias$((Co_Ctx$(_T))(union Co_Ctx$(_T)))
#define __stmt__T_impl_Co_Ctx$(_T...) \
    union Co_Ctx$(_T) { \
        T_embed$(struct { \
            var_(ctrl, Co_Ctrl); \
            var_(suspended_data, P$raw); \
            T_embed$(union { \
                var_(ret, _T); \
                var_(ret_, _T) $like_ref; \
            }); \
            var_(args_, V$raw) $flexible; \
            var_(data_, V$raw) $flexible; \
        }); \
        var_(as_raw, Co_Ctx$raw) $flexible; \
    }
/*--- Coroutine's Routine ---*/
#define __alias__Co_Rtn$(_T...) tpl$(Co_Rtn, _T)
#define __stmt__T_use_Co_Rtn$(_T...) \
    T_alias$((Co_Rtn$(_T))(fn_(((*)(P$$(Co_Ctx$(_T)) ctx))(P$$(Co_Ctx$(_T)))$T)))
/*--- Coroutine's Frame ---*/
#define __alias__Co_Frame$(_T...) tpl$(Co_Frame, _T)
#define __stmt__T_use_Co_Frame$(_T...) \
    T_decl_Co_Frame$(_T); \
    T_impl_Co_Frame$(_T)
#define __stmt__T_decl_Co_Frame$(_T...) \
    T_alias$((Co_Frame$(_T))(union Co_Frame$(_T)))
#define __stmt__T_impl_Co_Frame$(_T...) \
    union Co_Frame$(_T) { \
        T_embed$(struct { \
            var_(rtn, Co_Rtn$(_T)); \
            T_embed$(union { \
                var_(ctx, Co_Ctx$(_T)); \
                var_(ctx_, Co_Ctx$(_T)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Co_Frame$raw) $flexible; \
    }
/*--- Coroutine's Field ---*/
#define __alias__Co_Ret_(_rtn...) tpl_(Co_Ret, _rtn)
#define __alias__Co_Args_(_rtn...) tpl_(Co_Args, _rtn)
#define __alias__Co_Suspended_(_rtn...) tpl_(Co_Suspended, _rtn)
#define __alias__Co_Locals_(_rtn...) tpl_(Co_Locals, _rtn)
#define __alias__Co_LocalsMut_(_rtn...) tpl_(Co_Locals_mut, _rtn)
#define __alias__Co_Data_(_rtn...) tpl_(Co_Data, _rtn)
#define __alias__Co_Ctx_(_rtn...) tpl_(Co_Ctx, _rtn)
#define __alias__Co_CtxBase_(_rtn...) tpl_(Co_CtxBase, _rtn)
#define __alias__Co_Rtn_(_rtn...) tpl_(Co_Rtn, _rtn)
#define __alias__Co_rtn_(_rtn...) tpl_(Co_rtn, _rtn)
#define __alias__Co_Frame_(_rtn...) tpl_(Co_Frame, _rtn)
#define __alias__Co_FrameBase_(_rtn...) tpl_(Co_FrameBase, _rtn)
/*--- Coroutine ---*/
#define __stmt__co_fn_(_fnName, Args, Ret_T...) \
    T_alias$((Co_FrameBase_(_fnName))(Co_Frame$(Ret_T))); \
    T_alias$((Co_Frame_(_fnName))(union Co_Frame_(_fnName))); \
    T_alias$((Co_CtxBase_(_fnName))(Co_Ctx$(Ret_T))); \
    T_alias$((Co_Ctx_(_fnName))(union Co_Ctx_(_fnName))); \
    T_alias$((Co_Rtn_(_fnName))(fn_(((*)(P$$(Co_Ctx_(_fnName)) ctx))(P$$(Co_Ctx_(_fnName)))$T))); \
    T_alias$((Co_Ret_(_fnName))(Ret_T)); \
    T_alias$((Co_Args_(_fnName))(struct Co_Args_(_fnName) { \
        pp_Tuple_unwrap Args; \
    })); \
    $static fn_((_fnName(P$$(Co_Ctx_(_fnName)) ctx))(P$$(Co_Ctx_(_fnName))))
#define __stmt__co_fn_scope(_fnName, _Locals, _LocalsMut, _Suspended...) \
    T_alias$((Co_Suspended_(_fnName))(union Co_Suspended_(_fnName) _Suspended)); \
    T_alias$((Co_Locals_(_fnName))(struct Co_Locals_(_fnName) _Locals)); \
    T_alias$((Co_LocalsMut_(_fnName))(struct Co_LocalsMut_(_fnName) _LocalsMut)); \
    T_alias$((Co_Data_(_fnName))(struct Co_Data_(_fnName) { \
        var_(suspended, Co_Suspended_(_fnName)); \
        var_(locals, Co_Locals_(_fnName)); \
        var_(locals_mut, Co_LocalsMut_(_fnName)); \
    })); \
    union Co_Ctx_(_fnName) { \
        T_embed$(struct { \
            var_(ctrl, Co_Ctrl); \
            var_(suspended_data, P$raw); \
            T_embed$(union { \
                var_(ret, Co_Ret_(_fnName)); \
                var_(ret_, Co_Ret_(_fnName)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(args, Co_Args_(_fnName)); \
                var_(args_, Co_Args_(_fnName)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(data, Co_Data_(_fnName)); \
                var_(data_, Co_Data_(_fnName)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Co_Ctx$raw) $flexible; \
        var_(as_base, Co_CtxBase_(_fnName)) $flexible; \
    }; \
    union Co_Frame_(_fnName) { \
        T_embed$(struct { \
            var_(rtn, Co_Rtn_(_fnName)); \
            T_embed$(union { \
                var_(ctx, Co_Ctx_(_fnName)); \
                var_(ctx_, Co_Ctx_(_fnName)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Co_Frame$raw) $flexible; \
        var_(as_base, Co_FrameBase_(_fnName)) $flexible; \
    }; \
    fn_((_fnName($P$(Co_Ctx_(_fnName)) ctx))($P$(Co_Ctx_(_fnName)))) { /* clang-format off */ \
        let __ctx = ctx; \
        $attr($maybe_unused) \
        let __frame = recordPtr(__ctx, Co_FrameBase_(_fnName), ctx); \
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
        var_(__scope_counter, struct fn__ScopeCounter) = { \
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
#define inline__$unscoped_co_fn() __stmt__$unscoped_co_fn
#define __stmt__$unscoped_co_fn \
        break; \
    } \
    if (false) { \
__step_suspend: \
__step_unscope: \
        return __ctx; \
    } \
}
/* clang-format on */
#define __stmt__$co_locals_mut(...) __VA_ARGS__
#define __stmt__$co_locals(...) __VA_ARGS__
#define __stmt__$co_suspended(...) __VA_ARGS__

#define __expr__$co_frame() (*__frame)
#define __expr__$co_arg(_ident...) (__args->_ident)
#define __expr__$co(_ident...) (__locals->_ident)
#define __expr__$co_mut(_ident...) (__locals_mut->_ident)
#define __stmt__co_let_(_ident, /*_T*/...) \
    __VA_OPT__(({ claim_assert_static(eqlType$(TypeOf(__ctx->data.locals._ident), __VA_ARGS__)); }), ) \
    __ctx->data.locals._ident
#define __stmt__co_var_(_ident, /*_T*/...) \
    __VA_OPT__(({ claim_assert_static(eqlType$(TypeOf(__locals_mut->_ident), __VA_ARGS__)); }), ) \
    __locals_mut->_ident

#define __stmt__suspend_(...) __step__suspend___emit(__step__suspend___parse __VA_ARGS__)
#define __step__suspend___parse(_ident...) pp_uniqTok(suspended_data), _ident, /*_expr*/
#define __step__suspend___emit(...) __inline__suspend_(__VA_ARGS__)
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
    claim_assert(__p_frame->ctx_->ctrl.state != Co_State_undefined); \
    local_return_(__p_frame->rtn(__p_frame->ctx_)->suspended_data); \
})

#define __expr__co_return_(_expr...) return_(_expr)
#define __expr__co_returned(...)
#define __expr__co_call_(_ctx_co, _fnCo_and_Args...)
#define __stmt__co_called(...)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Co_included */
