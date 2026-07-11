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
#pragma once
#ifndef builtin_type_info__included
#define builtin_type_info__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "comp.h"

/*========== Macros and Declarations ========================================*/

#define nameOf(_$Tok...) __expr__nameOf(_$Tok, #_$Tok)

#define isComptimeExpr(_$Expr... /*(bool)*/) __expr__isComptimeExpr(_$Expr)
#define isRuntimeExpr(_$Expr... /*(bool)*/) __expr__isRuntimeExpr(_$Expr)

#define TypeOf(_$Expr /*: T*/... /*(T)*/) __type__TypeOf(_$Expr)
#define TypeOfUnqual(_$Expr /*: T*/... /*(Unqual(T))*/) __type__TypeOfUnqual(_$Expr)

#define null __val__null
#define null$(_$P_T...) __val__null$(_$P_T)

#define $ref __capt__$ref
#define raw_ref(_$v /*: T*/... /*(P(T))*/) __expr__raw_ref(_$v)
#define RefType$(_$T /*: T*/... /*(P(T))*/) __type__RefType$(_$T)
#define RefTypeUnqual$(_$T /*: T*/... /*(P(Unqual(T)))*/) __type__RefTypeUnqual$(_$T)

#define $deref __capt__$deref
#define raw_deref(_$p /*: P(T)*/... /*(T)*/) __expr__raw_deref(_$p)
#define DerefType$(_$P_T /*: P(T)*/... /*(T)*/) __type__DerefType$(_$P_T)
#define DerefTypeUnqual$(_$P_T /*: P(T)*/... /*(Unqual(T))*/) __type__DerefTypeUnqual$(_$P_T)

#define eqlType$(_$T_LHS /*: T*/, _$T_RHS /*: U*/... /*(bool)*/) __expr__eqlType$(_$T_LHS, _$T_RHS)
#define eqlTypeUnqual$(_$T_LHS /*: T*/, _$T_RHS /*: U*/... /*(bool)*/) __expr__eqlTypeUnqual$(_$T_LHS, _$T_RHS)

#define neqType$(_$T_LHS /*: T*/, _$T_RHS /*: U*/... /*(bool)*/) __expr__neqType$(_$T_LHS, _$T_RHS)
#define neqTypeUnqual$(_$T_LHS /*: T*/, _$T_RHS /*: U*/... /*(bool)*/) __expr__neqTypeUnqual$(_$T_LHS, _$T_RHS)

#define sizeOf$(_$T... /*(usize)*/) __expr__sizeOf$(_$T)
#define countOf$(_$T... /*(usize)*/) __expr__countOf$(_$T)

#define alignOf$(_$T... /*(usize)*/) __expr__alignOf$(_$T)
#define alignOfLog2$(_$T... /*(u8)*/) __expr__alignOfLog2$(_$T)

#define $alignAs(_$align /*: usize*/...) __attr__$alignAs(_$align)
#define $alignAsLog2(_$log2_align /*: u8*/...) __attr__$alignAsLog2(_$log2_align)

/*========== Macros and Definitions =========================================*/

#define __expr__nameOf(_$ignored, _$Tok...) _$Tok

#define comp_when_(_$cond...) /*(comp_provide_(_$then...) comp_instead_(_$else...))*/ __inline__comp_when_(_$cond)
#define __inline__comp_when_(_$cond...) __builtin_choose_expr pp_begin(_$cond) __step__comp_when___parseProvideInstead
#define __step__comp_when___parseProvideInstead(_$provide, _$instead...) _$provide _$instead pp_end
#define comp_provide_(_$then...) , (_$then)
#define comp_instead_(_$else...) , (_$else)

#define __expr__isComptimeExpr(_$expr...) bool_(__builtin_constant_p(_$expr))
#define __expr__isRuntimeExpr(_$expr...) pri_not(isComptimeExpr(_$expr))

#define __type__TypeOf(_$Expr...) __typeof__(_$Expr)
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if defined(__clang__) && __clang_major__ >= 16
/* Clang >= 16.0 supports `__typeof_unqual__` */
#define __type__TypeOfUnqual(_$Expr...) __typeof_unqual__(_$Expr)
#elif !defined(__clang__) && GCC_VERSION >= 130100
/* GCC >= 13.1 supports `__typeof_unqual__` */
#define __type__TypeOfUnqual(_$Expr...) __typeof_unqual__(_$Expr)
#else
/* Fallback for no support of `__typeof_unqual__` */
#define __type__TypeOfUnqual(_$Expr...) TypeOf((TypeOf(_$Expr))(l0$((TypeOf(_$Expr)))))
#endif

#define __val__null null$(void*)
#define __val__null$(_$P_T...) /* NOLINT(bugprone-casting-through-void) */ ((_$P_T)0)

#define __capt__$ref $_ref,
#define __expr__raw_ref(_$v...) (&(_$v))
#define __type__RefType$(_$T...) TypeOf(_$T*)
#define __type__RefTypeUnqual$(_$T...) RefType$(TypeOfUnqual(_$T))

#define __capt__$deref $_deref,
#define __expr__raw_deref(_$p...) (*(_$p))
#define __type__DerefType$(_$P_T...) TypeOf(raw_deref(null$(_$P_T)))
#define __type__DerefTypeUnqual$(_$P_T...) TypeOfUnqual(raw_deref(null$(_$P_T)))

#define __expr__eqlType$(_$T_LHS, _$T_RHS...) __builtin_types_compatible_p(_$T_LHS, _$T_RHS)
#define __expr__eqlTypeUnqual$(_$T_LHS, _$T_RHS...) eqlType$(TypeOfUnqual(_$T_LHS), TypeOfUnqual(_$T_RHS))

#define __expr__neqType$(_$T_LHS, _$T_RHS...) (!eqlType$(_$T_LHS, _$T_RHS))
#define __expr__neqTypeUnqual$(_$T_LHS, _$T_RHS...) (!eqlTypeUnqual$(_$T_LHS, _$T_RHS))

#define __step__sizeOf$__expand(...) __VA_ARGS__
#define __expr__sizeOf$(_$T...) $suppress_pointer_arith(__step__sizeOf$__expand( \
    T_switch$ pp_begin(_$T)( \
        T_case$((void)(usize_(0))), \
        T_default_(as$(usize)(sizeof(_$T))) \
    ) pp_end \
))
#define __expr__countOf$(_$T...) ( \
    !sizeOf$(_$T) ? usize_(0) : (sizeOf$(_$T) / sizeOf$(TypeOf(raw_deref(null$(RefType$(_$T)))[0]))) \
)

#define __step__alignOf$__expand(...) __VA_ARGS__
#define __expr__alignOf$(_$T...) $suppress_pointer_arith(__step__alignOf$__expand( \
    T_switch$ pp_begin(_$T)( \
        T_case$((void)(usize_(1))), \
        T_default_(as$(usize)((_Alignof(_$T)))) \
    ) pp_end \
))
#define __expr__alignOfLog2$(_$T...) $suppress_pointer_arith( \
    (as$(u8)((64u - 1u) - as$(u32)(__builtin_clzll(alignOf$(_$T))))) \
)

#define __attr__$alignAs(_$align...) _Alignas(_$align)
#define __attr__$alignAsLog2(_$log2_align...) _Alignas(usize_(1) << (_$log2_align))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_type_info__included */
