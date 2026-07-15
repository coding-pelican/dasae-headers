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
#ifndef prl_Co__included
#define prl_Co__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Definitions =========================================*/

/*--- Coroutine's Control ---*/
T_alias$((Co_State)(enum_((Co_State $fits($packed))(
    Co_State_undefined = 0,
    Co_State_pending,
    Co_State_suspended,
    Co_State_ready
))));
claim_assert_static(eqlType$(Co_State, u8));
T_alias$((Co_FlowCtrlPacked)(fn__FlowCursorPacked));
#define Co_FlowCtrl_State_bits 2
#define Co_FlowCtrl_line_bits ((arch_bits_wide / 2) - Co_FlowCtrl_State_bits)
T_alias$((Co_FlowCtrl)(union Co_FlowCtrl {
    T_embed$(struct {
        var_(line, Co_FlowCtrlPacked)
            : Co_FlowCtrl_line_bits;
        var_(state, Co_FlowCtrlPacked)
            : Co_FlowCtrl_State_bits;
    });
    var_(packed, Co_FlowCtrlPacked);
}));
claim_assert_static((Co_FlowCtrl_line_bits + Co_FlowCtrl_State_bits) == (arch_bits_wide / 2));
/*--- Coroutine's Context ---*/
#define Co_Ctx$(_$T...) __alias__Co_Ctx$(_$T)
T_alias$((Co_Ctx$raw)(struct Co_Ctx$raw {
    var_(ctrl, Co_FlowCtrl);
    var_(suspended_data, P$raw);
    var_(ret_, V$raw) $flexible;
    var_(args_, V$raw) $flexible;
    var_(data_, V$raw) $flexible;
}));
#define T_use_Co_Ctx$(_$T...) __stmt__T_use_Co_Ctx$(_$T)
#define T_decl_Co_Ctx$(_$T...) __stmt__T_decl_Co_Ctx$(_$T)
#define T_impl_Co_Ctx$(_$T...) __stmt__T_impl_Co_Ctx$(_$T)
/*--- Coroutine's Routine ---*/
#define Co_Rtn$(_$T...) __alias__Co_Rtn$(_$T)
T_alias$((Co_Rtn)(fn_(((*)(P$$(Co_Ctx$raw) ctx))(P$$(Co_Ctx$raw)) $T)));
#define T_use_Co_Rtn$(_$T...) __stmt__T_use_Co_Rtn$(_$T)
/*--- Coroutine's Frame ---*/
#define Co_Frame$(_$T...) __alias__Co_Frame$(_$T)
T_alias$((Co_Frame$raw)(struct Co_Frame$raw {
    var_(rtn, Co_Rtn);
    var_(ctx_, Co_Ctx$raw) $flexible;
}));
#define T_use_Co_Frame$(_$T...) __stmt__T_use_Co_Frame$(_$T)
#define T_decl_Co_Frame$(_$T...) __stmt__T_decl_Co_Frame$(_$T)
#define T_impl_Co_Frame$(_$T...) __stmt__T_impl_Co_Frame$(_$T)
/*--- Coroutine's Field ---*/
#define Co_Ret_(_$rtn...) __alias__Co_Ret_(_$rtn)
// #define co_use_Co_Ret_(_$rtn...) __stmt__co_use_Co_Ret_(_$rtn)
#define Co_Args_(_$rtn...) __alias__Co_Args_(_$rtn)
// #define co_use_Co_Args_(_$rtn...) __stmt__co_use_Co_Args_(_$rtn)
#define Co_Suspended_(_$rtn...) __alias__Co_Suspended_(_$rtn)
// #define co_use_Co_Suspended_(_$rtn...) __stmt__co_use_Co_Suspended_(_$rtn)
#define Co_Locals_(_$rtn...) __alias__Co_Locals_(_$rtn)
// #define co_use_Co_Locals_(_$rtn...) __stmt__co_use_Co_Locals_(_$rtn)
#define Co_LocalsMut_(_$rtn...) __alias__Co_LocalsMut_(_$rtn)
// #define co_use_Co_LocalsMut_(_$rtn...) __stmt__co_use_Co_LocalsMut_(_$rtn)
#define Co_Data_(_$rtn...) __alias__Co_Data_(_$rtn)
// #define co_use_Co_Data_(_$rtn...) __stmt__co_use_Co_Data_(_$rtn)
#define Co_Ctx_(_$rtn...) __alias__Co_Ctx_(_$rtn)
// #define co_use_Co_Ctx_(_$rtn...) __stmt__co_use_Co_Ctx_(_$rtn)
#define Co_CtxBase_(_$rtn...) __alias__Co_CtxBase_(_$rtn)
// #define co_use_Co_CtxBase_(_$rtn...) __stmt__co_use_Co_CtxBase_(_$rtn)
#define Co_Rtn_(_$rtn...) __alias__Co_Rtn_(_$rtn)
// #define co_use_Co_Rtn_(_$rtn...) __stmt__co_use_Co_Rtn_(_$rtn)
#define Co_rtn_(_$rtn...) __alias__Co_rtn_(_$rtn)
// #define co_use_Co_rtn_(_$rtn...) __stmt__co_use_Co_rtn_(_$rtn)
#define Co_Frame_(_$rtn...) __alias__Co_Frame_(_$rtn)
// #define co_use_Co_Frame_(_$rtn...) __stmt__co_use_Co_Frame_(_$rtn)
#define Co_FrameBase_(_$rtn...) __alias__Co_FrameBase_(_$rtn)
// #define co_use_Co_FrameBase_(_$rtn...) __stmt__co_use_Co_FrameBase_(_$rtn)
/*--- Coroutine ---*/
#define co_fn_(_$fnName, _$Args, _$Ret_Type...) \
    __stmt__co_fn_(_$fnName, _$Args, _$Ret_Type)
