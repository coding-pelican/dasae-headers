#ifndef dal_c_ext_path__included
#define dal_c_ext_path__included 1

#include "base.h"
#include <errno.h>
#include <stdlib.h>

// Returns: allocated string; caller must `free()`
extern char* path_join(const char* base, const char* component);
// Returns: allocated string; caller must `free()`
extern char* path_parent(const char* path);
// Returns: allocated string; caller must `free()`
extern char* path_basename(const char* path);
// Returns: allocated string; caller must `free()`
extern char* path_abs(const char* path);
// Returns: path relative to base (allocated), or NULL if path is not under base; caller must `free()`
extern char* path_relative(const char* base, const char* path);

// Path queries
extern bool path_exists(const char* path);
extern bool path_isFile(const char* path);
extern bool path_isDir(const char* path);

#endif /* dal_c_ext_path__included */
