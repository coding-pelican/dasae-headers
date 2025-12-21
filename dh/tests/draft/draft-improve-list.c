#include "dh/main.h"

#define ListSgl_Link$$(_T...) __comp_anon__ListSgl_Link$$(_T)
#define ListSgl_Link$(_T...) __comp_alias__ListSgl_Link$(_T)
#define T_use_ListSgl_Link$(_T...) __comp_gen__T_use_ListSgl_Link$(_T)
#define T_decl_ListSgl_Link$(_T...) __comp_gen__T_decl_ListSgl_Link$(_T)
#define T_impl_ListSgl_Link$(_T...) __comp_gen__T_impl_ListSgl_Link$(_T)

typedef struct ListSgl_Link ListSgl_Link;
T_use_P$(ListSgl_Link);
T_use_O$(P$ListSgl_Link);
struct ListSgl_Link {
    var_(next, O$P$ListSgl_Link);
    debug_only(var_(type, TypeInfo);)
};

static fn_((ListSgl_Link_empty(TypeInfo type))(ListSgl_Link));
static fn_((ListSgl_Link_insertNext(P$ListSgl_Link self, P$ListSgl_Link after))(void));
static fn_((ListSgl_Link_removeNext(P$ListSgl_Link self))(O$P$ListSgl_Link));
static fn_((ListSgl_Link_findLast(P$ListSgl_Link self))(P$ListSgl_Link));
static fn_((ListSgl_Link_countChildren(P_const$ListSgl_Link self))(usize));
static fn_((ListSgl_Link_reverse(O$P$ListSgl_Link* indirect))(void));

typedef struct ListSgl_Adp$raw ListSgl_Adp$raw;
static fn_((ListSgl_Link_adp(P_const$ListSgl_Link self))(const ListSgl_Adp$raw*));
static fn_((ListSgl_Link_adpMut(P$ListSgl_Link self))(ListSgl_Adp$raw*));
static fn_((ListSgl_Link_data(P_const$ListSgl_Link self, TypeInfo type))(u_P_const$raw));
static fn_((ListSgl_Link_dataMut(P$ListSgl_Link self, TypeInfo type))(u_P$raw));


#define ListSgl_Adp$$(_T...) __comp_anon__ListSgl_Adp$$(_T)
#define ListSgl_Adp$(_T...) __comp_alias__ListSgl_Adp$(_T)
#define T_use_ListSgl_Adp$(_T...) __comp_gen__T_use_ListSgl_Adp$(_T)
#define T_decl_ListSgl_Adp$(_T...) __comp_gen__T_decl_ListSgl_Adp$(_T)
#define T_impl_ListSgl_Adp$(_T...) __comp_gen__T_impl_ListSgl_Adp$(_T)

struct ListSgl_Adp$raw {
    struct {
        var_(link, ListSgl_Link);
        debug_only(var_(type, TypeInfo);)
    };
    var_(data, u8) $flexible;
};

static fn_((ListSgl_Adp_empty(TypeInfo type, ListSgl_Adp$raw* ret_mem))(ListSgl_Adp$raw*));
static fn_((ListSgl_Adp_init(u_V$raw data, ListSgl_Adp$raw* ret_mem))(ListSgl_Adp$raw*));
static fn_((ListSgl_Adp_link(const ListSgl_Adp$raw* self))(P_const$ListSgl_Link));
static fn_((ListSgl_Adp_linkMut(ListSgl_Adp$raw* self))(P$ListSgl_Link));
static fn_((ListSgl_Adp_data(const ListSgl_Adp$raw* self, TypeInfo type))(u_P_const$raw));
static fn_((ListSgl_Adp_dataMut(ListSgl_Adp$raw* self, TypeInfo type))(u_P$raw));


#define ListSgl$$(_T...) __comp_anon__ListSgl$$(_T)
#define ListSgl$(_T...) __comp_alias__ListSgl$(_T)
#define T_use_ListSgl$(_T...) __comp_gen__T_use_ListSgl$(_T)
#define T_decl_ListSgl$(_T...) __comp_gen__T_decl_ListSgl$(_T)
#define T_impl_ListSgl$(_T...) __comp_gen__T_impl_ListSgl$(_T)

