#include "dh/main.h"

#include "dh/Arr.h"
#include "dh/heap/Fixed.h"
#include "dh/mem/Allocator.h"

TEST_fn_("Basic Allocator Reallocation Usage", $guard) {
    var_(buffer, Arr$$(1024, u8)) = Arr_zero();
    var_(fixed, heap_Fixed)       = heap_Fixed_init(Sli_arr$(Sli$u8, buffer));

    let allocator = heap_Fixed_allocator(&fixed);
    var sli       = meta_cast$(Sli$u8, try_(mem_Allocator_alloc(allocator, typeInfo$(u8), 10)));
    defer_(mem_Allocator_free(allocator, anySli(sli)));

    try_(TEST_expect(sli.ptr != null));
    try_(TEST_expect(sli.len == 10));
    try_(TEST_expect(heap_Fixed_isLastAllocation(&fixed, sli.as_const)));

    for_slice_indexed (sli, item, idx) { deref(item) = idx; }
    let res = meta_cast$(Sli$u8, try_(mem_Allocator_realloc(allocator, anySli(sli), 20)));
    try_(TEST_expect(res.ptr != null));
    try_(TEST_expect(res.len == 20));
    try_(TEST_expect(sli.ptr == res.ptr));
    try_(TEST_expect(sli.len < res.len));
    try_(TEST_expect(!heap_Fixed_isLastAllocation(&fixed, sli.as_const)));
    try_(TEST_expect(heap_Fixed_isLastAllocation(&fixed, res.as_const)));

    sli = res;
    try_(TEST_expect(sli.ptr != null));
    try_(TEST_expect(sli.len == 20));
    try_(TEST_expect(heap_Fixed_isLastAllocation(&fixed, sli.as_const)));

    for (usize i = 0; i < 10; ++i) { try_(TEST_expect(Sli_getAt(sli, i) == i)); }
} $unguarded_TEST_fn;
