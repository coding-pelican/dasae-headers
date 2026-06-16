/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    hash.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-22 (date of creation)
 * @updated 2026-05-22 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  hash
 *
 * @brief   Non-cryptographic hash utilities
 */
#pragma once
#ifndef hash__included
#define hash__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"

/*========== Extern Function Prototypes =====================================*/

$extern fn_((hash_int$usize(usize input))(usize));
$extern fn_((hash_int$u64(u64 input))(u64));
$extern fn_((hash_int$u32(u32 input))(u32));
$extern fn_((hash_int$u16(u16 input))(u16));
$extern fn_((hash_int$u8(u8 input))(u8));

$extern fn_((hash_fnv1a32(S_const$u8 input))(u32));
$extern fn_((hash_fnv1a64(S_const$u8 input))(u64));
$extern fn_((hash_adler32(S_const$u8 input))(u32));
$extern fn_((hash_wyhash(S_const$u8 input, u64 seed))(u64));
$extern fn_((hash_bytes64(S_const$u8 input))(u64));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* hash__included */
