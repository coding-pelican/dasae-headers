#define main_no_args (1)
#include "dh/main.h"

fn_(dh_main(void), Err$void, $scope) {
    let loc = srcLoc();
    printf("file_path: %s\n", loc.file_path);
    printf("file_name: %s\n", loc.file_name);
    printf("fn_name:   %s\n", loc.fn_name);
    printf("line:      %d\n", loc.line);
    printf("column:    %d\n", loc.column);
    printf("%s:%d:%d\n", loc.file_path, loc.line, loc.column);
    return_ok({});
} $unscoped;
