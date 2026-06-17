/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    key.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-17 (date of creation)
 * @updated 2026-06-17 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_key
 *
 * @brief   Key input domain types and xterm key options (XTMODKEYS / XTFMTKEYS).
 */
#pragma once
#ifndef dansi_key__included
#define dansi_key__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

typedef union dansi_key_Mods {
    struct {
        u8 shift     : 1;
        u8 ctrl      : 1;
        u8 alt       : 1;
        u8 reserved_ : 5;
    };
    u8 packed;
} dansi_key_Mods;
T_use_prl$(dansi_key_Mods);

typedef enum_((dansi_key_Code $fits($packed))(
    dansi_key_Code_enter = 0x0D,
    dansi_key_Code_esc = 0x1B,
    dansi_key_Code_backspace = 0x08,
    dansi_key_Code_tab = 0x09,

    dansi_key_Code_up = 0x10000,
    dansi_key_Code_down,
    dansi_key_Code_left,
    dansi_key_Code_right,
    dansi_key_Code_home,
    dansi_key_Code_end,
    dansi_key_Code_page_up,
    dansi_key_Code_page_down,
    dansi_key_Code_insert,
    dansi_key_Code_delete,

    dansi_key_Code_fn_base = 0x11000,
    dansi_key_Code_f1 = dansi_key_Code_fn_base + 1,
    dansi_key_Code_f2,
    dansi_key_Code_f3,
    dansi_key_Code_f4,
    dansi_key_Code_f5,
    dansi_key_Code_f6,
    dansi_key_Code_f7,
    dansi_key_Code_f8,
    dansi_key_Code_f9,
    dansi_key_Code_f10,
    dansi_key_Code_f11,
    dansi_key_Code_f12
)) dansi_key_Code;
T_use_prl$(dansi_key_Code);

#define dansi_key_fn_max 64u

typedef enum_((dansi_key_ModifyResource $fits($packed))(
    dansi_key_ModifyResource_keyboard = 0,
    dansi_key_ModifyResource_cursor_keys = 1,
    dansi_key_ModifyResource_function_keys = 2,
    dansi_key_ModifyResource_keypad_keys = 3,
    dansi_key_ModifyResource_other_keys = 4,
    dansi_key_ModifyResource_modifier_keys = 6,
    dansi_key_ModifyResource_special_keys = 7
)) dansi_key_ModifyResource;
#define dansi_key_ModifyResource_staticParse(_resource_tok) \
    ____dansi_key_ModifyResource_staticParse(_resource_tok)

typedef enum_((dansi_key_FormatResource $fits($packed))(
    dansi_key_FormatResource_keyboard = 0,
    dansi_key_FormatResource_cursor_keys = 1,
    dansi_key_FormatResource_function_keys = 2,
    dansi_key_FormatResource_keypad_keys = 3,
    dansi_key_FormatResource_other_keys = 4,
    dansi_key_FormatResource_modifier_keys = 6,
    dansi_key_FormatResource_special_keys = 7
)) dansi_key_FormatResource;
#define dansi_key_FormatResource_staticParse(_resource_tok) \
    ____dansi_key_FormatResource_staticParse(_resource_tok)

/// modifyOtherKeys level 2: report modified keys including Enter, Tab, etc.
#define dansi_key_ModifyOtherKeys_level2 2u
/// formatOtherKeys value 1: encode modified keys as CSI unicode ; modifiers u
#define dansi_key_FormatOtherKeys_csi_u 1u

typedef A$$(16, u8) dansi_key_ModifyOtherKeysBuf;
typedef A$$(16, u8) dansi_key_CSIUFormatBuf;
typedef A$$(16, u8) dansi_key_OptionBuf;

#define dansi_key_setModifyResource_static(_resource_tok, _value_tok) \
    ____dansi_key_setModifyResource_static(_resource_tok, _value_tok)
