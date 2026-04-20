#ifndef simd_V__included
#define simd_V__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

#define $simd_V$(_n /*: usize*/, _T...) __type__$simd_V$(_n, _T)
#define simd_V$$(_n /*: usize*/, _T...) __type__simd_V$$(_n, _T)
#define simd_V$(_n /*: usize*/, _T...) __alias__simd_V$(_n, _T)
#define T_decl_simd_V$(_n /*: usize*/, _T...) __stmt__T_decl_simd_V$(_n, _T)
#define T_impl_simd_V$(_n /*: usize*/, _T...) __stmt__T_impl_simd_V$(_n, _T)
#define T_use_simd_V$(_n /*: usize*/, _T...) __stmt__T_use_simd_V$(_n, _T)

#define simd_V_n$(_T /*: simd_V(n: usize, T)*/... /*(usize)*/) __val__simd_V_n$(_T)
#define simd_V_T$(_T /*: simd_V(n: usize, T)*/... /*(T)*/) __type__simd_V_T$(_T)
#define simd_V_TUnqual$(_T /*: simd_V(n: usize, T)*/... /*(Unqual(T))*/) __type__simd_V_TUnqual$(_T)

#define simd_V_zero$(/*(_T: simd_V(n: usize, T))*/... /*(_T)*/) __op__simd_V_zero$(__VA_ARGS__)
#define simd_V_zero() __op__simd_V_zero()
#define simd_V_init$(/*(_T: simd_V(n: usize, T)){ _initial... }*/... /*(_T)*/) __op__simd_V_init$(__VA_ARGS__)
#define simd_V_init(/*{ _initial... }*/...) __op__simd_V_init(__VA_ARGS__)
#define simd_V_from$(/*(_T){ _initial... }*/... /*(simd_V(n: usize, _T))*/) __op__simd_V_from$(__VA_ARGS__)
#define simd_V_fromA$(/*(_T: simd_V(n: usize, T))(_a: A(n: usize, T))*/... /*(_T)*/) __op__simd_V_fromA$(__VA_ARGS__)
#define simd_V_fromA(_a /*: A(n: usize, T)*/... /*(simd_V(n: usize, T))*/) __op__simd_V_fromA(_a)
#define simd_V_toA$(/*(_T: A(n: usize, T))(_v: simd_V(n: usize, T))*/... /*(_T)*/) __op__simd_V_toA$(__VA_ARGS__)
#define simd_V_toA(_v /*: simd_V(n: usize, T)*/... /*(A(n: usize, T))*/) __op__simd_V_toA(_v)

#define simd_V_val(_v /*: simd_V(n: usize, T)*/... /*($simd_V$(n: usize, T))*/) __op__simd_V_val(_v)
#define simd_V_len$(_T /*: simd_V(n: usize, T)*/... /*(usize)*/) __op__simd_V_len$(_T)
#define simd_V_len(_v /*: simd_V(n: usize, T)*/... /*(usize)*/) __op__simd_V_len(_v)
#define simd_V_at(/*(_v: simd_V(n: usize, T))[_idx: usize]*/... /*(P(T))*/) __op__simd_V_at(__VA_ARGS__)

#define simd_V_isZero(_x...) __op__simd_V_isZero(_x)
#define simd_V_isNonzero(_x...) __op__simd_V_isNonzero(_x)

#define simd_V_add(_lhs, _rhs...) __op__simd_V_add(_lhs, _rhs)
#define simd_V_sub(_lhs, _rhs...) __op__simd_V_sub(_lhs, _rhs)
#define simd_V_mul(_lhs, _rhs...) __op__simd_V_mul(_lhs, _rhs)
#define simd_V_mulAdd(_x, _y, _z...) __op__simd_V_mulAdd(_x, _y, _z)
#define simd_V_div(_lhs, _rhs...) __op__simd_V_div(_lhs, _rhs)
#define simd_V_rem(_x, _n...) __op__simd_V_rem(_x, _n)

#define simd_V_neg(_x...) __op__simd_V_neg(_x)
#define simd_V_abs(_x...) __op__simd_V_abs__step(_x)
#define simd_V_sgn(_x...) __op__simd_V_sgn__step(_x)

#define simd_V_not(_x...) __op__simd_V_not(_x)
#define simd_V_and(_x, _y...) __op__simd_V_and(_x, _y)
#define simd_V_or(_x, _y...) __op__simd_V_or(_x, _y)

#define simd_V_eql(_lhs, _rhs...) __op__simd_V_eql(_lhs, _rhs)
#define simd_V_neq(_lhs, _rhs...) __op__simd_V_neq(_lhs, _rhs)

#define simd_V_ord(_lhs, _rhs...) __op__simd_V_ord__step(_lhs, _rhs)
#define simd_V_eq(_lhs, _rhs...) __op__simd_V_eq(_lhs, _rhs)
#define simd_V_ne(_lhs, _rhs...) __op__simd_V_ne(_lhs, _rhs)
#define simd_V_lt(_lhs, _rhs...) __op__simd_V_lt(_lhs, _rhs)
#define simd_V_gt(_lhs, _rhs...) __op__simd_V_gt(_lhs, _rhs)
#define simd_V_le(_lhs, _rhs...) __op__simd_V_le(_lhs, _rhs)
#define simd_V_ge(_lhs, _rhs...) __op__simd_V_ge(_lhs, _rhs)

