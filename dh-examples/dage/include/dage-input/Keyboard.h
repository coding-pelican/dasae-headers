#ifndef dage_input_Keyboard__included
#define dage_input_Keyboard__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "KeyBtnFlags.h"
#include "KeyMods.h"
#include "KeyCode.h"

/*========== Macros and Declarations ========================================*/

T_alias$((dage_Keyboard_KeyStateBuf)(A$$(dage_KeyCode$count, dage_KeyBtnFlags)));
/// @brief Keyboard state snapshot
T_alias$((dage_Keyboard)(struct dage_Keyboard {
    var_(key_states, dage_Keyboard_KeyStateBuf);
}));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_input_Keyboard__included */
