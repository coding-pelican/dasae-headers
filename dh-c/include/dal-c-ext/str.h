#pragma once
#ifndef dal_c_ext_str__included
#define dal_c_ext_str__included 1

#include "base.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#if defined(__clang__) || defined(__GNUC__)
#define dal_c_ext__printf_format(_fmt_idx, _args_idx) __attribute__((format(printf, _fmt_idx, _args_idx)))
#else
#define dal_c_ext__printf_format(_fmt_idx, _args_idx)
#endif

// String comparison and testing
extern bool str_eql(const char* lhs, const char* rhs);
extern bool str_startsWith(const char* str, const char* prefix);
extern bool str_endsWith(const char* str, const char* suffix);

// String manipulation
// Returns: allocated string; caller must `free()`
extern char* str_join(const char* sep, const char** parts, int count);
// Returns: allocated array of strings; caller must `free()` each string and the array
extern char** str_split(const char* str, const char* delim, int* count);
// Returns: allocated string; caller must `free()`
extern char* str_trim(const char* str);
// Returns: allocated string; caller must `free()`
extern char* str_toUpper(const char* str);
// Returns: allocated string; caller must `free()`
extern char* str_format(const char* fmt, ...) dal_c_ext__printf_format(1, 2);

#endif /* dal_c_ext_str__included */
