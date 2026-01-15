#ifndef dal_c_ext_str__included
#define dal_c_ext_str__included 1

#include "dal-c-ext/base.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

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
extern char* str_format(const char* fmt, ...);

#endif /* dal_c_ext_str__included */
