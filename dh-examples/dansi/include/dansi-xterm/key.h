/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    key.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm_key
 */
#pragma once
#ifndef dansi_xterm_key__included
#define dansi_xterm_key__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/Seq.h"
#include "dansi-core/csi.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_xterm_key_Resrc $fits($packed))(
    dansi_xterm_key_Resrc_keyboard = 0,
    dansi_xterm_key_Resrc_cursor_keys = 1,
    dansi_xterm_key_Resrc_function_keys = 2,
    dansi_xterm_key_Resrc_keypad_keys = 3,
    dansi_xterm_key_Resrc_other_keys = 4,
    dansi_xterm_key_Resrc_modifier_keys = 6,
    dansi_xterm_key_Resrc_special_keys = 7
)) dansi_xterm_key_Resrc;
claim_assert_static(eqlType$(dansi_xterm_key_Resrc, u8));
T_use_prl$(dansi_xterm_key_Resrc);

#define dansi_xterm_key_Resrc_staticParse(_resrc_tok) \
    ____dansi_xterm_key_Resrc_staticParse(_resrc_tok)

typedef enum_((dansi_xterm_key_ModifyLevel $fits($packed))(
    dansi_xterm_key_ModifyLevel_default = 0,
    dansi_xterm_key_ModifyLevel_modifier_params = 1,
    dansi_xterm_key_ModifyLevel_other_keys = 2
)) dansi_xterm_key_ModifyLevel;
claim_assert_static(eqlType$(dansi_xterm_key_ModifyLevel, u8));
T_use_prl$(dansi_xterm_key_ModifyLevel);

#define dansi_xterm_key_ModifyLevel_staticParse(_level_tok) \
    ____dansi_xterm_key_ModifyLevel_staticParse(_level_tok)

typedef enum_((dansi_xterm_key_Format $fits($packed))(
    dansi_xterm_key_Format_legacy = 0,
    dansi_xterm_key_Format_csi_u = 1
)) dansi_xterm_key_Format;
claim_assert_static(eqlType$(dansi_xterm_key_Format, u8));
T_use_prl$(dansi_xterm_key_Format);

#define dansi_xterm_key_Format_staticParse(_format_tok) \
    ____dansi_xterm_key_Format_staticParse(_format_tok)

typedef union dansi_xterm_key_Mods {
    struct {
        u8 shift     : 1;
        u8 alt       : 1;
        u8 ctrl      : 1;
        u8 meta      : 1;
        u8 reserved_ : 4;
    };
    u8 packed;
} dansi_xterm_key_Mods;
T_use_prl$(dansi_xterm_key_Mods);

$attr($inline_always)
$static fn_((dansi_xterm_key_modsNone(void))(dansi_xterm_key_Mods)) {
    return (dansi_xterm_key_Mods){};
};

$attr($inline_always)
$static fn_((dansi_xterm_key_modsFromParam(u16 param))(dansi_xterm_key_Mods)) {
    if (param <= 1) return dansi_xterm_key_modsNone();
    let_(mask, u16) = param - 1;
    return (dansi_xterm_key_Mods){
        .shift = (mask & 1) != 0,
        .alt = (mask & 2) != 0,
        .ctrl = (mask & 4) != 0,
        .meta = (mask & 8) != 0,
    };
};

$attr($inline_always)
$static fn_((dansi_xterm_key_modsToParam(dansi_xterm_key_Mods mods))(u16)) {
    return 1
         + (mods.shift ? 1 : 0)
         + (mods.alt ? 2 : 0)
         + (mods.ctrl ? 4 : 0)
         + (mods.meta ? 8 : 0);
};

