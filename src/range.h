/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    range.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-23 (date of creation)
 * @updated 2024-10-23 (date of last update)
 * @version v1.0.0
 * @ingroup SOME_GROUP
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef RANGE_INCLUDED
#define RANGE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "primitive_types.h"


typedef struct Range Range;
struct Range {
    usize start; // start index in array
    usize end; // end index in array
};
#define Range_(...)               ((Range){ __VA_ARGS__ })
#define Range_make_(_start, _end) Range_(.start = (_start), .end = (_end))
Range Range_make(usize start, usize end);
usize Range_Length(const Ref(Range) self);
bool  Range_IsValid(const Ref(Range) self);
bool  Range_Contains(const Ref(Range) self, usize index);

bool Range_eq(Range a, Range b);
bool Range_ne(Range a, Range b);


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* RANGE_INCLUDED */
