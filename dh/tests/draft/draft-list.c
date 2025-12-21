#if UNUSED_CODE
// dh-c build draft-list.c --no-libdh --args="-IC:\dasae\dev\dasae-headers\dh\include -IC:\dasae\dev\dasae-headers\dh\libs\BlocksRuntime\include"

#include "dh/prl.h"
#include "dh/meta/common.h"

/*========== Macros and Declarations ========================================*/

#if UNUSED_CODE
typedef struct MRaw {
    var_(type, TypeInfo);
    var_(data $flexible, u8);
} MRaw;
typedef union MRaw$i32 {
    struct {
        var_(type, TypeInfo);
        var_(data, i32);
    };
    var_(ref_meta $like_ref, MRaw);
} MRaw$i32;
#endif /* UNUSED_CODE */

/* Singly Linked List */

#define T_use_ListSgl_Node$(_T) comp_type_gen__T_use_ListSgl_Node$(_T)
#define T_decl_ListSgl_Node$(_T) comp_type_gen__T_decl_ListSgl_Node$(_T)
#define T_impl_ListSgl_Node$(_T) comp_type_gen__T_impl_ListSgl_Node$(_T)
#define ListSgl_Node$(_T) comp_type_alias__ListSgl_Node$(_T)

typedef struct ListSgl_Node ListSgl_Node;
T_use_P$(ListSgl_Node);
T_use_O$(P$ListSgl_Node);
struct ListSgl_Node {
    var_(type, TypeInfo);
    var_(next, O$P$ListSgl_Node);
    struct {
        var_(data $flexible, u8);
    };
};

extern fn_((ListSgl_Node_insertNext(ListSgl_Node* node, ListSgl_Node* new_node))(void));
extern fn_((ListSgl_Node_removeNext(ListSgl_Node* node))(O$P$ListSgl_Node));
extern fn_((ListSgl_Node_findLast(ListSgl_Node* node))(ListSgl_Node*));
extern fn_((ListSgl_Node_countChildren(const ListSgl_Node* node))(usize));
extern fn_((ListSgl_Node_reverse(O$P$ListSgl_Node* indirect))(void));

#define T_use_ListSgl$(_T) comp_type_gen__T_use_ListSgl$(_T)
#define T_decl_ListSgl$(_T) comp_type_gen__T_decl_ListSgl$(_T)
#define T_impl_ListSgl$(_T) comp_type_gen__T_impl_ListSgl$(_T)
#define ListSgl$(_T) comp_type_alias__ListSgl$(_T)

typedef struct ListSgl$raw {
    var_(type, TypeInfo);
    var_(first, O$P$ListSgl_Node);
} ListSgl;

extern fn_((ListSgl_prepend(ListSgl* self, ListSgl_Node* new_node))(void));
extern fn_((ListSgl_remove(ListSgl* self, ListSgl_Node* node))(void));
extern fn_((ListSgl_shift(ListSgl* self))(O$P$ListSgl_Node));
extern fn_((ListSgl_len(const ListSgl* self))(usize));

/*========== Macros and Definitions =========================================*/

/* Singly Linked List */

#define comp_type_gen__T_use_ListSgl_Node$(_T) \
    T_decl_ListSgl_Node$(_T); \
    T_impl_ListSgl_Node$(_T)
#define comp_type_gen__T_decl_ListSgl_Node$(_T) \
    $maybe_unused typedef union ListSgl_Node$(_T) ListSgl_Node$(_T); \
    T_use_P$(ListSgl_Node$(_T)); \
    T_use_O$(P$(ListSgl_Node$(_T)))
#define comp_type_gen__T_impl_ListSgl_Node$(_T) \
    union ListSgl_Node$(_T) { \
        var_(raw, ListSgl_Node); \
        var_(raw_ref $like_ref, ListSgl_Node); \
        struct { \
            var_(type, TypeInfo); \
            var_(next, O$(P$(ListSgl_Node$(_T)))); \
            struct { \
                var_(data, _T); \
            }; \
        }; \
    }
#define comp_type_alias__ListSgl_Node$(_T) \
    pp_join($, ListSgl_Node, _T)

#define comp_type_gen__T_use_ListSgl$(_T) \
    T_decl_ListSgl$(_T); \
    T_impl_ListSgl$(_T)
#define comp_type_gen__T_decl_ListSgl$(_T) \
    $maybe_unused typedef union ListSgl$(_T) ListSgl$(_T)
