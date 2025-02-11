/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Str.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-31 (date of creation)
 * @updated 2025-02-11 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  Str
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

#include "meta/common.h"
#include "mem/Allocator.h"

/*========== String Types ===================================================*/

/// Unmodifiable string (const byte slice)
typedef SliConst$u8 StrConst;
/// Modifiable string (byte slice)
typedef Sli$u8      Str;
use_Opt$(StrConst);
use_Opt$(Str);
use_Err$(StrConst);
use_Err$(Str);

/// String hash value type
typedef u32 StrHash;
use_Opt$(StrHash);

/*========== String Creation ================================================*/

#define Str_l(_literal_as_readonly...) /* For read-only string literals */ SYN__Str_l(_literal_as_readonly)
#define Str_m(_literal_to_writable...) /* For writable string literals */ SYN__Str_m(_literal_to_writable)

/// Create StrConst from const u8* with explicit length
extern StrConst Str_const(const u8* ptr, usize len);
/// Create Str from u8* with explicit length
extern Str      Str_from(u8 ptr[], usize len);

/*========== String Operations ==============================================*/

/// Get string length in bytes
extern usize   Str_len(StrConst self);
/// Compare two strings
extern bool    Str_eql(StrConst lhs, StrConst rhs);
/// Compare strings ignoring case
extern bool    Str_eqlNoCase(StrConst lhs, StrConst rhs);
/// Check if string is modifiable
extern bool    Str_constCastable(StrConst self);
/// Convert StrConst to Str (if source is modifiable)
extern Opt$Str Str_constCast(StrConst self);

/// Concatenate strings
extern Err$Str  Str_cat(mem_Allocator allocator, StrConst lhs, StrConst rhs) must_check;
/// Format string with arguments
extern Err$Str  Str_format(mem_Allocator allocator, const char* format, ...) must_check;
/// Get substring
extern StrConst Str_slice(StrConst self, usize start, usize end);
/// Remove whitespace from both ends
extern StrConst Str_trim(StrConst self);
/// Remove whitespace from start
extern StrConst Str_ltrim(StrConst self);
/// Remove whitespace from end
extern StrConst Str_rtrim(StrConst self);
/// Convert to uppercase
extern Err$Str  Str_upper(mem_Allocator allocator, StrConst str) must_check;
/// Convert to lowercase
extern Err$Str  Str_lower(mem_Allocator allocator, StrConst str) must_check;

/// Check if string contains substring
extern bool      Str_contains(StrConst haystack, StrConst needle);
/// Find first occurrence of substring
extern Opt$usize Str_find(StrConst haystack, StrConst needle, usize start);
/// Find last occurrence of substring
extern Opt$usize Str_rfind(StrConst haystack, StrConst needle, usize start);
/// Check if string starts with prefix
extern bool      Str_startsWith(StrConst self, StrConst prefix);
/// Check if string ends with suffix
extern bool      Str_endsWith(StrConst self, StrConst suffix);

/// Calculate string hash
extern StrHash Str_hash(StrConst self);

/*========== Comparison Operations ==========================================*/

/* Comparison (StrConst) */
extern cmp_fnCmp(StrConst);
cmp_fnEq_default(StrConst);
cmp_fnNe_default(StrConst);
cmp_fnLt_default(StrConst);
cmp_fnGt_default(StrConst);
cmp_fnLe_default(StrConst);
cmp_fnGe_default(StrConst);

/* Comparison */
extern cmp_fnCmp(Str);
cmp_fnEq_default(Str);
cmp_fnNe_default(Str);
cmp_fnLt_default(Str);
cmp_fnGt_default(Str);
cmp_fnLe_default(Str);
cmp_fnGe_default(Str);

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
extern usize   StrUtf8_len(StrConst self);
/// Get byte length of UTF-8 sequence starting at given position
extern u8      StrUtf8_sequenceLen(StrConst self, usize pos);
/// Check if string is valid UTF-8
extern bool    StrUtf8_isValid(StrConst self);
/// Get UTF-8 code point at position
extern Opt$u32 StrUtf8_codepointAt(StrConst self, usize pos);

/// Iterator for UTF-8 code points
typedef struct StrUtf8Iter {
    StrConst str;
    usize    pos;
} StrUtf8Iter;

/// Get iterator for UTF-8 code points
extern StrUtf8Iter StrUtf8_iter(StrConst self);
/// Iterate over UTF-8 code points
extern bool        StrUtf8Iter_next(StrUtf8Iter* iter, Opt$u32* out_codepoint);

/*========== String Tokenization ============================================*/

/// Tokenizer for string
typedef struct StrTokenizer {
    StrConst str;    // Source string
    StrConst delims; // Delimiter characters
    usize    pos;    // Current position
} StrTokenizer;
use_Opt$(StrTokenizer);

/// Create tokenizer for string with given delimiters
extern StrTokenizer Str_tokenizer(StrConst self, StrConst delims);
/// Get next token, returns none when no more tokens
extern Opt$StrConst StrTokenizer_next(StrTokenizer* self);

/*========== Implementations ================================================*/

#define SYN__Str_l(_literal_as_readonly...) \
    ((StrConst){ .ptr = (const u8*)"" _literal_as_readonly, .len = sizeof(_literal_as_readonly) - 1 })
#define SYN__Str_m(_literal_to_writable...) \
    ((Str){ .ptr = (u8[]){ "" _literal_to_writable }, .len = sizeof(_literal_to_writable) - 1 })

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* STR_INCLUDED */
