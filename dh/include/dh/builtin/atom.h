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

#define atom_fence(_ord...) __op__atom_fence(_ord)
#define atom_load(_ptr, _ord...) __op__atom_load(pp_uniqTok(ret), _ptr, _ord)
#define atom_store(_ptr, _val, _ord...) __op__atom_store(_ptr, _val, _ord)

/// Compare-and-Swap
#define atom_cmpXchgWeak$(_OT, _ptr, _expected, _desired, _succ_ord, _fail_ord...) \
    __op__atom_cmpXchgWeak$( \
        pp_uniqTok(is_succ), pp_uniqTok(expected), \
        _OT, _ptr, _expected, _desired, _succ_ord, _fail_ord \
    )
#define atom_cmpXchgWeak(_ptr, _expected, _desired, _succ_ord, _fail_ord...) \
    atom_cmpXchgWeak$( \
        O$$(TypeOfUnqual(*_ptr)), _ptr, _expected, _desired, _succ_ord, _fail_ord \
    )
#define atom_cmpXchgStrong$(_OT, _ptr, _expected, _desired, _succ_ord, _fail_ord...) \
    __op__atom_cmpXchgStrong$( \
        pp_uniqTok(is_succ), pp_uniqTok(expected), \
        _OT, _ptr, _expected, _desired, _succ_ord, _fail_ord \
    )
#define atom_cmpXchgStrong(_ptr, _expected, _desired, _succ_ord, _fail_ord...) \
    atom_cmpXchgStrong$( \
        O$$(TypeOfUnqual(*_ptr)), _ptr, _expected, _desired, _succ_ord, _fail_ord \
    )

/// RMW (Read-Modify-Write) - returns the value before the operation (Fetch-and-Op)
#define atom_fetchXchg(_ptr, _val, _ord...) __op__atom_fetchXchg(pp_uniqTok(ret), _ptr, _val, _ord)
#define atom_pri_fetchAdd(_ptr, _val, _ord...) __op__atom_pri_fetchAdd(_ptr, _val, _ord)
#define atom_pri_fetchSub(_ptr, _val, _ord...) __op__atom_pri_fetchSub(_ptr, _val, _ord)
#define atom_pri_fetchMin(_ptr, _val, _ord...) __op__atom_pri_fetchMin__step(_ptr, _val, _ord)
#define atom_pri_fetchMax(_ptr, _val, _ord...) __op__atom_pri_fetchMax__step(_ptr, _val, _ord)
#define atom_int_fetchNand(_ptr, _val, _ord...) __op__atom_int_fetchNand(_ptr, _val, _ord)
#define atom_int_fetchAnd(_ptr, _val, _ord...) __op__atom_int_fetchAnd(_ptr, _val, _ord)
#define atom_int_fetchXor(_ptr, _val, _ord...) __op__atom_int_fetchXor(_ptr, _val, _ord)
#define atom_int_fetchOr(_ptr, _val, _ord...) __op__atom_int_fetchOr(_ptr, _val, _ord)

/*========== Macros and Definitions =========================================*/

#define __op__atom_fence(_ord...) __atomic_thread_fence(_ord)
#define __op__atom_load(__ret, _ptr, _ord...) local_({ \
    var __ret = l0$((TypeOf(*_ptr))); \
    __atomic_load(_ptr, &__ret, as$(int)(as$(atom_MemOrd)(_ord))); \
    local_return_(__ret); \
})
#define __op__atom_store(_ptr, _val, _ord...) \
    __atomic_store(_ptr, &from$((TypeOf(*_ptr))_val), as$(int)(as$(atom_MemOrd)(_ord)))

#define __op__atom_cmpXchgWeak$( \
    __is_succ, __expected, \
    _OT, _ptr, _expected, _desired, _succ_ord, _fail_ord... \
) local_({ \
    typedef _OT O$Ret$atom_cmpXchgWeak; \
    var __expected = l$((TypeOf(*_ptr))_expected); \
    let_(__is_success, bool) = __atomic_compare_exchange( \
        _ptr, &__expected, &from$((TypeOf(__expected))_desired), true, \
        as$(int)(as$(atom_MemOrd)(_succ_ord)), as$(int)(as$(atom_MemOrd)(_fail_ord)) \
    ); \
    __is_success ? local_return_(l$((O$Ret$atom_cmpXchgWeak)none())) \
                 : local_return_(l$((O$Ret$atom_cmpXchgWeak)some(__expected))); \
})
#define __op__atom_cmpXchgStrong$( \
    __is_succ, __expected, \
    _OT, _ptr, _expected, _desired, _succ_ord, _fail_ord... \
) local_({ \
    typedef _OT O$Ret$atom_cmpXchgStrong; \
    var __expected = l$((TypeOf(*_ptr))_expected); \
    let_(__is_success, bool) = __atomic_compare_exchange( \
        _ptr, &__expected, &from$((TypeOf(__expected))_desired), false, \
        as$(int)(as$(atom_MemOrd)(_succ_ord)), as$(int)(as$(atom_MemOrd)(_fail_ord)) \
    ); \
    __is_success ? local_return_(l$((O$Ret$atom_cmpXchgStrong)none())) \
                 : local_return_(l$((O$Ret$atom_cmpXchgStrong)some(__expected))); \
})

