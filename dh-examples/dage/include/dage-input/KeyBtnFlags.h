#ifndef dage_input_KeyBtnFlags__included
#define dage_input_KeyBtnFlags__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dage-base.h"

/*========== Macros and Declarations ========================================*/

/*--- Key/Button State Flags ---*/

/// @brief State flags for keys/buttons
T_alias$((dage_KeyBtnFlags)(enum_((dage_KeyBtnFlags $fits($packed))(
    dage_KeyBtnFlags_none = 0,
    dage_KeyBtnFlags_pressed = (1 << 0), /* Just pressed this frame */
    dage_KeyBtnFlags_held = (1 << 1), /* Currently held down */
    dage_KeyBtnFlags_released = (1 << 2), /* Just released this frame */
))));
claim_assert_static(eqlType$(dage_KeyBtnFlags, u8));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_input_KeyBtnFlags__included */
