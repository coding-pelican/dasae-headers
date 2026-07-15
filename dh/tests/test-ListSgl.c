#include "dh-main.h"
#include "dh/ListSgl.h"

T_use$((u32)(ListSgl_Link, ListSgl_Adp, ListSgl));
T_use$((u32)(
    ListSgl_Link_empty,
    ListSgl_Link_insertNext,
    ListSgl_Link_removeNext,
    ListSgl_Link_findLast,
    ListSgl_Link_countChildren,
    ListSgl_Link_reverse,
    ListSgl_Link_adp,
    ListSgl_Link_adpMut,
    ListSgl_Link_data,
    ListSgl_Link_dataMut,
    ListSgl_Adp_empty,
    ListSgl_Adp_init,
    ListSgl_Adp_link,
    ListSgl_Adp_linkMut,
    ListSgl_Adp_data,
    ListSgl_Adp_dataMut,
    ListSgl_empty,
    ListSgl_prepend,
    ListSgl_remove,
    ListSgl_shift,
    ListSgl_len
));

TEST_fn_("ListSgl: empty list and adapter initialization define optional state" $scope) {
    var list = ListSgl_empty$u32();
    var empty_adp = ListSgl_Adp_empty$u32();
    var init_adp = ListSgl_Adp_init$u32(42);
    var raw_link = ListSgl_Link_empty$u32();

    try_(TEST_expect(ListSgl_len$u32(&list) == 0));
    try_(TEST_expect(isNone(list.first)));
    try_(TEST_expect(isNone(ListSgl_shift$u32(&list))));

    try_(TEST_expect(*ListSgl_Adp_data$u32(&empty_adp) == 0));
    try_(TEST_expect(*ListSgl_Adp_data$u32(&init_adp) == 42));
    try_(TEST_expect(ListSgl_Adp_link$u32(&init_adp) == &init_adp.link));
    try_(TEST_expect(ListSgl_Adp_linkMut$u32(&init_adp) == &init_adp.link));
    try_(TEST_expect(isNone(empty_adp.link.next)));
    try_(TEST_expect(isNone(init_adp.link.next)));
    try_(TEST_expect(isNone(raw_link.next)));
} $unscoped(TEST_fn);

TEST_fn_("ListSgl: prepend insertNext traversal count and findLast define order" $guard) {
    var list = ListSgl_empty$u32();
    var one = ListSgl_Adp_init$u32(1);
    var two = ListSgl_Adp_init$u32(2);
    var three = ListSgl_Adp_init$u32(3);
    var four = ListSgl_Adp_init$u32(4);
    var five = ListSgl_Adp_init$u32(5);

    ListSgl_prepend$u32(&list, &two.link);
    ListSgl_Link_insertNext$u32(&two.link, &five.link);
    ListSgl_prepend$u32(&list, &one.link);
    ListSgl_Link_insertNext$u32(&two.link, &three.link);
    ListSgl_Link_insertNext$u32(&three.link, &four.link);

    try_(TEST_expect(ListSgl_len$u32(&list) == 5));
    try_(TEST_expect(ListSgl_Link_countChildren$u32(unwrap_(list.first)) == 4));
    try_(TEST_expect(*ListSgl_Link_data$u32(ListSgl_Link_findLast$u32(unwrap_(list.first))) == 5));

    using_(var it = list.first) {
        var expected = as$(u32)(1);
        while_some(it, node) loop_defer {
            defer_(expected += 1, it = node->next);
            try_(TEST_expect(*ListSgl_Link_data$u32(node) == expected));
        } loop_deferral;
        try_(TEST_expect(expected == 6));
    }
} $unguarded(TEST_fn);

