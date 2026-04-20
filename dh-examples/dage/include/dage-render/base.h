#ifndef dage_render_base__included
#define dage_render_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dage-base.h"
#include <dacolor.h>
#include <Grid.h>

/*========== Macros and Declarations ========================================*/

T_use_Grid$(dacolor_RGBA);
T_use_Grid$(dacolor_RGB);
T_use_Grid$(dacolor_HSL);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_render_base__included */
