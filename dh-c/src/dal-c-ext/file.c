#include "dal-c-ext/file.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

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
    // Count lines
    int line_count = 0;
    int ch = 0;
    int prev_ch = '\n';
    while ((ch = fgetc(fp)) != EOF) {
        if (prev_ch == '\n') { line_count++; }
        prev_ch = ch;
    }
    if (line_count == 0) {
        (void)fclose(fp);
        *count = 0;
        return NULL;
    }
    // Allocate array
    char** const lines = (char**)malloc((size_t)line_count * sizeof(char*));
    if (!lines) {
        (void)fclose(fp);
        *count = 0;
        return NULL;
    }
    // Read lines
    if (fseek(fp, 0, SEEK_SET) != 0) {
        (void)fclose(fp);
        free((void*)lines);
        *count = 0;
        return NULL;
    }
    int idx = 0;
    char buffer[4096] = {};
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Remove trailing newline
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        if (len > 0 && buffer[len - 1] == '\r') {
            buffer[len - 1] = '\0';
        }
        lines[idx] = strdup(buffer);
        idx++;
    }
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
