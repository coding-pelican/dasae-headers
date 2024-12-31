#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"

/* Any type */
typedef struct AnyType AnyType;
#define AnyType(var_ptr...) IMPL_AnyType(var_ptr)

/* Generic type */
typedef const anyptr    PtrConst;
typedef anyptr          Ptr;
typedef struct SliConst SliConst;
typedef union Sli       Sli;
extern Ptr              Ptr_constCast(PtrConst);
extern Sli              Sli_constCast(SliConst);

/* Meta types */
typedef struct meta_PtrConst meta_PtrConst;
typedef union meta_Ptr       meta_Ptr;
typedef struct meta_SliConst meta_SliConst;
typedef union meta_Sli       meta_Sli;
extern meta_Ptr              meta_Ptr_constCast(meta_PtrConst);
extern meta_Sli              meta_Sli_constCast(meta_SliConst);

/* Implementation any type */
struct AnyType {
    anyptr   ctx;
    TypeInfo type;
};
#define IMPL_AnyType(var_ptr...) ({          \
    let _var_ptr = var_ptr;                  \
    claim_assert_nonnull(_var_ptr);          \
    (AnyType){                               \
        .ctx  = _var_ptr,                    \
        .type = typeInfo(TypeOf(*_var_ptr)), \
    };                                       \
})

/* Implementation generic type */
struct SliConst {
    PtrConst ptr;
    usize    len;
};
union Sli {
    SliConst as_const;
    struct {
        Ptr   ptr;
        usize len;
    };
};

/* Implementation meta types */
struct meta_PtrConst {
    const anyptr addr;
    TypeInfo     type;
};
union meta_Ptr {
    meta_PtrConst as_const;
    struct {
        anyptr   addr;
        TypeInfo type;
    };
};
struct meta_SliConst {
    union {
        meta_PtrConst ptr;
        struct {
            const anyptr addr;
            TypeInfo     type;
        };
    };
    usize len;
};
union meta_Sli {
    meta_SliConst as_const;
    struct {
        union {
            meta_Ptr ptr;
            struct {
                anyptr   addr;
                TypeInfo type;
            };
        };
        usize len;
    };
};

typedef struct SliConst$u8 {
    u8*   ptr;
    usize len;
} SliConst$u8;
typedef union Sli$u8 {
    SliConst$u8 as_const;
    struct {
        u8*   ptr;
        usize len;
    };
} Sli$u8;

/* Constructors with proper type handling */
#define Sli_from(var_ptr, val_len)     { .ptr = (var_ptr), .len = (val_len) }
#define Sli_from$(T, var_ptr, val_len) ({ \
    let _ptr = var_ptr;                   \
    claim_assert_nonnull(_ptr);           \
    (T){                                  \
        .ptr = _ptr,                      \
        .len = (val_len)                  \
    };                                    \
})

#define Sli_range(var_ptr, val_begin, val_end)     { .ptr = (var_ptr) + (val_begin), .len = (val_end) - (val_begin) }
#define Sli_range$(T, var_ptr, val_begin, val_end) ({                                      \
    let   _ptr   = var_ptr;                                                                \
    usize _begin = val_begin;                                                              \
    usize _end   = val_end;                                                                \
    claim_assert_nonnull(_ptr);                                                            \
    claim_assert_fmt(_begin < _end, "Invalid range (begin: %zu, end: %zu)", _begin, _end); \
    (T){                                                                                   \
        .ptr = _ptr + _begin,                                                              \
        .len = _end - _begin                                                               \
    };                                                                                     \
})
#define Sli_arr(var_arr)     { .ptr = (var_arr), .len = countOf(var_arr) }
#define Sli_arr$(T, var_arr) ({ \
    let _arr = (var_arr);       \
    claim_assert_nonnull(_arr); \
    (pp_join($, Sli, T)){       \
        .ptr = _arr,            \
        .len = countOf(_arr)    \
    };                          \
})

