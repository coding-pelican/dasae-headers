/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    type_info.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-23 (date of creation)
 * @updated 2025-02-02 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  (none)
 *
 * @brief   Type information and checking utilities
 * @details Provides compile-time type analysis and validation capabilities
 */
#ifndef builtin_type_info__included
#define builtin_type_info__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "comp.h"

/*========== Macros and Declarations ========================================*/

#define nameOf(_Tok...) __expr__nameOf(_Tok, #_Tok)

#define isComptimeExpr(_Expr... /*(bool)*/) __expr__isComptimeExpr(_Expr)
#define isRuntimeExpr(_Expr... /*(bool)*/) __expr__isRuntimeExpr(_Expr)

#define TypeOf(_Expr /*: T*/... /*(T)*/) __type__TypeOf(_Expr)
#define TypeOfUnqual(_Expr /*: T*/... /*(Unqual(T))*/) __type__TypeOfUnqual(_Expr)

#define null __val__null
#define null$(_P_T...) __val__null$(_P_T)

#define $ref __capt__$ref
#define raw_ref(_v /*: T*/... /*(P(T))*/) __expr__raw_ref(_v)
#define RefType$(_T /*: T*/... /*(P(T))*/) __type__RefType$(_T)
#define RefTypeUnqual$(_T /*: T*/... /*(P(Unqual(T)))*/) __type__RefTypeUnqual$(_T)

#define $deref __capt__$deref
#define raw_deref(_p /*: P(T)*/... /*(T)*/) __expr__raw_deref(_p)
#define DerefType$(_P_T /*: P(T)*/... /*(T)*/) __type__DerefType$(_P_T)
#define DerefTypeUnqual$(_P_T /*: P(T)*/... /*(Unqual(T))*/) __type__DerefTypeUnqual$(_P_T)

#define eqlType$(_T_LHS /*: T*/, _T_RHS /*: U*/... /*(bool)*/) __expr__eqlType$(_T_LHS, _T_RHS)
#define eqlTypeUnqual$(_T_LHS /*: T*/, _T_RHS /*: U*/... /*(bool)*/) __expr__eqlTypeUnqual$(_T_LHS, _T_RHS)

#define neqType$(_T_LHS /*: T*/, _T_RHS /*: U*/... /*(bool)*/) __expr__neqType$(_T_LHS, _T_RHS)
#define neqTypeUnqual$(_T_LHS /*: T*/, _T_RHS /*: U*/... /*(bool)*/) __expr__neqTypeUnqual$(_T_LHS, _T_RHS)

#define sizeOf$(_T... /*(usize)*/) __expr__sizeOf$(_T)
#define countOf$(_T... /*(usize)*/) __expr__countOf$(_T)

#define alignOf$(_T... /*(usize)*/) __expr__alignOf$(_T)
#define alignOfLog2$(_T... /*(u8)*/) __expr__alignOfLog2$(_T)

#define $alignAs(_align /*: usize*/...) __attr__$alignAs(_align)
#define $alignAsLog2(_log2_align /*: u8*/...) __attr__$alignAsLog2(_log2_align)

/*========== Macros and Definitions =========================================*/

#define __expr__nameOf(_Tok, _Str...) #_Str

#define __expr__isComptimeExpr(_expr...) bool_(__builtin_constant_p(_expr))
#define __expr__isRuntimeExpr(_expr...) pri_not(isComptimeExpr(_expr))

#define __type__TypeOf(_Expr...) __typeof__(_Expr)
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if defined(__clang__) && __clang_major__ >= 16
/* Clang >= 16.0 supports `__typeof_unqual__` */
#define __type__TypeOfUnqual(_Expr...) __typeof_unqual__(_Expr)
#elif !defined(__clang__) && GCC_VERSION >= 130100
/* GCC >= 13.1 supports `__typeof_unqual__` */
#define __type__TypeOfUnqual(_Expr...) __typeof_unqual__(_Expr)
#else
/* Fallback for no support of `__typeof_unqual__` */
#define __type__TypeOfUnqual(_Expr...) TypeOf((TypeOf(_Expr))(l0$((TypeOf(_Expr)))))
#endif

#define __val__null null$(void*)
#define __val__null$(_P_T...) /* NOLINT(bugprone-casting-through-void) */ ((_P_T)0)

#define __capt__$ref $_ref,
#define __expr__raw_ref(_v...) (&(_v))
#define __type__RefType$(_T...) TypeOf(_T*)
#define __type__RefTypeUnqual$(_T...) RefType$(TypeOfUnqual(_T))

#define __capt__$deref $_deref,
#define __expr__raw_deref(_p...) (*(_p))
#define __type__DerefType$(_P_T...) TypeOf(raw_deref(null$(_P_T)))
#define __type__DerefTypeUnqual$(_P_T...) TypeOfUnqual(raw_deref(null$(_P_T)))

#define __expr__eqlType$(_T_LHS, _T_RHS...) __builtin_types_compatible_p(_T_LHS, _T_RHS)
#define __expr__eqlTypeUnqual$(_T_LHS, _T_RHS...) eqlType$(TypeOfUnqual(_T_LHS), TypeOfUnqual(_T_RHS))

#define __expr__neqType$(_T_LHS, _T_RHS...) (!eqlType$(_T_LHS, _T_RHS))
#define __expr__neqTypeUnqual$(_T_LHS, _T_RHS...) (!eqlTypeUnqual$(_T_LHS, _T_RHS))

#define __step__sizeOf$__expand(...) __VA_ARGS__
#define __expr__sizeOf$(_T...) $supress_pointer_arith(__step__sizeOf$__expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((void)(usize_(0))), \
        T_default_(as$(usize)(sizeof(_T))) \
    ) pp_end \
))
#define __expr__countOf$(_T...) ( \
    !sizeOf$(_T) ? usize_(0) : (sizeOf$(_T) / sizeOf$(TypeOf(raw_deref(null$(RefType$(_T)))[0]))) \
)

#define __step__alignOf$__expand(...) __VA_ARGS__
#define __expr__alignOf$(_T...) $supress_pointer_arith(__step__alignOf$__expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((void)(usize_(1))), \
        T_default_(as$(usize)((_Alignof(_T)))) \
    ) pp_end \
))
#define __expr__alignOfLog2$(_T...) $supress_pointer_arith( \
    (as$(u8)((64u - 1u) - as$(u32)(__builtin_clzll(alignOf$(_T))))) \
)

#define __attr__$alignAs(_align...) _Alignas(_align)
#define __attr__$alignAsLog2(_log2_align...) _Alignas(usize_(1) << (_log2_align))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_type_info__included */
