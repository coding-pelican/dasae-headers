// meta_Value_load
// meta_Value_store

#include "dh/main.h"
#include "dh/Arr.h"
#include "dh/meta.h"

#define use_Optptr$(T) \
    use_Ptr$(T); \
    decl_Optptr$(T); \
    impl_Optptr$(T)
#define decl_Optptr$(T) \
    typedef struct Optptr_const$(T) Optptr_const$(T); \
    typedef union Optptr$(T) Optptr$(T)
#define impl_Optptr$(T) \
    struct Optptr_const$(T) { \
        const rawptr$(T) addr; \
    }; \
    union Optptr$(T) { \
        struct { \
            rawptr$(T) addr; \
        }; \
        Optptr_const$(T) as_const; \
    }

#define Optptr_const$(T) pp_join($, Optptr_const, T)
#define Optptr$(T)       pp_join($, Optptr, T)
#define Optptr_const$$(T) \
    struct { \
        const rawptr$(T) addr; \
    }
#define Optptr$$(T) \
    union { \
        struct { \
            rawptr$(T) addr; \
        }; \
        Optptr_const$$(T) as_const; \
    }

#define someptr(...)                         { .addr = __VA_ARGS__ }
#define noneptr()                            { .addr = null }
#define someptr$(T, ...)                     ((T)someptr(__VA_ARGS__))
#define noneptr$(T)                          ((T)noneptr())
#define toSomeptr(var_addr_opt, val_some...) eval({ \
    let __addr_opt = var_addr_opt; \
    debug_assert_nonnull(__addr_opt); \
    *__addr_opt = someptr$(TypeOf(*__addr_opt), val_some); \
    eval_return __addr_opt; \
})
#define toNoneptr(var_addr_opt...) eval({ \
    let __addr_opt = var_addr_opt; \
    debug_assert_nonnull(__addr_opt); \
    *__addr_opt = noneptr$(TypeOf(*__addr_opt)); \
    eval_return __addr_opt; \
})

#define unwrapptr(var_ptr) eval({ \
    let __ptr = var_ptr; \
    debug_assert_nonnull(__ptr.addr); \
    eval_return __ptr.addr; \
})
#define orelseptr(var_ptr, val_orelse) eval({ \
    let __ptr = var_ptr; \
    eval_return __ptr.addr ? __ptr.addr : val_orelse; \
})

typedef struct meta_Value {
    var_(ref, meta_Ptr);
    var_(data[0], u8);
} meta_Value;

typedef struct Node Node;
use_Optptr$(Node);
struct Node {
    var_(next, Optptr$Node);
    var_(data, meta_Value);
};

fn_(Node_init(Node* self, meta_Ptr ptr), Node*) {
    toNoneptr(&self->next);
    self->data.ref.type = ptr.type;
    self->data.ref.addr = self->data.data;
    memcpy(self->data.data, ptr.addr, ptr.type.size);
    return self;
}

fn_(Node_data(Node* self), meta_Value) {
    return self->data;
}

typedef union Node$i32 Node$i32;
use_Optptr$(Node$i32);
union Node$i32 {
    var_(base[1], Node);
    struct {
        var_(next, Optptr$Node$i32);
        struct {
            var_(typeInfo, TypeInfo);
            var_(bytes, u8*);
            var_(data, i32);
        };
    };
};

#define meta_Value_load$(T, _self...) eval({ \
    union { \
        meta_Value meta; \
        struct { \
            meta_Ptr ref; \
            T        data; \
        } value; \
    } __self = { .meta = _self }; \
    debug_assert(TypeInfo_eq(typeInfo$(T), __self.meta.ref.type)); \
    __self.value.data = deref(as$(rawptr$(T), __self.meta.ref.addr)); \
    eval_return __self.value.data; \
})

#define $TEST_only TEST_only
#if !defined(COMP_TEST)
#define $release_only(...) __VA_ARGS__
#else
#define $release_only(...)
#endif

#define only_else

// clang-format off
$TEST_only(TEST_fn_("meta_Value_load", $scope))
$release_only(fn_(dh_main(Sli$Str_const args), Err$void, $scope)) {
    $release_only($ignore = args;)

    var_type(node, Node$i32*, Node_init(make$(Node$i32).base, meta_create$(i32, 123)));
    i32 x = meta_Value_load$(i32, Node_data(node->base));
    try_(TEST_expect(x == 123));

    toSomeptr(&node->next, node);
    i32 y = meta_Value_load$(i32, pipe(node->next,(unwrapptr,()->base),(Node_data,())));
    try_(TEST_expect(y == 123));

    $release_only(return_ok({});)
} $TEST_only($unscoped_TEST)
$release_only($unscoped);
// clang-format on
