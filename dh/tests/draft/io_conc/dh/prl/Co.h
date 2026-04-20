/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Co.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-28 (date of creation)
 * @updated 2026-03-28 (date of last update)
 * @version v0.2
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

#include "Co/Fiber.h"

/*========== Core Types =====================================================*/

typedef u32 Co_Count;
typedef enum $packed Co_State {
    Co_State_none = 0,
    Co_State_pending,
    Co_State_suspended,
    Co_State_ready,
} Co_State;
typedef struct Co_Ctx Co_Ctx;
typedef fn_(((*Co_FnWork)(Co_Ctx* ctx))(Co_Ctx*));
typedef struct Co_Ret {
    u8 data[0];
} Co_Ret;
typedef struct Co_Ctx {
    var_(fn, Co_FnWork);
    var_(state, Co_State);
    var_(count, Co_Count);
    var_(suspend_data, void*);
    var_(ret, Co_Ret*);
} Co_Ctx;

T_alias$((Sched)(struct Sched));
T_alias$((Future$raw)(struct Future$raw));
T_use_P$(Future$raw);
T_alias$((V$Future$raw)(P$Future$raw));
T_use_O$(P$Future$raw);

/*========== Typed Return Context ===========================================*/

#define Co_Ctx$(T_Return) pp_join($, Co_Ctx, T_Return)
#define use_Co_Ctx$(T_Return) \
    decl_Co_Ctx$(T_Return); \
    impl_Co_Ctx$(T_Return)
#define decl_Co_Ctx$(T_Return) \
    typedef union Co_Ctx$(T_Return) Co_Ctx$(T_Return)
#define impl_Co_Ctx$(T_Return) \
    union Co_Ctx$(T_Return) { \
        Co_Ctx anyraw $like_ref; \
        struct { \
            Co_FnWork fn; \
            Co_State state; \
            Co_Count count; \
            void* suspend_data; \
            union { \
                Co_Ret base $like_ref; \
                T_Return value; \
            } ret $like_ref; \
        }; \
    }

/*========== Typed Args Context =============================================*/

#define Co_CtxArgs$(fnName) pp_join($, Co_CtxArgs, fnName)
#define use_Co_CtxArgs$(fnName, Args) \
    decl_Co_CtxArgs$(fnName); \
    impl_Co_CtxArgs$(fnName, Args)
#define decl_Co_CtxArgs$(fnName) \
    typedef struct Co_CtxArgs$(fnName) Co_CtxArgs$(fnName)
#define impl_Co_CtxArgs$(fnName, Args) \
    struct Co_CtxArgs$(fnName) { \
        pp_Tuple_unwrap Args \
    }

/*========== Typed Function Context =========================================*/

#define Co_CtxFnBase$(fnName) pp_join($, Co_CtxFnBase, fnName)
#define use_Co_CtxFnBase$(fnName, T_Return) \
    typedef Co_Ctx$(T_Return) Co_CtxFnBase$(fnName)

#define Co_CtxFn$(fnName) pp_join($, Co_CtxFn, fnName)
#define use_Co_CtxFn$(fnName, Args, T_Return, Locals) \
    use_Co_CtxFnBase$(fnName, T_Return); \
    decl_Co_CtxFn$(fnName); \
    impl_Co_CtxFn$(fnName, Args, Locals)
#define decl_Co_CtxFn$(fnName) \
    typedef union Co_CtxFn$(fnName) Co_CtxFn$(fnName)
#define impl_Co_CtxFn$(fnName, Locals) \
    union Co_CtxFn$(fnName) { \
        Co_Ctx anyraw $like_ref; \
        Co_CtxFnBase$(fnName) base $like_ref; \
        struct { \
            Co_FnWork fn; \
            Co_State state; \
            Co_Count count; \
            void* suspend_data; \
            FieldType$(Co_CtxFnBase$(fnName), ret) ret; \
            Co_CtxArgs$(fnName) args; \
            struct Locals locals; \
        }; \
    }

#define Co_CtxFn_init$(fnName) ((Co_CtxFn$(fnName)){ \
    .fn = as$(Co_FnWork)(fnName), \
    .state = Co_State_pending, \
    .count = 0, \
    .suspend_data = null, \
    .ret = {}, \
    .args = {}, \
    .locals = {}, \
})

/*========== Coroutine Definition Macros ====================================*/

/// co_fn_: declare a coroutine function (replaces async_fn_)
#define co_fn_(_fnName, Args, T_Return...) \
    use_Co_CtxArgs$(_fnName, Args); \
    use_Co_CtxFnBase$(_fnName, T_Return); \
    decl_Co_CtxFn$(_fnName); \
    fn_((_fnName($P$(Co_CtxFn$(_fnName)) ctx))($P$(Co_CtxFn$(_fnName))))

