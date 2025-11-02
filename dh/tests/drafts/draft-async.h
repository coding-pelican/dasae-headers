// Co.h
#ifndef CO_INCLUDED
#define CO_INCLUDED (1)

#include "dh/core.h"
#include "dh/fn.h"


// Coroutine state
typedef struct Co_Ctx Co_Ctx;
typedef fn_((Co_FnWork(Co_Ctx* ctx))(Co_Ctx*));
typedef u32 Co_Count;
typedef enum Co_State {
    Co_State_pending = 0,
    Co_State_suspended,
    Co_State_ready,
} Co_State;
typedef struct Co_Ret {
    u8 data[0];
} Co_Ret;
typedef struct Co_Ctx {
    Co_FnWork* fn;
    Co_Count   count;
    Co_State   state;
    Co_Ret*    ret;
} Co_Ctx;
#define Co_Ctx$(fnName) pp_join($, Co_Ctx, fnName)
#define use_Co_Ctx$(fnName, Args, RetType, Locals) \
    typedef union pp_join($, Co_Ctx, fnName) { \
        Co_Ctx base[1]; \
        struct { \
            Co_FnWork* fn; \
            Co_Count   count; \
            Co_State   state; \
            union { \
                Co_Ret  base[1]; \
                RetType value; \
            } ret; \
            struct { \
                pp_Tuple_unwrap Args; \
            } args; \
            struct Locals locals; \
        }; \
    } pp_join($, Co_Ctx, fnName)

#endif /* CO_INCLUDED */


#define async_fn(_fnName) \
    fn_((_fnName(Co_Ctx * ctx))(Co_Ctx*))
#define async_fn_scope(_fnName) \
    async_fn(_fnName) { \
        let self   = as$((Co_Ctx$(_fnName)*)(ensureNonnull(ctx))); \
        let args   = &self->args; \
        let locals = &self->locals; \
        switch (self->count) { \
        default: \
            claim_unreachable; \
        case 0: \
        // suspend_();
#define async_unscoped \
    break; \
    } \
    }
#define async_return_frame(_frame, _expr...) \
    do { \
        debug_assert_nonnull(self); \
        self->count     = __LINE__; \
        self->ret.value = *(TypeOf(self->ret.value)[1]){ [0] = _expr }; \
    case __LINE__: \
        debug_assert_nonnull(self); \
        self->state = Co_State_ready; \
        return ensureNonnull(_frame); \
    } while (false)
#define async_return_(_expr...) \
    do { \
        debug_assert_nonnull(self); \
        self->count     = __LINE__; \
        self->ret.value = *(TypeOf(self->ret.value)[1]){ [0] = _expr }; \
    case __LINE__: \
        debug_assert_nonnull(self); \
        self->state = Co_State_ready; \
        return ensureNonnull(self->base); \
    } while (false)

#define async_(_fnAsync_and_Args...) \
    pp_expand(pp_defer(__exec_async_)()(pp_Tuple_unwrapSufComma _fnAsync_and_Args))
#define async_ctx(_fnAsync_and_Args...) \
    pp_expand(pp_defer(__exec_async_ctx)()(pp_Tuple_unwrapSufComma _fnAsync_and_Args))
#define async_with(_ctx, _fnAsync_and_Args...) \
    pp_expand(pp_defer(__exec_async_with)()(_ctx, pp_Tuple_unwrapSufComma _fnAsync_and_Args))

#define __exec_async_() __async_
#define __async_(_fnAsync, _args...) \
    as$((Co_Ctx$(_fnAsync)*)(_fnAsync((Co_Ctx$(_fnAsync)){ .fn = _fnAsync, .count = 0, .state = Co_State_pending, .args = { pp_Tuple_unwrap _args }, .locals = {} }.base)))

#define __exec_async_ctx() __async_ctx
#define __async_ctx(_fnAsync, _args...) \
    ((Co_Ctx$(_fnAsync)){ .fn = _fnAsync, .count = 0, .state = Co_State_pending, .args = { pp_Tuple_unwrap _args }, .locals = {} })

#define __exec_async_with() __async_with
#define __async_with(_ctx, _fnAsync, _args...) \
    suspend_(_ctx = *__async_(_fnAsync, _args))

#define suspend_(_expr...) comp_syn__suspend_(_expr)
#define comp_syn__suspend_(_expr...) \
    do { \
        debug_assert_nonnull(self); \
        self->state = Co_State_suspended; \
        self->count = __LINE__; \
        _expr; \
        return ensureNonnull(self->base); \
    case __LINE__:; \
        debug_assert_nonnull(self); \
    } while (false)

#define resume_(_ctx...)                  comp_syn__resume_(pp_uniqTok(ctx), _ctx)
#define comp_syn__resume_(__ctx, _ctx...) blk({ \
    let __ctx = ensureNonnull(_ctx); \
    callFn((__ctx->fn)(as$((Co_Ctx*)(__ctx)))); \
})

#define await_(_ctx...) comp_syn__await_(_ctx)
#define comp_syn__await_(_ctx...) \
    while (deref(_ctx).state != Co_State_ready) { suspend_(); }

#define nosuspend_await_(_ctx)                     comp_syn__nosuspend_await_(pp_uniqTok(ctx), _ctx)
#define comp_syn__nosuspend_await_(__ctx, _ctx...) blk({ \
    let __ctx = ensureNonnull(_ctx); \
    if (__ctx->state != Co_State_ready) { resume_(__ctx); } \
    debug_assert(__ctx->state == Co_State_ready); \
    blk_return __ctx->ret; \
})
