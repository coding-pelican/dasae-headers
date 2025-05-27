/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    atomic.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-22 (date of creation)
 * @updated 2025-02-01 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/atomic
 * @prefix  atomic
 *
 * @brief   Cross-platform atomic operations
 * @details This provides atomic operations for primitive types to prevent data races.
 */

#ifndef ATOMIC_INCLUDED
#define ATOMIC_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"

/*========== Memory Ordering ===============================================*/

/**
 * Memory ordering for atomic operations
 */
typedef enum atomic_MemOrd {
    /**
     * No ordering constraints. Only guarantees atomicity.
     */
    atomic_MemOrd_unordered = __ATOMIC_RELAXED,
    /**
     * Ensures that previous operations are complete before this operation.
     */
    atomic_MemOrd_monotonic = __ATOMIC_RELAXED,
    /**
     * Acquires a memory fence from other threads' releases.
     */
    atomic_MemOrd_acquire   = __ATOMIC_ACQUIRE,
    /**
     * Releases a memory fence to other threads' acquires.
     */
    atomic_MemOrd_release   = __ATOMIC_RELEASE,
    /**
     * Both acquires and releases a memory fence.
     */
    atomic_MemOrd_acq_rel   = __ATOMIC_ACQ_REL,
    /**
     * All memory operations are sequentially consistent.
     */
    atomic_MemOrd_seq_cst   = __ATOMIC_SEQ_CST
} atomic_MemOrd;

/*========== Atomic Value Type ============================================*/

/**
 * Atomic wrapper around primitive values to prevent data races.
 */
#define atomic_Value$(T) TYPE_UNNAMED__atomic_Value$(T)
#define TYPE_UNNAMED__atomic_Value$(T)                                                           \
    __attribute__((aligned(sizeOf$(T)))) struct {                                                \
        /** Care must be taken to avoid data races when interacting with this field directly. */ \
        volatile T raw;                                                                          \
    }
#define use_atomic_Value$(T)                                                          \
    typedef __attribute__((aligned(sizeOf$(T)))) struct pp_join($, atomic_Value, T) { \
        volatile T raw;                                                               \
    } pp_join($, atomic_Value, T)
#define atomic_Value_anonCast$(TNamedValue, var_unnamed_value) eval({                                 \
    let _unnamed_value = var_unnamed_value;                                                           \
    claim_assert_static(sizeOf(TypeOf(_unnamed_value)) == sizeOf(TNamedValue));                       \
    claim_assert_static(alignOf(TypeOf(_unnamed_value)) == alignOf(TNamedValue));                     \
    claim_assert_static(hasField(TypeOf(_unnamed_value), raw));                                       \
    claim_assert_static(validateField(TypeOf(_unnamed_value), raw, FieldType$(TNamedValue, raw)));    \
    claim_assert_static(fieldPadding(TypeOf(_unnamed_value), raw) == fieldPadding(TNamedValue, raw)); \
    eval_return(*(TNamedValue*)&_unnamed_value);                                                      \
})

/*========== Value Operations ============================================*/

/**
 * Initialize an atomic value
 */
#define atomic_init$(TValue, val_raw) \
    ((TValue){ .raw = (val_raw) })
#define atomic_init(var_self, val_raw) \
    ((var_self).raw = (val_raw))

/**
 * Atomically load a value with specified memory ordering
 */
#define atomic_load$(T, _ptr, val_order) \
    _Generic((_ptr), volatile rawptr$(T): __atomic_load_n((_ptr), (val_order)))
#define atomic_load(var_self, val_order) eval({                             \
    let __self = &(var_self);                                               \
    eval_return atomic_load$(TypeOf(__self->raw), &__self->raw, val_order); \
})

/**
 * Atomically store a value with specified memory ordering
 */
#define atomic_store$(T, _ptr, val_raw, val_order) \
    _Generic((_ptr), volatile rawptr$(T): __atomic_store_n((_ptr), (val_raw), (val_order)))
#define atomic_store(var_self, val_raw, val_order) eval({                             \
    let __self = &(var_self);                                                         \
    eval_return atomic_store$(TypeOf(__self->raw), &__self->raw, val_raw, val_order); \
})

/**
 * Atomically exchange a value with specified memory ordering
 */
#define atomic_swap$(T, _ptr, val_raw, val_order) \
    _Generic((_ptr), volatile rawptr$(T): __atomic_exchange_n((_ptr), (val_raw), (val_order)))
#define atomic_swap(var_self, val_raw, val_order) eval({                             \
    let __self = &(var_self);                                                        \
    eval_return atomic_swap$(TypeOf(__self->raw), &__self->raw, val_raw, val_order); \
})

/**
 * Atomically compare and exchange value if expected matches
 * Returns the previous value
 */
