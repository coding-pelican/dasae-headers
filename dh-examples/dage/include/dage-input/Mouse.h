#ifndef dage_input_Mouse__included
#define dage_input_Mouse__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "KeyBtnFlags.h"
#include "KeyMods.h"
#include "MouseBtn.h"

/*========== Macros and Declarations ========================================*/

T_alias$((dage_Mouse_BtnStateBuf)(A$$(dage_MouseBtn$count, dage_KeyBtnFlags)));
/// @brief Mouse state snapshot
T_alias$((dage_Mouse)(struct dage_Mouse {
    var_(btn_states, dage_Mouse_BtnStateBuf);
    var_(pos, m_V2i32); /* current (window-space) */
    var_(scroll_delta, m_V2f32); /* since last frame */
    var_(inside_window, bool); /* bounds (window-space) */
}));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_input_Mouse__included */
