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

#define atom_V$(_$T...) tpl$(atom_V, _$T)
#define atom_V$$(_$T...) \
    struct { \
        volatile _$T raw; \
    }
#define T_use_atom_V$(_$T...) \
    T_decl_atom_V$(_$T); \
    T_impl_atom_V$(_$T)
#define T_decl_atom_V$(_$T...) \
    typedef struct atom_V$(_$T) atom_V$(_$T);
#define T_impl_atom_V$(_$T...) \
    struct atom_V$(_$T) { \
        volatile _$T raw; \
    }

#define atom_V_zero() __op__atom_V_zero()
#define atom_V_zero$(_$VT) __op__atom_V_zero$(_$VT)
#define atom_V_init(_$val...) __op__atom_V_init(_$val)
#define atom_V_init$(_$VT, _$val...) __op__atom_V_init$(_$VT, _$val)
#define atom_V_from(_$val...) __op__atom_V_from(_$val)
#define atom_V_load(_$p_self, _$ord...) __op__atom_V_load(_$p_self, _$ord)
#define atom_V_store(_$p_self, _$val, _$ord...) __op__atom_V_store(_$p_self, _$val, _$ord)

#define atom_V_alwaysLockFree$(_$T...) __op__atom_V_alwaysLockFree$(_$T)
#define atom_V_isLockFree(_$p_self) __op__atom_V_isLockFree(_$p_self)

#define atom_V_cmpXchgWeak$(_OT, _$p_self, _$expected, _$desired, _$succ_ord, _$fail_ord...) \
    __op__atom_V_cmpXchgWeak$(_OT, _$p_self, _$expected, _$desired, _$succ_ord, _$fail_ord)
#define atom_V_cmpXchgWeak(_$p_self, _$expected, _$desired, _$succ_ord, _$fail_ord...) \
    __op__atom_V_cmpXchgWeak(_$p_self, _$expected, _$desired, _$succ_ord, _$fail_ord)
#define atom_V_cmpXchgStrong$(_OT, _$p_self, _$expected, _$desired, _$succ_ord, _$fail_ord...) \
    __op__atom_V_cmpXchgStrong$(_OT, _$p_self, _$expected, _$desired, _$succ_ord, _$fail_ord)
#define atom_V_cmpXchgStrong(_$p_self, _$expected, _$desired, _$succ_ord, _$fail_ord...) \
    __op__atom_V_cmpXchgStrong(_$p_self, _$expected, _$desired, _$succ_ord, _$fail_ord)

#define atom_V_fetchXchg(_$p_self, _$val, _$ord...) __op__atom_V_fetchXchg(_$p_self, _$val, _$ord)
#define atom_V_pri_fetchAdd(_$p_self, _$val, _$ord...) __op__atom_V_pri_fetchAdd(_$p_self, _$val, _$ord)
#define atom_V_pri_fetchSub(_$p_self, _$val, _$ord...) __op__atom_V_pri_fetchSub(_$p_self, _$val, _$ord)
#define atom_V_pri_fetchMin(_$p_self, _$val, _$ord...) __op__atom_V_pri_fetchMin(_$p_self, _$val, _$ord)
#define atom_V_pri_fetchMax(_$p_self, _$val, _$ord...) __op__atom_V_pri_fetchMax(_$p_self, _$val, _$ord)
#define atom_V_int_fetchNand(_$p_self, _$val, _$ord...) __op__atom_V_int_fetchNand(_$p_self, _$val, _$ord)
#define atom_V_int_fetchAnd(_$p_self, _$val, _$ord...) __op__atom_V_int_fetchAnd(_$p_self, _$val, _$ord)
#define atom_V_int_fetchXor(_$p_self, _$val, _$ord...) __op__atom_V_int_fetchXor(_$p_self, _$val, _$ord)
#define atom_V_int_fetchOr(_$p_self, _$val, _$ord...) __op__atom_V_int_fetchOr(_$p_self, _$val, _$ord)

