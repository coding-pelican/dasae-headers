#include "dh/core.h"

int main(void) {
    let a = srcLoc();
    printf("file_path: %s\n", a.file_path);
    printf("file_name: %s\n", a.file_name);
    printf("fn_name:   %s\n", a.fn_name);
    printf("line:      %d\n", a.line);
    printf("column:    %d\n", a.column);
    printf("%s:%d:%d\n", a.file_path, a.line, a.column);
    return 0;
}
