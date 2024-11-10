// #include "ints.h"

// #include "../common.h"
// #include "../compat/compat_inline.h"


// #define INT_CMP_FUNCTIONS_DEFINITION(_T)                     \
//     force_inline bool _T##_eq(_T x, _T y) { return x == y; } \
//     force_inline bool _T##_ne(_T x, _T y) { return x != y; } \
//     force_inline bool _T##_lt(_T x, _T y) { return x < y; }  \
//     force_inline bool _T##_le(_T x, _T y) { return x <= y; } \
//     force_inline bool _T##_gt(_T x, _T y) { return x > y; }  \
//     force_inline bool _T##_ge(_T x, _T y) { return x >= y; }

// #define INT_UNARY_FUNCTIONS_DEFINITION(_T)        \
//     force_inline _T _T##_neg(_T x) { return -x; } \
//     force_inline _T _T##_abs(_T x) { return pp_abs_(_T, x); }

// #define INT_ARITH_FUNCTIONS_DEFINITION(_T)                 \
//     force_inline _T _T##_add(_T x, _T y) { return x + y; } \
//     force_inline _T _T##_sub(_T x, _T y) { return x - y; } \
//     force_inline _T _T##_mul(_T x, _T y) { return x * y; } \
//     force_inline _T _T##_div(_T x, _T y) { return x / y; } \
//     force_inline _T _T##_mod(_T x, _T y) { return x % y; }

// #define INT_UTILS_FUNCTIONS_DEFINITION(_T)                             \
//     force_inline _T _T##_min(_T x, _T y) { return pp_min_(_T, x, y); } \
//     force_inline _T _T##_max(_T x, _T y) { return pp_max_(_T, x, y); } \
//     force_inline _T _T##_clamp(_T x, _T low, _T high) {                \
//         return pp_clamp_(_T, x, low, high);                            \
//     }                                                                  \
//     force_inline _T _T##_wrap(_T x, _T low, _T high) {                 \
//         return pp_wrap_(_T, x, low, high);                             \
//     }


// INT_CMP_FUNCTIONS_DEFINITION(i8)
// INT_UNARY_FUNCTIONS_DEFINITION(i8)
// INT_ARITH_FUNCTIONS_DEFINITION(i8)
// INT_UTILS_FUNCTIONS_DEFINITION(i8)

// INT_CMP_FUNCTIONS_DEFINITION(u8)
// INT_ARITH_FUNCTIONS_DEFINITION(u8)
// INT_UTILS_FUNCTIONS_DEFINITION(u8)

// INT_CMP_FUNCTIONS_DEFINITION(i16)
// INT_ARITH_FUNCTIONS_DEFINITION(i16)
// INT_UTILS_FUNCTIONS_DEFINITION(i16)

// INT_CMP_FUNCTIONS_DEFINITION(u16)
// INT_ARITH_FUNCTIONS_DEFINITION(u16)
// INT_UTILS_FUNCTIONS_DEFINITION(u16)

// INT_CMP_FUNCTIONS_DEFINITION(i32)
// INT_ARITH_FUNCTIONS_DEFINITION(i32)
// INT_UTILS_FUNCTIONS_DEFINITION(i32)

// INT_CMP_FUNCTIONS_DEFINITION(u32)
// INT_ARITH_FUNCTIONS_DEFINITION(u32)
// INT_UTILS_FUNCTIONS_DEFINITION(u32)

// INT_CMP_FUNCTIONS_DEFINITION(i64)
// INT_ARITH_FUNCTIONS_DEFINITION(i64)
// INT_UTILS_FUNCTIONS_DEFINITION(i64)

// INT_CMP_FUNCTIONS_DEFINITION(u64)
// INT_ARITH_FUNCTIONS_DEFINITION(u64)
// INT_UTILS_FUNCTIONS_DEFINITION(u64)

// INT_CMP_FUNCTIONS_DEFINITION(isize)
// INT_ARITH_FUNCTIONS_DEFINITION(isize)
// INT_UTILS_FUNCTIONS_DEFINITION(isize)

// INT_CMP_FUNCTIONS_DEFINITION(usize)
// INT_ARITH_FUNCTIONS_DEFINITION(usize)
// INT_UTILS_FUNCTIONS_DEFINITION(usize)


// #undef INT_CMP_FUNCTIONS_DEFINITION
// #undef INT_UNARY_FUNCTIONS_DEFINITION
// #undef INT_ARITH_FUNCTIONS_DEFINITION
// #undef INT_UTILS_FUNCTIONS_DEFINITION
