/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Grid.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-11 (date of creation)
 * @updated 2025-03-15 (date of last update)
 * @version v0.1-alpha.3
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
#include "dh/sli.h"

/*========== Macros and Declarations ========================================*/

typedef struct Grid_const Grid_const;
typedef union Grid        Grid;

#define Grid_const$(T)                      comp_type_anon__Grid_const$(T)
#define Grid$(T)                            comp_type_anon__Grid$(T)
#define Grid_anonCast$(T_Grid, var_anon...) comp_op__Grid_anonCast$(pp_uniqTok(anon), T_Grid, var_anon)

#define use_Grid$(T)  comp_gen__use_Grid$(T)
#define decl_Grid$(T) comp_gen__decl_Grid$(T)
#define impl_Grid$(T) comp_gen__impl_Grid$(T)

#define Grid_fromSli$(T_Grid, var_sli, u32_width, u32_height...) comp_op__Grid_fromSli$(pp_uniqTok(sli), pp_uniqTok(width), pp_uniqTok(height), T_Grid, var_sli, u32_width, u32_height)

#define Grid_width(var_self...)                         comp_op__Grid_width(pp_uniqTok(self), var_self)
#define Grid_height(var_self...)                        comp_op__Grid_height(pp_uniqTok(self), var_self)
#define Grid_at(var_self, u32_x, u32_y...)              comp_op__Grid_at(pp_uniqTok(self), pp_uniqTok(x), pp_uniqTok(y), var_self, u32_x, u32_y)
#define Grid_getAt(var_self, u32_x, u32_y...)           comp_op__Grid_getAt(pp_uniqTok(self), pp_uniqTok(x), pp_uniqTok(y), var_self, u32_x, u32_y)
#define Grid_setAt(var_self, u32_x, u32_y, val_item...) comp_op__Grid_setAt(pp_uniqTok(self), pp_uniqTok(x), pp_uniqTok(y), var_self, u32_x, u32_y, val_item)

/*========== Macros and Definitions =========================================*/

struct Grid_const {
    Sli_const items;
    u32       width;
    u32       height;
};
union Grid {
    struct {
        Sli items;
        u32 width;
        u32 height;
    };
    Grid_const as_const;
};

#define comp_type_anon__Grid_const$(T) \
    struct {                           \
        Sli_const$(T) items;           \
        u32 width;                     \
        u32 height;                    \
    }
#define comp_type_anon__Grid$(T) \
    union {                      \
        struct {                 \
            Sli$(T) items;       \
            u32 width;           \
            u32 height;          \
        };                       \
        Grid_const$(T) as_const; \
    }
#define comp_op__Grid_anonCast$(__anon, T_Grid, var_anon...) eval({                            \
    const TypeOf(var_anon) __anon = var_anon;                                                  \
    claim_assert_static(sizeOf(TypeOf(__anon)) == sizeOf(T_Grid));                             \
    claim_assert_static(alignOf(TypeOf(__anon)) == alignOf(T_Grid));                           \
    claim_assert_static(fieldAnonTypeCastable(T_Grid, __anon, Sli, items));                    \
    claim_assert_static(validateField(__anon, width, FieldTypeOf(u32, width)));                \
    claim_assert_static(validateField(__anon, height, FieldTypeOf(u32, height)));              \
    claim_assert_static(fieldPadding(TypeOf(__anon), items) == fieldPadding(T_Grid, items));   \
    claim_assert_static(fieldPadding(TypeOf(__anon), width) == fieldPadding(T_Grid, width));   \
    claim_assert_static(fieldPadding(TypeOf(__anon), height) == fieldPadding(T_Grid, height)); \
    eval_return rawderef(as$(rawptr$(T_Grid), &__anon));                                       \
})

#define comp_gen__use_Grid$(T) \
    decl_Grid$(T);             \
    impl_Grid$(T)
#define comp_gen__decl_Grid$(T)                                         \
    typedef struct pp_join($, Grid_const, T) pp_join($, Grid_const, T); \
    typedef union pp_join($, Grid, T) pp_join($, Grid, T)