#define comp_type_gen__T_impl_ListSgl$(_T) \
    union ListSgl$(_T) { \
        struct { \
            var_(type, TypeInfo); \
            var_(first, O$(P$(ListSgl_Node$(_T)))); \
        }; \
        var_(raw, ListSgl); \
        var_(raw_ref $like_ref, ListSgl); \
    }
#define comp_type_alias__ListSgl$(_T) \
    pp_join($, ListSgl, _T)

// 절대 단독으로 사용되면 안됨
// 절대 untyped MRaw*를 deref하면 안됨
// void*와 같음.
typedef struct MRaw {
    var_(untyped $flexible, u8);
} MRaw;

typedef struct MVal {
    var_(type, TypeInfo);
    var_(inner, MRaw*);
} MVal;
typedef struct MArr {
    var_(type, TypeInfo);
    var_(inner, MRaw*);
    var_(len, usize);
} MArr;

typedef union MPtr_const {
    struct {
        var_(type, TypeInfo);
        var_(inner, const MRaw*);
    };
    var_(deref, MVal);
} MPtr_const;
typedef union MPtr {
    struct {
        var_(type, TypeInfo);
        var_(inner, MRaw*);
    };
    var_(deref, MVal);
    var_(as_const, MPtr_const);
} MPtr;

typedef union MSli_const {
    struct {
        var_(ptr, MPtr_const);
        var_(len, usize);
    };
    var_(deref, MArr);
} MSli_const;
typedef union MSli {
    struct {
        var_(ptr, MPtr);
        var_(len, usize);
    };
    var_(deref, MArr);
    var_(as_const, MSli_const);
} MSli;

typedef union MRaw$i32 {
    struct {
        var_(data, i32);
    };
    var_(into_inner $like_ref, MRaw);
} MRaw$i32;

typedef struct Label {
    u8 name[32];
    usize len;
} Label;
typedef union MRaw$Label {
    struct {
        var_(data, Label);
    };
    var_(into_inner $like_ref, MRaw);
} MRaw$Label;

typedef O$$(MVal) O$MVal;

#define meta_fn_(/*(_ident(_Params...))(_TReturn) <$ext> | (_Params...)(_TReturn) $T*/...) \
    pp_expand(pp_defer(__block_inline__meta_fn_)(__param_expand__meta_fn_ __VA_ARGS__))
#define __param_expand__meta_fn_(...) (__VA_ARGS__), pp_expand
#define __block_inline__meta_fn_(...) \
    pp_overload(__block_inline__meta_fn, __VA_ARGS__)(__VA_ARGS__)
#define __block_inline__meta_fn_2(_ident_w_Params, _TReturn...) comp_syn__meta_fn_(__param_extract__meta_fn_ _ident_w_Params, _TReturn)
#define __block_inline__meta_fn_3(_ident_w_Params_OR_Params, _TReturn, _$ext...) \
    pp_join(_, __block_inline__meta_fn_3, _$ext)(_ident_w_Params_OR_Params, _TReturn)
#define __block_inline__meta_fn_3_$_scope(_ident_w_Params, _TReturn...) comp_syn__meta_fn_$_scope(__param_extract__meta_fn_ _ident_w_Params, _TReturn)
#define __block_inline__meta_fn_3_$_guard(_ident_w_Params, _TReturn...) comp_syn__meta_fn_$_guard(__param_extract__meta_fn_ _ident_w_Params, _TReturn)
#define __block_inline__meta_fn_3_$_T(_Params, _TReturn...) comp_syn__meta_fn_$_T(_Params, _TReturn)
#define __param_extract__meta_fn_(...) __VA_ARGS__

#define comp_syn__meta_fn_$_scope(_ident_w_Params, _TReturn...) \
    _TReturn _ident_w_Params { \
        let __reserved_return = &ret_mem; \
        if (false) { \
__step_return: \
            goto __step_unscope; \
        } \
        do
#define inline__$unscoped_meta_fn() \
    while (false); \
    _Generic(TypeOf(*__reserved_return), void: ({ goto __step_return; }), Void: ({ goto __step_return; }), default: ({})); \
    if (false) { \
__step_unscope: \
        return *__reserved_return; \
    } \
    }

