#ifndef dage_input_MouseBtn__included
#define dage_input_MouseBtn__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dage-base.h"

/*========== Macros and Declarations ========================================*/

/// Mouse button identifiers
T_alias$((dage_MouseBtn)(enum_((dage_MouseBtn $fits($packed))(
    dage_MouseBtn_unknown = 0xFF,

    /* Standard mouse buttons */
    dage_MouseBtn_left = 0,
    dage_MouseBtn_right = 1,
    dage_MouseBtn_middle = 2,

    /* Extended mouse buttons */
    dage_MouseBtn_x1 = 3,
    dage_MouseBtn_x2 = 4,

    dage_MouseBtn$count = 5
))));
claim_assert_static(eqlType$(dage_MouseBtn, u8));
T_use_prl$(dage_MouseBtn);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_input_MouseBtn__included */
