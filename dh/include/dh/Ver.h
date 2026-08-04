/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Ver.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-25 (date of creation)
 * @updated 2026-07-25 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  Ver
 *
 * @brief   dh ecosystem version and ABI value.
 * @details Exposes dh's compact, totally ordered version semantics used by
 *          dh itself and projects sharing the same release/ABI convention.
 */
#pragma once
#ifndef Ver__included
#define Ver__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

T_alias$((Ver_Label)(enum_((Ver_Label $fits($packed))(
    Ver_Label_alpha = ver_label_prefix_as_num_alpha,
    Ver_Label_beta = ver_label_prefix_as_num_beta,
    Ver_Label_rc = ver_label_prefix_as_num_rc,
    Ver_Label_release = ver_label_prefix_as_num_release
))));

bitfield_((Ver)(u32)(
    (major, u8, 8),
    (minor, u8, 8),
    (patch, u8, 8),
    (label, u8, 2),
    (label_suffix, u8, 6)
));
T_use_O$(Ver);
$extern fn_((Ver_self(void))(Ver));
$extern fn_((Ver_comp(void))(Ver));
$extern fn_((Ver_clang(void))(O$Ver));
$extern fn_((Ver_gcc(void))(O$Ver));
$extern fn_((Ver_gnu(void))(O$Ver));

$attr($inline_always)
$static fn_((Ver_from(u8 major, u8 minor, u8 patch, Ver_Label label, u8 label_suffix))(Ver));
$attr($inline_always)
$static fn_((Ver_fromNum(u32 num))(Ver));
$attr($inline_always)
$static fn_((Ver_intoNum(Ver self))(u32));

$extern cmp_fn_eql$((Ver)(lhs, rhs));
$extern cmp_fn_neq$((Ver)(lhs, rhs));

$extern cmp_fn_ord$((Ver)(lhs, rhs));
$extern cmp_fn_eq$((Ver)(lhs, rhs));
$extern cmp_fn_ne$((Ver)(lhs, rhs));
$extern cmp_fn_lt$((Ver)(lhs, rhs));
$extern cmp_fn_gt$((Ver)(lhs, rhs));
$extern cmp_fn_le$((Ver)(lhs, rhs));
$extern cmp_fn_ge$((Ver)(lhs, rhs));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((Ver_from(u8 major, u8 minor, u8 patch, Ver_Label label, u8 label_suffix))(Ver)) {
    return (Ver){
        .major = major,
        .minor = minor,
        .patch = patch,
        .label = label,
        .label_suffix = label_suffix,
    };
};
fn_((Ver_fromNum(u32 num))(Ver)) {
    return (Ver){
        .packed = num,
    };
};
fn_((Ver_intoNum(Ver self))(u32)) {
    return self.packed;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Ver__included */
