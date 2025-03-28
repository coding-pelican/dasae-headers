/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    bti_lang.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2025-03-28 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/bti
 * @prefix  bti_lang
 *
 * @brief   Language standards detection and configuration
 * @details Detects C/C++ language standards, features, and compilation modes.
 *          Includes standard versions, language extensions, and dialect support.
 *          Part of the BTI (Built-in) module for core system configuration.
 */

#ifndef BTI_LANG_INCLUDED
#define BTI_LANG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

/* Enumerable Language Properties */

#define bti_lang_mode                             \
    /**                                           \
     * @brief Current language mode               \
     * @return Enumerated value for language mode \
     * @see bti_lang_mode_* constants             \
     */                                           \
    comp_value__bti_lang_mode
/* Language Mode Constants */
#define bti_lang_mode_unknown (0)
#define bti_lang_mode_c       (1)
#define bti_lang_mode_cpp     (2)
#define bti_lang_mode_count   (3)

#define bti_lang_c_ver                         \
    /**                                        \
     * @brief Current C standard version       \
     * @return Enumerated value for C standard \
     * @see bti_lang_c_ver_* constants         \
     */                                        \
    comp_value__bti_lang_c_ver
/* C Standard Version Constants */
#define bti_lang_c_ver_unknown (0)
#define bti_lang_c_ver_90      (1)
#define bti_lang_c_ver_99      (2)
#define bti_lang_c_ver_11      (3)
#define bti_lang_c_ver_17      (4)
#define bti_lang_c_ver_23      (5)
#define bti_lang_c_ver_count   (6)

#define bti_lang_cpp_ver                         \
    /**                                          \
     * @brief Current C++ standard version       \
     * @return Enumerated value for C++ standard \
     * @see bti_lang_cpp_ver_* constants         \
     */                                          \
    comp_value__bti_lang_cpp_ver
/* C++ Standard Version Constants */
#define bti_lang_cpp_ver_unknown (0)
#define bti_lang_cpp_ver_98      (1)
#define bti_lang_cpp_ver_11      (2)
#define bti_lang_cpp_ver_14      (3)
#define bti_lang_cpp_ver_17      (4)
#define bti_lang_cpp_ver_20      (5)
#define bti_lang_cpp_ver_23      (6)
#define bti_lang_cpp_ver_count   (7)

/* C Language Standard Detection */

#define bti_lang_detects_stdc                           \
    /**                                                 \
     * @brief Standard C implementation detection       \
     * @return 1 if compiler claims standard compliance \
     */                                                 \
    comp_bool__bti_lang_detects_stdc
#define bti_lang_detects_c_90                \
    /**                                      \
     * @brief C90 standard support detection \
     * @return 1 if C90 or later supported   \
     */                                      \
    comp_bool__bti_lang_detects_c_90
#define bti_lang_detects_c_99                \
    /**                                      \
     * @brief C99 standard support detection \
     * @return 1 if C99 or later supported   \
     */                                      \
    comp_bool__bti_lang_detects_c_99
#define bti_lang_detects_c_11                \
    /**                                      \
     * @brief C11 standard support detection \
     * @return 1 if C11 or later supported   \
     */                                      \
    comp_bool__bti_lang_detects_c_11
#define bti_lang_detects_c_17                \
    /**                                      \
     * @brief C17 standard support detection \
     * @return 1 if C17 or later supported   \
     */                                      \
    comp_bool__bti_lang_detects_c_17
#define bti_lang_detects_c_23                \
    /**                                      \
     * @brief C23 standard support detection \
     * @return 1 if C23 supported            \
     */                                      \
    comp_bool__bti_lang_detects_c_23

/* C++ Language Standard Detection */

#define bti_lang_detects_cpp         \
    /**                              \
     * @brief C++ language detection \
     * @return 1 if compiling as C++ \
     */                              \
    comp_bool__bti_lang_detects_cpp
#define bti_lang_detects_cpp_98                \
    /**                                        \
     * @brief C++98 standard support detection \
     * @return 1 if C++98 or later supported   \
     */                                        \
    comp_bool__bti_lang_detects_cpp_98
