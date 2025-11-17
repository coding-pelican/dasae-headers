/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    StrCompHash.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-27 (date of creation)
 * @updated 2025-03-23 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  StrCompHash
 *
 * @brief   String hashing implementation using DJB hash algorithm
 * @details This implementation uses a variant of DJB hash algorithm
 *          with a multiplier of 65599 for efficient string hashing.
 *          It provides a compile-time hash value for null-terminated strings
 *          and a runtime hash value for any string.
 */

/*========== Cheat Sheet ====================================================*/

#if CHEAT_SHEET
/* Creating Hash Values */
StrCompHash hash1 = StrCompHash_createRaw("hello"); // From raw C string
S_const$u8 str = Str_fromRaw("world");
StrCompHash hash2 = StrCompHash_create(str); // From S_const$u8

/* Calculating Hash Values */
u32 value1 = StrCompHash_calculateRaw("hello"); // From raw C string
u32 value2 = StrCompHash_calculate(data, 5);    // From byte array with length
u32 value3 = StrCompHash_value(hash1);          // Extract from StrCompHash

/* Hash Value Based on String Length */
u32 hash_1char = StrCompHash__char1('a');                // Single character hash
u32 hash_2chars = StrCompHash__char2((const u8*)"ab");   // 2-character string hash
u32 hash_3chars = StrCompHash__char3((const u8*)"abc");  // 3-character string hash
u32 hash_4chars = StrCompHash__char4((const u8*)"abcd"); // 4-character string hash

/* API Reference List =======================================================*/

/// String hash value container structure
typedef struct StrCompHash {
    u32 value; ///< The 32-bit hash value
} StrCompHash;

/* Creating Hash Values */
/// Calculate hash value from a null-terminated string
fn_(StrCompHash_calculateRaw(const char* raw_str), u32);
/// Create a StrCompHash from a null-terminated string
fn_(StrCompHash_createRaw(const char* raw_str), StrCompHash);

/* Calculating Hash Values */
/// Calculate hash value from a byte array with specified length
fn_(StrCompHash_calculate(const u8* ptr, usize len), u32);
/// Create a StrCompHash from a S_const$u8
fn_(StrCompHash_create(S_const$u8 str), StrCompHash);
/// Extract the hash value from a StrCompHash structure
fn_(StrCompHash_value(StrCompHash self), u32);

/* Base Cases for Different String Lengths */
#define StrCompHash__char1(ch)       // Hash single character
#define StrCompHash__char2(str)      // Hash 2-character string
#define StrCompHash__char3(str)      // Hash 3-character string
#define StrCompHash__char4(str)      // Hash 4-character string
#define StrCompHash__recur(ptr, len) // Recursively hash string of any length

/* Usage Examples ===========================================================*/

// Creating and using hash values
StrCompHash hash = StrCompHash_createRaw("hello");
u32 value = StrCompHash_value(hash); // Get the numeric hash value

// Directly calculating hash values
u32 direct_hash = StrCompHash_calculateRaw("hello");

// Using with byte arrays
const u8* data = (const u8*)"hello";
u32 byte_hash = StrCompHash_calculate(data, 5);

// Using with S_const$u8 strings
S_const$u8 str = Str_fromRaw("hello");
StrCompHash str_hash = StrCompHash_create(str);
#endif /* CHEAT_SHEET */

/*========== Header =========================================================*/

#ifndef STR_COMP_HASH_INCLUDED
#define STR_COMP_HASH_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"

/*========== Macros and Declarations ========================================*/

/// @brief String hash value container structure
/// @details Holds the calculated 32-bit hash value for a string
typedef struct StrCompHash {
    u32 value; ///< The 32-bit hash value
} StrCompHash;

#if !on_comptime
/// @brief  Calculate hash value from a null-terminated string
/// @param  raw_str Null-terminated string to hash
/// @return 32-bit hash value
/// @example
///   u32 hash = StrCompHash_calculateRaw("hello");
///   // hash now contains the 32-bit hash value for "hello"
$inline_always fn_((StrCompHash_calculateRaw(const char* raw_str))(u32));

/// @brief  Create a StrCompHash from a null-terminated string
/// @param  raw_str Null-terminated string to hash
/// @return StrCompHash structure containing the hash value
/// @example
///   StrCompHash hash = StrCompHash_createRaw("hello");
///   // hash.value now contains the hash for "hello"
$inline_always fn_((StrCompHash_createRaw(const char* raw_str))(StrCompHash));
#endif /* !on_comptime */

/// @brief  Calculate hash value from a byte array with specified length
/// @param  ptr Pointer to byte array
/// @param  len Length of the byte array
/// @return 32-bit hash value
/// @example
///   const u8* data = as$(const u8*)("hello");
///   u32 hash = StrCompHash_calculate(data, 5);
///   // hash now contains the 32-bit hash value for "hello"
$inline_always fn_((StrCompHash_calculate(const u8* ptr, usize len))(u32));

/// @brief  Create a StrCompHash from a S_const$u8
/// @param  str Constant string to hash
/// @return StrCompHash structure containing the hash value
/// @example
///   S_const$u8 str = Str_fromRaw("hello");
///   StrCompHash hash = StrCompHash_create(str);
///   // hash.value now contains the hash for "hello"
$inline_always fn_((StrCompHash_create(S_const$u8 str))(StrCompHash));

