#ifndef COMPAT_BOOL_INCLUDED
#define COMPAT_BOOL_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


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
#  if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#    include <stdbool.h>
#  else
/* Pre-C99 - define bool manually */
#    define bool  _Bool
#    define true  (1)
#    define false (0)
typedef unsigned char _Bool;
#  endif
#endif /* !defined(__cplusplus) */

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


// Ensure bool is always exactly 1 byte
typedef char ensure_bool_size[(sizeof(bool) == 1) ? 1 : -1];
_Static_assert(sizeof(bool) == 1, "bool must be 1 byte");


#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* COMPAT_BOOL_INCLUDED */
