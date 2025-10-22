/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    AnyType.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-19 (date of creation)
 * @updated 2025-03-19 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   AnyType is a type that can be used to store any type of data.
 * @details It is a union of all types that can be stored in a variable.
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
#include "dh/variant.h"

/*========== Macros and Declarations ========================================*/

typedef variant_(
    (AnyType),
    (AnyType_ptr, struct {
        union {
            TypeInfo type;
            struct {
                u32 size;
                u32 align;
            };
        };
        anyptr addr;
    }),
    (AnyType_sli_z, struct {
        union {
            TypeInfo type;
            struct {
                u32 size;
                u32 align;
            };
        };
        anyptr addr;
    }),
    (AnyType_sli_s, struct {
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
    (AnyType_sli, struct {
        union {
            TypeInfo type;
            struct {
                u32 size;
                u32 align;
            };
        };
        anyptr addr;
        usize  len;
    }),
    (AnyType_opt, struct {
        union {
            TypeInfo type;
            struct {
                u32 size;
                u32 align;
            };
        };
        bool   has_value;
        anyptr addr;
    }),
    (AnyType_err_res, struct {
        union {
            TypeInfo type;
            struct {
                u32 size;
                u32 align;
            };
        };
        bool is_err;
        union {
            Err    err;
            anyptr addr;
        } data;
    })
) AnyType;

#define anyPtr(var_ptr...)        comp_op__anyPtr(var_ptr)
#define anySliZ(var_sli...)       comp_op__anySliZ(var_sli) /* Not implemented */
#define anySliS(var_sli...)       comp_op__anySliS(var_sli) /* Not implemented */
#define anySli(var_sli...)        comp_op__anySli(var_sli)
#define anyOpt(var_opt...)        comp_op__anyOpt(var_opt)
#define anyErrRes(var_err_res...) comp_op__anyErrSet(var_err_res)

/*========== Macros and Definitions =========================================*/

#define comp_op__anyPtr(var_ptr...) eval({ \
    let __ptr = var_ptr; \
    claim_assert_static_msg(!isSameType$(TypeOf(__ptr), meta_Ptr), "`meta_Ptr` is not compatible with `anyPtr`. Use `meta_ptrToAny`."); \
    claim_assert_static_msg(!isSameType$(TypeOf(__ptr), meta_Sli), "`meta_Sli` is not compatible with `anyPtr`. Use `meta_sliToAny`."); \
    eval_return variant_of$(AnyType, AnyType_ptr, { .type = typeInfo$(TypeOf(*__ptr)), .addr = __ptr }); \
})
#define comp_op__anySliZ(var_sli...) eval({ \
    let __sli = var_sli; \
    eval_return variant_of$(AnyType, AnyType_sli_z, {}); \
})
#define comp_op__anySliS(var_sli...) eval({ \
    let __sli = var_sli; \
    eval_return variant_of$(AnyType, AnyType_sli_s, {}); \
})
#define comp_op__anySli(var_sli...) eval({ \
    let __sli = var_sli; \
    claim_assert_static_msg(!isSameType$(TypeOf(__sli), meta_Ptr), "`meta_Ptr` is not compatible with `anySli`. Use `meta_ptrToAny`."); \
    claim_assert_static_msg(!isSameType$(TypeOf(__sli), meta_Sli), "`meta_Sli` is not compatible with `anySli`. Use `meta_sliToAny`."); \
    eval_return variant_of$(AnyType, AnyType_sli, { .type = typeInfo$(TypeOf(*__sli.ptr)), .addr = __sli.ptr, .len = __sli.len }); \
})
#define comp_op__anyOpt(var_opt...) eval({ \
    let __opt = var_opt; \
    eval_return variant_of(AnyType, AnyType_opt, { .type = typeInfo$(TypeOf(*__opt.value)), .has_value = __opt.has_value, .value = __opt.value }); \
})
#define comp_op__anyErrSet(var_err_res...) eval({ \
    let __err_res = var_err_res; \
    eval_return variant_of(AnyType, AnyType_err_res, { .type = typeInfo$(TypeOf(*__err_res.data.ok)), .is_err = __err_res.is_err, .data = __err_res.data }); \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ANY_TYPE_INCLUDED */
