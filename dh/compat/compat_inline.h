#ifndef COMPAT_INLINE_INCLUDED
#define COMPAT_INLINE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/* Detect compiler and language standard */
#if defined(__cplusplus)
/* C++ always supports 'inline' */
#define static_inline static inline
#define extern_inline inline
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
/* C99 or later */
#define static_inline static inline
#define extern_inline inline
#elif defined(_MSC_VER)
/* Microsoft Visual Studio */
#define static_inline static __inline
#define extern_inline __inline
#elif defined(__GNUC__) || defined(__clang__)
/* GNU C and Clang support 'inline' */
#define static_inline static inline
#define extern_inline inline
#else
/* Fallback for compilers without 'inline' */
#define static_inline static
#define extern_inline
#endif

/* Force inline for performance-critical functions */
#if defined(_MSC_VER)
/* Microsoft Visual Studio */
#define force_inline __forceinline
#elif defined(__GNUC__) || defined(__clang__)
/* GNU C and Clang support 'inline' */
#define force_inline inline __attribute__((always_inline))
#else
/* Fallback for compilers without 'static_inline' */
#define force_inline static_inline
#endif


#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* COMPAT_INLINE_INCLUDED */
