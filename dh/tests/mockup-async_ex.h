// clang-format off
#define blk_(_label, _BreakType_and_Body...) \
    pp_overload(__blk, _BreakType_and_Body)(_label, _BreakType_and_Body)
#define __blk_1(_label, _Body...)                    comp_syn__blk_((_label, Void), _Body)
#define __blk_2(_label, _RetType, _Body...)          comp_syn__blk_((_label, _RetType), _Body)
#define comp_syn__blk_(_Label_and_RetType, _Body...) eval({ \
    local_label pp_Tuple_get1st _Label_and_RetType; \
    var pp_cat(__reserved_val_, pp_Tuple_get1st _Label_and_RetType) = _Generic( \
        TypeOf(pp_Tuple_get2nd _Label_and_RetType), \
        void: (Void){}, \
        default: (pp_Tuple_get2nd _Label_and_RetType){} \
    ); \
    _Body; \
    pp_Tuple_get1st _Label_and_RetType: \
    pp_cat(__reserved_val_, pp_Tuple_get1st _Label_and_RetType); \
})
// clang-format on

#define blk_break_(_label, ...) comp_syn__blk_break_(pp_cat(__reserved_val_, _label), _label, __VA_ARGS__)
#define comp_syn__blk_break_(__reserved_val, _label, ...) \
    __reserved_val = *(TypeOf(__reserved_val)[1]){ [0] = __VA_ARGS__ }; \
    goto _label

// Co.h
#ifndef CO_INCLUDED
#define CO_INCLUDED (1)

#include "dh/core.h"
#include "dh/fn.h"

// Coroutine state
typedef struct Co_Ctx Co_Ctx;
typedef fn_(Co_FnWork(Co_Ctx* ctx), Co_Ctx*);
typedef u32 Co_Count;
typedef enum Co_State {
    Co_State_pending = 0,
    Co_State_suspended,
    Co_State_ready,
} Co_State;
typedef struct Co_Ret {
    u8 data[];
} Co_Ret;
typedef struct Co_Ctx {
    Co_FnWork* fn;
    Co_Count   count;
    Co_State   state;
    Co_Ret*    ret;
} Co_Ctx;

#define Co_Ctx$(T_Return) pp_join($, Co_Ctx, T_Return)
#define use_Co_Ctx$(T_Return) \
    decl_Co_Ctx$(T_Return); \
    impl_Co_Ctx$(T_Return)
#define decl_Co_Ctx$(T_Return) \
    typedef union Co_Ctx$(T_Return) Co_Ctx$(T_Return)
#define impl_Co_Ctx$(T_Return) \
    union Co_Ctx$(T_Return) { \
        Co_Ctx anyraw $like_ptr; \
        struct { \
            Co_FnWork* fn; \
            Co_Count   count; \
            Co_State   state; \
            union { \
                Co_Ret base $like_ptr; \
                T_Return    value; \
            } ret $like_ptr; \
        }; \
    }

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
        Co_Ctx anyraw $like_ptr; \
        Co_CtxFnBase$(fnName) base $like_ptr; \
        struct { \
            Co_FnWork* fn; \
            Co_Count   count; \
            Co_State   state; \
            FieldType$(Co_CtxFnBase$(fnName), ret) ret; \
            Co_CtxArgs$(fnName) args; \
            struct Locals locals; \
        }; \
    }

#endif /* CO_INCLUDED */

#define async_fn_(_fnName, Args, T_Return...) \
    use_Co_CtxArgs$(_fnName, Args); \
    use_Co_CtxFnBase$(_fnName, T_Return); \
    decl_Co_CtxFn$(_fnName); \
    fn_(_fnName(Co_CtxFn$(_fnName) * ctx), Co_CtxFnBase$(_fnName)*)

