/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-29 (date of creation)
 * @updated 2025-03-19 (date of last update)
 * @version v0.1-alpha.4
 * @ingroup dasae-headers(dh)/meta
 * @prefix
 *
 * @brief   Common meta utilities
 * @details Provides common meta utilities and functions for:
 *          - Meta type creation
 *          - Meta type casting
 *          - Meta type conversion
 *          - Meta type comparison
 *          - Meta type utility functions
 */

#ifndef META_COMMON_INCLUDED
#define META_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"

#include "dh/ptr.h"
#include "dh/sli.h"
#include "dh/AnyType.h"

/*========== Meta Utilities =================================================*/

typedef struct meta_Ptr_const meta_Ptr_const;
typedef union meta_Ptr        meta_Ptr;

typedef struct meta_Sli_const meta_Sli_const;
typedef union meta_Sli        meta_Sli;

extern fn_((meta_Ptr_constCast(meta_Ptr_const))(meta_Ptr));
extern fn_((meta_Ptr_mutCast(meta_Ptr))(meta_Ptr_const));
extern fn_((meta_Ptr_toSli(meta_Ptr, TypeInfo type))(meta_Sli));

extern fn_((meta_Ptr_copy(meta_Ptr, meta_Ptr_const))(meta_Ptr));
extern fn_((meta_Ptr_move(meta_Ptr, meta_Ptr))(meta_Ptr));

extern fn_((meta_Sli_constCast(meta_Sli_const))(meta_Sli));
extern fn_((meta_Sli_mutCast(meta_Sli))(meta_Sli_const));
extern fn_((meta_Sli_asPtr(meta_Sli))(meta_Ptr));
extern fn_((meta_Sli_len(meta_Sli))(usize));
extern fn_((meta_Sli_at(meta_Sli, usize index))(meta_Ptr));
extern fn_((meta_Sli_slice(meta_Sli, usize begin, usize end))(meta_Sli));

extern fn_((meta_Sli_set(meta_Sli, meta_Ptr_const))(meta_Sli));
extern fn_((meta_Sli_copy(meta_Sli, meta_Sli_const))(meta_Sli));
extern fn_((meta_Sli_move(meta_Sli, meta_Sli))(meta_Sli));

#define Opt_some$(T_Lit) pp_defer(some$) pp_Tuple_cat(T_Lit)

#define literal$(T_Lit...) pp_defer(comp_op__literal$) pp_Tuple_cat(T_Lit)
#define comp_op__literal$(T_Lit, _Initial...) \
    ((T_Lit)_Initial)

#define meta_create$(T_Lit_Initial...) pp_expand(pp_defer(comp_op__meta_create$)(pp_Tuple_unwrapSufCommaExpand T_Lit_Initial))
#define meta_refPtr(var_ptr...)        comp_op__meta_refPtr(var_ptr)
#define meta_refSli(var_sli...)        comp_op__meta_refSli(var_sli)

#define meta_cast$(T_Dest, var_meta...)              comp_op__meta_cast$(T_Dest, var_meta)
#define meta_castPtr$(T_DestPtr, var_meta_ptr...)    comp_op__meta_castPtr$(T_DestPtr, var_meta_ptr)
#define meta_castSli$(T_DestSli, var_meta_sli...)    comp_op__meta_castSli$(T_DestSli, var_meta_sli)
#define meta_castOpt$(T_DestOpt, var_meta_opt...)    comp_op__meta_castOpt$(T_DestOpt, var_meta_opt)
#define meta_castErr$(T_DestErrRes, var_meta_err...) comp_op__meta_castErr$(T_DestErrRes, var_meta_err)

#define meta_ptrToAny(var_meta_ptr...) comp_op__meta_ptrToAny(var_meta_ptr)
#define meta_sliToAny(var_meta_sli...) comp_op__meta_sliToAny(var_meta_sli)

/* Implementation meta types */
struct meta_Ptr_const {
    TypeInfo     type; // Type info first
    anyptr_const addr; // Then address
};
union meta_Ptr {
    struct {
        TypeInfo type; // Type info first
        anyptr   addr; // Then address
    };
    meta_Ptr_const as_const;
};

struct meta_Sli_const {
    TypeInfo     type; // Type info first
    anyptr_const addr; // Then address
    usize        len;  // Then length
};
union meta_Sli {
    struct {
        TypeInfo type; // Type info first
        anyptr   addr; // Then address
        usize    len;  // Then length
    };
    meta_Sli_const as_const;
};

#define comp_op__meta_create$(T_Lit, _Initial...) meta_refPtr(literal$(T_Lit[1])({ [0] = _Initial }))
#define comp_op__meta_refPtr(var_ptr...)          ((meta_Ptr){ \
    .type = typeInfo$(TypeOf(*var_ptr)), \
    .addr = var_ptr, \
})
#define comp_op__meta_refSli(var_sli...) eval({ \
    let __sli = var_sli; \
    eval_return((meta_Sli){ \
        .type = typeInfo$(TypeOf(*__sli.ptr)), \
        .addr = __sli.ptr, \
        .len  = __sli.len, \
    }); \
})

