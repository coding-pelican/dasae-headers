#include "dh/main.h"
#include "dh/ListSgl.h"

T_use$((u32)(ListSgl_Link, ListSgl_Adp, ListSgl));
T_use$((u32)(
    ListSgl_Adp_init,
    ListSgl_empty,
    ListSgl_len,
    ListSgl_prepend,
    ListSgl_Link_insertNext,
    ListSgl_Link_data,
    ListSgl_remove,
    ListSgl_Link_removeNext,
    ListSgl_shift,
    ListSgl_Link_reverse
));

TEST_fn_("ListSgl: Basic SinglyLinkedList Operations" $guard) {
    let LAdp_init = ListSgl_Adp_init$u32;
    let L_empty = ListSgl_empty$u32;
    let L_len = ListSgl_len$u32;
    let L_prepend = ListSgl_prepend$u32;
    let LLink_insertNext = ListSgl_Link_insertNext$u32;
    let LLink_data = ListSgl_Link_data$u32;
    let L_remove = ListSgl_remove$u32;
    let LLink_removeNext = ListSgl_Link_removeNext$u32;
    let L_shift = ListSgl_shift$u32;
    let LLink_reverse = ListSgl_Link_reverse$u32;

    var list = L_empty();
    try_(TEST_expect(L_len(&list) == 0));

    var one = LAdp_init(1);
    var two = LAdp_init(2);
    var three = LAdp_init(3);
    var four = LAdp_init(4);
    var five = LAdp_init(5);

    L_prepend(&list, &two.link);               // {2}
    LLink_insertNext(&two.link, &five.link);   // {2, 5}
    L_prepend(&list, &one.link);               // {1, 2, 5}
    LLink_insertNext(&two.link, &three.link);  // {1, 2, 3, 5}
    LLink_insertNext(&three.link, &four.link); // {1, 2, 3, 4, 5}

    try_(TEST_expect(L_len(&list) == 5));

    // Traverse forwards.
    {
        var it = list.first;
        u32 index = 1;
        while_some(it, node) blk_defer_({
            defer_(index += 1, it = node->next);
            try_(TEST_expect(*LLink_data(node) == index));
        }) blk_deferral;
    }

    let_ignore = L_shift(&list);              // {2, 3, 4, 5}
    L_remove(&list, &five.link);              // {2, 3, 4}
    let_ignore = LLink_removeNext(&two.link); // {2, 4}

    try_(TEST_expect(pipe(list.first,(unwrap_,()),(LLink_data,()),(deref,())) == 2));
    // try_(TEST_expect(unwrap_(list.first)->data == 2));
    try_(TEST_expect(pipe(list.first,(unwrap_,()->next),(unwrap_,()),(LLink_data,()),(deref,())) == 4));
    // // try_(TEST_expect(unwrap_(unwrap_(list.first)->next)->data == 4));
    try_(TEST_expect(pipe(list.first,(unwrap_,()->next),(unwrap_,()->next),(isNone,()))));
    // // try_(TEST_expect(isNone(unwrap_(unwrap_(list.first)->next)->next)));

    LLink_reverse(&list.first);

    try_(TEST_expect(pipe(list.first,(unwrap_,()),(LLink_data,()),(deref,())) == 4));
    // // try_(TEST_expect(unwrap_(list.first)->data == 4));
    try_(TEST_expect(pipe(list.first,(unwrap_,()->next),(unwrap_,()),(LLink_data,()),(deref,())) == 2));
    // // try_(TEST_expect(unwrap_(unwrap_(list.first)->next)->data == 2));
    try_(TEST_expect(pipe(list.first,(unwrap_,()->next),(unwrap_,()->next),(isNone,()))));
    // // try_(TEST_expect(isNone(unwrap_(unwrap_(list.first)->next)->next)));
} $unguarded_(TEST_fn);
