/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    AnyType.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-19 (date of creation)
 * @updated 2025-03-19 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix
 *
 * @brief
 * @details
 */

#ifndef ANY_TYPE_INCLUDED
#define ANY_TYPE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/ptr.h"
#include "dh/sli.h"
#include "dh/opt.h"
#include "dh/err_res.h"
#include "dh/union_enum.h"

/*========== Macros and Declarations ========================================*/

config_UnionEnum(AnyType,
    (AnyType_Ptr, struct {
        union {
            TypeInfo type;
            struct {
                u32 size;
                u32 align;
            };
        };
        anyptr addr;
    }),
    (AnyType_SliZ, struct {
        union {
            TypeInfo type;
            struct {
                u32 size;
                u32 align;
            };
        };
        anyptr addr;
    }),
    (AnyType_SliS, struct {
        union {
            TypeInfo type;
            struct {
                u32 size;
                u32 align;
            };
        };
        anyptr addr;
        anyptr sentinel;
    }),
    (AnyType_Sli, struct {
        union {
            TypeInfo type;
            struct {
                u32 size;
                u32 align;
            };
        };
        anyptr addr;
        usize len;
    }),
    (AnyType_Opt, struct {
        union {
            TypeInfo type;
            struct {
                u32 size;
                u32 align;
            };
        };
        bool has_value;
        anyptr addr;
    }),
    (AnyType_ErrRes, struct {
        union {
            TypeInfo type;
            struct {
                u32 size;
                u32 align;
            };
        };
        bool is_err;
        union {
            Err err;
            anyptr addr;
        } data;
    })
);

#define anyPtr(var_ptr...)        comp_op__anyPtr(var_ptr)
#define anySliZ(var_sli...)       comp_op__anySliZ(var_sli)
#define anySliS(var_sli...)       comp_op__anySliS(var_sli)
#define anySli(var_sli...)        comp_op__anySli(var_sli)
#define anyOpt(var_opt...)        comp_op__anyOpt(var_opt)
#define anyErrRes(var_err_res...) comp_op__anyErrSet(var_err_res)

/*========== Macros and Definitions =========================================*/

#define comp_op__anyPtr(var_ptr...) eval({                                                    \
    let __ptr = var_ptr;                                                                      \
    eval_return tagUnion$(AnyType, AnyType_Ptr, { .type = typeInfo$(TypeOf(*__ptr)), .addr = __ptr }); \
})
#define comp_op__anySliZ(var_sli...) eval({         \
    let __sli = var_sli;                            \
    eval_return tagUnion$(AnyType, AnyType_SliZ, {  }); \
})
#define comp_op__anySliS(var_sli...) eval({         \
    let __sli = var_sli;                            \
    eval_return tagUnion$(AnyType, AnyType_SliS, {  }); \
})
#define comp_op__anySli(var_sli...) eval({                                                                           \
    let __sli = var_sli;                                                                                             \
    eval_return tagUnion$(AnyType, AnyType_Sli, { .type = typeInfo$(TypeOf(*__sli.ptr)), .addr = __sli.ptr, .len = __sli.len }); \
})
#define comp_op__anyOpt(var_opt...) eval({                                                                                           \
    let __opt = var_opt;                                                                                                             \
    eval_return tagUnion(AnyType, AnyType_Opt, { \
        .type = typeInfo$(TypeOf(*__opt.value)), \
        .has_value = __opt.has_value, \
        .value = __opt.value \
    }); \
})
#define comp_op__anyErrSet(var_err_res...) eval({                                                                                             \
    let __err_res = var_err_res;                                                                                                              \
    eval_return tagUnion(AnyType, AnyType_ErrRes, { \
        .type = typeInfo$(TypeOf(*__err_res.data.ok)), \
        .is_err = __err_res.is_err, \
        .data = __err_res.data \
    }); \
})


#define OP__anyPtr(var_ptr...) eval({                                                                                                  \
    TypeOf(var_ptr) _ptr = var_ptr;                                                                                                    \
    claim_assert_static_msg(!isSameType$(TypeOf(_ptr), meta_Sli), "`meta_Sli` is not compatible with `anyPtr`. Use `meta_sliToAny`."); \
    claim_assert_static_msg(!isSameType$(TypeOf(_ptr), meta_Ptr), "`meta_Ptr` is not compatible with `anyPtr`. Use `meta_ptrToAny`."); \
    debug_assert_nonnull(_ptr);                                                                                                        \
    eval_return((AnyType){                                                                                                             \
        .type = typeInfo$(TypeOf(*_ptr)),                                                                                              \
        .ctx  = (void*)_ptr,                                                                                                           \
        .len  = 1,                                                                                                                     \
    });                                                                                                                                \
})

#define OP__anySli(var_sli...) eval({                                                                                                  \
    TypeOf(var_sli) _sli = var_sli;                                                                                                    \
    claim_assert_static_msg(!isSameType$(TypeOf(_sli), meta_Ptr), "`meta_Ptr` is not compatible with `anySli`. Use `meta_ptrToAny`."); \
    claim_assert_static_msg(!isSameType$(TypeOf(_sli), meta_Sli), "`meta_Sli` is not compatible with `anySli`. Use `meta_sliToAny`."); \
    debug_assert_nonnull(_sli.ptr);                                                                                                    \
    eval_return((AnyType){                                                                                                             \
        .type = typeInfo$(TypeOf(*_sli.ptr)),                                                                                          \
        .ctx  = (void*)_sli.ptr,                                                                                                       \
        .len  = _sli.len,                                                                                                              \
    });                                                                                                                                \
})

// (1-a)
// (a*b)
// (1-(1-a)(1-b))
// (1-(a*b))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ANY_TYPE_INCLUDED */
