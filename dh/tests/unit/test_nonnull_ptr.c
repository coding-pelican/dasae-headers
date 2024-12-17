#include "dh/core/prim.h"
#include "dh/debug/assert.h"
#include "dh/claim/assert.h"
#include "dh/claim/assert_static.h"
#include <stdalign.h>

#undef ref

/* Single-item pointer (*T in Zig) */
#define Ref(T)   \
    struct {     \
        T value; \
    }*
#define RefConst(T) const Ref(T)

/* Array ([N]T in Zig) */
#define Arr(N, T)             \
    struct {                  \
        T           items[N]; \
        const usize len;      \
    }
#define ArrConst(N, T)        \
    struct {                  \
        const T     items[N]; \
        const usize len;      \
    }

/* Many-item pointer ([*]T in Zig) */
#define Ptr(T)         \
    struct {           \
        T* const addr; \
    }
#define PtrConst(T)          \
    struct {                 \
        const T* const addr; \
    }
typedef struct Ptr {
    anyptr const addr;
} Ptr;
typedef struct PtrConst {
    const anyptr addr;
} PtrConst;

/* Sentinel-terminated pointer ([*:sentinel]T in Zig) */
#define SPtr(T)            \
    struct {               \
        T* const addr;     \
        const T  sentinel; \
    }
#define SPtrConst(T)             \
    struct {                     \
        const T* const addr;     \
        const T        sentinel; \
    }

/* Slice ([]T in Zig) */
#define Sli(T)             \
    struct {               \
        const struct {     \
            T* const addr; \
        } ptr;             \
        const usize len;   \
    }
#define SliConst(T)              \
    struct {                     \
        const struct {           \
            const T* const addr; \
        } ptr;                   \
        const usize len;         \
    }
typedef struct Sli {
    anyptr const addr;
    const usize  len;
} Sli;
typedef struct SliConst {
    const anyptr const addr;
    const usize        len;
} SliConst;

/* Sentinel-terminated slice ([:sentinel]T in Zig) */
#define SSli(T)                \
    struct {                   \
        const struct {         \
            T* const addr;     \
            const T  sentinel; \
        } ptr;                 \
        const usize len;       \
    }
#define SSliConst(T)                 \
    struct {                         \
        const struct {               \
            const T* const addr;     \
            const T        sentinel; \
        } ptr;                       \
        const usize len;             \
    }

force_inline Nonnull(anyptr) ensureNonnull(Nonnull(anyptr) const p) {
    return claim_assert_nonnull(p), p;
}

#define ref(TRef, var)                          ((TRef){ ensureNonnull(&(var)) })
#define ptr(TPtr, addr)                         ((TPtr){ (addr) })
#define sptr(TSPtr, sentinel, addr)             ((TSPtr){ (addr), (sentinel) })
#define sli(TSli, addr, begin, end)             ((TSli){ { (addr) + (begin) }, (end) - (begin) })
#define ssli(TSSli, sentinel, addr, begin, end) ((TSSli){ { (addr) + (begin), (sentinel) }, (end) - (begin) })

#define Sli_at(sli, i) ((sli).ptr.addr[(debug_assert((i) < (sli).len), (i))])

// force_inline const Nonnull(anyptr) ensureNonnullConst(const Nonnull(anyptr) const p) {
//     return claim_assert_nonnull(p), p;
// }
// #define points(var)      ensureNonnull(&(var))
// #define pointsConst(var) ensureNonnullConst(&(var))

typedef Ref(i32) Ref_i32;
typedef RefConst(i32) RefConst_i32;
typedef Ptr(i32) Ptr_i32;
typedef PtrConst(i32) PtrConst_i32;
typedef SPtr(u8) SPtr_u8;
typedef SPtrConst(u8) SPtrConst_u8;
typedef Sli(i32) Sli_i32;
typedef SliConst(i32) SliConst_i32;
typedef SSli(u8) SSli_u8;
typedef SSliConst(u8) SSliConst_u8;

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
    i32           n     = 123000;
    const Ref_i32 ref_n = ref(Ref_i32, n);
    debug_assert(ref_n->value == n);
    ref_n->value += 456;
    debug_assert(ref_n->value == 123456);
    debug_assert(n == 123456);
    debug_assert(ref_n->value == n);
    printf("%d\n", ref_n->value);

    const SSli_u8 ssli_str = ssli(SSli_u8, '\0', u8StrRaw("hello"), 0, 5);
    debug_assert(ssli_str.len == 5);
    debug_assert(ssli_str.ptr.addr[0] == 'h');
    debug_assert(ssli_str.ptr.addr[1] == 'e');
    debug_assert(ssli_str.ptr.addr[2] == 'l');
    debug_assert(ssli_str.ptr.addr[3] == 'l');
    debug_assert(ssli_str.ptr.addr[4] == 'o');
    debug_assert(ssli_str.ptr.addr[5] == '\0');
    debug_assert(ssli_str.ptr.sentinel == '\0');
    debug_assert(ssli_str.ptr.addr[ssli_str.len] == ssli_str.ptr.sentinel);
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

