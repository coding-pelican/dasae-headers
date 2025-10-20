#include "dh/main.h"

#include "dh/heap/Classic.h"
#include "dh/mem/Allocator.h"

fn_((dh_main(Sli$Sli_const$u8 args))(Err$void) $scope) {
    let_ignore       = args;
    let allocator = heap_Classic_allocator(&(heap_Classic){});
    let_ignore       = meta_cast$(Sli$u8, try_(mem_Allocator_alloc(allocator, typeInfo$(u8), 1024)));
    return_ok({});
} $unscoped_(fn);
