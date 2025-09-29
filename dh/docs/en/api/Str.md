---
title: String Processing (Str)
description: Documentation for the Str module that provides string, ASCII, and UTF-8 utilities
created: 2025-01-27
updated: 2025-01-27
version: v0.1
target_version: pre-alpha
---

# String Processing (Str)

## Overview

The `Str` module provides comprehensive string processing utilities for byte slices, ASCII character operations, and UTF-8 text handling. This module offers safe and efficient text manipulation functions with proper Unicode support.

## Features

- Substring search and manipulation
- ASCII character classification and conversion
- UTF-8 encoding/decoding and iteration
- Case-sensitive and case-insensitive string comparison
- String trimming, splitting, and joining operations
- Memory-safe string operations with proper error handling

## Modules

The string API is organized into three main categories:

1. **str_*** - General string operations on byte slices
2. **ascii_*** - ASCII character utilities
3. **utf8_*** - UTF-8 encoding and Unicode operations

---

## String Operations (str_*)

### Search Functions

#### str_find

```c
/// @brief Find first occurrence of substring in haystack
/// @param haystack The string to search in
/// @param needle The substring to search for
/// @return Optional index of first occurrence, none if not found
/// @example
///   Sli_const$u8 text = sli_from_cstr("Hello, World!");
///   Sli_const$u8 pattern = sli_from_cstr("World");
///   Opt$usize pos = str_find(text, pattern); // Returns some(7)
/// @dependencies sli.h, opt.h
/// @limitations Linear search algorithm O(n*m)
extern fn_(str_find(Sli_const$u8 haystack, Sli_const$u8 needle), Opt$usize);
```

#### str_findFirst

```c
/// @brief Find first occurrence of substring starting from specified position
/// @param haystack The string to search in
/// @param needle The substring to search for
/// @param start_front Starting position for search
/// @return Optional index of first occurrence from start_front, none if not found
/// @example
///   Sli_const$u8 text = sli_from_cstr("test test test");
///   Sli_const$u8 pattern = sli_from_cstr("test");
///   Opt$usize pos = str_findFirst(text, pattern, 5); // Returns some(10)
/// @dependencies sli.h, opt.h
/// @limitations start_front must be valid index within haystack
extern fn_(str_findFirst(Sli_const$u8 haystack, Sli_const$u8 needle, usize start_front), Opt$usize);
```

#### str_findLast

```c
/// @brief Find last occurrence of substring searching backwards from specified position
/// @param haystack The string to search in
/// @param needle The substring to search for
/// @param start_back Starting position for backwards search
/// @return Optional index of last occurrence before start_back, none if not found
/// @example
///   Sli_const$u8 text = sli_from_cstr("test test test");
///   Sli_const$u8 pattern = sli_from_cstr("test");
///   Opt$usize pos = str_findLast(text, pattern, 10); // Returns some(5)
/// @dependencies sli.h, opt.h
/// @limitations start_back must be valid index within haystack
extern fn_(str_findLast(Sli_const$u8 haystack, Sli_const$u8 needle, usize start_back), Opt$usize);
```

#### str_contains

```c
/// @brief Check if haystack contains needle
/// @param haystack The string to search in
/// @param needle The substring to search for
/// @return true if needle is found in haystack, false otherwise
/// @example
///   Sli_const$u8 text = sli_from_cstr("Hello, World!");
///   Sli_const$u8 pattern = sli_from_cstr("World");
///   bool found = str_contains(text, pattern); // Returns true
/// @dependencies sli.h
/// @limitations Linear search algorithm O(n*m)
extern fn_(str_contains(Sli_const$u8 haystack, Sli_const$u8 needle), bool);
```

### Prefix and Suffix Operations

#### str_startsWith

```c
/// @brief Check if string starts with specified prefix
/// @param bytes The string to check
/// @param prefix The prefix to look for
/// @return Optional index where prefix ends if found, none otherwise
/// @example
///   Sli_const$u8 text = sli_from_cstr("Hello, World!");
///   Sli_const$u8 prefix = sli_from_cstr("Hello");
///   Opt$usize end = str_startsWith(text, prefix); // Returns some(5)
/// @dependencies sli.h, opt.h
/// @limitations None
extern fn_(str_startsWith(Sli_const$u8 bytes, Sli_const$u8 prefix), Opt$usize);
```

