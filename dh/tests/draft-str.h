#include "dh/core.h"
#include "dh/fn.h"
#include "dh/Arr.h"
#include "dh/sli.h"
#include "dh/err_res.h"
#include "dh/mem/Allocator.h"

// str - byte-slice operations as strings
/// @brief Find first occurrence of substring in haystack
/// @param haystack The string to search in
/// @param needle The substring to search for
/// @return Optional index of first occurrence, none if not found
/// @example
///   Sli_const$u8 text = u8_l("Hello, World!");
///   Sli_const$u8 pattern = u8_l("World");
///   Opt$usize pos = str_find(text, pattern); // Returns some(7)
/// @dependencies sli.h, opt.h
/// @limitations Linear search algorithm O(n*m)
extern fn_(str_find(Sli_const$u8 haystack, Sli_const$u8 needle), Opt$usize);
/// @brief Find first occurrence of substring starting from specified position
/// @param haystack The string to search in
/// @param needle The substring to search for
/// @param start_front Starting position for search
/// @return Optional index of first occurrence from start_front, none if not found
/// @example
///   Sli_const$u8 text = u8_l("test test test");
///   Sli_const$u8 pattern = u8_l("test");
///   Opt$usize pos = str_findFirst(text, pattern, 5); // Returns some(10)
/// @dependencies sli.h, opt.h
/// @limitations start_front must be valid index within haystack
extern fn_(str_findFirst(Sli_const$u8 haystack, Sli_const$u8 needle, usize start_front), Opt$usize);
/// @brief Find last occurrence of substring searching backwards from specified position
/// @param haystack The string to search in
/// @param needle The substring to search for
/// @param start_back Starting position for backwards search
/// @return Optional index of last occurrence before start_back, none if not found
/// @example
///   Sli_const$u8 text = u8_l("test test test");
///   Sli_const$u8 pattern = u8_l("test");
///   Opt$usize pos = str_findLast(text, pattern, 10); // Returns some(5)
/// @dependencies sli.h, opt.h
/// @limitations start_back must be valid index within haystack
extern fn_(str_findLast(Sli_const$u8 haystack, Sli_const$u8 needle, usize start_back), Opt$usize);
/// @brief Check if string starts with specified prefix
/// @param bytes The string to check
/// @param prefix The prefix to look for
/// @return Optional index where prefix ends if found, none otherwise
/// @example
///   Sli_const$u8 text = u8_l("Hello, World!");
///   Sli_const$u8 prefix = u8_l("Hello");
///   Opt$usize end = str_startsWith(text, prefix); // Returns some(5)
/// @dependencies sli.h, opt.h
/// @limitations None
extern fn_(str_startsWith(Sli_const$u8 bytes, Sli_const$u8 prefix), Opt$usize);
/// @brief Check if string ends with specified suffix
/// @param bytes The string to check
/// @param suffix The suffix to look for
/// @return Optional index where suffix starts if found, none otherwise
/// @example
///   Sli_const$u8 text = u8_l("Hello, World!");
///   Sli_const$u8 suffix = u8_l("World!");
///   Opt$usize start = str_endsWith(text, suffix); // Returns some(7)
/// @dependencies sli.h, opt.h
/// @limitations None
extern fn_(str_endsWith(Sli_const$u8 bytes, Sli_const$u8 suffix), Opt$usize);
/// @brief Split string by delimiter string
/// @param bytes The string to split
/// @param delim The delimiter string to split by
/// @return Slice of string slices representing the split parts
/// @example
///   Sli_const$u8 text = u8_l("apple,banana,cherry");
///   Sli_const$u8 delim = u8_l(",");
///   Sli_const$Sli_const$u8 parts = str_split(text, delim);
/// @dependencies sli.h
/// @limitations Delimiter can be multi-character string
extern fn_(str_split(Sli_const$u8 bytes, Sli_const$u8 delim), Sli_const$Sli_const$u8);
/// @brief Join string parts with separator
/// @param allocator Memory allocator for result
/// @param sep Separator string to insert between parts
/// @param parts Array of string parts to join
/// @return Result containing joined string or error
/// @example
///   mem_Allocator alloc = heap_getDefaultAllocator();
///   Sli_const$u8 sep = u8_l(", ");
///   Sli$Sli_const$u8 parts = /* array of strings */;
///   Err$Sli$u8 result = str_join(alloc, sep, parts);
/// @dependencies mem.h, err_res.h, sli.h
/// @limitations May fail if memory allocation fails
extern fn_(str_join(mem_Allocator allocator, Sli_const$u8 sep, Sli$Sli_const$u8 parts), Err$Sli$u8);
/// @brief Remove pattern from both ends of string
/// @param bytes The string to trim
/// @param pattern The pattern to remove from both ends
/// @return Trimmed string slice
/// @example
///   Sli_const$u8 text = u8_l("  hello  ");
///   Sli_const$u8 pattern = u8_l(" ");
///   Sli_const$u8 trimmed = str_trim(text, pattern); // "hello"
/// @dependencies sli.h
/// @limitations Pattern can be multi-character string
extern fn_(str_trim(Sli_const$u8 bytes, Sli_const$u8 pattern), Sli_const$u8);
/// @brief Remove pattern from left end of string
/// @param bytes The string to trim
/// @param pattern The pattern to remove from left end
/// @return Left-trimmed string slice
/// @example
///   Sli_const$u8 text = u8_l("  hello  ");
///   Sli_const$u8 pattern = u8_l(" ");
///   Sli_const$u8 trimmed = str_trimLeft(text, pattern); // "hello  "
/// @dependencies sli.h
/// @limitations Pattern can be multi-character string
extern fn_(str_trimLeft(Sli_const$u8 bytes, Sli_const$u8 pattern), Sli_const$u8);
/// @brief Remove pattern from right end of string
/// @param bytes The string to trim
/// @param pattern The pattern to remove from right end
/// @return Right-trimmed string slice
/// @example
///   Sli_const$u8 text = u8_l("  hello  ");
///   Sli_const$u8 pattern = u8_l(" ");
///   Sli_const$u8 trimmed = str_trimRight(text, pattern); // "  hello"
/// @dependencies sli.h
/// @limitations Pattern can be multi-character string
extern fn_(str_trimRight(Sli_const$u8 bytes, Sli_const$u8 pattern), Sli_const$u8);
/// @brief Replace all occurrences of needle with replacement
/// @param allocator Memory allocator for result
/// @param haystack The string to perform replacements on
/// @param needle The substring to replace
/// @param replacement The replacement string
/// @return Result containing new string with replacements or error
/// @example
///   mem_Allocator alloc = heap_getDefaultAllocator();
///   Sli_const$u8 text = u8_l("Hello World");
///   Sli_const$u8 needle = u8_l("World");
///   Sli_const$u8 replacement = u8_l("Universe");
///   Err$Sli$u8 result = str_replace(alloc, text, needle, replacement);
/// @dependencies mem.h, err_res.h, sli.h
/// @limitations May fail if memory allocation fails
extern fn_(str_replace(mem_Allocator allocator, Sli_const$u8 haystack, Sli_const$u8 needle, Sli_const$u8 replacement), Err$Sli$u8);
/// @brief Check if haystack contains needle
/// @param haystack The string to search in
/// @param needle The substring to search for
/// @return true if needle is found in haystack, false otherwise
/// @example
///   Sli_const$u8 text = u8_l("Hello, World!");
///   Sli_const$u8 pattern = u8_l("World");
///   bool found = str_contains(text, pattern); // Returns true
/// @dependencies sli.h
/// @limitations Linear search algorithm O(n*m)
extern fn_(str_contains(Sli_const$u8 haystack, Sli_const$u8 needle), bool);
/// @brief Compare two strings with optional case sensitivity
/// @param lhs Left-hand side string
/// @param rhs Right-hand side string
/// @param ignores_case Whether to ignore case during comparison
/// @return Comparison result (Less, Equal, Greater)
/// @example
///   Sli_const$u8 str1 = u8_l("Hello");
///   Sli_const$u8 str2 = u8_l("hello");
///   cmp_Ord result = str_cmp(str1, str2, true); // Equal (ignoring case)
/// @dependencies core/cmp.h, sli.h
/// @limitations ASCII-only case insensitive comparison
extern fn_(str_cmp(Sli_const$u8 lhs, Sli_const$u8 rhs, bool ignores_case), cmp_Ord);
/// @brief Compare two strings with case sensitivity
/// @param lhs Left-hand side string
/// @param rhs Right-hand side string
/// @return Comparison result (Less, Equal, Greater)
/// @example
///   Sli_const$u8 str1 = u8_l("Hello");
///   Sli_const$u8 str2 = u8_l("World");
///   cmp_Ord result = str_cmpSenseCase(str1, str2); // Less
/// @dependencies core/cmp.h, sli.h
/// @limitations None
extern fn_(str_cmpSenseCase(Sli_const$u8 lhs, Sli_const$u8 rhs), cmp_Ord);
/// @brief Compare two strings ignoring case
/// @param lhs Left-hand side string
/// @param rhs Right-hand side string
/// @return Comparison result (Less, Equal, Greater)
/// @example
///   Sli_const$u8 str1 = u8_l("Hello");
///   Sli_const$u8 str2 = u8_l("HELLO");
///   cmp_Ord result = str_cmpIgnoreCase(str1, str2); // Equal
/// @dependencies core/cmp.h, sli.h
/// @limitations ASCII-only case insensitive comparison
extern fn_(str_cmpIgnoreCase(Sli_const$u8 lhs, Sli_const$u8 rhs), cmp_Ord);

