#include "dal-c-ext/path.h"
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <io.h>
#define PATH_SEP '\\'
#define PATH_SEP_STR "\\"
#else
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#define PATH_SEP '/'
#define PATH_SEP_STR "/"
#endif

char* path_join(const char* base, const char* component) {
    if (!base || !component) { return NULL; }

    size_t base_len = strlen(base);
    size_t comp_len = strlen(component);
    // Remove trailing separator from base
    while (base_len > 0 && (base[base_len - 1] == '/' || base[base_len - 1] == '\\')) { base_len--; }
    // Remove leading separator from component
    size_t comp_start = 0;
    while (comp_start < comp_len && (component[comp_start] == '/' || component[comp_start] == '\\')) { comp_start++; }
    comp_len -= comp_start;

    // Allocate result
    char* const result = (char*)malloc(base_len + comp_len + 2);
    if (!result) { return NULL; }
    // Copy base
    strncpy(result, base, base_len);
    result[base_len] = '\0';
    // Add separator if needed
    if (base_len > 0 && comp_len > 0) { strcat(result, PATH_SEP_STR); }
    // Add component
    if (comp_len > 0) { strcat(result, component + comp_start); }

    return result;
}

char* path_parent(const char* path) {
    if (!path) { return NULL; }

    size_t len = strlen(path);
    if (len == 0) { return strdup("."); }
    // Remove trailing separators
    while (len > 0 && (path[len - 1] == '/' || path[len - 1] == '\\')) { len--; }
    if (len == 0) { return strdup(PATH_SEP_STR); }

    // Find last separator
    size_t last_sep = len;
    for (size_t i = len; i > 0; --i) {
        if (path[i - 1] == '/' || path[i - 1] == '\\') {
            last_sep = i - 1;
            break;
        }
    }
    if (last_sep == len) { return strdup("."); } // No separator found
    if (last_sep == 0) {
        // Root directory
        char* const result = (char*)malloc(2);
        if (!result) { return NULL; }
        result[0] = path[0];
        result[1] = '\0';
        return result;
    }

    // Copy parent path
    char* const result = (char*)malloc(last_sep + 1);
    if (!result) { return NULL; }
    strncpy(result, path, last_sep);
    result[last_sep] = '\0';

    return result;
}

char* path_basename(const char* path) {
    if (!path) { return NULL; }

    size_t len = strlen(path);
    if (len == 0) { return strdup(""); }
    // Remove trailing separators
    while (len > 0 && (path[len - 1] == '/' || path[len - 1] == '\\')) { len--; }
    if (len == 0) { return strdup(""); }

    // Find last separator
    size_t last_sep = len;
    for (size_t i = len; i > 0; --i) {
        if (path[i - 1] == '/' || path[i - 1] == '\\') {
            last_sep = i;
            break;
        }
    }

    return strdup(path + last_sep);
}

char* path_abs(const char* path) {
    if (!path) { return NULL; }
#ifdef _WIN32
    char full_path[MAX_PATH] = {};
    if (_fullpath(full_path, path, MAX_PATH) == NULL) { return NULL; }
    return strdup(full_path);
#else
    char* full_path = realpath(path, NULL);
    return full_path; // NULL if path doesn't exist
#endif
}

bool path_exists(const char* path) {
    if (!path) { return false; }
#ifdef _WIN32
    return _access(path, 0) == 0;
#else
    return access(path, F_OK) == 0;
#endif
}

bool path_isFile(const char* path) {
    if (!path) { return false; }
#ifdef _WIN32
    const DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) { return false; }
    return (attrs & FILE_ATTRIBUTE_DIRECTORY) == 0;
#else
    struct stat st = {};
    if (stat(path, &st) != 0) { return false; }
    return S_ISREG(st.st_mode);
#endif
}

bool path_isDir(const char* path) {
    if (!path) { return false; }
#ifdef _WIN32
    const DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) { return false; }
    return (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
    struct stat st = {};
    if (stat(path, &st) != 0) { return false; }
    return S_ISDIR(st.st_mode);
#endif
}
