#include "dh/core.h"
#include "dh/fn.h"
#include "dh/Arr.h"
#include "dh/sli.h"
#include "dh/err_res.h"
#include "dh/mem/Allocator.h"

// str - byte-slice operations as strings
/// Find first occurrence of substring
extern fn_((str_find(Sli_const$u8 haystack, Sli_const$u8 needle))(Opt$usize));
/// Find first occurrence of substring from the front
extern fn_((str_findFirst(Sli_const$u8 haystack, Sli_const$u8 needle, usize start_front))(Opt$usize));
/// Find last occurrence of substring from the back
extern fn_((str_findLast(Sli_const$u8 haystack, Sli_const$u8 needle, usize start_back))(Opt$usize));

/// Check if string starts with prefix
/// Returns index where prefix ends if found, otherwise none
extern fn_((str_startsWith(Sli_const$u8 bytes, Sli_const$u8 prefix))(Opt$usize));
/// Check if string ends with suffix
/// Returns index where suffix starts if found, otherwise none
extern fn_((str_endsWith(Sli_const$u8 bytes, Sli_const$u8 suffix))(Opt$usize));

extern fn_((str_split(Sli_const$u8 bytes, Sli_const$u8 delim))(Sli_const$Sli_const$u8));
extern fn_((str_join(mem_Allocator allocator, Sli_const$u8 sep, Sli$Sli_const$u8 parts))(Err$Sli$u8));
extern fn_((str_trim(Sli_const$u8 bytes, Sli_const$u8 pattern))(Sli_const$u8));
extern fn_((str_trimLeft(Sli_const$u8 bytes, Sli_const$u8 pattern))(Sli_const$u8));
extern fn_((str_trimRight(Sli_const$u8 bytes, Sli_const$u8 pattern))(Sli_const$u8));
extern fn_((str_replace(mem_Allocator allocator, Sli_const$u8 haystack, Sli_const$u8 needle, Sli_const$u8 replacement))(Err$Sli$u8));
extern fn_((str_contains(Sli_const$u8 haystack, Sli_const$u8 needle))(bool));

extern fn_((str_cmp(Sli_const$u8 lhs, Sli_const$u8 rhs, bool ignores_case))(cmp_Ord));
extern fn_((str_cmpSenseCase(Sli_const$u8 lhs, Sli_const$u8 rhs))(cmp_Ord));
extern fn_((str_cmpIgnoreCase(Sli_const$u8 lhs, Sli_const$u8 rhs))(cmp_Ord));

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
typedef Arr$$(4, u8) utf8_A4U8Buf;
/// Encode codepoint into UTF-8 sequence
extern fn_((utf8_encode(mem_Allocator allocator, u32 codepoint, utf8_A4U8Buf* buf))(Err$usize));
/// Decode UTF-8 sequence into codepoint
extern fn_((utf8_decode(Sli_const$u8 bytes))(Opt$u32));
/// Check if string is valid UTF-8
extern fn_((utf8_isValid(Sli_const$u8 bytes))(bool));

typedef struct utf8_View utf8_View;
typedef struct utf8_Iter utf8_Iter;

struct utf8_View {
    Sli_const$u8 bytes;
};
use_Err$(utf8_View);
/// Initialize UTF-8 view
extern fn_((utf8_View_init(Sli_const$u8 bytes))(Err$utf8_View));
/// Initialize UTF-8 view without checking if string is valid UTF-8
extern fn_((utf8_View_initUnchkd(Sli_const$u8 bytes))(utf8_View));
/// Get number of UTF-8 code points in string
extern fn_((utf8_View_len(utf8_View self))(usize));
/// Get byte length of UTF-8 sequence starting at given position
extern fn_((utf8_View_seqLen(utf8_View self, usize pos))(u8));
/// Get iterator over UTF-8 code points
extern fn_((utf8_View_iter(utf8_View self))(utf8_Iter));

struct utf8_Iter {
    Sli_const$u8 bytes;
    usize        pos;
};
extern fn_((utf8_Iter_next(utf8_Iter* self))(Opt$u32));
extern fn_((utf8_Iter_nextSlice(utf8_Iter* self))(Opt$Sli_const$u8));
extern fn_((utf8_Iter_nextCodepoint(utf8_Iter* self))(Opt$u32));
extern fn_((utf8_Iter_nextCodepointSlice(utf8_Iter* self))(Opt$Sli_const$u8));