// ascii - ASCII character utilities
/// @brief Check if character is alphanumeric (letter or digit)
/// @param c The character to check
/// @return true if character is alphanumeric, false otherwise
/// @example
///   bool result = ascii_isAlNum('a'); // true
///   bool result2 = ascii_isAlNum('5'); // true
///   bool result3 = ascii_isAlNum('!'); // false
/// @dependencies None
/// @limitations ASCII characters only (0-127)
extern fn_(ascii_isAlNum(u8 c), bool);
/// @brief Check if character is alphabetic (letter)
/// @param c The character to check
/// @return true if character is alphabetic, false otherwise
/// @example
///   bool result = ascii_isAlpha('a'); // true
///   bool result2 = ascii_isAlpha('Z'); // true
///   bool result3 = ascii_isAlpha('5'); // false
/// @dependencies None
/// @limitations ASCII characters only (0-127)
extern fn_(ascii_isAlpha(u8 c), bool);
/// @brief Check if character is a digit
/// @param c The character to check
/// @return true if character is a digit, false otherwise
/// @example
///   bool result = ascii_isDigit('5'); // true
///   bool result2 = ascii_isDigit('a'); // false
/// @dependencies None
/// @limitations ASCII characters only (0-127)
extern fn_(ascii_isDigit(u8 c), bool);
/// @brief Check if character is lowercase letter
/// @param c The character to check
/// @return true if character is lowercase letter, false otherwise
/// @example
///   bool result = ascii_isLower('a'); // true
///   bool result2 = ascii_isLower('A'); // false
/// @dependencies None
/// @limitations ASCII characters only (0-127)
extern fn_(ascii_isLower(u8 c), bool);
/// @brief Check if character is uppercase letter
/// @param c The character to check
/// @return true if character is uppercase letter, false otherwise
/// @example
///   bool result = ascii_isUpper('A'); // true
///   bool result2 = ascii_isUpper('a'); // false
/// @dependencies None
/// @limitations ASCII characters only (0-127)
extern fn_(ascii_isUpper(u8 c), bool);
/// @brief Convert character to lowercase
/// @param c The character to convert
/// @return Lowercase version of character, unchanged if not uppercase letter
/// @example
///   u8 result = ascii_toLower('A'); // 'a'
///   u8 result2 = ascii_toLower('5'); // '5' (unchanged)
/// @dependencies None
/// @limitations ASCII characters only (0-127)
extern fn_(ascii_toLower(u8 c), u8);
/// @brief Convert character to uppercase
/// @param c The character to convert
/// @return Uppercase version of character, unchanged if not lowercase letter
/// @example
///   u8 result = ascii_toUpper('a'); // 'A'
///   u8 result2 = ascii_toUpper('5'); // '5' (unchanged)
/// @dependencies None
/// @limitations ASCII characters only (0-127)
extern fn_(ascii_toUpper(u8 c), u8);
/// @brief Compare two ASCII characters with optional case sensitivity
/// @param lhs Left-hand side character
/// @param rhs Right-hand side character
/// @param ignores_case Whether to ignore case during comparison
/// @return Comparison result (Less, Equal, Greater)
/// @example
///   cmp_Ord result = ascii_cmp('a', 'A', true); // Equal (ignoring case)
///   cmp_Ord result2 = ascii_cmp('a', 'b', false); // Less
/// @dependencies core/cmp.h
/// @limitations ASCII characters only (0-127)
extern fn_(ascii_cmp(u8 lhs, u8 rhs, bool ignores_case), cmp_Ord);
/// @brief Compare two ASCII characters with case sensitivity
/// @param lhs Left-hand side character
/// @param rhs Right-hand side character
/// @return Comparison result (Less, Equal, Greater)
/// @example
///   cmp_Ord result = ascii_cmpSenseCase('a', 'A'); // Greater
///   cmp_Ord result2 = ascii_cmpSenseCase('a', 'a'); // Equal
/// @dependencies core/cmp.h
/// @limitations ASCII characters only (0-127)
extern fn_(ascii_cmpSenseCase(u8 lhs, u8 rhs), cmp_Ord);
/// @brief Compare two ASCII characters ignoring case
/// @param lhs Left-hand side character
/// @param rhs Right-hand side character
/// @return Comparison result (Less, Equal, Greater)
/// @example
///   cmp_Ord result = ascii_cmpIgnoreCase('a', 'A'); // Equal
///   cmp_Ord result2 = ascii_cmpIgnoreCase('a', 'b'); // Less
/// @dependencies core/cmp.h
/// @limitations ASCII characters only (0-127)
extern fn_(ascii_cmpIgnoreCase(u8 lhs, u8 rhs), cmp_Ord);

