#include "dal-c-ext/dir.h"
#include "dal-c-ext/path.h"
#include "dal-c-ext/str.h"
#include <stdio.h>
#include <assert.h>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#endif


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

/* NOLINTNEXTLINE(misc-no-recursion) */
bool dir_createRecur(const char* path) {
    if (!path) { return false; }
    // Check if already exists
    if (path_isDir(path)) { return true; }
    // Create parent first
    char* const parent = path_parent(path);
    if (parent && !str_eql(parent, path) && !str_eql(parent, ".")) {
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
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0) { continue; }
            char* const full_path = path_join(path, find_data.cFileName);
            if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                dir_removeRecur(full_path);
            } else {
                DeleteFileA(full_path);
            }
            free(full_path);
        } while (FindNextFileA(hFind, &find_data));
        FindClose(hFind);
    }
    return RemoveDirectoryA(path) != 0;
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
                    for (int i = 0; i < sub_count; ++i) {
                        if (*count >= capacity) {
                            capacity = capacity == 0 ? 16 : capacity * 2;
                            char** const temp = (char**)realloc((void*)files, (size_t)capacity * sizeof(char*));
                            if (!temp) {
                                free((void*)files);
                                free((void*)sub_files);
                                free(full_path);
                                FindClose(hFind);
                                *count = 0;
                                return NULL;
                            }
                            files = temp;
                        }
                        files[*count] = sub_files[i];
                        (*count)++;
                    }
                    free((void*)sub_files);
                }
            } else {
                // Add file
                if (*count >= capacity) {
                    capacity = capacity == 0 ? 16 : capacity * 2;
                    char** const temp = (char**)realloc((void*)files, (size_t)capacity * sizeof(char*));
                    if (!temp) {
                        free((void*)files);
                        free(full_path);
                        FindClose(hFind);
                        *count = 0;
                        return NULL;
                    }
                    files = temp;
                }
                files[*count] = full_path;
                (*count)++;
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
                for (int i = 0; i < sub_count; ++i) {
                    if (*count >= capacity) {
                        capacity = capacity == 0 ? 16 : capacity * 2;
                        char** const temp = (char**)realloc((void*)files, (size_t)capacity * sizeof(char*));
                        if (!temp) {
                            free((void*)files);
                            free((void*)sub_files);
                            free(full_path);
                            (void)closedir(dir);
                            *count = 0;
                            return NULL;
                        }
                        files = temp;
                    }
                    files[*count] = sub_files[i];
                    (*count)++;
                }
                free((void*)sub_files);
            }
        } else {
            // Add file
            if (*count >= capacity) {
                capacity = capacity == 0 ? 16 : capacity * 2;
                char** const temp = (char**)realloc((void*)files, (size_t)capacity * sizeof(char*));
                if (!temp) {
                    free((void*)files);
                    free(full_path);
                    (void)closedir(dir);
                    *count = 0;
                    return NULL;
                }
                files = temp;
            }
            files[*count] = full_path;
            (*count)++;
        }
    }
    (void)closedir(dir);
#endif
    return files;
}
