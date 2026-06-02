/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    prl.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-21 (date of creation)
 * @updated 2025-06-21 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  (none)
 *
 * @brief   Prelude for dh
 * @details Provides a prelude for dh.
 */
#ifndef prl__included
#define prl__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl/raw.h"
#include "dh/prl/tpl.h"
#include "dh/prl/meta.h"

#include "dh/prl/base.h"

// #include "dh/prl/Co.h"
// #include "dh/prl/Clsr.h"

#include "dh/prl/CompHash.h"
#include "dh/prl/Err.h"
#include "dh/prl/ErrTrace.h"

// #include "dh/prl/common.h"

// #include "dh/prl/int.h"
// #include "dh/prl/flt.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl__included */
/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-21 (date of creation)
 * @updated 2025-06-21 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/prl
 * @prefix  (none)
 *
 * @brief   Common prelude for dh/prl
 * @details Provides common prelude for dh/prl.
 */
#ifndef prl_common__included
#define prl_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

// #include "base.h"
// #include "Co.h"
// #include "Clsr.h"
// #include "CompHash.h"
// #include "Err.h"
// #include "ErrTrace.h"

/*========== Macros and Declarations ========================================*/

#define T_use_prl$(_T...) \
    T_use_prl_refs$(_T); \
    T_use_prl_vals$(_T); \
    T_use_prl_exts$(_T)

T_use$((TypeInfo)(P, S));
#define typeInfos$(_T...) ____typeInfos$(_T)
#define typeInfosFrom(_type_infos...) ____typeInfosFrom(_type_infos)

/*========== Macros and Definitions =========================================*/

#define ____typeInfos$(_T...) \
    A_ref$((S_const$TypeInfo)A_from$((TypeInfo){ pp_foreach(____typeInfos$__each, ~, _T) }))
#define ____typeInfos$__each(_$ignored, _T...) \
    typeInfo$(_T),
#define ____typeInfosFrom(_type_infos...) \
    A_ref$((S_const$TypeInfo)A_from$((TypeInfo){ pp_foreach(____typeInfosFrom__each, ~, _type_infos) }))
#define ____typeInfosFrom__each(_$ignored, _type_info...) \
    (_type_info),

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_common__included */
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
#ifndef prl_Co_included
#define prl_Co_included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

