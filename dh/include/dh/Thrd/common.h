/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-23 (date of creation)
 * @updated 2025-05-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd
 *
 * @brief   Common definitions for thread management
 * @details Defines common definitions for thread management.
 */
#ifndef Thrd_common__included
#define Thrd_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/mem/Allocator.h"
#include "dh/time/common.h"

/*========== Macros and Declarations ========================================*/

// Thread ID type
typedef Thrd_Id__Impl Thrd_Id;
// Thread handle type
typedef Thrd_Handle__Impl Thrd_Handle;

// Thread function context type instantiation
#define use_Thrd_FnCtx$(_fnName, _Args, _T_Return) comp_type_gen__use_Thrd_FnCtx$(_fnName, _Args, _T_Return)
// Thread function context type alias
#define Thrd_FnCtx$(_fnName...) __alias__Thrd_FnCtx$(_fnName)
// Thread function context type erasure
typedef struct Thrd_FnCtx Thrd_FnCtx;
// Thread function arguments type erasure
typedef struct Thrd_FnArgs {
    u8 data $flexible;
} Thrd_FnArgs;
// Thread function return type erasure
typedef struct Thrd_FnRet {
    u8 data $flexible;
} Thrd_FnRet;
// Thread function type
typedef fn_(((*Thrd_WorkFn)(Thrd_FnCtx* thrd_ctx))(Thrd_FnCtx*));
struct Thrd_FnCtx {
    struct {
        Thrd_WorkFn fn;
        Thrd_Handle handle;
    };
    union {
        u8 data $flexible;
    };
};
#define Thrd_FnCtx_from$(/*(_fnName)(_args...)*/... /*(Thrd_FnCtx$(_fnName))*/) \
    __step__Thrd_FnCtx_from$(__step__Thrd_FnCtx_from$__parseFnName __VA_ARGS__)
#define Thrd_FnCtx_ret$(/*(_fnName)(_ctx...)*/... /*(_ReturnType$(_fnName))*/) \
    __step__Thrd_FnCtx_ret$(__step__Thrd_FnCtx_ret$__parseFnName __VA_ARGS__)
#define Thrd_FnCtx_call$(/*(_fnName)(_args...)*/... /*(_ReturnType$(_fnName))*/) \
    __step__Thrd_FnCtx_call$(__step__Thrd_FnCtx_call$__parseFnName __VA_ARGS__)

// Thread function syntax
#define Thrd_fn_(_fnName, _Tuple_Args_Ret_w_Tuple_Captures_w_Extension...) \
    pp_overload(__Thrd_fn, _Tuple_Args_Ret_w_Tuple_Captures_w_Extension)(_fnName, _Tuple_Args_Ret_w_Tuple_Captures_w_Extension)
#define __Thrd_fn_0(_fnName) \
    comp_syn__Thrd_fn_decl(_fnName)
#define __Thrd_fn_1(_fnName, _Tuple_Args_Ret) \
    comp_syn__Thrd_fn_decl_w_type(_fnName, _Tuple_Args_Ret)
#define __Thrd_fn_2(_fnName, _Tuple_Captures, _Extension) \
    pp_cat(comp_syn__Thrd_fn_impl, _Extension)(_fnName, _Tuple_Captures)
#define __Thrd_fn_3(_fnName, _Tuple_Args_Ret, _Tuple_Captures, _Extension) \
    pp_cat(comp_syn__Thrd_fn_impl_w_type, _Extension)(_fnName, _Tuple_Args_Ret, _Tuple_Captures)
#define $unscoped_Thrd_fn comp_syn__$unscoped_Thrd_fn
#define $unguarded_Thrd_fn comp_syn__$unguarded_Thrd_fn

// Thread functions
$extern fn_((Thrd_sleep(time_Duration duration))(void));
$extern fn_((Thrd_yield(void))(E$void)) $must_check;
$extern fn_((Thrd_currentId(void))(Thrd_Id));
$extern fn_((Thrd_cpuCount(void))(E$usize)) $must_check;

// Thread type
typedef struct Thrd {
    var_(inner, Thrd_FnCtx*);
} Thrd;
T_use_E$(Thrd);
$extern fn_((Thrd_handle(Thrd self))(Thrd_Handle));

// Thread name buffer type
typedef A$$(Thrd_max_name_len, u8) Thrd_NameBuf;
T_use_O$(Thrd_NameBuf);
T_use_E$(O$S_const$u8);
$extern fn_((Thrd_getName(Thrd self, Thrd_NameBuf* buf_ptr))(E$O$S_const$u8)) $must_check;
$extern fn_((Thrd_setName(Thrd self, S_const$u8 name))(E$void)) $must_check;