#define atomic_cmpxchgWeak$(T, _ptr, _expected_ptr, _desired_val, _success_order, _fail_order) ({ \
    T          __expected = *(_expected_ptr);                                                     \
    const bool __success  = __atomic_compare_exchange_n(                                          \
        (_ptr), (_expected_ptr), (_desired_val), true, (_success_order), (_fail_order)           \
    );                                                                                           \
    eval_return __success ? null : __expected;                                                    \
})
#define atomic_cmpxchgWeak(var_self, _expected_ptr, _desired_val, _success_order, _fail_order) eval({ \
    let __self = &(var_self);                                                                       \
    eval_return atomic_cmpxchgWeak$(TypeOf(__self->raw), &__self->raw, _expected_ptr, _desired_val, _success_order, _fail_order); \
})

/**
 * Atomically compare and exchange value if expected matches
 * Returns the previous value. Retries on weak failures.
 */
#define atomic_cmpxchgStrong$(T, _ptr, _expected_ptr, _desired_val, _success_order, _fail_order) ({ \
    T          __expected = *(_expected_ptr);                                                       \
    const bool __success  = __atomic_compare_exchange_n(                                            \
        (_ptr), (_expected_ptr), (_desired_val), false, (_success_order), (_fail_order)            \
    );                                                                                             \
    eval_return __success ? null : __expected;                                                      \
})
#define atomic_cmpxchgStrong(var_self, _expected_ptr, _desired_val, _success_order, _fail_order) eval({ \
    let __self = &(var_self);                                                                       \
    eval_return atomic_cmpxchgStrong$(TypeOf(__self->raw), &__self->raw, _expected_ptr, _desired_val, _success_order, _fail_order); \
})

/**
 * Atomically add a value and return previous value
 */
#define atomic_fetchAdd$(T, _ptr, val_raw, val_order) \
    _Generic((_ptr), volatile rawptr$(T): __atomic_fetch_add((_ptr), (val_raw), (val_order)))

/**
 * Atomically subtract a value and return previous value
 */
#define atomic_fetchSub$(T, _ptr, val_raw, val_order) \
    _Generic((_ptr), volatile rawptr$(T): __atomic_fetch_sub((_ptr), (val_raw), (val_order)))

/**
 * Atomically bitwise AND a value and return previous value
 */
#define atomic_fetchAnd$(T, _ptr, val_raw, val_order) \
    _Generic((_ptr), volatile rawptr$(T): __atomic_fetch_and((_ptr), (val_raw), (val_order)))

/**
 * Atomically bitwise OR a value and return previous value
 */
#define atomic_fetchOr$(T, _ptr, val_raw, val_order) \
    _Generic((_ptr), volatile rawptr$(T): __atomic_fetch_or((_ptr), (val_raw), (val_order)))

/**
 * Atomically bitwise XOR a value and return previous value
 */
#define atomic_fetchXor$(T, _ptr, val_raw, val_order) \
    _Generic((_ptr), volatile rawptr$(T): __atomic_fetch_xor((_ptr), (val_raw), (val_order)))

/*======== spinLoopHint: Platform-specific CPU hints for spin-loops =========
 *
 * On various architectures, there are instructions to reduce power usage
 * or improve back-off behavior in tight spin loops, e.g.:
 *   - x86/x86_64: 'pause'
 *   - ARM64:      'isb' or 'yield' (sometimes 'wfe')
 *   - ARMv6K:     'yield'
 *   - RISC-V:     'pause' if Zihintpause is available
 *
 * Adjust as needed for your usage patterns or correct instructions.
 *======================================================================== */
$inline_always void atomic_spinLoopHint(void) {
    // NOLINTBEGIN
#if bti_arch_x86 || bti_arch_x64
    __asm__ volatile("pause");
#elif bti_arch_arm64
    /*
     * Some code uses 'isb' in spin-wait.
     * Alternatively, "yield" or "wfe" might be used for different strategies.
     */
    __asm__ volatile("isb");
#elif bti_arch_arm32
#if bti_arch_has_armv6k || bti_arch_has_armv6m
    __asm__ volatile("yield");
#else
    /* On older ARM architectures without yield,
     * consider a simple 'nop' or other fallback. */
    __asm__ volatile("nop");
#endif

#elif (bti_arch_riscv32 || bti_arch_riscv64)
#if bti_arch_has_zihintpause
    /* RISC-V Zihintpause extension offers 'pause'
     * (some compilers also accept "__asm__ volatile(\"pause\")").
     */
    __asm__ volatile("pause");
#else
    /* Fallback: 'nop' if no Zihintpause ext. */
    __asm__ volatile("nop");
#endif

#else
    /* Fallback for unknown or unhandled architectures */
    __asm__ volatile("nop");
#endif
    // NOLINTEND
}

/*========== Cache Line Size ===========================================*/

#define atomic_cache_line bti_arch_cache_line

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ATOMIC_INCLUDED */
