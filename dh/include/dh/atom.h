/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    atom.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-22 (date of creation)
 * @updated 2026-06-21 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  atom
 *
 * @brief   Cross-platform atomic operations
 * @details This provides atomic operations for primitive types to prevent data races.
 */
#pragma once
#ifndef atom__included
#define atom__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

#define atom_cache_line_bytes __comp_int__atom_cache_line_bytes

/// Signals to the processor that the caller is inside a busy-wait spin loop.
$attr($inline_always)
$static fn_((atom_spinLoopHint(void))(void));

typedef enum $packed atom_SpinLock {
    atom_SpinLock_unlocked,
    atom_SpinLock_locked,
} atom_SpinLock;
#define atom_SpinLock_init_static() \
    ____atom_SpinLock_init_static()
$attr($inline_always)
$static fn_((atom_SpinLock_init(void))(atom_SpinLock));
$attr($inline_always)
$static fn_((atom_SpinLock_isLocked(const atom_SpinLock* self))(bool));
$attr($inline_always)
$static fn_((atom_SpinLock_lock(atom_SpinLock* self))(void));
$attr($inline_always)
$static fn_((atom_SpinLock_tryLock(atom_SpinLock* self))(bool));
$attr($inline_always)
$static fn_((atom_SpinLock_unlock(atom_SpinLock* self))(void));

#define atom_V$(_T...) tpl$(atom_V, _T)
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

#define atom_V_zero() __op__atom_V_zero()
#define atom_V_zero$(_VT) __op__atom_V_zero$(_VT)
#define atom_V_init(_val...) __op__atom_V_init(_val)
#define atom_V_init$(_VT, _val...) __op__atom_V_init$(_VT, _val)
#define atom_V_from(_val...) __op__atom_V_from(_val)
#define atom_V_load(_p_self, _ord...) __op__atom_V_load(_p_self, _ord)
#define atom_V_store(_p_self, _val, _ord...) __op__atom_V_store(_p_self, _val, _ord)

#define atom_V_cmpXchgWeak$(_OT, _p_self, _expected, _desired, _succ_ord, _fail_ord...) \
    __op__atom_V_cmpXchgWeak$(_OT, _p_self, _expected, _desired, _succ_ord, _fail_ord)
#define atom_V_cmpXchgWeak(_p_self, _expected, _desired, _succ_ord, _fail_ord...) \
    __op__atom_V_cmpXchgWeak(_p_self, _expected, _desired, _succ_ord, _fail_ord)
#define atom_V_cmpXchgStrong$(_OT, _p_self, _expected, _desired, _succ_ord, _fail_ord...) \
    __op__atom_V_cmpXchgStrong$(_OT, _p_self, _expected, _desired, _succ_ord, _fail_ord)
#define atom_V_cmpXchgStrong(_p_self, _expected, _desired, _succ_ord, _fail_ord...) \
    __op__atom_V_cmpXchgStrong(_p_self, _expected, _desired, _succ_ord, _fail_ord)

#define atom_V_fetchXchg(_p_self, _val, _ord...) __op__atom_V_fetchXchg(_p_self, _val, _ord)
#define atom_V_pri_fetchAdd(_p_self, _val, _ord...) __op__atom_V_pri_fetchAdd(_p_self, _val, _ord)
#define atom_V_pri_fetchSub(_p_self, _val, _ord...) __op__atom_V_pri_fetchSub(_p_self, _val, _ord)
#define atom_V_pri_fetchMin(_p_self, _val, _ord...) __op__atom_V_pri_fetchMin(_p_self, _val, _ord)
#define atom_V_pri_fetchMax(_p_self, _val, _ord...) __op__atom_V_pri_fetchMax(_p_self, _val, _ord)
#define atom_V_int_fetchNand(_p_self, _val, _ord...) __op__atom_V_int_fetchNand(_p_self, _val, _ord)
#define atom_V_int_fetchAnd(_p_self, _val, _ord...) __op__atom_V_int_fetchAnd(_p_self, _val, _ord)
#define atom_V_int_fetchXor(_p_self, _val, _ord...) __op__atom_V_int_fetchXor(_p_self, _val, _ord)
#define atom_V_int_fetchOr(_p_self, _val, _ord...) __op__atom_V_int_fetchOr(_p_self, _val, _ord)