#define bti_lang_detects_cpp_11                \
    /**                                        \
     * @brief C++11 standard support detection \
     * @return 1 if C++11 or later supported   \
     */                                        \
    comp_bool__bti_lang_detects_cpp_11
#define bti_lang_detects_cpp_14                \
    /**                                        \
     * @brief C++14 standard support detection \
     * @return 1 if C++14 or later supported   \
     */                                        \
    comp_bool__bti_lang_detects_cpp_14
#define bti_lang_detects_cpp_17                \
    /**                                        \
     * @brief C++17 standard support detection \
     * @return 1 if C++17 or later supported   \
     */                                        \
    comp_bool__bti_lang_detects_cpp_17
#define bti_lang_detects_cpp_20                \
    /**                                        \
     * @brief C++20 standard support detection \
     * @return 1 if C++20 or later supported   \
     */                                        \
    comp_bool__bti_lang_detects_cpp_20
#define bti_lang_detects_cpp_23                \
    /**                                        \
     * @brief C++23 standard support detection \
     * @return 1 if C++23 supported            \
     */                                        \
    comp_bool__bti_lang_detects_cpp_23

/* Language Mode */

#define bti_lang_detects_mode_c         \
    /**                                 \
     * @brief C language mode detection \
     * @return 1 if compiling as C      \
     */                                 \
    comp_bool__bti_lang_detects_mode_c
#define bti_lang_detects_mode_cpp         \
    /**                                   \
     * @brief C++ language mode detection \
     * @return 1 if compiling as C++      \
     */                                   \
    comp_bool__bti_lang_detects_mode_cpp

/* Language Extensions */

#define bti_lang_has_exts_gnu                   \
    /**                                         \
     * @brief GNU language extensions detection \
     * @return 1 if GNU extensions available    \
     */                                         \
    comp_bool__bti_lang_has_exts_gnu
#define bti_lang_has_exts_ms                          \
    /**                                               \
     * @brief Microsoft language extensions detection \
     * @return 1 if Microsoft extensions available    \
     */                                               \
    comp_bool__bti_lang_has_exts_ms

/* Strict Standards Conformance */

#define bti_lang_detects_strict_stdc               \
    /**                                            \
     * @brief Strict standard C mode detection     \
     * @return 1 if strict standard C mode enabled \
     */                                            \
    comp_bool__bti_lang_detects_strict_stdc
#define bti_lang_detects_strict_cpp                  \
    /**                                              \
     * @brief Strict standard C++ mode detection     \
     * @return 1 if strict standard C++ mode enabled \
     */                                              \
    comp_bool__bti_lang_detects_strict_cpp

/*========== Macros and Definitions =========================================*/

/* C Language Standard Detection - Implementations */

#if defined(__STDC__)
#define comp_bool__bti_lang_detects_stdc (1)
#if defined(__STDC_VERSION__)
#define comp_bool__bti_lang_detects_c_90 (1)
#define comp_bool__bti_lang_detects_c_99 (__STDC_VERSION__ >= 199901L)
#define comp_bool__bti_lang_detects_c_11 (__STDC_VERSION__ >= 201112L)
#define comp_bool__bti_lang_detects_c_17 (__STDC_VERSION__ >= 201710L)
#define comp_bool__bti_lang_detects_c_23 (__STDC_VERSION__ >= 202311L)
#else
#define comp_bool__bti_lang_detects_c_90 (1)
#define comp_bool__bti_lang_detects_c_99 (0)
#define comp_bool__bti_lang_detects_c_11 (0)
#define comp_bool__bti_lang_detects_c_17 (0)
#define comp_bool__bti_lang_detects_c_23 (0)
#endif
#endif

/* C++ Language Standard Detection - Implementations */