#define comp_gen__impl_Grid$(T)             \
    struct pp_join($, Grid_const, T) {      \
        pp_join($, Sli_const, T) items;     \
        u32 width;                          \
        u32 height;                         \
    };                                      \
    union pp_join($, Grid, T) {             \
        struct {                            \
            pp_join($, Sli, T) items;       \
            u32 width;                      \
            u32 height;                     \
        };                                  \
        pp_join($, Grid_const, T) as_const; \
    }

#define comp_op__Grid_fromSli$(__sli, __width, __height, T_Grid, var_sli, u32_width, u32_height...) eval({ \
    const TypeOf(var_sli) __sli = var_sli;                                                                 \
    const u32 __width           = u32_width;                                                               \
    const u32 __height          = u32_height;                                                              \
    debug_assert_fmt(                                                                                      \
        __sli.len == as$(usize, __width) * __height,                                                       \
        "width and height mismatch: %zu != %zu * %zu",                                                     \
        __sli.len,                                                                                         \
        __width,                                                                                           \
        __height                                                                                           \
    );                                                                                                     \
    eval_return make$(                                                                                     \
        T_Grid,                                                                                            \
        .items  = __sli,                                                                                   \
        .width  = __width,                                                                                 \
        .height = __height                                                                                 \
    );                                                                                                     \
})

#define comp_op__Grid_width(__self, var_self...) eval({ \
    const TypeOf(var_self) __self = var_self;           \
    eval_return __self.width;                           \
})
#define comp_op__Grid_height(__self, var_self...) eval({ \
    const TypeOf(var_self) __self = var_self;            \
    eval_return __self.height;                           \
})
#define comp_op__Grid_at(__self, __x, __y, var_self, u32_x, u32_y...) eval({ \
    const TypeOf(var_self) __self = var_self;                                \
    const u32 __x                 = u32_x;                                   \
    const u32 __y                 = u32_y;                                   \
    debug_assert_fmt(                                                        \
        __x < __self.width,                                                  \
        "X out of bounds: %zu >= %zu",                                       \
        __x,                                                                 \
        __self.width                                                         \
    );                                                                       \
    debug_assert_fmt(                                                        \
        __y < __self.height,                                                 \
        "Y out of bounds: %zu >= %zu",                                       \
        __y,                                                                 \
        __self.height                                                        \
    );                                                                       \
    eval_return Sli_at(                                                      \
        __self.items,                                                        \
        as$(usize, __x) + (as$(usize, __y) * __self.width)                   \
    );                                                                       \
})
#define comp_op__Grid_getAt(__self, __x, __y, var_self, u32_x, u32_y...) eval({ \
    const TypeOf(var_self) __self = var_self;                                   \
    const u32 __x                 = u32_x;                                      \
    const u32 __y                 = u32_y;                                      \
    debug_assert_fmt(                                                           \
        __x < __self.width,                                                     \
        "X out of bounds: %zu >= %zu",                                          \
        __x,                                                                    \
        __self.width                                                            \
    );                                                                          \
    debug_assert_fmt(                                                           \
        __y < __self.height,                                                    \
        "Y out of bounds: %zu >= %zu",                                          \
        __y,                                                                    \
        __self.height                                                           \
    );                                                                          \
    eval_return Sli_getAt(                                                      \
        __self.items,                                                           \
        as$(usize, __x) + (as$(usize, __y) * __self.width)                      \
    );                                                                          \
})
#define comp_op__Grid_setAt(__self, __x, __y, var_self, u32_x, u32_y, val_item...) eval({ \
    const TypeOf(var_self) __self = var_self;                                             \
    const u32 __x                 = u32_x;                                                \
    const u32 __y                 = u32_y;                                                \
    debug_assert_fmt(                                                                     \
        __x < __self.width,                                                               \
        "X out of bounds: %zu >= %zu",                                                    \
        __x,                                                                              \
        __self.width                                                                      \
    );                                                                                    \
    debug_assert_fmt(                                                                     \
        __y < __self.height,                                                              \
        "Y out of bounds: %zu >= %zu",                                                    \
        __y,                                                                              \
        __self.height                                                                     \
    );                                                                                    \
    eval_return Sli_setAt(                                                                \
        __self.items,                                                                     \
        as$(usize, __x) + (as$(usize, __y) * __self.width),                               \
        val_item                                                                          \
    );                                                                                    \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* GRID_INCLUDED */