// Thread spawn configuration
typedef struct Thrd_SpawnCfg {
    var_(allocator, O$mem_Allocator);
    var_(stack_size, usize);
} Thrd_SpawnCfg;
#define Thrd_SpawnCfg_default_stack_size (16ull * 1024ull * 1024ull)
static const Thrd_SpawnCfg Thrd_SpawnCfg_default = {
    .allocator = none(),
    .stack_size = Thrd_SpawnCfg_default_stack_size,
};
$extern fn_((Thrd_spawn(Thrd_SpawnCfg cfg, Thrd_FnCtx* fn_ctx))(E$Thrd)) $must_check;
$extern fn_((Thrd_detach(Thrd self))(void));
$extern fn_((Thrd_join(Thrd self))(Thrd_FnCtx*));

// Mutex type
typedef struct Thrd_Mtx Thrd_Mtx;
// Mutex recursive type
typedef struct Thrd_Mtx_Recur Thrd_Mtx_Recur;
// Condition variable type
typedef struct Thrd_Cond Thrd_Cond;
// Read-Write lock type
typedef struct Thrd_RWLock Thrd_RWLock;

/*========== Macros and Definitions =========================================*/

#define comp_type_gen__use_Thrd_FnCtx$(_fnName, _Args, _T_Return) \
    typedef union Thrd_FnCtx$(_fnName) Thrd_FnCtx$(_fnName); \
    union Thrd_FnCtx$(_fnName) { \
        Thrd_FnCtx as_raw $like_ref; \
        struct { \
            struct { \
                Thrd_WorkFn fn; \
                Thrd_Handle handle; \
            }; \
            union { \
                struct { \
                    union { \
                        Thrd_FnArgs as_raw $like_ref; \
                        struct _Args as_typed; \
                    } args; \
                    union { \
                        Thrd_FnRet as_raw $like_ref; \
                        _T_Return as_typed; \
                    } ret; \
                } data; \
                struct { \
                    union { \
                        Thrd_FnArgs as_raw $like_ref; \
                        struct _Args as_typed; \
                    } args; \
                    union { \
                        Thrd_FnRet as_raw $like_ref; \
                        _T_Return as_typed; \
                    } ret; \
                }; \
            } \
        }; \
    }

#define __alias__Thrd_FnCtx$(_fnName...) pp_join($, Thrd_FnCtx, _fnName)

#define __step__Thrd_FnCtx_from$(...) \
    __step__Thrd_FnCtx_from$__emit(__VA_ARGS__)
#define __step__Thrd_FnCtx_from$__parseFnName(_fnName...) \
    _fnName,
#define __step__Thrd_FnCtx_from$__emit(_fnName, _args...) lit$((Thrd_FnCtx$(_fnName)){ \
    .fn = _fnName, \
    .args = { .as_typed = { __step__Thrd_FnCtx_from$__expand _args } }, \
    .ret = { .as_typed = {} }, \
})
#define __step__Thrd_FnCtx_from$__expand(_args...) \
    _args

#define __step__Thrd_FnCtx_ret$(...) \
    __step__Thrd_FnCtx_ret$__emit(__VA_ARGS__)
#define __step__Thrd_FnCtx_ret$__parseFnName(_fnName...) \
    _fnName,
#define __step__Thrd_FnCtx_ret$__emit(_fnName, _ctx...) \
    ((as$(Thrd_FnCtx$(_fnName)*)(_ctx))->ret.as_typed)

#define __step__Thrd_FnCtx_call$(...) \
    __step__Thrd_FnCtx_call$__emit(__VA_ARGS__)
#define __step__Thrd_FnCtx_call$__parseFnName(_fnName...) \
    _fnName,
#define __step__Thrd_FnCtx_call$__emit(_fnName, _args...) \
    Thrd_FnCtx_ret$((_fnName)(_fnName(Thrd_FnCtx_from$((_fnName)_args).as_raw)))

// clang-format off
#define comp_syn__Thrd_fn_decl(_fnName) \
Thrd_FnCtx* _fnName(Thrd_FnCtx* thrd_ctx)

