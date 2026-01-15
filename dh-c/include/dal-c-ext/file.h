#ifndef dal_c_ext_file__included
#define dal_c_ext_file__included 1

#include "dal-c-ext/base.h"
#include <errno.h>
#include <stdio.h>
#include <time.h>

// Returns: allocated string; caller must `free()`
extern char* file_read(const char* path);
// Returns: allocated array of strings; caller must `free()` each string and the array
extern char** file_readLines(const char* path, int* count);
extern bool file_write(const char* path, const char* content);
extern bool file_copy(const char* src, const char* dst);
// Returns: creation time of file, or 0 on error
extern time_t file_ctime(const char* path);
// Returns: modification time of file, or 0 on error
extern time_t file_mtime(const char* path);
// Returns: access time of file, or 0 on error
extern time_t file_atime(const char* path);

#endif /* dal_c_ext_file__included */
