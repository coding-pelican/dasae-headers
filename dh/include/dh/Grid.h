/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Grid.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-11 (date of creation)
 * @updated 2025-02-22 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)
 * @prefix  Grid
 *
 * @brief   Using one-dimensional slice as two-dimensional grid
 * @details
 */

#ifndef GRID_INCLUDED
#define GRID_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/meta.h"

/*========== Macros and Definitions =========================================*/

typedef struct Grid_const Grid_const;
typedef union Grid        Grid;

#define Grid_const$(T) TYPE_UNNAMED__Grid_const$(T)
#define Grid$(T)       TYPE_UNNAMED__Grid$(T)
#define use_Grid$(T)   GEN__use_Grid$(T)
#define decl_Grid$(T)  GEN__decl_Grid$(T)
#define impl_Grid$(T)  GEN__impl_Grid$(T)

#define Grid_fromSli$(T_Grid, var_sli, u32_width, u32_height) OP__Grid_fromSli$(T_Grid, var_sli, u32_width, u32_height)
#define Grid_at(var_self, u32_x, u32_y)                       OP__Grid_at(pp_uniqTok(self), pp_uniqTok(x), pp_uniqTok(y), var_self, u32_x, u32_y)

/*========== Implementations ================================================*/

struct Grid_const {
    Sli_const items;
    u32       width;
    u32       height;
};
union Grid {
    Grid_const as_const;
    struct {
        Sli items;
        u32 width;
        u32 height;
    };
};

#define TYPE_UNNAMED__Grid_const$(T) \
    struct {                         \
        Sli_const$(T) items;         \
        u32 width;                   \
        u32 height;                  \
    }
#define TYPE_UNNAMED__Grid$(T)   \
    union {                      \
        Grid_const$(T) as_const; \
        struct {                 \
            Sli$(T) items;       \
            u32 width;           \
            u32 height;          \
        };                       \
    }
#define GEN__use_Grid$(T) \
    decl_Grid$(T);        \
    impl_Grid$(T)
#define GEN__decl_Grid$(T)                                              \
    typedef struct pp_join($, Grid_const, T) pp_join($, Grid_const, T); \
    typedef union pp_join($, Grid, T) pp_join($, Grid, T)
#define GEN__impl_Grid$(T)                  \
    struct pp_join($, Grid_const, T) {      \
        pp_join($, Sli_const, T) items;     \
        u32 width;                          \
        u32 height;                         \
    };                                      \
    union pp_join($, Grid, T) {             \
        pp_join($, Grid_const, T) as_const; \
        struct {                            \
            pp_join($, Sli, T) items;       \
            u32 width;                      \
            u32 height;                     \
        };                                  \
    }

#define OP__Grid_fromSli$(T_Grid, var_sli, u32_width, u32_height) eval({ \
    let       _sli    = var_sli;                                         \
    const u32 _width  = u32_width;                                       \
    const u32 _height = u32_height;                                      \
    debug_assert(_sli.len == as$(usize, _width) * _height);              \
    eval_return((T_Grid){                                                \
        .items  = _sli,                                                  \
        .width  = _width,                                                \
        .height = _height,                                               \
    });                                                                  \
})
#define OP__Grid_at(__self, __x, __y, var_self, u32_x, u32_y) eval({                            \
    let       __self = &(var_self);                                                             \
    const u32 __x    = u32_x;                                                                   \
    const u32 __y    = u32_y;                                                                   \
    debug_assert((__x) < (__self)->width);                                                      \
    debug_assert((__y) < (__self)->height);                                                     \
    eval_return Sli_at((__self)->items, as$(usize, __x) + (as$(usize, __y) * (__self)->width)); \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* GRID_INCLUDED */