// #include "base.h"

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
#define Co_Ctx$(_T...) __alias__Co_Ctx$(_T)
T_alias$((Co_Ctx$raw)(struct Co_Ctx$raw {
    var_(ctrl, Co_FlowCtrl);
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
#define co_fn_frame_scope(_fnName, _Locals, _LocalsMut, _Suspended...) \
    __stmt__co_fn_frame_scope(_fnName, _Locals, _LocalsMut, _Suspended)
#define co_fn_scope(_fnName...) \
    __stmt__co_fn_scope(_fnName)
#define $unscoped_co_fn \
    __stmt__$unscoped_co_fn()
#define co_fn_frame_guard(_fnName, _Locals, _LocalsMut, _Suspended, _Deferrable...) \
    __stmt__co_fn_frame_guard(_fnName, _Locals, _LocalsMut, _Suspended, _Deferrable)
#define co_fn_guard(_fnName...) \
    __stmt__co_fn_guard(_fnName)
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
#define $co_arg(_ident...) __expr__$co_arg(_ident)
#define $co(_ident...) __expr__$co(_ident)
#define $co_mut(_ident...) __expr__$co_mut(_ident)
#define $co_suspended(_ident...) __expr__$co_suspended(_ident)
#define co_let_(_ident, _T...) __stmt__co_let_(_ident, _T)
#define co_var_(_ident, _T...) __stmt__co_var_(_ident, _T)
#define $co_init(_ident...) __expr__$co_init(_ident)
#define $co_init_mut(_ident...) __expr__$co_init_mut(_ident)
#define suspend_(/*(_ident)(_expr...)*/... /*(void)*/) __stmt__suspend_(__VA_ARGS__)
#define resume_(_p_frame /*: P(Co_Frame)*/... /*(P(Co_Frame))*/) __expr__resume_(_p_frame)
#define co_return_(_expr...) __expr__co_return_(_expr)
#define co_defer_(_expr...) __stmt__co_defer_(_expr)
#define co_errdefer_(_capt, _expr...) __stmt__co_errdefer_(_capt, _expr)
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
#define __alias__Co_Ctx$(_T...) tpl$(Co_Ctx, _T)
#define __stmt__T_use_Co_Ctx$(_T...) \
    T_decl_Co_Ctx$(_T); \
    T_impl_Co_Ctx$(_T)
#define __stmt__T_decl_Co_Ctx$(_T...) \
    T_alias$((Co_Ctx$(_T))(union Co_Ctx$(_T)))
#define __stmt__T_impl_Co_Ctx$(_T...) \
    union Co_Ctx$(_T) { \
        T_embed$(struct { \
            var_(ctrl, Co_FlowCtrl); \
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
    fn_((_fnName(P$$(union Co_Ctx_(_fnName)) ctx))(P$$(union Co_Ctx_(_fnName)))); \
    T_alias$((Co_FrameBase_(_fnName))(Co_Frame$(Ret_T))); \
    T_alias$((Co_Frame_(_fnName))(union Co_Frame_(_fnName))); \
    T_alias$((Co_CtxBase_(_fnName))(Co_Ctx$(Ret_T))); \
    T_alias$((Co_Ctx_(_fnName))(union Co_Ctx_(_fnName))); \
    T_alias$((Co_Rtn_(_fnName))(fn_(((*)(P$$(Co_Ctx_(_fnName)) ctx))(P$$(Co_Ctx_(_fnName)))$T))); \
    T_alias$((Co_Ret_(_fnName))(Ret_T)); \
    T_alias$((Co_Args_(_fnName))(struct Co_Args_(_fnName) { \
        pp_Tuple_unwrap Args; \
    }))
#define __stmt__co_fn_frame_scope(_fnName, _Locals, _LocalsMut, _Suspended...) \
    claim_assert_static(eqlType$( \
        TypeOf(_fnName)*, \
        fn_(((*)(P$$(union Co_Ctx_(_fnName)) ctx))(P$$(union Co_Ctx_(_fnName)))$T) \
    )); \
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
            var_(ctrl, Co_FlowCtrl); \
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
    }
#define __stmt__co_fn_scope(_fnName...) \
    fn_((_fnName(P$$(union Co_Ctx_(_fnName)) ctx))(P$$(union Co_Ctx_(_fnName)))) { /* clang-format off */ \
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
#define __stmt__co_fn_frame_guard(_fnName, _Locals, _LocalsMut, _Suspended, _Deferrable...) \
    claim_assert_static(eqlType$( \
        TypeOf(_fnName)*, \
        fn_(((*)(P$$(union Co_Ctx_(_fnName)) ctx))(P$$(union Co_Ctx_(_fnName)))$T) \
    )); \
    T_alias$((Co_Suspended_(_fnName))(union Co_Suspended_(_fnName) _Suspended)); \
    T_alias$((Co_Locals_(_fnName))(struct Co_Locals_(_fnName) _Locals)); \
    T_alias$((Co_LocalsMut_(_fnName))(struct Co_LocalsMut_(_fnName) _LocalsMut)); \
    T_alias$((Co_Data_(_fnName))(struct Co_Data_(_fnName) { \
        var_(suspended, Co_Suspended_(_fnName)); \
        var_(locals, Co_Locals_(_fnName)); \
        var_(locals_mut, Co_LocalsMut_(_fnName)); \
        var_(deferrable_top, u32); \
        var_(deferrable_stack, A$$(_Deferrable, fn__FlowCursorPacked)); \
        var_(deferrable_break_stack, A$$(_Deferrable, bool)); \
    })); \
    union Co_Ctx_(_fnName) { \
        T_embed$(struct { \
            var_(ctrl, Co_FlowCtrl); \
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
    }
#define __stmt__co_fn_guard(_fnName, _Locals, _LocalsMut, _Suspended, _Deferrable...) \
    fn_((_fnName(P$$(union Co_Ctx_(_fnName)) ctx))(P$$(union Co_Ctx_(_fnName)))) { /* clang-format off */ \
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
#define __expr__$co_arg(_ident...) (__args->_ident)
#define __expr__$co(_ident...) (__locals->_ident)
#define __expr__$co_mut(_ident...) (__locals_mut->_ident)
#define __expr__$co_suspended(_ident...) (__suspended->_ident)
#define __stmt__co_let_(_ident, /*_T*/...) \
    __VA_OPT__(({ claim_assert_static(eqlType$(TypeOf(__ctx->data.locals._ident), __VA_ARGS__)); }), ) \
    __ctx->data.locals._ident
#define __stmt__co_var_(_ident, /*_T*/...) \
    __VA_OPT__(({ claim_assert_static(eqlType$(TypeOf(__locals_mut->_ident), __VA_ARGS__)); }), ) \
    __locals_mut->_ident
#define __step__$co_init__expand(...) __VA_ARGS__
#define __expr__$co_init(_ident...) (TypeOf(__ctx->data.locals._ident)) __step__$co_init__expand
#define __step__$co_init_mut__expand(...) __VA_ARGS__
#define __expr__$co_init_mut(_ident...) (TypeOf(__locals_mut->_ident)) __step__$co_init_mut__expand

#define __stmt__suspend_(...) __step__suspend___emit(__step__suspend___parse __VA_ARGS__)
#define __step__suspend___parse(_ident...) pp_uniqTok(suspended_data), _ident, /*_expr*/
#define __step__suspend___emit(...) __inline__suspend_(__VA_ARGS__)
#define __inline__suspend_(__suspended_data, _ident, _expr...) \
    do { \
        let __suspended_data = &__suspended->_ident; \
        *__suspended_data = _expr; \
        __ctx->suspended_data = __suspended_data; \
        __ctx->ctrl.state = Co_State_suspended; \
        __ctx->ctrl.line = __LINE__; \
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

#define __stmt__co_defer_(_expr...) comp_syn__co_defer__op_snapshot(_expr; goto __step_deferred)
#define __stmt__co_errdefer_(_capt, _expr...) co_defer_(if (!__reserved_return->is_ok) { \
    claim_assert(__reserved_return->payload.err.ctx != 0); \
    claim_assert_nonnull(__reserved_return->payload.err.vt); \
    let _capt = __reserved_return->payload.err; \
    _expr; \
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
#define comp_syn__co_defer__op_snapshot(_expr...) comp_syn__co_defer__op_snapshot_(false, _expr)
#define comp_syn__co_defer__op_snapshot_(_is_breaking, _expr...) \
    { \
        let __deferrable_idx = (*__deferrable_top)++; \
        *S_at((__deferrable_stack)[__deferrable_idx]) = __flow_cursor.curr_line; \
        *S_at((__deferrable_break_stack)[__deferrable_idx]) = (_is_breaking); \
        __flow_cursor.curr_line = __LINE__; \
        if (false) { \
        case __LINE__: \
            __flow_cursor.curr_line = *S_at((__deferrable_stack)[--(*__deferrable_top)]); \
            _expr; \
        } \
    }

#define __expr__co_call_(_ctx_co, _fnCo_and_Args...)
#define __stmt__co_called(...)

T_use_Co_Ctx$(Void);
T_use_Co_Rtn$(Void);
T_use_Co_Frame$(Void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_Co_included */
#ifndef prl_Clsr_included
#define prl_Clsr_included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

// #include "Co.h"

/*========== Macros and Declarations ========================================*/

/*--- Closure's Context ---*/
#define Clsr_Ctx$(_T...) __alias__Clsr_Ctx$(_T)
T_alias$((Clsr_Ctx$raw)(struct Clsr_Ctx$raw {
    var_(ret_, V$raw) $flexible;
    var_(data_, V$raw) $flexible;
}));
#define T_use_Clsr_Ctx$(_T...) __stmt__T_use_Clsr_Ctx$(_T)
#define T_decl_Clsr_Ctx$(_T...) __stmt__T_decl_Clsr_Ctx$(_T)
#define T_impl_Clsr_Ctx$(_T...) __stmt__T_impl_Clsr_Ctx$(_T)
/*--- Closure's Routine ---*/
#define Clsr_Rtn$(_T...) __alias__Clsr_Rtn$(_T)
T_alias$((Clsr_Rtn)(fn_(((*)(P$$(Clsr_Ctx$raw) ctx))(P$$(Clsr_Ctx$raw)) $T)));
#define T_use_Clsr_Rtn$(_T...) __stmt__T_use_Clsr_Rtn$(_T)
/*--- Closure's Kind ---*/
T_alias$((Clsr_Kind)(enum_((Clsr_Kind $fits($packed))(
    Clsr_Kind_undefined = 0,
    Clsr_Kind_fn = 1,
    Clsr_Kind_co = 2,
))));
claim_assert_static(eqlType$(Clsr_Kind, u8));
/*--- Closure's Base ---*/
#define Clsr$(_T...) __alias__Clsr$(_T)
T_alias$((Clsr$raw)(struct Clsr$raw {
    T_embed$(struct {
        var_(kind, Clsr_Kind);
        var_(rtn, Clsr_Rtn);
    });
    var_(ctx_, Clsr_Ctx$raw) $flexible;
}));
#define T_use_Clsr$(_T...) __stmt__T_use_Clsr$(_T)
#define T_decl_Clsr$(_T...) __stmt__T_decl_Clsr$(_T)
#define T_impl_Clsr$(_T...) __stmt__T_impl_Clsr$(_T)
/*--- Closure's Fields ---*/
#define Clsr_Ret_(_rtn...) __alias__Clsr_Ret_(_rtn)
// #define fn_use_Clsr_Ret_(_rtn...) __stmt__fn_use_Clsr_Ret_(_rtn)
// #define co_use_Clsr_Ret_(_rtn...) __stmt__co_use_Clsr_Ret_(_rtn)
#define Clsr_Data_(_rtn...) __alias__Clsr_Data_(_rtn)
// #define fn_use_Clsr_Data_(_rtn...) __stmt__fn_use_Clsr_Data_(_rtn)
// #define co_use_Clsr_Data_(_rtn...) __stmt__co_use_Clsr_Data_(_rtn)
#define Clsr_Ctx_(_rtn...) __alias__Clsr_Ctx_(_rtn)
// #define fn_use_Clsr_Ctx_(_rtn...) __stmt__fn_use_Clsr_Ctx_(_rtn)
// #define co_use_Clsr_Ctx_(_rtn...) __stmt__co_use_Clsr_Ctx_(_rtn)
#define Clsr_Rtn_(_rtn...) __alias__Clsr_Rtn_(_rtn)
// #define fn_use_Clsr_Rtn_(_rtn...) __stmt__fn_use_Clsr_Rtn_(_rtn)
// #define co_use_Clsr_Rtn_(_rtn...) __stmt__co_use_Clsr_Rtn_(_rtn)
#define Clsr_rtn_(_rtn...) __alias__Clsr_rtn_(_rtn)
// #define fn_use_Clsr_rtn_(_rtn...) __stmt__fn_use_Clsr_rtn_(_rtn)
// #define co_use_Clsr_rtn_(_rtn...) __stmt__co_use_Clsr_rtn_(_rtn)
#define Clsr_from_(_rtn...) __alias__Clsr_from_(_rtn)
// #define fn_use_Clsr_from_(_rtn...) __stmt__fn_use_Clsr_from_(_rtn)
// #define co_use_Clsr_from_(_rtn...) __stmt__co_use_Clsr_from_(_rtn)
/*--- Closure ---*/
#define Clsr_(_rtn...) __alias__Clsr_(_rtn)
#define clsr_(/*(_rtn)(_args...)*/... /*(Clsr)*/) __expr__clsr_(__VA_ARGS__)
#define invoke_(_p_clsr... /*-> (Clsr_Ctx)*/) __expr__invoke_(_p_clsr)
#define fn_use_Clsr_(/*(_fn)(_Arg_T...)(_Ret_T)*/...) __stmt__fn_use_Clsr_(__VA_ARGS__)
#define co_use_Clsr_(/*(_co)(_Arg_T...)(_Ret_T)*/...) __stmt__co_use_Clsr_(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

/*--- Closure's Context ---*/
#define __alias__Clsr_Ctx$(_T...) tpl$(Clsr_Ctx, _T)
#define __stmt__T_use_Clsr_Ctx$(_T...) \
    T_decl_Clsr_Ctx$(_T); \
    T_impl_Clsr_Ctx$(_T)
#define __stmt__T_decl_Clsr_Ctx$(_T...) \
    T_alias$((Clsr_Ctx$(_T))(union Clsr_Ctx$(_T)))
#define __stmt__T_impl_Clsr_Ctx$(_T...) \
    union Clsr_Ctx$(_T) { \
        T_embed$(struct { \
            T_embed$(union { \
                var_(ret, _T); \
                var_(ret_, _T) $like_ref; \
            }); \
            var_(data_, V$raw) $flexible; \
        }); \
        var_(as_raw, Clsr_Ctx$raw) $flexible; \
    }
/*--- Closure's Routine ---*/
#define __alias__Clsr_Rtn$(_T...) tpl$(Clsr_Rtn, _T)
#define __stmt__T_use_Clsr_Rtn$(_T...) \
    T_alias$((Clsr_Rtn$(_T))(fn_(((*)(P$$(Clsr_Ctx$(_T)) ctx))(P$$(Clsr_Ctx$(_T)))$T)))
/*--- Closure's Base ---*/
#define __alias__Clsr$(_T...) tpl$(Clsr, _T)
#define __stmt__T_use_Clsr$(_T...) \
    T_decl_Clsr$(_T); \
    T_impl_Clsr$(_T)
#define __stmt__T_decl_Clsr$(_T...) \
    T_alias$((Clsr$(_T))(union Clsr$(_T)))
#define __stmt__T_impl_Clsr$(_T...) \
    union Clsr$(_T) { \
        T_embed$(struct { \
            var_(kind, Clsr_Kind); \
            var_(rtn, Clsr_Rtn$(_T)); \
            T_embed$(union { \
                var_(ctx, Clsr_Ctx$(_T)); \
                var_(ctx_, Clsr_Ctx$(_T)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Clsr$raw) $flexible; \
    }
/*--- Closure's Fields ---*/
#define __alias__Clsr_Ret_(_rtn...) tpl_(Clsr_Ret, _rtn)
#define __alias__Clsr_Data_(_rtn...) tpl_(Clsr_Data, _rtn)
#define __alias__Clsr_Ctx_(_rtn...) tpl_(Clsr_Ctx, _rtn)
#define __alias__Clsr_Rtn_(_rtn...) tpl_(Clsr_Rtn, _rtn)
#define __alias__Clsr_rtn_(_rtn...) tpl_(Clsr_rtn, _rtn)
#define __alias__Clsr_from_(_rtn...) tpl_(Clsr_from, _rtn)
/*--- Closure ---*/
#define __alias__Clsr_(_rtn...) tpl_(Clsr, _rtn)
#define __alias__clsr_(_rtn...) tpl_(clsr, _rtn)
#define __expr__clsr_(...) __step__clsr___emit(__step__clsr___parseRtn __VA_ARGS__)
#define __step__clsr___parseRtn(_rtn...) _rtn,
#define __step__clsr___emit(...) __inline__clsr_(__VA_ARGS__)
#define __inline__clsr_(_rtn, _args...) copy(Clsr_from_(_rtn) _args)
#define __expr__invoke_(_p_clsr...) __inline__invoke_(pp_uniqTok(p_ctx), pp_uniqTok(p_clsr), _p_clsr)
#define __inline__invoke_(__p_ctx, __p_clsr, _p_clsr...) local_({ \
    let __p_clsr = _p_clsr; \
    claim_assert(__p_clsr->kind != Clsr_Kind_undefined); \
    let __p_ctx = __p_clsr->ctx_; \
    local_return_(ptrAlignCast$((P$$(TypeOf(*__p_ctx)))( \
        __p_clsr->rtn(ptrCast$((Clsr_Ctx$raw*)(__p_ctx))) \
    ))); \
})
/* --- Closure of Function --- */
#define Clsr_Fn_Args_(_rtn...) tpl_(Clsr_Fn_Args, _rtn)
// #define fn_use_Clsr_Fn_Args_(_rtn...) __stmt__fn_use_Clsr_Args_(_rtn)
#define __stmt__fn_use_Clsr_(...) __step__fn_use_Clsr_(__step__fn_use_Clsr___parse0 __VA_ARGS__)
#define __step__fn_use_Clsr___parse0(_fn...) \
    _fn, __step__fn_use_Clsr___parse1
#define __step__fn_use_Clsr___parse1(_Arg_T...) \
    pp_countArg(_Arg_T), (_Arg_T), __step__fn_use_Clsr___parse2
#define __step__fn_use_Clsr___parse2(_Ret_T...) \
    _Ret_T
#define __step__fn_use_Clsr_(...) __inline__fn_use_Clsr_(__VA_ARGS__)
#define __inline__fn_use_Clsr_(_fn, _N_Arg_T, _Arg_T, _Ret_T...) \
    T_alias$((Clsr_Ret_(_fn))(_Ret_T)); \
    T_alias$((Clsr_Fn_Args_(_fn))(Tup$$ _Arg_T)); \
    T_alias$((Clsr_Data_(_fn))(struct Clsr_Data_(_fn) { \
        var_(args, Clsr_Fn_Args_(_fn)); \
    })); \
    T_alias$((Clsr_Ctx_(_fn))(union Clsr_Ctx_(_fn) { \
        T_embed$(struct { \
            T_embed$(union { \
                var_(ret, Clsr_Ret_(_fn)); \
                var_(ret_, Clsr_Ret_(_fn)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(data, Clsr_Data_(_fn)); \
                var_(data_, Clsr_Data_(_fn)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Clsr_Ctx$raw) $flexible; \
        var_(as_base, Clsr_Ctx$(_Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((Clsr_rtn_(_fn)(P$$(Clsr_Ctx_(_fn)) ctx))(P$$(Clsr_Ctx_(_fn)))) { \
        let rtn = _fn; \
        ctx->ret = rtn(__step__fn_use_Clsr___passTupFields(_N_Arg_T, ctx->data.args.)); \
        return ctx; \
    }; \
    T_alias$((Clsr_Rtn_(_fn))(TypeOf(Clsr_rtn_(_fn))*)); \
    T_alias$((Clsr_(_fn))(union Clsr_(_fn) { \
        T_embed$(struct { \
            var_(kind, Clsr_Kind); \
            var_(rtn, Clsr_Rtn_(_fn)); \
            T_embed$(union { \
                var_(ctx, Clsr_Ctx_(_fn)); \
                var_(ctx_, Clsr_Ctx_(_fn)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Clsr$raw) $flexible; \
        var_(as_base, Clsr$(_Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((Clsr_from_(_fn)(__step__fn_use_Clsr___tupFieldsToParams(_N_Arg_T, _Arg_T)))(Clsr_(_fn))) { \
        return (Clsr_(_fn)){ \
            .kind = Clsr_Kind_fn, \
            .rtn = Clsr_rtn_(_fn), \
            .ctx.data.args = { __step__fn_use_Clsr___passTupFields(_N_Arg_T) }, \
        }; \
    }
#define __step__fn_use_Clsr___passTupFields(_N_Arg_T, _field_path...) \
    pp_cat(__step__fn_use_Clsr___passTupFields, _N_Arg_T)(_field_path)
#define __step__fn_use_Clsr___passTupFields1(_field_path...) \
    _field_path $0
#define __step__fn_use_Clsr___passTupFields2(_field_path...) \
    _field_path $0, _field_path $1
#define __step__fn_use_Clsr___passTupFields3(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2
#define __step__fn_use_Clsr___passTupFields4(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3
#define __step__fn_use_Clsr___passTupFields5(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4
#define __step__fn_use_Clsr___passTupFields6(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4, _field_path $5
#define __step__fn_use_Clsr___passTupFields7(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4, _field_path $5, _field_path $6
#define __step__fn_use_Clsr___passTupFields8(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4, _field_path $5, _field_path $6, _field_path $7
#define __step__fn_use_Clsr___tupFieldsToParams(_N_Arg_T, _Arg_T...) \
    pp_cat(__step__fn_use_Clsr___tupFieldsToParams, _N_Arg_T) _Arg_T
#define __step__fn_use_Clsr___tupFieldsToParams1(_Arg_T1...) \
    _Arg_T1 $0
#define __step__fn_use_Clsr___tupFieldsToParams2(_Arg_T1, _Arg_T2...) \
    _Arg_T1 $0, _Arg_T2 $1
#define __step__fn_use_Clsr___tupFieldsToParams3(_Arg_T1, _Arg_T2, _Arg_T3...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2
#define __step__fn_use_Clsr___tupFieldsToParams4(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3
#define __step__fn_use_Clsr___tupFieldsToParams5(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4
#define __step__fn_use_Clsr___tupFieldsToParams6(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5, _Arg_T6...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4, _Arg_T6 $5
#define __step__fn_use_Clsr___tupFieldsToParams7(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5, _Arg_T6, _Arg_T7...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4, _Arg_T6 $5, _Arg_T7 $6
#define __step__fn_use_Clsr___tupFieldsToParams8(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5, _Arg_T6, _Arg_T7, _Arg_T8...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4, _Arg_T6 $5, _Arg_T7 $6, _Arg_T8 $7
/* --- Closure of Coroutine --- */
#define Clsr_Co_Frame_(_rtn...) tpl_(Clsr_Co_Frame, _rtn)
// #define co_use_Clsr_Co_Frame_(_rtn...) __stmt__co_use_Clsr_Co_Frame_(_rtn)
#define __stmt__co_use_Clsr_(...) __step__co_use_Clsr_(__step__co_use_Clsr___parse0 __VA_ARGS__)
#define __step__co_use_Clsr___parse0(_co...) \
    _co, __step__co_use_Clsr___parse1
#define __step__co_use_Clsr___parse1(_Arg_T...) \
    pp_countArg(_Arg_T), (_Arg_T), __step__co_use_Clsr___parse2
#define __step__co_use_Clsr___parse2(_Ret_T...) \
    _Ret_T
#define __step__co_use_Clsr_(...) __inline__co_use_Clsr_(__VA_ARGS__)
#define __inline__co_use_Clsr_(_co, _N_Arg_T, _Arg_T, _Ret_T...) \
    T_alias$((Clsr_Ret_(_co))(_Ret_T)); \
    T_alias$((Clsr_Co_Frame_(_co))(Co_Frame_(_co))); \
    T_alias$((Clsr_Data_(_co))(struct Clsr_Data_(_co) { \
        var_(frame, Clsr_Co_Frame_(_co)); \
    })); \
    T_alias$((Clsr_Ctx_(_co))(union Clsr_Ctx_(_co) { \
        T_embed$(struct { \
            T_embed$(union { \
                var_(ret, Clsr_Ret_(_co)); \
                var_(ret_, Clsr_Ret_(_co)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(data, Clsr_Data_(_co)); \
                var_(data_, Clsr_Data_(_co)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Clsr_Ctx$raw) $flexible; \
        var_(as_base, Clsr_Ctx$(_Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((Clsr_rtn_(_co)(P$$(Clsr_Ctx_(_co)) ctx))(P$$(Clsr_Ctx_(_co)))) { \
        let frame = &ctx->data.frame; \
        let_ignore = resume_(frame); \
        if (frame->ctx.ctrl.state == Co_State_ready) { \
            ctx->ret = frame->ctx.ret; \
        } \
        return ctx; \
    }; \
    T_alias$((Clsr_Rtn_(_co))(TypeOf(Clsr_rtn_(_co))*)); \
    T_alias$((Clsr_(_co))(union Clsr_(_co) { \
        T_embed$(struct { \
            var_(kind, Clsr_Kind); \
            var_(rtn, Clsr_Rtn_(_co)); \
            T_embed$(union { \
                var_(ctx, Clsr_Ctx_(_co)); \
                var_(ctx_, Clsr_Ctx_(_co)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Clsr$raw) $flexible; \
        var_(as_base, Clsr$(_Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((Clsr_from_(_co)(__step__co_use_Clsr___tupFieldsToParams(_N_Arg_T, _Arg_T)))(Clsr_(_co))) { \
        return (Clsr_(_co)){ \
            .kind = Clsr_Kind_co, \
            .rtn = Clsr_rtn_(_co), \
            .ctx.data.frame = { \
                .rtn = _co, \
                .ctx = { \
                    .ctrl = { \
                        .state = Co_State_pending, \
                        .line = 0, \
                    }, \
                    .suspended_data = {}, \
                    .args = { __step__co_use_Clsr___passTupFields(_N_Arg_T) }, \
                    .data = { \
                        .suspended = {}, \
                        .locals = {}, \
                        .locals_mut = {}, \
                    }, \
                }, \
            }, \
        }; \
    }
#define __step__co_use_Clsr___passTupFields(_N_Arg_T, _field_path...) \
    pp_cat(__step__co_use_Clsr___passTupFields, _N_Arg_T)(_field_path)
#define __step__co_use_Clsr___passTupFields1(_field_path...) \
    _field_path $0
#define __step__co_use_Clsr___passTupFields2(_field_path...) \
    _field_path $0, _field_path $1
#define __step__co_use_Clsr___passTupFields3(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2
#define __step__co_use_Clsr___passTupFields4(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3
#define __step__co_use_Clsr___passTupFields5(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4
#define __step__co_use_Clsr___passTupFields6(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4, _field_path $5
#define __step__co_use_Clsr___passTupFields7(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4, _field_path $5, _field_path $6
#define __step__co_use_Clsr___passTupFields8(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4, _field_path $5, _field_path $6, _field_path $7
#define __step__co_use_Clsr___tupFieldsToParams(_N_Arg_T, _Arg_T...) \
    pp_cat(__step__co_use_Clsr___tupFieldsToParams, _N_Arg_T) _Arg_T
#define __step__co_use_Clsr___tupFieldsToParams1(_Arg_T1...) \
    _Arg_T1 $0
#define __step__co_use_Clsr___tupFieldsToParams2(_Arg_T1, _Arg_T2...) \
    _Arg_T1 $0, _Arg_T2 $1
#define __step__co_use_Clsr___tupFieldsToParams3(_Arg_T1, _Arg_T2, _Arg_T3...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2
#define __step__co_use_Clsr___tupFieldsToParams4(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3
#define __step__co_use_Clsr___tupFieldsToParams5(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4
#define __step__co_use_Clsr___tupFieldsToParams6(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5, _Arg_T6...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4, _Arg_T6 $5
#define __step__co_use_Clsr___tupFieldsToParams7(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5, _Arg_T6, _Arg_T7...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4, _Arg_T6 $5, _Arg_T7 $6
#define __step__co_use_Clsr___tupFieldsToParams8(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5, _Arg_T6, _Arg_T7, _Arg_T8...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4, _Arg_T6 $5, _Arg_T7 $6, _Arg_T8 $7

T_use_Clsr_Ctx$(Void);
T_use_Clsr_Rtn$(Void);
T_use_Clsr$(Void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_Clsr_included */



/// \brief Report a message
/// \param label The label to report
/// \param fmt The format string
/// \param ... The arguments to the format string
$extern fn_((report(S_const$u8 label, S_const$u8 fmt, ...))(void));

T_use$((f64)(Co_Ctx, Co_Rtn, Co_Frame));
$static co_fn_(coOnlyArgs, (usize n; f64 interval_secs; S_const$u8 label), f64);
co_fn_frame_scope(
    coOnlyArgs,
    co_locals_({
        var_(instant, f64);
        var_(elapsed_secs, f64);
    }),
    co_locals_mut_({
        var_(i, usize);
    }),
    co_suspended_({
        var_(sleeping, Void);
    })
);
co_fn_scope(coOnlyArgs) {
    co_let_(instant) = time_Awake_now($co_arg(sys).time);
    report($co_arg(label), u8_l("before loop {:.1fl}"), $co_arg(interval_secs));

    for (co_var_(i) = 0; $co_mut(i) < $co_arg(n); ++$co_mut(i)) {
        suspend_((sleeping)(catch_((time_Awake_sleep(
            $co_arg(sys).time, time_Dur_fromSecs$f64($co_arg(interval_secs))
        ))($ignore, $do_nothing))));
        report(
            $co_arg(label), u8_l("slept {:.1fl} | i: {:uz} < n: {:uz}"),
            $co_arg(interval_secs), $co_mut(i), $co_arg(n)
        );
    }

    co_let_(elapsed_secs) = pipe_(($co(instant))(
        (t)(time_Awake_Inst_elapsed(t, $co_arg(sys).time)),
        (t)(time_Dur_asSecs$f64(t))
    ));
    report($co_arg(label), u8_l("after loop {:.1fl}"), $co(elapsed_secs));
    co_return_($co(elapsed_secs));
} $unscoped(co_fn);
T_use$((f64)(Clsr_Ctx, Clsr_Rtn, Clsr));
co_use_Clsr_((coOnlyArgs)(usize, f64, S_const$u8)(f64));
