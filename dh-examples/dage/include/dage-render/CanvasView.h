#ifndef dage_render_CanvasView__included
#define dage_render_CanvasView__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Canvas.h"

/*========== Macros and Declarations ========================================*/

T_alias$((dage_CanvasView)(struct dage_CanvasView {
    var_(canvas, dage_Canvas*); // Associated canvas
    var_(pos_on_window, struct {
        var_(top_left, m_V2i32);
    }); // Position in window coordinates
    var_(rect, struct {
        var_(size, m_V2u32); // Size in window coordinates
        var_(scale, m_V2f32); // Scale in window coordinates
        /* TODO: Add options: stretch, crop, resizable, scalable, ... */
        var_(is_resizable, struct {
            var_(x, bool);
            var_(y, bool);
        });
    });
    var_(is_visible, bool); // Visibility flag
}));
T_use_prl$(dage_CanvasView);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_render_CanvasView__included */
