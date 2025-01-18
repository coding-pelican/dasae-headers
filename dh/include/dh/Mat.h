/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Mat.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-11 (date of creation)
 * @updated 2025-01-18 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  Mat
 *
 * @brief   Using one-dimensional slice as two-dimensional matrix
 * @details
 */

#ifndef MAT_INCLUDED
#define MAT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/meta.h"

/*========== Macros and Definitions =========================================*/

typedef struct MatConst MatConst;
typedef union Mat       Mat;

#define use_Mat$(T)  GEN__use_Mat$(T)
#define decl_Mat$(T) GEN__decl_Mat$(T)
#define impl_Mat$(T) GEN__impl_Mat$(T)

#define Mat_fromSli$(TMat, var_sli, u32_width, u32_height) OP__Mat_fromSli$(TMat, var_sli, u32_width, u32_height)
#define Mat_at(var_self, u32_x, u32_y)                     OP__Mat_at(pp_uniqueToken(_self), pp_uniqueToken(_x), pp_uniqueToken(_y), var_self, u32_x, u32_y)

/*========== Implementations ================================================*/

struct MatConst {
    SliConst items;
    u32      width;
    u32      height;
};
union Mat {
    MatConst as_const;
    struct {
        Sli items;
        u32 width;
        u32 height;
    };
};

#define GEN__use_Mat$(T) \
    decl_Mat$(T);        \
    impl_Mat$(T)
#define GEN__decl_Mat$(T)                                           \
    typedef struct pp_join($, MatConst, T) pp_join($, MatConst, T); \
    typedef union pp_join($, Mat, T) pp_join($, Mat, T)
#define GEN__impl_Mat$(T)                 \
    struct pp_join($, MatConst, T) {      \
        pp_join($, SliConst, T) items;    \
        u32 width;                        \
        u32 height;                       \
    };                                    \
    union pp_join($, Mat, T) {            \
        pp_join($, MatConst, T) as_const; \
        struct {                          \
            pp_join($, Sli, T) items;     \
            u32 width;                    \
            u32 height;                   \
        };                                \
    }

#define OP__Mat_fromSli$(TMat, var_sli, u32_width, u32_height) eval( \
    let       _sli    = var_sli;                                     \
    const u32 _width  = u32_width;                                   \
    const u32 _height = u32_height;                                  \
    debug_assert(_sli.len == as$(usize, _width) * _height);          \
    eval_return((TMat){                                              \
        .items  = _sli,                                              \
        .width  = _width,                                            \
        .height = _height,                                           \
    });                                                              \
)
#define OP__Mat_at(_self, _x, _y, var_self, u32_x, u32_y) eval(                             \
    let       _self = &(var_self);                                                          \
    const u32 _x    = u32_x;                                                                \
    const u32 _y    = u32_y;                                                                \
    debug_assert((_x) < (_self)->width);                                                    \
    debug_assert((_y) < (_self)->height);                                                   \
    eval_return Sli_at((_self)->items, as$(usize, _x) + (_y) * as$(usize, (_self)->width)); \
)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MAT_INCLUDED */