meta_fn_((spawnItem(O$MVal ret_mem))(O$MVal)$scope) {
    let label = (Label){ .name = "Hello, world!\0", .len = 10 };
    return_some({
        .type = typeInfo$(TypeOf(label)),
        .inner = lit$((TypeOf(union {
                     struct {
                         var_(data, Label);
                     };
                     var_(into_inner $like_ref, MRaw);
                 })){ .data = label })
                    .into_inner,
    });
} $unscoped_(meta_fn);

#include <stdio.h>

int main(void) {
    i32 origin_i32 = 123;
    MVal origin_copy = {
        .type = typeInfo$(TypeOf(origin_i32)),
        .inner = lit$((TypeOf(union {
                     struct {
                         var_(data, i32);
                     };
                     var_(into_inner $like_ref, MRaw);
                 })){ .data = origin_i32 })
                    .into_inner,
    };
    MVal shallow_copy = origin_copy;
    *as$(i32*)(origin_copy.inner->untyped) += origin_i32;
    let origin_copy1 = ({
        ((MVal){
            .type = typeInfo$(TypeOf(origin_i32)),
            .inner = lit$((TypeOf(union {
                         struct {
                             var_(data, TypeOf(origin_i32));
                         };
                         var_(into_inner $like_ref, MRaw);
                     })){ .data = origin_i32 })
                        .into_inner });
    });
    printf("origin_i32: %d\n", origin_i32);
    printf("origin_copy: %d\n", *as$(i32*)(origin_copy.inner->untyped));
    printf("origin_copy1: %d\n", *as$(i32*)(origin_copy1.inner->untyped));
    printf("shallow_copy(): %d\n", *as$(i32*)(shallow_copy.inner->untyped));

    let item = spawnItem(lit$((O$MVal){ .payload = { .some = { .inner = (void*)&lit$((Label){}) } } }));
    printf("item: %s\n", (*as$(Label*) ((item.payload.some.inner->untyped))).name);
    return 0;
}
#endif /* UNUSED_CODE */

#include "dh/main.h"

T_use_P$(TypeInfo);
T_use_S$(TypeInfo);

#define T_use_ListSgl_Node$(_T) __comp_gen__T_use_ListSgl_Node$(_T)
#define T_decl_ListSgl_Node$(_T) __comp_gen__T_decl_ListSgl_Node$(_T)
#define T_impl_ListSgl_Node$(_T) __comp_gen__T_impl_ListSgl_Node$(_T)
#define ListSgl_Node$(_T) __comp_alias__ListSgl_Node$(_T)

typedef struct ListSgl_Node$raw ListSgl_Node$raw;
T_use_P$(ListSgl_Node$raw);
T_use_O$(P$ListSgl_Node$raw);
struct ListSgl_Node$raw {
    struct {
        var_(next, O$P$ListSgl_Node$raw);
        debug_only(var_(type, TypeInfo);)
    };
    var_(data $flexible, u8);
};

extern fn_((ListSgl_Node_init(P$ListSgl_Node$raw self, TypeInfo type))(P$ListSgl_Node$raw));
extern fn_((ListSgl_Node_data(P_const$ListSgl_Node$raw self, TypeInfo type))(u_P_const$raw));
extern fn_((ListSgl_Node_dataMut(P$ListSgl_Node$raw self, TypeInfo type))(u_P$raw));
extern fn_((ListSgl_Node_insertNext(P$ListSgl_Node$raw self, P$ListSgl_Node$raw after))(void));
extern fn_((ListSgl_Node_removeNext(P$ListSgl_Node$raw self))(O$P$ListSgl_Node$raw));
extern fn_((ListSgl_Node_findLast(P$ListSgl_Node$raw self))(P$ListSgl_Node$raw));
extern fn_((ListSgl_Node_countChildren(P_const$ListSgl_Node$raw self))(usize));
extern fn_((ListSgl_Node_reverse(O$P$ListSgl_Node$raw* indirect))(void));

#define T_use_ListSgl$(_T) __comp_gen__T_use_ListSgl$(_T)
#define T_decl_ListSgl$(_T) __comp_gen__T_decl_ListSgl$(_T)
#define T_impl_ListSgl$(_T) __comp_gen__T_impl_ListSgl$(_T)
#define ListSgl$(_T) __comp_alias__ListSgl$(_T)

typedef struct ListSgl$raw {
    var_(first, O$P$ListSgl_Node$raw);
    debug_only(var_(type, TypeInfo);)
} ListSgl$raw;

