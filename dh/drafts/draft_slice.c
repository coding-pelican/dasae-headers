#include "dh/core.h"
#include "dh/scope.h"
#include "dh/mem/common.h"

force_inline usize mem_slen(TypeInfo type, anyptr addr, anyptr sentinel) {
    debug_assert_nonnull(addr);
    debug_assert_nonnull(sentinel);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_fmt(
        mem_isAligned(as(usize, addr), type.align),
        "Address must be properly aligned (addr: %p, type.align: %zu)",
        addr,
        type.align
    );
    debug_assert_fmt(
        mem_isAligned(as(usize, sentinel), type.align),
        "Sentinel must be properly aligned (sentinel: %p, type.align: %zu)",
        sentinel,
        type.align
    );
    usize len  = 0;
    u8*   curr = as(u8*, addr);
    while (mem_cmp(curr, sentinel, type.size) != 0) {
        len++;
        curr += type.size;
    }
    return len;
}

typedef struct Sli {
    void* ptr;
    usize len;
} Sli;

typedef struct SliConst {
    const void* ptr;
    usize       len;
} SliConst;

typedef struct SliS {
    void* ptr;
    usize len;
    void* sentinel;
} SliS;

typedef struct SliSConst {
    const void* ptr;
    usize       len;
    void*       sentinel;
} SliSConst;

/* Sli: a reference to a contiguous sequence of values */
#define Sli(T)     \
    struct {       \
        T*    ptr; \
        usize len; \
    }

#define SliConst(T)   \
    struct {          \
        const T* ptr; \
        usize    len; \
    }

#define SliS(T)         \
    struct {            \
        T*    ptr;      \
        usize len;      \
        T     sentinel; \
    }

#define SliSConst(T)       \
    struct {               \
        const T* ptr;      \
        usize    len;      \
        T        sentinel; \
    }

#define using_Sli(T) \
    decl_Sli(T);     \
    impl_Sli(T)

#define decl_Sli(T)                                                 \
    typedef struct pp_join($, Sli, T) pp_join($, Sli, T);           \
    typedef struct pp_join($, SliConst, T) pp_join($, SliConst, T); \
    typedef struct pp_join($, SliS, T) pp_join($, SliS, T);         \
    typedef struct pp_join($, SliSConst, T) pp_join($, SliSConst, T)

#define impl_Sli(T)                   \
    struct pp_join($, Sli, T) {       \
        T*    ptr;                    \
        usize len;                    \
    };                                \
    struct pp_join($, SliConst, T) {  \
        const T* ptr;                 \
        usize    len;                 \
    };                                \
    struct pp_join($, SliS, T) {      \
        T*    ptr;                    \
        usize len;                    \
        T     sentinel;               \
    };                                \
    struct pp_join($, SliSConst, T) { \
        const T* ptr;                 \
        usize    len;                 \
        T        sentinel;            \
    }

/* Slice creation (similar to Zig's [start..end])*/
#define slice(arr, start, end)                                              \
    {                                                                       \
        .ptr = ({                                                           \
            var _arr   = (arr);                                             \
            let _start = (start);                                           \
            debug_assert_nonnull(_arr);                                     \
            debug_assert_fmt(                                               \
                mem_isAligned(as(usize, _arr), alignof(TypeOf((_arr)[0]))), \
                "Address must be properly aligned "                         \
                "(addr: %p, align: %zu)",                                   \
                (const void*)(_arr),                                        \
                alignof(TypeOf((_arr)[0]))                                  \
            );                                                              \
            &(_arr)[_start];                                                \
        }),                                                                 \
        .len = ({                                                           \
            let _start = (start);                                           \
            let _end   = (end);                                             \
            debug_assert_fmt(                                               \
                (_start) < (_end),                                          \
                "Begin index must be less than end index "                  \
                "(begin: %zu, end: %zu)",                                   \
                (usize)(_start),                                            \
                (usize)(_end)                                               \
            );                                                              \
            (_end) - (_start);                                              \
        })                                                                  \
    }

/* Sentinel operations */
#define sliceS(_ptr, _sentinel)                                              \
    {                                                                        \
        .ptr      = (_ptr),                                                  \
        .len      = mem_slen(typeInfo(TypeOf(*(_ptr))), _ptr, &(_sentinel)), \
        .sentinel = (_sentinel)                                              \
    }

/* Access operations with detailed assertions */
#define Sli_at(slice, index)                \
    ({                                      \
        var _slice = (slice);               \
        let _index = (index);               \
        debug_assert_nonnull((_slice).ptr); \
        debug_assert_fmt(                   \
            (_index) < (_slice).len,        \
            "Index out of bounds "          \
            "(index: %zu, len: %zu)",       \
            (usize)(_index),                \
            (_slice).len                    \
        );                                  \
        &(_slice).ptr[_index];              \
    })

/* Iterator support with scope (similar to Zig's for loops over slices) */
#define for_slice(slice, item)                 \
    for (usize _i = 0; _i < (slice).len; ++_i) \
    scope_with(let item = Sli_at(slice, _i))

// Example usage:
using_Sli(i32);
static i32 sumSlice(SliConst$i32 numbers) {
    i32 total = 0;
    for_slice(numbers, num) {
        total += *num;
    };
    return total;
}

void example(void) {
    // Fixed arrays
    const i32    numbers[] = { 1, 2, 3, 4, 5 };
    // Create slices
    SliConst$i32 slice1    = slice(numbers, 1, 4); // [2, 3, 4]
    SliConst$i32 slice2    = slice(numbers, 2, 5); // [3, 4, 5]

    for_slice(slice1, num) {
        printf("%d ", *num);
    };
    printf("\n");
    for_slice(slice2, num) {
        printf("%d ", *num);
    };
    printf("\n");

    // Multi-dimensional arrays and slices
    i32 matrix[3][3] = {
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 }
    };
    unused(matrix);

    // MdSli(i32, 2) matrix_slice = {
    //     .ptr = (Sli(i32)[]){
    //         slice(matrix[0], 0, 3),
    //         slice(matrix[1], 0, 3),
    //         slice(matrix[2], 0, 3),
    //     },
    //     .len = 3
    // };

    // Mutable slices
    let     mutable_numbers = (i32[]){ 1, 2, 3, 4, 5 };
    Sli$i32 mutable_slice   = slice(mutable_numbers, 1, 4);
    mutable_slice.ptr[1]    = 10; // Modifies original array

    // Using with string literals (const slices)
    SliConst(u8) str = slice((u8*)"Hello, World!", 0, 5); // "Hello"

    SliSConst(u8) cstr = sliceS((u8*)"hello, world", '\n');
    unused(str);
}

int main(void) {
    example();
    return 0;
}