$extern fn_((dansi_key_setModifyResource(dansi_key_ModifyResource resource, u8 value, dansi_key_OptionBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_key_setModifyResourceWrite(dansi_key_ModifyResource resource, u8 value, io_Writer writer))(E$void));

#define dansi_key_resetModifyResource_static(_resource_tok) \
    ____dansi_key_resetModifyResource_static(_resource_tok)
$extern fn_((dansi_key_resetModifyResource(dansi_key_ModifyResource resource, dansi_key_OptionBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_key_resetModifyResourceWrite(dansi_key_ModifyResource resource, io_Writer writer))(E$void));

#define dansi_key_disableModifyResource_static(_resource_tok) \
    ____dansi_key_disableModifyResource_static(_resource_tok)
$extern fn_((dansi_key_disableModifyResource(dansi_key_ModifyResource resource, dansi_key_OptionBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_key_disableModifyResourceWrite(dansi_key_ModifyResource resource, io_Writer writer))(E$void));

#define dansi_key_queryModifyResource_static(_resource_tok) \
    ____dansi_key_queryModifyResource_static(_resource_tok)
$extern fn_((dansi_key_queryModifyResource(dansi_key_ModifyResource resource, dansi_key_OptionBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_key_queryModifyResourceWrite(dansi_key_ModifyResource resource, io_Writer writer))(E$void));

#define dansi_key_setFormatResource_static(_resource_tok, _value_tok) \
    ____dansi_key_setFormatResource_static(_resource_tok, _value_tok)
$extern fn_((dansi_key_setFormatResource(dansi_key_FormatResource resource, u8 value, dansi_key_OptionBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_key_setFormatResourceWrite(dansi_key_FormatResource resource, u8 value, io_Writer writer))(E$void));

#define dansi_key_resetFormatResource_static(_resource_tok) \
    ____dansi_key_resetFormatResource_static(_resource_tok)
$extern fn_((dansi_key_resetFormatResource(dansi_key_FormatResource resource, dansi_key_OptionBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_key_resetFormatResourceWrite(dansi_key_FormatResource resource, io_Writer writer))(E$void));

#define dansi_key_queryFormatResource_static(_resource_tok) \
    ____dansi_key_queryFormatResource_static(_resource_tok)
$extern fn_((dansi_key_queryFormatResource(dansi_key_FormatResource resource, dansi_key_OptionBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_key_queryFormatResourceWrite(dansi_key_FormatResource resource, io_Writer writer))(E$void));

#define dansi_key_enableModifyOtherKeys_static(_level_tok) \
    ____dansi_key_enableModifyOtherKeys_static(_level_tok)
$extern fn_((dansi_key_enableModifyOtherKeys(u8 level, dansi_key_ModifyOtherKeysBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_key_enableModifyOtherKeysWrite(u8 level, io_Writer writer))(E$void));

#define dansi_key_disableModifyOtherKeys_static() \
    ____dansi_key_disableModifyOtherKeys_static()
$attr($must_check)
$extern fn_((dansi_key_disableModifyOtherKeysWrite(io_Writer writer))(E$void));

#define dansi_key_enableCSIUFormat_static() \
    ____dansi_key_enableCSIUFormat_static()
$extern fn_((dansi_key_enableCSIUFormat(dansi_key_CSIUFormatBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_key_enableCSIUFormatWrite(io_Writer writer))(E$void));

#define dansi_key_disableCSIUFormat_static() \
    ____dansi_key_disableCSIUFormat_static()
$attr($must_check)
$extern fn_((dansi_key_disableCSIUFormatWrite(io_Writer writer))(E$void));

#define dansi_key_enableEnhanced_static() \
    ____dansi_key_enableEnhanced_static()
$extern fn_((dansi_key_enableEnhanced(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_key_enableEnhancedWrite(io_Writer writer))(E$void));

#define dansi_key_disableEnhanced_static() \
    ____dansi_key_disableEnhanced_static()
$extern fn_((dansi_key_disableEnhanced(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_key_disableEnhancedWrite(io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "utils.h"

$attr($inline_always)
$static fn_((dansi_key_modsNone(void))(dansi_key_Mods)) {
    return (dansi_key_Mods){};
}

$attr($inline_always)
$static fn_((dansi_key_modsFromParam(u8 param))(dansi_key_Mods)) {
    if (param <= 1) return dansi_key_modsNone();
    let mask = as$(u8)(param - 1);
    return (dansi_key_Mods){
        .shift = (mask & 1) != 0,
        .alt = (mask & 2) != 0,
        .ctrl = (mask & 4) != 0,
    };
}

$attr($inline_always)
$static fn_((dansi_key_Code_fn(u8 number))(dansi_key_Code)) {
    claim_assert(1 <= number && number <= dansi_key_fn_max);
    return as$(dansi_key_Code)(dansi_key_Code_fn_base + number);
}

#define ____dansi_key_enableModifyOtherKeys_static(_level_tok) \
    dansi_key_setModifyResource_static(dansi_utils_key_modify_other_keys, _level_tok)
#define ____dansi_key_disableModifyOtherKeys_static() \
    dansi_key_resetModifyResource_static(dansi_utils_key_modify_other_keys)
#define ____dansi_key_enableCSIUFormat_static() \
    dansi_key_setFormatResource_static(dansi_utils_key_format_other_keys, "1")
#define ____dansi_key_disableCSIUFormat_static() \
    dansi_key_resetFormatResource_static(dansi_utils_key_format_other_keys)
#define ____dansi_key_setModifyResource_static(_resource_tok, _value_tok) \
    dansi_utils_csi ">" _resource_tok dansi_utils_sep _value_tok dansi_utils_key_set_modify_options
#define ____dansi_key_resetModifyResource_static(_resource_tok) \
    dansi_utils_csi ">" _resource_tok dansi_utils_key_set_modify_options
#define ____dansi_key_disableModifyResource_static(_resource_tok) \
    dansi_utils_csi ">" _resource_tok dansi_utils_key_disable_modify_options
#define ____dansi_key_queryModifyResource_static(_resource_tok) \
    dansi_utils_csi "?" _resource_tok dansi_utils_key_query_modify_options
#define ____dansi_key_setFormatResource_static(_resource_tok, _value_tok) \
    dansi_utils_csi ">" _resource_tok dansi_utils_sep _value_tok dansi_utils_key_set_format_options
#define ____dansi_key_resetFormatResource_static(_resource_tok) \
    dansi_utils_csi ">" _resource_tok dansi_utils_key_set_format_options
#define ____dansi_key_queryFormatResource_static(_resource_tok) \
    dansi_utils_csi "?" _resource_tok dansi_utils_key_query_format_options
#define ____dansi_key_ModifyResource_staticParse(_resource_tok) \
    pp_Tok_if_(pp_Tok_hasCmp(_resource_tok))( \
        pp_Tok_then_(#_resource_tok), \
        pp_Tok_else_(pp_join($, ____dansi_key__pp_ModifyResource_str, _resource_tok)) \
    )
#define ____dansi_key_FormatResource_staticParse(_resource_tok) \
    pp_Tok_if_(pp_Tok_hasCmp(_resource_tok))( \
        pp_Tok_then_(#_resource_tok), \
        pp_Tok_else_(pp_join($, ____dansi_key__pp_FormatResource_str, _resource_tok)) \
    )

#pragma region "pp key resource str"
#define ____dansi_key__pp_ModifyResource_str$dansi_key_ModifyResource_keyboard "0"
#define ____dansi_key__pp_ModifyResource_str$dansi_key_ModifyResource_cursor_keys "1"
#define ____dansi_key__pp_ModifyResource_str$dansi_key_ModifyResource_function_keys "2"
#define ____dansi_key__pp_ModifyResource_str$dansi_key_ModifyResource_keypad_keys "3"
#define ____dansi_key__pp_ModifyResource_str$dansi_key_ModifyResource_other_keys "4"
#define ____dansi_key__pp_ModifyResource_str$dansi_key_ModifyResource_modifier_keys "6"
#define ____dansi_key__pp_ModifyResource_str$dansi_key_ModifyResource_special_keys "7"
#define ____dansi_key__pp_FormatResource_str$dansi_key_FormatResource_keyboard "0"
#define ____dansi_key__pp_FormatResource_str$dansi_key_FormatResource_cursor_keys "1"
#define ____dansi_key__pp_FormatResource_str$dansi_key_FormatResource_function_keys "2"
#define ____dansi_key__pp_FormatResource_str$dansi_key_FormatResource_keypad_keys "3"
#define ____dansi_key__pp_FormatResource_str$dansi_key_FormatResource_other_keys "4"
#define ____dansi_key__pp_FormatResource_str$dansi_key_FormatResource_modifier_keys "6"
#define ____dansi_key__pp_FormatResource_str$dansi_key_FormatResource_special_keys "7"
#pragma endregion "pp key resource str"
#define ____dansi_key_enableEnhanced_static() \
    dansi_key_enableModifyOtherKeys_static("2") \
        dansi_key_enableCSIUFormat_static()
#define ____dansi_key_disableEnhanced_static() \
    dansi_key_disableCSIUFormat_static() \
        dansi_key_disableModifyOtherKeys_static()

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_key__included */