#### str_endsWith

```c
/// @brief Check if string ends with specified suffix
/// @param bytes The string to check
/// @param suffix The suffix to look for
/// @return Optional index where suffix starts if found, none otherwise
/// @example
///   Sli_const$u8 text = sli_from_cstr("Hello, World!");
///   Sli_const$u8 suffix = sli_from_cstr("World!");
///   Opt$usize start = str_endsWith(text, suffix); // Returns some(7)
/// @dependencies sli.h, opt.h
/// @limitations None
extern fn_(str_endsWith(Sli_const$u8 bytes, Sli_const$u8 suffix), Opt$usize);
```

### String Manipulation

#### str_split

```c
/// @brief Split string by delimiter character
/// @param bytes The string to split
/// @param delim The delimiter character (as u32 codepoint)
/// @return Slice of string slices representing the split parts
/// @example
///   Sli_const$u8 text = sli_from_cstr("apple,banana,cherry");
///   Sli_const$Sli_const$u8 parts = str_split(text, ',');
/// @dependencies sli.h
/// @limitations Delimiter must be single character codepoint
extern fn_(str_split(Sli_const$u8 bytes, u32 delim), Sli_const$Sli_const$u8);
```

#### str_join

```c
/// @brief Join string parts with separator
/// @param allocator Memory allocator for result
/// @param sep Separator string to insert between parts
/// @param parts Array of string parts to join
/// @return Result containing joined string or error
/// @example
///   mem_Allocator alloc = heap_getDefaultAllocator();
///   Sli_const$u8 sep = sli_from_cstr(", ");
///   Sli$Sli_const$u8 parts = /* array of strings */;
///   Err$Sli$u8 result = str_join(alloc, sep, parts);
/// @dependencies mem.h, err_res.h, sli.h
/// @limitations May fail if memory allocation fails
extern fn_(str_join(mem_Allocator allocator, Sli_const$u8 sep, Sli$Sli_const$u8 parts), Err$Sli$u8);
```

#### str_replace

```c
/// @brief Replace all occurrences of needle with replacement
/// @param allocator Memory allocator for result
/// @param haystack The string to perform replacements on
/// @param needle The substring to replace
/// @param replacement The replacement string
/// @return Result containing new string with replacements or error
/// @example
///   mem_Allocator alloc = heap_getDefaultAllocator();
///   Sli_const$u8 text = sli_from_cstr("Hello World");
///   Sli_const$u8 needle = sli_from_cstr("World");
///   Sli_const$u8 replacement = sli_from_cstr("Universe");
///   Err$Sli$u8 result = str_replace(alloc, text, needle, replacement);
/// @dependencies mem.h, err_res.h, sli.h
/// @limitations May fail if memory allocation fails
extern fn_(str_replace(mem_Allocator allocator, Sli_const$u8 haystack, Sli_const$u8 needle, Sli_const$u8 replacement), Err$Sli$u8);
```

### Trimming Operations

#### str_trim

```c
/// @brief Remove pattern from both ends of string
/// @param bytes The string to trim
/// @param pattern Characters to remove from both ends
/// @return Trimmed string slice
/// @example
///   Sli_const$u8 text = sli_from_cstr("  hello  ");
///   Sli_const$u8 whitespace = sli_from_cstr(" \t\n\r");
///   Sli_const$u8 trimmed = str_trim(text, whitespace); // "hello"
/// @dependencies sli.h
/// @limitations Returns view into original string, doesn't allocate
extern fn_(str_trim(Sli_const$u8 bytes, Sli_const$u8 pattern), Sli_const$u8);
```

#### str_trimLeft

