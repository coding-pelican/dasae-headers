/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Grid.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-11 (date of creation)
 * @updated 2025-03-15 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers-examples(dh-examples)
 * @prefix  Grid
 *
 * @brief   Using one-dimensional slice as two-dimensional grid
 * @details Provides a grid implementation using a one-dimensional slice as a two-dimensional grid.
 */
#ifndef Grid__included
#define Grid__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/prl.h>

/*========== Macros and Declarations ========================================*/

typedef struct Grid_const$raw Grid_const$raw;
typedef union Grid$raw Grid$raw;

#define T_decl_Grid$(T) comp_gen__decl_Grid$(T)
#define T_impl_Grid$(T) comp_gen__impl_Grid$(T)
#define T_use_Grid$(T) comp_gen__use_Grid$(T)

#define Grid_const$(T) comp_type_alias__Grid_const$(T)
#define Grid$(T) comp_type_alias__Grid$(T)
#define Grid_const$$(T) comp_type_anon__Grid_const$$(T)
#define Grid$$(T) comp_type_anon__Grid$$(T)

#define Grid_fromS$(T_Grid, var_sli, u32_width, u32_height...) comp_op__Grid_fromS$(pp_uniqTok(sli), pp_uniqTok(width), pp_uniqTok(height), T_Grid, var_sli, u32_width, u32_height)

#define Grid_width(var_self...) comp_op__Grid_width(var_self)
#define Grid_height(var_self...) comp_op__Grid_height(var_self)
#define Grid_at(var_self, u32_x, u32_y...) comp_op__Grid_at(pp_uniqTok(self), pp_uniqTok(x), pp_uniqTok(y), var_self, u32_x, u32_y)

#define for_grid(_Grid, _Tuple_Captures /* _Tuple_Item_w_Tuple_X_Y_Idx */, _Body...) \
    /** \
     * for_grid(grid, (item)) \
     * for_grid(grid, (item, (x, y))) \
     * for_grid(grid, (item, (x, y), (width, height))) \
     * for_grid(grid, (item, (x, y, idx))) \
     * for_grid(grid, (item, (x, y, idx), (width, height, len))) \
     */ \
    pp_overload(__for_grid, pp_Tuple_unwrap _Tuple_Captures)(_Grid, _Tuple_Captures, _Body)
#define __for_grid_1(_Grid, _Tuple_Item, _Body...) \
    comp_syn__for_grid_item(_Grid, _Tuple_Item, _Body)
#define __for_grid_2(_Grid, _Tuple_Item_w_Tuple_Captures, _Body...) \
    pp_overload(__for_grid_2, pp_Tuple_unwrap pp_Tuple_get2nd _Tuple_Item_w_Tuple_Captures)(_Grid, _Tuple_Item_w_Tuple_Captures, _Body)
#define __for_grid_2_2(_Grid, _Tuple_Item_w_Tuple_X_Y, _Body...) \
    comp_syn__for_grid_item_w_tuple_x_y(_Grid, _Tuple_Item_w_Tuple_X_Y, _Body)
#define __for_grid_2_3(_Grid, _Tuple_Item_w_Tuple_X_Y_Idx, _Body...) \
    comp_syn__for_grid_item_w_tuple_x_y_idx(_Grid, _Tuple_Item_w_Tuple_X_Y_Idx, _Body)
#define __for_grid_3(_Grid, _Tuple_Item_w_Tuple_Captures, _Body...) \
    pp_overload(__for_grid_3, pp_Tuple_unwrap pp_Tuple_get2nd _Tuple_Item_w_Tuple_Captures)(_Grid, _Tuple_Item_w_Tuple_Captures, _Body)
#define __for_grid_3_2(_Grid, _Tuple_Item_w_Tuple_X_Y_w_Tuple_Width_Height, _Body...) \
    comp_syn__for_grid_item_w_tuple_x_y_w_tuple_width_height(_Grid, _Tuple_Item_w_Tuple_X_Y_w_Tuple_Width_Height, _Body)
#define __for_grid_3_3(_Grid, _Tuple_Item_w_Tuple_X_Y_Idx_w_Tuple_Width_Height_Len, _Body...) \
    comp_syn__for_grid_item_w_tuple_x_y_idx_w_tuple_width_height_len(_Grid, _Tuple_Item_w_Tuple_X_Y_Idx_w_Tuple_Width_Height_Len, _Body)

/*========== Macros and Definitions =========================================*/

typedef struct Grid_const$raw {
    S_const$raw items;
    u32 width;
    u32 height;
} Grid_const$raw;
typedef union Grid$raw {
    struct {
        S$raw items;
        u32 width;
        u32 height;
    };
    Grid_const$raw as_const;
} Grid$raw;