typedef enum_((dansi_xterm_key_Named $fits($packed))(
    dansi_xterm_key_Named_up,
    dansi_xterm_key_Named_down,
    dansi_xterm_key_Named_right,
    dansi_xterm_key_Named_left,
    dansi_xterm_key_Named_home,
    dansi_xterm_key_Named_end,
    dansi_xterm_key_Named_insert,
    dansi_xterm_key_Named_delete,
    dansi_xterm_key_Named_page_up,
    dansi_xterm_key_Named_page_down,
    dansi_xterm_key_Named_begin,
    dansi_xterm_key_Named_tab,
    dansi_xterm_key_Named_back_tab,
    dansi_xterm_key_Named_enter,
    dansi_xterm_key_Named_escape,
    dansi_xterm_key_Named_backspace,
    dansi_xterm_key_Named_f1,
    dansi_xterm_key_Named_f2,
    dansi_xterm_key_Named_f3,
    dansi_xterm_key_Named_f4,
    dansi_xterm_key_Named_f5,
    dansi_xterm_key_Named_f6,
    dansi_xterm_key_Named_f7,
    dansi_xterm_key_Named_f8,
    dansi_xterm_key_Named_f9,
    dansi_xterm_key_Named_f10,
    dansi_xterm_key_Named_f11,
    dansi_xterm_key_Named_f12,
    dansi_xterm_key_Named_f13,
    dansi_xterm_key_Named_f14,
    dansi_xterm_key_Named_f15,
    dansi_xterm_key_Named_f16,
    dansi_xterm_key_Named_f17,
    dansi_xterm_key_Named_f18,
    dansi_xterm_key_Named_f19,
    dansi_xterm_key_Named_f20,
    dansi_xterm_key_Named_f21,
    dansi_xterm_key_Named_f22,
    dansi_xterm_key_Named_f23,
    dansi_xterm_key_Named_f24
)) dansi_xterm_key_Named;
claim_assert_static(eqlType$(dansi_xterm_key_Named, u8));
T_use_prl$(dansi_xterm_key_Named);

typedef struct dansi_xterm_key_LegacyESCReport {
    var_(final, u8);
    var_(bytes, S_const$u8);
} dansi_xterm_key_LegacyESCReport;
T_use_prl$(dansi_xterm_key_LegacyESCReport);

typedef struct dansi_xterm_key_LegacySS3Report {
    var_(final, u8);
    var_(bytes, S_const$u8);
} dansi_xterm_key_LegacySS3Report;
T_use_prl$(dansi_xterm_key_LegacySS3Report);

typedef struct dansi_xterm_key_CSIReport {
    var_(frame, dansi_csi_Frame);
} dansi_xterm_key_CSIReport;
T_use_prl$(dansi_xterm_key_CSIReport);

typedef struct dansi_xterm_key_ModifiedCSIReport {
    var_(frame, dansi_csi_Frame);
} dansi_xterm_key_ModifiedCSIReport;
T_use_prl$(dansi_xterm_key_ModifiedCSIReport);

typedef struct dansi_xterm_key_ModifyOtherReport {
    var_(codepoint, u32);
    var_(modifier_param, u16);
    var_(frame, dansi_csi_Frame);
} dansi_xterm_key_ModifyOtherReport;
T_use_prl$(dansi_xterm_key_ModifyOtherReport);

typedef struct dansi_xterm_key_CSIUReport {
    var_(codepoint, u32);
    var_(modifier_param, O$u16);
    var_(frame, dansi_csi_Frame);
} dansi_xterm_key_CSIUReport;
T_use_prl$(dansi_xterm_key_CSIUReport);

typedef variant_((dansi_xterm_key_Report $fits($packed))(
    (dansi_xterm_key_Report_legacy_esc, dansi_xterm_key_LegacyESCReport),
    (dansi_xterm_key_Report_legacy_ss3, dansi_xterm_key_LegacySS3Report),
    (dansi_xterm_key_Report_legacy_csi, dansi_xterm_key_CSIReport),
    (dansi_xterm_key_Report_modified_csi, dansi_xterm_key_ModifiedCSIReport),
    (dansi_xterm_key_Report_modify_other, dansi_xterm_key_ModifyOtherReport),
    (dansi_xterm_key_Report_csi_u, dansi_xterm_key_CSIUReport)
)) dansi_xterm_key_Report;
claim_assert_static(eqlType$(FieldType$(dansi_xterm_key_Report, tag), u8));
T_use_prl$(dansi_xterm_key_Report);

