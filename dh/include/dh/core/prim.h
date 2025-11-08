/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    prim.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  prim
 *
 * @brief   Primitive types and operations
 * @details Provides primitive types and operations for:
 *          - Bitwise operations
 *          - Boolean operations
 *          - Integer operations
 *          - Floating-point operations
 *          - Pointer operations
 */

#ifndef core_prim__included
#define core_prim__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prim/cfg.h"

#include "prim/bool.h"
#include "prim/int.h"
#include "prim/flt.h"
#include "prim/ptr.h"
#include "prim/null.h"
#include "prim/wchar.h"

#include "prim/struct.h"
#include "prim/union.h"
#include "prim/switch.h"

/*========== Macros and Definitions =========================================*/

#define prim_cmp(val_lhs, val_rhs) comp_inline__prim_cmp(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_eq(val_lhs, val_rhs)  comp_inline__prim_eq(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_ne(val_lhs, val_rhs)  comp_inline__prim_ne(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_lt(val_lhs, val_rhs)  comp_inline__prim_lt(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_gt(val_lhs, val_rhs)  comp_inline__prim_gt(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_le(val_lhs, val_rhs)  comp_inline__prim_le(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_ge(val_lhs, val_rhs)  comp_inline__prim_ge(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)

#define prim_min(val_lhs, val_rhs)                    comp_inline__prim_min2(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_min2(val_lhs, val_rhs)                   comp_inline__prim_min2(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_min3(val_1st, val_2nd, val_3rd)          comp_inline__prim_min3(pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), val_1st, val_2nd, val_3rd)
#define prim_min4(val_1st, val_2nd, val_3rd, val_4th) comp_inline__prim_min4(pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), pp_uniqTok(4th), val_1st, val_2nd, val_3rd, val_4th)
#define prim_max(val_lhs, val_rhs)                    comp_inline__prim_max2(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_max2(val_lhs, val_rhs)                   comp_inline__prim_max2(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_max3(val_1st, val_2nd, val_3rd)          comp_inline__prim_max3(pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), val_1st, val_2nd, val_3rd)
#define prim_max4(val_1st, val_2nd, val_3rd, val_4th) comp_inline__prim_max4(pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), pp_uniqTok(4th), val_1st, val_2nd, val_3rd, val_4th)
#define prim_clamp(val_x, val_min, val_max)           comp_inline__prim_clamp(val_x, val_min, val_max)
#define prim_wrap(val_x, val_min, val_max)            comp_inline__prim_wrap(val_x, val_min, val_max)

#define prim_swap(val_lhs, val_rhs)                          __op__prim_swap(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(tmp), val_lhs, val_rhs)
#define prim_memset(val_ptr, i32_value, usize_len...)        __op__prim_memset(val_ptr, i32_value, usize_len)
#define prim_memcpy(val_dst_ptr, val_src_ptr, usize_len...)  __op__prim_memcpy(val_dst_ptr, val_src_ptr, usize_len)
#define prim_memmove(val_dst_ptr, val_src_ptr, usize_len...) __op__prim_memmove(val_dst_ptr, val_src_ptr, usize_len)
#define prim_memcmp(val_lhs_ptr, val_rhs_ptr, usize_len...)  __op__prim_memcmp(val_lhs_ptr, val_rhs_ptr, usize_len)
#define prim_memchr(val_ptr, i32_value, usize_len...)        __op__prim_memchr(val_ptr, i32_value, usize_len)

#define prim_divisible(val_lhs, val_rhs) comp_inline__prim_divisible(val_lhs, val_rhs)
#define prim_sign(val_x)                 comp_inline__prim_sign(val_x)

typedef struct Void {
    u8 _bytes_zero[0];
} Void;

/*========== Macros Implementation ==========================================*/

#define comp_inline__prim_cmp(__lhs, __rhs, val_lhs, val_rhs) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    blk_return( \
        ((__lhs) < (__rhs))   ? -1 /* cmp_Ord_lt */ \
        : ((__lhs) > (__rhs)) ? 1  /* cmp_Ord_gt */ \
                              : 0  /* cmp_Ord_eq */ \
    ); \
})
#define comp_inline__prim_eq(__lhs, __rhs, val_lhs, val_rhs) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    blk_return __lhs == (__rhs); \
})
#define comp_inline__prim_ne(__lhs, __rhs, val_lhs, val_rhs) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    blk_return __lhs != (__rhs); \
})
#define comp_inline__prim_lt(__lhs, __rhs, val_lhs, val_rhs) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    blk_return __lhs < (__rhs); \
})
#define comp_inline__prim_gt(__lhs, __rhs, val_lhs, val_rhs) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    blk_return __lhs > (__rhs); \
})
#define comp_inline__prim_le(__lhs, __rhs, val_lhs, val_rhs) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    blk_return __lhs <= (__rhs); \
})
#define comp_inline__prim_ge(__lhs, __rhs, val_lhs, val_rhs) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    blk_return __lhs >= (__rhs); \
})

