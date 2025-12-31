/**
 * @file    Viewport.h
 * @brief   Viewport system for dage
 * @details Manages the mapping from Canvas (source) to Window (destination).
 *          Handles coordinate transformation, scaling, and composition order.
 *
 * ## Coordinate Spaces
 * 1. Canvas Space  - Pixel coordinates within the source Canvas
 * 2. Viewport Space - Normalized [0,1] or absolute within the viewport
 * 3. Window Space  - Pixel coordinates within the Window's composite buffer
 * 4. Screen Space  - Physical screen pixels (handled by Backend)
 *
 * ## Composition
 * Window contains multiple Viewports, each referencing a Canvas.
 * Viewports are composited in z_order to the Window's composite buffer.
 */
#ifndef dage_Viewport__included
#define dage_Viewport__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dage/common.h"
#include "dh/ArrList.h"

/*========== Macros and Declarations ========================================*/

/* Forward declaration */
typedef struct dage_Canvas dage_Canvas;

/*========== Viewport Fit Modes ==========*/

/// @brief How to fit source canvas into destination rectangle
typedef enum_(dage_Viewport_Fit $bits(8)) {
    dage_Viewport_Fit_stretch, /* Stretch to fill dst (ignores aspect ratio) */
    dage_Viewport_Fit_contain, /* Fit inside dst (letterbox/pillarbox) */
    dage_Viewport_Fit_cover, /* Fill dst completely (may crop) */
    dage_Viewport_Fit_none, /* 1:1 pixel mapping (may overflow) */
} dage_Viewport_Fit;

/*========== Rectangle Types ==========*/

/// @brief Integer rectangle (for pixel coordinates)
typedef struct dage_Rect_i32 {
    m_V2i32 pos; /* Top-left corner */
    m_V2u32 size; /* Width and height */
} dage_Rect_i32;
T_use_prl$(dage_Rect_i32);

/// @brief Float rectangle (for normalized coordinates)
typedef struct dage_Rect_f32 {
    m_V2f32 pos; /* Top-left corner */
    m_V2f32 size; /* Width and height */
} dage_Rect_f32;
T_use_prl$(dage_Rect_f32);

/*========== Viewport Structure ==========*/

/// @brief Viewport - maps a Canvas region to a Window region
typedef struct dage_Viewport {
    /*=== Source (Canvas Space) ===*/
    dage_Canvas* canvas; /* Source canvas (non-owning reference) */
    dage_Rect_i32 src; /* Region of canvas to display (0,0 + canvas size = full) */

    /*=== Destination (Window Space) ===*/
    dage_Rect_i32 dst; /* Where to render in Window */

    /*=== Transform ===*/
    dage_Viewport_Fit fit; /* How to fit src into dst */
    f32 rotation; /* Rotation in radians (0 = no rotation) */
    f32 opacity; /* Blend opacity [0,1] */

    /*=== Ordering ===*/
    i16 z_order; /* Composition order (lower = behind) */
    bool visible; /* Whether to render this viewport */
} dage_Viewport;
T_use_prl$(dage_Viewport);
T_use_ArrList$(dage_Viewport);

/*========== Viewport ID ==========*/

typedef u32 dage_ViewportId;
T_use_prl$(dage_ViewportId);
#define dage_ViewportId_invalid ((dage_ViewportId)UINT32_MAX)

/*========== Viewport Configuration ==========*/

/// @brief Configuration for creating a viewport
typedef struct dage_Viewport_Cfg {
    dage_Canvas* canvas; /* Required: source canvas */
    O$dage_Rect_i32 src_rect; /* Optional: source region (None = full canvas) */
    dage_Rect_i32 dst_rect; /* Required: destination in window */
    dage_Viewport_Fit fit; /* Default: stretch */
    f32 opacity; /* Default: 1.0 */
    i16 z_order; /* Default: 0 */
    bool visible; /* Default: true */
} dage_Viewport_Cfg;

/*========== Viewport API ==========*/

/// @brief Initialize viewport from configuration
$extern fn_((dage_Viewport_init(dage_Viewport_Cfg cfg))(dage_Viewport));
/// @brief Update source canvas reference
$attr($inline_always)
$static fn_((dage_Viewport_setCanvas(dage_Viewport* self, dage_Canvas* canvas))(void));
/// @brief Update destination rectangle
$attr($inline_always)
$static fn_((dage_Viewport_setDstRect(dage_Viewport* self, dage_Rect_i32 rect))(void));
/// @brief Set visibility
$attr($inline_always)
$static fn_((dage_Viewport_setVisible(dage_Viewport* self, bool visible))(void));

/*========== Coordinate Transformation ==========*/

/// @brief Transform Canvas-space coordinate to Window-space
/// @param self       Viewport
/// @param canvas_pos Position in Canvas coordinates
/// @return           Position in Window coordinates
$extern fn_((dage_Viewport_canvasToWindow(const dage_Viewport* self, m_V2i32 canvas_pos))(m_V2i32));
/// @brief Transform Window-space coordinate to Canvas-space
/// @param self       Viewport
/// @param window_pos Position in Window coordinates
/// @return           Position in Canvas coordinates, or None if outside viewport
$extern fn_((dage_Viewport_windowToCanvas(const dage_Viewport* self, m_V2i32 window_pos))(O$m_V2i32));
/// @brief Check if Window-space point is inside this viewport
$extern fn_((dage_Viewport_containsPoint(const dage_Viewport* self, m_V2i32 window_pos))(bool));
/// @brief Get effective scale factor (based on src/dst sizes and fit mode)
$extern fn_((dage_Viewport_getEffectiveScale(const dage_Viewport* self))(m_V2f32));

/*========== Macros and Definitions =========================================*/

fn_((dage_Viewport_setCanvas(dage_Viewport* self, dage_Canvas* canvas))(void)) {
    debug_assert_nonnull(self);
    self->canvas = canvas;
};

fn_((dage_Viewport_setDstRect(dage_Viewport* self, dage_Rect_i32 rect))(void)) {
    debug_assert_nonnull(self);
    self->dst = rect;
};

fn_((dage_Viewport_setVisible(dage_Viewport* self, bool visible))(void)) {
    debug_assert_nonnull(self);
    self->visible = visible;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_Viewport__included */
