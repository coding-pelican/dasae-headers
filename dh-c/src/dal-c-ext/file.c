#include "dal-c-ext/file.h"
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

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

bool file_copy(const char* src, const char* dst) {
    if (!src || !dst) { return false; }
    FILE* const src_fp = fopen(src, "rb");
    if (!src_fp) { return false; }
    FILE* const dst_fp = fopen(dst, "wb");
    if (!dst_fp) { return fclose(src_fp), false; }

    char buffer[4096] = {};
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
