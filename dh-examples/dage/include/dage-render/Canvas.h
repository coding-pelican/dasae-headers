#ifndef dage_render_Canvas__included
#define dage_render_Canvas__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

T_alias$((dage_Canvas)(struct dage_Canvas {
    var_(gird, Grid$dacolor_RGBA);
    var_(default_color, dacolor_RGBA);
}));
T_use_prl$(dage_Canvas);

T_alias$((dage_Canvas_Cfg)(struct dage_Canvas_Cfg {
    var_(gpa, mem_Alctr);
    var_(width, u32);
    var_(height, u32);
    var_(default_color, O$dacolor_RGBA);
}));

/// Canvas creation with specific type
$attr($must_check)
$extern fn_((dage_Canvas_init(dage_Canvas_Cfg cfg))(E$dage_Canvas));
$extern fn_((dage_Canvas_fini(dage_Canvas* self, mem_Alctr gpa))(void));
$attr($must_check)
$extern fn_((dage_Canvas_resize(dage_Canvas* self, mem_Alctr gpa, u32 width, u32 height))(E$void));
$extern fn_((dage_Canvas_clear(dage_Canvas* self, O$dacolor_RGBA color))(void));

$extern fn_((dage_Canvas_drawPixel(dage_Canvas* self, i32 x, i32 y, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_drawLine(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_drawHLine(dage_Canvas* self, i32 x1, i32 x2, i32 y, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_drawVLine(dage_Canvas* self, i32 x, i32 y1, i32 y2, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_drawRect(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_drawCircle(dage_Canvas* self, i32 cx, i32 cy, i32 radius, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_drawRing(dage_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_drawArc(dage_Canvas* self, i32 cx, i32 cy, i32 radius, f32 start_angle, f32 end_angle, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_drawAngleRing(dage_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, f32 start_angle, f32 end_angle, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_drawCapsule(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_drawLineThick(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, f32 thickness, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_drawRectThick(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, f32 thickness, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_drawRectBorderByCutout(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, i32 thickness, dacolor_RGBA inner_color, dacolor_RGBA border_color))(void));

$extern fn_((dage_Canvas_fillRect(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_fillCircle(dage_Canvas* self, i32 cx, i32 cy, i32 radius, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_fillRingByCutout(dage_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, dacolor_RGBA inner_color, dacolor_RGBA outer_color))(void));
$extern fn_((dage_Canvas_fillRingByScanlines(dage_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_fillArc(dage_Canvas* self, i32 cx, i32 cy, i32 radius, f32 start_angle, f32 end_angle, dacolor_RGBA color))(void));
$extern fn_((dage_Canvas_fillAngleRingByCutout(dage_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, f32 start_angle, f32 end_angle, dacolor_RGBA inner_color, dacolor_RGBA outer_color))(void));
$extern fn_((dage_Canvas_fillCapsule(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, dacolor_RGBA color))(void));

/// Specialized canvas operations
$extern fn_((dage_Canvas_blit(dage_Canvas* dst, const dage_Canvas* src, i32 x, i32 y))(void));
$extern fn_((dage_Canvas_blitScaled(dage_Canvas* dst, const dage_Canvas* src, i32 x, i32 y, f32 scale))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_render_Canvas__included */
