#include "dh/core.h"
#include "dh/main.h"
#include "dh/Str.h"
#include <stdio.h>
#include <string.h>

Err$void dh_main(int argc, const char* argv[]) {
    unused(argc), unused(argv);
    reserveReturn(Err$void);

    char   buffer[32] = cleared();
    ignore fgets(buffer, 32, stdin);
    let    str = Str_from(as$(u8*, buffer), strlen(buffer));
    for_slice_rev(str, ch) {
        ignore putchar(*ch);
    }

    return_ok({});
}
