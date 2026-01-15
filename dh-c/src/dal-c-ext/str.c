#include "dal-c-ext/str.h"
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

bool str_eql(const char* lhs, const char* rhs) {
    if (!lhs || !rhs) { return lhs == rhs; }
    return strcmp(lhs, rhs) == 0;
}

bool str_startsWith(const char* str, const char* prefix) {
    if (!str || !prefix) { return false; }
    const size_t prefix_len = strlen(prefix);
    if (prefix_len == 0) { return true; }
    return strncmp(str, prefix, prefix_len) == 0;
}

bool str_endsWith(const char* str, const char* suffix) {
    if (!str || !suffix) { return false; }
    const size_t str_len = strlen(str);
    const size_t suffix_len = strlen(suffix);
    if (suffix_len > str_len) { return false; }
    return strcmp(str + str_len - suffix_len, suffix) == 0;
}

char* str_join(const char* sep, const char** parts, int count) {
    if (!sep || !parts || count <= 0) { return strdup(""); }
    // Calculate total length
    const size_t sep_len = strlen(sep);
    size_t total_len = 0;
    for (int i = 0; i < count; ++i) {
        if (parts[i]) { total_len += strlen(parts[i]); }
        if (i < count - 1) { total_len += sep_len; }
    }
    // Allocate and build string
    char* const result = (char*)malloc(total_len + 1);
    if (!result) { return NULL; }
    result[0] = '\0';
    for (int i = 0; i < count; ++i) {
        if (parts[i]) { strcat(result, parts[i]); }
        if (i < count - 1) { strcat(result, sep); }
    }
    return result;
}

char** str_split(const char* str, const char* delim, int* count) {
    assert(count != NULL);
    if (!str || !delim) {
        *count = 0;
        return NULL;
    }
    *count = 0;
    const size_t delim_len = strlen(delim);
    if (delim_len == 0) { return NULL; }
    // Count occurrences
    const char* pos = str;
    while ((pos = strstr(pos, delim)) != NULL) {
        (*count)++;
        pos += delim_len;
    }
    (*count)++; // One more for the last part
    // Allocate array
    char** const result = (char**)malloc((size_t)(*count) * sizeof(char*));
    if (!result) {
        *count = 0;
        return NULL;
    }
    // Split string
    pos = str;
    for (int i = 0; i < *count; ++i) {
        const char* next = strstr(pos, delim);
        if (next) {
            const size_t len = (size_t)(next - pos);
            result[i] = (char*)malloc(len + 1);
            if (result[i]) {
                strncpy(result[i], pos, len);
                result[i][len] = '\0';
            }
            pos = next + delim_len;
        } else {
            result[i] = strdup(pos);
        }
    }
    return result;
}

char* str_trim(const char* str) {
    if (!str) { return NULL; }
    // Find start (skip leading whitespace)
    const char* start = str;
    while (*start && isspace((unsigned char)*start)) { start++; }
    // Empty string
    if (*start == '\0') { return strdup(""); }
    // Find end (skip trailing whitespace)
    const char* end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) { end--; }
    // Copy trimmed string
    const size_t len = (size_t)(end - start + 1);
    char* const result = (char*)malloc(len + 1);
    if (!result) { return NULL; }
    strncpy(result, start, len);
    result[len] = '\0';
    return result;
}

char* str_toUpper(const char* str) {
    if (!str) { return NULL; }
    const size_t len = strlen(str);
    char* const result = (char*)malloc(len + 1);
    if (!result) { return NULL; }
    for (size_t i = 0; i < len; ++i) {
        result[i] = (char)toupper((unsigned char)str[i]);
    }
    result[len] = '\0';
    return result;
}

char* str_format(const char* fmt, ...) {
    if (!fmt) { return NULL; }
    va_list args = NULL;
    va_start(args, fmt);

    // Determine size needed
    va_list args_copy = NULL;
    va_copy(args_copy, args);
    int size = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);
    if (size < 0) { return va_end(args), NULL; }

    // Allocate and format
    char* const result = (char*)malloc((size_t)size + 1);
    if (!result) { return va_end(args), NULL; }

    (void)vsnprintf(result, (size_t)size + 1, fmt, args);
    return va_end(args), result;
}
