/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    range.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-23 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
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


#include "types.h"


typedef struct Range Range;
struct Range {
    usize start; // start index in array
    usize end;   // end index in array
};
Range Range_from(usize start, usize end);
usize Range_length(Range r);
bool  Range_isValid(Range r);
bool  Range_contains(Range r, usize index);

bool Range_eq(Range lhs, Range rhs);
bool Range_ne(Range lhs, Range rhs);


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* RANGE_INCLUDED */