typedef struct ListSgl {
    var_(first, O$P$ListSgl_Link);
    debug_only(var_(type, TypeInfo);)
} ListSgl;

static fn_((ListSgl_empty(TypeInfo type))(ListSgl));
static fn_((ListSgl_prepend(ListSgl* self, P$ListSgl_Link before))(void));
static fn_((ListSgl_remove(ListSgl* self, P$ListSgl_Link node))(void));
static fn_((ListSgl_shift(ListSgl* self))(O$P$ListSgl_Link));
static fn_((ListSgl_len(const ListSgl* self))(usize));

/*========== Macros and Definitions =========================================*/

#define __comp_anon__ListSgl_Link$$(_T...) \
    union { \
        struct { \
            var_(next, O$$(P$$(ListSgl_Link$(_T)))); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ListSgl_Link) $like_ref; \
    }
#define __comp_alias__ListSgl_Link$(_T...) \
    pp_join($, ListSgl_Link, _T)
#define __comp_gen__T_use_ListSgl_Link$(_T...) \
    T_decl_ListSgl_Link$(_T); \
    T_impl_ListSgl_Link$(_T)
#define __comp_gen__T_decl_ListSgl_Link$(_T...) \
    $maybe_unused typedef union ListSgl_Link$(_T) ListSgl_Link$(_T); \
    T_use_P$(ListSgl_Link$(_T)); \
    T_decl_O$(P$(ListSgl_Link$(_T)))
#define __comp_gen__T_impl_ListSgl_Link$(_T...) \
    T_impl_O$(P$(ListSgl_Link$(_T))); \
    union ListSgl_Link$(_T) { \
        struct { \
            var_(next, O$(P$(ListSgl_Link$(_T)))); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ListSgl_Link) $like_ref; \
    }

/* clang-format off */
#define T_use_ListSgl_Link_empty$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_Link_empty, _T)(void))(ListSgl_Link$(_T))) { \
        return type$((ListSgl_Link$(_T))(ListSgl_Link_empty(typeInfo$(_T)))); \
    }
#define T_use_ListSgl_Link_insertNext$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_Link_insertNext, _T)(P$(ListSgl_Link$(_T)) self, P$(ListSgl_Link$(_T)) after))(void)) { \
        return ListSgl_Link_insertNext(self->as_raw, after->as_raw); \
    }
#define T_use_ListSgl_Link_removeNext$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_Link_removeNext, _T)(P$(ListSgl_Link$(_T)) self))(O$(P$(ListSgl_Link$(_T))))) { \
        return typeO$((O$(P$(ListSgl_Link$(_T))))(ListSgl_Link_removeNext(self->as_raw))); \
    }
#define T_use_ListSgl_Link_findLast$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_Link_findLast, _T)(P$(ListSgl_Link$(_T)) self))(P$(ListSgl_Link$(_T)))) { \
        return as$(P$(ListSgl_Link$(_T)))(ListSgl_Link_findLast(self->as_raw)); \
    }
#define T_use_ListSgl_Link_countChildren$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_Link_countChildren, _T)(P_const$(ListSgl_Link$(_T)) self))(usize)) { \
        return ListSgl_Link_countChildren(self->as_raw); \
    }
#define T_use_ListSgl_Link_reverse$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_Link_reverse, _T)(O$(P$(ListSgl_Link$(_T)))* indirect))(void)) { \
        return ListSgl_Link_reverse(as$(O$P$ListSgl_Link*)(indirect)); \
    }

#define T_use_ListSgl_Link_adp$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_Link_adp, _T)(P_const$(ListSgl_Link$(_T)) self))(const ListSgl_Adp$(_T)*)) { \
        return as$(const ListSgl_Adp$(_T)*)(ListSgl_Link_adp(self->as_raw)); \
    }
