/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Mat.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-11 (date of creation)
 * @updated 2025-01-11 (date of last update)
 * @version v0.1-alpha
 * @ingroup voxel-space
 * @prefix  Mat
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

typedef struct Mat {
    Sli   items;
    usize width;
    usize height;
} Mat;
#define use_Mat$(T)                                            GEN__use_Mat$(T)
#define decl_Mat$(T)                                           GEN__decl_Mat$(T)
#define impl_Mat$(T)                                           GEN__impl_Mat$(T)
#define Mat_fromSli$(TMat, var_sli, usize_width, usize_height) OP__Mat_fromSli$(TMat, var_sli, usize_width, usize_height)
#define Mat_at(var_self, usize_x, usize_y)                     OP__Mat_at(pp_uniqueToken(_self), pp_uniqueToken(_x), pp_uniqueToken(_y), var_self, usize_x, usize_y)

/*========== Implementations ================================================*/

#define GEN__use_Mat$(T) \
    decl_Mat$(T);        \
    impl_Mat$(T)
#define GEN__decl_Mat$(T) \
    typedef struct pp_join($, Mat, T) pp_join($, Mat, T)
#define GEN__impl_Mat$(T)         \
    struct pp_join($, Mat, T) {   \
        pp_join($, Sli, T) items; \
        usize width;              \
        usize height;             \
    }
#define OP__Mat_fromSli$(TMat, var_sli, usize_width, usize_height) eval( \
    let         _sli    = var_sli;                                       \
    const usize _width  = usize_width;                                   \
    const usize _height = usize_height;                                  \
    debug_assert(_sli.len == _width * _height);                          \
    eval_return((TMat){                                                  \
        .items  = _sli,                                                  \
        .width  = _width,                                                \
        .height = _height,                                               \
    });                                                                  \
)
#define OP__Mat_at(_self, _x, _y, var_self, usize_x, usize_y) eval( \
    let         _self = var_self;                                   \
    const usize _x    = usize_x;                                    \
    const usize _y    = usize_y;                                    \
    debug_assert((_x) < (_self).width);                             \
    debug_assert((_y) < (_self).height);                            \
    eval_return Sli_at((_self).items, (_x) + (_y) * (_self).width); \
)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MAT_INCLUDED */
