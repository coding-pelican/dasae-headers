/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    charset.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_dec_charset
 */
#pragma once
#ifndef dansi_dec_charset__included
#define dansi_dec_charset__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/esc.h"

/*========== Macros and Declarations ========================================*/

#define dansi_dec_charset_shift_in __str__dansi_dec_charset_shift_in
#define dansi_dec_charset_shift_in_byte __uint__dansi_dec_charset_shift_in_byte
#define dansi_dec_charset_shift_out __str__dansi_dec_charset_shift_out
#define dansi_dec_charset_shift_out_byte __uint__dansi_dec_charset_shift_out_byte

typedef enum_((dansi_dec_charset_Slot $fits($packed))(
    dansi_dec_charset_Slot_g0 = '(',
    dansi_dec_charset_Slot_g1 = ')',
    dansi_dec_charset_Slot_g2 = '*',
    dansi_dec_charset_Slot_g3 = '+'
)) dansi_dec_charset_Slot;
claim_assert_static(eqlType$(dansi_dec_charset_Slot, u8));
T_use_prl$(dansi_dec_charset_Slot);

#define dansi_dec_charset_Slot_staticParse(_slot_tok) \
    ____dansi_dec_charset_Slot_staticParse(_slot_tok)

typedef enum_((dansi_dec_charset_Set $fits($packed))(
    dansi_dec_charset_Set_uk = 'A',
    dansi_dec_charset_Set_us_ascii = 'B',
    dansi_dec_charset_Set_special_graphics = '0',
    dansi_dec_charset_Set_alt_rom = '1',
    dansi_dec_charset_Set_alt_rom_special_graphics = '2',
    dansi_dec_charset_Set_dec_supplemental = '<'
)) dansi_dec_charset_Set;
claim_assert_static(eqlType$(dansi_dec_charset_Set, u8));
T_use_prl$(dansi_dec_charset_Set);

#define dansi_dec_charset_Set_staticParse(_set_tok) \
    ____dansi_dec_charset_Set_staticParse(_set_tok)

#define dansi_dec_charset_designate_static(_slot_tok, _set_tok) \
    ____dansi_dec_charset_designate_static(_slot_tok, _set_tok)
typedef A$$(3, u8) dansi_dec_charset_DesignateBuf;
$attr($must_check)
$extern fn_((dansi_dec_charset_designate(
    dansi_dec_charset_Slot slot, dansi_dec_charset_Set set, dansi_dec_charset_DesignateBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_dec_charset_designateWrite(
    dansi_dec_charset_Slot slot, dansi_dec_charset_Set set, io_Writer out
))(io_PrintE$void));

#define dansi_dec_charset_shiftIn_static() \
    ____dansi_dec_charset_shiftIn_static()
$extern fn_((dansi_dec_charset_shiftIn(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_dec_charset_shiftInWrite(io_Writer out))(io_WriteE$void));

#define dansi_dec_charset_shiftOut_static() \
    ____dansi_dec_charset_shiftOut_static()
$extern fn_((dansi_dec_charset_shiftOut(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_dec_charset_shiftOutWrite(io_Writer out))(io_WriteE$void));

/*========== Macros and Definitions =========================================*/

#define __str__dansi_dec_charset_shift_in "\x0f"
#define __uint__dansi_dec_charset_shift_in_byte 0x0f
#define __str__dansi_dec_charset_shift_out "\x0e"
#define __uint__dansi_dec_charset_shift_out_byte 0x0e
#define ____dansi_dec_charset_designate_static(_slot_tok, _set_tok) \
    dansi_esc_make_static(dansi_dec_charset_Slot_staticParse(_slot_tok), dansi_dec_charset_Set_staticParse(_set_tok))
#define ____dansi_dec_charset_shiftIn_static() dansi_dec_charset_shift_in
#define ____dansi_dec_charset_shiftOut_static() dansi_dec_charset_shift_out

#define ____dansi_dec_charset_Slot_staticParse(_slot_tok) \
    pp_join($, ____dansi_dec_charset_Slot_str, _slot_tok)
#define ____dansi_dec_charset_Slot_str$dansi_dec_charset_Slot_g0 "("
#define ____dansi_dec_charset_Slot_str$dansi_dec_charset_Slot_g1 ")"
#define ____dansi_dec_charset_Slot_str$dansi_dec_charset_Slot_g2 "*"
#define ____dansi_dec_charset_Slot_str$dansi_dec_charset_Slot_g3 "+"

#define ____dansi_dec_charset_Set_staticParse(_set_tok) \
    pp_join($, ____dansi_dec_charset_Set_str, _set_tok)
#define ____dansi_dec_charset_Set_str$dansi_dec_charset_Set_uk "A"
#define ____dansi_dec_charset_Set_str$dansi_dec_charset_Set_us_ascii "B"
#define ____dansi_dec_charset_Set_str$dansi_dec_charset_Set_special_graphics "0"
#define ____dansi_dec_charset_Set_str$dansi_dec_charset_Set_alt_rom "1"
#define ____dansi_dec_charset_Set_str$dansi_dec_charset_Set_alt_rom_special_graphics "2"
#define ____dansi_dec_charset_Set_str$dansi_dec_charset_Set_dec_supplemental "<"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_dec_charset__included */
