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

#ifndef THRD_COMMON_INCLUDED
#define THRD_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/mem/Allocator.h"
#include "dh/time/common.h"

/*========== Macros and Definitions =========================================*/

// Thread ID type
typedef Thrd_IdImpl     Thrd_Id;
// Thread handle type
typedef Thrd_HandleImpl Thrd_Handle;

// Thread function context type instantiation
#define use_Thrd_FnCtx$(_fnName, _Args, _T_Return) comp_type_gen__use_Thrd_FnCtx$(_fnName, _Args, _T_Return)
// Thread function context type alias
#define Thrd_FnCtx$(_fnName)                       comp_type_alias__Thrd_FnCtx$(_fnName)
// Thread function context type erasure
typedef struct Thrd_FnCtx Thrd_FnCtx;
// Thread function arguments type erasure
typedef struct Thrd_FnArgs {
    u8 data[0];
} Thrd_FnArgs;
// Thread function return type erasure
typedef struct Thrd_FnRet {
    u8 data[0];
} Thrd_FnRet;
// Thread function type
typedef fn_(((*Thrd_WorkFn)(Thrd_FnCtx* thrd_ctx))(Thrd_FnCtx*));
struct Thrd_FnCtx {
    Thrd_WorkFn fn;
    union {
        Thrd_FnArgs args[0];
        Thrd_FnRet  ret[0];
        u8          data[0];
    };
};
#define Thrd_FnCtx_from(_fnName, _Args...)    comp_inline__Thrd_FnCtx_from(_fnName, _Args)
#define Thrd_FnCtx_returned(_fnName, _ctx...) comp_inline__Thrd_FnCtx_returned(_fnName, _ctx)

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
#define $unscoped_Thrd_fn  comp_syn__$unscoped_Thrd_fn
#define $unguarded_Thrd_fn comp_syn__$unguarded_Thrd_fn

// Thread functions
extern fn_((Thrd_sleep(time_Duration duration))(void));
extern fn_((Thrd_yield(void))(Err$void)) $must_check;
extern fn_((Thrd_getCurrentId(void))(Thrd_Id));
extern fn_((Thrd_getCpuCount(void))(Err$usize)) $must_check;

// Thread type
typedef struct Thrd {
    Thrd_Handle handle;
} Thrd;
use_Err$(Thrd);
extern fn_((Thrd_getHandle(Thrd self))(Thrd_Handle));

// Thread name buffer type
typedef Arr$$(Thrd_max_name_len, u8) Thrd_NameBuf;
use_Opt$(Thrd_NameBuf);
use_Err$(Opt$Sli_const$u8);
extern fn_((Thrd_getName(Thrd self, Thrd_NameBuf* buf_ptr))(Err$Opt$Sli_const$u8)) $must_check;
extern fn_((Thrd_setName(Thrd self, Sli_const$u8 name))(Err$void)) $must_check;

// Thread spawn configuration
typedef struct Thrd_SpawnConfig {
    usize             stack_size;
    Opt$mem_Allocator allocator;
} Thrd_SpawnConfig;
#define Thrd_SpawnConfig_default_stack_size (16ull * 1024ull * 1024ull)
static const Thrd_SpawnConfig Thrd_SpawnConfig_default = {
    .stack_size = Thrd_SpawnConfig_default_stack_size,
    .allocator  = none()
};
extern fn_((Thrd_spawn(Thrd_SpawnConfig config, Thrd_FnCtx* fn_ctx))(Err$Thrd)) $must_check;
extern fn_((Thrd_detach(Thrd self))(void));
extern fn_((Thrd_join(Thrd self))(Thrd_FnCtx*));

// Mutex type
typedef struct Thrd_Mtx    Thrd_Mtx;
// Condition variable type
typedef struct Thrd_Cond   Thrd_Cond;
// Read-write lock type
typedef struct Thrd_RwLock Thrd_RwLock;

/*========== Macros and Definitions =========================================*/

