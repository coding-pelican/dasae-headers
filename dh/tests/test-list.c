#include "dh/main.h"
#include "dh/TEST.h"
#include "dh/list.h"

TEST_fn_("list: Basic SinglyLinkedList Operations" $guard) {
    use_ListSgl$(u32);
    var list = make$(ListSgl$u32, .first = none());

    try_(TEST_expect(ListSgl_len(list.base) == 0));

    var one = make$(ListSgl_Node$u32, .data = 1);
    var two = make$(ListSgl_Node$u32, .data = 2);
    var three = make$(ListSgl_Node$u32, .data = 3);
    var four = make$(ListSgl_Node$u32, .data = 4);
    var five = make$(ListSgl_Node$u32, .data = 5);

    ListSgl_prepend(list.base, two.base);            // {2}
    ListSgl_Node_insertAfter(two.base, five.base);   // {2, 5}
    ListSgl_prepend(list.base, one.base);            // {1, 2, 5}
    ListSgl_Node_insertAfter(two.base, three.base);  // {1, 2, 3, 5}
    ListSgl_Node_insertAfter(three.base, four.base); // {1, 2, 3, 4, 5}

    try_(TEST_expect(ListSgl_len(list.base) == 5));

    // Traverse forwards.
    {
        var it    = list.first;
        u32 index = 1;
        while_some(it, node) blk_defer {
            defer_(index += 1, it = node->next);
            try_(TEST_expect(node->data == index));
        } blk_deferral;
    }

    let_ignore = ListSgl_popFirst(list.base);          // {2, 3, 4, 5}
    ListSgl_remove(list.base, five.base); // {2, 3, 4}
    let_ignore = ListSgl_Node_removeNext(two.base);    // {2, 4}

    try_(TEST_expect(pipe(list.first,(unwrap,()->data)) == 2));
    // try_(TEST_expect(unwrap(list.first)->data == 2));
    try_(TEST_expect(pipe(list.first,(unwrap,()->next),(unwrap,()->data)) == 4));
    // try_(TEST_expect(unwrap(unwrap(list.first)->next)->data == 4));
    try_(TEST_expect(pipe(list.first,(unwrap,()->next),(unwrap,()->next),(isNone,()))));
    // try_(TEST_expect(isNone(unwrap(unwrap(list.first)->next)->next)));

    ListSgl_Node_reverse(type$(Opt$Ptr$ListSgl_Node*, &list.first));

    try_(TEST_expect(pipe(list.first,(unwrap,()->data)) == 4));
    // try_(TEST_expect(unwrap(list.first)->data == 4));
    try_(TEST_expect(pipe(list.first,(unwrap,()->next),(unwrap,()->data)) == 2));
    // try_(TEST_expect(unwrap(unwrap(list.first)->next)->data == 2));
    try_(TEST_expect(pipe(list.first,(unwrap,()->next),(unwrap,()->next),(isNone,()))));
    // try_(TEST_expect(isNone(unwrap(unwrap(list.first)->next)->next)));
} $unguarded_(TEST_fn);
