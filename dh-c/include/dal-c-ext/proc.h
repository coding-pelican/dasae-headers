#pragma once
#ifndef dal_c_ext_proc__included
#define dal_c_ext_proc__included 1

#include "base.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

// Process execution
typedef struct proc_Child proc_Child;

extern proc_Child* proc_spawn(const char** argv, bool show_output);
extern int proc_wait(proc_Child** child);
extern int proc_waitAny(proc_Child** children, int count, int* index_out);
extern int proc_run(const char** argv, bool show_output);
extern int proc_runMergedOutput(const char** argv, bool show_output);
// Returns: allocated string; caller must `free()`
extern char* proc_output(const char** argv);

#endif /* dal_c_ext_proc__included */
