#include "dh/main.h"
#include "dh/heap/Page.h"
#include "dh/heap/Smp.h"

TEST_fn_("SmpAllocator with custom parent" $guard) {
    $static var_(thrd_metas, A$$(16, heap_Smp_ThrdMeta)) = A_zero();
    var page = lit0$((heap_Page));
    var smp = heap_Smp_from(heap_Page_allocator(&page), A_ref$((S$heap_Smp_ThrdMeta)(thrd_metas)));
    let gpa = heap_Smp_allocator(&smp);

    // Test basic allocation
    let slice1 = u_castS$((S$u8)(try_(mem_Allocator_alloc(gpa, typeInfo$(u8), 100))));
    defer_(mem_Allocator_free(gpa, u_anyS(slice1)));

    // Test large allocation (delegates to parent)
    let slice2 = u_castS$((S$u8)(try_(mem_Allocator_alloc(gpa, typeInfo$(u8), 1024ull * 1024))));
    defer_(mem_Allocator_free(gpa, u_anyS(slice2)));

    try_(TEST_expect(slice1.len == 100));
    try_(TEST_expect(slice2.len == 1024ull * 1024));
} $unguarded_(TEST_fn);

TEST_fn_("SmpAllocator heap allocation" $guard) {
    var page = lit0$((heap_Page));
    var smp = try_(heap_Smp_createOnHeap(heap_Page_allocator(&page), heap_Smp_max_thrd_count));
    defer_(heap_Smp_destroyOnHeap(&smp));
    let gpa = heap_Smp_allocator(smp);

    // Test heap allocation for large thread counts
    let slice = u_castS$((S$u8)(try_(mem_Allocator_alloc(gpa, typeInfo$(u8), 100))));
    defer_(mem_Allocator_free(gpa, u_anyS(slice)));

    try_(TEST_expect(slice.len == 100));
} $unguarded_(TEST_fn);
