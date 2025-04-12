#define main_no_args (1)
#include "dh/main.h"

#include "dh/heap/Classic.h"
#include "dh/mem/Allocator.h"

fn_scope(dh_main(void), Err$void) {
    let allocator = heap_Classic_allocator(&(heap_Classic){});
    let mem       = meta_cast$(Sli$u8, try_(mem_Allocator_alloc(allocator, typeInfo$(u8), 1024)));
    mem_Allocator_free(allocator, anySli(mem));
    mem_Allocator_free(allocator, anySli(mem)); // Double free
    return_ok({});
} unscoped;
