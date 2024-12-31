/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Str.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-31 (date of creation)
 * @updated 2024-12-31 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  Str
 *
 * @brief
 * @details
 */

#ifndef STR_INCLUDED
#define STR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "meta/common.h"

/*========== String Types ==================================================*/

typedef SliConst$u8 StrConst; // Unmodifiable string (const byte slice)
typedef Sli$u8      Str;      // Modifiable string (byte slice)
using_Opt$(StrConst);
using_Opt$(Str);
using_Err$(StrConst);
using_Err$(Str);

/*========== String Creation ==============================================*/

// For string literals (read-only)
#define strL(_lit) ((StrConst){ .ptr = (const u8*)"" _lit, .len = sizeof(_lit) - 1 })
// For modifiable strings (creates new array)
#define strM(_lit) ((Str){ .ptr = (u8[]){ "" _lit }, .len = sizeof(_lit) - 1 })
// Create StrConst from const u8* with explicit length
extern StrConst Str_const(const u8* ptr, usize len);
// Create Str from u8* with explicit length
extern Str      Str_from(u8 ptr[], usize len);

/*========== String Operations ===========================================*/

// Get string length in bytes
extern usize    Str_len(StrConst str);
// Compare two strings
extern bool     Str_eq(StrConst lhs, StrConst rhs);
// Check if string is modifiable
extern bool     Str_constCastable(StrConst str);
// Convert StrConst to Str (if source is modifiable)
extern Opt$Str  Str_mut(StrConst str);
// Get substring
extern StrConst Str_slice(StrConst str, usize start, usize end);
// Check if string contains substring
extern bool     Str_contains(StrConst haystack, StrConst needle);

/*========== UTF-8 Constants =============================================*/

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

/*========== UTF-8 Operations ==========================================*/

// Get number of UTF-8 code points in string
extern usize   StrUtf8_len(StrConst str);
// Get byte length of UTF-8 sequence starting at given position
extern u8      StrUtf8_sequenceLen(StrConst str, usize pos);
// Check if string is valid UTF-8
extern bool    StrUtf8_isValid(StrConst str);
// Get UTF-8 code point at position
extern Opt$u32 StrUtf8_codepointAt(StrConst str, usize pos);

// Iterator for UTF-8 code points
typedef struct StrUtf8Iter {
    StrConst str;
    usize    pos;
} StrUtf8Iter;

// Get iterator for UTF-8 code points
extern StrUtf8Iter StrUtf8_iter(StrConst str);
// Iterate over UTF-8 code points
extern bool        StrUtf8Iter_next(StrUtf8Iter* iter, Opt$u32* out_codepoint);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* STR_INCLUDED */