/* Core operations */
extern anyptr Sli_rawAt(TypeInfo, anyptr, usize, usize);
extern anyptr Sli_rawSlice(TypeInfo, anyptr, usize, usize, usize);

/* Type-safe access macros */
#define Sli_at(var_sli, val_index) (({                                                          \
    let _sli = var_sli;                                                                         \
    (TypeOf(_sli.ptr)) Sli_rawAt(typeInfo(TypeOf(*(_sli.ptr))), _sli.ptr, _sli.len, val_index); \
})[0])

#define Sli_slice(var_sli, val_begin, val_end) ({                                             \
    let   _sli   = var_sli;                                                                   \
    usize _begin = val_begin;                                                                 \
    usize _end   = val_end;                                                                   \
    (TypeOf(_sli)){                                                                           \
        .ptr = Sli_rawSlice(typeInfo(TypeOf(*(_sli.ptr))), _sli.ptr, _sli.len, _begin, _end), \
        .len = _end - _begin                                                                  \
    };                                                                                        \
})

#define Sli_prefix(var_sli, val_end) ({ \
    let _sli = var_sli;                 \
    Sli_slice(_sli, 0, val_end);        \
})

#define Sli_suffix(sli, val_begin) ({     \
    let _sli = var_sli;                   \
    Sli_slice(_sli, val_begin, _sli.len); \
})

/* Iterator */
#define for_sli(slice, item)                   \
    for (usize _i = 0; _i < (slice).len; ++_i) \
    scope_with(let item = Sli_at(slice, _i))


// force_inline void* Ptr_at(Ptr self, usize index) {
//     claim_assert_nonnull(self.addr);
//     return (u8*)self.addr + (index * self.type.size);
// }

// force_inline const void* PtrConst_at(PtrConst self, usize index) {
//     claim_assert_nonnull(self.addr);
//     return (const u8*)self.addr + (index * self.type.size);
// }

// // Slice operations
// force_inline void* Sli_at(Sli self, usize index) {
//     claim_assert_nonnull(self.ptr.addr);
//     claim_assert_fmt(index < self.len, "Index out of bounds (len: %zu, index: %zu)", self.len, index);
//     return Ptr_at(self.ptr, index);
// }

// force_inline SliConst SliConst_slice(SliConst self, usize begin, usize end) {
//     claim_assert_nonnull(self.ptr.addr);
//     claim_assert_fmt(begin <= end, "Invalid slice range (begin: %zu, end: %zu)", begin, end);
//     claim_assert_fmt(begin <= self.len, "Slice begin out of bounds (len: %zu, begin: %zu)", self.len, begin);
//     claim_assert_fmt(end <= self.len, "Slice end out of bounds (len: %zu, end: %zu)", self.len, end);
//     return (SliConst){
//         .ptr = { PtrConst_at(self.ptr, begin), self.ptr.type },
//         .len = end - begin
//     };
// }

// force_inline const void* SliConst_at(SliConst self, usize index) {
//     claim_assert_nonnull(self.ptr.addr);
//     claim_assert_fmt(index < self.len, "Index out of bounds (len: %zu, index: %zu)", self.len, index);
//     return PtrConst_at(self.ptr, index);
// }

// force_inline Sli Sli_slice(Sli self, usize begin, usize end) {
//     claim_assert_nonnull(self.ptr.addr);
//     claim_assert_fmt(begin <= end, "Invalid slice range (begin: %zu, end: %zu)", begin, end);
//     claim_assert_fmt(begin <= self.len, "Slice begin out of bounds (len: %zu, begin: %zu)", self.len, begin);
//     claim_assert_fmt(end <= self.len, "Slice end out of bounds (len: %zu, end: %zu)", self.len, end);
//     return (Sli){
//         .ptr = { Ptr_at(self.ptr, begin), self.ptr.type },
//         .len = end - begin
//     };
// }
