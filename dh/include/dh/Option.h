/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Option.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-29 (date of creation)
 * @updated 2024-11-30 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  Option
 *
 * @brief   Header for Option type handling
 * @details Generic Option type for error handling
 */

#ifndef OPTION_INCLUDED
#define OPTION_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core/prim.h"
#include "dh/core/ptr.h"
#include "dh/debug/assert.h"

/*========== Macros and Definitions =========================================*/

// Option tag enum

typedef enum OptionTag {
    OptionTag_none = 0,
    OptionTag_some
} OptionTag;

// Base Option structure for internal use
typedef struct OptionBase {
    bool has_value;
    union {
        anyptr value;
    } data;
} OptionBase;

force_inline bool OptionBase_hasValue(OptionBase* self) {
    return self->has_value;
}

force_inline OptionBase* OptionBase_unwrap(OptionBase* self) {
    return debug_assert_true(OptionBase_hasValue(self)), self;
}

force_inline OptionBase* OptionBase_unwrapOr(OptionBase* self, OptionBase* default_val) {
    return OptionBase_hasValue(self) ? self : default_val;
}

// Option definition for concrete types
#define Option(T)           \
    typedef union {         \
        OptionBase Base[1]; \
        struct {            \
            bool has_value; \
            union {         \
                T value;    \
            } data;         \
        };                  \
    }

// Option creation
#define Option_some(TOption, val) ((TOption){ .has_value = true, .data.value = (val) })
#define Option_none(TOption)      ((TOption){ .has_value = false })

// Option operations
#define Option_hasValue(self) OptionBase_hasValue((OptionBase*)&(self))

#define Option_unwrap(TOption, self) \
    (((TOption*)OptionBase_unwrap((OptionBase*)(TOption[1]){ self }))->data.value) // NOLINT

#define Option_unwrapOr(TOption, self, default_val) \
    (((TOption*)OptionBase_unwrapOr((OptionBase*)(TOption[1]){ self }, (OptionBase*)(TOption[1]){ Option_some(TOption, default_val) }))->data.value) // NOLINT

// Option built-in types
Option(u8) Option_u8;
Option(u16) Option_u16;
Option(u32) Option_u32;
Option(u64) Option_u64;
Option(uptr) Option_uptr;
Option(usize) Option_usize;

Option(i8) Option_i8;
Option(i16) Option_i16;
Option(i32) Option_i32;
Option(i64) Option_i64;
Option(iptr) Option_iptr;
Option(isize) Option_isize;

Option(f32) Option_f32;
Option(f64) Option_f64;

Option(bool) Option_bool;
Option(char) Option_char;

Option(anyptr) Option_anyptr;
Option(cptr) Option_cptr;
Option(sptr) Option_sptr;
Option(mptr) Option_mptr;
Option(Slice) Option_Slice;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* OPTION_INCLUDED */
