#include "ints.h"
#include "common.h"


#define INT_CMP_FUNCTIONS_DEFINITION(_Type)              \
    bool _Type##_eq(_Type x, _Type y) { return x == y; } \
    bool _Type##_ne(_Type x, _Type y) { return x != y; } \
    bool _Type##_lt(_Type x, _Type y) { return x < y; }  \
    bool _Type##_le(_Type x, _Type y) { return x <= y; } \
    bool _Type##_gt(_Type x, _Type y) { return x > y; }  \
    bool _Type##_ge(_Type x, _Type y) { return x >= y; }

#define INT_ARITH_FUNCTIONS_DEFINITION(_Type)             \
    _Type _Type##_add(_Type x, _Type y) { return x + y; } \
    _Type _Type##_sub(_Type x, _Type y) { return x - y; } \
    _Type _Type##_mul(_Type x, _Type y) { return x * y; } \
    _Type _Type##_div(_Type x, _Type y) { return x / y; } \
    _Type _Type##_mod(_Type x, _Type y) { return x % y; }

#define INT_UTILS_FUNCTIONS_DEFINITION(_Type)                        \
    _Type _Type##_min(_Type x, _Type y) { return Min(_Type, x, y); } \
    _Type _Type##_max(_Type x, _Type y) { return Max(_Type, x, y); } \
    _Type _Type##_clamp(_Type x, _Type low, _Type high) {            \
        return Clamp(_Type, x, low, high);                           \
    }                                                                \
    _Type _Type##_wrap(_Type x, _Type low, _Type high) {             \
        return Wrap(_Type, x, low, high);                            \
    }

INT_CMP_FUNCTIONS_DEFINITION(i8)
INT_ARITH_FUNCTIONS_DEFINITION(i8)
INT_UTILS_FUNCTIONS_DEFINITION(i8)

INT_CMP_FUNCTIONS_DEFINITION(u8)
INT_ARITH_FUNCTIONS_DEFINITION(u8)
INT_UTILS_FUNCTIONS_DEFINITION(u8)

INT_CMP_FUNCTIONS_DEFINITION(i16)
INT_ARITH_FUNCTIONS_DEFINITION(i16)
INT_UTILS_FUNCTIONS_DEFINITION(i16)

INT_CMP_FUNCTIONS_DEFINITION(u16)
INT_ARITH_FUNCTIONS_DEFINITION(u16)
INT_UTILS_FUNCTIONS_DEFINITION(u16)

INT_CMP_FUNCTIONS_DEFINITION(i32)
INT_ARITH_FUNCTIONS_DEFINITION(i32)
INT_UTILS_FUNCTIONS_DEFINITION(i32)

INT_CMP_FUNCTIONS_DEFINITION(u32)
INT_ARITH_FUNCTIONS_DEFINITION(u32)
INT_UTILS_FUNCTIONS_DEFINITION(u32)

INT_CMP_FUNCTIONS_DEFINITION(i64)
INT_ARITH_FUNCTIONS_DEFINITION(i64)
INT_UTILS_FUNCTIONS_DEFINITION(i64)

INT_CMP_FUNCTIONS_DEFINITION(u64)
INT_ARITH_FUNCTIONS_DEFINITION(u64)
INT_UTILS_FUNCTIONS_DEFINITION(u64)

INT_CMP_FUNCTIONS_DEFINITION(isize)
INT_ARITH_FUNCTIONS_DEFINITION(isize)
INT_UTILS_FUNCTIONS_DEFINITION(isize)

INT_CMP_FUNCTIONS_DEFINITION(usize)
INT_ARITH_FUNCTIONS_DEFINITION(usize)
INT_UTILS_FUNCTIONS_DEFINITION(usize)
