#ifndef prl_ErrTrace__included
#define prl_ErrTrace__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Definitions =========================================*/

#if !defined(ErrTrace_comp_enabled)
#define ErrTrace_comp_enabled (debug_comp_enabled)
#endif /* !defined(ErrTrace_comp_enabled) */

#define ErrTrace_max_frames /* Platform-specific stack trace size optimization */ __comp_int__ErrTrace_max_frames
/// Stack frame info
typedef struct ErrTrace_Frame ErrTrace_Frame;
/// Fixed-size stack trace buffer
typedef struct ErrTrace ErrTrace;

#if !on_comptime
extern void ErrTrace_reset(void);
extern void ErrTrace_captureFrame(void);
extern void ErrTrace_print(void);
#endif /* !on_comptime */

/*========== Implementations ================================================*/

/* clang-format off */
#define __comp_int__ErrTrace_max_frames pp_switch_((arch_bits_unit)( \
    pp_case_((arch_bits_unit_64bit)(pp_switch_((arch_family_type)(\
        pp_case_((arch_family_type_x86)(32)), \
        pp_case_((arch_family_type_arm)(24)), \
        pp_case_((arch_family_type_riscv)(24)), \
        pp_default_(8) \
    )))), \
    pp_case_((arch_bits_unit_32bit)(pp_switch_((arch_family_type)(\
        pp_case_((arch_family_type_x86)(16)), \
        pp_case_((arch_family_type_arm)(12)), \
        pp_case_((arch_family_type_riscv)(12)), \
        pp_default_(8) \
    )))), \
    pp_default_(8) \
))
/* clang-format on */

struct ErrTrace_Frame {
    SrcLoc src_loc;
    P$raw ret_addr;
};

struct ErrTrace {
    var_(frames, A$$(ErrTrace_max_frames, ErrTrace_Frame));
    var_(len, usize);
};

#if on_comptime
#if !ErrTrace_comp_enabled

#define ErrTrace_reset() $unused(0)
#define ErrTrace_captureFrame() $unused(0)
#define ErrTrace_print() $unused(0)

#else /* ErrTrace_comp_enabled */

#define ErrTrace_reset() ErrTrace_reset_callDebug()
#define ErrTrace_captureFrame() ErrTrace_captureFrame_callDebug()
#define ErrTrace_print() ErrTrace_print_callDebug()

#define ErrTrace_reset_callDebug() ErrTrace_reset_debug()
#define ErrTrace_captureFrame_callDebug() ErrTrace_captureFrame_debug(srcLoc(), __builtin_return_address(0))
#define ErrTrace_print_callDebug() ErrTrace_print_debug()

extern void ErrTrace_reset_debug(void);
extern void ErrTrace_captureFrame_debug(SrcLoc src_loc, P$raw ret_addr);
extern void ErrTrace_print_debug(void);

#endif /* ErrTrace_comp_enabled */
#endif /* on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_ErrTrace__included */
