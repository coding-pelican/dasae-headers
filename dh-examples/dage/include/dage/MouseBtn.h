#ifndef dage_MouseBtn__included
#define dage_MouseBtn__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dage/common.h"

/// Mouse button identifiers
typedef enum_(dage_MouseBtn $bits(8)) {
    dage_MouseBtn_unknown = -1,

    /* Standard mouse buttons */
    dage_MouseBtn_left = 0,
    dage_MouseBtn_right = 1,
    dage_MouseBtn_middle = 2,

    /* Extended mouse buttons */
    dage_MouseBtn_x1 = 3,
    dage_MouseBtn_x2 = 4,

    dage_MouseBtn_count = 5
} dage_MouseBtn;
T_use_prl$(dage_MouseBtn);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_MouseBtn__included */
