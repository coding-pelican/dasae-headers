/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    range.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-23 (date of creation)
 * @updated 2025-04-18 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/core
 * @prefix  (none)
 */

#ifndef core_range__included
#define core_range__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "fn.h"
#include "claim.h"

/*========== Macros and Declarations ========================================*/

#define $incl(_point...) comp_expand__$incl(_point)
#define $excl(_point...) comp_expand__$excl(_point)

#define $r(_begin, _end...) comp_expand__$r(_begin, _end)
#define $r_begin(...) pp_overload(__$r_begin, __VA_ARGS__)(__VA_ARGS__)
#define __$r_begin_1(_point...) (_point)
#define __$r_begin_2(_bound_type, _point...) (R_Bound_begin(_bound_type, _point))
#define $r_end(...) pp_overload(__$r_end, __VA_ARGS__)(__VA_ARGS__)
#define __$r_end_1(_point...) (_point)
#define __$r_end_2(_bound_type, _point...) (R_Bound_end(_bound_type, _point))

#define from$R R_from
#define slice$R R_slice
#define prefix$R R_prefix
#define suffix$R R_suffix

#define len$R comp_expand__len$R
#define at$R comp_expand__at$R
#define contains$R R_contains
#define isValid$R R_isValid

#define eq$R R_eq
#define ne$R R_ne

typedef enum R_Bound_Tag : u8 {
    R_Bound_incl = 0,
    R_Bound_excl = 1,
} R_Bound;
/// default: incl
$inline_always
$static fn_((R_Bound_begin(R_Bound bound, usize point))(usize));
/// default: excl
$inline_always
$static fn_((R_Bound_end(R_Bound bound, usize point))(usize));

/// (0 <= begin) && (begin <= end)
typedef struct R {
    usize begin; ///< Beginning index (inclusive)
    usize end;   ///< Ending index (exclusive)
} R;
/// [begin..end] => [begin, end)
$inline_always
$static fn_((R_from(usize begin, usize end))(R));
/// self[begin..end] => [self.begin + range.begin, self.begin + range.end)
/// (self.begin <= range.begin) && (range.begin <= range.end) && (range.end <= self.end)
$inline_always
$static fn_((R_slice(R self, R range))(R));
/// self[..end] => [self.begin, self.begin + end)
$inline_always
$static fn_((R_prefix(R self, usize end))(R));
/// self[begin..] => [self.begin + begin, self.end)
$inline_always
$static fn_((R_suffix(R self, usize begin))(R));

/// self.end - self.begin
$inline_always
$static fn_((R_len(R self))(usize));
/// self.begin + idx
$inline_always
$static fn_((R_at(R self, usize idx))(usize));

/// self.begin <= self.end
$inline_always
$static fn_((R_isValid(R self))(bool));
/// (self.begin <= idx) && (idx < self.end)
$inline_always
$static fn_((R_contains(R self, usize idx))(bool));

$inline_always
$static fn_((R_eq(R lhs, R rhs))(bool));
$inline_always
$static fn_((R_ne(R lhs, R rhs))(bool));

/*========== Macros and Definitions =========================================*/

#if !on_comptime
#define comp_expand__$incl(_point...)    _point
#define comp_expand__$excl(_point...)    _point
#define comp_expand__$r(_begin, _end...) (R_from(_begin, _end))
$extern fn_((R__len(R))(usize));
$extern fn_((R__at(R, usize))(usize));
#define comp_expand__len$R               R__len
#define comp_expand__at$R                R__at
#else
#define comp_expand__$incl(_point...)    R_Bound_incl, _point
#define comp_expand__$excl(_point...)    R_Bound_excl, _point
#define comp_expand__$r(_begin, _end...) (R_from($r_begin(_begin), $r_end(_end)))
#define comp_expand__len$R               R_len
#define comp_expand__at$R                R_at
#endif

$inline_always
$static fn_((R_Bound_begin(R_Bound bound, usize point))(usize)) { return point + bound; /* bound == R_Bound_Tag_incl ? point : point + 1 */ }
$inline_always
$static fn_((R_Bound_end(R_Bound bound, usize point))(usize)) { return point + (1 - bound); /* bound == R_Bound_Tag_excl ? point : point + 1 */ }

$inline_always
$static fn_((R_from(usize begin, usize end))(R)) {
    claim_assert_fmt(begin <= end, "Invalid range: begin(%zu) > end(%zu)", begin, end);
    return (R){ .begin = begin, .end = end };
}
$inline_always
$static fn_((R_slice(R self, R range))(R)) {
    claim_assert_fmt(R_isValid(self), "Invalid range: self.begin(%zu) > self.end(%zu)", self.begin, self.end);
    claim_assert_fmt(self.begin <= range.begin, "Invalid slice range: self.begin(%zu) > range.begin(%zu)", self.begin, range.begin);
    claim_assert_fmt(R_isValid(range), "Invalid range: range.begin(%zu) > range.end(%zu)", range.begin, range.end);
    claim_assert_fmt(range.end <= self.end, "Invalid slice range: range.end(%zu) > self.end(%zu)", range.end, self.end);
    return (R){ .begin = self.begin + range.begin, .end = self.begin + range.end };
}
$inline_always
$static fn_((R_prefix(R self, usize end))(R)) {
    claim_assert_fmt(R_isValid(self), "Invalid range: self.begin(%zu) > self.end(%zu)", self.begin, self.end);
    claim_assert_fmt(self.begin + end <= self.end, "Invalid slice range: self.begin(%zu) + end(%zu) > self.end(%zu)", self.begin, end, self.end);
    return (R){ .begin = self.begin, .end = self.begin + end };
}
$inline_always
$static fn_((R_suffix(R self, usize begin))(R)) {
    claim_assert_fmt(R_isValid(self), "Invalid range: self.begin(%zu) > self.end(%zu)", self.begin, self.end);
    claim_assert_fmt(self.begin + begin <= self.end, "Invalid slice range: self.begin(%zu) + begin(%zu) > self.end(%zu)", self.begin, begin, self.end);
    return (R){ .begin = self.begin + begin, .end = self.end };
}
$inline_always
$static fn_((R_len(R self))(usize)) {
    claim_assert_fmt(R_isValid(self), "Invalid range: self.begin(%zu) > self.end(%zu)", self.begin, self.end);
    return self.end - self.begin;
}
$inline_always
$static fn_((R_at(R self, usize idx))(usize)) {
    claim_assert_fmt(R_contains(self, self.begin + idx), "Index out of bounds: self.begin(%zu) + idx(%zu) >= self.end(%zu)", self.begin, idx, self.end);
    return self.begin + idx;
}

$inline_always
$static fn_((R_isValid(R self))(bool)) { return self.begin <= self.end; }
$inline_always
$static fn_((R_contains(R self, usize idx))(bool)) {
    claim_assert_fmt(R_isValid(self), "Invalid range: self.begin(%zu) > self.end(%zu)", self.begin, self.end);
    return self.begin <= idx && idx < self.end;
}

$inline_always
$static fn_((R_eq(R lhs, R rhs))(bool)) { return lhs.begin == rhs.begin && lhs.end == rhs.end; }
$inline_always
$static fn_((R_ne(R lhs, R rhs))(bool)) { return !R_eq(lhs, rhs); }

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* core_range__included */
