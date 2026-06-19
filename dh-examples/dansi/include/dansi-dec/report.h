/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    report.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_dec_report
 */
#pragma once
#ifndef dansi_dec_report__included
#define dansi_dec_report__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/csi.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_dec_report_PrinterStatus $fits($packed))(
    dansi_dec_report_PrinterStatus_ready = 10,
    dansi_dec_report_PrinterStatus_not_ready = 11
)) dansi_dec_report_PrinterStatus;
claim_assert_static(eqlType$(dansi_dec_report_PrinterStatus, u8));
T_use_prl$(dansi_dec_report_PrinterStatus);

typedef enum_((dansi_dec_report_KeyboardStatus $fits($packed))(
    dansi_dec_report_KeyboardStatus_ready = 20,
    dansi_dec_report_KeyboardStatus_locked = 21
)) dansi_dec_report_KeyboardStatus;
claim_assert_static(eqlType$(dansi_dec_report_KeyboardStatus, u8));
T_use_prl$(dansi_dec_report_KeyboardStatus);

errset_((dansi_dec_report_E)(dansi_dec_report_InvalidResponse));
T_use_E$($set(dansi_dec_report_E)(dansi_dec_report_PrinterStatus));
T_use_E$($set(dansi_dec_report_E)(dansi_dec_report_KeyboardStatus));

#define dansi_dec_report_requestPrinterStatus_static() \
    ____dansi_dec_report_requestPrinterStatus_static()
$extern fn_((dansi_dec_report_requestPrinterStatus(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_dec_report_requestPrinterStatusWrite(io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_dec_report_receivePrinterStatusReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_dec_report_parsePrinterStatus(
    S_const$u8 report
))(dansi_dec_report_E$dansi_dec_report_PrinterStatus));
$attr($must_check)
$extern fn_((dansi_dec_report_fetchPrinterStatus(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_dec_report_E$dansi_dec_report_PrinterStatus));

#define dansi_dec_report_requestKeyboardStatus_static() \
    ____dansi_dec_report_requestKeyboardStatus_static()
$extern fn_((dansi_dec_report_requestKeyboardStatus(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_dec_report_requestKeyboardStatusWrite(io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_dec_report_receiveKeyboardStatusReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_dec_report_parseKeyboardStatus(
    S_const$u8 report
))(dansi_dec_report_E$dansi_dec_report_KeyboardStatus));
$attr($must_check)
$extern fn_((dansi_dec_report_fetchKeyboardStatus(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_dec_report_E$dansi_dec_report_KeyboardStatus));

/*========== Macros and Definitions =========================================*/

#define ____dansi_dec_report_requestPrinterStatus_static() \
    dansi_csi_makePrivate1_static("15", "n")
#define ____dansi_dec_report_requestKeyboardStatus_static() \
    dansi_csi_makePrivate1_static("26", "n")

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_dec_report__included */
