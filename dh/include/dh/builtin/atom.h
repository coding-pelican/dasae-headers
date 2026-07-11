/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    atom.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-11-04 (date of creation)
 * @updated 2026-06-21 (date of last update)
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  atom
 */
#pragma once
#ifndef builtin_atom__included
#define builtin_atom__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "type_info.h"

/*========== Macros and Declarations ========================================*/

typedef enum $packed atom_MemOrd {
    atom_MemOrd_unordered = __ATOMIC_RELAXED, ///< No ordering constraints. Only guarantees atomicity.
    atom_MemOrd_monotonic = __ATOMIC_RELAXED, ///< Ensures that previous operations are complete before this operation.
    atom_MemOrd_acquire = __ATOMIC_ACQUIRE, ///< Acquires memory fence from other threads' releases.
    atom_MemOrd_release = __ATOMIC_RELEASE, ///< Releases memory fence to other threads' acquires.
    atom_MemOrd_acq_rel = __ATOMIC_ACQ_REL, ///< Both acquires and releases memory fence.
    atom_MemOrd_seq_cst = __ATOMIC_SEQ_CST, ///< All memory operations are sequentially consistent.
} atom_MemOrd;

#define atom_fence(_$ord...) __op__atom_fence(_$ord)
#define atom_load(_$ptr, _$ord...) __op__atom_load(pp_uniqTok(ret), _$ptr, _$ord)
#define atom_store(_$ptr, _$val, _$ord...) __op__atom_store(_$ptr, _$val, _$ord)

/// Compare-and-Swap
#define atom_cmpXchgWeak$(_$OT, _$ptr, _$expected, _$desired, _$succ_ord, _$fail_ord...) \
    __op__atom_cmpXchgWeak$( \
        pp_uniqTok(is_succ), pp_uniqTok(expected), \
        _$OT, _$ptr, _$expected, _$desired, _$succ_ord, _$fail_ord \
    )
#define atom_cmpXchgWeak(_$ptr, _$expected, _$desired, _$succ_ord, _$fail_ord...) \
    atom_cmpXchgWeak$( \
        O$$(TypeOfUnqual(*_$ptr)), _$ptr, _$expected, _$desired, _$succ_ord, _$fail_ord \
    )
#define atom_cmpXchgStrong$(_$OT, _$ptr, _$expected, _$desired, _$succ_ord, _$fail_ord...) \
    __op__atom_cmpXchgStrong$( \
        pp_uniqTok(is_succ), pp_uniqTok(expected), \
        _$OT, _$ptr, _$expected, _$desired, _$succ_ord, _$fail_ord \
    )
#define atom_cmpXchgStrong(_$ptr, _$expected, _$desired, _$succ_ord, _$fail_ord...) \
    atom_cmpXchgStrong$( \
        O$$(TypeOfUnqual(*_$ptr)), _$ptr, _$expected, _$desired, _$succ_ord, _$fail_ord \
    )

/// RMW (Read-Modify-Write) - returns the value before the operation (Fetch-and-Op)
#define atom_fetchXchg(_$ptr, _$val, _$ord...) __op__atom_fetchXchg(pp_uniqTok(ret), _$ptr, _$val, _$ord)
#define atom_pri_fetchAdd(_$ptr, _$val, _$ord...) __op__atom_pri_fetchAdd(_$ptr, _$val, _$ord)
#define atom_pri_fetchSub(_$ptr, _$val, _$ord...) __op__atom_pri_fetchSub(_$ptr, _$val, _$ord)
#define atom_pri_fetchMin(_$ptr, _$val, _$ord...) __op__atom_pri_fetchMin__step(_$ptr, _$val, _$ord)
#define atom_pri_fetchMax(_$ptr, _$val, _$ord...) __op__atom_pri_fetchMax__step(_$ptr, _$val, _$ord)
#define atom_int_fetchNand(_$ptr, _$val, _$ord...) __op__atom_int_fetchNand(_$ptr, _$val, _$ord)
#define atom_int_fetchAnd(_$ptr, _$val, _$ord...) __op__atom_int_fetchAnd(_$ptr, _$val, _$ord)
#define atom_int_fetchXor(_$ptr, _$val, _$ord...) __op__atom_int_fetchXor(_$ptr, _$val, _$ord)
#define atom_int_fetchOr(_$ptr, _$val, _$ord...) __op__atom_int_fetchOr(_$ptr, _$val, _$ord)