#define atom_V_int_setBit(_p_self, _bit, _ord...) __step__atom_V_int_setBit(_p_self, _bit, _ord)
#define atom_V_int_resetBit(_p_self, _bit, _ord...) __step__atom_V_int_resetBit(_p_self, _bit, _ord)
#define atom_V_int_toggleBit(_p_self, _bit, _ord...) __step__atom_V_int_toggleBit(_p_self, _bit, _ord)

/*========== Macros and Definitions =========================================*/

#define __comp_int__atom_cache_line_bytes arch_cache_line_bytes

#if on_analysis_active_only || on_comptime
fn_((atom_spinLoopHint(void))(void)) { /* NOLINTBEGIN(hicpp-no-assembler) */
#if arch_family_type == arch_family_type_x86
    asm_volatile("pause");
#elif arch_type == arch_type_aarch64
    asm_volatile("isb");
#elif arch_type == arch_type_arm
#if arch_has_arm_yield
    asm_volatile("yield");
#else
    asm_volatile("");
#endif
#elif arch_family_type == arch_family_type_riscv
#if arch_has_riscv_zihintpause
    asm_volatile("pause");
#else
    asm_volatile("");
#endif
#else
    asm_volatile("");
#endif
}; /* NOLINTEND(hicpp-no-assembler) */
#endif /* on_analysis_active_only || on_comptime */

#define ____atom_SpinLock_init_static() \
    enum_of$((atom_SpinLock)(atom_SpinLock_unlocked))
#if on_analysis_active_only || on_comptime
fn_((atom_SpinLock_init(void))(atom_SpinLock)) {
    return atom_SpinLock_unlocked;
};
fn_((atom_SpinLock_isLocked(const atom_SpinLock* self))(bool)) {
    return atom_load(self, atom_MemOrd_unordered) == atom_SpinLock_locked;
};
fn_((atom_SpinLock_lock(atom_SpinLock* self))(void)) {
    while (!atom_SpinLock_tryLock(self)) {
        while (atom_SpinLock_isLocked(self)) atom_spinLoopHint();
    }
};
fn_((atom_SpinLock_tryLock(atom_SpinLock* self))(bool)) {
    return isNone(atom_cmpXchgStrong(
        self,
        atom_SpinLock_unlocked,
        atom_SpinLock_locked,
        atom_MemOrd_acquire,
        atom_MemOrd_monotonic
    ));
};
fn_((atom_SpinLock_unlock(atom_SpinLock* self))(void)) {
    claim_assert(atom_SpinLock_isLocked(self));
    atom_store(self, atom_SpinLock_unlocked, atom_MemOrd_release);
};
#endif /* on_analysis_active_only || on_comptime */

#define __op__atom_V_zero() cleared()
#define __op__atom_V_zero$(_VT) l$((_VT)cleared())
#define __op__atom_V_init(_val...) initial(.raw = _val)
#define __op__atom_V_init$(_VT, _val...) l$((_VT)atom_V_init(_val))
#define __op__atom_V_from(_val...) atom_V_init$(atom_V$$(TypeOf(_val)), _val)
#define __op__atom_V_load(_p_self, _ord...) atom_load(&(_p_self)->raw, _ord)
#define __op__atom_V_store(_p_self, _val, _ord...) atom_store(&(_p_self)->raw, _val, _ord)

#define __op__atom_V_cmpXchgWeak$(_OT, _p_self, _expected, _desired, _succ_ord, _fail_ord...) \
    atom_cmpXchgWeak$(_OT, &(_p_self)->raw, _expected, _desired, _succ_ord, _fail_ord)
#define __op__atom_V_cmpXchgWeak(_p_self, _expected, _desired, _succ_ord, _fail_ord...) \
    atom_cmpXchgWeak(&(_p_self)->raw, _expected, _desired, _succ_ord, _fail_ord)
