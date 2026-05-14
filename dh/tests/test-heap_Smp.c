#include "dh-main.h"
#include "dh/heap/Sys.h"
#include "dh/heap/Smp.h"

TEST_fn_("heap/Smp: PageAlctr provides slab alignment for Smp backing allocations" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    let align = mem_alignToLog2(heap_Smp_slab_len);
    let ptr = orelse_((mem_Alctr_rawAlloc($trace gpa, heap_Smp_slab_len, align))(return_err(E_cause$Unexpected())));
    defer_(mem_Alctr_rawFree($trace gpa, l$((S$u8){ .ptr = ptr, .len = heap_Smp_slab_len }), align));

    try_(TEST_expect(mem_isAligned(ptrToInt(ptr), heap_Smp_slab_len)));
} $unguarded(TEST_fn);

TEST_fn_("heap/Smp: with custom parent" $guard) {
    $static var_(thrd_metas, A$$(16, heap_Smp_ThrdMeta)) = A_zero();
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var smp = heap_Smp_from(heap_Sys_alctr(&heap), A_ref$((S$heap_Smp_ThrdMeta)(thrd_metas)));
    let gpa = heap_Smp_alctr(&smp);

    // Test basic allocation
    let slice1 = u_castS$((S$u8)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(u8), 100))));
    defer_(mem_Alctr_free($trace gpa, u_anyS(slice1)));

    // Test large allocation (delegates to parent)
    let slice2 = u_castS$((S$u8)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(u8), 1024ull * 1024))));
    defer_(mem_Alctr_free($trace gpa, u_anyS(slice2)));

    try_(TEST_expect(slice1.len == 100));
    try_(TEST_expect(slice2.len == 1024ull * 1024));
} $unguarded(TEST_fn);

TEST_fn_("heap/Smp: heap allocation" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var smp = try_(heap_Smp_createOnHeap(heap_Sys_alctr(&heap), heap_Smp_max_thrd_count));
    defer_(heap_Smp_destroyOnHeap(&smp));
    let gpa = heap_Smp_alctr(smp);

    // Test heap allocation for large thread counts
    let slice = u_castS$((S$u8)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(u8), 100))));
    defer_(mem_Alctr_free($trace gpa, u_anyS(slice)));

    try_(TEST_expect(slice.len == 100));
} $unguarded(TEST_fn);