// utf8 - UTF‑8, code points utilities
typedef Arr$$(4, u8) utf8_A4U8Buf;
/// @brief Encode codepoint into UTF-8 sequence
/// @param allocator Memory allocator (currently unused, for future compatibility)
/// @param codepoint Unicode codepoint to encode
/// @param buf Buffer to store UTF-8 bytes (max 4 bytes)
/// @return Result containing number of bytes written or error
/// @example
///   utf8_A4U8Buf buffer;
///   mem_Allocator alloc = heap_getDefaultAllocator();
///   Err$usize result = utf8_encode(alloc, 0x1F600, &buffer); // 😀 emoji
/// @dependencies mem.h, err_res.h, Arr.h
/// @limitations Codepoint must be valid Unicode (0-0x10FFFF)
extern fn_(utf8_encode(mem_Allocator allocator, u32 codepoint, utf8_A4U8Buf* buf), Err$usize);
/// @brief Decode UTF-8 sequence into codepoint
/// @param bytes UTF-8 byte sequence to decode
/// @return Optional codepoint if valid UTF-8, none if invalid
/// @example
///   Sli_const$u8 utf8_bytes = u8_l("😀");
///   Opt$u32 codepoint = utf8_decode(utf8_bytes); // Returns some(0x1F600)
/// @dependencies sli.h, opt.h
/// @limitations Only decodes first complete UTF-8 sequence in bytes
extern fn_(utf8_decode(Sli_const$u8 bytes), Opt$u32);
/// @brief Check if string is valid UTF-8
/// @param bytes Byte sequence to validate
/// @return true if bytes form valid UTF-8 sequence, false otherwise
/// @example
///   Sli_const$u8 valid_utf8 = u8_l("Hello 世界");
///   Sli_const$u8 invalid_utf8 = sli_from_bytes((u8[]){0xFF, 0xFE}, 2);
///   bool is_valid1 = utf8_isValid(valid_utf8); // true
///   bool is_valid2 = utf8_isValid(invalid_utf8); // false
/// @dependencies sli.h
/// @limitations None
extern fn_(utf8_isValid(Sli_const$u8 bytes), bool);

