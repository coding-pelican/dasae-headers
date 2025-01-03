/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    comp.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-03 (date of creation)
 * @updated 2024-11-22 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  NONE
 *
 * @brief   Compiler-specific configurations and optimizations
 * @details Provides compiler-specific inline directives and optimizations based on detected environment
 */

#ifndef BUILTIN_COMP_INCLUDED
#define BUILTIN_COMP_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "arch_cfg.h"
#include "comp_cfg.h"
#include "lang_cfg.h"
#include "pltf_cfg.h"

/* Inline Definitions Based on Language Mode */
#if BUILTIN_LANG_MODE_CPP
/* C++ always supports 'inline' */
#define static_inline static inline
#define extern_inline inline
#elif BUILTIN_LANG_C_99
/* C99 or later */
#define static_inline static inline
#define extern_inline inline
#else
/* Use compiler-specific inline directives */
#define static_inline BUILTIN_COMP_INLINE static
#define extern_inline BUILTIN_COMP_INLINE
#endif

/* Force Inline Definition */
#define force_inline BUILTIN_COMP_FORCE_INLINE

/* Must Check Definition */
#define must_check BUILTIN_COMP_MUST_CHECK

#define eval(expr...) ({ expr; })
#define eval_return   /* just comment */

#if !defined(COMP)
#define COMP_TIME (0)
#else
#define COMP_TIME (1)
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_COMP_INCLUDED */
