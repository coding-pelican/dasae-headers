#include "dh/core.h"
#include "dh/fn.h"
#include "dh/Arr.h"
#include "dh/sli.h"
#include "dh/err_res.h"
#include "dh/mem/Allocator.h"

// str - byte-slice operations as strings
/// Find first occurrence of substring
extern fn_((str_find(S_const$u8 haystack, S_const$u8 needle))(O$usize));
/// Find first occurrence of substring from the front
extern fn_((str_findFirst(S_const$u8 haystack, S_const$u8 needle, usize start_front))(O$usize));
/// Find last occurrence of substring from the back
extern fn_((str_findLast(S_const$u8 haystack, S_const$u8 needle, usize start_back))(O$usize));

/// Check if string starts with prefix
/// Returns index where prefix ends if found, otherwise none
extern fn_((str_startsWith(S_const$u8 bytes, S_const$u8 prefix))(O$usize));
/// Check if string ends with suffix
/// Returns index where suffix starts if found, otherwise none
extern fn_((str_endsWith(S_const$u8 bytes, S_const$u8 suffix))(O$usize));

extern fn_((str_split(S_const$u8 bytes, S_const$u8 delim))(S_const$S_const$u8));
extern fn_((str_join(mem_Allocator allocator, S_const$u8 sep, S$S_const$u8 parts))(E$S$u8));
extern fn_((str_trim(S_const$u8 bytes, S_const$u8 pattern))(S_const$u8));
extern fn_((str_trimLeft(S_const$u8 bytes, S_const$u8 pattern))(S_const$u8));
extern fn_((str_trimRight(S_const$u8 bytes, S_const$u8 pattern))(S_const$u8));
extern fn_((str_replace(mem_Allocator allocator, S_const$u8 haystack, S_const$u8 needle, S_const$u8 replacement))(E$S$u8));
extern fn_((str_contains(S_const$u8 haystack, S_const$u8 needle))(bool));

extern fn_((str_cmp(S_const$u8 lhs, S_const$u8 rhs, bool ignores_case))(cmp_Ord));
extern fn_((str_cmpSenseCase(S_const$u8 lhs, S_const$u8 rhs))(cmp_Ord));
extern fn_((str_cmpIgnoreCase(S_const$u8 lhs, S_const$u8 rhs))(cmp_Ord));

// ascii - ASCII character utilities
extern fn_((ascii_isAlNum(u8 c))(bool));
extern fn_((ascii_isAlpha(u8 c))(bool));
extern fn_((ascii_isDigit(u8 c))(bool));
extern fn_((ascii_isLower(u8 c))(bool));
extern fn_((ascii_isUpper(u8 c))(bool));
extern fn_((ascii_toLower(u8 c))(u8));
extern fn_((ascii_toUpper(u8 c))(u8));

extern fn_((ascii_cmp(u8 lhs, u8 rhs, bool ignores_case))(cmp_Ord));
extern fn_((ascii_cmpSenseCase(u8 lhs, u8 rhs))(cmp_Ord));
extern fn_((ascii_cmpIgnoreCase(u8 lhs, u8 rhs))(cmp_Ord));

// utf8 - UTF‑8, code points utilities
typedef A$$(4, u8) utf8_A4U8Buf;
/// Encode codepoint into UTF-8 sequence
extern fn_((utf8_encode(mem_Allocator allocator, u32 codepoint, utf8_A4U8Buf* buf))(E$usize));
/// Decode UTF-8 sequence into codepoint
extern fn_((utf8_decode(S_const$u8 bytes))(O$u32));
/// Check if string is valid UTF-8
extern fn_((utf8_isValid(S_const$u8 bytes))(bool));

typedef struct utf8_View utf8_View;
typedef struct utf8_Iter utf8_Iter;

struct utf8_View {
    S_const$u8 bytes;
};
use_E$(utf8_View);
/// Initialize UTF-8 view
extern fn_((utf8_View_init(S_const$u8 bytes))(E$utf8_View));
/// Initialize UTF-8 view without checking if string is valid UTF-8
extern fn_((utf8_View_initUnchkd(S_const$u8 bytes))(utf8_View));
/// Get number of UTF-8 code points in string
extern fn_((utf8_View_len(utf8_View self))(usize));
/// Get byte length of UTF-8 sequence starting at given position
extern fn_((utf8_View_seqLen(utf8_View self, usize pos))(u8));
/// Get iterator over UTF-8 code points
extern fn_((utf8_View_iter(utf8_View self))(utf8_Iter));

struct utf8_Iter {
    S_const$u8 bytes;
    usize      pos;
};
extern fn_((utf8_Iter_next(utf8_Iter* self))(O$u32));
extern fn_((utf8_Iter_nextSlice(utf8_Iter* self))(O$S_const$u8));
extern fn_((utf8_Iter_nextCodepoint(utf8_Iter* self))(O$u32));
extern fn_((utf8_Iter_nextCodepointSlice(utf8_Iter* self))(O$S_const$u8));
