#include "dh/core/prim.h"
#include "dh/debug/assert.h"
#include "dh/claim/assert.h"
#include "dh/claim/assert_static.h"
#include <stdalign.h>
#undef ref
#undef deref

/* Minimal type information */
typedef struct TypeInfo {
    usize size  : 52;
    usize align : 12;
} TypeInfo;
#define typeInfo(_T) (                                     \
    (TypeInfo){ .size = sizeof(_T), .align = alignof(_T) } \
)

/* Pointer to any type (void*) */
typedef struct AnyPtr {
    anyptr const addr;
} AnyPtr;

/* Slice of any type (void*) */
typedef struct AnySli {
    AnyPtr ptr;
    usize  len;
} AnySli;

/* Single/Many-item pointer (*T/[*]T in Zig) */
#define Ptr(T)                \
    union {                   \
        AnyPtr base;          \
        rawptr(T) const addr; \
    }

/* Slice ([]T in Zig) */
#define Sli(T)          \
    union {             \
        AnySli base;    \
        struct {        \
            Ptr(T) ptr; \
            usize len;  \
        };              \
    }

/* Sentinel-terminated pointer ([*:sentinel]T in Zig) */
#define SPtr(T)     \
    struct {        \
        Ptr(T);     \
        T sentinel; \
    }

/* Sentinel-terminated slice ([:sentinel]T in Zig) */
#define SSli(T)     \
    struct {        \
        Sli(T);     \
        T sentinel; \
    }


#define typedef_PtrTypes(T)   \
    typedef Ptr(T) Ptr_##T;   \
    typedef SPtr(T) SPtr_##T; \
    typedef Sli(T) Sli_##T;   \
    typedef SSli(T) SSli_##T;

/* Array ([N]T in Zig) */
#define Arr(N, T)   \
    struct {        \
        T items[N]; \
    }

// /* Initialization macros */
// #define Ptr_init(_addr)                           { .addr = (_addr) }
// #define SPtr_init(_addr, _sentinel)               { .addr = (_addr), .sentinel = (_sentinel) }
// #define Sli_init(_addr, _begin, _end)             { .ptr = Ptr_init((_addr) + (_begin)), .len = ((_end) - (_begin)) }
// #define SSli_init(_addr, _begin, _end, _sentinel) { .ptr = SPtr_init((_addr) + (_begin), _sentinel), .len = ((_end) - (_begin)) }

/* Pointers */
// Creations
#define Ptr_ref(T, _var)                 IMPL_Ptr_ref(T, _var)           /* &var // reference (with nonnull validation) */
#define Ptr_fromArr(T, _arr)             IMPL_Ptr_fromArr(T, _arr)       /* &arr // from array */
#define Ptr_fromInt(T, _usize_int)       IMPL_Ptr_fromInt(T, _usize_int) /* create from integer */
// Operators
#define Ptr_deref(T, _ptr)               IMPL_Ptr_deref(T, _ptr)               /* ptr.* // dereference (with nonnull validation) */
#define Ptr_at(T, _ptr, _index)          IMPL_Ptr_at(T, _ptr, _index)          /* ptr[index] // indexing */
#define Ptr_slice(T, _ptr, _begin, _end) IMPL_Ptr_slice(T, _ptr, _begin, _end) /* ptr[begin..end] // slicing */
#define Ptr_add(T, _ptr, _count)         IMPL_Ptr_add(T, _ptr, _count)         /* ptr + count // arithmetic */
#define Ptr_sub(T, _ptr, _count)         IMPL_Ptr_sub(T, _ptr, _count)         /* ptr - count // arithmetic */

/* Slices */
// Creations
#define Sli_fromPtr(T, _ptr, _begin, _end) IMPL_Sli_fromPtr(T, _ptr, _begin, _end) /* ptr[begin..end] // from pointer */
#define Sli_fromArr(T, _arr, _begin, _end) IMPL_Sli_fromArr(T, _arr, _begin, _end) /* arr[begin..end] // from array */
// Operators
#define Sli_at(T, _sli, _index)            IMPL_Sli_at(T, _sli, _index)          /* sli[index] // indexing (with index validation) */
#define Sli_slice(T, _sli, _begin, _end)   IMPL_Sli_slice(T, _sli, _begin, _end) /* sli[begin..end] // slicing (with index validation) */

/* Arrays */
// Operators
#define Arr_at(T, _arr, _index)          IMPL_Arr_at(T, _arr, _index)          /* arr[index] // indexing (with index validation) */
#define Arr_slice(T, _arr, _begin, _end) IMPL_Arr_slice(T, _arr, _begin, _end) /* arr[begin..end] // slicing (with index validation) */
#define Arr_ptr(T, _arr)                 IMPL_Arr_ptr(T, _arr)                 /* &arr // get pointer */

/* Helper macros */
force_inline        Nonnull(anyptr) ensureNonnull(Nonnull(anyptr) const p);
force_inline anyptr Ptr__at(TypeInfo type, AnyPtr ptr, usize index);
force_inline anyptr Sli__at(TypeInfo type, AnySli sli, usize index);
force_inline AnySli Sli__range(TypeInfo type, AnySli sli, usize begin, usize end);

