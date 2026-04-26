/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    CompHash.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-27 (date of creation)
 * @updated 2026-04-26 (date of last update)
 * @ingroup dal-project/da-dsl/prelude
 * @prefix  (none)
 *
 * @brief   String hashing implementation using FNV-1a hash algorithm
 * @details This implementation uses a FNV-1a hash algorithm
 *          with a multiplier of 65599 for efficient string hashing.
 *          It provides a compile-time hash value for null-terminated strings
 *          and a runtime hash value for any string.
 */
#if CHEAT_SHEET
/* Usage Examples ===========================================================*/

/* Directly calculate hash value from string literal */
const u32 hash_val = compHash("Hello");
/* Calculate hash value from byte slice */
const CompHash hashed = CompHash_from(u8_l("world"));
/* Extract hash value from structure */
const u32 hashed_val = CompHash_val(hashed);
/* Directly calculate hash value from byte slice */
const u32 calc_val = CompHash_calc(u8_l("world"));

/* API Reference List =======================================================*/

/// String hash value container structure
typedef struct CompHash {
    u32 val; ///< The 32-bit hash value
} CompHash;

/// Directly calculate hash value from a null-terminated string
fn_((compHash(const char* raw_str))(u32));
/// Create a CompHash from a string
fn_((CompHash_from(S_const$u8 str))(CompHash));
/// Extract the hash value from a CompHash
fn_((CompHash_val(CompHash self))(u32));
/// Directly calculate hash value from a string
fn_((CompHash_calc(S_const$u8 str))(u32));

#endif /* CHEAT_SHEET */
#ifndef da_dsl_prelude_CompHash__included
#define da_dsl_prelude_CompHash__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

/// @brief String hash value container structure
/// @details Holds the calculated 32-bit hash value for a string
T_alias$((CompHash)(struct CompHash {
    var_(val, u32); ///< The 32-bit hash value
}));

/// @brief  Directly calculate hash value from a null-terminated string
/// @param  raw_str Null-terminated string to hash
/// @return 32-bit hash value
/// @example
///   u32 hash = compHash("hello");
///   // hash now contains the 32-bit hash value for "hello"
#define compHash(_raw_str...) \
    ____compHash(_raw_str)

/// @brief  Create a CompHash from a string
/// @param  str String to hash
/// @return CompHash structure containing the hash value
/// @example
///   CompHash hash = CompHash_from(u8_l("hello"));
///   // hash.value now contains the hash for "hello"
$attr($inline_always)
$static fn_((CompHash_from(S_const$u8 str))(CompHash));

/// @brief  Extract the hash value from a CompHash structure
/// @param  self CompHash structure
/// @return 32-bit hash value
/// @example
///   CompHash hash = CompHash_from(u8_l("hello"));
///   u32 value = CompHash_val(hash);
///   // value now contains the 32-bit hash value
$attr($inline_always)
$static fn_((CompHash_val(CompHash self))(u32));

/// @brief  Directly calculate hash value from a string
/// @param  str String to hash
/// @return 32-bit hash value
/// @example
///   u32 hash = CompHash_calc(u8_l("hello"));
///   // hash now contains the 32-bit hash value for "hello"
$attr($inline_always)
$static fn_((CompHash_calc(S_const$u8 str))(u32));

/*========== Macros and Definitions =========================================*/

#define ____compHash(_raw_str...) \
    /** Calculate hash value from a compile-time string literal */ \
    CompHash_calc(u8_l(_raw_str))

fn_((CompHash_from(S_const$u8 str))(CompHash)) {
    claim_assert_nonnullS(str);
    return (CompHash){ .val = isZero(str.len) ? 0 : CompHash_calc(str) };
};

fn_((CompHash_val(CompHash self))(u32)) {
    return self.val;
};

fn_((CompHash_calc(S_const$u8 str))(u32)) {
    claim_assert_nonnullS(str);
    var_(hash, u32) = 0;
    for_(($s(str))(ch)) {
        hash = 65599 * hash + *ch;
    } $end(for);
    return hash ^ (hash >> 16);
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* da_dsl_prelude_CompHash__included */