```c
/// @brief Remove pattern from left end of string
/// @param bytes The string to trim
/// @param pattern Characters to remove from left end
/// @return Left-trimmed string slice
/// @example
///   Sli_const$u8 text = sli_from_cstr("  hello  ");
///   Sli_const$u8 whitespace = sli_from_cstr(" \t\n\r");
///   Sli_const$u8 trimmed = str_trimLeft(text, whitespace); // "hello  "
/// @dependencies sli.h
/// @limitations Returns view into original string, doesn't allocate
extern fn_(str_trimLeft(Sli_const$u8 bytes, Sli_const$u8 pattern), Sli_const$u8);
```

#### str_trimRight

```c
/// @brief Remove pattern from right end of string
/// @param bytes The string to trim
/// @param pattern Characters to remove from right end
/// @return Right-trimmed string slice
/// @example
///   Sli_const$u8 text = sli_from_cstr("  hello  ");
///   Sli_const$u8 whitespace = sli_from_cstr(" \t\n\r");
///   Sli_const$u8 trimmed = str_trimRight(text, whitespace); // "  hello"
/// @dependencies sli.h
/// @limitations Returns view into original string, doesn't allocate
extern fn_(str_trimRight(Sli_const$u8 bytes, Sli_const$u8 pattern), Sli_const$u8);
```

### Comparison Functions

#### str_cmp

```c
/// @brief Compare two strings with case sensitivity option
/// @param lhs Left-hand side string
/// @param rhs Right-hand side string
/// @param ignores_case Whether to ignore case differences
/// @return Comparison result (Less, Equal, Greater)
/// @example
///   Sli_const$u8 str1 = sli_from_cstr("Hello");
///   Sli_const$u8 str2 = sli_from_cstr("hello");
///   cmp_Ord result = str_cmp(str1, str2, true); // Equal
/// @dependencies core/cmp.h, sli.h
/// @limitations Uses simple byte comparison for case-insensitive mode
extern fn_(str_cmp(Sli_const$u8 lhs, Sli_const$u8 rhs, bool ignores_case), cmp_Ord);
```

#### str_cmpSenseCase

```c
/// @brief Compare two strings with case sensitivity
/// @param lhs Left-hand side string
/// @param rhs Right-hand side string
/// @return Comparison result (Less, Equal, Greater)
/// @example
///   Sli_const$u8 str1 = sli_from_cstr("Hello");
///   Sli_const$u8 str2 = sli_from_cstr("hello");
///   cmp_Ord result = str_cmpSenseCase(str1, str2); // Greater
/// @dependencies core/cmp.h, sli.h
/// @limitations Byte-wise comparison
extern fn_(str_cmpSenseCase(Sli_const$u8 lhs, Sli_const$u8 rhs), cmp_Ord);
```

#### str_cmpIgnoreCase

```c
/// @brief Compare two strings ignoring case differences
/// @param lhs Left-hand side string
/// @param rhs Right-hand side string
/// @return Comparison result (Less, Equal, Greater)
/// @example
///   Sli_const$u8 str1 = sli_from_cstr("Hello");
///   Sli_const$u8 str2 = sli_from_cstr("HELLO");
///   cmp_Ord result = str_cmpIgnoreCase(str1, str2); // Equal
/// @dependencies core/cmp.h, sli.h
/// @limitations Uses simple ASCII case folding
extern fn_(str_cmpIgnoreCase(Sli_const$u8 lhs, Sli_const$u8 rhs), cmp_Ord);
```

---

## ASCII Character Utilities (ascii_*)

### Character Classification

#### ascii_isAlNum

```c
/// @brief Check if character is alphanumeric (a-z, A-Z, 0-9)
/// @param c Character to check
/// @return true if alphanumeric, false otherwise
/// @example
///   bool result = ascii_isAlNum('A'); // true
///   bool result2 = ascii_isAlNum('!'); // false
/// @dependencies None
/// @limitations ASCII only, doesn't handle Unicode
extern fn_(ascii_isAlNum(u8 c), bool);
```

#### ascii_isAlpha

```c
/// @brief Check if character is alphabetic (a-z, A-Z)
/// @param c Character to check
/// @return true if alphabetic, false otherwise
/// @example
///   bool result = ascii_isAlpha('Z'); // true
///   bool result2 = ascii_isAlpha('9'); // false
/// @dependencies None
/// @limitations ASCII only, doesn't handle Unicode
extern fn_(ascii_isAlpha(u8 c), bool);
```

