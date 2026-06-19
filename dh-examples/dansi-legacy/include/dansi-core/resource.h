/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    resource.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-17 (date of creation)
 * @updated 2026-06-17 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_resource
 *
 * @brief   xterm resource and termcap/terminfo DCS controls.
 */
#pragma once
#ifndef dansi_resource__included
#define dansi_resource__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

#define dansi_resource_requestValues_static(_names_hex_tok) \
    ____dansi_resource_requestValues_static(_names_hex_tok)
$extern fn_((dansi_resource_requestValues(S_const$u8 names, S$u8 buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_resource_requestValuesWrite(S_const$u8 names, io_Writer writer))(E$void));

#define dansi_resource_setTermcap_static(_name_hex_tok) \
    ____dansi_resource_setTermcap_static(_name_hex_tok)
$extern fn_((dansi_resource_setTermcap(S_const$u8 name, S$u8 buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_resource_setTermcapWrite(S_const$u8 name, io_Writer writer))(E$void));

#define dansi_resource_requestTermcap_static(_names_hex_tok) \
    ____dansi_resource_requestTermcap_static(_names_hex_tok)
$extern fn_((dansi_resource_requestTermcap(S_const$u8 names, S$u8 buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_resource_requestTermcapWrite(S_const$u8 names, io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "utils.h"

#define ____dansi_resource_requestValues_static(_names_hex_tok) \
    dansi_utils_dcs dansi_utils_resource_request_values _names_hex_tok dansi_utils_st
#define ____dansi_resource_setTermcap_static(_name_hex_tok) \
    dansi_utils_dcs dansi_utils_termcap_set _name_hex_tok dansi_utils_st
#define ____dansi_resource_requestTermcap_static(_names_hex_tok) \
    dansi_utils_dcs dansi_utils_termcap_request _names_hex_tok dansi_utils_st

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_resource__included */
