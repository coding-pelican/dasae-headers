#include "dh/main.h"

#include "dh/heap/Classic.h"
#include "dh/mem/Allocator.h"

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore    = args;
    let allocator = heap_Classic_allocator(&(heap_Classic){});
    let mem       = meta_cast$(S$u8, try_(mem_Allocator_alloc(allocator, typeInfo$(u8), 1024)));
    mem_Allocator_free(allocator, anySli(mem));
    mem_Allocator_free(allocator, anySli(mem)); // Double free
    return_ok({});
} $unscoped_(fn);