#define co_fn_frame_scope(_$fnName, _$Locals, _$LocalsMut, _$Suspended...) \
    __stmt__co_fn_frame_scope(_$fnName, _$Locals, _$LocalsMut, _$Suspended)
#define co_fn_scope(_$fnName...) \
    __stmt__co_fn_scope(_$fnName)
#define $unscoped_co_fn \
    __stmt__$unscoped_co_fn()
#define co_fn_frame_guard(_$fnName, _$Locals, _$LocalsMut, _$Suspended, _$Deferrable...) \
    __stmt__co_fn_frame_guard(_$fnName, _$Locals, _$LocalsMut, _$Suspended, _$Deferrable)
#define co_fn_guard(_$fnName...) \
    __stmt__co_fn_guard(_$fnName)
#define $unguarded_co_fn \
    __stmt__$unguarded_co_fn()
#define co_locals_mut_(...) \
    __stmt__co_locals_mut_(__VA_ARGS__)
#define co_locals_(...) \
    __stmt__co_locals_(__VA_ARGS__)
#define co_suspended_(...) \
    __stmt__co_suspended_(__VA_ARGS__)
#define co_deferrable_(...) \
    __stmt__co_deferrable_(__VA_ARGS__)
#define $co_frame(/*void) -> (P(Co_Frame)*/) __expr__$co_frame()
#define $co_arg(_$ident...) __expr__$co_arg(_$ident)
#define $co(_$ident...) __expr__$co(_$ident)
#define $co_mut(_$ident...) __expr__$co_mut(_$ident)
#define $co_suspended(_$ident...) __expr__$co_suspended(_$ident)
#define co_let_(_$ident, _$T...) __stmt__co_let_(_$ident, _$T)
#define co_var_(_$ident, _$T...) __stmt__co_var_(_$ident, _$T)
#define $co_init(_$ident...) __expr__$co_init(_$ident)
#define $co_init_mut(_$ident...) __expr__$co_init_mut(_$ident)
#define suspend_(/*(_$ident)(_$expr...)*/... /*(void)*/) __stmt__suspend_(__VA_ARGS__)
#define resume_(_$p_frame /*: P(Co_Frame)*/... /*(P(Co_Frame))*/) __expr__resume_(_$p_frame)
#define co_return_(_$expr...) __expr__co_return_(_$expr)
#define co_defer_(_$expr...) __stmt__co_defer_(_$expr)
#define co_errdefer_(_$capt, _$expr...) __stmt__co_errdefer_(_$capt, _$expr)
#define co_blk_defer comp_syn__co_blk_defer
#define co_blk_deferral comp_syn__co_blk_deferral
#define co_loop_defer comp_syn__co_loop_defer
#define co_loop_deferral comp_syn__co_loop_deferral
#if UNUSED_CODE
#define co_returned(...) __expr__co_returned(__VA_ARGS__)
#define co_call_(...) __expr__co_call_(__VA_ARGS__)
#define co_called(...) __stmt__co_called(__VA_ARGS__)
#endif /* UNUSED_CODE */

