
/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    blk.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-01 (date of creation)
 * @updated 2025-06-01 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  blk
 */

#ifndef BLK_INCLUDED
#define BLK_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

#define blk_(_label, _BreakType_and_Body...) pp_overload(__blk, _BreakType_and_Body)(_label, _BreakType_and_Body)
#define __blk_1(_label, _Body...)            comp_syn__blk_((_label, Void), _Body)
#define __blk_2(_label, _RetType, _Body...)  comp_syn__blk_((_label, _RetType), _Body)
#define blk_break_(_label, ...)              comp_syn__blk_break_(pp_cat(__reserved_val_, _label), _label, __VA_ARGS__)
#define break_(_label, ...)                  comp_syn__blk_break_(pp_cat(__reserved_val_, _label), _label, __VA_ARGS__)

/*========== Macros and Definitions =========================================*/

// clang-format off
#define comp_syn__blk_(_Label_and_RetType, _Body...) blk({ \
    local_label pp_Tuple_get1st _Label_and_RetType; \
    var pp_cat(__reserved_val_, pp_Tuple_get1st _Label_and_RetType) = _Generic( \
        TypeOf(pp_Tuple_get2nd _Label_and_RetType), \
        void: (Void){}, \
        default: (pp_Tuple_get2nd _Label_and_RetType){} \
    ); \
    _Body; \
    pp_Tuple_get1st _Label_and_RetType: \
    blk_return pp_cat(__reserved_val_, pp_Tuple_get1st _Label_and_RetType); \
})
#define comp_syn__blk_break_(__reserved_val, _label, ...) \
    __reserved_val = *(TypeOf(__reserved_val)[1]){ [0] = __VA_ARGS__ }; \
    goto _label
// clang-format on

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BLK_INCLUDED */
