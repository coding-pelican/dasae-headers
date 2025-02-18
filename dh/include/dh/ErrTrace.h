#ifndef ERR_TRACE_INCLUDED
#define ERR_TRACE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Definitions =========================================*/

#if !defined(ErrTrace_comp_enabled)
#define ErrTrace_comp_enabled (debug_comp_enabled)
#endif /* !defined(ErrTrace_comp_enabled) */

#define ErrTrace_max_frames /* Platform-specific stack trace size optimization */ VAL__ErrTrace_max_frames
/// Stack frame info
typedef struct ErrTrace_Frame ErrTrace_Frame;
/// Fixed-size stack trace buffer
typedef struct ErrTrace       ErrTrace;

#if !COMP_TIME
extern void ErrTrace_reset(void);
extern void ErrTrace_captureFrame(void);
extern void ErrTrace_print(void);
#endif /* !COMP_TIME */

/*========== Implementations ================================================*/

// clang-format off
#if bti_plat_64bit
    #if bti_arch_family_x86
        #define VAL__ErrTrace_max_frames (32)
    #elif bti_arch_family_arm
        #define VAL__ErrTrace_max_frames (24)
    #elif bti_arch_family_riscv
        #define VAL__ErrTrace_max_frames (24)
    #endif /* bti_arch_family_riscv */
#else /* bti_plat_32bit */
    #if bti_arch_family_x86
        #define VAL__ErrTrace_max_frames (16)
    #elif bti_arch_family_arm
        #define VAL__ErrTrace_max_frames (12)
    #elif bti_arch_family_riscv
        #define VAL__ErrTrace_max_frames (12)
    #endif /* bti_arch_family_riscv */
#endif /* bti_plat_32bit */
#if !defined(VAL__ErrTrace_max_frames)
    #define VAL__ErrTrace_max_frames (8)
#endif /* !defined(VAL__ErrTrace_max_frames) */
// clang-format on

struct ErrTrace_Frame {
    SrcLoc src_loc;
    anyptr ret_addr;
};

struct ErrTrace {
    ErrTrace_Frame frames[ErrTrace_max_frames];
    usize          len;
};

#if COMP_TIME
#if !ErrTrace_comp_enabled

#define ErrTrace_reset()        unused(0)
#define ErrTrace_captureFrame() unused(0)
#define ErrTrace_print()        unused(0)

#else /* ErrTrace_comp_enabled */

#define ErrTrace_reset()        ErrTrace_reset_callDebug()
#define ErrTrace_captureFrame() ErrTrace_captureFrame_callDebug()
#define ErrTrace_print()        ErrTrace_print_callDebug()

#define ErrTrace_reset_callDebug()        ErrTrace_reset_debug()
#define ErrTrace_captureFrame_callDebug() ErrTrace_captureFrame_debug(srcLoc(), __builtin_return_address(0))
#define ErrTrace_print_callDebug()        ErrTrace_print_debug()

extern void ErrTrace_reset_debug(void);
extern void ErrTrace_captureFrame_debug(SrcLoc src_loc, anyptr ret_addr);
extern void ErrTrace_print_debug(void);

#endif /* ErrTrace_comp_enabled */
#endif /* COMP_TIME */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ERR_TRACE_INCLUDED */