#define atom_V_pri_addFetch(_$p_self, _$val, _$ord...) __op__atom_V_pri_addFetch(_$p_self, _$val, _$ord)
#define atom_V_pri_subFetch(_$p_self, _$val, _$ord...) __op__atom_V_pri_subFetch(_$p_self, _$val, _$ord)
#define atom_V_pri_minFetch(_$p_self, _$val, _$ord...) __op__atom_V_pri_minFetch(_$p_self, _$val, _$ord)
#define atom_V_pri_maxFetch(_$p_self, _$val, _$ord...) __op__atom_V_pri_maxFetch(_$p_self, _$val, _$ord)
#define atom_V_int_nandFetch(_$p_self, _$val, _$ord...) __op__atom_V_int_nandFetch(_$p_self, _$val, _$ord)
#define atom_V_int_andFetch(_$p_self, _$val, _$ord...) __op__atom_V_int_andFetch(_$p_self, _$val, _$ord)
#define atom_V_int_xorFetch(_$p_self, _$val, _$ord...) __op__atom_V_int_xorFetch(_$p_self, _$val, _$ord)
#define atom_V_int_orFetch(_$p_self, _$val, _$ord...) __op__atom_V_int_orFetch(_$p_self, _$val, _$ord)

#define atom_V_int_setBit(_$p_self, _$bit, _$ord...) __step__atom_V_int_setBit(_$p_self, _$bit, _$ord)
#define atom_V_int_resetBit(_$p_self, _$bit, _$ord...) __step__atom_V_int_resetBit(_$p_self, _$bit, _$ord)
#define atom_V_int_toggleBit(_$p_self, _$bit, _$ord...) __step__atom_V_int_toggleBit(_$p_self, _$bit, _$ord)

/*========== Macros and Definitions =========================================*/

#define __comp_int__atom_cache_line_bytes arch_cache_line_bytes

#if in_analysis_active_only || in_comptime
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
#endif /* in_analysis_active_only || in_comptime */

#define ____atom_SpinLock_init_static() \
    enum_of$((atom_SpinLock)(atom_SpinLock_unlocked))
#if in_analysis_active_only || in_comptime
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
#endif /* in_analysis_active_only || in_comptime */

#define __op__atom_V_zero() cleared()
#define __op__atom_V_zero$(_$VT) l$((_$VT)cleared())
#define __op__atom_V_init(_$val...) initial(.raw = _$val)
#define __op__atom_V_init$(_$VT, _$val...) l$((_$VT)atom_V_init(_$val))
#define __op__atom_V_from(_$val...) atom_V_init$(atom_V$$(TypeOf(_$val)), _$val)
#define __op__atom_V_load(_$p_self, _$ord...) atom_load(&(_$p_self)->raw, _$ord)
#define __op__atom_V_store(_$p_self, _$val, _$ord...) atom_store(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_alwaysLockFree$(_$T...) atom_alwaysLockFree$(_$T)
#define __op__atom_V_isLockFree(_$p_self) atom_isLockFree(&(_$p_self)->raw)

#define __op__atom_V_cmpXchgWeak$(_OT, _$p_self, _$expected, _$desired, _$succ_ord, _$fail_ord...) \
    atom_cmpXchgWeak$(_OT, &(_$p_self)->raw, _$expected, _$desired, _$succ_ord, _$fail_ord)
#define __op__atom_V_cmpXchgWeak(_$p_self, _$expected, _$desired, _$succ_ord, _$fail_ord...) \
    atom_cmpXchgWeak(&(_$p_self)->raw, _$expected, _$desired, _$succ_ord, _$fail_ord)
#define __op__atom_V_cmpXchgStrong$(_OT, _$p_self, _$expected, _$desired, _$succ_ord, _$fail_ord...) \
    atom_cmpXchgStrong$(_OT, &(_$p_self)->raw, _$expected, _$desired, _$succ_ord, _$fail_ord)
#define __op__atom_V_cmpXchgStrong(_$p_self, _$expected, _$desired, _$succ_ord, _$fail_ord...) \
    atom_cmpXchgStrong(&(_$p_self)->raw, _$expected, _$desired, _$succ_ord, _$fail_ord)

