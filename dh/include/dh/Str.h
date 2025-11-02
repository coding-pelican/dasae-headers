/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    S$u8.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-31 (date of creation)
 * @updated 2025-02-11 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  S$u8
 *
 * @brief   String type and manipulation utilities
 * @details Provides string slicing, UTF-8 handling, and string manipulation functions
 */

#ifndef STR_INCLUDED
#define STR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"
#include "mem/Allocator.h"

/*========== String Types ===================================================*/

// /// Unmodifiable raw string (const byte null terminated slice)
// typedef SZ_const$u8 StrZ_const;
// /// Modifiable raw string (byte null terminated slice)
// typedef SZ$u8       StrZ;
// /// Using variant type for raw string */
// use_O$(StrZ_const);
// use_O$(StrZ);
// use_E$(StrZ_const);
// use_E$(StrZ);

// /// Unmodifiable sentinel string (const byte sentinel terminated slice)
// typedef SS_const$u8 StrS_const;
// /// Modifiable sentinel string (byte sentinel terminated slice)
// typedef SS$u8       StrS;
// /// Using variant type for sentinel string */
// use_O$(StrS_const);
// use_O$(StrS);
// use_E$(StrS_const);
// use_E$(StrS);

// /// Unmodifiable string (const byte slice)
// typedef S_const$u8 S_const$u8;
// /// Modifiable string (byte slice)
// typedef S$u8       S$u8;
// /// Using variant type for string */
// use_O$(S_const$u8);
// use_O$(S$u8);
// use_E$(S_const$u8);
// use_E$(S$u8);

/// String hash value type
typedef u32 StrHash;
T_use_O$(StrHash);

/*========== String Creation ================================================*/

/// Create S_const$u8 from const u8* with explicit length
extern S_const$u8 Str_view(const u8* ptr, usize len);
extern S_const$u8 Str_viewZ(const u8* ptr);
/// Create S$u8 from u8* with explicit length
extern S$u8 Str_from(u8 ptr[], usize len);
extern S$u8 Str_fromZ(u8 ptr[]);

/*========== String Operations ==============================================*/

/// Get string length in bytes
extern usize u8_len(S_const$u8 self);
/// Compare two strings
extern bool Str_eql(S_const$u8 lhs, S_const$u8 rhs);
/// Compare strings ignoring case
extern bool Str_eqlNoCase(S_const$u8 lhs, S_const$u8 rhs);
/// Check if string is modifiable
extern bool S_const$u8Castable(S_const$u8 self);
/// Convert S_const$u8 to S$u8 (if source is modifiable)
extern O$S$u8 S_const$u8Cast(S_const$u8 self);

/// Concatenate strings
extern E$S$u8 Str_cat(mem_Allocator allocator, S_const$u8 lhs, S_const$u8 rhs) $must_check;
/// Get substring
extern S_const$u8 Str_slice(S_const$u8 self, usize start, usize end);
/// Remove whitespace from both ends
extern S_const$u8 Str_trim(S_const$u8 self);
/// Remove whitespace from start
extern S_const$u8 u8_ltrim(S_const$u8 self);
/// Remove whitespace from end
extern S_const$u8 Str_rtrim(S_const$u8 self);
/// Convert to uppercase
extern E$S$u8 Str_upper(mem_Allocator allocator, S_const$u8 str) $must_check;
/// Convert to lowercase
extern E$S$u8 u8_lower(mem_Allocator allocator, S_const$u8 str) $must_check;

/// Check if string contains substring
extern bool Str_contains(S_const$u8 haystack, S_const$u8 needle);
/// Find first occurrence of substring
extern O$usize Str_find(S_const$u8 haystack, S_const$u8 needle, usize start);
/// Find last occurrence of substring
extern O$usize Str_rfind(S_const$u8 haystack, S_const$u8 needle, usize start);
/// Check if string starts with prefix
/// Returns index where prefix ends if found, otherwise none
extern O$usize Str_startsWith(S_const$u8 self, S_const$u8 prefix);
/// Check if string ends with suffix
/// Returns index where suffix starts if found, otherwise none
extern O$usize Str_endsWith(S_const$u8 self, S_const$u8 suffix);

/// Calculate string hash
extern StrHash Str_hash(S_const$u8 self);

/*========== Comparison Operations ==========================================*/

/* Comparison (S_const$u8) */
extern cmp_fnCmp(S_const$u8);
cmp_fnEq_default(S_const$u8);
cmp_fnNe_default(S_const$u8);
cmp_fnLt_default(S_const$u8);
cmp_fnGt_default(S_const$u8);
cmp_fnLe_default(S_const$u8);
cmp_fnGe_default(S_const$u8);

/* Comparison */
extern cmp_fnCmp(S$u8);
cmp_fnEq_default(S$u8);
cmp_fnNe_default(S$u8);
cmp_fnLt_default(S$u8);
cmp_fnGt_default(S$u8);
cmp_fnLe_default(S$u8);
cmp_fnGe_default(S$u8);

/*========== UTF-8 Constants ================================================*/

#define StrUtf8_1byte_mask /* 10000000 */ (0x80)
#define StrUtf8_2byte_mask /* 11100000 */ (0xE0)
#define StrUtf8_3byte_mask /* 11110000 */ (0xF0)
#define StrUtf8_4byte_mask /* 11111000 */ (0xF8)
#define StrUtf8_cont_mask  /* 11000000 */ (0xC0)

#define StrUtf8_1byte_pattern /* 0xxxxxxx */ (0x00)
#define StrUtf8_2byte_pattern /* 110xxxxx */ (0xC0)
#define StrUtf8_3byte_pattern /* 1110xxxx */ (0xE0)
#define StrUtf8_4byte_pattern /* 11110xxx */ (0xF0)
#define StrUtf8_cont_pattern  /* 10xxxxxx */ (0x80)

/*========== UTF-8 Operations ===============================================*/

/// Get number of UTF-8 code points in string
extern usize StrUtf8_len(S_const$u8 self);
/// Get byte length of UTF-8 sequence starting at given position
extern u8 StrUtf8_seqLen(S_const$u8 self, usize pos);
/// Check if string is valid UTF-8
extern bool StrUtf8_isValid(S_const$u8 self);
/// Get UTF-8 code point at position
extern O$u32 StrUtf8_codepointAt(S_const$u8 self, usize pos);

/// Iterator for UTF-8 code points
typedef struct StrUtf8Iter {
    S_const$u8 str;
    usize pos;
} StrUtf8Iter;

/// Get iterator for UTF-8 code points
extern StrUtf8Iter StrUtf8_iter(S_const$u8 self);
/// Iterate over UTF-8 code points
extern bool StrUtf8Iter_next(StrUtf8Iter* iter, O$u32* out_codepoint);

/*========== String Tokenization ============================================*/

/// Tokenizer for string
typedef struct StrTokenizer {
    S_const$u8 str;    // Source string
    S_const$u8 delims; // Delimiter characters
    usize pos;         // Current position
} StrTokenizer;
T_use_O$(StrTokenizer);

/// Create tokenizer for string with given delimiters
extern StrTokenizer Str_tokenizer(S_const$u8 self, S_const$u8 delims);
/// Get next token, returns none when no more tokens
extern O$S_const$u8 StrTokenizer_next(StrTokenizer* self);

/*========== Implementations ================================================*/



#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* STR_INCLUDED */