/*========== Macros and Definitions =========================================*/

#define anyptr                            \
    /**                                   \
     * @brief Pointer to any type (void*) \
     */                                   \
    IMPL_anyptr

/* Null definition */
#ifndef NULL
#ifdef __cplusplus
#ifndef _WIN64 /* _WIN32 */
#define NULL (0)
#else /* _WIN64 */
#define NULL (0ll)
#endif
#else
#define NULL ((void*)0)
#endif
#endif /* NULL */
/* Null pointer definition */
#if defined(__cplusplus)
#if (__cplusplus >= 201103L)
/* C++11 or later - nullptr is available */
#define null nullptr
#else
/* Pre-C++11 */
#define nullptr NULL
#define null    nullptr
#endif
#else
/* C */
#define nullptr NULL
#define null    nullptr
#endif
/* Nullable-Nonnull attributes */
#define nullable       IMPL_nullable
#define nonnull        IMPL_nonnull
#define Nullable(TPtr) IMPL_Nullable(TPtr)
#define Nonnull(TPtr)  IMPL_Nonnull(TPtr)

/* Utils */
#define initial(_Inital...)     IMPL_initial(_Inital)
#define cleared()               IMPL_cleared()
#define make(_T, _Inital...)    IMPL_make(_T, _Inital)
#define makeCleared(_T)         IMPL_makeCleared(_T)
#define create(_T, _Initial...) IMPL_create(_T, _Initial)
#define createCleared(_T)       IMPL_createCleared(_T)
#define createFrom(_T, _var...) IMPL_createFrom(_T, _var)
#define getContainer(_ptr_member, _TContainer)                        \
    /**                                                               \
     * Get struct pointer from member pointer.                        \
     * this for _ptr_member same as _MemberName.                      \
     *                                                                \
     * _ptr_member: the pointer that point struct's member.           \
     *            it's the member address offset from struct address. \
     *                                                                \
     */                                                               \
    IMPL_getContainer(_ptr_member, _TContainer)
#define getContainerByFieldName(_ptr_member, _TContainer, _MemberName) \
    /**                                                                \
     * Get struct pointer from member pointer with _MemberName.        \
     * this for _ptr_member not same as _MemberName.                   \
     *                                                                 \
     * _ptr_member: the pointer that point struct's member.            \
     *            it's the member address offset from struct address.  \
     */                                                                \
    IMPL_getContainerByFieldName(_ptr_member, _TContainer, _MemberName)

/*========== Macros Implementation ==========================================*/

#define IMPL_anyptr void*

#if defined(__clang__)
#define IMPL_nullable _Nullable
#define IMPL_nonnull  _Nonnull
#elif defined(__GNUC__)
#define IMPL_nullable __attribute__((nullable))
#define IMPL_nonnull  __attribute__((nonnull))
#else
#define IMPL_nullable
#define IMPL_nonnull
#endif
#define IMPL_Nullable(TPtr) TPtr nullable
#define IMPL_Nonnull(TPtr)  TPtr nonnull

#define IMPL_initial(_Inital...) \
    { _Inital }
#define IMPL_cleared() \
    { 0 }
// NOLINTBEGIN(bugprone-macro-parentheses)
#define IMPL_make(_T, _Inital...) \
    ((_T){ _Inital })
#define IMPL_makeCleared(_T) \
    ((_T){ 0 })
#define IMPL_create(_T, _Inital...) \
    ((_T[1]){ (_T){ _Inital } })
#define IMPL_createCleared(_T) \
    ((_T[1]){ (_T){ 0 } })
#define IMPL_createFrom(_T, _var...) \
    ((_T[1]){ _var })
// NOLINTEND(bugprone-macro-parentheses)
#define IMPL_getContainer(_ptr_member, _TContainer) \
    ((Parent_TContainerType*)((u8*)(_ptr_member) - offsetof(_TContainer, _ptr_member)))
#define IMPL_getContainerByFieldName(_ptr_member, _TContainer, _MemberName) \
    ((_TContainer*)((u8*)(_ptr_member) - offsetof(_TContainer, _MemberName)))