typedef struct dansi_xterm_key_Text {
    var_(codepoint, u32);
    var_(mods, dansi_xterm_key_Mods);
} dansi_xterm_key_Text;
T_use_prl$(dansi_xterm_key_Text);

typedef struct dansi_xterm_key_Special {
    var_(key, dansi_xterm_key_Named);
    var_(mods, dansi_xterm_key_Mods);
} dansi_xterm_key_Special;
T_use_prl$(dansi_xterm_key_Special);

typedef struct dansi_xterm_key_ModifyOther {
    var_(codepoint, u32);
    var_(mods, dansi_xterm_key_Mods);
} dansi_xterm_key_ModifyOther;
T_use_prl$(dansi_xterm_key_ModifyOther);

typedef variant_((dansi_xterm_key_Event $fits($packed))(
    (dansi_xterm_key_Event_text, dansi_xterm_key_Text),
    (dansi_xterm_key_Event_special, dansi_xterm_key_Special),
    (dansi_xterm_key_Event_modify_other, dansi_xterm_key_ModifyOther)
)) dansi_xterm_key_Event;
claim_assert_static(eqlType$(FieldType$(dansi_xterm_key_Event, tag), u8));
T_use_prl$(dansi_xterm_key_Event);

errset_((dansi_xterm_key_E)(dansi_xterm_key_InvalidReport));
T_use_E$($set(dansi_xterm_key_E)(dansi_xterm_key_Event));

#define dansi_xterm_key_setModifyRaw_static(_resrc_tok, _value_tok) \
    ____dansi_xterm_key_setModifyRaw_static(_resrc_tok, _value_tok)
