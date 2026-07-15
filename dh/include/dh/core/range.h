/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    range.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-23 (date of creation)
 * @updated 2026-06-14 (date of last update)
 * @ingroup dasae-headers(dh)/core
 * @prefix  (none)
 */
#pragma once
#ifndef core_range__included
#define core_range__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "fn.h"
#include "claim.h"

/*========== Macros and Declarations ========================================*/

#define $incl(_$point...) comp_expand__$incl(_$point)
#define $excl(_$point...) comp_expand__$excl(_$point)

#define $r(_$begin, _$end...) comp_expand__$r(_$begin, _$end)
#define $r_begin(...) pp_overload(__$r_begin, __VA_ARGS__)(__VA_ARGS__)
#define __$r_begin_1(_$point...) (_$point)
#define __$r_begin_2(_$bound_type, _$point...) (R_Bound_begin(_$bound_type, _$point))
#define $r_end(...) pp_overload(__$r_end, __VA_ARGS__)(__VA_ARGS__)
#define __$r_end_1(_$point...) (_$point)
#define __$r_end_2(_$bound_type, _$point...) (R_Bound_end(_$bound_type, _$point))
#define $rf(_$expr...) $r(_$expr, usize_limit_max)
#define $rt(_$expr...) $r(0, _$expr)

typedef enum_((R_Bound $fits($packed))(
    R_Bound_incl = true,
    R_Bound_excl = false
)) R_Bound;
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
#define fromR R_from

/// self.begin <= self.end
$attr($inline_always)
$static fn_((R_isValid(R self))(bool));
#define isValidR R_isValid
$attr($inline_always)
$static fn_((R_assertValid(usize begin, usize end))(void));
$attr($inline_always)
$static fn_((R_ensureValid(R self))(R));

/// (self.begin <= idx) && (idx < self.end)
$attr($inline_always)
$static fn_((R_contains(R self, usize idx))(bool));
#define containsR R_contains
/// self.end - self.begin
$attr($inline_always)
$static fn_((R_len(R self))(usize));
#define lenR R_len
/// self.begin + idx
$attr($inline_always)
$static fn_((R_at(R self, usize idx))(usize));
#define atR R_at

/// self[begin..end] => [self.begin + range.begin, self.begin + range.end)
/// (self.begin <= range.begin) && (range.begin <= range.end) && (range.end <= self.end)
$attr($inline_always)
$static fn_((R_slice(R self, R range))(R));
#define sliceR R_slice
/// self[..end] => [self.begin, self.begin + end)
$attr($inline_always)
$static fn_((R_prefix(R self, usize end))(R));
#define prefixR R_prefix
/// self[begin..] => [self.begin + begin, self.end)
$attr($inline_always)
$static fn_((R_suffix(R self, usize begin))(R));
#define suffixR R_suffix

$attr($inline_always)
$static fn_((R_eq(R lhs, R rhs))(bool));
#define eqR R_eq
$attr($inline_always)
$static fn_((R_ne(R lhs, R rhs))(bool));
#define neR R_ne

/*========== Macros and Definitions =========================================*/

#if !in_comptime
#define comp_expand__$incl(_$point...) _$point
#define comp_expand__$excl(_$point...) _$point
#define comp_expand__$r(_$begin, _$end...) (R_from(_$begin, _$end))
$extern fn_((R__len(R))(usize));
$extern fn_((R__at(R, usize))(usize));
#define comp_expand__lenR R__len
#define comp_expand__atR R__at
#else
#define comp_expand__$incl(_$point...) R_Bound_incl, _$point
#define comp_expand__$excl(_$point...) R_Bound_excl, _$point
#define comp_expand__$r(_$begin, _$end...) (R_from($r_begin(_$begin), $r_end(_$end)))
#define comp_expand__lenR R_len
#define comp_expand__atR R_at
#endif

#if in_analysis_active_only || in_comptime
fn_((R_Bound_begin(R_Bound bound, usize point))(usize)) { return point + (1 - as$(usize)(bound)); /* bound == R_Bound_incl ? point : point + 1 */ };
fn_((R_Bound_end(R_Bound bound, usize point))(usize)) { return point + as$(usize)(bound); /* bound == R_Bound_excl ? point : point + 1 */ };

fn_((R_from(usize begin, usize end))(R)) {
    return R_assertValid(begin, end), (R){ .begin = begin, .end = end };
};

fn_((R_isValid(R self))(bool)) {
    return self.begin <= self.end;
};
fn_((R_assertValid(usize begin, usize end))(void)) {
    claim_assert_fmt(begin <= end, "Invalid range: begin({:uz}) > end({:uz})", begin, end);
}
fn_((R_ensureValid(R self))(R)) {
    return R_assertValid(self.begin, self.end), self;
};

fn_((R_contains(R self, usize idx))(bool)) {
    self = R_ensureValid(self);
    return self.begin <= idx && idx < self.end;
};
fn_((R_len(R self))(usize)) {
    self = R_ensureValid(self);
    return self.end - self.begin;
};
fn_((R_at(R self, usize idx))(usize)) {
    claim_assert_fmt(R_contains(self, self.begin + idx), "Index out of bounds: self.begin({:uz}) + idx({:uz}) >= self.end({:uz})", self.begin, idx, self.end);
    return self.begin + idx;
};

fn_((R_slice(R self, R range))(R)) {
    self = R_ensureValid(self);
    claim_assert_fmt(self.begin <= range.begin, "Invalid slice range: self.begin({:uz}) > range.begin({:uz})", self.begin, range.begin);
    range = R_ensureValid(range);
    claim_assert_fmt(range.end <= self.end, "Invalid slice range: range.end({:uz}) > self.end({:uz})", range.end, self.end);
    return (R){ .begin = self.begin + range.begin, .end = self.begin + range.end };
};
fn_((R_prefix(R self, usize end))(R)) {
    self = R_ensureValid(self);
    claim_assert_fmt(self.begin + end <= self.end, "Invalid slice range: self.begin({:uz}) + end({:uz}) > self.end({:uz})", self.begin, end, self.end);
    return (R){ .begin = self.begin, .end = self.begin + end };
};
fn_((R_suffix(R self, usize begin))(R)) {
    self = R_ensureValid(self);
    claim_assert_fmt(self.begin + begin <= self.end, "Invalid slice range: self.begin({:uz}) + begin({:uz}) > self.end({:uz})", self.begin, begin, self.end);
    return (R){ .begin = self.begin + begin, .end = self.end };
};

fn_((R_eq(R lhs, R rhs))(bool)) { return lhs.begin == rhs.begin && lhs.end == rhs.end; };
fn_((R_ne(R lhs, R rhs))(bool)) { return !R_eq(lhs, rhs); };
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_range__included */
