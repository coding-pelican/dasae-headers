/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Sys.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-20 (date of creation)
 * @updated 2026-05-09 (date of last update)
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_Sys
 *
 * @brief   Default system allocator surface
 * @details Selects the platform-appropriate system allocator backend.
 *          - Windows/POSIX: `heap_Page`
 *          - WASI: `heap_Sbrk` on top of `heap_Sbrk_Sys_Wasm`
 */
#pragma once
#ifndef heap_Sys__included
#define heap_Sys__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Page.h"
#include "Sbrk.h"

/*========== Macros and Declarations ========================================*/

errset_((heap_Sys_E)(heap_Sys_Unsupported) $union_errset_(heap_VMap_E, heap_VMem_E));

T_alias$((heap_Sys__Impl)(pp_if_(plat_is_wasi)(
    pp_then_(struct heap_Sys__Impl {
        var_(local, heap_Sbrk_local_Large);
        var_(sys_wasm, heap_Sbrk_Sys_Wasm);
        var_(ctx, heap_Sbrk_Ctx);
        var_(sbrk, heap_Sbrk);
    }),
    pp_else_(struct heap_Sys__Impl {
        var_(page, heap_Page);
    }))));
/// Default system allocator instance
T_alias$((heap_Sys)(struct heap_Sys {
    var_(_impl, heap_Sys__Impl);
}));
T_use_E$($set(heap_Sys_E)(heap_Sys));
T_use_E$($set(heap_Sys_E)(mem_Alctr));
/// Get allocator interface for instance
$extern fn_((heap_Sys_alctr(heap_Sys* self))(mem_Alctr));
/// Get thread-safe allocator interface for instance
$attr($must_check)
$extern fn_((heap_Sys_thrdSafeAlctr(heap_Sys* self))(heap_Sys_E$mem_Alctr));

/// Create an initialized system allocator instance
$attr($must_check)
$extern fn_((heap_Sys_init(void))(heap_Sys_E$heap_Sys));
/// Finalize platform-specific state and clear the instance
$extern fn_((heap_Sys_fini(heap_Sys* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_Sys__included */