#### ascii_isDigit

```c
/// @brief Check if character is numeric digit (0-9)
/// @param c Character to check
/// @return true if digit, false otherwise
/// @example
///   bool result = ascii_isDigit('5'); // true
///   bool result2 = ascii_isDigit('A'); // false
/// @dependencies None
/// @limitations ASCII only, doesn't handle Unicode digits
extern fn_(ascii_isDigit(u8 c), bool);
```

#### ascii_isLower

```c
/// @brief Check if character is lowercase letter (a-z)
/// @param c Character to check
/// @return true if lowercase, false otherwise
/// @example
///   bool result = ascii_isLower('a'); // true
///   bool result2 = ascii_isLower('A'); // false
/// @dependencies None
/// @limitations ASCII only, doesn't handle Unicode
extern fn_(ascii_isLower(u8 c), bool);
```

#### ascii_isUpper

```c
/// @brief Check if character is uppercase letter (A-Z)
/// @param c Character to check
/// @return true if uppercase, false otherwise
/// @example
///   bool result = ascii_isUpper('A'); // true
///   bool result2 = ascii_isUpper('a'); // false
/// @dependencies None
/// @limitations ASCII only, doesn't handle Unicode
extern fn_(ascii_isUpper(u8 c), bool);
```

### Character Conversion

#### ascii_toLower

```c
/// @brief Convert character to lowercase
/// @param c Character to convert
/// @return Lowercase version of character
/// @example
///   u8 result = ascii_toLower('A'); // 'a'
///   u8 result2 = ascii_toLower('5'); // '5' (unchanged)
/// @dependencies None
/// @limitations ASCII only, non-letters returned unchanged
extern fn_(ascii_toLower(u8 c), u8);
```

#### ascii_toUpper

```c
/// @brief Convert character to uppercase
/// @param c Character to convert
/// @return Uppercase version of character
/// @example
///   u8 result = ascii_toUpper('a'); // 'A'
///   u8 result2 = ascii_toUpper('5'); // '5' (unchanged)
/// @dependencies None
/// @limitations ASCII only, non-letters returned unchanged
extern fn_(ascii_toUpper(u8 c), u8);
```

### Character Comparison

#### ascii_cmp

```c
/// @brief Compare two ASCII characters with case sensitivity option
/// @param lhs Left-hand side character
/// @param rhs Right-hand side character
/// @param ignores_case Whether to ignore case differences
/// @return Comparison result (Less, Equal, Greater)
/// @example
///   cmp_Ord result = ascii_cmp('A', 'a', true); // Equal
///   cmp_Ord result2 = ascii_cmp('A', 'a', false); // Greater
/// @dependencies core/cmp.h
/// @limitations ASCII only
extern fn_(ascii_cmp(u8 lhs, u8 rhs, bool ignores_case), cmp_Ord);
```

#### ascii_cmpSenseCase

```c
/// @brief Compare two ASCII characters with case sensitivity
/// @param lhs Left-hand side character
/// @param rhs Right-hand side character
/// @return Comparison result (Less, Equal, Greater)
/// @example
///   cmp_Ord result = ascii_cmpSenseCase('A', 'a'); // Greater
/// @dependencies core/cmp.h
/// @limitations ASCII only
extern fn_(ascii_cmpSenseCase(u8 lhs, u8 rhs), cmp_Ord);
```

#### ascii_cmpIgnoreCase

```c
/// @brief Compare two ASCII characters ignoring case differences
/// @param lhs Left-hand side character
/// @param rhs Right-hand side character
/// @return Comparison result (Less, Equal, Greater)
/// @example
///   cmp_Ord result = ascii_cmpIgnoreCase('A', 'a'); // Equal
/// @dependencies core/cmp.h
/// @limitations ASCII only
extern fn_(ascii_cmpIgnoreCase(u8 lhs, u8 rhs), cmp_Ord);
```

---

## UTF-8 Utilities (utf8_*)

### Encoding and Decoding

#### utf8_encode

