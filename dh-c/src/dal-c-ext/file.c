#include "dal-c-ext/file.h"
#include "dal-c-ext/str.h"
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#endif

static void file__freeLines(char** lines, int count) {
    if (!lines) { return; }
    for (int i = 0; i < count; ++i) {
        free(lines[i]);
    }
    free((void*)lines);
}

static bool file__pushLine(char*** lines, int* count, int* capacity, const char* line) {
    assert(lines != NULL);
    assert(count != NULL);
    assert(capacity != NULL);
    assert(line != NULL);

    if (*count >= *capacity) {
        const int new_capacity = (*capacity == 0) ? 16 : (*capacity * 2);
        char** const grown = (char**)realloc((void*)*lines, (size_t)new_capacity * sizeof(char*));
        if (!grown) {
            return false;
        }
        *lines = grown;
        *capacity = new_capacity;
    }

    (*lines)[*count] = strdup(line);
    if (!(*lines)[*count]) {
        return false;
    }
    (*count)++;
    return true;
}

char* file_read(const char* path) {
    if (!path) { return NULL; }
    FILE* const fp = fopen(path, "rb");
    if (!fp) { return NULL; }

    // Get file size
    (void)fseek(fp, 0, SEEK_END);
    const long size = ftell(fp);
    (void)fseek(fp, 0, SEEK_SET);
    if (size < 0) { return fclose(fp), NULL; }

    // Allocate buffer
    char* const content = (char*)malloc((size_t)size + 1);
    if (!content) { return fclose(fp), NULL; }

    // Read file
    const size_t read_size = fread(content, 1, (size_t)size, fp);
    if (read_size != (size_t)size && ferror(fp) != 0) {
        free(content);
        return fclose(fp), NULL;
    }
    content[read_size] = '\0';
    return fclose(fp), content;
}

char** file_readLines(const char* path, int* count) {
    assert(count != NULL);
    if (!path || !count) {
        *count = 0;
        return NULL;
    }

    FILE* const fp = fopen(path, "r");
    if (!fp) {
        *count = 0;
        return NULL;
    }

    char** lines = NULL;
    int line_count = 0;
    int line_capacity = 0;

    char* line_buf = NULL;
    size_t line_len = 0;
    size_t line_capacity_bytes = 0;

    int ch = 0;
    while ((ch = fgetc(fp)) != EOF) {
        if (line_len + 1 >= line_capacity_bytes) {
            const size_t new_capacity = (line_capacity_bytes == 0) ? 128u : (line_capacity_bytes * 2u);
            char* const grown = (char*)realloc(line_buf, new_capacity);
            if (!grown) {
                free(line_buf);
                file__freeLines(lines, line_count);
                (void)fclose(fp);
                *count = 0;
                return NULL;
            }
            line_buf = grown;
            line_capacity_bytes = new_capacity;
        }

        if (ch == '\n') {
            if (line_len > 0 && line_buf[line_len - 1] == '\r') {
                line_len--;
            }
            line_buf[line_len] = '\0';
            if (!file__pushLine(&lines, &line_count, &line_capacity, line_buf)) {
                free(line_buf);
                file__freeLines(lines, line_count);
                (void)fclose(fp);
                *count = 0;
                return NULL;
            }
            line_len = 0;
            continue;
        }

        line_buf[line_len++] = (char)ch;
    }

    if (ferror(fp) != 0) {
        free(line_buf);
        file__freeLines(lines, line_count);
        (void)fclose(fp);
        *count = 0;
        return NULL;
    }

    if (line_len > 0 || line_count == 0) {
        if (line_len > 0 && line_buf[line_len - 1] == '\r') {
            line_len--;
        }
        if (line_buf) {
            line_buf[line_len] = '\0';
        }
        if (line_len > 0 || line_count == 0) {
            const char* final_line = line_buf ? line_buf : "";
            if (!(line_count == 0 && line_len == 0)) {
                if (!file__pushLine(&lines, &line_count, &line_capacity, final_line)) {
                    free(line_buf);
                    file__freeLines(lines, line_count);
                    (void)fclose(fp);
                    *count = 0;
                    return NULL;
                }
            }
        }
    }

    free(line_buf);
    (void)fclose(fp);
    *count = line_count;
    return lines;
}

bool file_write(const char* path, const char* content) {
    if (!path || !content) { return false; }
    FILE* const fp = fopen(path, "wb");
    if (!fp) { return false; }
    const size_t len = strlen(content);
    const size_t written = fwrite(content, 1, len, fp);
    const bool success = (written == len);
    return fclose(fp), success;
}

