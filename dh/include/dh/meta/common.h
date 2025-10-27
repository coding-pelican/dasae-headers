#if UNUSED_CODE
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

typedef struct meta_P_const$raw meta_P_const$raw;
typedef union meta_P$raw        meta_P$raw;

typedef struct meta_S_const$raw meta_S_const$raw;
typedef union meta_S$raw        meta_S$raw;

extern fn_((meta_constCast$P(meta_P_const$raw))(meta_P$raw));
extern fn_((meta_mutCast$P(meta_P$raw))(meta_P_const$raw));
extern fn_((meta_P_toSli(meta_P$raw, TypeInfo type))(meta_S$raw));

extern fn_((meta_P_copy(meta_P$raw, meta_P_const$raw))(meta_P$raw));
extern fn_((meta_P_move(meta_P$raw, meta_P$raw))(meta_P$raw));

extern fn_((meta_S_constCast(meta_S_const$raw))(meta_S$raw));
extern fn_((meta_S_mutCast(meta_S$raw))(meta_S_const$raw));
extern fn_((meta_S_asPtr(meta_S$raw))(meta_P$raw));
extern fn_((meta_S_len(meta_S$raw))(usize));
extern fn_((meta_S_at(meta_S$raw, usize index))(meta_P$raw));
extern fn_((meta_S_slice(meta_S$raw, usize begin, usize end))(meta_S$raw));

extern fn_((meta_S_set(meta_S$raw, meta_P_const$raw))(meta_S$raw));
extern fn_((meta_S_copy(meta_S$raw, meta_S_const$raw))(meta_S$raw));
extern fn_((meta_S_move(meta_S$raw, meta_S$raw))(meta_S$raw));

#define O_some$(T_Lit) pp_defer(some$) pp_Tuple_cat(T_Lit)

#define literal$(T_Lit...) pp_defer(comp_op__literal$) pp_Tuple_cat(T_Lit)
#define comp_op__literal$(T_Lit, _Initial...) \
    ((T_Lit)_Initial)

#define meta_create$(T_Lit_Initial...) pp_expand(pp_defer(comp_op__meta_create$)(pp_Tuple_unwrapSufCommaExpand T_Lit_Initial))
#define meta_refPtr(var_ptr...)        comp_op__meta_refPtr(var_ptr)
#define meta_refSli(var_sli...)        comp_op__meta_refSli(var_sli)

#define meta_cast$(T_Dest, var_meta...)            comp_op__meta_cast$(T_Dest, var_meta)
#define meta_castP$(T_DestPtr, var_meta_ptr...)    comp_op__meta_castP$(T_DestPtr, var_meta_ptr)
#define meta_castS$(T_DestSli, var_meta_sli...)    comp_op__meta_castS$(T_DestSli, var_meta_sli)
#define meta_castO$(T_DestOpt, var_meta_opt...)    comp_op__meta_castO$(T_DestOpt, var_meta_opt)
#define meta_castE$(T_DestErrRes, var_meta_err...) comp_op__meta_castE$(T_DestErrRes, var_meta_err)

#define meta_ptrToAny(var_meta_ptr...) comp_op__meta_ptrToAny(var_meta_ptr)
#define meta_sliToAny(var_meta_sli...) comp_op__meta_sliToAny(var_meta_sli)

/* Implementation meta types */
struct meta_P_const$raw {
    TypeInfo    type; // Type info first
    P_const$raw addr; // Then address
};
union meta_P$raw {
    struct {
        TypeInfo type; // Type info first
        P$raw    addr; // Then address
    };
    meta_P_const$raw as_const;
};

struct meta_S_const$raw {
    TypeInfo    type; // Type info first
    P_const$raw addr; // Then address
    usize       len;  // Then length
};
union meta_S$raw {
    struct {
        TypeInfo type; // Type info first
        P$raw    addr; // Then address
        usize    len;  // Then length
    };
    meta_S_const$raw as_const;
};

#define comp_op__meta_create$(T_Lit, _Initial...) meta_refPtr(literal$(T_Lit[1])({ [0] = _Initial }))
#define comp_op__meta_refPtr(var_ptr...)          ((meta_P$raw){ \
    .type = typeInfo$(TypeOf(*var_ptr)), \
    .addr = var_ptr, \
})
#define comp_op__meta_refSli(var_sli...) blk({ \
    let __sli = var_sli; \
    blk_return((meta_S$raw){ \
        .type = typeInfo$(TypeOf(*__sli.ptr)), \
        .addr = __sli.ptr, \
        .len  = __sli.len, \
    }); \
})