/*========== Macros and Definitions =========================================*/

/*--- Coroutine's Context ---*/
#define __alias__Co_Ctx$(_$T...) tpl$(Co_Ctx, _$T)
#define __stmt__T_use_Co_Ctx$(_$T...) \
    T_decl_Co_Ctx$(_$T); \
    T_impl_Co_Ctx$(_$T)
#define __stmt__T_decl_Co_Ctx$(_$T...) \
    T_alias$((Co_Ctx$(_$T))(union Co_Ctx$(_$T)))
#define __stmt__T_impl_Co_Ctx$(_$T...) \
    union Co_Ctx$(_$T) { \
        T_embed$(struct { \
            var_(ctrl, Co_FlowCtrl); \
            var_(suspended_data, P$raw); \
            T_embed$(union { \
                var_(ret, _$T); \
                var_(ret_, _$T) $like_ref; \
            }); \
            var_(args_, V$raw) $flexible; \
            var_(data_, V$raw) $flexible; \
        }); \
        var_(as_raw, Co_Ctx$raw) $flexible; \
    }
/*--- Coroutine's Routine ---*/
#define __alias__Co_Rtn$(_$T...) tpl$(Co_Rtn, _$T)
#define __stmt__T_use_Co_Rtn$(_$T...) \
    T_alias$((Co_Rtn$(_$T))(fn_(((*)(P$$(Co_Ctx$(_$T)) ctx))(P$$(Co_Ctx$(_$T)))$T)))
/*--- Coroutine's Frame ---*/
#define __alias__Co_Frame$(_$T...) tpl$(Co_Frame, _$T)
#define __stmt__T_use_Co_Frame$(_$T...) \
    T_decl_Co_Frame$(_$T); \
    T_impl_Co_Frame$(_$T)
#define __stmt__T_decl_Co_Frame$(_$T...) \
    T_alias$((Co_Frame$(_$T))(union Co_Frame$(_$T)))
