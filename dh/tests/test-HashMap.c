#include "dh/main.h"
#include "dh/HashMap.h"
#include "dh/heap/Page.h"

T_use$((usize, u16)(
    HashMap,
    HashMap_Iter,
    HashMap_Entry,
    HashMap_init,
    HashMap_fini,
    HashMap_put,
    HashMap_by,
    HashMap_iter,
    HashMap_Iter_next,
    HashMap_Entry_key
));

TEST_fn_("basic usage" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashMap_Ctx_default();
    var map_value = try_(HashMap_init$1usize$2u16(ctx, gpa, 256));
    defer_(HashMap_fini$1usize$2u16(&map_value, gpa));

    let_(count, u32) = 128;
    var_(total, u32) = 0;
    for_(($r(0, count))(i) {
        try_(HashMap_put$1usize$2u16(&map_value, gpa, i, i));
        total += i;
    });

    with_(var_(sum, u32) = 0) {
        var it = HashMap_iter$1usize$2u16(&map_value);
        while_some(HashMap_Iter_next$1usize$2u16(&it), entry) {
            sum += *HashMap_Entry_key$1usize$2u16(entry);
        }
        try_(TEST_expect(sum == total));
    }

    with_(var_(sum, u32) = 0) {
        for_(($r(0, count))(i) {
            try_(TEST_expect(i == unwrap_(HashMap_by$1usize$2u16(map_value, i))));
            sum += unwrap_(HashMap_by$1usize$2u16(map_value, i));
        });
        try_(TEST_expect(sum == total));
    }
} $unguarded_(TEST_fn);

TEST_fn_("basic usage - no templates used" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashMap_Ctx_default();
    var map_value = try_(HashMap_init(
        typeInfo$(usize), typeInfo$(u16), ctx, gpa, 256
    ));
    defer_(HashMap_fini(&map_value, typeInfo$(usize), typeInfo$(u16), gpa));

    let_(count, u32) = 128;
    var_(total, u32) = 0;
    for_(($r(0, count))(i) {
        try_(HashMap_put(&map_value, gpa, u_anyV(as$(usize)(i)), u_anyV(as$(u16)(i))));
        total += i;
    });

    with_(var_(sum, u32) = 0) {
        var it = HashMap_iter(&map_value, typeInfo$(usize), typeInfo$(u16));
        while_some(HashMap_Iter_next(&it, typeInfo$(usize), typeInfo$(u16)), entry) {
            sum += *u_castP$((const usize*)(HashMap_Entry_key(entry, typeInfo$(usize))));
        }
        try_(TEST_expect(sum == total));
    }

    with_(var_(sum, u32) = 0) {
        for_(($r(0, count))(i) {
            try_(TEST_expect(i == unwrap_(u_castO$((O$u16)(HashMap_by(map_value, u_anyV(as$(usize)(i)), u_retV$(u16)))))));
            sum += unwrap_(u_castO$((O$u16)(HashMap_by(map_value, u_anyV(as$(usize)(i)), u_retV$(u16)))));
        });
        try_(TEST_expect(sum == total));
    }
} $unguarded_(TEST_fn);

#if UNUSED_CODE
fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashMap_Ctx_default();
    var map_value = try_(HashMap_init(
        typeInfo$(u32), typeInfo$(u32), ctx, gpa, 16
    ));
    defer_(HashMap_fini(&map_value, typeInfo$(u32), typeInfo$(u32), gpa));

    let_(count, u32) = 5;
    var_(total, u32) = 0;
    for_(($r(0, count))(i) {
        try_(HashMap_put(&map_value, gpa, u_anyV(as$(u32)(i)), u_anyV(as$(u32)(i))));
        total += i;
    });

    with_(var_(sum, u32) = 0) {
        var it = HashMap_iter(&map_value, typeInfo$(u32), typeInfo$(u32));
        while_some(HashMap_Iter_next(&it, typeInfo$(u32), typeInfo$(u32)), entry) {
            sum += *u_castP$((const u32*)(HashMap_Entry_key(entry, typeInfo$(u32))));
        }
        try_(TEST_expect(sum == total));
    }

    with_(var_(sum, u32) = 0) {
        for_(($r(0, count))(i) {
            try_(TEST_expect(i == unwrap_(u_castO$((O$u32)(HashMap_by(map_value, u_anyV(as$(u32)(i)), u_retV$(u32)))))));
            sum += unwrap_(u_castO$((O$u32)(HashMap_by(map_value, u_anyV(as$(u32)(i)), u_retV$(u32)))));
        });
        try_(TEST_expect(sum == total));
    }

    return_ok({});
} $unguarded_(fn);
#endif /* UNUSED_CODE */
