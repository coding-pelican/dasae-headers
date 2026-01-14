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
#define fromR from$R
#define slice$R R_slice
#define sliceR slice$R
#define prefix$R R_prefix
#define prefixR prefix$R
#define suffix$R R_suffix
#define suffixR suffix$R

#define len$R comp_expand__len$R
#define lenR len$R
#define at$R comp_expand__at$R
#define atR at$R
#define contains$R R_contains
#define containsR contains$R
#define isValid$R R_isValid
#define isValidR isValid$R

#define eqR eq$R
#define eq$R R_eq
#define neR ne$R
#define ne$R R_ne

typedef enum_(R_Bound_Tag $bits(8)) {
    R_Bound_incl = 0,
    R_Bound_excl = 1,
} R_Bound;
/// default: incl
$attr($inline_always)
$static fn_((R_Bound_begin(R_Bound bound, usize point))(usize));
/// default: excl
$attr($inline_always)
$static fn_((R_Bound_end(R_Bound bound, usize point))(usize));

/// (0 <= begin) && (begin <= end)
typedef struct R {
    usize begin; ///< Beginning index (inclusive)
    usize end; ///< Ending index (exclusive)
} R;
/// [begin..end] => [begin, end)
$attr($inline_always)
$static fn_((R_from(usize begin, usize end))(R));
/// self[begin..end] => [self.begin + range.begin, self.begin + range.end)
/// (self.begin <= range.begin) && (range.begin <= range.end) && (range.end <= self.end)
$attr($inline_always)
$static fn_((R_slice(R self, R range))(R));
/// self[..end] => [self.begin, self.begin + end)
$attr($inline_always)
$static fn_((R_prefix(R self, usize end))(R));
/// self[begin..] => [self.begin + begin, self.end)
$attr($inline_always)
$static fn_((R_suffix(R self, usize begin))(R));

/// self.end - self.begin
$attr($inline_always)
$static fn_((R_len(R self))(usize));
/// self.begin + idx
$attr($inline_always)
$static fn_((R_at(R self, usize idx))(usize));

/// self.begin <= self.end
$attr($inline_always)
$static fn_((R_isValid(R self))(bool));
$attr($inline_always)
$static fn_((R_assertValid(usize begin, usize end))(void));
$attr($inline_always)
$static fn_((R_ensureValid(R self))(R));
/// (self.begin <= idx) && (idx < self.end)
$attr($inline_always)
$static fn_((R_contains(R self, usize idx))(bool));

$attr($inline_always)
$static fn_((R_eq(R lhs, R rhs))(bool));
$attr($inline_always)
$static fn_((R_ne(R lhs, R rhs))(bool));

/*========== Macros and Definitions =========================================*/

#if !on_comptime
#define comp_expand__$incl(_point...) _point
#define comp_expand__$excl(_point...) _point
#define comp_expand__$r(_begin, _end...) (R_from(_begin, _end))
$extern fn_((R__len(R))(usize));
$extern fn_((R__at(R, usize))(usize));
#define comp_expand__len$R R__len
#define comp_expand__at$R R__at
#else
#define comp_expand__$incl(_point...) R_Bound_incl, _point
#define comp_expand__$excl(_point...) R_Bound_excl, _point
#define comp_expand__$r(_begin, _end...) (R_from($r_begin(_begin), $r_end(_end)))
#define comp_expand__len$R R_len
#define comp_expand__at$R R_at
#endif

$attr($inline_always)
$static fn_((R_Bound_begin(R_Bound bound, usize point))(usize)) { return point + as$(usize)(bound); /* bound == R_Bound_Tag_incl ? point : point + 1 */ }
$attr($inline_always)
$static fn_((R_Bound_end(R_Bound bound, usize point))(usize)) { return point + (1 - as$(usize)(bound)); /* bound == R_Bound_Tag_excl ? point : point + 1 */ }

$attr($inline_always)
$static fn_((R_from(usize begin, usize end))(R)) {
    return R_assertValid(begin, end), (R){ .begin = begin, .end = end };
}
$attr($inline_always)
$static fn_((R_slice(R self, R range))(R)) {
    self = R_ensureValid(self);
    claim_assert_fmt(self.begin <= range.begin, "Invalid slice range: self.begin({:uz}) > range.begin({:uz})", self.begin, range.begin);
    range = R_ensureValid(self);
    claim_assert_fmt(range.end <= self.end, "Invalid slice range: range.end({:uz}) > self.end({:uz})", range.end, self.end);
    return (R){ .begin = self.begin + range.begin, .end = self.begin + range.end };
}
$attr($inline_always)
$static fn_((R_prefix(R self, usize end))(R)) {
    self = R_ensureValid(self);
    claim_assert_fmt(self.begin + end <= self.end, "Invalid slice range: self.begin({:uz}) + end({:uz}) > self.end({:uz})", self.begin, end, self.end);
    return (R){ .begin = self.begin, .end = self.begin + end };
}
$attr($inline_always)
$static fn_((R_suffix(R self, usize begin))(R)) {
    self = R_ensureValid(self);
    claim_assert_fmt(self.begin + begin <= self.end, "Invalid slice range: self.begin({:uz}) + begin({:uz}) > self.end({:uz})", self.begin, begin, self.end);
    return (R){ .begin = self.begin + begin, .end = self.end };
}
$attr($inline_always)
$static fn_((R_len(R self))(usize)) {
    self = R_ensureValid(self);
    return self.end - self.begin;
}
$attr($inline_always)
$static fn_((R_at(R self, usize idx))(usize)) {
    claim_assert_fmt(R_contains(self, self.begin + idx), "Index out of bounds: self.begin({:uz}) + idx({:uz}) >= self.end({:uz})", self.begin, idx, self.end);
    return self.begin + idx;
}

$attr($inline_always)
$static fn_((R_isValid(R self))(bool)) {
    return self.begin <= self.end;
};
$attr($inline_always)
$static fn_((R_assertValid(usize begin, usize end))(void)) {
    claim_assert_fmt(begin <= end, "Invalid range: begin({:uz}) > end({:uz})", begin, end);
}
$attr($inline_always)
$static fn_((R_ensureValid(R self))(R)) {
    return R_assertValid(self.begin, self.end), self;
};
$attr($inline_always)
$static fn_((R_contains(R self, usize idx))(bool)) {
    self = R_ensureValid(self);
    return self.begin <= idx && idx < self.end;
};

$attr($inline_always)
$static fn_((R_eq(R lhs, R rhs))(bool)) { return lhs.begin == rhs.begin && lhs.end == rhs.end; };
$attr($inline_always)
$static fn_((R_ne(R lhs, R rhs))(bool)) { return !R_eq(lhs, rhs); };

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* core_range__included */
