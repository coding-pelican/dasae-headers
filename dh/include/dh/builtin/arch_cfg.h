#ifndef BUILTIN_ARCH_INCLUDED
#define BUILTIN_ARCH_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/* Architecture detection for proper alignment */
#if defined(__x86_64__) || defined(_M_X64)
/* 64-bit */
#define BUILTIN_ARCH_64BIT (1)
#define BUILTIN_ARCH_32BIT (0)
#elif defined(__i386__) || defined(_M_IX86)
/* 32-bit */
#define BUILTIN_ARCH_64BIT (0)
#define BUILTIN_ARCH_32BIT (1)
#else
// #  error "Unknown architecture"
#endif

#define arch_64bit (BUILTIN_ARCH_64BIT)
#define arch_32bit (BUILTIN_ARCH_32BIT)


#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* BUILTIN_ARCH_INCLUDED */
