/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ptr.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2026-01-28 (date of last update)
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
#ifndef core_prim_ptr__included
#define core_prim_ptr__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "null.h"
#include <stddef.h>

/*========== Macros and Declarations ========================================*/

typedef TypeOf(const void*) P_const$raw;
typedef TypeOf(void*) P$raw;

#define $P_const$(_T...) TypeOf(const _T*)
#define $P$(_T...) TypeOf(_T*)

#define isNull(_p /*: P(_T)*/... /*(bool)*/) ____isNull(_p)
#define isNonnull(_p /*: P(_T)*/... /*(bool)*/) ____isNonnull(_p)
#define ensureNonnull(_p /*: P(_T)*/... /*(P(_T))*/) __step__ensureNonnull(_p)

#define raw_ref(_v /*: _T*/... /*(P(_T))*/) ____raw_ref(_v)
#define raw_deref(_p /*: P(_T)*/... /*(_T)*/) ____raw_deref(_p)

#define ref(_v /*: _T*/... /*(P(_T))*/) ____ref(_v)
#define deref(_p /*: P(_T)*/... /*(_T)*/) ____deref(_p)

#define ptrToInt(_p /*: PtrType*/... /*(usize)*/) ____ptrToInt(_p)
#define intToPtr$(/*(_P_T: PtrType)(_val: usize)*/... /*(_P_T)*/) __step__intToPtr$(__VA_ARGS__)

#define ptrCast$(/*(_P_T: PtrType)(_val: PtrType)*/... /*(_P_T)*/) __step__ptrCast$(__VA_ARGS__)
#define ptrAlignCast$(/*(_P_T: PtrType)(_val: PtrType)*/... /*(_P_T)*/) __step__ptrAlignCast$(__VA_ARGS__)
#define ptrQualCast$(/*(_P_T: PtrType)(_val: PtrType)*/... /*(_P_T)*/) ____ptrQualCast$(__VA_ARGS__)
#define ptrAlignQualCast$(/*(_P_T: PtrType)(_val: PtrType)*/... /*(_P_T)*/) ____ptrAlignQualCast$(__VA_ARGS__)

#define mutCast(_p /*: P(_T)*/... /*(P_const(_T))*/) ____mutCast(_p)
#define constCast(_p /*: P_const(_T)*/... /*(P(_T))*/) ____constCast(_p)

/*========== Macros and Definitions =========================================*/

#define ____isNull(_p...) (as$(bool)((_p) == null))
#define ____isNonnull(_p...) (as$(bool)((_p) != null))

#define __step__ensureNonnull(_p...) ____ensureNonnull(pp_uniqTok(p), _p)
#define ____ensureNonnull(__p, _p...) blk({ \
    let_(__p, TypeOf(_p)) = _p; \
    blk_return_((claim_assert_nonnull(__p), __p)); \
})

#define ____raw_ref(_v...) (&(_v))
#define ____raw_deref(_p...) (*(_p))

#define ____ref(_v...) (&(_v))
#define ____deref(_p...) (*ensureNonnull(_p))

#define ____ptrToInt(_p...) /* NOLINT(performance-no-int-to-ptr) */ ((usize)(_p))

#define __step__intToPtr$(...) __step__intToPtr$__emit(__step__intToPtr$__parse __VA_ARGS__)
#define __step__intToPtr$__parse(_P_T...) _P_T,
#define __step__intToPtr$__emit(...) ____intToPtr$(__VA_ARGS__)
#define ____intToPtr$(_P_T, _val...) /* NOLINT(performance-no-int-to-ptr) */ ((_P_T)(_val))

#define __step__ptrCast$(...) __step__ptrCast$__emit(__step__ptrCast$__parse __VA_ARGS__)
#define __step__ptrCast$__parse(_P_T...) _P_T,
#define __step__ptrCast$__emit(...) ____ptrCast$(__VA_ARGS__)
#define ____ptrCast$(_P_T, _val...) (as$(_P_T)(_val))

#define __step__ptrAlignCast$(...) __step__ptrAlignCast$__emit(__step__ptrAlignCast$__parse __VA_ARGS__)
#define __step__ptrAlignCast$__parse(_P_T...) _P_T,
#define __step__ptrAlignCast$__emit(...) ____ptrAlignCast$(__VA_ARGS__)
#define ____ptrAlignCast$(_P_T, _val...) $supress_cast_align( \
    (as$(_P_T)(alignCast((alignOf$(DerefType$(_P_T)))(_val)))) \
)

#define ____ptrQualCast$(...) $supress_cast_qual(ptrCast$(__VA_ARGS__))
#define ____ptrAlignQualCast$(...) $supress_cast_align(ptrAlignCast$(__VA_ARGS__))

#define ____mutCast(_p...) ptrCast$((const TypeOfUnqual(*_p)*)(_p))
#define ____constCast(_p...) ptrQualCast$((TypeOfUnqual(*_p)*)(_p))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_prim_ptr__included */