#define comp_type_gen__use_Thrd_FnCtx$(_fnName, _Args, _T_Return) \
    typedef union Thrd_FnCtx$(_fnName) Thrd_FnCtx$(_fnName); \
    union Thrd_FnCtx$(_fnName) { \
        Thrd_FnCtx base[1]; \
        struct { \
            Thrd_WorkFn fn; \
            union { \
                Thrd_FnArgs  base[1]; \
                struct _Args typed; \
            } args; \
            union { \
                Thrd_FnRet base[1]; \
                _T_Return  typed; \
            } ret; \
        }; \
    }
#define comp_type_alias__Thrd_FnCtx$(_fnName) pp_join($, Thrd_FnCtx, _fnName)
#define comp_inline__Thrd_FnCtx_from(_fnName, _Args...) \
    ((Thrd_FnCtx$(_fnName)){ \
        .fn   = _fnName, \
        .args = { .typed = _Args }, \
        .ret  = { .typed = {} }, \
    })
#define comp_inline__Thrd_FnCtx_returned(_fnName, _ctx...) \
    as$((Thrd_FnCtx$(_fnName)*)(_ctx))->ret.typed

// clang-format off
#define comp_syn__Thrd_fn_decl(_fnName) \
Thrd_FnCtx* _fnName(Thrd_FnCtx* thrd_ctx)

#define comp_syn__Thrd_fn_decl_w_type(_fnName, _Tuple_Args_Ret) \
Thrd_FnCtx* _fnName(Thrd_FnCtx* thrd_ctx); \
typedef union Thrd_FnCtx$(_fnName) Thrd_FnCtx$(_fnName); \
union Thrd_FnCtx$(_fnName) { \
    Thrd_FnCtx base[1]; \
    struct { \
        Thrd_WorkFn fn;\
        union { \
            Thrd_FnArgs  base[1]; \
            struct pp_Tuple_get1st _Tuple_Args_Ret typed; \
        } args; \
        union { \
            Thrd_FnRet base[1]; \
            pp_Tuple_get2nd _Tuple_Args_Ret typed; \
        } ret; \
    }; \
}

#define comp_syn__Thrd_fn_impl$scope(_fnName, _Tuple_Captures) \
Thrd_FnCtx* _fnName(Thrd_FnCtx* thrd_ctx) { \
    let __passed_ctx = as$((Thrd_FnCtx$(_fnName)*)(thrd_ctx)); \
    let __passed_args = __passed_ctx->args.typed; \
    let __passed_ret = &__passed_ctx->ret.typed; \
    let pp_Tuple_get1st _Tuple_Captures = __passed_ctx; \
    let pp_Tuple_get2nd _Tuple_Captures = &__passed_args; \
    let __reserved_return = as$((TypeOf(*__passed_ret)*)( \
        (u8[sizeOf$(TypeOf(*__passed_ret))]){} \
    )); \
    if (false) { \
__step_return: goto __step_unscope; \
    } \
    do
#define comp_syn__$unscoped_Thrd_fn \
while (false); \
    if (false) { \
__step_unscope: return *__passed_ret = *__reserved_return, __passed_ctx->base; \
    } \
}

#define comp_syn__Thrd_fn_impl$guard(_fnName, _Tuple_Captures) \
Thrd_FnCtx* _fnName(Thrd_FnCtx* thrd_ctx) { \
    let __passed_ctx = as$((Thrd_FnCtx$(_fnName)*)(thrd_ctx)); \
    let __passed_args = __passed_ctx->args.typed; \
    let __passed_ret = &__passed_ctx->ret.typed; \
    let pp_Tuple_get1st _Tuple_Captures = __passed_ctx; \
    let pp_Tuple_get2nd _Tuple_Captures = &__passed_args; \
    let __reserved_return = as$((TypeOf(*__passed_ret)*)( \
        (u8[sizeOf$(TypeOf(*__passed_ret))]){} \
    )); \
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
__step_unscope: return *__passed_ret = *__reserved_return, __passed_ctx->base; \
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
#endif /* THRD_COMMON_INCLUDED */