```c
/// @brief Encode Unicode codepoint to UTF-8 bytes
/// @param allocator Memory allocator (may be unused for fixed buffer)
/// @param codepoint Unicode codepoint to encode
/// @param buf Fixed-size buffer to store encoded bytes
/// @return Result containing number of bytes written or error
/// @example
///   utf8_A4U8Buf buffer;
///   mem_Allocator alloc = heap_getDefaultAllocator();
///   Err$usize result = utf8_encode(alloc, 0x1F600, &buffer); // 😀 emoji
/// @dependencies mem.h, err_res.h, Arr.h
/// @limitations Buffer size is fixed at 4 bytes max
extern fn_(utf8_encode(mem_Allocator allocator, u32 codepoint, utf8_A4U8Buf* buf), Err$usize);
```

#### utf8_decode

```c
/// @brief Decode first UTF-8 codepoint from byte sequence
/// @param bytes Byte sequence containing UTF-8 data
/// @return Optional codepoint, none if invalid UTF-8
/// @example
///   Sli_const$u8 utf8_bytes = sli_from_cstr("😀Hello");
///   Opt$u32 codepoint = utf8_decode(utf8_bytes); // Returns some(0x1F600)
/// @dependencies sli.h, opt.h
/// @limitations Only decodes first codepoint, doesn't validate entire sequence
extern fn_(utf8_decode(Sli_const$u8 bytes), Opt$u32);
```

### UTF-8 View and Iterator

#### utf8_View

```c
/// @brief UTF-8 string view for safe iteration
typedef struct utf8_View utf8_View;
struct utf8_View {
    Sli_const$u8 bytes;  ///< Underlying byte slice
};
```

#### utf8_View_init

```c
/// @brief Initialize UTF-8 view from byte slice
/// @param bytes Byte slice containing UTF-8 data
/// @return Initialized UTF-8 view
/// @example
///   Sli_const$u8 text = sli_from_cstr("Hello 世界");
///   utf8_View view = utf8_View_init(text);
/// @dependencies sli.h
/// @limitations Doesn't validate UTF-8 encoding
extern fn_(utf8_View_init(Sli_const$u8 bytes), utf8_View);
```

#### utf8_View_iter

```c
/// @brief Create iterator from UTF-8 view
/// @param self UTF-8 view to iterate
/// @return UTF-8 iterator positioned at start
/// @example
///   utf8_View view = utf8_View_init(text);
///   utf8_Iter iter = utf8_View_iter(view);
/// @dependencies None
/// @limitations Iterator doesn't validate UTF-8 during iteration
extern fn_(utf8_View_iter(utf8_View self), utf8_Iter);
```

#### utf8_Iter

```c
/// @brief UTF-8 iterator for codepoint-by-codepoint traversal
typedef struct utf8_Iter utf8_Iter;
struct utf8_Iter {
    Sli_const$u8 bytes;  ///< Remaining byte slice
    usize        pos;    ///< Current position in bytes
};
```

#### utf8_Iter_next

```c
/// @brief Get next codepoint from iterator
/// @param self Iterator to advance
/// @return Optional codepoint, none if end of string or invalid UTF-8
/// @example
///   utf8_Iter iter = utf8_View_iter(view);
///   while (true) {
///       Opt$u32 cp = utf8_Iter_next(&iter);
///       if (opt_isNone$(u32, cp)) break;
///       // Process codepoint
///   }
/// @dependencies opt.h
/// @limitations Advances iterator on each call
extern fn_(utf8_Iter_next(utf8_Iter* self), Opt$u32);
```

#### utf8_Iter_nextSlice

```c
/// @brief Get next codepoint as byte slice from iterator
/// @param self Iterator to advance
/// @return Optional byte slice containing single codepoint, none if end
/// @example
///   utf8_Iter iter = utf8_View_iter(view);
///   Opt$Sli_const$u8 slice = utf8_Iter_nextSlice(&iter);
/// @dependencies opt.h, sli.h
/// @limitations Advances iterator on each call
extern fn_(utf8_Iter_nextSlice(utf8_Iter* self), Opt$Sli_const$u8);
```

