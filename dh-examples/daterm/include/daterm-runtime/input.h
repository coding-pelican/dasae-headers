/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    input.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-20 (date of creation)
 * @updated 2026-06-20 (date of last update)
 * @ingroup daterm-runtime
 * @prefix  daterm_input
 */
#pragma once
#ifndef daterm_input__included
#define daterm_input__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "base.h"

typedef union daterm_input_Mods {
    struct {
        u8 shift     : 1;
        u8 alt       : 1;
        u8 ctrl      : 1;
        u8 meta      : 1;
        u8 reserved_ : 4;
    };
    u8 packed;
} daterm_input_Mods;
T_use_prl$(daterm_input_Mods);

$attr($inline_always)
$static fn_((daterm_input_modsNone(void))(daterm_input_Mods)) {
    return (daterm_input_Mods){};
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_input__included */