#define comp_gen__use_Grid$(T) \
    T_decl_Grid$(T); \
    T_impl_Grid$(T)
#define comp_gen__decl_Grid$(T) \
    $maybe_unused typedef struct pp_join($, Grid_const, T) pp_join($, Grid_const, T); \
    $maybe_unused typedef union pp_join($, Grid, T) pp_join($, Grid, T)
#define comp_gen__impl_Grid$(T) \
    struct Grid_const$(T) { \
        var_(items, S$(const T)); \
        u32 width; \
        u32 height; \
    }; \
    union Grid$(T) { \
        struct { \
            var_(items, S$(T)); \
            u32 width; \
            u32 height; \
        }; \
        Grid_const$(T) as_const; \
    }

#define comp_type_alias__Grid_const$(T) pp_join($, Grid_const, T)
#define comp_type_alias__Grid$(T) pp_join($, Grid, T)
#define comp_type_anon__Grid_const$$(T) \
    struct { \
        var_(items, S$$(const T)); \
        u32 width; \
        u32 height; \
    }
#define comp_type_anon__Grid$$(T) \
    struct { \
        var_(items, S$$(T)); \
        u32 width; \
        u32 height; \
    };

#define comp_op__Grid_fromS$(__sli, __width, __height, T_Grid, var_sli, u32_width, u32_height...) blk({ \
    const TypeOf(var_sli) __sli = var_sli; \
    const u32 __width = intCast$((u32)(u32_width)); \
    const u32 __height = intCast$((u32)(u32_height)); \
    debug_assert_fmt( \
        __sli.len == as$(usize)(__width) * __height, \
        "width and height mismatch: %zu != %zu * %zu", \
        __sli.len, \
        __width, \
        __height \
    ); \
    blk_return_(lit$((T_Grid){ \
        .items = __sli, \
        .width = __width, \
        .height = __height, \
    })); \
})

#define comp_op__Grid_width(var_self...) ((var_self).width)
#define comp_op__Grid_height(var_self...) ((var_self).height)
#define comp_op__Grid_at(__self, __x, __y, var_self, u32_x, u32_y...) blk({ \
    let __self = var_self; \
    const u32 __x = intCast$((u32)(u32_x)); \
    const u32 __y = intCast$((u32)(u32_y)); \
    debug_assert_fmt( \
        __x < __self.width, \
        "X out of bounds: %zu >= %zu", \
        __x, \
        __self.width \
    ); \
    debug_assert_fmt( \
        __y < __self.height, \
        "Y out of bounds: %zu >= %zu", \
        __y, \
        __self.height \
    ); \
    blk_return_(S_at((__self.items)[as$(usize)(__x) + ((as$(usize)(__y)) * __self.width)])); \
})

