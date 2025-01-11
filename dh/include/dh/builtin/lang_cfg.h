/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    lang_cfg.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2024-11-22 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  BUILTIN_LANG
 *
 * @brief   Language standards detection configuration
 * @details Detects C/C++ language standards, features, and compilation modes.
 *          Includes standard versions, language extensions, and dialect support.
 */

#ifndef BUILTIN_LANG_CFG_INCLUDED
#define BUILTIN_LANG_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== C Language Standard Detection ==================================*/
#if defined(__STDC__)
#define BUILTIN_LANG_STDC (1)
#if defined(__STDC_VERSION__)
#define BUILTIN_LANG_C_90 (1)
#define BUILTIN_LANG_C_99 (__STDC_VERSION__ >= 199901L)
#define BUILTIN_LANG_C_11 (__STDC_VERSION__ >= 201112L)
#define BUILTIN_LANG_C_17 (__STDC_VERSION__ >= 201710L)
#define BUILTIN_LANG_C_23 (__STDC_VERSION__ >= 202311L)
#else
#define BUILTIN_LANG_C_90 (1)
#define BUILTIN_LANG_C_99 (0)
#define BUILTIN_LANG_C_11 (0)
#define BUILTIN_LANG_C_17 (0)
#define BUILTIN_LANG_C_23 (0)
#endif
#endif

/*========== C++ Language Standard Detection ================================*/
#if defined(__cplusplus)
#define BUILTIN_LANG_CPP    (1)
#define BUILTIN_LANG_CPP_98 (__cplusplus >= 199711L)
#define BUILTIN_LANG_CPP_11 (__cplusplus >= 201103L)
#define BUILTIN_LANG_CPP_14 (__cplusplus >= 201402L)
#define BUILTIN_LANG_CPP_17 (__cplusplus >= 201703L)
#define BUILTIN_LANG_CPP_20 (__cplusplus >= 202002L)
#define BUILTIN_LANG_CPP_23 (__cplusplus >= 202302L)
#else
#define BUILTIN_LANG_CPP    (0)
#define BUILTIN_LANG_CPP_98 (0)
#define BUILTIN_LANG_CPP_11 (0)
#define BUILTIN_LANG_CPP_14 (0)
#define BUILTIN_LANG_CPP_17 (0)
#define BUILTIN_LANG_CPP_20 (0)
#define BUILTIN_LANG_CPP_23 (0)
#endif

/*========== Language Mode and Features =====================================*/
#if !defined(__cplusplus)
#define BUILTIN_LANG_MODE_C   (1)
#define BUILTIN_LANG_MODE_CPP (0)
#else
#define BUILTIN_LANG_MODE_C   (0)
#define BUILTIN_LANG_MODE_CPP (1)
#endif

/* Language Extensions */
#if defined(__GNUC__)
#define BUILTIN_LANG_GNU_EXTENSIONS (1)
#endif
#if defined(_MSC_EXTENSIONS)
#define BUILTIN_LANG_MS_EXTENSIONS (1)
#endif

/* Strict Standards Conformance */
#if defined(__STRICT_ANSI__)
#define BUILTIN_LANG_STRICT_STDC (1)
#endif
#if defined(__STRICT_ANSI__) && defined(__cplusplus)
#define BUILTIN_LANG_STRICT_CPP (1)
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_LANG_CFG_INCLUDED */
