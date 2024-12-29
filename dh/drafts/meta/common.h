#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"

/* Generic type */
#define PtrConst$(T) const T*
#define Ptr$(T)      T*

#define using_PtrConst$(T) \
    typedef PtrConst$(T) pp_join($, PtrConst, T)
#define using_Ptr$(T) \
    typedef Ptr$(T) pp_join($, Ptr, T)

#define SliConst$(T)      \
    struct {              \
        PtrConst$(T) ptr; \
        usize len;        \
    }
#define Sli$(T)      \
    struct {         \
        Ptr$(T) ptr; \
        usize len;   \
    }

#define using_SliConst$(T) \
    decl_SliConst$(T);     \
    impl_SliConst$(T)
#define using_Sli$(T) \
    decl_Sli$(T);     \
    impl_Sli$(T)

#define decl_SliConst$(T) \
    typedef struct pp_join($, SliConst, T) pp_join($, SliConst, T)
#define decl_Sli$(T) \
    typedef struct pp_join($, Sli, T) pp_join($, Sli, T)

#define impl_SliConst$(T)            \
    struct pp_join($, SliConst, T) { \
        PtrConst$(T) ptr;            \
        usize len;                   \
    }
#define impl_Sli$(T)            \
    struct pp_join($, Sli, T) { \
        Ptr$(T) ptr;            \
        usize len;              \
    }

/* Any type */
typedef struct AnyType {
    anyptr   ctx;
    TypeInfo type;
} AnyType;

#define AnyPtr(var_ptr) \
    ((AnyType){ .ctx = &(var_ptr), .type = typeInfo(*(var_ptr)) })

#define AnySli(var_sli) \
    ((AnyType){ .ctx = &(var_sli), .type = typeInfo(*((var_sli).ptr)) })

/* Meta types */
typedef struct meta_PtrConst meta_PtrConst;
typedef union meta_Ptr       meta_Ptr;
typedef struct meta_SliConst meta_SliConst;
typedef union meta_Sli       meta_Sli;

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

using_Opt$(meta_PtrConst);
using_Opt$(meta_Ptr);
using_Opt$(meta_SliConst);
using_Opt$(meta_Sli);

using_Err$(meta_PtrConst);
using_Err$(meta_Ptr);
using_Err$(meta_SliConst);
using_Err$(meta_Sli);

#define meta_ptr(var_ptr...) ({      \
    let _var_ptr = var_ptr;          \
    (meta_Ptr){                      \
        .addr = _var_ptr,            \
        .type = typeInfo(*_var_ptr), \
    };                               \
})
#define meta_sli(var_sli...) ({              \
    let _var_sli = var_sli;                  \
    (meta_Sli){                              \
        .ptr = {                             \
            .addr = _var_sli.ptr,            \
            .type = typeInfo(*_var_sli.ptr), \
        },                                   \
        .len = _var_sli.len,                 \
    };                                       \
})
#define meta_castPtr(T, ptr...) ({ \
    let _ptr = ptr;                \
    (T)((_ptr).addr);              \
})
#define meta_castSli(T, sli...) ({                \
    let _sli = sli;                               \
    (T){ .ptr = (_sli).addr, .len = (_sli).len }; \
})
