#include "dh-main.h"
#include "dh/ListDbl.h"

T_use$((u32)(ListDbl_Link, ListDbl_Adp, ListDbl));
T_use$((u32)(
    ListDbl_Adp_init,
    ListDbl_empty,
    ListDbl_insertNext,
    ListDbl_insertPrev,
    ListDbl_concatByMoving,
    ListDbl_append,
    ListDbl_prepend,
    ListDbl_remove,
    ListDbl_pop,
    ListDbl_shift,
    ListDbl_len,
    ListDbl_Link_data
));

TEST_fn_("ListDbl: insert remove and traverse both directions" $guard) {
    var list = ListDbl_empty$u32();
    var one = ListDbl_Adp_init$u32(1);
    var two = ListDbl_Adp_init$u32(2);
    var three = ListDbl_Adp_init$u32(3);
    var four = ListDbl_Adp_init$u32(4);
    var five = ListDbl_Adp_init$u32(5);

    ListDbl_append$u32(&list, &two.link);
    ListDbl_append$u32(&list, &five.link);
    ListDbl_prepend$u32(&list, &one.link);
    ListDbl_insertPrev$u32(&list, &five.link, &four.link);
    ListDbl_insertNext$u32(&list, &two.link, &three.link);

    try_(TEST_expect(ListDbl_len$u32(&list) == 5));

    using_(var it = list.first) {
        var expected = as$(u32)(1);
        while_some(it, node) loop_defer {
            defer_(expected += 1, it = node->next);
            try_(TEST_expect(*ListDbl_Link_data$u32(node) == expected));
        } loop_deferral;
        try_(TEST_expect(expected == 6));
    }

    using_(var it = list.last) {
        var expected = as$(u32)(5);
        while_some(it, node) loop_defer {
            defer_(expected -= 1, it = node->prev);
            try_(TEST_expect(*ListDbl_Link_data$u32(node) == expected));
        } loop_deferral;
        try_(TEST_expect(expected == 0));
    }

    try_(TEST_expect(*ListDbl_Link_data$u32(unwrap_(ListDbl_shift$u32(&list))) == 1));
    try_(TEST_expect(*ListDbl_Link_data$u32(unwrap_(ListDbl_pop$u32(&list))) == 5));
    ListDbl_remove$u32(&list, &three.link);

    try_(TEST_expect(ListDbl_len$u32(&list) == 2));
    try_(TEST_expect(*ListDbl_Link_data$u32(unwrap_(list.first)) == 2));
    try_(TEST_expect(*ListDbl_Link_data$u32(unwrap_(list.last)) == 4));
    try_(TEST_expect(isNone(three.link.prev)));
    try_(TEST_expect(isNone(three.link.next)));
} $unguarded(TEST_fn);

TEST_fn_("ListDbl: concat moves nodes and clears source" $guard) {
    var list1 = ListDbl_empty$u32();
    var list2 = ListDbl_empty$u32();
    var one = ListDbl_Adp_init$u32(1);
    var two = ListDbl_Adp_init$u32(2);
    var three = ListDbl_Adp_init$u32(3);
    var four = ListDbl_Adp_init$u32(4);
    var five = ListDbl_Adp_init$u32(5);

    ListDbl_append$u32(&list1, &one.link);
    ListDbl_append$u32(&list1, &two.link);
    ListDbl_append$u32(&list2, &three.link);
    ListDbl_append$u32(&list2, &four.link);
    ListDbl_append$u32(&list2, &five.link);

    ListDbl_concatByMoving$u32(&list1, &list2);

    try_(TEST_expect(ListDbl_len$u32(&list1) == 5));
    try_(TEST_expect(ListDbl_len$u32(&list2) == 0));
    try_(TEST_expect(isNone(list2.first)));
    try_(TEST_expect(isNone(list2.last)));
    try_(TEST_expect(*ListDbl_Link_data$u32(unwrap_(list1.last)) == 5));

    using_(var it = list1.first) {
        var expected = as$(u32)(1);
        while_some(it, node) loop_defer {
            defer_(expected += 1, it = node->next);
            try_(TEST_expect(*ListDbl_Link_data$u32(node) == expected));
        } loop_deferral;
        try_(TEST_expect(expected == 6));
    }

    ListDbl_concatByMoving$u32(&list2, &list1);

    try_(TEST_expect(ListDbl_len$u32(&list1) == 0));
    try_(TEST_expect(ListDbl_len$u32(&list2) == 5));
    try_(TEST_expect(isNone(list1.first)));
    try_(TEST_expect(isNone(list1.last)));
    try_(TEST_expect(*ListDbl_Link_data$u32(unwrap_(list2.first)) == 1));
    try_(TEST_expect(*ListDbl_Link_data$u32(unwrap_(list2.last)) == 5));
} $unguarded(TEST_fn);

TEST_fn_("ListDbl: empty and single-node operations define endpoint state" $scope) {
    var list = ListDbl_empty$u32();
    var one = ListDbl_Adp_init$u32(1);

    try_(TEST_expect(ListDbl_len$u32(&list) == 0));
    try_(TEST_expect(isNone(ListDbl_pop$u32(&list))));
    try_(TEST_expect(isNone(ListDbl_shift$u32(&list))));

    ListDbl_append$u32(&list, &one.link);
    try_(TEST_expect(ListDbl_len$u32(&list) == 1));
    try_(TEST_expect(*ListDbl_Link_data$u32(unwrap_(list.first)) == 1));
    try_(TEST_expect(*ListDbl_Link_data$u32(unwrap_(list.last)) == 1));

    ListDbl_remove$u32(&list, &one.link);
    try_(TEST_expect(ListDbl_len$u32(&list) == 0));
    try_(TEST_expect(isNone(list.first)));
    try_(TEST_expect(isNone(list.last)));
    try_(TEST_expect(isNone(one.link.prev)));
    try_(TEST_expect(isNone(one.link.next)));
} $unscoped(TEST_fn);

TEST_fn_("ListDbl: concat with empty lists keeps ownership obvious" $scope) {
    var dst = ListDbl_empty$u32();
    var src = ListDbl_empty$u32();
    var one = ListDbl_Adp_init$u32(1);

    ListDbl_concatByMoving$u32(&dst, &src);
    try_(TEST_expect(ListDbl_len$u32(&dst) == 0));
    try_(TEST_expect(ListDbl_len$u32(&src) == 0));

    ListDbl_append$u32(&src, &one.link);
    ListDbl_concatByMoving$u32(&dst, &src);
    try_(TEST_expect(ListDbl_len$u32(&dst) == 1));
    try_(TEST_expect(ListDbl_len$u32(&src) == 0));
    try_(TEST_expect(isNone(src.first)));
    try_(TEST_expect(isNone(src.last)));
    try_(TEST_expect(*ListDbl_Link_data$u32(unwrap_(dst.first)) == 1));
    try_(TEST_expect(*ListDbl_Link_data$u32(unwrap_(dst.last)) == 1));
} $unscoped(TEST_fn);
