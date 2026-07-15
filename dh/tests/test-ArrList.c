#include "dh-main.h"
#include "dh/ArrList.h"
#include "dh/heap/Sys.h"

T_use$((u32)(
    ArrList,
    ArrList_empty,
    ArrList_fixed,
    ArrList_init,
    ArrList_fini,
    ArrList_clone,
    ArrList_len,
    ArrList_cap,
    ArrList_at,
    ArrList_atMut,
    ArrList_front,
    ArrList_back,
    ArrList_items,
    ArrList_itemsMut,
    ArrList_itemsCapped,
    ArrList_itemsUnused
));
T_use$((u32)(
    ArrList_ensureCapPrecise,
    ArrList_ensureUnusedCap,
    ArrList_resize,
    ArrList_shrinkRetainingCap,
    ArrList_shrinkAndFree,
    ArrList_clearRetainingCap,
    ArrList_clearAndFree
));
T_use$((u32)(
    ArrList_appendFixed,
    ArrList_appendSFixed,
    ArrList_appendNFixed,
    ArrList_insertFixed,
    ArrList_insertSFixed,
    ArrList_prependFixed,
    ArrList_prependSFixed
));
T_use$((u32)(
    ArrList_replace,
    ArrList_replaceFixed,
    ArrList_replaceWithin,
    ArrList_pop,
    ArrList_removeOrdd,
    ArrList_removeSwap,
    ArrList_shift
));

$static var_(test_ArrList_failing_ctx, Void) $undefined_static;
$static let_(test_ArrList_failing_vtbl, mem_Alctr_VTbl) = {
    .allocFn = mem_Alctr_VTbl_noAlloc,
    .resizeFn = mem_Alctr_VTbl_noResize,
    .remapFn = mem_Alctr_VTbl_noRemap,
    .freeFn = mem_Alctr_VTbl_noFree,
};
$static let_(test_ArrList_failing_alctr, mem_Alctr) = {
    .ctx = &test_ArrList_failing_ctx,
    .vtbl = &test_ArrList_failing_vtbl,
};

TEST_fn_("ArrList: empty and fixed views define length capacity and endpoints" $scope) {
    var empty = ArrList_empty$u32();
    try_(TEST_expect(ArrList_len$u32(empty) == 0));
    try_(TEST_expect(ArrList_cap$u32(empty) == 0));
    try_(TEST_expect(ArrList_items$u32(empty).len == 0));
    try_(TEST_expect(ArrList_itemsCapped$u32(empty).len == 0));
    try_(TEST_expect(ArrList_itemsUnused$u32(empty).len == 0));
    try_(TEST_expect(isNone(ArrList_front$u32(empty))));
    try_(TEST_expect(isNone(ArrList_back$u32(empty))));

    var buf = A_from$((u32){ 10, 20, 30 });
    var list = ArrList_fixed$u32(A_ref$((S$u32)(buf)));
    try_(TEST_expect(ArrList_len$u32(list) == 0));
    try_(TEST_expect(ArrList_cap$u32(list) == 3));
    try_(TEST_expect(ArrList_items$u32(list).len == 0));
    try_(TEST_expect(ArrList_itemsCapped$u32(list).len == 3));
    try_(TEST_expect(ArrList_itemsUnused$u32(list).len == 3));
} $unscoped(TEST_fn);

TEST_fn_("ArrList: fixed append prepend and insert define contiguous order" $scope) {
    var buf = A_from$((u32){ 0, 0, 0, 0, 0, 0, 0, 0 });
    var list = ArrList_fixed$u32(A_ref$((S$u32)(buf)));
    let middle = A_from$((u32){ 4, 5 });
    let front = A_from$((u32){ 1, 2 });

    try_(ArrList_appendFixed$u32(&list, 3));
    try_(ArrList_appendSFixed$u32(&list, A_ref$((S_const$u32)(middle))));
    try_(ArrList_prependSFixed$u32(&list, A_ref$((S_const$u32)(front))));
    try_(ArrList_insertFixed$u32(&list, 5, 8));
    try_(ArrList_insertSFixed$u32(&list, 5, A_ref$((S_const$u32)(A_from$((u32){ 6, 7 })))));

    try_(TEST_expect(ArrList_len$u32(list) == 8));
    for (usize i = 0; i < 8; i++) {
        try_(TEST_expect(*ArrList_at$u32(list, i) == as$(u32)(i + 1)));
    }
    try_(TEST_expect(*unwrap_(ArrList_front$u32(list)) == 1));
    try_(TEST_expect(*unwrap_(ArrList_back$u32(list)) == 8));
} $unscoped(TEST_fn);