#define comp_op__meta_cast$(T_Dest, var_meta...) eval({ \
    TypeOf(var_meta) _meta = var_meta; \
    claim_assert_static_msg( \
        isSameType$(TypeOf(_meta), meta_Ptr_const) || isSameType$(TypeOf(_meta), meta_Ptr) \
            || isSameType$(TypeOf(_meta), meta_Sli_const) || isSameType$(TypeOf(_meta), meta_Sli), \
        "Invalid meta type" \
    ); \
    eval_return(*((T_Dest*)&_meta.addr)); \
})
#define comp_op__meta_castPtr$(T_DestPtr, var_meta_ptr...) eval({ \
    const TypeOf(var_meta_ptr) _ptr = var_meta_ptr; \
    claim_assert_static_msg(isSameType$(TypeOf(_ptr), meta_Ptr), "Invalid meta type"); \
    eval_return((T_DestPtr)_ptr.addr); \
})
#define comp_op__meta_castSli$(T_DestSli, var_meta_sli...) eval({ \
    const TypeOf(var_meta_sli) _sli = var_meta_sli; \
    claim_assert_static_msg(isSameType$(TypeOf(_sli), meta_Sli), "Invalid meta type"); \
    eval_return((T_DestSli){ .ptr = _sli.addr, .len = _sli.len }); \
})
#define comp_op__meta_castOpt$(T_DestOpt, var_meta_opt...) eval({ \
    const TypeOf(var_meta_opt) __opt = var_meta_opt; \
    T_DestOpt __result               = cleared(); \
    if (isNone(__opt)) { \
        toNone(&__result); \
    } else { \
        toSome(&__result, __opt.value.addr); \
    } \
    eval_return __result; \
})
#define comp_op__meta_castErr$(T_DestErrRes, var_meta_err...) eval({ \
    const TypeOf(var_meta_err) __err_res = var_meta_err; \
    T_DestErrRes __result                = cleared(); \
    if (isOk(__err_res)) { \
        toOk(&__result, __err_res.data.ok.addr); \
    } else { \
        toErr(&__result, __err_res.data.err); \
    } \
    eval_return __result; \
})

#define comp_op__meta_ptrToAny(var_meta_ptr) eval({ \
    const TypeOf(var_meta_ptr) _ptr = var_meta_ptr; \
    claim_assert_static_msg(isSameType$(TypeOf(_ptr), meta_Ptr), "Invalid meta type"); \
    eval_return variant_of$(AnyType, AnyType_ptr, { .type = _ptr.type, .addr = _ptr.addr }); \
})
#define comp_op__meta_sliToAny(var_meta_sli) eval({ \
    const TypeOf(var_meta_sli) _sli = var_meta_sli; \
    claim_assert_static_msg(isSameType$(TypeOf(_sli), meta_Sli), "Invalid meta type"); \
    eval_return variant_of$(AnyType, AnyType_sli, { .type = _sli.type, .addr = _sli.addr, .len = _sli.len }); \
})

// clang-format off
use_Opt$(Ptr_const); use_Opt$(Ptr);
use_Err$(Ptr_const); use_Err$(Ptr);

use_Opt$(Sli_const); use_Opt$(Sli);
use_Err$(Sli_const); use_Err$(Sli);

use_Opt$(meta_Ptr_const); use_Opt$(meta_Ptr);
use_Err$(meta_Ptr_const); use_Err$(meta_Ptr);

use_Opt$(meta_Sli_const); use_Opt$(meta_Sli);
use_Err$(meta_Sli_const); use_Err$(meta_Sli);

// /* builtin types */
// use_Ptr$(u8); use_Ptr$(u16); use_Ptr$(u32); use_Ptr$(u64); use_Ptr$(usize);
// use_Sli$(u8); use_Sli$(u16); use_Sli$(u32); use_Sli$(u64); use_Sli$(usize);
// use_Opt$(u8); use_Opt$(u16); use_Opt$(u32); use_Opt$(u64); use_Opt$(usize);
// use_Err$(u8); use_Err$(u16); use_Err$(u32); use_Err$(u64); use_Err$(usize);

// use_Ptr$(i8); use_Ptr$(i16); use_Ptr$(i32); use_Ptr$(i64); use_Ptr$(isize);
// use_Sli$(i8); use_Sli$(i16); use_Sli$(i32); use_Sli$(i64); use_Sli$(isize);
// use_Opt$(i8); use_Opt$(i16); use_Opt$(i32); use_Opt$(i64); use_Opt$(isize);
// use_Err$(i8); use_Err$(i16); use_Err$(i32); use_Err$(i64); use_Err$(isize);

// use_Ptr$(f32); use_Ptr$(f64);
// use_Sli$(f32); use_Sli$(f64);
// use_Opt$(f32); use_Opt$(f64);
// use_Err$(f32); use_Err$(f64);

// use_Ptr$(bool); use_Ptr$(char);
// use_Sli$(bool); use_Sli$(char);
// use_Opt$(bool); use_Opt$(char);
// use_Err$(bool); use_Err$(char);
// clang-format on

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* META_COMMON_INCLUDED */