#### utf8_Iter_nextCodepoint

```c
/// @brief Alias for utf8_Iter_next for explicit codepoint retrieval
/// @param self Iterator to advance
/// @return Optional codepoint, none if end of string or invalid UTF-8
/// @example
///   utf8_Iter iter = utf8_View_iter(view);
///   Opt$u32 cp = utf8_Iter_nextCodepoint(&iter);
/// @dependencies opt.h
/// @limitations Identical to utf8_Iter_next
extern fn_(utf8_Iter_nextCodepoint(utf8_Iter* self), Opt$u32);
```

#### utf8_Iter_nextCodepointSlice

```c
/// @brief Alias for utf8_Iter_nextSlice for explicit slice retrieval
/// @param self Iterator to advance
/// @return Optional byte slice containing single codepoint, none if end
/// @example
///   utf8_Iter iter = utf8_View_iter(view);
///   Opt$Sli_const$u8 slice = utf8_Iter_nextCodepointSlice(&iter);
/// @dependencies opt.h, sli.h
/// @limitations Identical to utf8_Iter_nextSlice
extern fn_(utf8_Iter_nextCodepointSlice(utf8_Iter* self), Opt$Sli_const$u8);
```

---

## Complete Example

```c
#include "dh/main.h"
#include "dh/Str.h"
#include "dh/mem.h"
#include "dh/heap.h"

fn_(dh_main(Sli$Sli_const$u8 args), Err$void $scope) {
    mem_Allocator allocator = heap_getDefaultAllocator();

    // String operations
    Sli_const$u8 text = sli_from_cstr("Hello, 世界! How are you?");
    Sli_const$u8 pattern = sli_from_cstr("世界");

    // Find substring
    Opt$usize pos = str_find(text, pattern);
    if (opt_isSome$(usize, pos)) {
        printf("Found '世界' at position: %zu\n", opt_unwrap$(usize, pos));
    }

    // Check if starts with prefix
    Sli_const$u8 greeting = sli_from_cstr("Hello");
    if (opt_isSome$(usize, str_startsWith(text, greeting))) {
        printf("Text starts with 'Hello'\n");
    }

    // ASCII character operations
    u8 ch = 'A';
    if (ascii_isUpper(ch)) {
        printf("'%c' is uppercase, lowercase: '%c'\n", ch, ascii_toLower(ch));
    }

    // UTF-8 iteration
    printf("Codepoints: ");
    utf8_View view = utf8_View_init(text);
    utf8_Iter iter = utf8_View_iter(view);

    while (true) {
        Opt$u32 codepoint = utf8_Iter_next(&iter);
        if (opt_isNone$(u32, codepoint)) break;
        printf("U+%04X ", opt_unwrap$(u32, codepoint));
    }
    printf("\n");

    // String manipulation
    Sli_const$u8 dirty = sli_from_cstr("  trim me  ");
    Sli_const$u8 whitespace = sli_from_cstr(" \t\n\r");
    Sli_const$u8 clean = str_trim(dirty, whitespace);
    printf("Trimmed: '%.*s'\n", (int)sli_len(clean), sli_ptr(clean));

    return_ok({});
} $unscoped;
```

## Dependencies

- `dh/sli.h` - Slice operations
- `dh/opt.h` - Optional types
- `dh/mem.h` - Memory management
- `dh/err_res.h` - Error handling
- `dh/core/cmp.h` - Comparison operations
- `dh/Arr.h` - Fixed-size arrays

## Notes

- All string operations work on byte slices (`Sli_const$u8`) for maximum flexibility
- ASCII functions only work with 7-bit ASCII characters
- UTF-8 functions properly handle Unicode codepoints
- String manipulation functions that allocate memory require a memory allocator
- View and slice operations return references to original data without copying
- Case-insensitive operations use simple ASCII case folding

## Performance Characteristics

- String search: O(n*m) where n = haystack length, m = needle length
- Trimming: O(n) where n = string length
- UTF-8 iteration: O(n) where n = number of codepoints
- ASCII operations: O(1)
- String comparison: O(min(n,m)) where n,m = string lengths
