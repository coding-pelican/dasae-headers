#ifndef dage_MouseButton__included
#define dage_MouseButton__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dage/common.h"

/// Mouse button identifiers
typedef enum_(dage_MouseButton $bits(8)) {
    dage_MouseButton_unknown = -1,

    /* Standard mouse buttons */
    dage_MouseButton_left = 0,
    dage_MouseButton_right = 1,
    dage_MouseButton_middle = 2,

    /* Extended mouse buttons */
    dage_MouseButton_x1 = 3,
    dage_MouseButton_x2 = 4,

    dage_MouseButton_count = 5
} dage_MouseButton;
T_use_prl$(dage_MouseButton);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_MouseButton__included */
