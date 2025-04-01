/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
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

#include "sli.h"
#include "mem/Allocator.h"

/*========== String Types ===================================================*/

/// Unmodifiable raw string (const byte null terminated slice)
typedef SliZ_const$u8 StrZ_const;
/// Modifiable raw string (byte null terminated slice)
typedef SliZ$u8       StrZ;
/// Using variant type for raw string */
use_Opt$(StrZ_const);
use_Opt$(StrZ);
use_Err$(StrZ_const);
use_Err$(StrZ);

/// Unmodifiable sentinel string (const byte sentinel terminated slice)
typedef SliS_const$u8 StrS_const;
/// Modifiable sentinel string (byte sentinel terminated slice)
typedef SliS$u8       StrS;
/// Using variant type for sentinel string */
use_Opt$(StrS_const);
use_Opt$(StrS);
use_Err$(StrS_const);
use_Err$(StrS);

/// Unmodifiable string (const byte slice)
typedef Sli_const$u8 Str_const;
/// Modifiable string (byte slice)
typedef Sli$u8       Str;
/// Using variant type for string */
use_Opt$(Str_const);
use_Opt$(Str);
use_Err$(Str_const);
use_Err$(Str);

/// String hash value type
typedef u32 StrHash;
use_Opt$(StrHash);

/*========== String Creation ================================================*/

#define StrZ_l(_literal_as_readonly...) /* For read-only raw string literals */ comp_syn__StrZ_l(_literal_as_readonly)
#define StrZ_m(_literal_to_writable...) /* For writable raw string literals */ comp_syn__StrZ_m(_literal_to_writable)

#define StrS_l(_literal_as_readonly...) /* For read-only sentinel string literals */ comp_syn__StrS_l(_literal_as_readonly)
#define StrS_m(_literal_to_writable...) /* For writable sentinel string literals */ comp_syn__StrS_m(_literal_to_writable)

#define Str_l(_literal_as_readonly...) /* For read-only string literals */ comp_syn__Str_l(_literal_as_readonly)
#define Str_m(_literal_to_writable...) /* For writable string literals */ comp_syn__Str_m(_literal_to_writable)

/// Create Str_const from const u8* with explicit length
extern Str_const Str_view(const u8* ptr, usize len);
extern Str_const Str_viewZ(const u8* ptr);
/// Create Str from u8* with explicit length
extern Str       Str_from(u8 ptr[], usize len);
extern Str       Str_fromZ(u8 ptr[]);

/*========== String Operations ==============================================*/

/// Get string length in bytes
extern usize   Str_len(Str_const self);
/// Compare two strings
extern bool    Str_eql(Str_const lhs, Str_const rhs);
/// Compare strings ignoring case
extern bool    Str_eqlNoCase(Str_const lhs, Str_const rhs);
/// Check if string is modifiable
extern bool    Str_constCastable(Str_const self);
/// Convert Str_const to Str (if source is modifiable)
extern Opt$Str Str_constCast(Str_const self);

/// Concatenate strings
extern Err$Str   Str_cat(mem_Allocator allocator, Str_const lhs, Str_const rhs) $must_check;
/// Print string
extern void      Str_print(Str_const self);
/// Print string with newline
extern void      Str_println(Str_const self);
/// Format string with arguments
extern Err$Str   Str_format(mem_Allocator allocator, const char* format, ...) $must_check;
/// Get substring
extern Str_const Str_slice(Str_const self, usize start, usize end);
/// Remove whitespace from both ends
extern Str_const Str_trim(Str_const self);
/// Remove whitespace from start
extern Str_const Str_ltrim(Str_const self);
/// Remove whitespace from end
extern Str_const Str_rtrim(Str_const self);
/// Convert to uppercase
extern Err$Str   Str_upper(mem_Allocator allocator, Str_const str) $must_check;
/// Convert to lowercase
extern Err$Str   Str_lower(mem_Allocator allocator, Str_const str) $must_check;

/// Check if string contains substring
extern bool      Str_contains(Str_const haystack, Str_const needle);
/// Find first occurrence of substring
extern Opt$usize Str_find(Str_const haystack, Str_const needle, usize start);
/// Find last occurrence of substring
extern Opt$usize Str_rfind(Str_const haystack, Str_const needle, usize start);
/// Check if string starts with prefix
extern bool      Str_startsWith(Str_const self, Str_const prefix);
/// Check if string ends with suffix
extern bool      Str_endsWith(Str_const self, Str_const suffix);

/// Calculate string hash
extern StrHash Str_hash(Str_const self);

/*========== Comparison Operations ==========================================*/

/* Comparison (Str_const) */
extern cmp_fnCmp(Str_const);
cmp_fnEq_default(Str_const);
cmp_fnNe_default(Str_const);
cmp_fnLt_default(Str_const);
cmp_fnGt_default(Str_const);
cmp_fnLe_default(Str_const);
cmp_fnGe_default(Str_const);

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
extern usize   StrUtf8_len(Str_const self);
/// Get byte length of UTF-8 sequence starting at given position
extern u8      StrUtf8_seqLen(Str_const self, usize pos);
/// Check if string is valid UTF-8
extern bool    StrUtf8_isValid(Str_const self);
/// Get UTF-8 code point at position
extern Opt$u32 StrUtf8_codepointAt(Str_const self, usize pos);

/// Iterator for UTF-8 code points
typedef struct StrUtf8Iter {
    Str_const str;
    usize     pos;
} StrUtf8Iter;

/// Get iterator for UTF-8 code points
extern StrUtf8Iter StrUtf8_iter(Str_const self);
/// Iterate over UTF-8 code points
extern bool        StrUtf8Iter_next(StrUtf8Iter* iter, Opt$u32* out_codepoint);

/*========== String Tokenization ============================================*/

/// Tokenizer for string
typedef struct StrTokenizer {
    Str_const str;    // Source string
    Str_const delims; // Delimiter characters
    usize     pos;    // Current position
} StrTokenizer;
use_Opt$(StrTokenizer);

/// Create tokenizer for string with given delimiters
extern StrTokenizer  Str_tokenizer(Str_const self, Str_const delims);
/// Get next token, returns none when no more tokens
extern Opt$Str_const StrTokenizer_next(StrTokenizer* self);

/*========== Implementations ================================================*/

#define comp_syn__StrZ_l(_literal_as_readonly...) \
    ((StrZ_const){ .ptr = as$(const u8*, "" _literal_as_readonly) })
#define comp_syn__StrZ_m(_literal_to_writable...) \
    ((StrZ){ .ptr = lit$(u8[], "" _literal_to_writable) })

#define comp_syn__StrS_l(_literal_as_readonly...) \
    ((StrS_const){ .ptr = as$(const u8*, "" _literal_as_readonly), .sentinel = '\0' })
#define comp_syn__StrS_m(_literal_to_writable...) \
    ((StrS){ .ptr = lit$(u8[], "" _literal_to_writable), .sentinel = '\0' })

#define comp_syn__Str_l(_literal_as_readonly...) \
    ((Str_const){ .ptr = as$(const u8*, "" _literal_as_readonly), .len = sizeOf(_literal_as_readonly) - 1 })
#define comp_syn__Str_m(_literal_to_writable...) \
    ((Str){ .ptr = lit$(u8[], "" _literal_to_writable), .len = sizeOf(_literal_to_writable) - 1 })

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* STR_INCLUDED */