#define simd_V_min(_lhs, _rhs...) __op__simd_V_min2__step(_lhs, _rhs)
#define simd_V_min2(_lhs, _rhs...) __op__simd_V_min2__step(_lhs, _rhs)
#define simd_V_min3(_1st, _2nd, _3rd...) __op__simd_V_min3__step(_1st, _2nd, _3rd)
#define simd_V_min4(_1st, _2nd, _3rd, _4th...) __op__simd_V_min4__step(_1st, _2nd, _3rd, _4th)
#define simd_V_findMin(_vals...) __op__simd_V_findMin__step(_vals)

#define simd_V_max(_lhs, _rhs...) __op__simd_V_max2__step(_lhs, _rhs)
#define simd_V_max2(_lhs, _rhs...) __op__simd_V_max2__step(_lhs, _rhs)
#define simd_V_max3(_1st, _2nd, _3rd...) __op__simd_V_max3__step(_1st, _2nd, _3rd)
#define simd_V_max4(_1st, _2nd, _3rd, _4th...) __op__simd_V_max3__step(_1st, _2nd, _3rd, _4th)
#define simd_V_findMax(_vals...) __op__simd_V_findMax__step(_vals)

#define simd_V_clamp(_x, _lo /*$incl*/, _hi /*$incl*/...) __op__simd_V_clamp__step(_x, _lo, _hi)

#define simd_V_bitNot(_x...) __op__simd_V_bitNot(_x)
#define simd_V_bitShl(_x, _y...) __op__simd_V_bitShl(_x, _y)
#define simd_V_bitShr(_x, _y...) __op__simd_V_bitShr(_x, _y)
#define simd_V_bitAnd(_x, _y...) __op__simd_V_bitAnd(_x, _y)
#define simd_V_bitXor(_x, _y...) __op__simd_V_bitXor(_x, _y)
#define simd_V_bitOr(_x, _y...) __op__simd_V_bitOr(_x, _y)

#define simd_V_bitSet(_x, _bit...) __op__simd_V_bitSet(_x, _bit)
#define simd_V_bitReset(_x, _bit...) __op__simd_V_bitReset(_x, _bit)
#define simd_V_bitToggle(_x, _bit...) __op__simd_V_bitToggle(_x, _bit)

/*========== Macros and Definitions =========================================*/

#define __type__$simd_V$(_n, _T...) \
    TypeOf(_T __attribute__((vector_size(_n * sizeOf$(_T)))))
#define __type__simd_V$$(_n, _T...) \
    union { \
        var_(val, $simd_V$(_n, _T)); \
        var_(as_arr, A$$(_n, _T)); \
    }
#define __alias__simd_V$(_n, _T...) \
    pp_join3($, simd_V, _n, _T)
#define __stmt__T_decl_simd_V$(_n, _T...) \
    $maybe_unused typedef union simd_V$(_n, _T) simd_V$(_n, _T)
#define __stmt__T_impl_simd_V$(_n, _T...) \
    union simd_V$(_n, _T) { \
        var_(val, $simd_V$(_n, _T)); \
        var_(as_arr, A$(_n, _T)); \
    }
#define __stmt__T_use_simd_V$(_n, _T...) \
    T_decl_simd_V$(_n, _T); \
    T_impl_simd_V$(_n, _T)

#define __val__simd_V_n$(_T...) simd_V_len$(_T)
#define __type__simd_V_T$(_T...) TypeOf(null$(_T*)->val[0])
#define __type__simd_V_TUnqual$(_T...) TypeOfUnqual(null$(_T*)->val[0])

#define __op__simd_V_zero$(_T...) l0$(_T)
#define __op__simd_V_zero() cleared()
#define __op__simd_V_init$(...) __inline__simd_V_init$(__inline__simd_V_init$__parseT __VA_ARGS__)
#define __inline__simd_V_init$__parseT(_T...) _T,
#define __inline__simd_V_init$(_T, _initial...) l$((_T){ .val = _initial })
#define __op__simd_V_init(_initial...) { .val = _initial }
#define __op__simd_V_from$(...)
#define __op__simd_V_fromA$(...)
#define __op__simd_V_fromA(_a...)
#define __op__simd_V_toA$(...)
#define __op__simd_V_toA(_v...)

#define __op__simd_V_val(_v...) ((_v).val)
#define __op__simd_V_len$(_T...) simd_V_len(*null$(_T*))
#define __op__simd_V_len(_v...) countOf$(TypeOf((_v).as_arr.val))
#define __op__simd_V_at(...) __inline__simd_V_at(__inline__simd_V_at__parse __VA_ARGS__)
#define __inline__simd_V_at__parse(_v...) _v,
#define __inline__simd_V_at(_v, _idx...) A_at(((_v).as_arr)_idx)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* simd_V__included */
