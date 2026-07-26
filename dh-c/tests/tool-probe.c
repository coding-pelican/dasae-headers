#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int append_log(const char* path, int argc, char* argv[]) {
    if (!path || !path[0]) {
        return 0;
    }
    FILE* log = fopen(path, "a");
    if (!log) {
        return 2;
    }
    (void)fprintf(log, "argc=%d\n", argc - 1);
    for (int i = 1; i < argc; ++i) {
        (void)fprintf(log, "arg=%s\n", argv[i]);
    }
    return fclose(log) == 0 ? 0 : 3;
}

int main(int argc, char* argv[]) {
    const char* log_path = getenv("DH_TEST_TOOL_LOG");
    int log_result = append_log(log_path, argc, argv);
    if (log_result != 0) {
        return log_result;
    }

    const char* target = getenv("DH_TEST_TOOL_TARGET");
    if (target && target[0] && argc >= 2 &&
        (strcmp(argv[1], "--print-target-triple") == 0 || strcmp(argv[1], "-dumpmachine") == 0)) {
        (void)puts(target);
        return 0;
    }

    const char* touch_arg = getenv("DH_TEST_TOOL_TOUCH_ARG");
    if (touch_arg && touch_arg[0]) {
        char* end = NULL;
        long index = strtol(touch_arg, &end, 10);
        if (!end || *end != '\0' || index <= 0 || index >= argc) {
            return 4;
        }
        FILE* output = fopen(argv[index], "ab");
        if (!output) {
            return 5;
        }
        if (fclose(output) != 0) {
            return 6;
        }
    }

    return 0;
}
