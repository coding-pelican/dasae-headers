/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    AlcTrace.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-09 (date of creation)
 * @updated 2026-02-26 (date of last update)
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem_AlcTrace
 *
 * @brief   Debug memory tracking system
 * @details Tracks memory allocations, frees, remaps, and detects issues like
 *          memory leaks, double frees, and invalid frees. Provides detailed
 *          reports with allocation source locations and timestamps.
 */
#ifndef mem_AlcTrace__included
#define mem_AlcTrace__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Alctr.h"
#include "../io/Writer.h"
#include "../ListSgl.h"

/*========== Macros and Declarations ========================================*/

/// Memory Tracer
T_alias$((mem_AlcTrace)(pp_if_(mem_tracing_comp_enabled)(
    pp_then_(struct mem_AlcTrace),
    pp_else_(Void))));

/// Initialize memory tracker with custom log path
$attr($must_check)
$extern fn_((mem_AlcTrace_initWithLogPath(S_const$u8 log_path))(E$void));
/// Generate final report and cleanup
$extern fn_((mem_AlcTrace_finiAndGenerateReport(void))(void));

/// Register allocation (e.g., rawAlloc)
$extern fn_((mem_AlcTrace_registerAlloc($traced P$raw ptr, usize size))(void));
/// Register memory remapping (e.g., rawResize/rawRemap)
$extern fn_((mem_AlcTrace_registerRemap($traced P$raw old_ptr, P$raw new_ptr, usize new_size))(void));
/// Register deallocation (e.g., rawFree)
$extern fn_((mem_AlcTrace_registerFree($traced P$raw ptr))(bool));

/*========== Macros and Definitions =========================================*/

pp_if_(mem_tracing_comp_enabled)((
    T_alias$((mem_AlcTrace_Unit)(struct mem_AlcTrace_Unit));
    T_use$((mem_AlcTrace_Unit)(ListSgl_Link, ListSgl));
    struct mem_AlcTrace {
        var_(writer, io_Writer);
        var_(gpa, mem_Alctr);
        var_(alc_list, ListSgl$mem_AlcTrace_Unit);
        var_(alc_total_count, usize);
        var_(alc_active_count, usize);
    };
));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* mem_AlcTrace__included */
