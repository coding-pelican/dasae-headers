#include "dal-c-ext/path.h"
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <io.h>
#define PATH_SEP '\\'
#define PATH_SEP_STR "\\"
#else
#include <unistd.h>
#include <sys/stat.h>
#define PATH_SEP '/'
#define PATH_SEP_STR "/"
#endif
#include <string.h>

static bool path__isSeparator(char ch) {
    return ch == '/' || ch == '\\';
}

static char* path__dupRange(const char* src, size_t len) {
    char* const copy = (char*)malloc(len + 1);
    if (!copy) { return NULL; }
    memcpy(copy, src, len);
    copy[len] = '\0';
    return copy;
}

static bool path__isWindowsDriveRoot(const char* path, size_t len) {
#ifdef _WIN32
    return (len == 2 && ((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z')) && path[1] == ':')
        || (len == 3 && ((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z')) && path[1] == ':' && path__isSeparator(path[2]));
#else
    (void)path;
    (void)len;
    return false;
#endif
}

char* path_join(const char* base, const char* component) {
    if (!base || !component) { return NULL; }

    size_t base_len = strlen(base);
    size_t comp_len = strlen(component);
    // Remove trailing separator from base
    while (base_len > 0 && path__isSeparator(base[base_len - 1])) { base_len--; }
    // Remove leading separator from component
    size_t comp_start = 0;
    while (comp_start < comp_len && path__isSeparator(component[comp_start])) { comp_start++; }
    comp_len -= comp_start;

    // Allocate result
    char* const result = (char*)malloc(base_len + comp_len + 2);
    if (!result) { return NULL; }
    memcpy(result, base, base_len);
    size_t out_len = base_len;
    if (base_len > 0 && comp_len > 0) {
        result[out_len++] = PATH_SEP;
    }
    if (comp_len > 0) {
        memcpy(result + out_len, component + comp_start, comp_len);
        out_len += comp_len;
    }
    result[out_len] = '\0';

    return result;
}

char* path_parent(const char* path) {
    if (!path) { return NULL; }

    size_t len = strlen(path);
    if (len == 0) { return strdup("."); }
    // Remove trailing separators
    while (len > 0 && path__isSeparator(path[len - 1]) && !path__isWindowsDriveRoot(path, len)) { len--; }
    if (len == 0 || path__isWindowsDriveRoot(path, len)) { return path__dupRange(path, len); }

    // Find last separator
    size_t last_sep = len;
    for (size_t i = len; i > 0; --i) {
        if (path__isSeparator(path[i - 1])) {
            last_sep = i - 1;
            break;
        }
    }
    if (last_sep == len) { return strdup("."); } // No separator found
    if (last_sep == 0) {
        return strdup(PATH_SEP_STR);
    }
    if (last_sep == 2 && path__isWindowsDriveRoot(path, 3)) {
        return path__dupRange(path, 3);
    }

    // Copy parent path
    char* const result = (char*)malloc(last_sep + 1);
    if (!result) { return NULL; }
    memcpy(result, path, last_sep);
    result[last_sep] = '\0';

    return result;
}

char* path_basename(const char* path) {
    if (!path) { return NULL; }

    size_t len = strlen(path);
    if (len == 0) { return strdup(""); }
    // Remove trailing separators
    while (len > 0 && path__isSeparator(path[len - 1]) && !path__isWindowsDriveRoot(path, len)) { len--; }
    if (len == 0) { return strdup(""); }
    if (path__isWindowsDriveRoot(path, len)) { return path__dupRange(path, len); }

    // Find last separator
    size_t last_sep = 0;
    for (size_t i = len; i > 0; --i) {
        if (path__isSeparator(path[i - 1])) {
            last_sep = i;
            break;
        }
    }

    return path__dupRange(path + last_sep, len - last_sep);
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

char* path_relative(const char* base, const char* path) {
    if (!base || !path) { return NULL; }
    size_t base_len = strlen(base);
    size_t path_len = strlen(path);
    while (base_len > 0 && (base[base_len - 1] == '/' || base[base_len - 1] == '\\')) { base_len--; }
    if (base_len == 0) { return strdup(path); }
    if (path_len <= base_len) { return NULL; }
    int is_sep = (path[base_len] == '/' || path[base_len] == '\\');
    if (!is_sep && path[base_len] != '\0') { return NULL; }
    for (size_t i = 0; i < base_len; i++) {
        char bc = (base[i] == '\\') ? '/' : base[i];
        char pc = (path[i] == '\\') ? '/' : path[i];
        if (bc != pc) { return NULL; }
    }
    const char* rel = path + base_len;
    if (*rel == '/' || *rel == '\\') { rel++; }
    return strdup(rel);
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