#define comp_syn__for_grid_item(_Grid, _Tuple_Item, _Body...) blk({ \
    let __grid = _Grid; \
    const u32 __width = Grid_width(__grid); \
    const u32 __height = Grid_height(__grid); \
    for (u32 __y = 0; __y < __height; ++__y) { \
        for (u32 __x = 0; __x < __width; ++__x) { \
            let _Tuple_Item = Grid_at(__grid, __x, __y); \
            _Body; \
        } \
    } \
})
#define comp_syn__for_grid_item_w_tuple_x_y(_Grid, _Tuple_Item_w_Tuple_X_Y, _Body...) blk({ \
    let __grid = _Grid; \
    const u32 __width = Grid_width(__grid); \
    const u32 __height = Grid_height(__grid); \
    for (u32 __y = 0; __y < __height; ++__y) { \
        for (u32 __x = 0; __x < __width; ++__x) { \
            let pp_Tuple_get1st _Tuple_Item_w_Tuple_X_Y \
                = Grid_at(__grid, __x, __y); \
            let pp_exec_expand( \
                pp_exec_defer(pp_Tuple_get1st) pp_Tuple_get2nd _Tuple_Item_w_Tuple_X_Y \
            ) = __x; \
            let pp_exec_expand( \
                pp_exec_defer(pp_Tuple_get2nd) pp_Tuple_get2nd _Tuple_Item_w_Tuple_X_Y \
            ) = __y; \
            _Body; \
        } \
    } \
})
#define comp_syn__for_grid_item_w_tuple_x_y_idx(_Grid, _Tuple_Item_w_Tuple_X_Y_Idx, _Body...) blk({ \
    let __grid = _Grid; \
    const u32 __width = Grid_width(__grid); \
    const u32 __height = Grid_height(__grid); \
    for (u32 __y = 0; __y < __height; ++__y) { \
        for (u32 __x = 0; __x < __width; ++__x) { \
            let pp_Tuple_get1st _Tuple_Item_w_Tuple_X_Y_Idx \
                = Grid_at(__grid, __x, __y); \
            let pp_exec_expand( \
                pp_exec_defer(pp_Tuple_get1st) pp_Tuple_get2nd _Tuple_Item_w_Tuple_X_Y_Idx \
            ) = __x; \
            let pp_exec_expand( \
                pp_exec_defer(pp_Tuple_get2nd) pp_Tuple_get2nd _Tuple_Item_w_Tuple_X_Y_Idx \
            ) = __y; \
            let pp_exec_expand( \
                pp_exec_defer(pp_Tuple_get3rd) pp_Tuple_get2nd _Tuple_Item_w_Tuple_X_Y_Idx \
            ) = __x + (__y * __width); \
            _Body; \
        } \
    } \
})
#define comp_syn__for_grid_item_w_tuple_x_y_w_tuple_width_height(_Grid, _Tuple_Item_w_Tuple_X_Y_w_Tuple_Width_Height, _Body...) blk({ \
    let __grid = _Grid; \
    const u32 __width = Grid_width(__grid); \
    const u32 __height = Grid_height(__grid); \
    for (u32 __y = 0; __y < __height; ++__y) { \
        for (u32 __x = 0; __x < __width; ++__x) { \
            let pp_Tuple_get1st _Tuple_Item_w_Tuple_X_Y_w_Tuple_Width_Height \
                = Grid_at(__grid, __x, __y); \
            let pp_exec_expand( \
                pp_exec_defer(pp_Tuple_get1st) pp_Tuple_get2nd _Tuple_Item_w_Tuple_X_Y_w_Tuple_Width_Height \
            ) = __x; \
            let pp_exec_expand( \
                pp_exec_defer(pp_Tuple_get2nd) pp_Tuple_get2nd _Tuple_Item_w_Tuple_X_Y_w_Tuple_Width_Height \
            ) = __y; \
            let pp_exec_expand( \
                pp_exec_defer(pp_Tuple_get1st) pp_Tuple_get3rd _Tuple_Item_w_Tuple_X_Y_w_Tuple_Width_Height \
            ) = __width; \
            let pp_exec_expand( \
                pp_exec_defer(pp_Tuple_get2nd) pp_Tuple_get3rd _Tuple_Item_w_Tuple_X_Y_w_Tuple_Width_Height \
            ) = __height; \
            _Body; \
        } \
    } \
})
#define comp_syn__for_grid_item_w_tuple_x_y_idx_w_tuple_width_height_len(_Grid, _Tuple_Item_w_Tuple_X_Y_Idx_w_Tuple_Width_Height_Len, _Body...) blk({ \
    let __grid = _Grid; \
    const u32 __width = Grid_width(__grid); \
    const u32 __height = Grid_height(__grid); \
    const usize __len = S_len(__grid.items); \
    for (u32 __y = 0; __y < __height; ++__y) { \
        for (u32 __x = 0; __x < __width; ++__x) { \
            let pp_Tuple_get1st _Tuple_Item_w_Tuple_X_Y_Idx_w_Tuple_Width_Height_Len \
                = Grid_at(__grid, __x, __y); \
            let pp_exec_expand( \
                pp_exec_defer(pp_Tuple_get1st) pp_Tuple_get2nd _Tuple_Item_w_Tuple_X_Y_Idx_w_Tuple_Width_Height_Len \
            ) = __x; \
            let pp_exec_expand( \
                pp_exec_defer(pp_Tuple_get2nd) pp_Tuple_get2nd _Tuple_Item_w_Tuple_X_Y_Idx_w_Tuple_Width_Height_Len \
            ) = __y; \
            let pp_exec_expand( \
                pp_exec_defer(pp_Tuple_get3rd) pp_Tuple_get2nd _Tuple_Item_w_Tuple_X_Y_Idx_w_Tuple_Width_Height_Len \
            ) = __x + (__y * __width); \
            let pp_exec_expand( \
                pp_exec_defer(pp_Tuple_get1st) pp_Tuple_get3rd _Tuple_Item_w_Tuple_X_Y_Idx_w_Tuple_Width_Height_Len \
            ) = __width; \
            let pp_exec_expand( \
                pp_exec_defer(pp_Tuple_get2nd) pp_Tuple_get3rd _Tuple_Item_w_Tuple_X_Y_Idx_w_Tuple_Width_Height_Len \
            ) = __height; \
            let pp_exec_expand( \
                pp_exec_defer(pp_Tuple_get3rd) pp_Tuple_get3rd _Tuple_Item_w_Tuple_X_Y_Idx_w_Tuple_Width_Height_Len \
            ) = __len; \
            _Body; \
        } \
    } \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Grid__included */
