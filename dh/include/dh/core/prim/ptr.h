/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ptr.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  (none)
 *
 * @brief   Pointer utilities
 * @details Provides pointer utilities and functions for:
 *          - Pointer definition
 *          - Pointer usage
 *          - Pointer operations
 *          - Pointer utilities
 */
#ifndef CORE_PRIM_PTR_INCLUDED
#define CORE_PRIM_PTR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "null.h"
#include <stddef.h>

/*========== Macros and Definitions =========================================*/

typedef ptrdiff_t ptrdiff;

typedef TypeOf(const void*) P_const$raw;
typedef TypeOf(void*) P$raw;

#define $P_const$(_T...) TypeOf(const _T*)
#define $P$(_T...) TypeOf(_T*)

#define ptrCast$(/*(_T: PtrType)(_val: PtrType)*/... /*(_T)*/) __step__ptrCast$(__VA_ARGS__)
#define ptrAlignCast$(/*(_T: PtrType)(_val: PtrType)*/... /*(_T)*/) __step__ptrAlignCast$(__VA_ARGS__)
#define ptrToInt(_p /*: PtrType*/... /*(usize)*/) ____ptrToInt(_p)
#define intToPtr$(/*(_T: PtrType)(_val: usize)*/... /*(_T)*/) __step__intToPtr$(__VA_ARGS__)

#define isNull(_p /*: P(_T)*/... /*(bool)*/) ____isNull(_p)
#define isNonnull(_p /*: P(_T)*/... /*(bool)*/) ____isNonnull(_p)
#define ensureNonnull(_p /*: P(_T)*/... /*P(_T)*/) __step__ensureNonnull(_p)

#define rawref(var_src...) comp_op__rawref(var_src)
#define rawderef(val_ptr...) comp_op__rawderef(val_ptr)

#define ref(val_src...) comp_op__ref(val_src)
#define deref(val_ptr...) comp_op__deref(val_ptr)

/*========== Macros Implementation ==========================================*/

#define __step__ptrCast$(...) __step__ptrCast$__emit(__step__ptrCast$__parse __VA_ARGS__)
#define __step__ptrCast$__parse(_T...) _T,
#define __step__ptrCast$__emit(...) ____ptrCast$(__VA_ARGS__)
#define ____ptrCast$(_T, _val...) (as$(_T)(_val))
#define __step__ptrAlignCast$(...) __step__ptrAlignCast$__emit(__step__ptrAlignCast$__parse __VA_ARGS__)
#define __step__ptrAlignCast$__parse(_T...) _T,
#define __step__ptrAlignCast$__emit(...) ____ptrAlignCast$(__VA_ARGS__)
#define ____ptrAlignCast$(_T, _val...) $pragma_guard_( \
    "clang diagnostic push", "clang diagnostic ignored \"-Wcast-align\"", "clang diagnostic pop", \
    (as$(_T)(alignCast((alignOf$(_T))(_val)))) \
)
#define ____ptrToInt(_p...) ((usize)(_p)) /* NOLINT(performance-no-int-to-ptr) */
#define __step__intToPtr$(...) __step__intToPtr$__emit(__step__intToPtr$__parse __VA_ARGS__)
#define __step__intToPtr$__parse(_T...) _T,
#define __step__intToPtr$__emit(...) ____intToPtr$(__VA_ARGS__)
#define ____intToPtr$(_T, _val...) ((_T)(_val)) /* NOLINT(performance-no-int-to-ptr) */

#define ____isNull(_p...) (as$(bool)((_p) == null))
#define ____isNonnull(_p...) (as$(bool)((_p) != null))
#define __step__ensureNonnull(_p...) ____ensureNonnull(pp_uniqTok(p), _p)
#define ____ensureNonnull(__p, _p...) blk({ \
    let_(__p, TypeOf(_p)) = _p; \
    blk_return_(claim_assert_nonnull(__p), __p); \
})

#define comp_op__rawref(var_src...) (&(var_src))
#define comp_op__rawderef(val_ptr...) (*(val_ptr))

#define comp_op__ref(val_src...) (&(val_src))
#define comp_op__deref(val_ptr...) (*ensureNonnull(val_ptr))

/*========== Validation Checks ==============================================*/

#if arch_bits_is_64bit
/* 64-bit */
claim_assert_static_msg(sizeof(P$raw) == 8, "pointer must be 8 bytes on 64-bit");
#elif arch_bits_is_32bit
/* 32-bit */
claim_assert_static_msg(sizeof(P$raw) == 4, "pointer must be 4 bytes on 32-bit");
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_PTR_INCLUDED */