TEST_fn_("ArrList: fixed capacity errors do not mutate length or contents" $scope) {
    var buf = A_from$((u32){ 0, 0, 0 });
    var list = ArrList_fixed$u32(A_ref$((S$u32)(buf)));
    try_(ArrList_appendNFixed$u32(&list, 7, 3));

    let append_result = ArrList_appendFixed$u32(&list, 8);
    let prepend_result = ArrList_prependFixed$u32(&list, 6);
    let insert_result = ArrList_insertFixed$u32(&list, 1, 9);
    try_(TEST_expect(isErr(append_result)));
    try_(TEST_expect(isErr(prepend_result)));
    try_(TEST_expect(isErr(insert_result)));
    try_(TEST_expect(ArrList_len$u32(list) == 3));
    try_(TEST_expect(*ArrList_at$u32(list, 0) == 7));
    try_(TEST_expect(*ArrList_at$u32(list, 1) == 7));
    try_(TEST_expect(*ArrList_at$u32(list, 2) == 7));
} $unscoped(TEST_fn);

TEST_fn_("ArrList: replace handles equal shorter and longer ranges" $scope) {
    var buf = A_from$((u32){ 0, 0, 0, 0, 0, 0, 0, 0 });
    var list = ArrList_fixed$u32(A_ref$((S$u32)(buf)));
    try_(ArrList_appendSFixed$u32(&list, A_ref$((S_const$u32)(A_from$((u32){ 1, 2, 3, 4 })))));

    ArrList_replaceWithin$u32(&list, $r(1, 3), A_ref$((S_const$u32)(A_from$((u32){ 20, 30 }))));
    try_(TEST_expect(ArrList_len$u32(list) == 4));
    try_(TEST_expect(*ArrList_at$u32(list, 0) == 1));
    try_(TEST_expect(*ArrList_at$u32(list, 1) == 20));
    try_(TEST_expect(*ArrList_at$u32(list, 2) == 30));
    try_(TEST_expect(*ArrList_at$u32(list, 3) == 4));

    try_(ArrList_replaceFixed$u32(&list, $r(1, 3), A_ref$((S_const$u32)(A_from$((u32){ 2 })))));
    try_(TEST_expect(ArrList_len$u32(list) == 3));
    try_(TEST_expect(*ArrList_at$u32(list, 0) == 1));
    try_(TEST_expect(*ArrList_at$u32(list, 1) == 2));
    try_(TEST_expect(*ArrList_at$u32(list, 2) == 4));

    try_(ArrList_replaceFixed$u32(&list, $r(1, 2), A_ref$((S_const$u32)(A_from$((u32){ 2, 3, 5 })))));
    try_(TEST_expect(ArrList_len$u32(list) == 5));
    try_(TEST_expect(*ArrList_at$u32(list, 0) == 1));
    try_(TEST_expect(*ArrList_at$u32(list, 1) == 2));
    try_(TEST_expect(*ArrList_at$u32(list, 2) == 3));
    try_(TEST_expect(*ArrList_at$u32(list, 3) == 5));
    try_(TEST_expect(*ArrList_at$u32(list, 4) == 4));
} $unscoped(TEST_fn);