/// RMW (Read-Modify-Write) - returns the value after the operation (Op-and-Fetch)
#define atom_pri_addFetch(_$ptr, _$val, _$ord...) __op__atom_pri_addFetch(_$ptr, _$val, _$ord)
#define atom_pri_subFetch(_$ptr, _$val, _$ord...) __op__atom_pri_subFetch(_$ptr, _$val, _$ord)
#define atom_pri_minFetch(_$ptr, _$val, _$ord...) __op__atom_pri_minFetch__step(_$ptr, _$val, _$ord)
#define atom_pri_maxFetch(_$ptr, _$val, _$ord...) __op__atom_pri_maxFetch__step(_$ptr, _$val, _$ord)
#define atom_int_nandFetch(_$ptr, _$val, _$ord...) __op__atom_int_nandFetch(_$ptr, _$val, _$ord)
#define atom_int_andFetch(_$ptr, _$val, _$ord...) __op__atom_int_andFetch(_$ptr, _$val, _$ord)
#define atom_int_xorFetch(_$ptr, _$val, _$ord...) __op__atom_int_xorFetch(_$ptr, _$val, _$ord)
#define atom_int_orFetch(_$ptr, _$val, _$ord...) __op__atom_int_orFetch(_$ptr, _$val, _$ord)

/*========== Macros and Definitions =========================================*/

#define __op__atom_fence(_$ord...) __atomic_thread_fence(_$ord)
#define __op__atom_load(__ret, _$ptr, _$ord...) local_({ \
    var __ret = l0$((TypeOf(*_$ptr))); \
    __atomic_load(_$ptr, &__ret, as$(int)(as$(atom_MemOrd)(_$ord))); \
    local_return_(__ret); \
})
#define __op__atom_store(_$ptr, _$val, _$ord...) \
    __atomic_store(_$ptr, &from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))

#if UNUSED_CODE
#define __op__atom_alwaysLockFree_size(_size) \
    __atomic_always_lock_free((_size), null)
#define __op__atom_alwaysLockFree$(_T...) \
    __op__atom_alwaysLockFree_size(sizeOf$(_T))
#define __op__atom_isLockFree(_$ptr) \
    __atomic_is_lock_free(sizeOf$(TypeOf(*(_$ptr))), (_$ptr))
#endif /* UNUSED_CODE */

#define __op__atom_cmpXchgWeak$( \
    __is_succ, __expected, \
    _$OT, _$ptr, _$expected, _$desired, _$succ_ord, _$fail_ord... \
) local_({ \
    typedef _$OT O$Ret$atom_cmpXchgWeak; \
    var __expected = l$((TypeOf(*_$ptr))_$expected); \
    let_(__is_success, bool) = __atomic_compare_exchange( \
        _$ptr, &__expected, &from$((TypeOf(__expected))_$desired), true, \
        as$(int)(as$(atom_MemOrd)(_$succ_ord)), as$(int)(as$(atom_MemOrd)(_$fail_ord)) \
    ); \
    __is_success ? local_return_(l$((O$Ret$atom_cmpXchgWeak)none())) \
                 : local_return_(l$((O$Ret$atom_cmpXchgWeak)some(__expected))); \
})
#define __op__atom_cmpXchgStrong$( \
    __is_succ, __expected, \
    _$OT, _$ptr, _$expected, _$desired, _$succ_ord, _$fail_ord... \
) local_({ \
    typedef _$OT O$Ret$atom_cmpXchgStrong; \
    var __expected = l$((TypeOf(*_$ptr))_$expected); \
    let_(__is_success, bool) = __atomic_compare_exchange( \
        _$ptr, &__expected, &from$((TypeOf(__expected))_$desired), false, \
        as$(int)(as$(atom_MemOrd)(_$succ_ord)), as$(int)(as$(atom_MemOrd)(_$fail_ord)) \
    ); \
    __is_success ? local_return_(l$((O$Ret$atom_cmpXchgStrong)none())) \
                 : local_return_(l$((O$Ret$atom_cmpXchgStrong)some(__expected))); \
})

