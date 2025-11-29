#include "dh/main.h"
#include "dh/heap/Page.h"

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    let gpa = heap_Page_allocator(&(heap_Page){});
    let_ignore = u_castS$((S$u8)(try_(mem_Allocator_alloc(gpa, typeInfo$(InnerType), 1024))));
    return_ok({});
} $unscoped_(fn);
