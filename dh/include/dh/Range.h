/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Range.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-23 (date of creation)
 * @updated 2025-04-18 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)
 * @prefix  Range
 *
 * @brief   Range type implementation
 * @details Provides a range type that can be used to represent a range of values.
 */

#ifndef RANGE_INCLUDED
#define RANGE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "fn.h"

/*========== Macros and Declarations ========================================*/

/// Range type for slice indexing operations
typedef struct Range {
    usize begin; ///< Beginning index (inclusive)
    usize end;   ///< Ending index (exclusive)
} Range;

/// Create a Range from begin and end indices (exclusive) [begin..end] => [begin, end)
$inline_always fn_(Range_from(usize begin, usize end), Range);
/// Create a Range from begin to specified end (exclusive) [begin..] => [begin, self.end)
$inline_always fn_(Range_prefix(Range self, usize end), Range);
/// Create a Range from specified begin to end (exclusive) [..end] => [self.begin, end)
$inline_always fn_(Range_suffix(Range self, usize begin), Range);
/// Get the length of a Range
$inline_always fn_(Range_len(Range self), usize);
/// Check if an index is within a Range
$inline_always fn_(Range_contains(Range self, usize index), bool);
/// Check if Range is valid (begin < end)
$inline_always fn_(Range_isValid(Range self), bool);

/// Compare two Ranges for equality
$inline_always fn_(Range_eq(Range lhs, Range rhs), bool);
/// Compare two Ranges for inequality
$inline_always fn_(Range_ne(Range lhs, Range rhs), bool);

/*========== Macros and Definitions =========================================*/

$inline_always fn_(Range_from(usize begin, usize end), Range) {
    debug_assert_fmt(begin < end, "Invalid range: begin(%zu) >= end(%zu)", begin, end);
    return (Range){ .begin = begin, .end = end };
}
$inline_always fn_(Range_prefix(Range self, usize end), Range) {
    debug_assert_fmt(end > self.begin, "Invalid range: end(%zu) <= begin(%zu)", end, self.begin);
    return (Range){ .begin = self.begin, .end = end };
}
$inline_always fn_(Range_suffix(Range self, usize begin), Range) {
    debug_assert_fmt(begin < self.end, "Invalid range: begin(%zu) >= end(%zu)", begin, self.end);
    return (Range){ .begin = begin, .end = self.end };
}
$inline_always fn_(Range_len(Range self), usize) { return self.end - self.begin; }
$inline_always fn_(Range_contains(Range self, usize index), bool) { return self.begin <= index && index < self.end; }
$inline_always fn_(Range_isValid(Range self), bool) { return self.begin < self.end; }

$inline_always fn_(Range_eq(Range lhs, Range rhs), bool) { return lhs.begin == rhs.begin && lhs.end == rhs.end; }
$inline_always fn_(Range_ne(Range lhs, Range rhs), bool) { return !Range_eq(lhs, rhs); }

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* RANGE_INCLUDED */