#define comp_op__meta_cast$(T_Dest, var_meta...) blk({ \
    TypeOf(var_meta) _meta = var_meta; \
    claim_assert_static_msg( \
        isSameType$(TypeOf(_meta), meta_P_const$raw) || isSameType$(TypeOf(_meta), meta_P$raw) \
            || isSameType$(TypeOf(_meta), meta_S_const$raw) || isSameType$(TypeOf(_meta), meta_S$raw), \
        "Invalid meta type" \
    ); \
    blk_return(*((T_Dest*)&_meta.addr)); \
})
#define comp_op__meta_castP$(T_DestPtr, var_meta_ptr...) blk({ \
    const TypeOf(var_meta_ptr) _ptr = var_meta_ptr; \
    claim_assert_static_msg(isSameType$(TypeOf(_ptr), meta_P$raw), "Invalid meta type"); \
    blk_return((T_DestPtr)_ptr.addr); \
})
#define comp_op__meta_castS$(T_DestSli, var_meta_sli...) blk({ \
    const TypeOf(var_meta_sli) _sli = var_meta_sli; \
    claim_assert_static_msg(isSameType$(TypeOf(_sli), meta_S$raw), "Invalid meta type"); \
    blk_return((T_DestSli){ .ptr = _sli.addr, .len = _sli.len }); \
})
#define comp_op__meta_castO$(T_DestOpt, var_meta_opt...) blk({ \
    const TypeOf(var_meta_opt) __opt = var_meta_opt; \
    T_DestOpt __result               = cleared(); \
    if (isNone(__opt)) { \
        toNone(&__result); \
    } else { \
        toSome(&__result, __opt.value.addr); \
    } \
    blk_return __result; \
})
#define comp_op__meta_castE$(T_DestErrRes, var_meta_err...) blk({ \
    const TypeOf(var_meta_err) __err_res = var_meta_err; \
    T_DestErrRes __result                = cleared(); \
    if (isOk(__err_res)) { \
        toOk(&__result, __err_res.data.ok.addr); \
    } else { \
        toErr(&__result, __err_res.data.err); \
    } \
    blk_return __result; \
})

#define comp_op__meta_ptrToAny(var_meta_ptr) blk({ \
    const TypeOf(var_meta_ptr) _ptr = var_meta_ptr; \
    claim_assert_static_msg(isSameType$(TypeOf(_ptr), meta_P$raw), "Invalid meta type"); \
    blk_return variant_of$(AnyType, AnyType_ptr, { .type = _ptr.type, .addr = _ptr.addr }); \
})
#define comp_op__meta_sliToAny(var_meta_sli) blk({ \
    const TypeOf(var_meta_sli) _sli = var_meta_sli; \
    claim_assert_static_msg(isSameType$(TypeOf(_sli), meta_S$raw), "Invalid meta type"); \
    blk_return variant_of$(AnyType, AnyType_sli, { .type = _sli.type, .addr = _sli.addr, .len = _sli.len }); \
})

// clang-format off
T_use_O$(P_const$raw); T_use_O$(P$raw);
T_use_E$(P_const$raw); T_use_E$(P$raw);

T_use_O$(S_const$raw); T_use_O$(S$raw);
T_use_E$(S_const$raw); T_use_E$(S$raw);

T_use_O$(meta_P_const$raw); T_use_O$(meta_P$raw);
T_use_E$(meta_P_const$raw); T_use_E$(meta_P$raw);

T_use_O$(meta_S_const$raw); T_use_O$(meta_S$raw);
T_use_E$(meta_S_const$raw); T_use_E$(meta_S$raw);

// /* builtin types */
// use_P$(u8); use_P$(u16); use_P$(u32); use_P$(u64); use_P$(usize);
// use_S$(u8); use_S$(u16); use_S$(u32); use_S$(u64); use_S$(usize);
// use_O$(u8); use_O$(u16); use_O$(u32); use_O$(u64); use_O$(usize);
// use_E$(u8); use_E$(u16); use_E$(u32); use_E$(u64); use_E$(usize);

// use_P$(i8); use_P$(i16); use_P$(i32); use_P$(i64); use_P$(isize);
// use_S$(i8); use_S$(i16); use_S$(i32); use_S$(i64); use_S$(isize);
// use_O$(i8); use_O$(i16); use_O$(i32); use_O$(i64); use_O$(isize);
// use_E$(i8); use_E$(i16); use_E$(i32); use_E$(i64); use_E$(isize);

// use_P$(f32); use_P$(f64);
// use_S$(f32); use_S$(f64);
// use_O$(f32); use_O$(f64);
// use_E$(f32); use_E$(f64);

// use_P$(bool); use_P$(char);
// use_S$(bool); use_S$(char);
// use_O$(bool); use_O$(char);
// use_E$(bool); use_E$(char);
// clang-format on

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* META_COMMON_INCLUDED */

#endif /* UNUSED_CODE */
