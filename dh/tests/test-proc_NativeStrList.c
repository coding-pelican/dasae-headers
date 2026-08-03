#include "dh-main.h"
#include "dh/proc/NativeStrList.h"
#include "dh/heap/Sys.h"
#include "dh/mem/common.h"

TEST_fn_("proc/NativeStrList: owned native strings preserve item boundaries" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var items = A_from$((S_const$u8){
        u8_l("alpha"),
        u8_l("beta=two"),
    });
    var list = try_(proc_NativeStrList_from(
        gpa, A_ref$((S$S_const$u8)(items))
    ));
    defer_(proc_NativeStrList_fini(&list, gpa));

    try_(TEST_expect(proc_NativeStrList_len(list) == 2));
    try_(TEST_expect(mem_eqlBytes(
        proc_NativeStrList_at(list, 0), u8_l("alpha")
    )));
    try_(TEST_expect(mem_eqlBytes(
        proc_NativeStrList_at(list, 1), u8_l("beta=two")
    )));
} $unguarded(TEST_fn);