#define __op__atom_fetchXchg(__ret, _$ptr, _$val, _$ord...) local_({ \
    var __ret = l0$((TypeOf(*_$ptr))); \
    __atomic_exchange(_$ptr, &from$((TypeOf(__ret))_$val), &__ret, as$(int)(as$(atom_MemOrd)(_$ord))); \
    local_return_(__ret); \
})
#define __op__atom_pri_fetchAdd(_$ptr, _$val, _$ord...) \
    __atomic_fetch_add(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))
#define __op__atom_pri_fetchSub(_$ptr, _$val, _$ord...) \
    __atomic_fetch_sub(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))
#define __op__atom_pri_fetchMin__step(_$ptr, _$val, _$ord...) __op__atom_pri_fetchMin( \
    pp_uniqTok(ptr), pp_uniqTok(operand), pp_uniqTok(expected), pp_uniqTok(desired), \
    _$ptr, _$val, _$ord \
)
#define __op__atom_pri_fetchMax__step(_$ptr, _$val, _$ord...) __op__atom_pri_fetchMax( \
    pp_uniqTok(ptr), pp_uniqTok(operand), pp_uniqTok(expected), pp_uniqTok(desired), \
    _$ptr, _$val, _$ord \
)
#if !defined(__comp_bool__atom_has_fetch_min_max) && defined(__has_builtin)
#if __has_builtin(__atomic_fetch_min) && __has_builtin(__atomic_fetch_max)
#define __comp_bool__atom_has_fetch_min_max 1
#endif
#endif
#if !defined(__comp_bool__atom_has_fetch_min_max)
#define __comp_bool__atom_has_fetch_min_max 0
#endif
#if __comp_bool__atom_has_fetch_min_max
#define __op__atom_pri_fetchMin(__ptr, __operand, __expected, __desired, _$ptr, _$val, _$ord...) \
    __atomic_fetch_min(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))
#define __op__atom_pri_fetchMax(__ptr, __operand, __expected, __desired, _$ptr, _$val, _$ord...) \
    __atomic_fetch_max(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))
#else
#define __op__atom_cmpXchgFailOrd(_$ord...) ( \
    (as$(atom_MemOrd)(_$ord) == atom_MemOrd_release)   ? atom_MemOrd_monotonic \
    : (as$(atom_MemOrd)(_$ord) == atom_MemOrd_acq_rel) ? atom_MemOrd_acquire \
                                                       : as$(atom_MemOrd)(_$ord) \
)
#define __op__atom_pri_fetchMin(__ptr, __operand, __expected, __desired, _$ptr, _$val, _$ord...) local_({ \
    let __ptr = (_$ptr); \
    typedef TypeOfUnqual(*__ptr) AtomType; \
    let_(__operand, AtomType) = _$val; \
    var_(__expected, AtomType) = atom_load(__ptr, atom_MemOrd_monotonic); \
    while (true) { \
        var_(__desired, AtomType) = flt_isNaN(__expected)    ? __operand \
                                  : flt_isNaN(__operand)     ? __expected \
                                  : (__operand < __expected) ? __operand \
                                                             : __expected; \
        if (__atomic_compare_exchange( \
                __ptr, &__expected, &__desired, false, \
                as$(int)(as$(atom_MemOrd)(_$ord)), as$(int)(__op__atom_cmpXchgFailOrd(_$ord)) \
            )) { \
            break; \
        } \
    } \
    local_return_(__expected); \
})
#define __op__atom_pri_fetchMax(__ptr, __operand, __expected, __desired, _$ptr, _$val, _$ord...) local_({ \
    let __ptr = (_$ptr); \
    typedef TypeOfUnqual(*__ptr) AtomType; \
    let_(__operand, AtomType) = _$val; \
    var_(__expected, AtomType) = atom_load(__ptr, atom_MemOrd_monotonic); \
    while (true) { \
        var_(__desired, AtomType) = flt_isNaN(__expected)    ? __operand \
                                  : flt_isNaN(__operand)     ? __expected \
                                  : (__operand > __expected) ? __operand \
                                                             : __expected; \
        if (__atomic_compare_exchange( \
                __ptr, &__expected, &__desired, false, \
                as$(int)(as$(atom_MemOrd)(_$ord)), as$(int)(__op__atom_cmpXchgFailOrd(_$ord)) \
            )) { \
            break; \
        } \
    } \
    local_return_(__expected); \
})
#endif
#define __op__atom_int_fetchNand(_$ptr, _$val, _$ord...) \
    __atomic_fetch_nand(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))
