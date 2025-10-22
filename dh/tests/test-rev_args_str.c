#include "dh/main.h"
#include "dh/Str.h"
#include "dh/Arr.h"

#include <stdio.h>

fn_((dh_main(Sli$Sli_const$u8 args))(Err$void $scope)) {
    let_ignore = args;

    printf("passed args:\n");
    for_slice_indexed (args, arg, index) {
        printf("  arg %llu: %s\n", index, arg->ptr);
    }
    printf("\n");

    var_(input, Arr$$(32, u8)) = Arr_zero();
    let_ignore                 = fgets(as$((char*)(input.buf)), Arr_len(input), stdin);
    let str                    = Str_fromZ(input.buf);
    for_slice_rev (str, ch) {
        let_ignore = putchar(*ch);
    }

    return_ok({});
} $unscoped_(fn);