static char* file__tmpPathFor(const char* path) {
    assert(path != NULL);
#ifdef _WIN32
    return str_format("%s.tmp.%lu.%lu", path, (unsigned long)GetCurrentProcessId(), (unsigned long)GetCurrentThreadId());
#else
    return str_format("%s.tmp.%ld", path, (long)getpid());
#endif
}

static bool file__replacePath(const char* src, const char* dst) {
    assert(src != NULL);
    assert(dst != NULL);
#ifdef _WIN32
    return MoveFileExA(src, dst, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
#else
    return rename(src, dst) == 0;
#endif
}

bool file_writeAtomic(const char* path, const char* content) {
    if (!path || !content) { return false; }
    char* tmp_path = file__tmpPathFor(path);
    if (!tmp_path) { return false; }
    bool success = file_write(tmp_path, content) && file__replacePath(tmp_path, path);
    if (!success) {
        (void)remove(tmp_path);
    }
    free(tmp_path);
    return success;
}

bool file_copy(const char* src, const char* dst) {
    if (!src || !dst) { return false; }
    FILE* const src_fp = fopen(src, "rb");
    if (!src_fp) { return false; }
    FILE* const dst_fp = fopen(dst, "wb");
    if (!dst_fp) { return fclose(src_fp), false; }

    char buffer[2048] = {};
    size_t bytes_read = 0;
    bool success = true;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src_fp)) > 0) {
        const size_t bytes_written = fwrite(buffer, 1, bytes_read, dst_fp);
        if (bytes_written != bytes_read) {
            success = false;
            break;
        }
    }
    (void)fclose(src_fp);
    (void)fclose(dst_fp);
    return success;
}

bool file_lockAcquire(file_Lock* lock, const char* path) {
    if (!lock || !path) { return false; }
    memset(lock, 0, sizeof(*lock));
    lock->path = strdup(path);
    if (!lock->path) { return false; }
#ifdef _WIN32
    bool waiting_reported = false;
    for (;;) {
        HANDLE handle = CreateFileA(
            path,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        if (handle != INVALID_HANDLE_VALUE) {
            lock->handle = handle;
            return true;
        }
        DWORD err = GetLastError();
        if (err != ERROR_SHARING_VIOLATION && err != ERROR_LOCK_VIOLATION) {
            free(lock->path);
            memset(lock, 0, sizeof(*lock));
            return false;
        }
        if (!waiting_reported) {
            (void)fprintf(stderr, "Waiting for build lock: %s\n", path);
            waiting_reported = true;
        }
        Sleep(50);
    }
#else
    int fd = open(path, O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        free(lock->path);
        memset(lock, 0, sizeof(*lock));
        return false;
    }
    if (flock(fd, LOCK_EX | LOCK_NB) != 0) {
        if (errno != EWOULDBLOCK && errno != EAGAIN) {
            close(fd);
            free(lock->path);
            memset(lock, 0, sizeof(*lock));
            return false;
        }
        (void)fprintf(stderr, "Waiting for build lock: %s\n", path);
        if (flock(fd, LOCK_EX) != 0) {
            close(fd);
            free(lock->path);
            memset(lock, 0, sizeof(*lock));
            return false;
        }
    }
    lock->fd = fd;
    return true;
#endif
}

void file_lockRelease(file_Lock* lock) {
    if (!lock) { return; }
#ifdef _WIN32
    if (lock->handle) {
        CloseHandle((HANDLE)lock->handle);
    }
#else
    if (lock->fd >= 0) {
        (void)flock(lock->fd, LOCK_UN);
        (void)close(lock->fd);
    }
#endif
    free(lock->path);
    memset(lock, 0, sizeof(*lock));
}

time_t file_ctime(const char* path) {
    if (!path) { return 0; }
    struct stat st = { 0 };
    if (stat(path, &st) != 0) { return 0; }
    return st.st_ctime;
}

time_t file_mtime(const char* path) {
    if (!path) { return 0; }
    struct stat st = { 0 };
    if (stat(path, &st) != 0) { return 0; }
    return st.st_mtime;
}

time_t file_atime(const char* path) {
    if (!path) { return 0; }
    struct stat st = { 0 };
    if (stat(path, &st) != 0) { return 0; }
    return st.st_atime;
}
