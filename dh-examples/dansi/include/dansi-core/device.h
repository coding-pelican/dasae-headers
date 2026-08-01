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

#define dansi_device_status_request_param __str__dansi_device_status_request_param
#define dansi_device_status_request_param_u16 __uint__dansi_device_status_request_param_u16
#define dansi_device_status_report_final __str__dansi_device_status_report_final
#define dansi_device_status_report_final_byte __uint__dansi_device_status_report_final_byte
#define dansi_device_attrs_report_final __str__dansi_device_attrs_report_final
#define dansi_device_attrs_report_final_byte __uint__dansi_device_attrs_report_final_byte
#define dansi_device_status_report_param_status __uint__dansi_device_status_report_param_status

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
$extern fn_((dansi_device_requestStatusWrite(io_Writer out))(io_WriteE$void));
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
$extern fn_((dansi_device_requestAttrsWrite(io_Writer out))(io_WriteE$void));
$attr($must_check)
$extern fn_((dansi_device_receiveAttrsReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_device_parseAttrsReport(S_const$u8 report))(dansi_device_E$dansi_device_Attrs));
$attr($must_check)
$extern fn_((dansi_device_fetchAttrs(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_device_E$dansi_device_Attrs));

/*========== Macros and Definitions =========================================*/

#define __str__dansi_device_status_request_param "5"
#define __uint__dansi_device_status_request_param_u16 5
#define __str__dansi_device_status_report_final "n"
#define __uint__dansi_device_status_report_final_byte u8_c('n')
#define __str__dansi_device_attrs_report_final "c"
#define __uint__dansi_device_attrs_report_final_byte u8_c('c')
#define __uint__dansi_device_status_report_param_status 0
#define ____dansi_device_requestStatus_static() \
    dansi_csi_make1_static(dansi_device_status_request_param, dansi_device_status_report_final)
#define ____dansi_device_requestAttrs_static() \
    dansi_csi_make0_static(dansi_device_attrs_report_final)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_device__included */
