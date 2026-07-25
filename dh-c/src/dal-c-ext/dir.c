#include "dal-c-ext/dir.h"
#include "dal-c-ext/path.h"
#include "dal-c-ext/str.h"
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

static void dir__freePathArray(char** files, int count) {
    if (!files) { return; }
    for (int i = 0; i < count; ++i) {
        free(files[i]);
    }
    free((void*)files);
}

static bool dir__pushPath(char*** files, int* count, int* capacity, char* path) {
    assert(files != NULL);
    assert(count != NULL);
    assert(capacity != NULL);
    assert(path != NULL);

    if (*count >= *capacity) {
        const int new_capacity = (*capacity == 0) ? 16 : (*capacity * 2);
        char** const grown = (char**)realloc((void*)*files, (size_t)new_capacity * sizeof(char*));
        if (!grown) {
            return false;
        }
        *files = grown;
        *capacity = new_capacity;
    }

    (*files)[*count] = path;
    (*count)++;
    return true;
}

static bool dir__appendPaths(char*** dst, int* dst_count, int* dst_capacity, char** src, int src_count) {
    assert(dst != NULL);
    assert(dst_count != NULL);
    assert(dst_capacity != NULL);

    for (int i = 0; i < src_count; ++i) {
        if (!dir__pushPath(dst, dst_count, dst_capacity, src[i])) {
            return false;
        }
        src[i] = NULL;
    }
    return true;
}


bool dir_create(const char* path) {
    if (!path) { return false; }
#ifdef _WIN32
    const int result = _mkdir(path);
    if (result == 0) { return true; }
    return (errno == EEXIST);
#else
    const int result = mkdir(path, 0755);
    if (result == 0) { return true; }
    return (errno == EEXIST);
#endif
}

bool dir_linkDir(const char* link_path, const char* target_path) {
    if (!link_path || !target_path) { return false; }
#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(link_path);
    if (attrs != INVALID_FILE_ATTRIBUTES) { return true; }
    DWORD flags = SYMBOLIC_LINK_FLAG_DIRECTORY;
#ifdef SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE
    flags |= SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE;
#endif
    return CreateSymbolicLinkA(link_path, target_path, flags) != 0;
#else
    struct stat st = {0};
    if (lstat(link_path, &st) == 0) { return S_ISLNK(st.st_mode); }
    return symlink(target_path, link_path) == 0;
#endif
}

static bool dir_isRootPath(const char* path) {
    if (!path) { return false; }
#ifdef _WIN32
    size_t const len = strlen(path);
    if (len == 2 && isalpha((unsigned char)path[0]) && path[1] == ':') {
        return true;
    }
    if (len == 3 && isalpha((unsigned char)path[0]) && path[1] == ':'
        && (path[2] == '/' || path[2] == '\\')) {
        return true;
    }
    return false;
#else
    return str_eql(path, "/");
#endif
}

#ifdef _WIN32
static bool dir__deleteFilePath(const char* path) {
    assert(path != NULL);
    for (int attempt = 0; attempt < 8; ++attempt) {
        if (DeleteFileA(path)) { return true; }
        const DWORD err = GetLastError();
        if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND) { return true; }
        if (err == ERROR_ACCESS_DENIED) {
            const DWORD attrs = GetFileAttributesA(path);
            if (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_READONLY)) {
                (void)SetFileAttributesA(path, attrs & ~(DWORD)FILE_ATTRIBUTE_READONLY);
            }
        }
        Sleep(10);
    }
    return DeleteFileA(path) != 0;
}

static bool dir__removeEmptyDirPath(const char* path) {
    assert(path != NULL);
    for (int attempt = 0; attempt < 8; ++attempt) {
        if (RemoveDirectoryA(path)) { return true; }
        const DWORD err = GetLastError();
        if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND) { return true; }
        if (err == ERROR_ACCESS_DENIED) {
            const DWORD attrs = GetFileAttributesA(path);
            if (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_READONLY)) {
                (void)SetFileAttributesA(path, attrs & ~(DWORD)FILE_ATTRIBUTE_READONLY);
            }
        }
        Sleep(10);
    }
    return RemoveDirectoryA(path) != 0;
}
#endif

/* NOLINTNEXTLINE(misc-no-recursion) */
bool dir_createRecur(const char* path) {
    if (!path) { return false; }
    if (dir_isRootPath(path)) { return true; }
    // Check if already exists
    if (path_isDir(path)) { return true; }
    // Create parent first
    char* const parent = path_parent(path);
    if (parent && !str_eql(parent, path) && !str_eql(parent, ".") && !dir_isRootPath(parent)) {
        if (!dir_createRecur(parent)) { return free(parent), false; }
    }
    if (parent) { free(parent); }
    // Create this directory
    return dir_create(path);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool dir_removeRecur(const char* path) {
    if (!path) { return false; }
    if (!path_isDir(path)) { return false; }
#ifdef _WIN32
    // Windows: Use SHFileOperation or manual recursion
    WIN32_FIND_DATAA find_data = {};
    char search_path[MAX_PATH] = {};
    (void)snprintf(search_path, MAX_PATH, "%s\\*", path);
    HANDLE const hFind = FindFirstFileA(search_path, &find_data);
    bool success = true;
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0) { continue; }
            char* const full_path = path_join(path, find_data.cFileName);
            if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (!dir_removeRecur(full_path)) {
                    success = false;
                }
            } else {
                if (!dir__deleteFilePath(full_path)) {
                    success = false;
                }
            }
            free(full_path);
        } while (FindNextFileA(hFind, &find_data));
        FindClose(hFind);
    }
    return success && dir__removeEmptyDirPath(path);
