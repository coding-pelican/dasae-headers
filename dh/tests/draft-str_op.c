// //! String Operation API Reference for Zig Standard Library
// //! =====================================================
// //! This file contains function signatures for string operations available in Zig's standard library.
// //! Functions are organized by module: mem, ascii, and unicode.
// //! All signatures are documented as comments to serve as a reference.

// const std = @import("std");
// const mem = std.mem;

// ==== MEM.ZIG STRING OPERATIONS ====
// Generic slice operations that work with []const u8 for strings

// EQUALITY AND COMPARISON
//   Returns true if slices are equal, false otherwise
#define mem_eql$(_T, /* Sli_const$T */ _lhs, /* Sli_const$T */ _rhs... /* bool */)
#define mem_eql_useT$(_T...)
//   Returns index of first difference between slices, or null if equal
#define mem_idxOfDiff$(_T, /* Sli_const$T */ _lhs, /* Sli_const$T */ _rhs... /* Opt$usize */)
#define mem_idxOfDiff_useT$(_T...)

// TRIMMING OPERATIONS
//   Trims characters from both sides of a slice
#define mem_trim$(_T, /* Sli_const$T */ _bytes, /* Sli_const$T */ _to_strip... /* Sli_const$T */)
#define mem_trim_useT$(_T...)
//   Trims characters from the left side of a slice
#define mem_trimLeft$(_T, /* Sli_const$T */ _bytes, /* Sli_const$T */ _to_strip... /* Sli_const$T */)
#define mem_trimLeft_useT$(_T...)
//   Trims characters from the right side of a slice
#define mem_trimRight$(_T, /* Sli_const$T */ _bytes, /* Sli_const$T */ _to_strip... /* Sli_const$T */)
#define mem_trimRight_useT$(_T...)

// SINGLE VALUE SEARCH
//   Finds first occurrence of scalar value in slice
#define mem_idxOfScalar$(_T, /* Sli_const$T */ _slice, /* _T */ _value... /* Opt$usize */)
#define mem_idxOfScalar_useT$(_T...)
//   Finds last occurrence of scalar value in slice
#define mem_lastIdxOfScalar$(_T, /* Sli_const$T */ _slice, /* _T */ _value... /* Opt$usize */)
#define mem_lastIdxOfScalar_useT$(_T...)
//   Finds first occurrence of scalar value starting from start_index
#define mem_idxOfScalarPos$(_T, /* Sli_const$T */ _slice, /* usize */ _start_index, /* _T */ _value... /* Opt$usize */)
#define mem_idxOfScalarPos_useT$(_T...)

// MULTIPLE VALUE SEARCH
//   Finds first occurrence of any value from values array
#define mem_idxOfAny$(_T, /* Sli_const$T */ _slice, /* Sli_const$T */ _values... /* Opt$usize */)
#define mem_idxOfAny_useT$(_T...)
//   Finds last occurrence of any value from values array
#define mem_lastIdxOfAny$(_T, /* Sli_const$T */ _slice, /* Sli_const$T */ _values... /* Opt$usize */)
#define mem_lastIdxOfAny_useT$(_T...)
//   Finds first occurrence of any value from values starting from start_index
#define mem_idxOfAnyPos$(_T, /* Sli_const$T */ _slice, /* usize */ _start_index, /* Sli_const$T */ _values... /* Opt$usize */)
#define mem_idxOfAnyPos_useT$(_T...)
//   Finds first occurrence of value NOT in values
#define mem_idxOfNone$(_T, /* Sli_const$T */ _slice, /* Sli_const$T */ _values... /* Opt$usize */)
#define mem_idxOfNone_useT$(_T...)
//   Finds last occurrence of value NOT in values
#define mem_lastIdxOfNone$(_T, /* Sli_const$T */ _slice, /* Sli_const$T */ _values... /* Opt$usize */)
#define mem_lastIdxOfNone_useT$(_T...)
//   Finds first occurrence of value NOT in values starting from start_index
#define mem_idxOfNonePos$(_T, /* Sli_const$T */ _slice, /* usize */ _start_index, /* Sli_const$T */ _values... /* Opt$usize */)
#define mem_idxOfNonePos_useT$(_T...)

