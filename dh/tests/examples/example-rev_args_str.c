#include "dh/main.h"
#include "dh/Str.h"
#include "dh/Arr.h"

#include <stdio.h>

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    printf("passed args:\n");
    for_($rev($s(args), $rf(0))(arg, index) {
        printf("  arg %llu: %s\n", index, arg->ptr);
    });
    printf("\n");

    var_(input, A$$(32, u8)) = zero$A();
    let_ignore = fgets(as$((char*)(&input)), len$A(input), stdin);
    let str = Str_fromZ(input.val);
    for_($rev($s(str))(ch) {
        let_ignore = putchar(*ch);
    });

    return_ok({});
} $unscoped_(fn);