TEST_fn_("ArrList: pop shift ordered remove and swap remove define removal order" $scope) {
    var buf = A_from$((u32){ 0, 0, 0, 0, 0 });
    var list = ArrList_fixed$u32(A_ref$((S$u32)(buf)));
    try_(ArrList_appendSFixed$u32(&list, A_ref$((S_const$u32)(A_from$((u32){ 1, 2, 3, 4, 5 })))));

    try_(TEST_expect(unwrap_(ArrList_pop$u32(&list)) == 5));
    try_(TEST_expect(unwrap_(ArrList_shift$u32(&list)) == 1));
    try_(TEST_expect(ArrList_removeOrdd$u32(&list, 1) == 3));
    try_(TEST_expect(ArrList_len$u32(list) == 2));
    try_(TEST_expect(*ArrList_at$u32(list, 0) == 2));
    try_(TEST_expect(*ArrList_at$u32(list, 1) == 4));

    try_(ArrList_appendFixed$u32(&list, 9));
    try_(TEST_expect(ArrList_removeSwap$u32(&list, 0) == 2));
    try_(TEST_expect(ArrList_len$u32(list) == 2));
    try_(TEST_expect(*ArrList_at$u32(list, 0) == 9));
    try_(TEST_expect(*ArrList_at$u32(list, 1) == 4));
    try_(TEST_expect(unwrap_(ArrList_pop$u32(&list)) == 4));
    try_(TEST_expect(unwrap_(ArrList_pop$u32(&list)) == 9));
    try_(TEST_expect(isNone(ArrList_pop$u32(&list))));
    try_(TEST_expect(isNone(ArrList_shift$u32(&list))));
} $unscoped(TEST_fn);

TEST_fn_("ArrList: mutable access mutates the stored item in place" $scope) {
    var buf = A_from$((u32){ 0, 0, 0 });
    var list = ArrList_fixed$u32(A_ref$((S$u32)(buf)));
    try_(ArrList_appendSFixed$u32(&list, A_ref$((S_const$u32)(A_from$((u32){ 1, 2, 3 })))));

    *ArrList_atMut$u32(list, 1) = 20;
    ArrList_itemsMut$u32(list).ptr[2] = 30;

    try_(TEST_expect(*ArrList_at$u32(list, 0) == 1));
    try_(TEST_expect(*ArrList_at$u32(list, 1) == 20));
    try_(TEST_expect(*ArrList_at$u32(list, 2) == 30));
} $unscoped(TEST_fn);

TEST_fn_("ArrList: dynamic capacity operations preserve contents and define clear semantics" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var list = try_(ArrList_init$u32(gpa, 2));
    defer_(ArrList_fini$u32(&list, gpa));

    try_(ArrList_appendSFixed$u32(&list, A_ref$((S_const$u32)(A_from$((u32){ 1, 2 })))));
    try_(ArrList_ensureCapPrecise$u32(&list, gpa, 6));
    try_(TEST_expect(ArrList_cap$u32(list) >= 6));
    try_(ArrList_ensureUnusedCap$u32(&list, gpa, 4));
    try_(TEST_expect(ArrList_cap$u32(list) >= 6));
    try_(TEST_expect(*ArrList_at$u32(list, 0) == 1));
    try_(TEST_expect(*ArrList_at$u32(list, 1) == 2));

    try_(ArrList_resize$u32(&list, gpa, 5));
    try_(TEST_expect(ArrList_len$u32(list) == 5));
    *ArrList_atMut$u32(list, 2) = 3;
    *ArrList_atMut$u32(list, 3) = 4;
    *ArrList_atMut$u32(list, 4) = 5;
    ArrList_shrinkRetainingCap$u32(&list, 3);
    try_(TEST_expect(ArrList_len$u32(list) == 3));
    try_(TEST_expect(ArrList_cap$u32(list) >= 6));
    try_(TEST_expect(*ArrList_at$u32(list, 2) == 3));

    let retained_cap = ArrList_cap$u32(list);
    ArrList_clearRetainingCap$u32(&list);
    try_(TEST_expect(ArrList_len$u32(list) == 0));
    try_(TEST_expect(ArrList_cap$u32(list) == retained_cap));
} $unguarded(TEST_fn);

TEST_fn_("ArrList: clone copies items and preserves independent storage" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var list = try_(ArrList_init$u32(gpa, 3));
    defer_(ArrList_fini$u32(&list, gpa));
    try_(ArrList_appendSFixed$u32(&list, A_ref$((S_const$u32)(A_from$((u32){ 1, 2, 3 })))));

    var cloned = try_(ArrList_clone$u32(list, gpa));
    defer_(ArrList_fini$u32(&cloned, gpa));
    *ArrList_atMut$u32(cloned, 1) = 20;

    try_(TEST_expect(ArrList_len$u32(cloned) == 3));
    try_(TEST_expect(ArrList_cap$u32(cloned) == ArrList_cap$u32(list)));
    try_(TEST_expect(*ArrList_at$u32(list, 1) == 2));
    try_(TEST_expect(*ArrList_at$u32(cloned, 1) == 20));
} $unguarded(TEST_fn);

