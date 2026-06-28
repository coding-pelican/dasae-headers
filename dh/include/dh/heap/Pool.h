/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Pool.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-04-23 (date of creation)
 * @updated 2026-04-23 (date of last update)
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_Pool
 */
#pragma once
#ifndef heap_Pool__included
#define heap_Pool__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Arena.h"

/*========== Macros and Declarations ========================================*/

T_alias$((heap_Pool_Opts)(struct heap_Pool_Opts {
    var_(growable, bool);
}));
$attr($inline_always)
$static fn_((heap_Pool_Opts_default(void))(heap_Pool_Opts));

T_alias$((heap_Pool_ResetMode)(heap_Arena_ResetMode));

#define heap_Pool$$(_T...) __comp_anon__heap_Pool$$(_T)
#define heap_Pool$(_T...) __comp_alias__heap_Pool$(_T)
#define T_decl_heap_Pool$(_T...) __comp_gen__T_decl_heap_Pool$(_T)
#define T_impl_heap_Pool$(_T...) __comp_gen__T_impl_heap_Pool$(_T)
#define T_use_heap_Pool$(_T...) __comp_gen__T_use_heap_Pool$(_T)
T_alias$((heap_Pool)(struct heap_Pool {
    var_(arena_state, heap_Arena_State);
    var_(free_list, ListSgl);
    var_(opts, heap_Pool_Opts);
    var_(type, debug_TypeInfo);
}));
T_use_E$($set(mem_E)(heap_Pool));

$extern fn_((heap_Pool_empty(TypeInfo type, heap_Pool_Opts opts))(heap_Pool));
$attr($must_check)
$extern fn_((heap_Pool_init(TypeInfo type, mem_Alctr child_alctr, usize cap, heap_Pool_Opts opts))(mem_E$heap_Pool));
$extern fn_((heap_Pool_fini(heap_Pool* self, TypeInfo type, mem_Alctr child_alctr))(void));

$attr($must_check)
$extern fn_((heap_Pool_addCap(heap_Pool* self, TypeInfo type, mem_Alctr child_alctr, usize additional))(mem_E$void));
$extern fn_((heap_Pool_reset(heap_Pool* self, TypeInfo type, mem_Alctr child_alctr, heap_Pool_ResetMode mode))(bool));

$attr($must_check)
$extern fn_((heap_Pool_create(heap_Pool* self, TypeInfo type, mem_Alctr child_alctr))(mem_E$u_P$raw));
$extern fn_((heap_Pool_destroy(heap_Pool* self, u_P$raw ptr))(void));

/*========== Macros and Definitions =========================================*/

#if on_analysis_active_only || on_comptime
fn_((heap_Pool_Opts_default(void))(heap_Pool_Opts)) {
    return (heap_Pool_Opts){
        .growable = true,
    };
};
#endif /* on_analysis_active_only || on_comptime */

#define __comp_anon__heap_Pool$$(_T...) \
    union { \
        T_embed$(struct { \
            var_(arena_state, heap_Arena_State); \
            var_(free_list, ListSgl$$(_T)); \
            var_(opts, heap_Pool_Opts); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, heap_Pool) $like_ref; \
    }
#define __comp_alias__heap_Pool$(_T...) tpl$(heap_Pool, _T)
#define __comp_gen__T_decl_heap_Pool$(_T...) \
    $maybe_unused typedef union heap_Pool$(_T) heap_Pool$(_T); \
    T_decl_E$($set(mem_E)(heap_Pool$(_T))); \
    T_decl_E$($set(mem_E)(P$(_T)))
#define __comp_gen__T_impl_heap_Pool$(_T...) \
    union heap_Pool$(_T) { \
        T_embed$(struct { \
            var_(arena_state, heap_Arena_State); \
            var_(free_list, ListSgl$(_T)); \
            var_(opts, heap_Pool_Opts); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, heap_Pool) $like_ref; \
    }; \
    T_impl_E$($set(mem_E)(heap_Pool$(_T))); \
    T_impl_E$($set(mem_E)(P$(_T)))
#define __comp_gen__T_use_heap_Pool$(_T...) \
    T_decl_heap_Pool$(_T); \
    T_impl_heap_Pool$(_T)

/* clang-format off */
#define T_use_heap_Pool_empty$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(heap_Pool_empty, _T)(heap_Pool_Opts opts))(heap_Pool$(_T))) { \
        return type$((heap_Pool$(_T))(heap_Pool_empty(typeInfo$(_T), opts))); \
    }
#define T_use_heap_Pool_init$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(heap_Pool_init, _T)(mem_Alctr child_alctr, usize cap, heap_Pool_Opts opts))(E$($set(mem_E)(heap_Pool$(_T)))) $scope) { \
        return_(typeE$((ReturnT)(heap_Pool_init(typeInfo$(_T), child_alctr, cap, opts)))); \
    } $unscoped(fn)
#define T_use_heap_Pool_fini$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(heap_Pool_fini, _T)(heap_Pool$(_T)* self, mem_Alctr child_alctr))(void)) { \
        return heap_Pool_fini(self->as_raw, typeInfo$(_T), child_alctr); \
    }
#define T_use_heap_Pool_addCap$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(heap_Pool_addCap, _T)(heap_Pool$(_T)* self, mem_Alctr child_alctr, usize additional))(mem_E$void)) { \
        return heap_Pool_addCap(self->as_raw, typeInfo$(_T), child_alctr, additional); \
    }
#define T_use_heap_Pool_reset$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(heap_Pool_reset, _T)(heap_Pool$(_T)* self, mem_Alctr child_alctr, heap_Pool_ResetMode mode))(bool)) { \
        return heap_Pool_reset(self->as_raw, typeInfo$(_T), child_alctr, mode); \
    }
#define T_use_heap_Pool_create$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(heap_Pool_create, _T)(heap_Pool$(_T)* self, mem_Alctr child_alctr))(E$($set(mem_E)(P$(_T)))) $scope) { \
        return_(u_castE$((ReturnT)(heap_Pool_create(self->as_raw, typeInfo$(_T), child_alctr)))); \
    } $unscoped(fn)
#define T_use_heap_Pool_destroy$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(heap_Pool_destroy, _T)(heap_Pool$(_T)* self, _T* ptr))(void)) { \
        return heap_Pool_destroy(self->as_raw, u_anyP(ptr)); \
    }
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_Pool__included */
