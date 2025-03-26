#include "dh/core.h"
#include "dh/main.h"
#include "dh/Str.h"
#include <stdio.h>
#include <string.h>

Err$void dh_main(Sli$Str_const args) {
    unused(args);
    reserveReturn(Err$void);

    printf("passed args:\n");
    for_slice_indexed (args, arg, index) {
        printf("  arg %llu: %s\n", index, arg->ptr);
    }
    printf("\n");

    char buffer[32] = cleared();
    ignore fgets(buffer, 32, stdin);
    let str = Str_fromZ(as$(u8*, buffer));
    for_slice_rev (str, ch) {
        ignore putchar(*ch);
    }

    return_ok({});
}
