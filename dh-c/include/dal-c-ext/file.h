#pragma once
#ifndef dal_c_ext_file__included
#define dal_c_ext_file__included 1

#include "base.h"
#include <errno.h>
#include <stdio.h>
#include <time.h>

typedef struct file_Lock {
#ifdef _WIN32
    void* handle;
#else
    int fd;
#endif
    char* path;
    bool waited;
} file_Lock;

// Returns: allocated string; caller must `free()`
extern char* file_read(const char* path);
// Returns: allocated array of strings; caller must `free()` each string and the array
extern char** file_readLines(const char* path, int* count);
extern bool file_write(const char* path, const char* content);
extern bool file_writeAtomic(const char* path, const char* content);
extern bool file_copy(const char* src, const char* dst);
extern bool file_lockAcquire(file_Lock* lock, const char* path);
extern void file_lockRelease(file_Lock* lock);
// Returns: creation time of file, or 0 on error
extern time_t file_ctime(const char* path);
// Returns: modification time of file, or 0 on error
extern time_t file_mtime(const char* path);
// Returns: access time of file, or 0 on error
extern time_t file_atime(const char* path);

#endif /* dal_c_ext_file__included */
