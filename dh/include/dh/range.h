/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Range.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-23 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  Range
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef RANGE_INCLUDED
#define RANGE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"

/*========== Macros and Definitions =========================================*/

typedef struct Range {
    usize begin; // begin index in array
    usize end;   // end index in array
} Range;

/*========== Extern Function Prototypes =====================================*/

extern Range Range_from(usize begin, usize end);
// length == (end - begin + 1)
extern usize Range_length(Range r);
extern bool  Range_isValid(Range r);
// index must be in range [begin, end)
extern bool  Range_contains(Range r, usize index);

extern bool Range_eq(Range lhs, Range rhs);
extern bool Range_ne(Range lhs, Range rhs);

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* RANGE_INCLUDED */
