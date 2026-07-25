#include <stdio.h>
#include <stdlib.h>

static void write_env(FILE* log, const char* name) {
    const char* value = getenv(name);
    (void)fprintf(log, "%s=%s\n", name, value ? value : "");
}

int main(int argc, char* argv[]) {
    const char* log_path = getenv("DH_TEST_PROVIDER_LOG");
    if (!log_path || !log_path[0]) {
        return 2;
    }

    FILE* log = fopen(log_path, "a");
    if (!log) {
        return 3;
    }

    (void)fputs("args=", log);
    for (int i = 1; i < argc; ++i) {
        if (i > 1) {
            (void)fputc(' ', log);
        }
        (void)fputs(argv[i], log);
    }
    (void)fputc('\n', log);
    write_env(log, "DH_DEP_TARGET");
    write_env(log, "DH_DEP_CC");
    write_env(log, "DH_DEP_AR");
    write_env(log, "DH_DEP_SYSROOT");
    write_env(log, "DH_DEP_CFLAGS");

    return fclose(log) == 0 ? 0 : 4;
}