TEST_fn_("ArrList: shrinkAndFree shrinks length preserves prefix and does not grow capacity" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var list = try_(ArrList_init$u32(gpa, 8));
    defer_(ArrList_fini$u32(&list, gpa));
    try_(ArrList_appendSFixed$u32(&list, A_ref$((S_const$u32)(A_from$((u32){ 1, 2, 3, 4 })))));

    let old_cap = ArrList_cap$u32(list);
    ArrList_shrinkAndFree$u32(&list, gpa, 2);
    try_(TEST_expect(ArrList_len$u32(list) == 2));
    try_(TEST_expect(ArrList_cap$u32(list) >= 2));
    try_(TEST_expect(ArrList_cap$u32(list) <= old_cap));
    try_(TEST_expect(*ArrList_at$u32(list, 0) == 1));
    try_(TEST_expect(*ArrList_at$u32(list, 1) == 2));

    ArrList_shrinkAndFree$u32(&list, gpa, 0);
    try_(TEST_expect(ArrList_len$u32(list) == 0));
    try_(TEST_expect(ArrList_cap$u32(list) == 0));
} $unguarded(TEST_fn);

TEST_fn_("ArrList: clearAndFree resets to empty no-capacity state" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var list = try_(ArrList_init$u32(gpa, 4));
    try_(ArrList_appendSFixed$u32(&list, A_ref$((S_const$u32)(A_from$((u32){ 1, 2 })))));

    ArrList_clearAndFree$u32(&list, gpa);
    try_(TEST_expect(ArrList_len$u32(list) == 0));
    try_(TEST_expect(ArrList_cap$u32(list) == 0));
    try_(TEST_expect(isNone(ArrList_front$u32(list))));
    try_(TEST_expect(isNone(ArrList_back$u32(list))));
} $unguarded(TEST_fn);

TEST_fn_("ArrList: replace is failure atomic on growth failure" $scope) {
    var buf = A_from$((u32){ 1, 2 });
    var list = ArrList_fixed$u32(A_ref$((S$u32)(buf)));
    try_(ArrList_appendNFixed$u32(&list, 0, 2));
    let original = A_from$((u32){ 1, 2 });
    ArrList_replaceWithin$u32(&list, $r(0, 2), A_ref$((S_const$u32)(original)));

    let replacement = A_from$((u32){ 9, 10 });
    let result = ArrList_replace$u32(&list, test_ArrList_failing_alctr, $r(0, 1), A_ref$((S_const$u32)(replacement)));
    try_(TEST_expect(isErr(result)));

    try_(TEST_expect(ArrList_len$u32(list) == 2));
    try_(TEST_expect(*ArrList_at$u32(list, 0) == 1));
    try_(TEST_expect(*ArrList_at$u32(list, 1) == 2));
} $unscoped(TEST_fn);

TEST_fn_("ArrList: fixed APIs reject length overflow before mutation" $scope) {
    var buf = A_from$((u32){ 0 });
    var list = ArrList_fixed$u32(A_ref$((S$u32)(buf)));
    list.as_raw->items.len = usize_limit_max;

    let append_n_result = ArrList_appendNFixed$u32(&list, 9, 1);
    let append_result = ArrList_appendFixed$u32(&list, 9);
    try_(TEST_expect(isErr(append_n_result)));
    try_(TEST_expect(isErr(append_result)));
    try_(TEST_expect(list.as_raw->items.len == usize_limit_max));
} $unscoped(TEST_fn);

TEST_fn_("ArrList: appendNFixed fills appended range" $scope) {
    var buf = A_from$((u32){ 0, 0, 0 });
    var list = ArrList_fixed$u32(A_ref$((S$u32)(buf)));

    try_(ArrList_appendNFixed$u32(&list, 7, 3));

    try_(TEST_expect(ArrList_len$u32(list) == 3));
    try_(TEST_expect(*ArrList_at$u32(list, 0) == 7));
    try_(TEST_expect(*ArrList_at$u32(list, 1) == 7));
    try_(TEST_expect(*ArrList_at$u32(list, 2) == 7));
} $unscoped(TEST_fn);
