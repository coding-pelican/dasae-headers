#ifndef dal_c_ext_dir__included
#define dal_c_ext_dir__included 1

#include "dal-c-ext/base.h"
#include <errno.h>
#include <stdlib.h>

extern bool dir_create(const char* path);
extern bool dir_createRecur(const char* path);
extern bool dir_removeRecur(const char* path);
// Returns: allocated array of strings; caller must `free()` each string and the array
extern char** dir_listRecur(const char* path, int* count);

#endif /* dal_c_ext_dir__included */