#define __op__atom_fetchXchg(__ret, _ptr, _val, _ord...) local_({ \
    var __ret = l0$((TypeOf(*_ptr))); \
    __atomic_exchange(_ptr, &from$((TypeOf(__ret))_val), &__ret, as$(int)(as$(atom_MemOrd)(_ord))); \
    local_return_(__ret); \
})
#define __op__atom_pri_fetchAdd(_ptr, _val, _ord...) \
    __atomic_fetch_add(_ptr, from$((TypeOf(*_ptr))_val), as$(int)(as$(atom_MemOrd)(_ord)))
#define __op__atom_pri_fetchSub(_ptr, _val, _ord...) \
    __atomic_fetch_sub(_ptr, from$((TypeOf(*_ptr))_val), as$(int)(as$(atom_MemOrd)(_ord)))
#define __op__atom_pri_fetchMin__step(_ptr, _val, _ord...) __op__atom_pri_fetchMin( \
    pp_uniqTok(ptr), pp_uniqTok(operand), pp_uniqTok(expected), pp_uniqTok(desired), \
    _ptr, _val, _ord \
)
#define __op__atom_pri_fetchMax__step(_ptr, _val, _ord...) __op__atom_pri_fetchMax( \
    pp_uniqTok(ptr), pp_uniqTok(operand), pp_uniqTok(expected), pp_uniqTok(desired), \
    _ptr, _val, _ord \
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
#define __op__atom_pri_fetchMin(__ptr, __operand, __expected, __desired, _ptr, _val, _ord...) \
    __atomic_fetch_min(_ptr, from$((TypeOf(*_ptr))_val), as$(int)(as$(atom_MemOrd)(_ord)))
#define __op__atom_pri_fetchMax(__ptr, __operand, __expected, __desired, _ptr, _val, _ord...) \
    __atomic_fetch_max(_ptr, from$((TypeOf(*_ptr))_val), as$(int)(as$(atom_MemOrd)(_ord)))
#else
#define __op__atom_cmpXchgFailOrd(_ord...) ( \
    (as$(atom_MemOrd)(_ord) == atom_MemOrd_release)   ? atom_MemOrd_monotonic \
    : (as$(atom_MemOrd)(_ord) == atom_MemOrd_acq_rel) ? atom_MemOrd_acquire \
                                                      : as$(atom_MemOrd)(_ord) \
)
#define __op__atom_pri_isNaN(_val...) __builtin_isnan(as$(long double)(_val))
#define __op__atom_pri_fetchMin(__ptr, __operand, __expected, __desired, _ptr, _val, _ord...) local_({ \
    let __ptr = (_ptr); \
    typedef TypeOfUnqual(*__ptr) AtomType; \
    let_(__operand, AtomType) = _val; \
    var_(__expected, AtomType) = atom_load(__ptr, atom_MemOrd_monotonic); \
    while (true) { \
        var_(__desired, AtomType) = __op__atom_pri_isNaN(__expected) ? __operand \
                                  : __op__atom_pri_isNaN(__operand)  ? __expected \
                                  : (__operand < __expected)         ? __operand \
                                                                     : __expected; \
        if (__atomic_compare_exchange( \
                __ptr, &__expected, &__desired, false, \
                as$(int)(as$(atom_MemOrd)(_ord)), as$(int)(__op__atom_cmpXchgFailOrd(_ord)) \
            )) { \
            break; \
        } \
    } \
    local_return_(__expected); \
})
#define __op__atom_pri_fetchMax(__ptr, __operand, __expected, __desired, _ptr, _val, _ord...) local_({ \
    let __ptr = (_ptr); \
    typedef TypeOfUnqual(*__ptr) AtomType; \
    let_(__operand, AtomType) = _val; \
    var_(__expected, AtomType) = atom_load(__ptr, atom_MemOrd_monotonic); \
    while (true) { \
        var_(__desired, AtomType) = __op__atom_pri_isNaN(__expected) ? __operand \
                                  : __op__atom_pri_isNaN(__operand)  ? __expected \
                                  : (__operand > __expected)         ? __operand \
                                                                     : __expected; \
        if (__atomic_compare_exchange( \
                __ptr, &__expected, &__desired, false, \
                as$(int)(as$(atom_MemOrd)(_ord)), as$(int)(__op__atom_cmpXchgFailOrd(_ord)) \
            )) { \
            break; \
        } \
    } \
    local_return_(__expected); \
})
#endif
#define __op__atom_int_fetchNand(_ptr, _val, _ord...) \
    __atomic_fetch_nand(_ptr, from$((TypeOf(*_ptr))_val), as$(int)(as$(atom_MemOrd)(_ord)))
#define __op__atom_int_fetchAnd(_ptr, _val, _ord...) \
    __atomic_fetch_and(_ptr, from$((TypeOf(*_ptr))_val), as$(int)(as$(atom_MemOrd)(_ord)))
#define __op__atom_int_fetchXor(_ptr, _val, _ord...) \
    __atomic_fetch_xor(_ptr, from$((TypeOf(*_ptr))_val), as$(int)(as$(atom_MemOrd)(_ord)))
#define __op__atom_int_fetchOr(_ptr, _val, _ord...) \
    __atomic_fetch_or(_ptr, from$((TypeOf(*_ptr))_val), as$(int)(as$(atom_MemOrd)(_ord)))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_atom__included */