typedef struct utf8_View utf8_View;
typedef struct utf8_Iter utf8_Iter;

struct utf8_View {
    Sli_const$u8 bytes;
};
use_Err$(utf8_View);
/// @brief Initialize UTF-8 view with validation
/// @param bytes Byte sequence to create view from
/// @return Result containing UTF-8 view or error if invalid UTF-8
/// @example
///   Sli_const$u8 text = u8_l("Hello 世界");
///   Err$utf8_View view = utf8_View_init(text);
/// @dependencies sli.h, err_res.h
/// @limitations Validates entire string during initialization
extern fn_(utf8_View_init(Sli_const$u8 bytes), Err$utf8_View);
/// @brief Initialize UTF-8 view without validation
/// @param bytes Byte sequence to create view from (assumed valid UTF-8)
/// @return UTF-8 view
/// @example
///   Sli_const$u8 text = u8_l("Hello World"); // ASCII is valid UTF-8
///   utf8_View view = utf8_View_initUnchkd(text);
/// @dependencies sli.h
/// @limitations Caller must ensure bytes are valid UTF-8
extern fn_(utf8_View_initUnchkd(Sli_const$u8 bytes), utf8_View);
/// @brief Get number of UTF-8 code points in view
/// @param self UTF-8 view
/// @return Number of Unicode code points (not bytes)
/// @example
///   utf8_View view = utf8_View_initUnchkd(u8_l("Hello 世界"));
///   usize len = utf8_View_len(view); // Returns 8 (not 11 bytes)
/// @dependencies None
/// @limitations Iterates through entire string to count code points
extern fn_(utf8_View_len(utf8_View self), usize);
/// @brief Get UTF-8 code point at position
/// @param self UTF-8 view
/// @param pos Byte position in the view
/// @return Optional UTF-8 code point, none if out of bounds
/// @example
///   utf8_View view = utf8_View_initUnchkd(u8_l("Hello 世界"));
///   Opt$u32 cp = utf8_View_codepointAt(view, 6); // Returns some(0x4E2D)
extern fn_(utf8_View_codepointAt(utf8_View self, usize pos), Opt$u32);
/// @brief Get byte length of UTF-8 sequence starting at given position
/// @param self UTF-8 view
/// @param pos Byte position in the view
/// @return Number of bytes in UTF-8 sequence (1-4)
/// @example
///   utf8_View view = utf8_View_initUnchkd(u8_l("Hello 世界"));
///   u8 len1 = utf8_View_seqLen(view, 0); // 1 (for 'H')
///   u8 len2 = utf8_View_seqLen(view, 6); // 3 (for '世')
/// @dependencies None
/// @limitations pos must be at start of valid UTF-8 sequence
extern fn_(utf8_View_seqLen(utf8_View self, usize pos), u8);
/// @brief Get iterator over UTF-8 code points
/// @param self UTF-8 view to iterate over
/// @return UTF-8 iterator positioned at start
/// @example
///   utf8_View view = utf8_View_initUnchkd(u8_l("Hello"));
///   utf8_Iter iter = utf8_View_iter(view);
/// @dependencies None
/// @limitations None
extern fn_(utf8_View_iter(utf8_View self), utf8_Iter);