typedef A$$(6 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_key_SetModifyRawBuf;
$attr($must_check)
$extern fn_((dansi_xterm_key_setModifyRaw(
    dansi_xterm_key_Resrc resource, u8 value, dansi_xterm_key_SetModifyRawBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_key_setModifyRawWrite(dansi_xterm_key_Resrc resource, u8 value, io_Writer out))(E$void));

#define dansi_xterm_key_setModifyLevel_static(_resrc_tok, _level_tok) \
    ____dansi_xterm_key_setModifyLevel_static(_resrc_tok, _level_tok)
typedef dansi_xterm_key_SetModifyRawBuf dansi_xterm_key_SetModifyLevelBuf;
$attr($must_check)
$extern fn_((dansi_xterm_key_setModifyLevel(
    dansi_xterm_key_Resrc resource, dansi_xterm_key_ModifyLevel level, dansi_xterm_key_SetModifyLevelBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_key_setModifyLevelWrite(
    dansi_xterm_key_Resrc resource, dansi_xterm_key_ModifyLevel level, io_Writer out
))(E$void));

#define dansi_xterm_key_resetModify_static(_resrc_tok) \
    ____dansi_xterm_key_resetModify_static(_resrc_tok)
typedef A$$(4 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_key_ResetModifyBuf;
$attr($must_check)
$extern fn_((dansi_xterm_key_resetModify(
    dansi_xterm_key_Resrc resource, dansi_xterm_key_ResetModifyBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_key_resetModifyWrite(dansi_xterm_key_Resrc resource, io_Writer out))(E$void));

#define dansi_xterm_key_disableModify_static(_resrc_tok) \
    ____dansi_xterm_key_disableModify_static(_resrc_tok)
typedef dansi_xterm_key_ResetModifyBuf dansi_xterm_key_DisableModifyBuf;
$attr($must_check)
$extern fn_((dansi_xterm_key_disableModify(
    dansi_xterm_key_Resrc resource, dansi_xterm_key_DisableModifyBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_key_disableModifyWrite(dansi_xterm_key_Resrc resource, io_Writer out))(E$void));

#define dansi_xterm_key_queryModify_static(_resrc_tok) \
    ____dansi_xterm_key_queryModify_static(_resrc_tok)
typedef dansi_xterm_key_ResetModifyBuf dansi_xterm_key_QueryModifyBuf;
$attr($must_check)
$extern fn_((dansi_xterm_key_queryModify(
    dansi_xterm_key_Resrc resource, dansi_xterm_key_QueryModifyBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_key_queryModifyWrite(dansi_xterm_key_Resrc resource, io_Writer out))(E$void));

#define dansi_xterm_key_setFormat_static(_resrc_tok, _format_tok) \
    ____dansi_xterm_key_setFormat_static(_resrc_tok, _format_tok)
typedef A$$(6 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_key_SetFormatBuf;
$attr($must_check)
$extern fn_((dansi_xterm_key_setFormat(
    dansi_xterm_key_Resrc resource, dansi_xterm_key_Format format, dansi_xterm_key_SetFormatBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_key_setFormatWrite(
    dansi_xterm_key_Resrc resource, dansi_xterm_key_Format format, io_Writer out
))(E$void));

#define dansi_xterm_key_resetFormat_static(_resrc_tok) \
    ____dansi_xterm_key_resetFormat_static(_resrc_tok)
typedef dansi_xterm_key_ResetModifyBuf dansi_xterm_key_ResetFormatBuf;
$attr($must_check)
$extern fn_((dansi_xterm_key_resetFormat(
    dansi_xterm_key_Resrc resource, dansi_xterm_key_ResetFormatBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_key_resetFormatWrite(dansi_xterm_key_Resrc resource, io_Writer out))(E$void));

#define dansi_xterm_key_queryFormat_static(_resrc_tok) \
    ____dansi_xterm_key_queryFormat_static(_resrc_tok)
typedef dansi_xterm_key_ResetModifyBuf dansi_xterm_key_QueryFormatBuf;
$attr($must_check)
$extern fn_((dansi_xterm_key_queryFormat(
    dansi_xterm_key_Resrc resource, dansi_xterm_key_QueryFormatBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_key_queryFormatWrite(dansi_xterm_key_Resrc resource, io_Writer out))(E$void));

#define dansi_xterm_key_enableEnhanced_static() \
    ____dansi_xterm_key_enableEnhanced_static()
typedef A$$(18, u8) dansi_xterm_key_EnableEnhancedBuf;
$attr($must_check)
$extern fn_((dansi_xterm_key_enableEnhanced(dansi_xterm_key_EnableEnhancedBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_key_enableEnhancedWrite(io_Writer out))(E$void));

#define dansi_xterm_key_disableEnhanced_static() \
    ____dansi_xterm_key_disableEnhanced_static()
typedef A$$(14, u8) dansi_xterm_key_DisableEnhancedBuf;
$attr($must_check)
$extern fn_((dansi_xterm_key_disableEnhanced(dansi_xterm_key_DisableEnhancedBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_key_disableEnhancedWrite(io_Writer out))(E$void));

$extern fn_((dansi_xterm_key_parseReport(dansi_Seq seq))(O$dansi_xterm_key_Report));
$attr($must_check)
$extern fn_((dansi_xterm_key_interpretReport(
    dansi_xterm_key_Report report
))(dansi_xterm_key_E$dansi_xterm_key_Event));
$extern fn_((dansi_xterm_key_parseEvent(dansi_Seq seq))(O$dansi_xterm_key_Event));

/*========== Macros and Definitions =========================================*/

#define ____dansi_xterm_key_setModifyRaw_static(_resrc_tok, _value_tok) \
    dansi_csi_make_static(">" dansi_xterm_key_Resrc_staticParse(_resrc_tok) ";" _value_tok, "", "m")
#define ____dansi_xterm_key_setModifyLevel_static(_resrc_tok, _level_tok) \
    dansi_xterm_key_setModifyRaw_static(_resrc_tok, dansi_xterm_key_ModifyLevel_staticParse(_level_tok))
#define ____dansi_xterm_key_resetModify_static(_resrc_tok) \
    dansi_csi_make_static(">" dansi_xterm_key_Resrc_staticParse(_resrc_tok), "", "m")
#define ____dansi_xterm_key_disableModify_static(_resrc_tok) \
    dansi_csi_make_static(">" dansi_xterm_key_Resrc_staticParse(_resrc_tok), "", "n")
#define ____dansi_xterm_key_queryModify_static(_resrc_tok) \
    dansi_csi_make_static("?" dansi_xterm_key_Resrc_staticParse(_resrc_tok), "", "m")
#define ____dansi_xterm_key_setFormat_static(_resrc_tok, _format_tok) \
    dansi_csi_make_static(">" dansi_xterm_key_Resrc_staticParse(_resrc_tok) ";" dansi_xterm_key_Format_staticParse(_format_tok), "", "f")
#define ____dansi_xterm_key_resetFormat_static(_resrc_tok) \
    dansi_csi_make_static(">" dansi_xterm_key_Resrc_staticParse(_resrc_tok), "", "f")
#define ____dansi_xterm_key_queryFormat_static(_resrc_tok) \
    dansi_csi_make_static("?" dansi_xterm_key_Resrc_staticParse(_resrc_tok), "", "g")
#define ____dansi_xterm_key_enableEnhanced_static() \
    dansi_xterm_key_setModifyLevel_static(dansi_xterm_key_Resrc_other_keys, dansi_xterm_key_ModifyLevel_other_keys) \
    dansi_xterm_key_setFormat_static(dansi_xterm_key_Resrc_other_keys, dansi_xterm_key_Format_csi_u)
#define ____dansi_xterm_key_disableEnhanced_static() \
    dansi_xterm_key_resetFormat_static(dansi_xterm_key_Resrc_other_keys) \
    dansi_xterm_key_resetModify_static(dansi_xterm_key_Resrc_other_keys)
#define ____dansi_xterm_key_Resrc_staticParse(_resrc_tok) \
    pp_join(____dansi_xterm_key_Resrc_str$, _resrc_tok)
#define ____dansi_xterm_key_Resrc_str$dansi_xterm_key_Resrc_keyboard "0"
#define ____dansi_xterm_key_Resrc_str$dansi_xterm_key_Resrc_cursor_keys "1"
#define ____dansi_xterm_key_Resrc_str$dansi_xterm_key_Resrc_function_keys "2"
#define ____dansi_xterm_key_Resrc_str$dansi_xterm_key_Resrc_keypad_keys "3"
#define ____dansi_xterm_key_Resrc_str$dansi_xterm_key_Resrc_other_keys "4"
#define ____dansi_xterm_key_Resrc_str$dansi_xterm_key_Resrc_modifier_keys "6"
#define ____dansi_xterm_key_Resrc_str$dansi_xterm_key_Resrc_special_keys "7"
#define ____dansi_xterm_key_ModifyLevel_staticParse(_level_tok) \
    pp_join(____dansi_xterm_key_ModifyLevel_str$, _level_tok)
#define ____dansi_xterm_key_ModifyLevel_str$dansi_xterm_key_ModifyLevel_default "0"
#define ____dansi_xterm_key_ModifyLevel_str$dansi_xterm_key_ModifyLevel_modifier_params "1"
#define ____dansi_xterm_key_ModifyLevel_str$dansi_xterm_key_ModifyLevel_other_keys "2"
#define ____dansi_xterm_key_Format_staticParse(_format_tok) \
    pp_join(____dansi_xterm_key_Format_str$, _format_tok)
#define ____dansi_xterm_key_Format_str$dansi_xterm_key_Format_legacy "0"
#define ____dansi_xterm_key_Format_str$dansi_xterm_key_Format_csi_u "1"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_key__included */
