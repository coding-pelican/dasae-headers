/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    comp_cfg.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2025-03-28 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/bti
 * @prefix  bti_comp
 *
 * @brief   Compiler detection and configuration
 * @details Detects compiler type (Clang, GCC, MSVC) and provides version information.
 *          Part of the BTI (Built-in) module for core system configuration.
 */

#ifndef BTI_COMP_CFG_INCLUDED
#define BTI_COMP_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations =========================================*/

/* Enumerable Compiler Properties */

#define bti_comp_type                                 \
    /**                                               \
     * @brief Enumerated compiler type                \
     * @return Constant identifying the compiler type \
     * @see bti_comp_type_* constants                 \
     */                                               \
    comp_value__bti_comp_type
/* Compiler Type Constants */
#define bti_comp_type_unknown (0)
#define bti_comp_type_clang   (1)
#define bti_comp_type_gcc     (2)
#define bti_comp_type_msvc    (3)
#define bti_comp_type_count   (4)

#define bti_comp_name                               \
    /**                                             \
     * @brief Compiler name as a string             \
     * @return "Clang", "GCC", "MSVC", or "Unknown" \
     */                                             \
    comp_detect__bti_comp_name

/* Compiler Type Detection */

#define bti_comp_clang                                         \
    /**                                                        \
     * @brief Clang compiler detection                         \
     * @return 1 if compiling with Clang compiler, 0 otherwise \
     */                                                        \
    comp_detect__bti_comp_clang
#define bti_comp_gcc                                         \
    /**                                                      \
     * @brief GCC compiler detection                         \
     * @return 1 if compiling with GCC compiler, 0 otherwise \
     */                                                      \
    comp_detect__bti_comp_gcc
#define bti_comp_msvc                                         \
    /**                                                       \
     * @brief MSVC compiler detection                         \
     * @return 1 if compiling with MSVC compiler, 0 otherwise \
     */                                                       \
    comp_detect__bti_comp_msvc

/* Compiler Version Information */

#define bti_comp_version                                  \
    /**                                                   \
     * @brief Compiler version as a numeric value         \
     * @return Version number in compiler-specific format \
     */                                                   \
    comp_value__bti_comp_version
#define bti_comp_version_major                       \
    /**                                              \
     * @brief Major version number of the compiler   \
     * @return Major version component as an integer \
     */                                              \
    comp_value__bti_comp_version_major
#define bti_comp_version_minor                       \
    /**                                              \
     * @brief Minor version number of the compiler   \
     * @return Minor version component as an integer \
     */                                              \
    comp_value__bti_comp_version_minor
#define bti_comp_version_patch                       \
    /**                                              \
     * @brief Patch version number of the compiler   \
     * @return Patch version component as an integer \
     */                                              \
    comp_value__bti_comp_version_patch

/*========== Macros and Definitions =========================================*/

/* Compiler Type Detection - Implementations */

#if defined(__clang__)
#define comp_detect__bti_comp_clang (1)
#define comp_detect__bti_comp_gcc   (0)
#define comp_detect__bti_comp_msvc  (0)
#define comp_detect__bti_comp_name  "Clang"
#define comp_value__bti_comp_type   bti_comp_type_clang
#elif defined(__GNUC__)
#define comp_detect__bti_comp_clang (0)
#define comp_detect__bti_comp_gcc   (1)
#define comp_detect__bti_comp_msvc  (0)
#define comp_detect__bti_comp_name  "GCC"
#define comp_value__bti_comp_type   bti_comp_type_gcc
#elif defined(_MSC_VER)
#define comp_detect__bti_comp_clang (0)
#define comp_detect__bti_comp_gcc   (0)
#define comp_detect__bti_comp_msvc  (1)
#define comp_detect__bti_comp_name  "MSVC"
#define comp_value__bti_comp_type   bti_comp_type_msvc
#else
#define comp_detect__bti_comp_clang (0)
#define comp_detect__bti_comp_gcc   (0)
#define comp_detect__bti_comp_msvc  (0)
#define comp_detect__bti_comp_name  "Unknown"
#define comp_value__bti_comp_type   bti_comp_type_unknown
#warning "Unknown compiler detected. Please check your compiler settings."
#endif

/* Compiler Version Information - Implementations */

#if comp_detect__bti_comp_clang
#define comp_value__bti_comp_version       (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#define comp_value__bti_comp_version_major __clang_major__
#define comp_value__bti_comp_version_minor __clang_minor__
#define comp_value__bti_comp_version_patch __clang_patchlevel__
#elif comp_detect__bti_comp_gcc
#define comp_value__bti_comp_version       (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#define comp_value__bti_comp_version_major __GNUC__
#define comp_value__bti_comp_version_minor __GNUC_MINOR__
#define comp_value__bti_comp_version_patch __GNUC_PATCHLEVEL__
#elif comp_detect__bti_comp_msvc
#define comp_value__bti_comp_version       _MSC_VER
#define comp_value__bti_comp_version_major (_MSC_VER / 100)
#define comp_value__bti_comp_version_minor (_MSC_VER % 100)
#define comp_value__bti_comp_version_patch (0)
#else
#define comp_value__bti_comp_version       (0)
#define comp_value__bti_comp_version_major (0)
#define comp_value__bti_comp_version_minor (0)
#define comp_value__bti_comp_version_patch (0)
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_COMP_CFG_INCLUDED */
