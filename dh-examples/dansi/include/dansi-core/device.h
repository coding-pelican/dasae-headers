/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    device.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-05-23 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_device
 */
#ifndef dansi_device__included
#define dansi_device__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

typedef A$$(64, u8) dansi_device_StatusReportBuf;
typedef A$$(64, u8) dansi_device_AttrsReportBuf;

typedef enum_((dansi_device_Status $fits($packed))(
    dansi_device_Status_ok = 0,
    dansi_device_Status_malformed = 1,
)) dansi_device_Status;

typedef enum_((dansi_device_AttrsKind $fits($packed))(
    dansi_device_AttrsKind_pri = 0,
    dansi_device_AttrsKind_sec = 1,
    dansi_device_AttrsKind_ter = 2,
)) dansi_device_AttrsKind;

typedef struct dansi_device_Attrs {
    var_(kind, dansi_device_AttrsKind);
    var_(params, S_const$u8);
} dansi_device_Attrs;

errset_((dansi_device_E)(dansi_device_InvalidResponse));
T_use_E$($set(dansi_device_E)(dansi_device_Status));
T_use_E$($set(dansi_device_E)(dansi_device_Attrs));

#define dansi_device_requestStatus_static() \
    ____dansi_device_requestStatus_static()
$extern fn_((dansi_device_requestStatus(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_device_requestStatusWrite(io_Writer writer))(E$void));
$attr($must_check)
$extern fn_((dansi_device_receiveStatusReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_device_parseStatusReport(S_const$u8 report))(dansi_device_E$dansi_device_Status));
$attr($must_check)
$extern fn_((dansi_device_fetchStatus(io_Writer out, io_Reader in, S$u8 buf))(dansi_device_E$dansi_device_Status));

#define dansi_device_requestPriAttrs_static() \
    ____dansi_device_requestPriAttrs_static()
$extern fn_((dansi_device_requestPriAttrs(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_device_requestPriAttrsWrite(io_Writer writer))(E$void));
$attr($must_check)
$extern fn_((dansi_device_receivePriAttrsReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_device_parsePriAttrsReport(S_const$u8 report))(dansi_device_E$dansi_device_Attrs));
$attr($must_check)
$extern fn_((dansi_device_fetchPriAttrs(io_Writer out, io_Reader in, S$u8 buf))(dansi_device_E$dansi_device_Attrs));

#define dansi_device_requestSecAttrs_static() \
    ____dansi_device_requestSecAttrs_static()
$extern fn_((dansi_device_requestSecAttrs(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_device_requestSecAttrsWrite(io_Writer writer))(E$void));
$attr($must_check)
$extern fn_((dansi_device_receiveSecAttrsReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_device_parseSecAttrsReport(S_const$u8 report))(dansi_device_E$dansi_device_Attrs));
$attr($must_check)
$extern fn_((dansi_device_fetchSecAttrs(io_Writer out, io_Reader in, S$u8 buf))(dansi_device_E$dansi_device_Attrs));

#define dansi_device_requestTerAttrs_static() \
    ____dansi_device_requestTerAttrs_static()
$extern fn_((dansi_device_requestTerAttrs(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_device_requestTerAttrsWrite(io_Writer writer))(E$void));
$attr($must_check)
$extern fn_((dansi_device_receiveTerAttrsReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_device_parseTerAttrsReport(S_const$u8 report))(dansi_device_E$dansi_device_Attrs));
$attr($must_check)
$extern fn_((dansi_device_fetchTerAttrs(io_Writer out, io_Reader in, S$u8 buf))(dansi_device_E$dansi_device_Attrs));

/*========== Macros and Definitions =========================================*/

#include "utils.h"

#define ____dansi_device_requestStatus_static() \
    dansi_utils_formatCSI_static(dansi_utils_device_request_status)
#define ____dansi_device_requestPriAttrs_static() \
    dansi_utils_formatCSI_static(dansi_utils_device_request_pri_attrs)
#define ____dansi_device_requestSecAttrs_static() \
    dansi_utils_formatCSI_static(dansi_utils_device_request_sec_attrs)
#define ____dansi_device_requestTerAttrs_static() \
    dansi_utils_formatCSI_static(dansi_utils_device_request_ter_attrs)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_device__included */
