#include "dal-c-ext/env.h"
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

char* env_get(const char* name) {
    if (!name) { return NULL; }
    const char* const value = getenv(name); // NOLINT(concurrency-mt-unsafe)
    if (!value) { return NULL; }
    return strdup(value);
}

char* env_getCWD(void) {
#ifdef _WIN32
    char* const cwd = (char*)malloc(MAX_PATH);
    if (!cwd) { return NULL; }
    if (_getcwd(cwd, MAX_PATH) == NULL) { return free(cwd), NULL; }
    return cwd;
#else
    char* const cwd = (char*)malloc(PATH_MAX);
    if (!cwd) { return NULL; }
    if (getcwd(cwd, PATH_MAX) == NULL) { return free(cwd), NULL; }
    return cwd;
#endif
}

bool env_setCWD(const char* path) {
    if (!path) { return false; }
#ifdef _WIN32
    return _chdir(path) == 0;
#else
    return chdir(path) == 0;
#endif
}

char* env_getExecutableDir(void) {
#ifdef _WIN32
    char exe_path[MAX_PATH] = {};
    const DWORD len = GetModuleFileNameA(NULL, exe_path, MAX_PATH);
    if (len == 0 || len >= MAX_PATH) { return NULL; }
    // Find last backslash
    char* const last_slash = strrchr(exe_path, '\\');
    if (!last_slash) { return NULL; }
    *last_slash = '\0';
    return strdup(exe_path);
#else
    char exe_path[PATH_MAX] = {};
    ssize_t len = readlink("/proc/self/exe", exe_path, PATH_MAX - 1);
    if (len == -1) {
        // Fallback: try /proc/curproc/file (FreeBSD)
        len = readlink("/proc/curproc/file", exe_path, PATH_MAX - 1);
        if (len == -1) { return NULL; }
    }
    exe_path[len] = '\0';
    // Find last slash
    char* const last_slash = strrchr(exe_path, '/');
    if (!last_slash) { return NULL; }
    *last_slash = '\0';
    return strdup(exe_path);
#endif
}

char* env_getExecutablePath(void) {
#ifdef _WIN32
    char exe_path[MAX_PATH] = {};
    const DWORD len = GetModuleFileNameA(NULL, exe_path, MAX_PATH);
    if (len == 0 || len >= MAX_PATH) { return NULL; }
    return strdup(exe_path);
#else
    char exe_path[PATH_MAX] = {};
    ssize_t len = readlink("/proc/self/exe", exe_path, PATH_MAX - 1);
    if (len == -1) {
        // Fallback: try /proc/curproc/file (FreeBSD)
        len = readlink("/proc/curproc/file", exe_path, PATH_MAX - 1);
        if (len == -1) { return NULL; }
    }
    exe_path[len] = '\0';
    return strdup(exe_path);
#endif
}