#define __stmt__T_impl_Co_Frame$(_$T...) \
    union Co_Frame$(_$T) { \
        T_embed$(struct { \
            var_(rtn, Co_Rtn$(_$T)); \
            T_embed$(union { \
                var_(ctx, Co_Ctx$(_$T)); \
                var_(ctx_, Co_Ctx$(_$T)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Co_Frame$raw) $flexible; \
    }
/*--- Coroutine's Field ---*/
#define __alias__Co_Ret_(_$rtn...) tpl_(Co_Ret, _$rtn)
#define __alias__Co_Args_(_$rtn...) tpl_(Co_Args, _$rtn)
#define __alias__Co_Suspended_(_$rtn...) tpl_(Co_Suspended, _$rtn)
#define __alias__Co_Locals_(_$rtn...) tpl_(Co_Locals, _$rtn)
#define __alias__Co_LocalsMut_(_$rtn...) tpl_(Co_Locals_mut, _$rtn)
#define __alias__Co_Data_(_$rtn...) tpl_(Co_Data, _$rtn)
#define __alias__Co_Ctx_(_$rtn...) tpl_(Co_Ctx, _$rtn)
#define __alias__Co_CtxBase_(_$rtn...) tpl_(Co_CtxBase, _$rtn)
#define __alias__Co_Rtn_(_$rtn...) tpl_(Co_Rtn, _$rtn)
#define __alias__Co_rtn_(_$rtn...) tpl_(Co_rtn, _$rtn)
#define __alias__Co_Frame_(_$rtn...) tpl_(Co_Frame, _$rtn)
#define __alias__Co_FrameBase_(_$rtn...) tpl_(Co_FrameBase, _$rtn)
/*--- Coroutine ---*/
#define __stmt__co_fn_(_$fnName, Args, Ret_T...) \
    fn_((_$fnName(P$$(union Co_Ctx_(_$fnName)) ctx))(P$$(union Co_Ctx_(_$fnName)))); \
    T_alias$((Co_FrameBase_(_$fnName))(Co_Frame$(Ret_T))); \
    T_alias$((Co_Frame_(_$fnName))(union Co_Frame_(_$fnName))); \
    T_alias$((Co_CtxBase_(_$fnName))(Co_Ctx$(Ret_T))); \
    T_alias$((Co_Ctx_(_$fnName))(union Co_Ctx_(_$fnName))); \
    T_alias$((Co_Rtn_(_$fnName))(fn_(((*)(P$$(Co_Ctx_(_$fnName)) ctx))(P$$(Co_Ctx_(_$fnName)))$T))); \
    T_alias$((Co_Ret_(_$fnName))(Ret_T)); \
    T_alias$((Co_Args_(_$fnName))(struct Co_Args_(_$fnName) { \
        pp_Tuple_unwrap Args; \
    }))
#define __stmt__co_fn_frame_scope(_$fnName, _$Locals, _$LocalsMut, _$Suspended...) \
    claim_assert_static(eqlType$( \
        TypeOf(_$fnName)*, \
        fn_(((*)(P$$(union Co_Ctx_(_$fnName)) ctx))(P$$(union Co_Ctx_(_$fnName)))$T) \
    )); \
    T_alias$((Co_Suspended_(_$fnName))(union Co_Suspended_(_$fnName) _$Suspended)); \
    T_alias$((Co_Locals_(_$fnName))(struct Co_Locals_(_$fnName) _$Locals)); \
    T_alias$((Co_LocalsMut_(_$fnName))(struct Co_LocalsMut_(_$fnName) _$LocalsMut)); \
    T_alias$((Co_Data_(_$fnName))(struct Co_Data_(_$fnName) { \
        var_(suspended, Co_Suspended_(_$fnName)); \
        var_(locals, Co_Locals_(_$fnName)); \
        var_(locals_mut, Co_LocalsMut_(_$fnName)); \
    })); \
    union Co_Ctx_(_$fnName) { \
        T_embed$(struct { \
            var_(ctrl, Co_FlowCtrl); \
            var_(suspended_data, P$raw); \
            T_embed$(union { \
                var_(ret, Co_Ret_(_$fnName)); \
                var_(ret_, Co_Ret_(_$fnName)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(args, Co_Args_(_$fnName)); \
                var_(args_, Co_Args_(_$fnName)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(data, Co_Data_(_$fnName)); \
                var_(data_, Co_Data_(_$fnName)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Co_Ctx$raw) $flexible; \
        var_(as_base, Co_CtxBase_(_$fnName)) $flexible; \
    }; \
    union Co_Frame_(_$fnName) { \
        T_embed$(struct { \
            var_(rtn, Co_Rtn_(_$fnName)); \
            T_embed$(union { \
                var_(ctx, Co_Ctx_(_$fnName)); \
                var_(ctx_, Co_Ctx_(_$fnName)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Co_Frame$raw) $flexible; \
        var_(as_base, Co_FrameBase_(_$fnName)) $flexible; \
    }
#define __stmt__co_fn_scope(_$fnName...) \
    fn_((_$fnName(P$$(union Co_Ctx_(_$fnName)) ctx))(P$$(union Co_Ctx_(_$fnName)))) { /* clang-format off */ \
        let __ctx = ctx; \
        $attr($maybe_unused) \
        let __frame = recordPtr(__ctx, Co_FrameBase_(_$fnName), ctx); \
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
        var_(__flow_cursor, struct fn__FlowCursor) = { \
            .is_returning = __ctx->ctrl.state == Co_State_ready, \
            .curr_line = __ctx->ctrl.line \
        }; \
        if (false) { __step_return: \
            __flow_cursor.is_returning = true; \
            __ctx->ctrl.state = Co_State_ready; \
            goto __step_unscope; \
        } \
        switch (__flow_cursor.curr_line) { \
        default: { goto __step_unscope; } break; \
        case 0: __flow_cursor.curr_line--; /* clang-format on */
#define inline__$unscoped_co_fn() __stmt__$unscoped_co_fn
#define __stmt__$unscoped_co_fn /* clang-format off */ \
        break; \
        } \
        if (false) goto __step_suspend; \
        if (false) { __step_suspend: __step_unscope: \
            return __ctx; \
        } \
    } /* clang-format on */
#define __stmt__co_fn_frame_guard(_$fnName, _$Locals, _$LocalsMut, _$Suspended, _$Deferrable...) \
    claim_assert_static(eqlType$( \
        TypeOf(_$fnName)*, \
        fn_(((*)(P$$(union Co_Ctx_(_$fnName)) ctx))(P$$(union Co_Ctx_(_$fnName)))$T) \
    )); \
    T_alias$((Co_Suspended_(_$fnName))(union Co_Suspended_(_$fnName) _$Suspended)); \
    T_alias$((Co_Locals_(_$fnName))(struct Co_Locals_(_$fnName) _$Locals)); \
    T_alias$((Co_LocalsMut_(_$fnName))(struct Co_LocalsMut_(_$fnName) _$LocalsMut)); \
    T_alias$((Co_Data_(_$fnName))(struct Co_Data_(_$fnName) { \
        var_(suspended, Co_Suspended_(_$fnName)); \
        var_(locals, Co_Locals_(_$fnName)); \
        var_(locals_mut, Co_LocalsMut_(_$fnName)); \
        var_(deferrable_top, u32); \
        var_(deferrable_stack, A$$(_$Deferrable, fn__FlowCursorPacked)); \
        var_(deferrable_break_stack, A$$(_$Deferrable, bool)); \
    })); \
    union Co_Ctx_(_$fnName) { \
        T_embed$(struct { \
            var_(ctrl, Co_FlowCtrl); \
            var_(suspended_data, P$raw); \
            T_embed$(union { \
                var_(ret, Co_Ret_(_$fnName)); \
                var_(ret_, Co_Ret_(_$fnName)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(args, Co_Args_(_$fnName)); \
                var_(args_, Co_Args_(_$fnName)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(data, Co_Data_(_$fnName)); \
                var_(data_, Co_Data_(_$fnName)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Co_Ctx$raw) $flexible; \
        var_(as_base, Co_CtxBase_(_$fnName)) $flexible; \
    }; \
    union Co_Frame_(_$fnName) { \
        T_embed$(struct { \
            var_(rtn, Co_Rtn_(_$fnName)); \
            T_embed$(union { \
                var_(ctx, Co_Ctx_(_$fnName)); \
                var_(ctx_, Co_Ctx_(_$fnName)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Co_Frame$raw) $flexible; \
        var_(as_base, Co_FrameBase_(_$fnName)) $flexible; \
    }
#define __stmt__co_fn_guard(_$fnName, _$Locals, _$LocalsMut, _$Suspended, _$Deferrable...) \
    fn_((_$fnName(P$$(union Co_Ctx_(_$fnName)) ctx))(P$$(union Co_Ctx_(_$fnName)))) { /* clang-format off */ \
        let __ctx = ctx; \
        $attr($maybe_unused) \
        let __frame = recordPtr(__ctx, Co_FrameBase_(_$fnName), ctx); \
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
        let __deferrable_top = &__ctx->data.deferrable_top; \
        let __deferrable_stack = A_ref(__ctx->data.deferrable_stack); \
        let __deferrable_break_stack = A_ref(__ctx->data.deferrable_break_stack); \
        var_(__flow_cursor, struct fn__FlowCursor) = { \
            .is_returning = __ctx->ctrl.state == Co_State_ready, \
            .curr_line = __ctx->ctrl.line \
        }; \
        if (false) { __step_return: \
            __flow_cursor.is_returning = true; \
            __ctx->ctrl.state = Co_State_ready; \
            goto __step_deferred; \
        } \
__step_deferred: switch (__flow_cursor.curr_line) { \
        default: { goto __step_unscope; } break; \
        case 0: __flow_cursor.curr_line--; /* clang-format on */
#define inline__$unguarded_co_fn() __stmt__$unguarded_co_fn
#define __stmt__$unguarded_co_fn /* clang-format off */ \
            break; \
        } \
        if (false) goto __step_suspend; \
        if (false) { __step_suspend: __step_unscope: \
            return __ctx; \
        } \
    } /* clang-format on */
#define __stmt__co_locals_mut_(...) __VA_ARGS__
#define __stmt__co_locals_(...) __VA_ARGS__
#define __stmt__co_suspended_(...) __VA_ARGS__
#define __stmt__co_deferrable_(...) __VA_ARGS__

#define __expr__$co_frame() (*__frame)
#define __expr__$co_arg(_$ident...) (__args->_$ident)
#define __expr__$co(_$ident...) (__locals->_$ident)
#define __expr__$co_mut(_$ident...) (__locals_mut->_$ident)
#define __expr__$co_suspended(_$ident...) (__suspended->_$ident)
#define __stmt__co_let_(_$ident, /*_$T*/...) \
    __VA_OPT__(({ claim_assert_static(eqlType$(TypeOf(__ctx->data.locals._$ident), __VA_ARGS__)); }), ) \
    __ctx->data.locals._$ident
#define __stmt__co_var_(_$ident, /*_$T*/...) \
    __VA_OPT__(({ claim_assert_static(eqlType$(TypeOf(__locals_mut->_$ident), __VA_ARGS__)); }), ) \
    __locals_mut->_$ident
#define __step__$co_init__expand(...) __VA_ARGS__
#define __expr__$co_init(_$ident...) (TypeOf(__ctx->data.locals._$ident)) __step__$co_init__expand
#define __step__$co_init_mut__expand(...) __VA_ARGS__
#define __expr__$co_init_mut(_$ident...) (TypeOf(__locals_mut->_$ident)) __step__$co_init_mut__expand

#define __stmt__suspend_(...) __step__suspend___emit(__step__suspend___parse __VA_ARGS__)
#define __step__suspend___parse(_$ident...) pp_uniqTok(suspended_data), _$ident, /*_$expr*/
#define __step__suspend___emit(...) __inline__suspend_(__VA_ARGS__)
#define __inline__suspend_(__suspended_data, _$ident, _$expr...) \
    do { \
        let __suspended_data = &__suspended->_$ident; \
        *__suspended_data = _$expr; \
        __ctx->suspended_data = __suspended_data; \
        __ctx->ctrl.state = Co_State_suspended; \
        __ctx->ctrl.line = __LINE__; \
        goto __step_suspend; \
    case __LINE__:; \
        claim_assert(__ctx->ctrl.state == Co_State_suspended); \
        __ctx->ctrl.state = Co_State_pending; \
    } while (false)

#define __expr__resume_(_$p_frame...) __inline__resume_(pp_uniqTok(p_frame), _$p_frame)
#define __inline__resume_(__p_frame, _$p_frame...) local_({ \
    let __p_frame = ensureNonnull(_$p_frame); \
    claim_assert(__p_frame->ctx_->ctrl.state != Co_State_undefined); \
    local_return_(__p_frame->rtn(__p_frame->ctx_)->suspended_data); \
})

#define __expr__co_return_(_$expr...) return_(_$expr)
#define __expr__co_returned(...)

#define __stmt__co_defer_(_$expr...) comp_syn__co_defer__op_snapshot(_$expr; goto __step_deferred)
#define __stmt__co_errdefer_(_$capt, _$expr...) co_defer_(if (!__reserved_return->is_ok) { \
    claim_assert(__reserved_return->payload.err.ctx != 0); \
    claim_assert_nonnull(__reserved_return->payload.err.vt); \
    let _$capt = __reserved_return->payload.err; \
    _$expr; \
})
/* clang-format off */
/* Coroutine block-local defer boundary. A raw break/continue inside the body
 * exits this synthetic block, not an outer loop. */
#define comp_syn__co_blk_defer { do { \
    comp_syn__co_defer__op_snapshot( \
        if (__flow_cursor.is_returning) { \
            goto __step_deferred; \
        } else { \
            continue; \
        } \
    ); \
    do
#define comp_syn__co_blk_deferral \
    while (false); \
    goto __step_deferred; \
} while (false); }

/* Coroutine loop-iteration defer boundary. Direct raw continue/break in the
 * body run iteration defers before continuing or breaking the enclosing loop. */
#define comp_syn__co_loop_defer { \
    local_label __co_loop_defer_break; \
    do { \
        comp_syn__co_defer__op_snapshot_(true, \
            if (__flow_cursor.is_returning) { \
                goto __step_deferred; \
            } else if (*S_at((__deferrable_break_stack)[*__deferrable_top])) { \
                goto __co_loop_defer_break; \
            } else { \
                continue; \
            } \
        ); \
        do
#define comp_syn__co_loop_deferral \
        while (comp_syn__co_loop_defer__mark_continuing(), false); \
        goto __step_deferred; \
    } while (false); \
    continue; \
__co_loop_defer_break: \
    break; \
}
/* clang-format on */
#define comp_syn__co_loop_defer__mark_continuing() ({ \
    for (usize __co_loop_defer_i = *__deferrable_top; __co_loop_defer_i > 0;) { \
        __co_loop_defer_i -= 1; \
        if (*S_at((__deferrable_break_stack)[__co_loop_defer_i])) { \
            *S_at((__deferrable_break_stack)[__co_loop_defer_i]) = false; \
            break; \
        } \
    } \
    false; \
})
#define comp_syn__co_defer__op_snapshot(_$expr...) comp_syn__co_defer__op_snapshot_(false, _$expr)
#define comp_syn__co_defer__op_snapshot_(_$is_breaking, _$expr...) \
    { \
        let __deferrable_idx = (*__deferrable_top)++; \
        *S_at((__deferrable_stack)[__deferrable_idx]) = __flow_cursor.curr_line; \
        *S_at((__deferrable_break_stack)[__deferrable_idx]) = (_$is_breaking); \
        __flow_cursor.curr_line = __LINE__; \
        if (false) { \
        case __LINE__: \
            __flow_cursor.curr_line = *S_at((__deferrable_stack)[--(*__deferrable_top)]); \
            _$expr; \
        } \
    }

#define __expr__co_call_(_$ctx_co, _$fnCo_and_Args...)
#define __stmt__co_called(...)

T_use_Co_Ctx$(Void);
T_use_Co_Rtn$(Void);
T_use_Co_Frame$(Void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_Co__included */
