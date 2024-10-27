#ifndef COMPAT_ARCH_INCLUDED
#define COMPAT_ARCH_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/* Architecture detection for proper alignment */
#if defined(__x86_64__) || defined(_M_X64)
/* 64-bit */
#  define COMPAT_ARCH_64BIT (1)
#  define COMPAT_ARCH_32BIT (0)
#elif defined(__i386__) || defined(_M_IX86)
/* 32-bit */
#  define COMPAT_ARCH_64BIT (0)
#  define COMPAT_ARCH_32BIT (1)
#else
// #  error "Unknown architecture"
#endif


#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* COMPAT_ARCH_INCLUDED */
