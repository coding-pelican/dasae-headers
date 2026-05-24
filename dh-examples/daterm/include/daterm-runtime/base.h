/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    base.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-05-23 (date of last update)
 * @ingroup daterm-runtime
 * @prefix  daterm
 */
#ifndef daterm_runtime_base__included
#define daterm_runtime_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include <dh/prl.h>
#include <dansi.h>

typedef struct daterm_Pos {
    var_(col, u16);
    var_(row, u16);
} daterm_Pos;
T_use_prl$(daterm_Pos);

typedef struct daterm_Size {
    var_(cols, u16);
    var_(rows, u16);
} daterm_Size;
T_use_prl$(daterm_Size);

typedef struct daterm_RGB {
    var_(r, u8);
    var_(g, u8);
    var_(b, u8);
} daterm_RGB;
T_use_prl$(daterm_RGB);

typedef struct daterm_CellStyle {
    var_(fg, daterm_RGB);
    var_(bg, daterm_RGB);
    var_(has_fg, bool);
    var_(has_bg, bool);
    var_(bold, bool);
    var_(italic, bool);
    var_(underline, bool);
    var_(reverse, bool);
} daterm_CellStyle;
T_use_prl$(daterm_CellStyle);

typedef struct daterm_Cell {
    var_(codepoint, u32);
    var_(style, daterm_CellStyle);
} daterm_Cell;
T_use_prl$(daterm_Cell);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_runtime_base__included */