/// @brief  Extract the hash value from a StrCompHash structure
/// @param  self StrCompHash structure
/// @return 32-bit hash value
/// @example
///   StrCompHash hash = StrCompHash_create(Str_fromRaw("hello"));
///   u32 value = StrCompHash_value(hash);
///   // value now contains the 32-bit hash value
$inline_always fn_((StrCompHash_value(StrCompHash self))(u32));

/* Base cases for recursive macro-based hashing =============================*/

#define StrCompHash__char1(_raw_ch) \
    /** \
     * @brief Calculates hash value for a single character \
     * @param _raw_ch Character to hash \
     * @return 32-bit hash value \
     * @example \
     *   u32 hash = comp_op__StrCompHash__char1('a'); \
     *   // hash now contains the 32-bit hash value for character 'a' \
     */ \
    comp_op__StrCompHash__char1(_raw_ch)

#define StrCompHash__char2(_raw_str) \
    /** \
     * @brief Calculates hash value for a 2-character string \
     * @param _raw_str String to hash \
     * @return 32-bit hash value \
     * @example \
     *   const u8 str[2] = {'a', 'b'}; \
     *   u32 hash = comp_op__StrCompHash__char2(str); \
     *   // hash now contains the 32-bit hash value for "ab" \
     */ \
    comp_op__StrCompHash__char2(_raw_str)

#define StrCompHash__char3(_raw_str) \
    /** \
     * @brief Calculates hash value for a 3-character string \
     * @param _raw_str String to hash \
     * @return 32-bit hash value \
     * @example \
     *   const u8 str[3] = {'a', 'b', 'c'}; \
     *   u32 hash = comp_op__StrCompHash__char3(str); \
     *   // hash now contains the 32-bit hash value for "abc" \
     */ \
    comp_op__StrCompHash__char3(_raw_str)

#define StrCompHash__char4(_raw_str) \
    /** \
     * @brief Calculates hash value for a 4-character string \
     * @param _raw_str String to hash \
     * @return 32-bit hash value \
     * @example \
     *   const u8 str[4] = {'a', 'b', 'c', 'd'}; \
     *   u32 hash = comp_op__StrCompHash__char4(str); \
     *   // hash now contains the 32-bit hash value for "abcd" \
     */ \
    comp_op__StrCompHash__char4(_raw_str)

/// @brief Recursively calculates hash value for string of any length
/// @param ptr Pointer to byte array
/// @param len Length of the byte array
/// @return 32-bit hash value
/// @details Uses specialized base cases for strings of length 1-4 and recursion for longer strings.
///          Implements a variant of the DJB hash algorithm with multiplier 65599.
/// @example
///   const u8* data = as$(const u8*)("hello");
///   u32 hash = StrCompHash__recur(data, 5);
///   // hash now contains the 32-bit hash value for "hello"
static $inline fn_((StrCompHash__recur(const u8* ptr, usize len))(u32));

/*========== Macros and Definitions =========================================*/

#if on_comptime
#define StrCompHash_calculateRaw(_raw_str) comp_op__StrCompHash_calculateRaw(_raw_str)
#define StrCompHash_createRaw(_raw_str)    comp_op__StrCompHash_createRaw(_raw_str)
#endif /* on_comptime */
#define comp_op__StrCompHash_calculateRaw(_raw_str) \
    /** Calculate hash value from a compile-time string literal */ \
    ((u32)StrCompHash__recur(_raw_str, sizeof(_raw_str) - 1))
#define comp_op__StrCompHash_createRaw(_raw_str) \
    /** Create a StrCompHash from a compile-time string literal */ \
    ((StrCompHash){ .value = StrCompHash_calculateRaw(_raw_str) })

#define comp_op__StrCompHash__char1(_raw_ch)  (as$(u32)(_raw_ch))
#define comp_op__StrCompHash__char2(_raw_str) (as$(u32)((_raw_str)[1] + 65599 * (_raw_str)[0]))
#define comp_op__StrCompHash__char3(_raw_str) (as$(u32)((_raw_str)[2] + 65599 * StrCompHash__char2(_raw_str)))
#define comp_op__StrCompHash__char4(_raw_str) (as$(u32)((_raw_str)[3] + 65599 * StrCompHash__char3(_raw_str)))
static $inline fn_((StrCompHash__recur(const u8* ptr, usize len))(u32)) { /* NOLINT(misc-no-recursion) */
    if (len == 1) { return StrCompHash__char1(ptr[0]); }
    if (len == 2) { return StrCompHash__char2(ptr); }
    if (len == 3) { return StrCompHash__char3(ptr); }
    if (len == 4) { return StrCompHash__char4(ptr); }
    // clang-format off
    return ptr[len - 1] + 65599
        * (ptr[len - 2] + 65599
        * (ptr[len - 3] + 65599
        * (ptr[len - 4] + 65599 * StrCompHash__recur(ptr, len - 4))
        )
    );
    // clang-format on
}

$inline_always fn_((StrCompHash_calculate(const u8* ptr, usize len))(u32)) {
    return StrCompHash__recur(ptr, len);
}
$inline_always fn_((StrCompHash_create(S_const$u8 str))(StrCompHash)) {
    return (StrCompHash){ .value = StrCompHash_calculate(str.ptr, str.len) };
}
$inline_always fn_((StrCompHash_value(StrCompHash self))(u32)) {
    return self.value;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* STR_COMP_HASH_INCLUDED */
