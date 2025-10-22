/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Sli$u8.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-31 (date of creation)
 * @updated 2025-02-11 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  Sli$u8
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
// typedef SliZ_const$u8 StrZ_const;
// /// Modifiable raw string (byte null terminated slice)
// typedef SliZ$u8       StrZ;
// /// Using variant type for raw string */
// use_Opt$(StrZ_const);
// use_Opt$(StrZ);
// use_Err$(StrZ_const);
// use_Err$(StrZ);

// /// Unmodifiable sentinel string (const byte sentinel terminated slice)
// typedef SliS_const$u8 StrS_const;
// /// Modifiable sentinel string (byte sentinel terminated slice)
// typedef SliS$u8       StrS;
// /// Using variant type for sentinel string */
// use_Opt$(StrS_const);
// use_Opt$(StrS);
// use_Err$(StrS_const);
// use_Err$(StrS);

// /// Unmodifiable string (const byte slice)
// typedef Sli_const$u8 Sli_const$u8;
// /// Modifiable string (byte slice)
// typedef Sli$u8       Sli$u8;
// /// Using variant type for string */
// use_Opt$(Sli_const$u8);
// use_Opt$(Sli$u8);
// use_Err$(Sli_const$u8);
// use_Err$(Sli$u8);

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

#define u8_c(_literal...)  comp_syn__u8_c(_literal)
#define u16_c(_literal...) comp_syn__u16_c(_literal)
#define u32_c(_literal...) comp_syn__u32_c(_literal)
#define u8_a(_literal...)  comp_syn__u8_a(_literal)
#define u8_s(_literal...)  comp_syn__u8_s(_literal)
#define u8_l(_literal...)  comp_syn__u8_l(_literal)
#define u8_az(_literal...) comp_syn__u8_az(_literal)
#define u8_sz(_literal...) comp_syn__u8_sz(_literal)
#define u8_lz(_literal...) comp_syn__u8_lz(_literal)
#define u8z_a(_literal...) comp_syn__u8z_a(_literal)
#define u8z_s(_literal...) comp_syn__u8z_s(_literal)
#define u8z_l(_literal...) comp_syn__u8z_l(_literal)

/// Create Sli_const$u8 from const u8* with explicit length
extern Sli_const$u8 Str_view(const u8* ptr, usize len);
extern Sli_const$u8 Str_viewZ(const u8* ptr);
/// Create Sli$u8 from u8* with explicit length
extern Sli$u8       Str_from(u8 ptr[], usize len);
extern Sli$u8       Str_fromZ(u8 ptr[]);

/*========== String Operations ==============================================*/

/// Get string length in bytes
extern usize      Str_len(Sli_const$u8 self);
/// Compare two strings
extern bool       Str_eql(Sli_const$u8 lhs, Sli_const$u8 rhs);
/// Compare strings ignoring case
extern bool       Str_eqlNoCase(Sli_const$u8 lhs, Sli_const$u8 rhs);
/// Check if string is modifiable
extern bool       Sli_const$u8Castable(Sli_const$u8 self);
/// Convert Sli_const$u8 to Sli$u8 (if source is modifiable)
extern Opt$Sli$u8 Sli_const$u8Cast(Sli_const$u8 self);

/// Concatenate strings
extern Err$Sli$u8   Str_cat(mem_Allocator allocator, Sli_const$u8 lhs, Sli_const$u8 rhs) $must_check;
/// Get substring
extern Sli_const$u8 Str_slice(Sli_const$u8 self, usize start, usize end);
/// Remove whitespace from both ends
extern Sli_const$u8 Str_trim(Sli_const$u8 self);
/// Remove whitespace from start
extern Sli_const$u8 Str_ltrim(Sli_const$u8 self);
/// Remove whitespace from end
extern Sli_const$u8 Str_rtrim(Sli_const$u8 self);
/// Convert to uppercase
extern Err$Sli$u8   Str_upper(mem_Allocator allocator, Sli_const$u8 str) $must_check;
/// Convert to lowercase
extern Err$Sli$u8   Str_lower(mem_Allocator allocator, Sli_const$u8 str) $must_check;

/// Check if string contains substring
extern bool      Str_contains(Sli_const$u8 haystack, Sli_const$u8 needle);
/// Find first occurrence of substring
extern Opt$usize Str_find(Sli_const$u8 haystack, Sli_const$u8 needle, usize start);
/// Find last occurrence of substring
extern Opt$usize Str_rfind(Sli_const$u8 haystack, Sli_const$u8 needle, usize start);
/// Check if string starts with prefix
/// Returns index where prefix ends if found, otherwise none
extern Opt$usize Str_startsWith(Sli_const$u8 self, Sli_const$u8 prefix);
/// Check if string ends with suffix
/// Returns index where suffix starts if found, otherwise none
extern Opt$usize Str_endsWith(Sli_const$u8 self, Sli_const$u8 suffix);

/// Calculate string hash
extern StrHash Str_hash(Sli_const$u8 self);

/*========== Comparison Operations ==========================================*/

