#ifndef prl_ErrTrace__included
#define prl_ErrTrace__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

#if !defined(ETrace_comp_enabled)
#define ETrace_comp_enabled debug_comp_enabled
#endif /* !defined(ETrace_comp_enabled) */

#define ETrace_max_frames /* Platform-specific stack trace size optimization */ __comp_int__ETrace_max_frames
/// Stack frame info
T_alias$((ETrace_Frame)(struct ETrace_Frame));
/// Fixed-size stack trace buffer
T_alias$((ETrace)(struct ETrace));

#if !on_comptime
$extern fn_((ETrace_reset(void))(void));
$extern fn_((ETrace_enable(void))(void));
$extern fn_((ETrace_disable(void))(void));
$extern fn_((ETrace_isEnabled(void))(bool));
$extern fn_((ETrace_depth(void))(usize));
$extern fn_((ETrace_captureFrame(void))(void));
$extern fn_((ETrace_print(void))(void));
#endif /* !on_comptime */

/*========== Macros and Definitions =========================================*/

#define __comp_int__ETrace_max_frames pp_expand( \
    pp_switch_ pp_begin(arch_bits_unit)( \
        pp_case_((arch_bits_unit_64bit)(pp_expand( \
            pp_switch_ pp_begin(arch_family_type)( \
                pp_case_((arch_family_type_x86)(64)), \
                pp_case_((arch_family_type_arm)(48)), \
                pp_case_((arch_family_type_riscv)(48)), \
                pp_default_(32) \
            ) pp_end \
        ))), \
        pp_case_((arch_bits_unit_32bit)(pp_expand( \
            pp_switch_ pp_begin(arch_family_type)( \
                pp_case_((arch_family_type_x86)(32)), \
                pp_case_((arch_family_type_arm)(24)), \
                pp_case_((arch_family_type_riscv)(24)), \
                pp_default_(16) \
            ) pp_end \
        ))), \
        pp_default_(16) \
    ) pp_end \
)

struct ETrace_Frame {
    var_(src_loc, SrcLoc);
    var_(ret_addr, P$raw);
};

struct ETrace {
    var_(rest_frames, A$$(ETrace_max_frames - 1, ETrace_Frame));
    var_(last_frame, O$$(ETrace_Frame));
    var_(depth, usize);
    var_(is_enabled, bool);
};

#if on_comptime
#if !ETrace_comp_enabled

#define ETrace_reset() $unused(0)
#define ETrace_enable() $unused(0)
#define ETrace_disable() $unused(0)
#define ETrace_isEnabled() (false)
#define ETrace_depth() (0)
#define ETrace_captureFrame() $unused(0)
#define ETrace_print() $unused(0)

#else /* ETrace_comp_enabled */

#define ETrace_reset() ETrace_reset_callDebug()
#define ETrace_enable() ETrace_enable_callDebug()
#define ETrace_disable() ETrace_disable_callDebug()
#define ETrace_isEnabled() ETrace_isEnabled_callDebug()
#define ETrace_depth() ETrace_depth_callDebug()
#define ETrace_captureFrame() ETrace_captureFrame_callDebug()
#define ETrace_print() ETrace_print_callDebug()

#define ETrace_reset_callDebug() ETrace_reset_debug()
#define ETrace_enable_callDebug() ETrace_enable_debug()
#define ETrace_disable_callDebug() ETrace_disable_debug()
#define ETrace_isEnabled_callDebug() ETrace_isEnabled_debug()
#define ETrace_depth_callDebug() ETrace_depth_debug()
#define ETrace_print_callDebug() ETrace_print_debug()

#if arch_is_wasm_family
#define ETrace_captureFrame_callDebug() ETrace_captureFrame_debug(srcLoc(), null)
#else
#define ETrace_captureFrame_callDebug() ETrace_captureFrame_debug(srcLoc(), __builtin_return_address(0))
#endif

#endif /* ETrace_comp_enabled */
#endif /* on_comptime */

$extern fn_((ETrace_reset_debug(void))(void));
$extern fn_((ETrace_enable_debug(void))(void));
$extern fn_((ETrace_disable_debug(void))(void));
$extern fn_((ETrace_isEnabled_debug(void))(bool));
$extern fn_((ETrace_depth_debug(void))(usize));
$extern fn_((ETrace_captureFrame_debug(SrcLoc src_loc, P$raw ret_addr))(void));
$extern fn_((ETrace_print_debug(void))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_ErrTrace__included */