#define __op__atom_V_cmpXchgStrong$(_OT, _p_self, _expected, _desired, _succ_ord, _fail_ord...) \
    atom_cmpXchgStrong$(_OT, &(_p_self)->raw, _expected, _desired, _succ_ord, _fail_ord)
#define __op__atom_V_cmpXchgStrong(_p_self, _expected, _desired, _succ_ord, _fail_ord...) \
    atom_cmpXchgStrong(&(_p_self)->raw, _expected, _desired, _succ_ord, _fail_ord)

#define __op__atom_V_fetchXchg(_p_self, _val, _ord...) \
    atom_fetchXchg(&(_p_self)->raw, _val, _ord)
#define __op__atom_V_pri_fetchAdd(_p_self, _val, _ord...) \
    atom_pri_fetchAdd(&(_p_self)->raw, _val, _ord)
#define __op__atom_V_pri_fetchSub(_p_self, _val, _ord...) \
    atom_pri_fetchSub(&(_p_self)->raw, _val, _ord)
#define __op__atom_V_pri_fetchMin(_p_self, _val, _ord...) \
    atom_pri_fetchMin(&(_p_self)->raw, _val, _ord)
#define __op__atom_V_pri_fetchMax(_p_self, _val, _ord...) \
    atom_pri_fetchMax(&(_p_self)->raw, _val, _ord)
#define __op__atom_V_int_fetchNand(_p_self, _val, _ord...) \
    atom_int_fetchNand(&(_p_self)->raw, _val, _ord)
#define __op__atom_V_int_fetchAnd(_p_self, _val, _ord...) \
    atom_int_fetchAnd(&(_p_self)->raw, _val, _ord)
#define __op__atom_V_int_fetchXor(_p_self, _val, _ord...) \
    atom_int_fetchXor(&(_p_self)->raw, _val, _ord)
#define __op__atom_V_int_fetchOr(_p_self, _val, _ord...) \
    atom_int_fetchOr(&(_p_self)->raw, _val, _ord)

#define __step__atom_V_int_setBit(_p_self, _bit, _ord...) \
    ____atom_V_int_setBit(pp_uniqTok(mask), pp_uniqTok(val), _p_self, _bit, _ord)
#define ____atom_V_int_setBit(__mask, __val, _p_self, _bit, _ord...) ({ \
    typedef TypeOf((_p_self)->raw) SelfType; \
    let_(__mask, SelfType) = int_shl(as$(SelfType)(1), _bit); \
    let_(__val, SelfType) = atom_V_int_fetchOr(_p_self, __mask, _ord); \
    ((__val & __mask) != 0); \
})
#define __step__atom_V_int_resetBit(_p_self, _bit, _ord...) \
    ____atom_V_int_resetBit(pp_uniqTok(mask), pp_uniqTok(val), _p_self, _bit, _ord)
#define ____atom_V_int_resetBit(__mask, __val, _p_self, _bit, _ord...) ({ \
    typedef TypeOf((_p_self)->raw) SelfType; \
    let_(__mask, SelfType) = int_shl(as$(SelfType)(1), _bit); \
    let_(__val, SelfType) = atom_V_int_fetchAnd(_p_self, ~__mask, _ord); \
    ((__val & __mask) != 0); \
})
#define __step__atom_V_int_toggleBit(_p_self, _bit, _ord...) \
    ____atom_V_int_toggleBit(pp_uniqTok(mask), pp_uniqTok(val), _p_self, _bit, _ord)
#define ____atom_V_int_toggleBit(__mask, __val, _p_self, _bit, _ord...) ({ \
    typedef TypeOf((_p_self)->raw) SelfType; \
    let_(__mask, SelfType) = int_shl(as$(SelfType)(1), _bit); \
    let_(__val, SelfType) = atom_V_int_fetchXor(_p_self, __mask, _ord); \
    ((__val & __mask) != 0); \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* atom__included */
