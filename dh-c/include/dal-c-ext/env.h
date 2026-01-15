#ifndef dal_c_ext_env__included
#define dal_c_ext_env__included 1

#include "dal-c-ext/base.h"
#include <errno.h>
#include <stdlib.h>

// Returns: allocated string; caller must `free()`
extern char* env_get(const char* name);
// Returns: allocated string; caller must `free()`
extern char* env_getCWD(void);
// Returns: true on success, false on failure
extern bool env_setCWD(const char* path);
// Returns: allocated string; caller must `free()`
extern char* env_getExecutableDir(void);
// Returns: allocated string; caller must `free()`
extern char* env_getExecutablePath(void);

#endif /* dal_c_ext_env__included */