#define __op__atom_int_fetchAnd(_$ptr, _$val, _$ord...) \
    __atomic_fetch_and(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))
#define __op__atom_int_fetchXor(_$ptr, _$val, _$ord...) \
    __atomic_fetch_xor(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))
#define __op__atom_int_fetchOr(_$ptr, _$val, _$ord...) \
    __atomic_fetch_or(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))

#define __op__atom_pri_addFetch(_$ptr, _$val, _$ord...) \
    __atomic_add_fetch(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))
#define __op__atom_pri_subFetch(_$ptr, _$val, _$ord...) \
    __atomic_sub_fetch(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))
#define __op__atom_pri_minFetch__step(_$ptr, _$val, _$ord...) __op__atom_pri_minFetch( \
    pp_uniqTok(ptr), pp_uniqTok(operand), pp_uniqTok(expected), pp_uniqTok(desired), \
    _$ptr, _$val, _$ord \
)
#define __op__atom_pri_maxFetch__step(_$ptr, _$val, _$ord...) __op__atom_pri_maxFetch( \
    pp_uniqTok(ptr), pp_uniqTok(operand), pp_uniqTok(expected), pp_uniqTok(desired), \
    _$ptr, _$val, _$ord \
)
#if !defined(__comp_bool__atom_has_min_max_fetch) && defined(__has_builtin)
#if __has_builtin(__atomic_min_fetch) && __has_builtin(__atomic_max_fetch)
#define __comp_bool__atom_has_min_max_fetch 1
#endif
#endif
#if !defined(__comp_bool__atom_has_min_max_fetch)
#define __comp_bool__atom_has_min_max_fetch 0
#endif
#if __comp_bool__atom_has_min_max_fetch
#define __op__atom_pri_minFetch(_$ptr, _$val, _$ord...) \
    __atomic_min_fetch(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))
#define __op__atom_pri_maxFetch(_$ptr, _$val, _$ord...) \
    __atomic_max_fetch(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))
#else
#define __op__atom_cmpXchgFailOrd(_$ord...) ( \
    (as$(atom_MemOrd)(_$ord) == atom_MemOrd_release)   ? atom_MemOrd_monotonic \
    : (as$(atom_MemOrd)(_$ord) == atom_MemOrd_acq_rel) ? atom_MemOrd_acquire \
                                                       : as$(atom_MemOrd)(_$ord) \
)
#define __op__atom_pri_fetchMin(__ptr, __operand, __expected, __desired, _$ptr, _$val, _$ord...) /* TODO: Implement __op__atom_pri_fetchMin */
#define __op__atom_pri_fetchMax(__ptr, __operand, __expected, __desired, _$ptr, _$val, _$ord...) /* TODO: Implement __op__atom_pri_fetchMax */
#endif
#define __op__atom_int_nandFetch(_$ptr, _$val, _$ord...) \
    __atomic_nand_fetch(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))
#define __op__atom_int_andFetch(_$ptr, _$val, _$ord...) \
    __atomic_and_fetch(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))
#define __op__atom_int_xorFetch(_$ptr, _$val, _$ord...) \
    __atomic_xor_fetch(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))
#define __op__atom_int_orFetch(_$ptr, _$val, _$ord...) \
    __atomic_or_fetch(_$ptr, from$((TypeOf(*_$ptr))_$val), as$(int)(as$(atom_MemOrd)(_$ord)))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_atom__included */