// SUBSTRING SEARCH
//   Finds needle in haystack (Boyer-Moore-Horspool for large inputs)
#define mem_idxOf$(_T, /* Sli_const$T */ _haystack, /* Sli_const$T */ _needle... /* Opt$usize */)
#define mem_idxOf_useT$(_T...)
//   Finds needle in haystack using linear search from the end
#define mem_lastIdxOfLinear$(_T, /* Sli_const$T */ _haystack, /* Sli_const$T */ _needle... /* Opt$usize */)
#define mem_lastIdxOfLinear_useT$(_T...)
//   Finds needle in haystack starting from start_index (linear search)
#define mem_idxOfPosLinear$(_T, /* Sli_const$T */ _haystack, /* usize */ _start_index, /* Sli_const$T */ _needle... /* Opt$usize */)
#define mem_idxOfPosLinear_useT$(_T...)
//   Finds needle in haystack from the end
#define mem_lastIdxOf$(_T, /* Sli_const$T */ _haystack, /* Sli_const$T */ _needle... /* Opt$usize */)
#define mem_lastIdxOf_useT$(_T...)
//   Finds needle in haystack starting from start_index
#define mem_idxOfPos$(_T, /* Sli_const$T */ _haystack, /* usize */ _start_index, /* Sli_const$T */ _needle... /* Opt$usize */)
#define mem_idxOfPos_useT$(_T...)

// PREFIX/SUFFIX OPERATIONS
//   Returns true if haystack starts with needle
#define mem_startsWith$(_T, /* Sli_const$T */ _haystack, /* Sli_const$T */ _needle... /* Opt$usize */)
#define mem_startsWith_useT$(_T...)
//   Returns true if haystack ends with needle
#define mem_endsWith$(_T, /* Sli_const$T */ _haystack, /* Sli_const$T */ _needle... /* Opt$usize */)
#define mem_endsWith_useT$(_T...)

// SPLITTING OPERATIONS
// pub fn splitSequence(comptime T: type, buffer: []const T, delimiter: []const T) SplitIterator(T, .sequence)
//   Splits a slice by a delimiter sequence

// pub fn splitAny(comptime T: type, buffer: []const T, delimiters: []const T) SplitIterator(T, .any)
//   Splits a slice by any character in delimiters

// pub fn splitScalar(comptime T: type, buffer: []const T, delimiter: T) SplitIterator(T, .scalar)
//   Splits a slice by a single delimiter

// pub fn splitBackwardsSequence(comptime T: type, buffer: []const T, delimiter: []const T) SplitBackwardsIterator(T, .sequence)
//   Splits a slice by a delimiter sequence from the end

// pub fn splitBackwardsAny(comptime T: type, buffer: []const T, delimiters: []const T) SplitBackwardsIterator(T, .any)
//   Splits a slice by any character in delimiters from the end

// pub fn splitBackwardsScalar(comptime T: type, buffer: []const T, delimiter: T) SplitBackwardsIterator(T, .scalar)
//   Splits a slice by a single delimiter from the end

// JOINING AND CONCATENATION
// pub fn concat(allocator: Allocator, comptime T: type, slices: []const []const T) Allocator.Error![]T
//   Concatenates slices together

// pub fn concatWithSentinel(allocator: Allocator, comptime T: type, slices: []const []const T, comptime s: T) Allocator.Error![:s]T
//   Concatenates slices together with a sentinel value

// pub fn concatMaybeSentinel(allocator: Allocator, comptime T: type, slices: []const []const T, comptime s: ?T) Allocator.Error![]T
//   Concatenates slices together with an optional sentinel value

// pub fn join(allocator: Allocator, separator: []const u8, slices: []const []const u8) Allocator.Error![]u8
//   Joins slices with a separator

// pub fn joinZ(allocator: Allocator, separator: []const u8, slices: []const []const u8) Allocator.Error![:0]u8
//   Joins slices with a separator and null-terminates the result

// REPLACEMENT OPERATIONS
//   Replaces all occurrences of needle with replacement in input, writing to output
#define mem_replace$(_T, /* Sli_const$T */ _input, /* Sli_const$T */ _needle, /* Sli_const$T */ _replacement, /* Sli$T */ _output... /* usize */)
#define mem_replace_useT$(_T...)
//   Replaces all occurrences of match with replacement in-place
#define mem_replaceScalar$(_T, /* Sli$T */ _slice, /* _T */ _match, /* _T */ _replacement... /* void */)
#define mem_replaceScalar_useT$(_T...)
//   Calculates the size needed for replacing all occurrences of needle with replacement
#define mem_replacementSize$(_T, /* Sli_const$T */ _input, /* Sli_const$T */ _needle, /* Sli_const$T */ _replacement... /* usize */)
#define mem_replacementSize_useT$(_T...)
//   Replaces all occurrences of needle with replacement, allocating a new slice
#define mem_replaceOwned$(_T, /* mem_Allocator */ _allocator, /* Sli_const$T */ _input, /* Sli_const$T */ _needle, /* Sli_const$T */ _replacement... /* mem_Alloc_Err$Sli$T */)
#define mem_replaceOwned_useT$(_T...)

