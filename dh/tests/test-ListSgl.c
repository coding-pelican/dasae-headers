#include "dh/main.h"
#include "dh/ListSgl.h"

T_use$((u32)(
    ListSgl_Node,
    ListSgl_Node_init,
    ListSgl_Node_insertNext,
    ListSgl_Node_removeNext,
    ListSgl_Node_reverse,
    ListSgl,
    ListSgl_init,
    ListSgl_prepend,
    ListSgl_remove,
    ListSgl_shift
));

TEST_fn_("ListSgl: Basic SinglyLinkedList Operations" $guard) {
    var list = ListSgl_init$u32();

    try_(TEST_expect(ListSgl_len(list.as_raw) == 0));

    var one = ListSgl_Node_init$u32(&lit$((ListSgl_Node$u32){ .data = 1 }));
    var two = ListSgl_Node_init$u32(&lit$((ListSgl_Node$u32){ .data = 2 }));
    var three = ListSgl_Node_init$u32(&lit$((ListSgl_Node$u32){ .data = 3 }));
    var four = ListSgl_Node_init$u32(&lit$((ListSgl_Node$u32){ .data = 4 }));
    var five = ListSgl_Node_init$u32(&lit$((ListSgl_Node$u32){ .data = 5 }));

    ListSgl_prepend$u32(&list, two);          // {2}
    ListSgl_Node_insertNext$u32(two, five);   // {2, 5}
    ListSgl_prepend$u32(&list, one);          // {1, 2, 5}
    ListSgl_Node_insertNext$u32(two, three);  // {1, 2, 3, 5}
    ListSgl_Node_insertNext$u32(three, four); // {1, 2, 3, 4, 5}

    try_(TEST_expect(ListSgl_len(list.as_raw) == 5));

    // Traverse forwards.
    {
        var it = list.first;
        u32 index = 1;
        while_some(it, node) blk_defer_({
            defer_(index += 1, it = node->next);
            try_(TEST_expect(node->data == index));
        }) blk_deferral;
    }

    let_ignore = ListSgl_shift$u32(&list);         // {2, 3, 4, 5}
    ListSgl_remove$u32(&list, five);               // {2, 3, 4}
    let_ignore = ListSgl_Node_removeNext$u32(two); // {2, 4}

    try_(TEST_expect(pipe(list.first,(unwrap_,()->data)) == 2));
    // try_(TEST_expect(unwrap_(list.first)->data == 2));
    try_(TEST_expect(pipe(list.first,(unwrap_,()->next),(unwrap_,()->data)) == 4));
    // // try_(TEST_expect(unwrap_(unwrap_(list.first)->next)->data == 4));
    try_(TEST_expect(pipe(list.first,(unwrap_,()->next),(unwrap_,()->next),(isNone,()))));
    // // try_(TEST_expect(isNone(unwrap_(unwrap_(list.first)->next)->next)));

    ListSgl_Node_reverse$u32(&list.first);

    try_(TEST_expect(pipe(list.first,(unwrap_,()->data)) == 4));
    // // try_(TEST_expect(unwrap_(list.first)->data == 4));
    try_(TEST_expect(pipe(list.first,(unwrap_,()->next),(unwrap_,()->data)) == 2));
    // // try_(TEST_expect(unwrap_(unwrap_(list.first)->next)->data == 2));
    try_(TEST_expect(pipe(list.first,(unwrap_,()->next),(unwrap_,()->next),(isNone,()))));
    // // try_(TEST_expect(isNone(unwrap_(unwrap_(list.first)->next)->next)));
} $unguarded_(TEST_fn);
