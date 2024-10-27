#ifndef COMPAT_TYPES_INCLUDED
#define COMPAT_TYPES_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/* Architecture detection for proper alignment */
#if defined(__x86_64__) || defined(_M_X64)
#  define ARCH_64BIT
#else
#  define ARCH_32BIT
#endif /* defined(__x86_64__) || defined(_M_X64) */


/* Boolean type definition */
#if defined(__cplusplus)
#  ifdef OVERRIDE_CPP_BOOL
#    undef bool
#    undef true
#    undef false
#  endif
#endif /* defined(__cplusplus) */

#if !defined(__cplusplus)
/* C99 or later - use _Bool */
#  if defined(__STDC_VERSION__) && (199901L <= __STDC_VERSION__)
#    include <stdbool.h>
#  else
/* Pre-C99 - define bool manually */
#    define bool  _Bool
#    define true  1
#    define false 0
typedef unsigned char _Bool;
#  endif
#endif /* !defined(__cplusplus) */

// Ensure bool is always exactly 1 byte
typedef char ensure_bool_size[(sizeof(bool) == 1) ? 1 : -1];

// Boolean operations that work identically in C and C++
#define bool_and(_lhs, _rhs) ((_lhs) && (_rhs))
#define bool_or(_lhs, _rhs)  ((_lhs) || (_rhs))
#define bool_not(_x)         (!(_x))
#define bool_xor(_lhs, _rhs) ((bool)((_lhs) != (_rhs)))

// Boolean conversion macros
#define to_bool(_x)   ((bool)(!!(_x)))
#define from_bool(_x) ((int)(_x))

// Compile-time boolean constant creation
#define bool_const(_x) ((bool)(!!(_x)))

// // Boolean type checking helper (for debugging)
// #define is_bool_type(x) _Generic((x), bool: true, default: false)

// // Safe boolean assignment
// #define bool_assign(_dest, _src)    \
//     do {                            \
//         bool __tmp = to_bool(_src); \
//         (_dest)    = __tmp;         \
//     } while (false)


/* Null pointer definition */
#if defined(__cplusplus)
#  ifdef OVERRIDE_CPP_NULL
#    undef NULL
#    undef nullptr
#  endif
#endif /* __cplusplus */

#if defined(__cplusplus)
#  if (201103L <= __cplusplus)
/* C++11 or later - nullptr is available */
#    define null nullptr
#  else
/* Pre-C++11 */
#    define null NULL
#  endif
#else
/* C */
#  define null ((void*)0)
#endif /* defined(__cplusplus) */

// Type-safe null checks
// #define is_null(_ptr)  ((_ptr) == null)
// #define not_null(_ptr) ((_ptr) != null)


#if defined(__cplusplus)
} /* extern "C" */
// // C++ specific extensions
// namespace compat {
// // Type-safe boolean wrapper for C++
// class Boolean {
//     bool value;

// public:
//     Boolean(bool v = false)
//         : value(v) {}
//     operator bool() const { return value; }
//     Boolean& operator=(bool v) {
//         value = v;
//         return *this;
//     }
// };

// // Null type that can be used in both languages
// class Nullptr {
// public:
//     template <typename T>
//     operator T*() const { return 0; }
// };

// static const Nullptr null_value;
// } // namespace compat
#endif /* __cplusplus */

// Size assertions
_Static_assert(sizeof(bool) == 1, "bool must be 1 byte");

#ifdef ARCH_64BIT
static_assert(sizeof(void*) == 8, "pointer must be 8 bytes on 64-bit");
#else
static_assert(sizeof(void*) == 4, "pointer must be 4 bytes on 32-bit");
#endif

#endif // /* COMPAT_TYPES_INCLUDED */