struct utf8_Iter {
    Sli_const$u8 bytes;
    usize        pos;
};
/// @brief Get next codepoint from iterator
/// @param self Pointer to UTF-8 iterator (modified)
/// @return Optional next codepoint, none if at end
/// @example
///   utf8_Iter iter = utf8_View_iter(view);
///   Opt$u32 cp = utf8_Iter_next(&iter);
/// @dependencies opt.h
/// @limitations Advances iterator position
extern fn_(utf8_Iter_next(utf8_Iter* self), Opt$u32);
/// @brief Get next UTF-8 sequence as byte slice from iterator
/// @param self Pointer to UTF-8 iterator (modified)
/// @return Optional slice of bytes for next UTF-8 sequence, none if at end
/// @example
///   utf8_Iter iter = utf8_View_iter(view);
///   Opt$Sli_const$u8 slice = utf8_Iter_nextSlice(&iter);
/// @dependencies opt.h, sli.h
/// @limitations Advances iterator position
extern fn_(utf8_Iter_nextSlice(utf8_Iter* self), Opt$Sli_const$u8);
/// @brief Get next codepoint from iterator (alias for utf8_Iter_next)
/// @param self Pointer to UTF-8 iterator (modified)
/// @return Optional next codepoint, none if at end
/// @example
///   utf8_Iter iter = utf8_View_iter(view);
///   Opt$u32 cp = utf8_Iter_nextCodepoint(&iter);
/// @dependencies opt.h
/// @limitations Advances iterator position
extern fn_(utf8_Iter_nextCodepoint(utf8_Iter* self), Opt$u32);
/// @brief Get next UTF-8 sequence as byte slice from iterator (alias for utf8_Iter_nextSlice)
/// @param self Pointer to UTF-8 iterator (modified)
/// @return Optional slice of bytes for next UTF-8 sequence, none if at end
/// @example
///   utf8_Iter iter = utf8_View_iter(view);
///   Opt$Sli_const$u8 slice = utf8_Iter_nextCodepointSlice(&iter);
/// @dependencies opt.h, sli.h
/// @limitations Advances iterator position
extern fn_(utf8_Iter_nextCodepointSlice(utf8_Iter* self), Opt$Sli_const$u8);