#define comp_inline__prim_min2(__lhs, __rhs, val_lhs, val_rhs...) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    blk_return __rhs < __lhs ? __rhs : __lhs; \
})
#define comp_inline__prim_min3(__1st, __2nd, __3rd, val_1st, val_2nd, val_3rd...) blk({ \
    let __1st = (val_1st); \
    let __2nd = (val_2nd); \
    let __3rd = (val_3rd); \
    blk_return prim_min(prim_min(__1st, __2nd), __3rd); \
})
#define comp_inline__prim_min4(__1st, __2nd, __3rd, __4th, val_1st, val_2nd, val_3rd, val_4th...) blk({ \
    let __1st = (val_1st); \
    let __2nd = (val_2nd); \
    let __3rd = (val_3rd); \
    let __4th = (val_4th); \
    blk_return prim_min(prim_min(prim_min(__1st, __2nd), __3rd), __4th); \
})
#define comp_inline__prim_max2(__lhs, __rhs, val_lhs, val_rhs...) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    blk_return __rhs > __lhs ? __rhs : __lhs; \
})
#define comp_inline__prim_max3(__1st, __2nd, __3rd, val_1st, val_2nd, val_3rd...) blk({ \
    let __1st = (val_1st); \
    let __2nd = (val_2nd); \
    let __3rd = (val_3rd); \
    blk_return prim_max(prim_max(__1st, __2nd), __3rd); \
})
#define comp_inline__prim_max4(__1st, __2nd, __3rd, __4th, val_1st, val_2nd, val_3rd, val_4th...) blk({ \
    let __1st = (val_1st); \
    let __2nd = (val_2nd); \
    let __3rd = (val_3rd); \
    let __4th = (val_4th); \
    blk_return prim_max(prim_max(prim_max(__1st, __2nd), __3rd), __4th); \
})
#define comp_inline__prim_clamp(val_x, val_min, val_max) \
    prim_min(prim_max(val_x, val_min), val_max)

#define __op__prim_swap(__lhs, __rhs, __tmp, val_lhs, val_rhs) blk({ \
    let __lhs = &(val_lhs); \
    let __rhs = &(val_rhs); \
    let __tmp = *(__lhs); \
    *(__lhs) = *(__rhs); \
    *(__rhs) = __tmp; \
    blk_return{}; \
})
#define __op__prim_memset(val_ptr, i32_value, usize_len...)        memset(val_ptr, i32_value, usize_len)
#define __op__prim_memcpy(val_dst_ptr, val_src_ptr, usize_len...)  memcpy(val_dst_ptr, val_src_ptr, usize_len)
#define __op__prim_memmove(val_dst_ptr, val_src_ptr, usize_len...) memmove(val_dst_ptr, val_src_ptr, usize_len)
#define __op__prim_memcmp(val_lhs_ptr, val_rhs_ptr, usize_len...)  memcmp(val_lhs_ptr, val_rhs_ptr, usize_len)
#define __op__prim_memchr(val_ptr, i32_value, usize_len...)        memchr(val_ptr, i32_value, usize_len)

#define comp_inline__prim_divisible(val_lhs, val_rhs) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    blk_return((__rhs == 0) ? 0 : (__lhs % __rhs == 0)); \
})
#define comp_inline__prim_sign(val_x) blk({ \
    let __scalar = (val_x); \
    blk_return as$((TypeOf(__scalar))(__scalar < 0 ? -1 : (__scalar > 0 ? 1 : 0))); \
})

#if NEXT_UPDATE
#define let_ignore \
    __attribute__((unused)) let pp_uniqTok(ignored)
#define let_ignore \
    __attribute__((unused)) pp_uniqTok(ignored) = (Void){}; \
    let_ignore
#define $ignore_void (void)

#define as$(/*(T)(Expr)*/...)           pp_expand(pp_defer(block_inline__as$)(param_expand__as$ __VA_ARGS__))
#define param_expand__as$(...)          __VA_ARGS__, pp_expand
#define block_inline__as$(_T, _Expr...) ((_T)(_Expr))

#define swap$(/*(T)(p_lhs, p_rhs)*/...)         pp_expand(pp_defer(block_inline__swap$)(param_expand__swap$ __VA_ARGS__))
#define param_expand__swap$(...)                __VA_ARGS__, pp_expand
#define block_inline__swap$(_T, _p_lhs, _p_rhs) ({ \
    p$(_T) __lhs = (__p_lhs); \
    p$(_T) __rhs = (__p_rhs); \
    _T __tmp = *__lhs; \
    *__lhs = *__rhs; \
    *__rhs = __tmp; \
    ({}); \
})

#define min$(/*(T)(lhs, rhs)*/...)         pp_expand(pp_defer(block_inline__min$)(param_expand__min$ __VA_ARGS__))
#define param_expand__min$(...)            __VA_ARGS__, pp_expand
#define block_inline__min$(_T, _lhs, _rhs) ({ \
    const _T __lhs = (_lhs); \
    const _T __rhs = (_rhs); \
    __rhs < __lhs ? __rhs : __lhs; \
})
#define max$(/*(T)(lhs, rhs)*/...)         pp_expand(pp_defer(block_inline__max$)(param_expand__max$ __VA_ARGS__))
#define param_expand__max$(...)            __VA_ARGS__, pp_expand
#define block_inline__max$(_T, _lhs, _rhs) ({ \
    const _T __lhs = (_lhs); \
    const _T __rhs = (_rhs); \
    __rhs > __lhs ? __rhs : __lhs; \
})
#endif /* NEXT_UPDATE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_prim__included */
