/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    search.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-13 (date of creation)
 * @updated 2025-12-13 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  search
 *
 * @brief   Searching utilities
 * @details Provides searching functions for arrays and slices.
 *          Includes linear search, binary search, lower bound, upper bound,
 *          partition point, and equal range search.
 *          Supports custom comparison functions.
 */
#ifndef search__included
#define search__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"
#include "cmp.h"
#include "mem/Allocator.h"

/*========== Macros and Declarations ========================================*/

/// Callable type for searching functions that returns an ordering value.
use_Callable(search_OrdFn, (u_V$raw val, u_V$raw ctx), cmp_Ord);
/// Callable type for searching functions that returns a boolean value.
use_Callable(search_PredFn, (u_V$raw val, u_V$raw ctx), bool);

/// Performs a linear scan to find the **first** index where `ordFn(val, ctx)` is `eq`.
/// - Scans forwards from the beginning (index 0).
/// - If no such element exists, returns `none`.
/// - Time Complexity: `O(n)`
$extern fn_((search_linearFirst(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(O$usize));
/// Performs a linear scan to find the **last** index where `ordFn(val, ctx)` is `eq`.
/// - Scans backwards from the end (index `seq.len - 1`).
/// - If no such element exists, returns `none`.
/// - Time Complexity: `O(n)`
$extern fn_((search_linearLast(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(O$usize));
/// Locates an element in `seq` that compares equal to `ctx`.
/// - If multiple matches exist, any valid index may be returned.
/// - If no match is found, returns `none`.
///
/// **Precondition:** `seq` must be partitioned according to `ordFn`:
/// ```txt
/// [0]                                                     [len]
/// ┌───┬───┬─/ /─┬───┬───┬───┬─/ /─┬───┬───┬───┬─/ /─┬───┐
/// │ < │ < │ ... │ < │ = │ = │ ... │ = │ > │ > │ ... │ > │
/// └───┴───┴─/ /─┴───┴───┴───┴─/ /─┴───┴───┴───┴─/ /─┴───┘
/// ├─────────────────┼─────────────────┼─────────────────┤
///   Elements < ctx    Elements == ctx   Elements > ctx
///                   ├─────────────────┤
///                      Target Range
/// ```
///
/// - Time Complexity: `O(log n)`
///
/// See also: `lowerBound`, `upperBound`, `eqRange`
$extern fn_((search_binary(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(O$usize));
/// Finds the first index where the element is **greater than or equal to** `ctx`.
/// - If all elements are smaller than `ctx`, returns `seq.len`.
///
/// **Precondition:** `seq` must be partitioned according to `ordFn`:
/// ```txt
/// [0]                                                     [len]
/// ┌───┬───┬─/ /─┬───┬───┬───┬─/ /─┬───┬───┬───┬─/ /─┬───┐
/// │ < │ < │ ... │ < │ = │ = │ ... │ = │ > │ > │ ... │ > │
/// └───┴───┴─/ /─┴───┴───┴───┴─/ /─┴───┴───┴───┴─/ /─┴───┘
/// ├─────────────────┼───────────────────────────────────┤
///   Elements < ctx   Elements >= ctx (Target)
///                   ├───┤
///                   ^ Returned Index
/// ```
///
/// - Time Complexity: `O(log n)`
///
/// See also: `upperBound`, `eqRange`, `partPoint`
$extern fn_((search_lowerBound(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(usize));
/// Finds the first index where the element is strictly **greater than** `ctx`.
/// - If no such element exists, returns `seq.len`.
///
/// **Precondition:** `seq` must be partitioned according to `ordFn`:
/// ```txt
/// [0]                                                     [len]
/// ┌───┬───┬─/ /─┬───┬───┬───┬─/ /─┬───┬───┬───┬─/ /─┬───┐
/// │ < │ < │ ... │ < │ = │ = │ ... │ = │ > │ > │ ... │ > │
/// └───┴───┴─/ /─┴───┴───┴───┴─/ /─┴───┴───┴───┴─/ /─┴───┘
/// ├───────────────────────────────────┼─────────────────┤
///   Elements <= ctx                    Elements > ctx
///                                     ├───┤
///                                     ^ Returned Index
/// ```
///
/// - Time Complexity: `O(log n)`
///
/// See also: `lowerBound`, `eqRange`, `partPoint`
$extern fn_((search_upperBound(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(usize));
/// Finds the partition point where the predicate changes from `true` to `false`.
/// - Returns `seq.len` if the predicate holds for all elements.
///
/// **Precondition:** The sequence must be partitioned such that all elements
/// satisfying the predicate precede those that do not.
/// ```txt
/// [0]                                                [len]
/// ┌──────┬──────┬─/ /─┬──────┬───────┬─/ /─┬───────┐
/// │ true │ true │ ... │ true │ false │ ... │ false │
/// └──────┴──────┴─/ /─┴──────┴───────┴─/ /─┴───────┘
/// ├──────────────────────────┼─────────────────────┤
///   Predicate Satisfied        Predicate Failed
///                            ├───┤
///                            ^ Returned Index
/// ```
///
/// - Time Complexity: `O(log n)`
$extern fn_((search_partPoint(u_S_const$raw seq, u_V$raw ctx, search_PredFn predFn))(usize));
/// Returns the range of indices where elements compare equal to `ctx`.
/// - Returns `[i, i)` (empty range) if no match is found.
/// - The range corresponds to `[lowerBound, upperBound)`.
///
/// **Precondition:** `seq` must be partitioned according to `ordFn`:
/// ```txt
/// [0]                                                     [len]
/// ┌───┬───┬─/ /─┬───┬───┬───┬─/ /─┬───┬───┬───┬─/ /─┬───┐
/// │ < │ < │ ... │ < │ = │ = │ ... │ = │ > │ > │ ... │ > │
/// └───┴───┴─/ /─┴───┴───┴───┴─/ /─┴───┴───┴───┴─/ /─┴───┘
/// ├─────────────────┼─────────────────┼─────────────────┤
///      < ctx             == ctx             > ctx
///                   ├─────────────────┤
///                     Returned Range
/// ```
///
/// - Time Complexity: `O(log n)`
///
/// See also: `lowerBound`, `upperBound`
$extern fn_((search_eqRange(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(R));

#if defined(__cplusplus)
} /* $extern "C" */
#endif /* defined(__cplusplus) */
#endif /* search__included */
