/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Seq.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-05-23 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_Seq
 *
 * @brief   Protocol-neutral ANSI byte sequence extraction.
 */
#pragma once
#ifndef dansi_Seq__included
#define dansi_Seq__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/Buf.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_Seq_Kind $fits($packed))(
    dansi_Seq_Kind_raw = 0,
    dansi_Seq_Kind_esc,
    dansi_Seq_Kind_csi,
    dansi_Seq_Kind_ss3,
    dansi_Seq_Kind_osc,
    dansi_Seq_Kind_dcs,
)) dansi_Seq_Kind;

typedef struct dansi_Seq {
    /// Borrowed bytes for one extracted or caller-provided sequence.
    /// When returned by `dansi_Seq_extract`, this slice is valid only until the
    /// source `io_Buf_Reader` buffer is mutated again.
    var_(kind, dansi_Seq_Kind);
    var_(bytes, S_const$u8);
} dansi_Seq;
T_use_prl$(dansi_Seq);

errset_((dansi_Seq_E)(
    dansi_Seq_Incomplete,
    dansi_Seq_TooLong
));
T_use_E$($set(dansi_Seq_E)(dansi_Seq));

$attr($inline_always)
$static fn_((dansi_Seq_from(dansi_Seq_Kind kind, S_const$u8 bytes))(dansi_Seq));
$attr($inline_always)
$static fn_((dansi_Seq_raw(S_const$u8 bytes))(dansi_Seq));
$attr($inline_always)
$static fn_((dansi_Seq_esc(S_const$u8 bytes))(dansi_Seq));
$attr($inline_always)
$static fn_((dansi_Seq_csi(S_const$u8 bytes))(dansi_Seq));
$attr($inline_always)
$static fn_((dansi_Seq_ss3(S_const$u8 bytes))(dansi_Seq));
$attr($inline_always)
$static fn_((dansi_Seq_osc(S_const$u8 bytes))(dansi_Seq));
$attr($inline_always)
$static fn_((dansi_Seq_dcs(S_const$u8 bytes))(dansi_Seq));

$attr($inline_always)
$static fn_((dansi_Seq_isEmpty(dansi_Seq self))(bool));

$attr($must_check)
$extern fn_((dansi_Seq_extract(io_Buf_Reader* reader))(dansi_Seq_E$dansi_Seq));
$attr($must_check)
$extern fn_((dansi_Seq_receiveCSI(io_Reader in, S$u8 buf))(E$S$u8));

/*========== Macros and Definitions =========================================*/

fn_((dansi_Seq_from(dansi_Seq_Kind kind, S_const$u8 bytes))(dansi_Seq)) {
    return (dansi_Seq){ .kind = kind, .bytes = bytes };
};
fn_((dansi_Seq_raw(S_const$u8 bytes))(dansi_Seq)) {
    return dansi_Seq_from(dansi_Seq_Kind_raw, bytes);
};
fn_((dansi_Seq_esc(S_const$u8 bytes))(dansi_Seq)) {
    return dansi_Seq_from(dansi_Seq_Kind_esc, bytes);
};
fn_((dansi_Seq_csi(S_const$u8 bytes))(dansi_Seq)) {
    return dansi_Seq_from(dansi_Seq_Kind_csi, bytes);
};
fn_((dansi_Seq_ss3(S_const$u8 bytes))(dansi_Seq)) {
    return dansi_Seq_from(dansi_Seq_Kind_ss3, bytes);
};
fn_((dansi_Seq_osc(S_const$u8 bytes))(dansi_Seq)) {
    return dansi_Seq_from(dansi_Seq_Kind_osc, bytes);
};
fn_((dansi_Seq_dcs(S_const$u8 bytes))(dansi_Seq)) {
    return dansi_Seq_from(dansi_Seq_Kind_dcs, bytes);
};

fn_((dansi_Seq_isEmpty(dansi_Seq self))(bool)) {
    return self.bytes.len == 0;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_Seq__included */
