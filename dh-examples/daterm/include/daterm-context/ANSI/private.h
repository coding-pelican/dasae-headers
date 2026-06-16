/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    private.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-25 (date of creation)
 * @updated 2026-05-25 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/daterm
 * @prefix  daterm_ANSI
 */
#pragma once
#ifndef daterm_ANSI_private__included
#define daterm_ANSI_private__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "daterm-context/ANSI.h"
#include <dansi-core/Seq.h>

$attr($must_check)
$extern fn_((daterm_ANSI_pollBufferedSeq(
    io_Buf_Reader* input,
    time_Clock time,
    O$time_Clock_Inst* esc_started_at,
    time_Dur esc_timeout
))(O$dansi_Seq));

$attr($must_check)
$extern fn_((daterm_ANSI_parseFocusSeq(dansi_Seq seq))(O$daterm_Event_Focus));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_ANSI_private__included */