#define comp_syn__Thrd_fn_decl_w_type(_fnName, _Tuple_Args_Ret) \
Thrd_FnCtx* _fnName(Thrd_FnCtx* thrd_ctx); \
typedef union Thrd_FnCtx$(_fnName) Thrd_FnCtx$(_fnName); \
union Thrd_FnCtx$(_fnName) { \
    Thrd_FnCtx as_raw $like_ref; \
    struct { \
        struct { \
            Thrd_WorkFn fn; \
            Thrd_Handle handle; \
        }; \
        union { \
            struct {\
                union { \
                    Thrd_FnArgs as_raw $like_ref; \
                    struct pp_Tuple_get1st _Tuple_Args_Ret as_typed; \
                } args; \
                union { \
                    Thrd_FnRet as_raw $like_ref; \
                    pp_Tuple_get2nd _Tuple_Args_Ret as_typed; \
                } ret; \
            } data; \
            struct { \
                union { \
                    Thrd_FnArgs as_raw $like_ref; \
                    struct pp_Tuple_get1st _Tuple_Args_Ret as_typed; \
                } args; \
                union { \
                    Thrd_FnRet as_raw $like_ref; \
                    pp_Tuple_get2nd _Tuple_Args_Ret as_typed; \
                } ret; \
            }; \
        }; \
    }; \
}

#define comp_syn__Thrd_fn_impl$scope(...) comp_syn__Thrd_fn_impl$_scope(__VA_ARGS__)
#define comp_syn__Thrd_fn_impl$_scope(_fnName, _Tuple_Captures) \
Thrd_FnCtx* _fnName(Thrd_FnCtx* thrd_ctx) { \
    let __passed_ctx = as$(Thrd_FnCtx$(_fnName)*)(thrd_ctx); \
    let __passed_args = __passed_ctx->args.as_typed; \
    let __passed_ret = &__passed_ctx->ret.as_typed; \
    let pp_Tuple_get1st _Tuple_Captures = __passed_ctx; \
    let pp_Tuple_get2nd _Tuple_Captures = &__passed_args; \
    let __reserved_return = as$(TypeOf(*__passed_ret)*)( \
        (u8[sizeOf$(TypeOf(*__passed_ret))]){} \
    ); \
    $maybe_unused typedef TypeOf(*__reserved_return) ReturnType; \
    $maybe_unused typedef ReturnType ReturnT; \
    if (false) { \
__step_return: goto __step_unscope; \
    } \
    do
#define comp_syn__$unscoped_Thrd_fn \
while (false); \
    if (false) { \
__step_unscope: return *__passed_ret = *__reserved_return, __passed_ctx->as_raw; \
    } \
}

#define comp_syn__Thrd_fn_impl$guard(...) comp_syn__Thrd_fn_impl$_guard(__VA_ARGS__)
#define comp_syn__Thrd_fn_impl$_guard(_fnName, _Tuple_Captures) \
Thrd_FnCtx* _fnName(Thrd_FnCtx* thrd_ctx) { \
    let __passed_ctx = as$(Thrd_FnCtx$(_fnName)*)(thrd_ctx); \
    let __passed_args = __passed_ctx->args.as_typed; \
    let __passed_ret = &__passed_ctx->ret.as_typed; \
    let pp_Tuple_get1st _Tuple_Captures = __passed_ctx; \
    let pp_Tuple_get2nd _Tuple_Captures = &__passed_args; \
    let __reserved_return = as$(TypeOf(*__passed_ret)*)( \
        (u8[sizeOf$(TypeOf(*__passed_ret))]){} \
    ); \
    $maybe_unused typedef TypeOf(*__reserved_return) ReturnType; \
    $maybe_unused typedef ReturnType ReturnT; \
    var __scope_counter   = (struct fn__ScopeCounter){ \
        .is_returning = false, .current_line = __LINE__ \
    }; \
    if (false) { \
__step_return: \
        __scope_counter.is_returning = true; \
        goto __step_deferred; \
    } \
__step_deferred: \
    switch (__scope_counter.current_line) { \
    default: { goto __step_unscope; } break; \
    case __LINE__: __scope_counter.current_line = __LINE__ - 1;
#define comp_syn__$unguarded_Thrd_fn \
break; \
    } \
    if (false) { \
__step_unscope: return *__passed_ret = *__reserved_return, __passed_ctx->as_raw; \
    } \
}

#define comp_syn__Thrd_fn_impl_w_type$_scope(_fnName, _Tuple_Args_Ret, _Tuple_Captures) \
comp_syn__Thrd_fn_decl_w_type(_fnName, _Tuple_Args_Ret); \
comp_syn__Thrd_fn_impl$scope(_fnName, _Tuple_Captures)

#define comp_syn__Thrd_fn_impl_w_type$_guard(_fnName, _Tuple_Args_Ret, _Tuple_Captures) \
comp_syn__Thrd_fn_decl_w_type(_fnName, _Tuple_Args_Ret); \
comp_syn__Thrd_fn_impl$guard(_fnName, _Tuple_Captures)
// clang-format on

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Thrd_common__included */
