#include "Allocator.h"

Err$void dh_main(int argc, const char* argv[]) {
    reserveReturn(Err$void);
    unused(argc), unused(argv);

    extern mem_AllocatorVT mem_AllocatorVT;

    let allocator = (mem_Allocator){
        .ctx = null,
        .vt  = &mem_AllocatorVT,
    };

    let int_ptr = meta_castPtr(isize*, try(mem_Allocator_create(allocator, typeInfo(isize))));
    let int_sli = meta_castSli(Sli$isize, try(mem_Allocator_alloc(allocator, typeInfo(isize), 10)));
}
