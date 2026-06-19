/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    csi.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_csi
 */
#pragma once
#ifndef dansi_csi__included
#define dansi_csi__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Seq.h"

/*========== Macros and Declarations ========================================*/

errset_((dansi_csi_E)(dansi_csi_Invalid));

typedef struct dansi_csi_Frame dansi_csi_Frame;
T_decl_E$($set(dansi_csi_E)(dansi_csi_Frame));

$attr($must_check)
$extern fn_((dansi_csi_parse(S_const$u8 bytes))(dansi_csi_E$dansi_csi_Frame));
#define dansi_csi_make_static(_params_tok, _intermediates_tok, _final_tok) \
    ____dansi_csi_make_static(_params_tok, _intermediates_tok, _final_tok)
#define dansi_csi_make0_static(_final_tok) \
    ____dansi_csi_make0_static(_final_tok)
#define dansi_csi_make1_static(_param_tok, _final_tok) \
    ____dansi_csi_make1_static(_param_tok, _final_tok)
#define dansi_csi_make2_static(_param0_tok, _param1_tok, _final_tok) \
    ____dansi_csi_make2_static(_param0_tok, _param1_tok, _final_tok)
#define dansi_csi_makePrivate1_static(_param_tok, _final_tok) \
    ____dansi_csi_makePrivate1_static(_param_tok, _final_tok)
$attr($must_check)
$extern fn_((dansi_csi_make(S_const$u8 params, S_const$u8 intermediates, u8 final, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_csi_write(S_const$u8 params, S_const$u8 intermediates, u8 final, io_Writer out))(E$void));

struct dansi_csi_Frame {
    var_(params, S_const$u8);
    var_(intermediates, S_const$u8);
    var_(final, u8);
    var_(bytes, S_const$u8);
};
T_use_prl$(dansi_csi_Frame);
T_impl_E$($set(dansi_csi_E)(dansi_csi_Frame));
$extern fn_((dansi_csi_Frame_isPrivate(dansi_csi_Frame self, u8 marker))(bool));
$extern fn_((dansi_csi_Frame_privateMarker(dansi_csi_Frame self))(O$u8));

typedef struct dansi_csi_Param dansi_csi_Param;
T_decl_prl$(dansi_csi_Param);
$extern fn_((dansi_csi_Frame_paramAt(dansi_csi_Frame self, usize index))(O$dansi_csi_Param));
$extern fn_((dansi_csi_Frame_paramAtAsU16(dansi_csi_Frame self, usize index))(O$u16));
typedef struct dansi_csi_ParamIter dansi_csi_ParamIter;
$extern fn_((dansi_csi_Frame_paramIter(dansi_csi_Frame self))(dansi_csi_ParamIter));
typedef struct dansi_csi_SubparamIter dansi_csi_SubparamIter;
$extern fn_((dansi_csi_Param_subparamIter(dansi_csi_Param self))(dansi_csi_SubparamIter));

struct dansi_csi_Param {
    var_(raw, S_const$u8);
};
T_impl_prl$(dansi_csi_Param);
$extern fn_((dansi_csi_Param_asU16(dansi_csi_Param self))(O$u16));

struct dansi_csi_ParamIter {
    var_(params, S_const$u8);
    var_(cursor, usize);
};
T_use_prl$(dansi_csi_ParamIter);
$extern fn_((dansi_csi_ParamIter_next(dansi_csi_ParamIter* self))(O$dansi_csi_Param));

struct dansi_csi_SubparamIter {
    var_(param, S_const$u8);
    var_(cursor, usize);
};
T_use_prl$(dansi_csi_SubparamIter);
$extern fn_((dansi_csi_SubparamIter_next(dansi_csi_SubparamIter* self))(O$S_const$u8));

/*========== Macros and Definitions =========================================*/

#define ____dansi_csi_make_static(_params_tok, _intermediates_tok, _final_tok) \
    "\x1b[" _params_tok _intermediates_tok _final_tok
#define ____dansi_csi_make0_static(_final_tok) \
    dansi_csi_make_static("", "", _final_tok)
#define ____dansi_csi_make1_static(_param_tok, _final_tok) \
    dansi_csi_make_static(_param_tok, "", _final_tok)
#define ____dansi_csi_make2_static(_param0_tok, _param1_tok, _final_tok) \
    dansi_csi_make_static(_param0_tok ";" _param1_tok, "", _final_tok)
#define ____dansi_csi_makePrivate1_static(_param_tok, _final_tok) \
    dansi_csi_make_static("?" _param_tok, "", _final_tok)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_csi__included */
