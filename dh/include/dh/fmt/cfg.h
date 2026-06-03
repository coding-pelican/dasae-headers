/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-22 (date of creation)
 * @updated 2026-06-03 (date of last update)
 * @ingroup dasae-headers(dh)/fmt
 * @prefix  fmt
 *
 * @brief   Configuration settings for fmt
 * @details Defines compile-time options and configuration parameters for fmt.
 */
#ifndef fmt_cfg__included
#define fmt_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

/* --- Version Configuration --- */

/**
 * @brief Define this to use small tables (~816 bytes instead of ~10.7 KB)
 *
 * Small version:
 * - Binary size: ~816 bytes (13x smaller)
 * - Speed: ~95% of full version
 * - Use case: Embedded systems, size-critical applications
 *
 * Full version (default):
 * - Binary size: ~10.7 KB
 * - Speed: Maximum performance
 * - Use case: Desktop, server, general applications
 */
#define fmt_flt_ryu_table_type __comp_enum__fmt_flt_ryu_table_type
#define fmt_flt_ryu_table_type_unknown __comp_enum__fmt_flt_ryu_table_type_unknown
#define fmt_flt_ryu_table_type_tableless __comp_enum__fmt_flt_ryu_table_type_tableless
#define fmt_flt_ryu_table_type_small __comp_enum__fmt_flt_ryu_table_type_small
#define fmt_flt_ryu_table_type_full __comp_enum__fmt_flt_ryu_table_type_full

#if !defined(fmt_comp_use_flt_ryu_tableless)
#define fmt_comp_use_flt_ryu_tableless __comp_bool__fmt_comp_use_flt_ryu_tableless
#endif /* !defined(fmt_comp_use_flt_ryu_tableless) */
#define fmt__comp_use_flt_ryu_tableless_default __comp_flag__fmt__comp_use_flt_ryu_tableless_default
#if !defined(fmt_comp_use_flt_ryu_table_small)
#define fmt_comp_use_flt_ryu_table_small __comp_bool__fmt_comp_use_flt_ryu_table_small
#endif /* !defined(fmt_comp_use_flt_ryu_table_small) */
#define fmt__comp_use_flt_ryu_table_small_default __comp_flag__fmt__comp_use_flt_ryu_table_small_default
#if !defined(fmt_comp_use_flt_ryu_table_full)
#define fmt_comp_use_flt_ryu_table_full __comp_bool__fmt_comp_use_flt_ryu_table_full
#endif /* !defined(fmt_comp_use_flt_ryu_table_full) */
#define fmt__comp_use_flt_ryu_table_full_default __comp_flag__fmt__comp_use_flt_ryu_table_full_default

/*========== Macros and Definitions =========================================*/

/* Default values */

#define __comp_enum__fmt_flt_ryu_table_type pp_if_(fmt_comp_use_flt_ryu_tableless)( \
    pp_then_(fmt_flt_ryu_table_type_tableless), \
    pp_else_(pp_if_(fmt_comp_use_flt_ryu_table_small)( \
        pp_then_(fmt_flt_ryu_table_type_small), \
        pp_else_(pp_if_(fmt_comp_use_flt_ryu_table_full)( \
            pp_then_(fmt_flt_ryu_table_type_full), \
            pp_else_(fmt_flt_ryu_table_type_unknown) \
        )) \
    )) \
)
#define __comp_enum__fmt_flt_ryu_table_type_unknown 0
#define __comp_enum__fmt_flt_ryu_table_type_tableless 1
#define __comp_enum__fmt_flt_ryu_table_type_small 2
#define __comp_enum__fmt_flt_ryu_table_type_full 3

#define __comp_bool__fmt_comp_use_flt_ryu_tableless fmt__comp_use_flt_ryu_tableless_default
#define __comp_flag__fmt__comp_use_flt_ryu_tableless_default pp_false
#define __comp_bool__fmt_comp_use_flt_ryu_table_small fmt__comp_use_flt_ryu_table_small_default
#define __comp_flag__fmt__comp_use_flt_ryu_table_small_default pp_false
#define __comp_bool__fmt_comp_use_flt_ryu_table_full fmt__comp_use_flt_ryu_table_full_default
#define __comp_flag__fmt__comp_use_flt_ryu_table_full_default pp_true

/* Override values */

#if defined(FMT_USE_FLT_RYU_TABLELESS)
#undef __comp_flag__fmt__comp_use_flt_ryu_tableless_default
#define __comp_flag__fmt__comp_use_flt_ryu_tableless_default pp_true
#endif /* defined(FMT_USE_FLT_RYU_TABLELESS) */

#if defined(FMT_USE_FLT_RYU_TABLE_SMALL)
#undef __comp_flag__fmt__comp_use_flt_ryu_table_small_default
#define __comp_flag__fmt__comp_use_flt_ryu_table_small_default pp_true
#endif /* defined(FMT_USE_FLT_RYU_TABLE_SMALL) */

#if defined(FMT_USE_FLT_RYU_TABLE_FULL)
#undef __comp_flag__fmt__comp_use_flt_ryu_table_full_default
#define __comp_flag__fmt__comp_use_flt_ryu_table_full_default pp_true
#endif /* defined(FMT_USE_FLT_RYU_TABLE_FULL) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fmt_cfg__included */