#define T_use_ListSgl_Link_adpMut$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_Link_adpMut, _T)(P$(ListSgl_Link$(_T)) self))(ListSgl_Adp$(_T)*)) { \
        return as$(ListSgl_Adp$(_T)*)(ListSgl_Link_adpMut(self->as_raw)); \
    }
#define T_use_ListSgl_Link_data$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_Link_data, _T)(P_const$(ListSgl_Link$(_T)) self))(const _T*)) { \
        return u_castP$((const _T*)(ListSgl_Link_data(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ListSgl_Link_dataMut$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_Link_dataMut, _T)(P$(ListSgl_Link$(_T)) self))(_T*)) { \
        return u_castP$((_T*)(ListSgl_Link_dataMut(self->as_raw, typeInfo$(_T)))); \
    }
/* clang-format on */

#define __comp_anon__ListSgl_Adp$$(_T...) \
    union { \
        struct { \
            struct { \
                var_(link, ListSgl_Link$(_T)); \
                debug_only(var_(type, TypeInfo);) \
            }; \
            var_(data, _T); \
        }; \
        var_(as_raw, ListSgl_Adp$raw) $like_ref; \
    }
#define __comp_alias__ListSgl_Adp$(_T) \
    pp_join($, ListSgl_Adp, _T)
#define __comp_gen__T_use_ListSgl_Adp$(_T) \
    T_decl_ListSgl_Adp$(_T); \
    T_impl_ListSgl_Adp$(_T)
#define __comp_gen__T_decl_ListSgl_Adp$(_T) \
    $maybe_unused typedef union ListSgl_Adp$(_T) ListSgl_Adp$(_T)
#define __comp_gen__T_impl_ListSgl_Adp$(_T) \
    union ListSgl_Adp$(_T) { \
        struct { \
            struct { \
                var_(link, ListSgl_Link$(_T)); \
                debug_only(var_(type, TypeInfo);) \
            }; \
            var_(data, _T); \
        }; \
        var_(as_raw, ListSgl_Adp$raw) $like_ref; \
    }

/* clang-format off */
#define T_use_ListSgl_Adp_empty$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_Adp_empty, _T)(void))(ListSgl_Adp$(_T))) { \
        return *as$(ListSgl_Adp$(_T)*)(ListSgl_Adp_empty(typeInfo$(_T), lit0$((ListSgl_Adp$(_T))).as_raw)); \
    }
#define T_use_ListSgl_Adp_init$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_Adp_init, _T)(_T data))(ListSgl_Adp$(_T))) { \
        return *as$(ListSgl_Adp$(_T)*)(ListSgl_Adp_init(u_anyV(data), lit0$((ListSgl_Adp$(_T))).as_raw)); \
    }
#define T_use_ListSgl_Adp_data$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_Adp_data, _T)(const ListSgl_Adp$(_T)* self))(const _T*)) { \
        return u_castP$((const _T*)(ListSgl_Adp_data(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ListSgl_Adp_dataMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_Adp_dataMut, _T)(ListSgl_Adp$(_T)* self))(_T*)) { \
        return u_castP$((_T*)(ListSgl_Adp_dataMut(self->as_raw, typeInfo$(_T)))); \
    }
/* clang-format on */

#define __comp_anon__ListSgl$$(_T...) \
    union { \
        struct { \
            var_(first, O$$(P$$(ListSgl_Link$(_T)))); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ListSgl) $like_ref; \
    }
#define __comp_alias__ListSgl$(_T...) \
    pp_join($, ListSgl, _T)
#define __comp_gen__T_use_ListSgl$(_T...) \
    T_decl_ListSgl$(_T); \
    T_impl_ListSgl$(_T)
#define __comp_gen__T_decl_ListSgl$(_T...) \
    $maybe_unused typedef union ListSgl$(_T) ListSgl$(_T)
#define __comp_gen__T_impl_ListSgl$(_T) \
    union ListSgl$(_T) { \
        struct { \
            var_(first, O$(P$(ListSgl_Link$(_T)))); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ListSgl) $like_ref; \
    }