/// co_fn_scope: define coroutine body with locals (replaces async_fn_scope)
#define co_fn_scope(_fnName, Locals...) \
    impl_Co_CtxFn$(_fnName, Locals); \
    fn_((_fnName($P$(Co_CtxFn$(_fnName)) ctx))($P$(Co_CtxFn$(_fnName)))) { \
        let args = &ctx->args; \
        let locals = &ctx->locals; \
        let __reserved_return = &ctx->ret->value; \
        var __scope_counter = (struct fn__ScopeCounter){ \
            .is_returning = ctx->state == Co_State_ready, \
            .current_line = ctx->count \
        }; \
        if (false) { \
__step_return: \
            __scope_counter.is_returning = true; \
            ctx->state = Co_State_ready; \
            goto __step_unscope; \
        } \
        switch (__scope_counter.current_line) { \
        default: { \
            goto __step_unscope; \
        } break; \
        case 0: \
            __scope_counter.current_line--;

// clang-format off
#define inline__$unscoped_co_fn() $unscoped_co_fn
#define $unscoped_co_fn \
        break; \
    } \
    if (false) { \
__step_suspend: \
__step_unscope: \
        return ctx; \
    } \
}
// clang-format on

/*========== Return / Suspend / Resume ======================================*/

#define co_return_(_expr...) comp_syn__co_return_(_expr)
#define comp_syn__co_return_(_expr...) local_({ \
    debug_assert_nonnull(ctx); \
    *__reserved_return = *(TypeOf(ctx->ret->value)[1]){ [0] = _expr }; \
    goto __step_return; \
})

#define suspend_(_expr...) comp_syn__suspend_(_expr)
#define comp_syn__suspend_(_expr...) \
    do { \
        debug_assert_nonnull(ctx); \
        ctx->state = Co_State_suspended; \
        ctx->count = __LINE__; \
        _expr; \
        goto __step_suspend; \
    case __LINE__:; \
        debug_assert_nonnull(ctx); \
        debug_assert(ctx->state == Co_State_suspended); \
        ctx->state = Co_State_pending; \
    } while (false)

#define resume_(_ctx...) comp_syn__resume_(pp_uniqTok(ctx), _ctx)
#define comp_syn__resume_(__ctx, _ctx...) local_({ \
    let __ctx = ensureNonnull(_ctx); \
    debug_assert(__ctx->state != Co_State_none); \
    local_return_((as$(Co_FnWork)(__ctx->fn))(as$(Co_Ctx*)(__ctx))); \
})

/*========== Coroutine Construction =========================================*/

#define co_ctx(_fnCo_and_Args...) \
    pp_expand(pp_defer(__exec_co_ctx)()(pp_Tuple_unwrapSufComma _fnCo_and_Args))
#define __exec_co_ctx() comp_syn__co_ctx
#define comp_syn__co_ctx(_fnCo, _args...) (&l$((Co_CtxFn$(_fnCo)){ \
    .fn = as$(Co_FnWork)(_fnCo), \
    .state = Co_State_pending, \
    .count = 0, \
    .suspend_data = null, \
    .args = { pp_Tuple_unwrap _args }, \
    .locals = {}, \
}))

/*========== Coroutine-to-Coroutine Call ====================================*/

/// co_call: call a child coroutine, propagating suspend_data upward
#define co_call(_ctx_co, _fnCo_and_Args...) \
    pp_expand(pp_defer(__exec_co_call)()(_ctx_co, pp_Tuple_unwrapSufComma _fnCo_and_Args))
#define __exec_co_call() comp_syn__co_call
#define comp_syn__co_call(_ctx_co, _fnCo, _args...) \
    *(_ctx_co) = *(&l$((Co_CtxFn$(_fnCo)){ \
        .fn = as$(Co_FnWork)(_fnCo), \
        .state = Co_State_pending, \
        .count = 0, \
        .suspend_data = null, \
        .args = { pp_Tuple_unwrap _args }, \
        .locals = {}, \
    })); \
    while (resume_(_ctx_co)->state == Co_State_suspended) { \
        ctx->suspend_data = (_ctx_co)->anyraw->suspend_data; \
        suspend_(); \
    }

/*========== Await / Nosuspend ==============================================*/

#define co_await_(_co_ctx...) comp_syn__co_await_(_co_ctx)
#define comp_syn__co_await_(_co_ctx...) \
    while ((_co_ctx)->state != Co_State_ready) { \
        ctx->suspend_data = (_co_ctx)->anyraw.suspend_data; \
        suspend_(); \
    }

#define nosuspend_(_expr...) comp_syn__nosuspend_(_expr)
#define comp_syn__nosuspend_(_expr...) expr_(__nosuspend, { \
    $local_label __step_suspend; \
    var ctx = (&l$((Co_Ctx){ .state = Co_State_pending, .count = 0, .suspend_data = null })); \
    switch (ctx->count) { \
    default: { \
        blk_break_(__nosuspend, {}); \
    } break; \
    case 0: { \
        _expr; \
    } break; \
    } \
    if (false) { \
__step_suspend: \
        debug_assert(ctx->state != Co_State_suspended); \
    } \
    blk_break_(__nosuspend, {}); \
})

/*========== Utility ========================================================*/

#define Co_Ctx_returned(_ctx...) comp_inline__Co_Ctx_returned(_ctx)
#define comp_inline__Co_Ctx_returned(_ctx...) \
    ((_ctx)->ret->value)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Co__included */
