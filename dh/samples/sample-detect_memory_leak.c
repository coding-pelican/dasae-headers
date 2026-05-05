#include "dh-main.h"
#include "dh/heap/Page.h"

T_use_E$($set(mem_E)(S$u8));
T_use$((u8)(mem_Alctr_alloc));
fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    let gpa = heap_Page_alctr(&l0$((heap_Page)));
    let_ignore = try_(mem_Alctr_alloc$u8($trace gpa, 1024));
    return_ok({});
} $unscoped(fn);
