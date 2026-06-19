/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    device.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_device
 */
#pragma once
#ifndef dansi_device__included
#define dansi_device__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "csi.h"

/*========== Macros and Declarations ========================================*/

/* ECMA-48 DA/DSR device status and attributes. */

typedef enum_((dansi_device_Status $fits($packed))(
    dansi_device_Status_ready = 0,
    dansi_device_Status_malformed = 3
)) dansi_device_Status;
T_use_prl$(dansi_device_Status);

typedef struct dansi_device_Attrs {
    var_(params, S_const$u8);
} dansi_device_Attrs;
T_use_prl$(dansi_device_Attrs);

errset_((dansi_device_E)(dansi_device_InvalidResponse));
T_use_E$($set(dansi_device_E)(dansi_device_Status));
T_use_E$($set(dansi_device_E)(dansi_device_Attrs));

#define dansi_device_requestStatus_static() \
    ____dansi_device_requestStatus_static()
$extern fn_((dansi_device_requestStatus(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_device_requestStatusWrite(io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_device_receiveStatusReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_device_parseStatusReport(S_const$u8 report))(dansi_device_E$dansi_device_Status));
$attr($must_check)
$extern fn_((dansi_device_fetchStatus(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_device_E$dansi_device_Status));

#define dansi_device_requestAttrs_static() \
    ____dansi_device_requestAttrs_static()
$extern fn_((dansi_device_requestAttrs(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_device_requestAttrsWrite(io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_device_receiveAttrsReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_device_parseAttrsReport(S_const$u8 report))(dansi_device_E$dansi_device_Attrs));
$attr($must_check)
$extern fn_((dansi_device_fetchAttrs(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_device_E$dansi_device_Attrs));

/*========== Macros and Definitions =========================================*/

#define ____dansi_device_requestStatus_static() \
    dansi_csi_make1_static("5", "n")
#define ____dansi_device_requestAttrs_static() \
    dansi_csi_make0_static("c")

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_device__included */
