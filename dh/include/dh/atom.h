/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    atom.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-22 (date of creation)
 * @updated 2025-11-04 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)
 * @prefix  atom
 *
 * @brief   Cross-platform atomic operations
 * @details This provides atomic operations for primitive types to prevent data races.
 */
#ifndef atom__included
#define atom__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"

/*========== Macros and Declarations ========================================*/

#define atom_cache_line_bytes __comp_int__atom_cache_line_bytes

#define atom_V$(_T...) pp_join($, atom_V, _T)
#define atom_V$$(_T...) \
    struct { \
        volatile _T raw; \
    }
#define T_use_atom_V$(_T...) \
    T_decl_atom_V$(_T); \
    T_impl_atom_V$(_T)
#define T_decl_atom_V$(_T...) \
    typedef struct atom_V$(_T) atom_V$(_T);
#define T_impl_atom_V$(_T...) \
    struct atom_V$(_T) { \
        volatile _T raw; \
    }

#define atom_V_init(_val...) __op__atom_V_init(_val)
#define atom_V_init$(_VT, _val...) __op__atom_V_init$(_VT, _val)
#define atom_V_load(_p_self, _ord...) __op__atom_V_load(_p_self, _ord)
#define atom_V_store(_p_self, _val, _ord...) __op__atom_V_store(_p_self, _val, _ord)

#define atom_V_fetchXchg(_p_self, _val, _ord...) __op__atom_V_fetchXchg(_p_self, _val, _ord)
#define atom_V_fetchAdd(_p_self, _val, _ord...) __op__atom_V_fetchAdd(_p_self, _val, _ord)
#define atom_V_fetchSub(_p_self, _val, _ord...) __op__atom_V_fetchSub(_p_self, _val, _ord)
#define atom_V_fetchNand(_p_self, _val, _ord...) __op__atom_V_fetchNand(_p_self, _val, _ord)
#define atom_V_fetchAnd(_p_self, _val, _ord...) __op__atom_V_fetchAnd(_p_self, _val, _ord)
#define atom_V_fetchOr(_p_self, _val, _ord...) __op__atom_V_fetchOr(_p_self, _val, _ord)
#define atom_V_fetchXor(_p_self, _val, _ord...) __op__atom_V_fetchXor(_p_self, _val, _ord)

/// spinLoopHint: Platform-specific CPU hints for spin-loops
///
/// On various architectures, there are instructions to reduce power usage
/// or improve back-off behavior in tight spin loops, e.g.:
///   - x86/x86_64: 'pause'
///   - ARM64:      'isb' or 'yield' (sometimes 'wfe')
///   - ARMv6K:     'yield'
///   - RISC-V:     'pause' if Zihintpause is available
$attr($inline_always)
$static fn_((atom_spinLoopHint(void))(void));

/*========== Macros and Definitions =========================================*/

#define __comp_int__atom_cache_line_bytes arch_cache_line_bytes

#define __op__atom_V_init(_val...) { .raw = _val }
#define __op__atom_V_init$(_VT, _val...) lit$((_VT)atom_V_init(_val))
#define __op__atom_V_load(_p_self, _ord...) atom_load(&(_p_self)->raw, _ord)
#define __op__atom_V_store(_p_self, _val, _ord...) atom_store(&(_p_self)->raw, make$((TypeOf((_p_self)->raw))_val), _ord)

#define __op__atom_V_fetchXchg(_p_self, _val, _ord...) atom_fetchXchg(&(_p_self)->raw, make$((TypeOf((_p_self)->raw))_val), _ord)
#define __op__atom_V_fetchAdd(_p_self, _val, _ord...) atom_fetchAdd(&(_p_self)->raw, make$((TypeOf((_p_self)->raw))_val), _ord)
#define __op__atom_V_fetchSub(_p_self, _val, _ord...) atom_fetchSub(&(_p_self)->raw, make$((TypeOf((_p_self)->raw))_val), _ord)
#define __op__atom_V_fetchNand(_p_self, _val, _ord...) atom_fetchNand(&(_p_self)->raw, make$((TypeOf((_p_self)->raw))_val), _ord)
#define __op__atom_V_fetchAnd(_p_self, _val, _ord...) atom_fetchAnd(&(_p_self)->raw, make$((TypeOf((_p_self)->raw))_val), _ord)
#define __op__atom_V_fetchOr(_p_self, _val, _ord...) atom_fetchOr(&(_p_self)->raw, make$((TypeOf((_p_self)->raw))_val), _ord)
#define __op__atom_V_fetchXor(_p_self, _val, _ord...) atom_fetchXor(&(_p_self)->raw, make$((TypeOf((_p_self)->raw))_val), _ord)

$attr($inline_always)
$static fn_((atom_spinLoopHint(void))(void)) {
    /* NOLINTBEGIN */
#if arch_family_type == arch_family_type_x86
    __asm__ volatile("pause");
#elif arch_type == arch_type_aarch64
    /*
     * Some code uses 'isb' in spin-wait.
     * Alternatively, "yield" or "wfe" might be used for different strategies.
     */
    __asm__ volatile("isb");
#elif arch_type == arch_type_arm
#if arch_has_armv6k || arch_has_armv6m
    __asm__ volatile("yield");
#else
    /* On older ARM architectures without yield,
     * consider a simple 'nop' or other fallback. */
    __asm__ volatile("nop");
#endif /* arch_has_armv6k || arch_has_armv6m */

#elif arch_family_type == arch_family_type_riscv
#if arch_has_zihintpause
    /* RISC-V Zihintpause extension offers 'pause'
     * (some compilers also accept "__asm__ volatile(\"pause\")").
     */
    __asm__ volatile("pause");
#else
    /* Fallback: 'nop' if no Zihintpause ext. */
    __asm__ volatile("nop");
#endif /* arch_has_zihintpause */

#else
    /* Fallback for unknown or unhandled architectures */
    __asm__ volatile("nop");
#endif
    /* NOLINTEND */
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* atom__included */
