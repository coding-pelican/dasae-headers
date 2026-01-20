/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-23 (date of creation)
 * @updated 2025-03-27 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap
 *
 * @brief   Configuration settings for heap management
 * @details Defines compile-time options and configuration parameters
 *          for memory allocation behavior and safety features.
 */
#ifndef heap_cfg__included
#define heap_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/mem/Allocator.h"

/*========== Macros and Declarations ========================================*/

#define heap_page_size __val__heap_page_size
#define heap_page_size_default __opt__heap_page_size_default

/*========== Macros and Definitions =========================================*/

#define __val__heap_page_size \
    pp_orelse_((heap_page_size_default)(1))
#define __opt__heap_page_size_default pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_case_((plat_type_windows)(pp_some(4 << 10))), \
        pp_case_((plat_type_linux)(pp_some(4 << 10))), \
        pp_case_((plat_type_darwin)(pp_some(4 << 10))), \
        pp_case_((plat_type_wasi)(pp_if_(arch_is_wasm_family)( \
            pp_then_(pp_some(64 << 10)), \
            pp_else_(claim_assert_static(false)) \
        ))), \
        pp_default_(pp_none()) \
    ) pp_end \
)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_cfg__included */