// MEMORY OPERATIONS
//   Copies data from source to dest (forward direction)
#define mem_copyForwards$(_T, /* Sli$T */ _dst, /* Sli_const$T */ _src... /* void */)
#define mem_copyForwards_useT$(_T...)
//   Copies data from source to dest (backward direction)
#define mem_copyBackwards$(_T, /* Sli$T */ _dst, /* Sli_const$T */ _src... /* void */)
#define mem_copyBackwards_useT$(_T...)

// ==== ASCII.ZIG STRING OPERATIONS ====
// ASCII-specific string operations

// CASE CONVERSION
// pub fn lowerString(output: []u8, ascii_string: []const u8) []u8
//   Writes a lowercase copy of ascii_string to output

// pub fn allocLowerString(allocator: Allocator, ascii_string: []const u8) ![]u8
//   Allocates a lowercase copy of ascii_string

// pub fn upperString(output: []u8, ascii_string: []const u8) []u8
//   Writes an uppercase copy of ascii_string to output

// pub fn allocUpperString(allocator: Allocator, ascii_string: []const u8) ![]u8
//   Allocates an uppercase copy of ascii_string

// CASE-INSENSITIVE OPERATIONS
// pub fn eqlIgnoreCase(a: []const u8, b: []const u8) bool
//   Compares strings case-insensitively

// pub fn startsWithIgnoreCase(haystack: []const u8, needle: []const u8) bool
//   Returns true if haystack starts with needle (case-insensitive)

// pub fn endsWithIgnoreCase(haystack: []const u8, needle: []const u8) bool
//   Returns true if haystack ends with needle (case-insensitive)

// pub fn idxOfIgnoreCase(haystack: []const u8, needle: []const u8) ?usize
//   Finds needle in haystack (case-insensitive)

// pub fn idxOfIgnoreCasePos(haystack: []const u8, start_index: usize, needle: []const u8) ?usize
//   Finds needle in haystack starting from start_index (case-insensitive)

// pub fn idxOfIgnoreCasePosLinear(haystack: []const u8, start_index: usize, needle: []const u8) ?usize
//   Finds needle in haystack starting from start_index (case-insensitive, linear search)

// pub fn orderIgnoreCase(lhs: []const u8, rhs: []const u8) std.math.Order
//   Returns lexicographical order of two strings (case-insensitive)

// pub fn lessThanIgnoreCase(lhs: []const u8, rhs: []const u8) bool
//   Returns true if lhs is lexicographically less than rhs (case-insensitive)

// ==== UNICODE.ZIG STRING OPERATIONS ====
// UTF-8 and Unicode string operations

// UTF-8 LENGTH AND VALIDATION
// pub fn utf8CodepointSequenceLength(c: u21) !u3
//   Returns how many bytes the UTF-8 representation would require for the given codepoint

// pub fn utf8ByteSequenceLength(first_byte: u8) !u3
//   Returns the total length of the UTF-8 codepoint in bytes given the first byte

// pub fn utf8ValidCodepoint(value: u21) bool
//   Returns true if the given unicode codepoint can be encoded in UTF-8

// pub fn utf8CountCodepoints(s: []const u8) !usize
//   Returns the length of a UTF-8 string in terms of unicode codepoints

// pub fn utf8ValidateSlice(input: []const u8) bool
//   Validates that a slice is valid UTF-8

// UTF-8 ENCODING/DECODING
// pub fn utf8Encode(c: u21, out: []u8) error{ Utf8CannotEncodeSurrogateHalf, CodepointTooLarge }!u3
//   Encodes the given codepoint into a UTF-8 byte sequence

// pub fn utf8Decode(bytes: []const u8) Utf8DecodeError!u21
//   Decodes a UTF-8 byte sequence into a codepoint

