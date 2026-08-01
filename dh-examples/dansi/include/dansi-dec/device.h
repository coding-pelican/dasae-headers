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
 * @prefix  dansi_dec_device
 */
#pragma once
#ifndef dansi_dec_device__included
#define dansi_dec_device__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/csi.h"
#include "dansi-core/esc.h"

/*========== Macros and Declarations ========================================*/

#define dansi_dec_device_attrs_report_final __str__dansi_dec_device_attrs_report_final
#define dansi_dec_device_attrs_report_final_byte __uint__dansi_dec_device_attrs_report_final_byte
#define dansi_dec_device_primary_marker __str__dansi_dec_device_primary_marker
#define dansi_dec_device_primary_marker_byte __uint__dansi_dec_device_primary_marker_byte
#define dansi_dec_device_secondary_marker __str__dansi_dec_device_secondary_marker
#define dansi_dec_device_secondary_marker_byte __uint__dansi_dec_device_secondary_marker_byte
#define dansi_dec_device_tertiary_marker __str__dansi_dec_device_tertiary_marker
#define dansi_dec_device_tertiary_marker_byte __uint__dansi_dec_device_tertiary_marker_byte
#define dansi_dec_device_attrs_request_param __str__dansi_dec_device_attrs_request_param
#define dansi_dec_device_attrs_request_param_u16 __uint__dansi_dec_device_attrs_request_param_u16
#define dansi_dec_device_identify_final __str__dansi_dec_device_identify_final
#define dansi_dec_device_identify_final_byte __uint__dansi_dec_device_identify_final_byte

/* DEC private DA variants and terminal identification. */

typedef enum_((dansi_dec_device_Kind $fits($packed))(
    dansi_dec_device_Kind_primary = 0,
    dansi_dec_device_Kind_secondary,
    dansi_dec_device_Kind_tertiary
)) dansi_dec_device_Kind;
claim_assert_static(eqlType$(dansi_dec_device_Kind, u8));
T_use_prl$(dansi_dec_device_Kind);

typedef struct dansi_dec_device_Attrs {
    var_(kind, dansi_dec_device_Kind);
    var_(params, S_const$u8);
} dansi_dec_device_Attrs;
T_use_prl$(dansi_dec_device_Attrs);

errset_((dansi_dec_device_E)(dansi_dec_device_InvalidResponse));
T_use_E$($set(dansi_dec_device_E)(dansi_dec_device_Attrs));

#define dansi_dec_device_requestPrimaryAttrs_static() \
    ____dansi_dec_device_requestPrimaryAttrs_static()
$extern fn_((dansi_dec_device_requestPrimaryAttrs(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_dec_device_requestPrimaryAttrsWrite(io_Writer out))(io_WriteE$void));
$attr($must_check)
$extern fn_((dansi_dec_device_fetchPrimaryAttrs(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_dec_device_E$dansi_dec_device_Attrs));

#define dansi_dec_device_requestSecondaryAttrs_static() \
    ____dansi_dec_device_requestSecondaryAttrs_static()
$extern fn_((dansi_dec_device_requestSecondaryAttrs(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_dec_device_requestSecondaryAttrsWrite(io_Writer out))(io_WriteE$void));
$attr($must_check)
$extern fn_((dansi_dec_device_fetchSecondaryAttrs(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_dec_device_E$dansi_dec_device_Attrs));

#define dansi_dec_device_requestTertiaryAttrs_static() \
    ____dansi_dec_device_requestTertiaryAttrs_static()
$extern fn_((dansi_dec_device_requestTertiaryAttrs(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_dec_device_requestTertiaryAttrsWrite(io_Writer out))(io_WriteE$void));
$attr($must_check)
$extern fn_((dansi_dec_device_fetchTertiaryAttrs(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_dec_device_E$dansi_dec_device_Attrs));

#define dansi_dec_device_identify_static() \
    ____dansi_dec_device_identify_static()
$extern fn_((dansi_dec_device_identify(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_dec_device_identifyWrite(io_Writer out))(io_WriteE$void));
$attr($must_check)
$extern fn_((dansi_dec_device_receiveAttrsReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_dec_device_parseAttrsReport(
    S_const$u8 report
))(dansi_dec_device_E$dansi_dec_device_Attrs));

/*========== Macros and Definitions =========================================*/

#define __str__dansi_dec_device_attrs_report_final "c"
#define __uint__dansi_dec_device_attrs_report_final_byte u8_c('c')
#define __str__dansi_dec_device_primary_marker dansi_csi_private_marker_query
#define __uint__dansi_dec_device_primary_marker_byte u8_c('?')
#define __str__dansi_dec_device_secondary_marker ">"
#define __uint__dansi_dec_device_secondary_marker_byte u8_c('>')
#define __str__dansi_dec_device_tertiary_marker "="
#define __uint__dansi_dec_device_tertiary_marker_byte u8_c('=')
#define __str__dansi_dec_device_attrs_request_param "0"
#define __uint__dansi_dec_device_attrs_request_param_u16 0
#define __str__dansi_dec_device_identify_final "Z"
#define __uint__dansi_dec_device_identify_final_byte u8_c('Z')
#define ____dansi_dec_device_requestPrimaryAttrs_static() \
    dansi_csi_makePrivate1_static("", dansi_dec_device_attrs_report_final)
#define ____dansi_dec_device_requestSecondaryAttrs_static() \
    dansi_csi_make1_static(dansi_dec_device_secondary_marker dansi_dec_device_attrs_request_param, dansi_dec_device_attrs_report_final)
#define ____dansi_dec_device_requestTertiaryAttrs_static() \
    dansi_csi_make1_static(dansi_dec_device_tertiary_marker dansi_dec_device_attrs_request_param, dansi_dec_device_attrs_report_final)
#define ____dansi_dec_device_identify_static() \
    dansi_esc_make_static("", dansi_dec_device_identify_final)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_dec_device__included */
