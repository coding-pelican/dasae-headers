/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Iter.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-08 (date of creation)
 * @updated 2024-12-08 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core
 * @prefix  Iter
 *
 * @brief   Generic Iterator type implementation
 * @details Implements iterator pattern for slice-containing types
 */

#ifndef CORE_ITER_INCLUDED
#define CORE_ITER_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core/prim.h"
#include "dh/core/ptr.h"
#include "dh/core/Opt.h"
#include "dh/debug/assert.h"

/*========== Iterator Definition ===========================================*/

typedef struct Iter {
    Mptr begin;
    Mptr end;
} Iter;
#define Iter(THint) typedef Iter

/* Iterator creation */
force_inline Iter Iter_from(Slice slice) {
    debug_assert_true(!Slice_isUndefined(slice));
    return (Iter){
        .begin = Slice_beginMptr(slice),
        .end   = Slice_endMptr(slice)
    };
}

/* State checks */
force_inline bool Iter_isDone(const Iter* self) {
    debug_assert_nonnull(self);
    return Mptr_rawCast(anyptr, self->begin) >= Mptr_rawCast(anyptr, self->end);
}

force_inline bool Iter_hasNext(const Iter* self) {
    debug_assert_nonnull(self);
    return Mptr_rawCast(anyptr, self->begin) < Mptr_rawCast(anyptr, self->end);
}

/* Iterator operations */
force_inline Opt_Sptr Iter_peek(const Iter* self) {
    debug_assert_nonnull(self);

    if (Iter_isDone(self)) {
        return Opt_none(Opt_Sptr);
    }

    return Opt_some(Opt_Sptr, Mptr_atSptr(self->begin, 0));
}

force_inline Opt_Sptr Iter_next(Iter* self) {
    debug_assert_nonnull(self);

    if (Iter_isDone(self)) {
        return Opt_none(Opt_Sptr);
    }

    Sptr current = Mptr_atSptr(self->begin, 0);
    self->begin  = Mptr_add(self->begin, 1);

    return Opt_some(Opt_Sptr, current);
}

/* For loop macro */
// NOLINTBEGIN
#define foreach(T, var, slice)                                                    \
    for (Iter* iter = (Iter[1]){ Iter_from(slice) }; iter != null; iter = null)   \
        for (                                                                     \
            T* var = Sptr_rawCast(anyptr, Opt_unwrap(Opt_Sptr, Iter_peek(iter))); \
            !Iter_isDone(iter);                                                   \
            (var) = Sptr_rawCast(anyptr, Opt_unwrap(Opt_Sptr, Iter_next(iter)))   \
        )
// NOLINTEND

/*========== Built-in Iterator Types ========================================*/

Iter(u8) Iter_u8;
Iter(u16) Iter_u16;
Iter(u32) Iter_u32;
Iter(u64) Iter_u64;
Iter(usize) Iter_usize;

Iter(i8) Iter_i8;
Iter(i16) Iter_i16;
Iter(i32) Iter_i32;
Iter(i64) Iter_i64;
Iter(isize) Iter_isize;

Iter(f32) Iter_f32;
Iter(f64) Iter_f64;

Iter(bool) Iter_bool;
Iter(char) Iter_char;

Iter(anyptr) Iter_anyptr;
Iter(PtrBase) Iter_PtrBase;
Iter(Sptr) Iter_Sptr;
Iter(Mptr) Iter_Mptr;
Iter(Slice) Iter_Slice;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_ITER_INCLUDED */