extern fn_((ListSgl_init(TypeInfo type))(ListSgl$raw));
extern fn_((ListSgl_prepend(ListSgl$raw* self, P$ListSgl_Node$raw before))(void));
extern fn_((ListSgl_remove(ListSgl$raw* self, P$ListSgl_Node$raw node))(void));
extern fn_((ListSgl_shift(ListSgl$raw* self))(O$P$ListSgl_Node$raw));
extern fn_((ListSgl_len(const ListSgl$raw* self))(usize));

typedef struct ListSgl_Link ListSgl_Link;
T_use_P$(ListSgl_Link);
T_use_O$(P$ListSgl_Link);
struct ListSgl_Link {
    var_(next, O$P$ListSgl_Link);
};
fn_((ListSgl_Link_data(P_const$ListSgl_Link self, usize in_offset))(u_P_const$raw)) {}
fn_((ListSgl_Link_dataMut(P$ListSgl_Link self, usize in_offset))(u_P$raw)) {}



#define typeInfos$(_T...) A_ref$((S_const$TypeInfo)A_from$((TypeInfo){ pp_foreach(__typeInfos$__each, ~, _T) }))
#define __typeInfos$__each(_$ignored, _T...) typeInfo$(_T),

let a = typeInfos$(u32, i32, usize);


fn_((ListSgl_Node_link(P$ListSgl_Node$raw self, P$ListSgl_Link link))(void));


TEST_fn_("ListSgl: Basic SinglyLinkedList Operations" $guard) {
    typedef struct L {
        var_(data, u32);
        var_(link, ListSgl_Link);
    } L;
    var list = (ListSgl){};

    try_(TEST_expect(ListSgl_len(&list) == 0));

    var one = (L){ .data = 1 };
    var two = (L){ .data = 2 };
    var three = (L){ .data = 3 };
    var four = (L){ .data = 4 };
    var five = (L){ .data = 5 };

    ListSgl_prepend(&list, &two.link); // {2}
    ListSgl_Node_insertNext(two, &five.link); // {2, 5}
    ListSgl_prepend(&list, &one.link); // {1, 2, 5}
    ListSgl_Node_insertNext(&two.link, &three.link); // {1, 2, 3, 5}
    ListSgl_Node_insertNext(&three.link, &four.link); // {1, 2, 3, 4, 5}

    try_(TEST_expect(ListSgl_len(list.as_raw) == 5));

    // Traverse forwards.
    {
        var it = list.first;
        u32 index = 1;
        while_some(it, node) blk_defer_({
            defer_(index += 1, it = node->next);
            try_(TEST_expect(ListSgl_Link_data(node, offsetTo(L, data)) == index));
        }) blk_deferral;
    }

    let_ignore = ListSgl_shift$u32(&list); // {2, 3, 4, 5}
    ListSgl_remove$u32(&list, five); // {2, 3, 4}
    let_ignore = ListSgl_Node_removeNext$u32(two); // {2, 4}

    try_(TEST_expect(pipe_(list.first, (unwrap_, ()->data)) == 2));
    // try_(TEST_expect(unwrap_(list.first)->data == 2));
    try_(TEST_expect(pipe_(list.first, (unwrap_, ()->next), (unwrap_, ()->data)) == 4));
    // // try_(TEST_expect(unwrap_(unwrap_(list.first)->next)->data == 4));
    try_(TEST_expect(pipe_(list.first, (unwrap_, ()->next), (unwrap_, ()->next), (isNone, ()))));
    // // try_(TEST_expect(isNone(unwrap_(unwrap_(list.first)->next)->next)));

    ListSgl_Node_reverse$u32(&list.first);

    try_(TEST_expect(pipe_(list.first, (unwrap_, ()->data)) == 4));
    // // try_(TEST_expect(unwrap_(list.first)->data == 4));
    try_(TEST_expect(pipe_(list.first, (unwrap_, ()->next), (unwrap_, ()->data)) == 2));
    // // try_(TEST_expect(unwrap_(unwrap_(list.first)->next)->data == 2));
    try_(TEST_expect(pipe_(list.first, (unwrap_, ()->next), (unwrap_, ()->next), (isNone, ()))));
    // // try_(TEST_expect(isNone(unwrap_(unwrap_(list.first)->next)->next)));
} $unguarded_(TEST_fn);
