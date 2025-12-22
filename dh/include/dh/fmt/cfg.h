/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-22 (date of creation)
 * @updated 2025-12-22 (date of last update)
 * @version v0.1-alpha
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
#define fmt_comp_use_flt_ryu_table_small __comp_bool__fmt_comp_use_flt_ryu_table_small
#define fmt__comp_use_flt_ryu_table_small_default __comp_flag__fmt__comp_use_flt_ryu_table_small_default

#define fmt_flt_ryu_table_type __comp_enum__fmt_flt_ryu_table_type
#define fmt_flt_ryu_table_type_small __comp_enum__fmt_flt_ryu_table_type_small
#define fmt_flt_ryu_table_type_full __comp_enum__fmt_flt_ryu_table_type_full

/*========== Macros and Definitions =========================================*/

/* Default values */
#define __comp_bool__fmt_comp_use_flt_ryu_table_small fmt__comp_use_flt_ryu_table_small_default
#define __comp_flag__fmt__comp_use_flt_ryu_table_small_default pp_false

#define __comp_enum__fmt_flt_ryu_table_type pp_if_(fmt_comp_use_flt_ryu_table_small)( \
    pp_then_(fmt_flt_ryu_table_type_small), \
    pp_else_(fmt_flt_ryu_table_type_full) \
)
#define __comp_enum__fmt_flt_ryu_table_type_small 0
#define __comp_enum__fmt_flt_ryu_table_type_full 1

/* Override values */

#if defined(FMT_USE_FLT_RYU_TABLE_SMALL)
#undef __comp_bool__fmt_comp_use_flt_ryu_table_small
#define __comp_bool__fmt_comp_use_flt_ryu_table_small pp_true
#endif /* defined(FMT_USE_FLT_RYU_TABLE_SMALL) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fmt_cfg__included */