// pub fn utf8Decode2(bytes: [2]u8) Utf8Decode2Error!u21
//   Decodes a 2-byte UTF-8 sequence

// pub fn utf8Decode3(bytes: [3]u8) Utf8Decode3Error!u21
//   Decodes a 3-byte UTF-8 sequence

// pub fn utf8Decode3AllowSurrogateHalf(bytes: [3]u8) Utf8Decode3AllowSurrogateHalfError!u21
//   Decodes a 3-byte UTF-8 sequence allowing surrogate halves

// pub fn utf8Decode4(bytes: [4]u8) Utf8Decode4Error!u21
//   Decodes a 4-byte UTF-8 sequence

// UTF-16 CONVERSION
// pub fn utf16LeToUtf8(utf8: []u8, utf16le: []const u16) Utf16LeToUtf8Error!usize
//   Converts UTF-16 little-endian to UTF-8

// pub fn utf16LeToUtf8Alloc(allocator: Allocator, utf16le: []const u16) Utf16LeToUtf8AllocError![]u8
//   Allocates and converts UTF-16 little-endian to UTF-8

// pub fn utf16LeToUtf8AllocZ(allocator: Allocator, utf16le: []const u16) Utf16LeToUtf8AllocError![:0]u8
//   Allocates and converts UTF-16 little-endian to null-terminated UTF-8

// pub fn utf8ToUtf16Le(utf16le: []u16, utf8: []const u8) error{InvalidUtf8}!usize
//   Converts UTF-8 to UTF-16 little-endian

// pub fn utf8ToUtf16LeAlloc(allocator: Allocator, utf8: []const u8) error{ InvalidUtf8, OutOfMemory }![]u16
//   Allocates and converts UTF-8 to UTF-16 little-endian

// pub fn utf8ToUtf16LeAllocZ(allocator: Allocator, utf8: []const u8) error{ InvalidUtf8, OutOfMemory }![:0]u16
//   Allocates and converts UTF-8 to null-terminated UTF-16 little-endian

// pub fn calcUtf16LeLen(utf8: []const u8) CalcUtf16LeLenError!usize
//   Calculates the length needed for UTF-16 little-endian representation of UTF-8 string

// WTF-8 CONVERSION
// pub fn wtf8ToUtf8Lossy(utf8: []u8, wtf8: []const u8) error{InvalidWtf8}!void
//   Converts WTF-8 to UTF-8 lossily (replacing invalid sequences)

// pub fn wtf8ToUtf8LossyAlloc(allocator: Allocator, wtf8: []const u8) error{ InvalidWtf8, OutOfMemory }![]u8
//   Allocates and converts WTF-8 to UTF-8 lossily

// pub fn wtf8ToUtf8LossyAllocZ(allocator: Allocator, wtf8: []const u8) error{ InvalidWtf8, OutOfMemory }![:0]u8
//   Allocates and converts WTF-8 to null-terminated UTF-8 lossily

// // ==== USAGE EXAMPLES ====

// pub fn demonstrateStringOperations() void {
//     // Example usage with actual Zig std library functions:

//     // String equality
//     const equal = std.mem.eql(u8, "hello", "hello"); // true
//     _ = equal;

//     // Case-insensitive comparison
//     const equal_ignore_case = std.ascii.eqlIgnoreCase("Hello", "HELLO"); // true
//     _ = equal_ignore_case;

//     // String searching
//     const index = std.mem.idxOf(u8, "hello world", "world"); // Some(6)
//     _ = index;

//     // Trimming whitespace
//     const trimmed = std.mem.trim(u8, "  hello  ", " "); // "hello"
//     _ = trimmed;

//     // UTF-8 validation
//     const valid = std.unicode.utf8ValidateSlice("Hello 世界"); // true
//     _ = valid;

//     // Check if string starts with prefix
//     const starts = std.mem.startsWith(u8, "hello world", "hello"); // true
//     _ = starts;

//     // Case-insensitive search
//     const case_index = std.ascii.idxOfIgnoreCase("Hello World", "WORLD"); // Some(6)
//     _ = case_index;
// }

// // ==== IMPORT REFERENCE ====
// // To use these functions in your code, import them from std:
// // const std = @import("std");
// // const mem = std.mem;           // For generic memory/slice operations
// // const ascii = std.ascii;       // For ASCII-specific operations
// // const unicode = std.unicode;   // For Unicode/UTF-8 operations
