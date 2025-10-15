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
 * @prefix  R
 */

#ifndef R_INCLUDED
#define R_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "fn.h"

/*========== Macros and Declarations ========================================*/

#define $incl(_point...) R_Bound_incl, _point
#define $excl(_point...) R_Bound_excl, _point

#define $r(_begin, _end...)                  R_from($r_begin(_begin), $r_end(_end))
#define $r_begin(...)                        pp_overload(__$r_begin, __VA_ARGS__)(__VA_ARGS__)
#define __$r_begin_1(_point...)              R_Bound_begin(R_Bound_incl, _point)
#define __$r_begin_2(_bound_type, _point...) R_Bound_begin(_bound_type, _point)
#define $r_end(...)                          pp_overload(__$r_end, __VA_ARGS__)(__VA_ARGS__)
#define __$r_end_1(_point...)                R_Bound_end(R_Bound_excl, _point)
#define __$r_end_2(_bound_type, _point...)   R_Bound_end(_bound_type, _point)

#define from$R   R_from
#define prefix$R R_prefix
#define suffix$R R_suffix

#define len$R      R_len
#define contains$R R_contains
#define isValid$R  R_isValid

#define eq$R R_eq
#define ne$R R_ne

typedef enum R_Bound : u8 {
    R_Bound_incl = 0,
    R_Bound_excl = 1,
} R_Bound;
/// default: incl
$static $inline_always
fn_(R_Bound_begin(R_Bound bound, usize point), usize);
/// default: excl
$static $inline_always
fn_(R_Bound_end(R_Bound bound, usize point), usize);

typedef struct R {
    usize begin; ///< Beginning index (inclusive)
    usize end;   ///< Ending index (exclusive)
} R;
/// [begin..end] => [begin, end)
$static $inline_always
fn_(R_from(usize begin, usize end), R);
/// [begin..] => [begin, self.end)
$static $inline_always
fn_(R_prefix(R self, usize end), R);
/// [..end] => [self.begin, end)
$static $inline_always
fn_(R_suffix(R self, usize begin), R);

$static $inline_always
fn_(R_len(R self), usize);
$static $inline_always
fn_(R_contains(R self, usize index), bool);
$static $inline_always
/// begin <= end
fn_(R_isValid(R self), bool);

$static $inline_always
fn_(R_eq(R lhs, R rhs), bool);
$static $inline_always
fn_(R_ne(R lhs, R rhs), bool);

/*========== Macros and Definitions =========================================*/

$static $inline_always
fn_(R_Bound_begin(R_Bound bound, usize point), usize) { return bound == R_Bound_incl ? point : point + 1; }
$static $inline_always
fn_(R_Bound_end(R_Bound bound, usize point), usize) { return bound == R_Bound_excl ? point : point + 1; }

$static $inline_always
fn_(R_from(usize begin, usize end), R) {
    debug_assert_fmt(begin <= end, "Invalid range: begin(%zu) > end(%zu)", begin, end);
    return (R){ .begin = begin, .end = end };
}
$static $inline_always
fn_(R_prefix(R self, usize end), R) {
    debug_assert_fmt(self.begin <= end, "Invalid range: begin(%zu) > end(%zu)", self.begin, end);
    return (R){ .begin = self.begin, .end = end };
}
$static $inline_always
fn_(R_suffix(R self, usize begin), R) {
    debug_assert_fmt(begin <= self.end, "Invalid range: begin(%zu) > end(%zu)", begin, self.end);
    return (R){ .begin = begin, .end = self.end };
}
$static $inline_always
fn_(R_len(R self), usize) { return self.end - self.begin; }
$static $inline_always
fn_(R_contains(R self, usize index), bool) { return self.begin <= index && index < self.end; }
$static $inline_always
fn_(R_isValid(R self), bool) { return self.begin <= self.end; }

$static $inline_always
fn_(R_eq(R lhs, R rhs), bool) { return lhs.begin == rhs.begin && lhs.end == rhs.end; }
$static $inline_always
fn_(R_ne(R lhs, R rhs), bool) { return !R_eq(lhs, rhs); }

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* R_INCLUDED */
