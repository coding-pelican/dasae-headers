#ifndef prl_ErrTrace__included
#define prl_ErrTrace__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

#if !defined(ETrace_comp_enabled)
#define ETrace_comp_enabled (debug_comp_enabled)
#endif /* !defined(ETrace_comp_enabled) */

#define ETrace_max_frames /* Platform-specific stack trace size optimization */ __comp_int__ETrace_max_frames
/// Stack frame info
typedef struct ETrace_Frame ETrace_Frame;
/// Fixed-size stack trace buffer
typedef struct ETrace ETrace;

#if !on_comptime
extern void ETrace_reset(void);
extern void ETrace_captureFrame(void);
extern void ETrace_print(void);
#endif /* !on_comptime */

/*========== Macros and Definitions =========================================*/

#define __comp_int__ETrace_max_frames pp_expand( \
    pp_switch_ pp_begin(arch_bits_unit)( \
        pp_case_((arch_bits_unit_64bit)(pp_expand( \
            pp_switch_ pp_begin(arch_family_type)( \
                pp_case_((arch_family_type_x86)(32)), \
                pp_case_((arch_family_type_arm)(24)), \
                pp_case_((arch_family_type_riscv)(24)), \
                pp_default_(8) \
            ) pp_end \
        ))), \
        pp_case_((arch_bits_unit_32bit)(pp_expand( \
            pp_switch_ pp_begin(arch_family_type)( \
                pp_case_((arch_family_type_x86)(16)), \
                pp_case_((arch_family_type_arm)(12)), \
                pp_case_((arch_family_type_riscv)(12)), \
                pp_default_(8) \
            ) pp_end \
        ))), \
        pp_default_(8) \
    ) pp_end \
)

struct ETrace_Frame {
    SrcLoc src_loc;
    P$raw ret_addr;
};

struct ETrace {
    var_(frames, A$$(ETrace_max_frames, ETrace_Frame));
    var_(len, usize);
};

#if on_comptime
#if !ETrace_comp_enabled

#define ETrace_reset() $unused(0)
#define ETrace_captureFrame() $unused(0)
#define ETrace_print() $unused(0)

#else /* ETrace_comp_enabled */

#define ETrace_reset() ETrace_reset_callDebug()
#define ETrace_captureFrame() ETrace_captureFrame_callDebug()
#define ETrace_print() ETrace_print_callDebug()

#define ETrace_reset_callDebug() ETrace_reset_debug()
#define ETrace_captureFrame_callDebug() ETrace_captureFrame_debug(srcLoc(), __builtin_return_address(0))
#define ETrace_print_callDebug() ETrace_print_debug()

extern void ETrace_reset_debug(void);
extern void ETrace_captureFrame_debug(SrcLoc src_loc, P$raw ret_addr);
extern void ETrace_print_debug(void);

#endif /* ETrace_comp_enabled */
#endif /* on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_ErrTrace__included */
