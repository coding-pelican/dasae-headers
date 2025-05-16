#include "dh/main.h"
#include "dh/TEST.h"
#include "dh/list.h"

fn_TEST_scope_ext("Basic SinglyLinkedList Operations") {
    use_ListSgl$(u32);
    var_type(list, ListSgl$u32, ListSgl_init());

    try_(TEST_expect(ListSgl_len(list.base) == 0));

    var_type(one, ListSgl_Node$u32, ListSgl_Node_init(meta_create$(u32, 1)));
    var_type(two, ListSgl_Node$u32, ListSgl_Node_init(meta_create$(u32, 2)));
    var_type(three, ListSgl_Node$u32, ListSgl_Node_init(meta_create$(u32, 3)));
    var_type(four, ListSgl_Node$u32, ListSgl_Node_init(meta_create$(u32, 4)));
    var_type(five, ListSgl_Node$u32, ListSgl_Node_init(meta_create$(u32, 5)));

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
        while_some(it, node) block_defer {
            defer_(index += 1, it = node->next);
            try_(TEST_expect(*node->data == index));
        } block_deferral;
    }

    $ignore = ListSgl_popFirst(list.base);          // {2, 3, 4, 5}
    ListSgl_remove(list.base, five.base); // {2, 3, 4}
    $ignore = ListSgl_Node_removeNext(two.base);    // {2, 4}

    // try_(TEST_expect(pipe(list.first,(unwrap,()->data),(deref,())) == 2));
    try_(TEST_expect(*unwrap(list.first)->data == 2));
    // try_(TEST_expect(pipe(list.first,(unwrap,()->next),(unwrap,()->data),(deref,())) == 4));
    try_(TEST_expect(*unwrap(unwrap(list.first)->next)->data == 4));
    // try_(TEST_expect(pipe(list.first,(unwrap,()->next),(unwrap,()->next),(isNone,()))));
    try_(TEST_expect(isNone(unwrap(unwrap(list.first)->next)->next)));

    ListSgl_Node_reverse(type$(Opt$Ptr$ListSgl_Node*, &list.first));

    // try_(TEST_expect(pipe(list.first,(unwrap,()->data),(deref,())) == 4));
    try_(TEST_expect(*unwrap(list.first)->data == 4));
    // try_(TEST_expect(pipe(list.first,(unwrap,()->next),(unwrap,()->data),(deref,())) == 2));
    try_(TEST_expect(*unwrap(unwrap(list.first)->next)->data == 2));
    // try_(TEST_expect(pipe(list.first,(unwrap,()->next),(unwrap,()->next),(isNone,()))));
    try_(TEST_expect(isNone(unwrap(unwrap(list.first)->next)->next)));
} TEST_unscoped_ext;