#define async_fn_scope(_fnName, Locals...) \
    impl_Co_CtxFn$(_fnName, Locals); \
    fn_(_fnName(Co_CtxFn$(_fnName) * ctx), Co_CtxFnBase$(_fnName)*) { \
        let args              = &ctx->args; \
        let locals            = &ctx->locals; \
        let __reserved_return = &ctx->ret->value; \
        var __scope_counter   = (struct fn__ScopeCounter){ \
              .is_returning = ctx->state == Co_State_ready, \
              .current_line = ctx->count \
        }; \
        if (false) { \
__step_return: \
            __scope_counter.is_returning = true; \
            ctx->state                   = Co_State_ready; \
            goto __step_unscope; \
        } \
        switch (__scope_counter.current_line) { \
        default: { \
            goto __step_unscope; \
        } break; \
        case 0: \
            __scope_counter.current_line--;
// clang-format off
#define async_unscoped \
        break; \
    } \
    if (false) { \
__step_suspend: \
__step_unscope: \
        return ctx->base; \
    } \
}
// clang-format on

#define areturn_(_expr...)           comp_syn__areturn_(_expr)
#define comp_syn__areturn_(_expr...) eval({ \
    debug_assert_nonnull(ctx); \
    *__reserved_return = *(TypeOf(ctx->ret->value)[1]){ [0] = _expr }; \
    goto __step_return; \
})

#define async_(_fnAsync_and_Args...) \
    pp_expand(pp_defer(__exec_async_)()(pp_Tuple_unwrapSufComma _fnAsync_and_Args))
#define async_ctx(_fnAsync_and_Args...) \
    pp_expand(pp_defer(__exec_async_ctx)()(pp_Tuple_unwrapSufComma _fnAsync_and_Args))
#define callAsync(_ctx_async, _fnAsync_and_Args...) \
    pp_expand(pp_defer(__exec_callAsync)()(_ctx_async, pp_Tuple_unwrapSufComma _fnAsync_and_Args))

#define __exec_async_() comp_syn__async_
#define comp_syn__async_(_fnAsync, _args...) \
    as$(Co_CtxFn$(_fnAsync)*, _fnAsync(&(Co_CtxFn$(_fnAsync)){ .fn = as$(Co_FnWork*, _fnAsync), .count = 0, .state = Co_State_pending, .args = { pp_Tuple_unwrap _args }, .locals = {} }))

#define __exec_async_ctx() comp_syn__async_ctx
#define comp_syn__async_ctx(_fnAsync, _args...) \
    (&(Co_CtxFn$(_fnAsync)){ .fn = as$(Co_FnWork*, _fnAsync), .count = 0, .state = Co_State_pending, .args = { pp_Tuple_unwrap _args }, .locals = {} })

#define __exec_callAsync() comp_syn__callAsync
#define comp_syn__callAsync(_ctx_async, _fnAsync, _args...) \
    *(_ctx_async) = *(&(Co_CtxFn$(_fnAsync)){ .fn = as$(Co_FnWork*, _fnAsync), .count = 0, .state = Co_State_pending, .args = { pp_Tuple_unwrap _args }, .locals = {} }); \
    while (resume_(_ctx_async)->state == Co_State_suspended) { suspend_(); }

#define await_(_co_ctx...) comp_syn__await_(_co_ctx)
#define comp_syn__await_(_co_ctx...) \
    while ((_co_ctx)->state != Co_State_ready) { suspend_(); }

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

#define resume_(_ctx...)                  comp_syn__resume_(pp_uniqTok(ctx), _ctx)
#define comp_syn__resume_(__ctx, _ctx...) eval({ \
    let __ctx = ensureNonnull(_ctx); \
    __callFn(as$(Co_FnWork*, __ctx->fn), as$(Co_Ctx*, __ctx)); \
})

#define nosuspend_(_expr...)           comp_syn__nosuspend_(_expr)
#define comp_syn__nosuspend_(_expr...) blk_(__nosuspend, { \
    local_label __step_suspend; \
    var         ctx = (&(Co_Ctx){ .count = 0, .state = Co_State_pending }); \
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