#define __op__atom_V_fetchXchg(_$p_self, _$val, _$ord...) \
    atom_fetchXchg(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_pri_fetchAdd(_$p_self, _$val, _$ord...) \
    atom_pri_fetchAdd(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_pri_fetchSub(_$p_self, _$val, _$ord...) \
    atom_pri_fetchSub(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_pri_fetchMin(_$p_self, _$val, _$ord...) \
    atom_pri_fetchMin(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_pri_fetchMax(_$p_self, _$val, _$ord...) \
    atom_pri_fetchMax(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_int_fetchNand(_$p_self, _$val, _$ord...) \
    atom_int_fetchNand(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_int_fetchAnd(_$p_self, _$val, _$ord...) \
    atom_int_fetchAnd(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_int_fetchXor(_$p_self, _$val, _$ord...) \
    atom_int_fetchXor(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_int_fetchOr(_$p_self, _$val, _$ord...) \
    atom_int_fetchOr(&(_$p_self)->raw, _$val, _$ord)

#define __op__atom_V_pri_addFetch(_$p_self, _$val, _$ord...) \
    atom_pri_addFetch(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_pri_subFetch(_$p_self, _$val, _$ord...) \
    atom_pri_subFetch(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_pri_minFetch(_$p_self, _$val, _$ord...) \
    atom_pri_minFetch(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_pri_maxFetch(_$p_self, _$val, _$ord...) \
    atom_pri_maxFetch(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_int_nandFetch(_$p_self, _$val, _$ord...) \
    atom_int_nandFetch(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_int_andFetch(_$p_self, _$val, _$ord...) \
    atom_int_andFetch(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_int_xorFetch(_$p_self, _$val, _$ord...) \
    atom_int_xorFetch(&(_$p_self)->raw, _$val, _$ord)
#define __op__atom_V_int_orFetch(_$p_self, _$val, _$ord...) \
    atom_int_orFetch(&(_$p_self)->raw, _$val, _$ord)

#define __step__atom_V_int_setBit(_$p_self, _$bit, _$ord...) \
    ____atom_V_int_setBit(pp_uniqTok(mask), pp_uniqTok(val), _$p_self, _$bit, _$ord)
#define ____atom_V_int_setBit(__mask, __val, _$p_self, _$bit, _$ord...) ({ \
    typedef TypeOf((_$p_self)->raw) SelfType; \
    let_(__mask, SelfType) = int_shl(as$(SelfType)(1), _$bit); \
    let_(__val, SelfType) = atom_V_int_fetchOr(_$p_self, __mask, _$ord); \
    ((__val & __mask) != 0); \
})
#define __step__atom_V_int_resetBit(_$p_self, _$bit, _$ord...) \
    ____atom_V_int_resetBit(pp_uniqTok(mask), pp_uniqTok(val), _$p_self, _$bit, _$ord)
#define ____atom_V_int_resetBit(__mask, __val, _$p_self, _$bit, _$ord...) ({ \
    typedef TypeOf((_$p_self)->raw) SelfType; \
    let_(__mask, SelfType) = int_shl(as$(SelfType)(1), _$bit); \
    let_(__val, SelfType) = atom_V_int_fetchAnd(_$p_self, ~__mask, _$ord); \
    ((__val & __mask) != 0); \
})
#define __step__atom_V_int_toggleBit(_$p_self, _$bit, _$ord...) \
    ____atom_V_int_toggleBit(pp_uniqTok(mask), pp_uniqTok(val), _$p_self, _$bit, _$ord)
#define ____atom_V_int_toggleBit(__mask, __val, _$p_self, _$bit, _$ord...) ({ \
    typedef TypeOf((_$p_self)->raw) SelfType; \
    let_(__mask, SelfType) = int_shl(as$(SelfType)(1), _$bit); \
    let_(__val, SelfType) = atom_V_int_fetchXor(_$p_self, __mask, _$ord); \
    ((__val & __mask) != 0); \
})

/* clang-format off */
T_use_atom_V$(bool);
T_use_atom_V$(u8); T_use_atom_V$(u16); T_use_atom_V$(u32);
T_use_atom_V$(ulong); T_use_atom_V$(u64); T_use_atom_V$(usize);
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* atom__included */
