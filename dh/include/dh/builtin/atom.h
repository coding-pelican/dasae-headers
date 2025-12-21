/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    atom.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-11-04 (date of creation)
 * @updated 2025-11-04 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  atom
 */
#ifndef builtin_atom__included
#define builtin_atom__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "pp.h"
#include "auto.h"
#include "comp.h"
#include "type_info.h"

/*========== Macros and Declarations ========================================*/

typedef enum atom_MemOrd {
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

/// RMW (Read-Modify-Write) - returns the value before the operation (Fetch-and-Op)
#define atom_fetchXchg(_ptr, _val, _ord...) __op__atom_fetchXchg(pp_uniqTok(ret), _ptr, _val, _ord)
#define atom_fetchAdd(_ptr, _val, _ord...) __op__atom_fetchAdd(_ptr, _val, _ord)
#define atom_fetchSub(_ptr, _val, _ord...) __op__atom_fetchSub(_ptr, _val, _ord)
#define atom_fetchNand(_ptr, _val, _ord...) __op__atom_fetchNand(_ptr, _val, _ord)
#define atom_fetchAnd(_ptr, _val, _ord...) __op__atom_fetchAnd(_ptr, _val, _ord)
#define atom_fetchXor(_ptr, _val, _ord...) __op__atom_fetchXor(_ptr, _val, _ord)
#define atom_fetchOr(_ptr, _val, _ord...) __op__atom_fetchOr(_ptr, _val, _ord)

/// Compare-and-Swap
#define atom_cmpXchgWeak$(_OT, _ptr, _expected, _desired, _succ_ord, _fail_ord...) \
    __op__atom_cmpXchgWeak$(pp_uniqTok(expected), _OT, _ptr, _expected, _desired, _succ_ord, _fail_ord)
#define atom_cmpXchgWeak(_ptr, _expected, _desired, _succ_ord, _fail_ord...) \
    atom_cmpXchgWeak$(O$$(TypeOfUnqual(*_ptr)), _ptr, _expected, _desired, _succ_ord, _fail_ord)
#define atom_cmpXchgStrong$(_OT, _ptr, _expected, _desired, _succ_ord, _fail_ord...) \
    __op__atom_cmpXchgStrong$(pp_uniqTok(expected), _OT, _ptr, _expected, _desired, _succ_ord, _fail_ord)
#define atom_cmpXchgStrong(_ptr, _expected, _desired, _succ_ord, _fail_ord...) \
    atom_cmpXchgStrong$(O$$(TypeOfUnqual(*_ptr)), _ptr, _expected, _desired, _succ_ord, _fail_ord)

/*========== Macros and Definitions =========================================*/

#define __op__atom_fence(_ord...) __atomic_thread_fence(_ord)
#define __op__atom_load(__ret, _ptr, _ord...) ({ \
    var __ret = lit0$((TypeOf(*_ptr))); \
    __atomic_load(_ptr, &__ret, _ord); \
    __ret; \
})
#define __op__atom_store(_ptr, _val, _ord...) \
    __atomic_store(_ptr, &make$((TypeOf(*_ptr))_val), _ord)

#define __op__atom_fetchXchg(__ret, _ptr, _val, _ord...) ({ \
    var __ret = lit0$((TypeOf(*_ptr))); \
    __atomic_exchange(_ptr, &make$((TypeOf(__ret))_val), &__ret, _ord); \
    __ret; \
})
#define __op__atom_fetchAdd(_ptr, _val, _ord...) __atomic_fetch_add(_ptr, make$((TypeOf(*_ptr))_val), _ord)
#define __op__atom_fetchSub(_ptr, _val, _ord...) __atomic_fetch_sub(_ptr, make$((TypeOf(*_ptr))_val), _ord)
#define __op__atom_fetchNand(_ptr, _val, _ord...) __atomic_fetch_nand(_ptr, make$((TypeOf(*_ptr))_val), _ord)
#define __op__atom_fetchAnd(_ptr, _val, _ord...) __atomic_fetch_and(_ptr, make$((TypeOf(*_ptr))_val), _ord)
#define __op__atom_fetchXor(_ptr, _val, _ord...) __atomic_fetch_xor(_ptr, make$((TypeOf(*_ptr))_val), _ord)
#define __op__atom_fetchOr(_ptr, _val, _ord...) __atomic_fetch_or(_ptr, make$((TypeOf(*_ptr))_val), _ord)

#define __op__atom_cmpXchgWeak$(__expected, _OT, _ptr, _expected, _desired, _succ_ord, _fail_ord...) ({ \
    typedef _OT O$Ret$atom_cmpXchgWeak; \
    var __expected = lit$((TypeOf(*_ptr))_expected); \
    __atomic_compare_exchange(_ptr, &__expected, &make$((TypeOf(__expected))_desired), true, _succ_ord, _fail_ord) \
        ? lit$((O$Ret$atom_cmpXchgWeak)none()) \
        : lit$((O$Ret$atom_cmpXchgWeak)some(__expected)); \
})
#define __op__atom_cmpXchgStrong$(__expected, _OT, _ptr, _expected, _desired, _succ_ord, _fail_ord...) ({ \
    typedef _OT O$Ret$atom_cmpXchgStrong; \
    var __expected = lit$((TypeOf(*_ptr))_expected); \
    __atomic_compare_exchange(_ptr, &__expected, &make$((TypeOf(__expected))_desired), false, _succ_ord, _fail_ord) \
        ? lit$((O$Ret$atom_cmpXchgStrong)none()) \
        : lit$((O$Ret$atom_cmpXchgStrong)some(__expected)); \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_atom__included */
