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
    if (attrs != INVALID_FILE_ATTRIBUTES) {
        if ((attrs & FILE_ATTRIBUTE_DIRECTORY) == 0 || (attrs & FILE_ATTRIBUTE_REPARSE_POINT) == 0) {
            return false; /* Never replace a user-owned real file or directory. */
        }
        if (RemoveDirectoryA(link_path) == 0) {
            return false;
        }
    }
    DWORD flags = SYMBOLIC_LINK_FLAG_DIRECTORY;
#ifdef SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE
    flags |= SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE;
#endif
    if (CreateSymbolicLinkA(link_path, target_path, flags) != 0) {
        return true;
    }

    /* Developer Mode or elevation may be unavailable. A directory junction keeps
     * build/native usable without changing the build contract. Resolve a relative
     * target against the link's parent because mklink resolves it against CWD. */
    char* link_parent = path_parent(link_path);
    char* joined_target = link_parent ? path_join(link_parent, target_path) : strdup(target_path);
    char target_full[MAX_PATH * 4] = { 0 };
    const char* junction_target = joined_target;
    if (joined_target && GetFullPathNameA(joined_target, (DWORD)sizeof(target_full), target_full, NULL) > 0) {
        junction_target = target_full;
    }
    char* command = str_format(
        "cmd.exe /D /C mklink /J \"%s\" \"%s\"",
        link_path,
        junction_target ? junction_target : target_path
    );
    STARTUPINFOA startup = { 0 };
    PROCESS_INFORMATION process = { 0 };
    startup.cb = sizeof(startup);
    BOOL started = command && CreateProcessA(NULL, command, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &startup, &process);
    int result = -1;
    if (started) {
        (void)WaitForSingleObject(process.hProcess, INFINITE);
        DWORD exit_code = 1;
        (void)GetExitCodeProcess(process.hProcess, &exit_code);
        result = (int)exit_code;
        (void)CloseHandle(process.hThread);
        (void)CloseHandle(process.hProcess);
    }
    free(command);
    free(joined_target);
    free(link_parent);
    return result == 0 && path_isDir(link_path);
#else
    struct stat st = {0};
    if (lstat(link_path, &st) == 0) {
        if (!S_ISLNK(st.st_mode)) { return false; }
        char current[PATH_MAX + 1] = {0};
        ssize_t current_len = readlink(link_path, current, PATH_MAX);
        if (current_len >= 0) {
            current[current_len] = '\0';
            if (str_eql(current, target_path)) { return true; }
        }
        if (unlink(link_path) != 0) { return false; }
    }
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
#ifdef _WIN32
    DWORD root_attrs = GetFileAttributesA(path);
    if (root_attrs == INVALID_FILE_ATTRIBUTES || (root_attrs & FILE_ATTRIBUTE_DIRECTORY) == 0) {
        return false;
    }
    if (root_attrs & FILE_ATTRIBUTE_REPARSE_POINT) {
        return dir__removeEmptyDirPath(path); /* Remove the junction/symlink, never its target. */
    }
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
                bool removed = (find_data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
                             ? dir__removeEmptyDirPath(full_path)
                             : dir_removeRecur(full_path);
                if (!removed) {
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
    struct stat root_st = {0};
    if (lstat(path, &root_st) != 0) { return false; }
    if (S_ISLNK(root_st.st_mode)) { return unlink(path) == 0; }
    if (!S_ISDIR(root_st.st_mode)) { return false; }
    DIR* const dir = opendir(path);
    if (!dir) { return false; }
    bool success = true;
    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) { continue; }
        char* full_path = path_join(path, entry->d_name);
        struct stat child_st = {0};
        if (full_path && lstat(full_path, &child_st) == 0) {
            if (S_ISLNK(child_st.st_mode)) {
                if (unlink(full_path) != 0) { success = false; }
            } else if (S_ISDIR(child_st.st_mode)) {
                if (!dir_removeRecur(full_path)) { success = false; }
            } else if (remove(full_path) != 0) {
                success = false;
            }
        } else {
            success = false;
        }
        free(full_path);
    }
    closedir(dir);
    return success && rmdir(path) == 0;
#endif
}

char** dir_listEntries(const char* path, int* count) {
    assert(count != NULL);
    if (!path) {
        *count = 0;
        return NULL;
    }
    *count = 0;
    char** entries = NULL;
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
            if (!dir__pushPath(&entries, count, &capacity, full_path)) {
                free(full_path);
                dir__freePathArray(entries, *count);
                FindClose(hFind);
                *count = 0;
                return NULL;
            }
        } while (FindNextFileA(hFind, &find_data));
        FindClose(hFind);
    }
#else
    DIR* const dir = opendir(path);
    if (dir) {
        struct dirent* entry = NULL;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) { continue; }
            char* const full_path = path_join(path, entry->d_name);
            if (!dir__pushPath(&entries, count, &capacity, full_path)) {
                free(full_path);
                dir__freePathArray(entries, *count);
                closedir(dir);
                *count = 0;
                return NULL;
            }
        }
        closedir(dir);
    }
#endif
    return entries;
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