/* clang-format off */
#define T_use_ListSgl_empty$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_empty, _T)(void))(ListSgl$(_T))) { \
        return type$((ListSgl$(_T))(ListSgl_empty(typeInfo$(_T)))); \
    }
#define T_use_ListSgl_prepend$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_prepend, _T)(ListSgl$(_T)* self, P$(ListSgl_Link$(_T)) before))(void)) { \
        return ListSgl_prepend(self->as_raw, before->as_raw); \
    }
#define T_use_ListSgl_remove$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_remove, _T)(ListSgl$(_T)* self, P$(ListSgl_Link$(_T)) node))(void)) { \
        return ListSgl_remove(self->as_raw, node->as_raw); \
    }
#define T_use_ListSgl_shift$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_shift, _T)(ListSgl$(_T)* self))(O$(P$(ListSgl_Link$(_T))))) { \
        return typeO$((O$(P$(ListSgl_Link$(_T))))(ListSgl_shift(self->as_raw))); \
    }
#define T_use_ListSgl_len$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListSgl_len, _T)(const ListSgl$(_T)* self))(usize)) { \
        return ListSgl_len(self->as_raw); \
    }
/* clang-format on */

#include "dh/io/stream.h"

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

    L_prepend(&list, &two.link); // {2}
    LLink_insertNext(&two.link, &five.link); // {2, 5}
    L_prepend(&list, &one.link); // {1, 2, 5}
    LLink_insertNext(&two.link, &three.link); // {1, 2, 3, 5}
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

    let_ignore = L_shift(&list); // {2, 3, 4, 5}
    L_remove(&list, &five.link); // {2, 3, 4}
    let_ignore = LLink_removeNext(&two.link); // {2, 4}

    try_(TEST_expect(pipe_(list.first, (unwrap_, ()), (LLink_data, ()), (deref, ())) == 2));
    // try_(TEST_expect(unwrap_(list.first)->data == 2));
    try_(TEST_expect(pipe_(list.first, (unwrap_, ()->next), (unwrap_, ()), (LLink_data, ()), (deref, ())) == 4));
    // // try_(TEST_expect(unwrap_(unwrap_(list.first)->next)->data == 4));
    try_(TEST_expect(pipe_(list.first, (unwrap_, ()->next), (unwrap_, ()->next), (isNone, ()))));
    // // try_(TEST_expect(isNone(unwrap_(unwrap_(list.first)->next)->next)));

    LLink_reverse(&list.first);
    {
        var it = list.first;
        u32 index = 1;
        while_some(it, node) blk_defer_({
            defer_(index += 1, it = node->next);
            io_stream_println(u8_l("index: {:u}, data: {:u}"), index, *LLink_data(node));
        }) blk_deferral;
    }

    try_(TEST_expect(pipe_(list.first, (unwrap_, ()), (LLink_data, ()), (deref, ())) == 4));
    // // try_(TEST_expect(unwrap_(list.first)->data == 4));
    try_(TEST_expect(pipe_(list.first, (unwrap_, ()->next), (unwrap_, ()), (LLink_data, ()), (deref, ())) == 2));
    // // try_(TEST_expect(unwrap_(unwrap_(list.first)->next)->data == 2));
    try_(TEST_expect(pipe_(list.first, (unwrap_, ()->next), (unwrap_, ()->next), (isNone, ()))));
    // // try_(TEST_expect(isNone(unwrap_(unwrap_(list.first)->next)->next)));
} $unguarded_(TEST_fn);


#if UNUSED_CODE
T_use_P$(TypeInfo);
T_use_S$(TypeInfo);
#define typeInfos$(_T...) A_ref$((S_const$TypeInfo)A_from$((TypeInfo){ pp_foreach(__typeInfos$__each, ~, _T) }))
#define __typeInfos$__each(_$ignored, _T...) typeInfo$(_T),

// let a = typeInfos$(u32, i32, usize);
#endif /* UNUSED_CODE */