TEST_fn_("ListSgl: remove shift and removeNext detach removed nodes" $guard) {
    var list = ListSgl_empty$u32();
    var one = ListSgl_Adp_init$u32(1);
    var two = ListSgl_Adp_init$u32(2);
    var three = ListSgl_Adp_init$u32(3);
    var four = ListSgl_Adp_init$u32(4);
    var five = ListSgl_Adp_init$u32(5);

    ListSgl_prepend$u32(&list, &five.link);
    ListSgl_prepend$u32(&list, &four.link);
    ListSgl_prepend$u32(&list, &three.link);
    ListSgl_prepend$u32(&list, &two.link);
    ListSgl_prepend$u32(&list, &one.link);

    try_(TEST_expect(*ListSgl_Link_data$u32(unwrap_(ListSgl_shift$u32(&list))) == 1));
    try_(TEST_expect(isNone(one.link.next)));
    try_(TEST_expect(*ListSgl_Link_data$u32(unwrap_(list.first)) == 2));

    ListSgl_remove$u32(&list, &five.link);
    try_(TEST_expect(isNone(five.link.next)));
    try_(TEST_expect(*ListSgl_Link_data$u32(ListSgl_Link_findLast$u32(unwrap_(list.first))) == 4));

    let removed = unwrap_(ListSgl_Link_removeNext$u32(&two.link));
    try_(TEST_expect(removed == &three.link));
    try_(TEST_expect(isNone(three.link.next)));
    try_(TEST_expect(*ListSgl_Link_data$u32(unwrap_(two.link.next)) == 4));
    try_(TEST_expect(isNone(ListSgl_Link_removeNext$u32(&four.link))));

    try_(TEST_expect(ListSgl_len$u32(&list) == 2));
    try_(TEST_expect(*ListSgl_Link_data$u32(unwrap_(list.first)) == 2));
    try_(TEST_expect(*ListSgl_Link_data$u32(unwrap_(unwrap_(list.first)->next)) == 4));
} $unguarded(TEST_fn);

TEST_fn_("ListSgl: removing a non-member is a no-op for list and foreign chain" $guard) {
    var list = ListSgl_empty$u32();
    var one = ListSgl_Adp_init$u32(1);
    var two = ListSgl_Adp_init$u32(2);
    var foreign = ListSgl_Adp_init$u32(9);
    var foreign_child = ListSgl_Adp_init$u32(10);

    ListSgl_prepend$u32(&list, &two.link);
    ListSgl_prepend$u32(&list, &one.link);
    ListSgl_Link_insertNext$u32(&foreign.link, &foreign_child.link);

    ListSgl_remove$u32(&list, &foreign.link);

    try_(TEST_expect(ListSgl_len$u32(&list) == 2));
    try_(TEST_expect(*ListSgl_Link_data$u32(unwrap_(list.first)) == 1));
    try_(TEST_expect(*ListSgl_Link_data$u32(unwrap_(unwrap_(list.first)->next)) == 2));
    try_(TEST_expect(*ListSgl_Link_data$u32(unwrap_(foreign.link.next)) == 10));
} $unguarded(TEST_fn);

TEST_fn_("ListSgl: reverse handles empty single and multi-node chains" $guard) {
    var list = ListSgl_empty$u32();
    var one = ListSgl_Adp_init$u32(1);
    var two = ListSgl_Adp_init$u32(2);
    var three = ListSgl_Adp_init$u32(3);

    ListSgl_Link_reverse$u32(&list.first);
    try_(TEST_expect(isNone(list.first)));

    ListSgl_prepend$u32(&list, &one.link);
    ListSgl_Link_reverse$u32(&list.first);
    try_(TEST_expect(*ListSgl_Link_data$u32(unwrap_(list.first)) == 1));
    try_(TEST_expect(isNone(one.link.next)));

    ListSgl_Link_insertNext$u32(&one.link, &two.link);
    ListSgl_Link_insertNext$u32(&two.link, &three.link);
    ListSgl_Link_reverse$u32(&list.first);

    try_(TEST_expect(ListSgl_len$u32(&list) == 3));
    try_(TEST_expect(*ListSgl_Link_data$u32(unwrap_(list.first)) == 3));
    try_(TEST_expect(*ListSgl_Link_data$u32(unwrap_(unwrap_(list.first)->next)) == 2));
    try_(TEST_expect(*ListSgl_Link_data$u32(ListSgl_Link_findLast$u32(unwrap_(list.first))) == 1));
    try_(TEST_expect(isNone(one.link.next)));
} $unguarded(TEST_fn);

TEST_fn_("ListSgl: link and adapter data access expose the same mutable payload" $scope) {
    var item = ListSgl_Adp_init$u32(7);

    *ListSgl_Adp_dataMut$u32(&item) = 8;
    try_(TEST_expect(*ListSgl_Link_data$u32(&item.link) == 8));

    *ListSgl_Link_dataMut$u32(&item.link) = 9;
    try_(TEST_expect(*ListSgl_Adp_data$u32(&item) == 9));
    try_(TEST_expect(ListSgl_Link_adp$u32(&item.link) == &item));
    try_(TEST_expect(ListSgl_Link_adpMut$u32(&item.link) == &item));
} $unscoped(TEST_fn);
