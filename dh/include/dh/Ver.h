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
 * @brief   dh ecosystem version and ABI value contract.
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
#include "cmp.h"

/*========== Macros and Declarations ========================================*/

T_alias$((Ver_Label)(enum_((Ver_Label $fits($packed))(
    Ver_Label_alpha = ver_label_prefix_as_num_alpha,
    Ver_Label_beta = ver_label_prefix_as_num_beta,
    Ver_Label_rc = ver_label_prefix_as_num_rc,
    Ver_Label_release = ver_label_prefix_as_num_release
))));

#define Ver_label_bits 2
#define Ver_label_suffix_bits 6
T_alias$((Ver)(union Ver {
    T_embed$(struct {
#if arch_byte_order_is_little_endian
        var_(label_suffix : Ver_label_suffix_bits, u8);
        var_(label : Ver_label_bits, u8);
        var_(patch, u8);
        var_(minor, u8);
        var_(major, u8);
#elif arch_byte_order_is_big_endian
        var_(major, u8);
        var_(minor, u8);
        var_(patch, u8);
        var_(label : Ver_label_bits, u8);
        var_(label_suffix : Ver_label_suffix_bits, u8);
#else
#error "arch_byte_order_is_little_endian or arch_byte_order_is_big_endian is required"
#endif /* arch_byte_order_is_little_endian, arch_byte_order_is_big_endian */
    });
    var_(packed, u32);
}));
claim_assert_static(Ver_label_bits + Ver_label_suffix_bits == int_bits$(u8));
claim_assert_static(3 * int_bits$(u8) + Ver_label_bits + Ver_label_suffix_bits == int_bits$(u32));
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

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Ver__included */
