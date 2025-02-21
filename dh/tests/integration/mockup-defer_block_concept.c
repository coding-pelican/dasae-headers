// NOLINTBEGIN(bugprone-terminating-continue)
#define scope_defer        \
    int _defer_return = 0; \
    int _defer_curr   = 0; \
    _deferred:             \
    switch (_defer_curr) { \
    default:               \
        break;             \
    case 0:                \
        _defer_curr = -1;

#define run_deferred() \
    goto _deferred;    \
    }                  \
    while (0)

#define defer__snapshot(A)             \
    {                                  \
        int _defer_prev = _defer_curr; \
        _defer_curr     = __LINE__;    \
        if (0) {                       \
        case __LINE__:                 \
            _defer_curr = _defer_prev; \
            A;                         \
        }                              \
    }

#define defer(F) \
    defer__snapshot(F; goto _deferred)

#define block_defer          \
    do {                     \
    defer__snapshot(         \
        if (_defer_return) { \
            goto _deferred;  \
        } else {             \
            continue;        \
        }                    \
    )

#define defer_break     \
    {                   \
        goto _deferred; \
    }

#define defer_return       \
    {                      \
        _defer_return = 1; \
        goto _deferred;    \
    }
// NOLINTEND(bugprone-terminating-continue)

#include <stdio.h>

int main(void) {
    int result = 0;
    printf("0\n");
    scope_defer {
        printf("1\n");
        FILE* log_file = fopen("log.txt", "r");
        if (!log_file) {
            result = -1;
            printf("2\n");
            defer_return;
            printf("3\n");
        }
        printf("4\n");
        defer({
            printf("5\n");
            (void)fclose(log_file); }
        );
        printf("6\n");

        block_defer {
            printf("7\n");
            FILE* info_log_file = fopen("info_log.txt", "w");
            if (!info_log_file) {
                result = -1;
                printf("8\n");
                defer_return;
                printf("9\n");
            }
            printf("10\n");
            defer({
                printf("11\n");
                (void)fclose(info_log_file);
            });
            printf("12\n");
            FILE* warning_log_file = fopen("warning_log.txt", "w");
            if (!warning_log_file) {
                result = -1;
                printf("13\n");
                defer_return;
                printf("14\n");
            }
            printf("15\n");
            defer({
                printf("16\n");
                (void)fclose(warning_log_file);
            });
            printf("17\n");
        }
        run_deferred();
        printf("18\n");

        FILE* error_log_file = fopen("error_log.txt", "w");
        if (!error_log_file) {
            result = -1;
            printf("19\n");
            defer_return;
            printf("20\n");
        }
        defer({
            printf("21\n");
            (void)fclose(error_log_file);
        });
        printf("22\n");
    }
    run_deferred();
    printf("23\n");

    return result;
}

/*
#define block_defer
#define block_defer_return return
#define defer

// output order
//  0
//      1 3 7
//          4 6
//              5
//          2
//  8
void defer_usage() {
    printf("0\n");
    block_defer {
        printf("1\n");
        defer printf("2\n");
        printf("3\n");
        defer {
            printf("4\n");
            defer printf("5\n");
            printf("6\n");
        }
        printf("7\n");
    }
    printf("8\n");
}
*/