#if defined(__cplusplus)
#define comp_bool__bti_lang_detects_cpp    (1)
#define comp_bool__bti_lang_detects_cpp_98 (__cplusplus >= 199711L)
#define comp_bool__bti_lang_detects_cpp_11 (__cplusplus >= 201103L)
#define comp_bool__bti_lang_detects_cpp_14 (__cplusplus >= 201402L)
#define comp_bool__bti_lang_detects_cpp_17 (__cplusplus >= 201703L)
#define comp_bool__bti_lang_detects_cpp_20 (__cplusplus >= 202002L)
#define comp_bool__bti_lang_detects_cpp_23 (__cplusplus >= 202302L)
#else
#define comp_bool__bti_lang_detects_cpp    (0)
#define comp_bool__bti_lang_detects_cpp_98 (0)
#define comp_bool__bti_lang_detects_cpp_11 (0)
#define comp_bool__bti_lang_detects_cpp_14 (0)
#define comp_bool__bti_lang_detects_cpp_17 (0)
#define comp_bool__bti_lang_detects_cpp_20 (0)
#define comp_bool__bti_lang_detects_cpp_23 (0)
#endif

/* Language Mode and Features - Implementations */

#if !defined(__cplusplus)
#define comp_bool__bti_lang_detects_mode_c   (1)
#define comp_bool__bti_lang_detects_mode_cpp (0)
#define comp_value__bti_lang_mode            bti_lang_mode_c
#else
#define comp_bool__bti_lang_detects_mode_c   (0)
#define comp_bool__bti_lang_detects_mode_cpp (1)
#define comp_value__bti_lang_mode            bti_lang_mode_cpp
#endif

/* Language Extensions - Implementations */

#if defined(__GNUC__)
#define comp_bool__bti_lang_has_exts_gnu (1)
#else
#define comp_bool__bti_lang_has_exts_gnu (0)
#endif

#if defined(_MSC_EXTENSIONS)
#define comp_bool__bti_lang_has_exts_ms (1)
#else
#define comp_bool__bti_lang_has_exts_ms (0)
#endif

/* Strict Standards Conformance - Implementations */

#if defined(__STRICT_ANSI__)
#define comp_bool__bti_lang_detects_strict_stdc (1)
#else
#define comp_bool__bti_lang_detects_strict_stdc (0)
#endif

#if defined(__STRICT_ANSI__) && defined(__cplusplus)
#define comp_bool__bti_lang_detects_strict_cpp (1)
#else
#define comp_bool__bti_lang_detects_strict_cpp (0)
#endif

/* Language Version Enumeration - Implementations */

#if bti_lang_detects_c_23
#define comp_value__bti_lang_c_ver bti_lang_c_ver_23
#elif bti_lang_detects_c_17
#define comp_value__bti_lang_c_ver bti_lang_c_ver_17
#elif bti_lang_detects_c_11
#define comp_value__bti_lang_c_ver bti_lang_c_ver_11
#elif bti_lang_detects_c_99
#define comp_value__bti_lang_c_ver bti_lang_c_ver_99
#elif bti_lang_detects_c_90
#define comp_value__bti_lang_c_ver bti_lang_c_ver_90
#else
#define comp_value__bti_lang_c_ver bti_lang_c_ver_unknown
#endif

#if bti_lang_detects_cpp_23
#define comp_value__bti_lang_cpp_ver bti_lang_cpp_ver_23
#elif bti_lang_detects_cpp_20
#define comp_value__bti_lang_cpp_ver bti_lang_cpp_ver_20
#elif bti_lang_detects_cpp_17
#define comp_value__bti_lang_cpp_ver bti_lang_cpp_ver_17
#elif bti_lang_detects_cpp_14
#define comp_value__bti_lang_cpp_ver bti_lang_cpp_ver_14
#elif bti_lang_detects_cpp_11
#define comp_value__bti_lang_cpp_ver bti_lang_cpp_ver_11
#elif bti_lang_detects_cpp_98
#define comp_value__bti_lang_cpp_ver bti_lang_cpp_ver_98
#else
#define comp_value__bti_lang_cpp_ver bti_lang_cpp_ver_unknown
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_LANG_INCLUDED */
