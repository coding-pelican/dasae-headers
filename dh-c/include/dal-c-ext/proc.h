#ifndef dal_c_ext_proc__included
#define dal_c_ext_proc__included 1

#include "dal-c-ext/base.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

// Process execution
extern int proc_run(char** argv, bool show_output);
// Returns: allocated string; caller must `free()`
extern char* proc_output(char** argv);

#endif /* dal_c_ext_proc__included */