/* Pointer operations */
#define IMPL_Ptr_ref(T, _var) ({ \
    rawptr(T) __ptr = &(_var);   \
    claim_assert_fmt(            \
        __ptr != null,           \
        "null reference"         \
    );                           \
    (Ptr_##T){ .addr = __ptr };  \
})

#define IMPL_Ptr_fromArr(T, _arr) \
    (Ptr_##_T) { .addr = &(_arr) }

#define IMPL_Ptr_fromInt(T, _addr) \
    (Ptr_##_T) { .addr = (rawptr(T))(_addr) }

#define IMPL_Ptr_deref(T, _ptr) ({ \
    claim_assert_fmt(              \
        (_ptr).addr != null,       \
        "null pointer dereference" \
    );                             \
    *((rawptr(T))(_ptr).addr);     \
})

#define IMPL_Ptr_at(T, _ptr, _index) \
    *(rawptr(T))Ptr__at(typeInfo(T), (_ptr).base, _index, sizeof(T))

#define IMPL_Ptr_slice(T, _ptr, _begin, _end) \
    (Sli_##T) { .ptr = (_ptr), .len = (_end) - (_begin) }

#define IMPL_Ptr_add(T, _ptr, _count) \
    (Ptr_##T) { .addr = (rawptr(T))(_ptr).addr + (_count) }

#define IMPL_Ptr_sub(T, _ptr, _count) \
    (Ptr_##T) { .addr = (rawptr(T))(_ptr).addr - (_count) }

/* Slice operations */
#define IMPL_Sli_fromPtr(T, _ptr, _begin, _end) \
    (Sli_##T) { .ptr = { .addr = (rawptr(T))(_ptr).addr + (_begin) }, .len = (_end) - (_begin) }

#define IMPL_Sli_fromArr(T, _arr, _begin, _end) \
    Sli_fromPtr(T, Ptr_fromArr(T, _arr), _begin, _end)

#define IMPL_Sli_at(T, _sli, _index) \
    *(rawptr(T))Sli__at(typeInfo(T), (_sli).base, _index, sizeof(T))

#define IMPL_Sli_slice(T, _sli, _begin, _end) \
    (Sli_##T) Sli__range(typeInfo(T), (_sli).base, _begin, _end)

/* Array operations */
#define IMPL_Arr_at(T, _arr, _index) ({                   \
    const usize __len = sizeof((_arr).items) / sizeof(T); \
    claim_assert_fmt(                                     \
        (_index) < __len,                                 \
        "index out of bounds: index %zu, len %zu",        \
        _index,                                           \
        __len                                             \
    );                                                    \
    (_arr).items[_index];                                 \
})

#define IMPL_Arr_slice(T, _arr, _begin, _end) ({                                      \
    const usize __len = sizeof((_arr).items) / sizeof(T);                             \
    claim_assert_fmt(                                                                 \
        (_begin) <= (_end) && (_end) <= __len,                                        \
        "slice out of bounds: begin %zu, end %zu, len %zu",                           \
        _begin,                                                                       \
        _end,                                                                         \
        __len                                                                         \
    );                                                                                \
    (Sli_##_T){ .ptr = { .addr = &(_arr).items[_begin] }, .len = (_end) - (_begin) }; \
})

#define IMPL_Arr_ptr(T, _arr) \
    (Ptr_##T) { .addr = (_arr).items }

/* Helper functions */

force_inline Nonnull(anyptr) ensureNonnull(Nonnull(anyptr) const p) {
    return claim_assert_nonnull(p), p;
}

force_inline anyptr Ptr__at(TypeInfo type, AnyPtr ptr, usize index) {
    return ((u8*)ptr.addr) + (index * type.size);
}

force_inline anyptr Sli__at(TypeInfo type, AnySli sli, usize index) {
    claim_assert_fmt(
        index < sli.len,
        "index out of bounds: index %zu, len %zu",
        index,
        sli.len
    );
    return Ptr__at(type, sli.ptr, index);
}

force_inline AnySli Sli__range(TypeInfo type, AnySli sli, usize begin, usize end) {
    claim_assert_fmt(
        begin <= end && end <= sli.len,
        "slice out of bounds: begin %zu, end %zu, len %zu",
        begin,
        end,
        sli.len
    );
    return (AnySli){
        .ptr = { .addr = Ptr__at(type, sli.ptr, begin) },
        .len = end - begin
    };
}

/* Examples */
typedef_PtrTypes(u8);
typedef_PtrTypes(i32);

/* String types and helpers */
typedef SSli_u8      Str;
typedef SSliConst_u8 StrConst;
#define u8StrRaw(str)      ((u8[]){ str })
#define u8StrRawConst(str) ((const u8*)(str))
#define SSli_u8(str)       ((SSli_u8){ { u8StrRaw(str), '\0' }, sizeof(str) - 1 })
#define SSliConst_u8(str)  ((SSliConst_u8){ { u8StrRawConst(str), '\0' }, sizeof(str) - 1 })
#define Str(str)           ((Str){ { u8StrRaw(str), '\0' }, sizeof(str) - 1 })
#define StrConst(str)      ((StrConst){ { u8StrRawConst(str), '\0' }, sizeof(str) - 1 })

void test1(void) {
    const SSli_u8 ssli_str = {
        .ptr = {
            .addr = (u8[]){ 'h', 'e', 'l', 'l', 'o', '\0' },
        },
        .len      = 5,
        .sentinel = '\0'
    };
    debug_assert(ssli_str.len == 5);
    debug_assert(ssli_str.ptr.addr[0] == 'h');
    debug_assert(ssli_str.ptr.addr[1] == 'e');
    debug_assert(ssli_str.ptr.addr[2] == 'l');
    debug_assert(ssli_str.ptr.addr[3] == 'l');
    debug_assert(ssli_str.ptr.addr[4] == 'o');
    debug_assert(ssli_str.ptr.addr[5] == '\0');
    debug_assert(ssli_str.sentinel == '\0');
    debug_assert(ssli_str.ptr.addr[ssli_str.len] == ssli_str.sentinel);
    printf("%s\n", ssli_str.ptr.addr);

    for (usize i = 0; i < ssli_str.len; ++i) {
        ssli_str.ptr.addr[i]++;
    }
    for (usize i = 0; i < ssli_str.len; ++i) {
        printf("%c", ssli_str.ptr.addr[i]);
    }
    printf("\n");
}

void test2() {
    i32                array[] = { 1, 2, 3, 4, 5 };
    const Ptr_i32      p_arr   = ptr(Ptr_i32, array);
    const PtrConst_i32 pc_arr  = ptr(PtrConst_i32, array);
    debug_assert(p_arr.addr == array);
    debug_assert(pc_arr.addr == array);
    debug_assert(p_arr.addr == pc_arr.addr);
    for (usize i = 0; i < 5; ++i) {
        debug_assert(p_arr.addr[i] == array[i]);
        debug_assert(pc_arr.addr[i] == array[i]);
        debug_assert(p_arr.addr[i] == pc_arr.addr[i]);
    }
    for (usize i = 0; i < 5; ++i) {
        printf("%d ", p_arr.addr[i]);
    }
    printf("\n");

    for (usize i = 0; i < 5; ++i) {
        p_arr.addr[i]++;
    }
    for (usize i = 0; i < 5; ++i) {
        printf("%d ", pc_arr.addr[i]);
    }
    printf("\n");

    const Sli_i32      sli_arr  = sli(Sli_i32, array, 0, 5);
    const SliConst_i32 slic_arr = sli(SliConst_i32, array, 0, 5);
    debug_assert(sli_arr.ptr.addr == array);
    debug_assert(slic_arr.ptr.addr == array);
    debug_assert(sli_arr.ptr.addr == slic_arr.ptr.addr);
    for (usize i = 0; i < sli_arr.len; ++i) {
        debug_assert(sli_arr.ptr.addr[i] == array[i]);
        debug_assert(slic_arr.ptr.addr[i] == array[i]);
        debug_assert(sli_arr.ptr.addr[i] == slic_arr.ptr.addr[i]);
    }
    for (usize i = 0; i < sli_arr.len; ++i) {
        printf("%d ", sli_arr.ptr.addr[i]);
    }
    printf("\n");

    const Sli_i32 sli_pre = sli(Sli_i32, sli_arr.ptr.addr, 0, 2);
    const Sli_i32 sli_suf = sli(Sli_i32, sli_arr.ptr.addr, 2, 5);
    for (usize i = 0; i < sli_pre.len; ++i) {
        printf("%d ", sli_pre.ptr.addr[i]);
    }
    for (usize i = 0; i < sli_suf.len; ++i) {
        printf("%d ", sli_suf.ptr.addr[i]);
    }
    printf("\n");

    SSliConst_u8 str = StrConst("Hello, world!");
    for (usize index = 0; index < str.len; ++index) {
        printf("%c", Sli_at(str, index));
    }
    printf("\n");
}

int main(void) {
    test1();
    test2();
    // Ptr_i32 p = points(create(i32, 1));
    // Ptr_i32 q = points(p->value);
    // p->value  = 2;
    // q->value  = 3;
    // debug_assert(q->value == 3);
    // debug_assert(p->value == q->value);
    // Ptr_f64 r = points(create(f64, 1.0));
    // Ptr_f64 s = points(r->value);
    // r->value  = 2.0;
    // s->value  = 3.0;
    // debug_assert(s->value == 3.0);
    // debug_assert(r->value == s->value);

    // p = q;
    // r = s;
    // // p = s; /* compile error: incompatible pointer types */
    // // r = q; /* compile error: incompatible pointer types */

    // debug_assert(sizeof(p->value) != sizeof(r->value));
    // debug_assert(alignof(p->value) != alignof(r->value));

    // PtrConst_i32 n = pointsConst(create(i32, 1));
    // Ptr_i32      m = points(n->value); /* compile error: incompatible pointer types discards qualifiers */
    return 0;
}