/* Comparison (Sli_const$u8) */
extern cmp_fnCmp(Sli_const$u8);
cmp_fnEq_default(Sli_const$u8);
cmp_fnNe_default(Sli_const$u8);
cmp_fnLt_default(Sli_const$u8);
cmp_fnGt_default(Sli_const$u8);
cmp_fnLe_default(Sli_const$u8);
cmp_fnGe_default(Sli_const$u8);

/* Comparison */
extern cmp_fnCmp(Sli$u8);
cmp_fnEq_default(Sli$u8);
cmp_fnNe_default(Sli$u8);
cmp_fnLt_default(Sli$u8);
cmp_fnGt_default(Sli$u8);
cmp_fnLe_default(Sli$u8);
cmp_fnGe_default(Sli$u8);

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
extern usize   StrUtf8_len(Sli_const$u8 self);
/// Get byte length of UTF-8 sequence starting at given position
extern u8      StrUtf8_seqLen(Sli_const$u8 self, usize pos);
/// Check if string is valid UTF-8
extern bool    StrUtf8_isValid(Sli_const$u8 self);
/// Get UTF-8 code point at position
extern Opt$u32 StrUtf8_codepointAt(Sli_const$u8 self, usize pos);

/// Iterator for UTF-8 code points
typedef struct StrUtf8Iter {
    Sli_const$u8 str;
    usize        pos;
} StrUtf8Iter;

/// Get iterator for UTF-8 code points
extern StrUtf8Iter StrUtf8_iter(Sli_const$u8 self);
/// Iterate over UTF-8 code points
extern bool        StrUtf8Iter_next(StrUtf8Iter* iter, Opt$u32* out_codepoint);

/*========== String Tokenization ============================================*/

/// Tokenizer for string
typedef struct StrTokenizer {
    Sli_const$u8 str;    // Source string
    Sli_const$u8 delims; // Delimiter characters
    usize        pos;    // Current position
} StrTokenizer;
use_Opt$(StrTokenizer);

/// Create tokenizer for string with given delimiters
extern StrTokenizer     Str_tokenizer(Sli_const$u8 self, Sli_const$u8 delims);
/// Get next token, returns none when no more tokens
extern Opt$Sli_const$u8 StrTokenizer_next(StrTokenizer* self);

/*========== Implementations ================================================*/

#define comp_syn__StrZ_l(_literal_as_readonly...) \
    ((StrZ_const){ .ptr = as$((const u8*)("" _literal_as_readonly)) })
#define comp_syn__StrZ_m(_literal_to_writable...) \
    ((StrZ){ .ptr = lit$(u8[], "" _literal_to_writable) })

#define comp_syn__StrS_l(_literal_as_readonly...) \
    ((StrS_const){ .ptr = as$((const u8*)("" _literal_as_readonly)), .sentinel = '\0' })
#define comp_syn__StrS_m(_literal_to_writable...) \
    ((StrS){ .ptr = lit$(u8[], "" _literal_to_writable), .sentinel = '\0' })

#define comp_syn__Str_l(_literal_as_readonly...) \
    ((Sli_const$u8){ .ptr = as$((const u8*)("" _literal_as_readonly)), .len = sizeOf(_literal_as_readonly) - 1 })
#define comp_syn__Str_m(_literal_to_writable...) \
    ((Sli$u8){ .ptr = lit$(u8[], "" _literal_to_writable), .len = sizeOf(_literal_to_writable) - 1 })

#define comp_syn__u8_c(_literal...)  as$((u8)(_literal))
#define comp_syn__u16_c(_literal...) as$((u16)(u##_literal))
#define comp_syn__u32_c(_literal...) as$((u32)(U##_literal))

#define comp_syn__u8_a(_literal...) ((Arr$$(sizeOf(_literal) - 1, u8)){ .buf = { _literal } })
#define comp_syn__u8_s(_literal...) ((Sli$u8){ .ptr = lit$(u8[], "" _literal), .len = sizeOf(_literal) - 1 })
#define comp_syn__u8_l(_literal...) ((Sli_const$u8){ .ptr = as$((const u8*)("" _literal)), .len = sizeOf(_literal) - 1 })

#define comp_syn__u8_az(_literal...) ((Arr$$(sizeOf(_literal), u8)){ .buf = { _literal } })
#define comp_syn__u8_sz(_literal...) ((Sli$u8){ .ptr = lit$(u8[], "" _literal), .len = sizeOf(_literal) })
#define comp_syn__u8_lz(_literal...) ((Sli_const$u8){ .ptr = as$((const u8*)("" _literal)), .len = sizeOf(_literal) })

#define comp_syn__u8z_a(_literal...) ((ArrZ$$(sizeOf(_literal) - 1, u8)){ .buf = { _literal } })
#define comp_syn__u8z_s(_literal...) ((SliZ$u8){ .ptr = lit$(u8[], "" _literal), .len = sizeOf(_literal) - 1 })
#define comp_syn__u8z_l(_literal...) ((SliZ_const$u8){ .ptr = as$((const u8*)("" _literal)), .len = sizeOf(_literal) - 1 })

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* STR_INCLUDED */