#else
    DIR* const dir = opendir(path);
    if (!dir) { return false; }
    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) { continue; }
        char* full_path = path_join(path, entry->d_name);
        if (path_isDir(full_path)) {
            dir_removeRecur(full_path);
        } else {
            remove(full_path);
        }
        free(full_path);
    }
    closedir(dir);
    return rmdir(path) == 0;
#endif
}

char** dir_list(const char* path, int* count) {
    assert(count != NULL);
    if (!path) {
        *count = 0;
        return NULL;
    }
    *count = 0;
    char** files = NULL;
    int capacity = 0;
#ifdef _WIN32
    WIN32_FIND_DATAA find_data = {};
    char search_path[MAX_PATH] = {};
    (void)snprintf(search_path, MAX_PATH, "%s\\*", path);
    HANDLE const hFind = FindFirstFileA(search_path, &find_data);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0) { continue; }
            char* const full_path = path_join(path, find_data.cFileName);
            if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                if (!dir__pushPath(&files, count, &capacity, full_path)) {
                    free(full_path);
                    dir__freePathArray(files, *count);
                    FindClose(hFind);
                    *count = 0;
                    return NULL;
                }
            } else {
                free(full_path);
            }
        } while (FindNextFileA(hFind, &find_data));
        FindClose(hFind);
    }
#else
    DIR* const dir = opendir(path);
    if (!dir) { return NULL; }
    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) { continue; }
        char* const full_path = path_join(path, entry->d_name);
        if (!path_isDir(full_path)) {
            if (!dir__pushPath(&files, count, &capacity, full_path)) {
                free(full_path);
                dir__freePathArray(files, *count);
                (void)closedir(dir);
                *count = 0;
                return NULL;
            }
        } else {
            free(full_path);
        }
    }
    (void)closedir(dir);
#endif
    return files;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
char** dir_listRecur(const char* path, int* count) {
    assert(count != NULL);
    if (!path) {
        *count = 0;
        return NULL;
    }
    *count = 0;
    char** files = NULL;
    int capacity = 0;
#ifdef _WIN32
    WIN32_FIND_DATAA find_data = {};
    char search_path[MAX_PATH] = {};
    (void)snprintf(search_path, MAX_PATH, "%s\\*", path);
    HANDLE const hFind = FindFirstFileA(search_path, &find_data);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0) { continue; }
            char* const full_path = path_join(path, find_data.cFileName);
            if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                // Recurse into subdirectory
                int sub_count = 0;
                char** const sub_files = dir_listRecur(full_path, &sub_count);
                if (sub_files) {
                    if (!dir__appendPaths(&files, count, &capacity, sub_files, sub_count)) {
                        dir__freePathArray(sub_files, sub_count);
                        dir__freePathArray(files, *count);
                        free(full_path);
                        FindClose(hFind);
                        *count = 0;
                        return NULL;
                    }
                    free((void*)sub_files);
                }
                free(full_path);
            } else {
                // Add file
                if (!dir__pushPath(&files, count, &capacity, full_path)) {
                    free(full_path);
                    dir__freePathArray(files, *count);
                    FindClose(hFind);
                    *count = 0;
                    return NULL;
                }
            }
        } while (FindNextFileA(hFind, &find_data));
        FindClose(hFind);
    }
#else
    DIR* const dir = opendir(path);
    if (!dir) { return NULL; }
    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) { continue; }
        char* const full_path = path_join(path, entry->d_name);
        if (path_isDir(full_path)) {
            // Recurse into subdirectory
            int sub_count = 0;
            char** const sub_files = dir_listRecur(full_path, &sub_count);
            if (sub_files) {
                if (!dir__appendPaths(&files, count, &capacity, sub_files, sub_count)) {
                    dir__freePathArray(sub_files, sub_count);
                    dir__freePathArray(files, *count);
                    free(full_path);
                    (void)closedir(dir);
                    *count = 0;
                    return NULL;
                }
                free((void*)sub_files);
            }
            free(full_path);
        } else {
            // Add file
            if (!dir__pushPath(&files, count, &capacity, full_path)) {
                free(full_path);
                dir__freePathArray(files, *count);
                (void)closedir(dir);
                *count = 0;
                return NULL;
            }
        }
    }
    (void)closedir(dir);
#endif
    return files;
}
