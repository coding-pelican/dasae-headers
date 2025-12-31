/**
 * @file    Window.h
 * @brief   Window management for dh-engine
 * @details A Window is a logical viewport container with its own input state.
 *
 * ## Key Design Changes (from original)
 * 1. **No Backend reference** - Window doesn't know about Backend
 *    - Only holds TargetId (opaque handle)
 *    - Runtime manages Backend ↔ Window relationship
 *
 * 2. **Owns InputState** - No global Input
 *    - Each Window has its own input state
 *    - Multi-window input is naturally separated
 *
 * 3. **Owns Event Queue** - Unified event stream
 *    - Both input and window events in one queue
 *    - Preserves event ordering
 *
 * ## Composition Pipeline
 * 1. Game draws to individual Canvases
 * 2. Window.viewports reference those Canvases
 * 3. dage_Window_composite() blits viewports → composite_buf
 * 4. Runtime presents composite_buf to Backend
 */
#ifndef dage_Window__included
#define dage_Window__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dage/common.h"
#include "dage/Backend.h"
#include "dage/Canvas.h"
#include "dage/Viewport.h"
#include "dage/InputState.h"
#include "dage/Event.h"

/*========== Macros and Declarations ========================================*/

/*========== Window ID ==========*/

typedef u32 dage_WindowId;
T_use_prl$(dage_WindowId);
#define dage_WindowId_invalid ((dage_WindowId)u32_limit_max)

/*========== Window Constants ==========*/

#define dage_Window_viewport_limit (16)
#define dage_Window_event_queue_capacity (256)

/*========== Window Structure ==========*/

/// @brief Window - logical viewport container
typedef struct dage_Window {
    /*=== Identity ===*/
    dage_WindowId id; /* Engine-assigned ID */
    dage_TargetId target_id; /* Backend render target (opaque) */

    /*=== Event System ===*/
    ArrDeq$dage_Event event_que; /* Unified event queue (owns) */
    dage_InputState input; /* Input state snapshot (owns) */

    /*=== Rendering ===*/
    dage_Canvas* composite_buf; /* Final composition target (owns) */
    O$S$dage_Viewport viewports_buf; /* External viewport buffer (optional) */
    ArrList$dage_Viewport viewports; /* Viewport list (owns) */

    /*=== Configuration ===*/
    struct {
        m_V2u32 min_size;
        m_V2u32 max_size;
        color_RGBA clear_color;
    } cfg;

    /*=== Allocator ===*/
    mem_Allocator gpa;
} dage_Window;
T_use_prl$(dage_Window);
T_use_ArrList$(P$dage_Window);

/*========== Window Configuration ==========*/

/// @brief Configuration for creating a window
typedef struct dage_Window_Cfg {
    mem_Allocator gpa;
    m_V2u32 size; /* Initial logical size */
    O$color_RGBA clear_color; /* Composite buffer clear color */
    O$m_V2u32 min_size; /* Minimum size constraint */
    O$m_V2u32 max_size; /* Maximum size constraint */
    O$S$dage_Viewport viewports_buf; /* External viewport buffer (optional) */
} dage_Window_Cfg;

/*========== Lifecycle ==========*/

/// @brief Create a new window
/// @note  Does NOT create Backend target - Runtime does that after init
/// @note  Error boundary - allocation may fail
$attr($must_check)
$extern fn_((dage_Window_init(dage_Window_Cfg cfg))(E$dage_Window));
/// @brief Destroy window and free resources
$extern fn_((dage_Window_fini(dage_Window* self))(void));
/// @brief Bind window to a backend target
/// @note  Called by Runtime after creating target
$attr($inline_always)
$static fn_((dage_Window_bindTarget(dage_Window* self, dage_TargetId target))(void));

/*========== Event Processing ==========*/

/// @brief Push an event to this window's queue
/// @note  Called by Runtime during event dispatch
$extern fn_((dage_Window_pushEvent(dage_Window* self, dage_Event event))(void));
/// @brief Poll next event from queue
/// @return Event if available, None if queue empty
$extern fn_((dage_Window_pollEvent(dage_Window* self))(O$dage_Event));
/// @brief Peek at next event without removing
$extern fn_((dage_Window_peekEvent(const dage_Window* self))(O$dage_Event));
/// @brief Clear all pending events
$extern fn_((dage_Window_clearEvents(dage_Window* self))(void));
/// @brief Check if event queue has events
$attr($inline_always)
$static fn_((dage_Window_hasEvents(const dage_Window* self))(bool));

/*========== Input Queries (Convenience Wrappers) ==========*/

/// @brief Check if key was just pressed this frame
$attr($inline_always)
$static fn_((dage_Window_isKeyPressed(const dage_Window* self, dage_KeyCode key))(bool));
/// @brief Check if key is currently held
$attr($inline_always)
$static fn_((dage_Window_isKeyHeld(const dage_Window* self, dage_KeyCode key))(bool));
/// @brief Check if key was just released
$attr($inline_always)
$static fn_((dage_Window_isKeyReleased(const dage_Window* self, dage_KeyCode key))(bool));
/// @brief Get mouse position in window coordinates
$attr($inline_always)
$static fn_((dage_Window_getMousePos(const dage_Window* self))(m_V2i32));
/// @brief Get mouse position in specific viewport's canvas coordinates
/// @return Canvas coordinates, or None if mouse outside viewport
$extern fn_((dage_Window_getMouseCanvasPos(const dage_Window* self, dage_ViewportId viewport_id))(O$m_V2i32));

/*========== Viewport Management ==========*/

/// @brief Add a viewport to this window
/// @return Viewport ID, or None if limit reached
$extern fn_((dage_Window_addViewport(dage_Window* self, dage_Viewport_Cfg cfg))(O$dage_ViewportId));
/// @brief Remove a viewport
$extern fn_((dage_Window_removeViewport(dage_Window* self, dage_ViewportId id))(void));
/// @brief Get viewport by ID
$extern fn_((dage_Window_getViewport(dage_Window* self, dage_ViewportId id))(O$P$dage_Viewport));
/// @brief Get viewport count
$attr($inline_always)
$static fn_((dage_Window_getViewportCount(const dage_Window* self))(usize));

/*========== Composition ==========*/

/// @brief Composite all viewports into composite_buf
/// @details Viewports are rendered in z_order. Called before present.
$extern fn_((dage_Window_composite(dage_Window* self))(void));
/// @brief Get composite buffer pixels for presentation
/// @return Slice of RGBA32 pixels (read-only)
$attr($inline_always)
$static fn_((dage_Window_getPixels(const dage_Window* self))(S_const$u32));
/// @brief Get composite buffer dimensions
$attr($inline_always)
$static fn_((dage_Window_getSize(const dage_Window* self))(m_V2u32));

/*========== Frame Lifecycle ==========*/

#if UNUSED_CODE
/// @brief Begin frame - prepare for new frame
/// @note  Called by Runtime at frame start
$extern fn_((dage_Window_beginFrame(dage_Window* self))(void));
#endif /* UNUSED_CODE */
/// @brief End frame - clear transient input state
/// @note  Called by Runtime at frame end
$extern fn_((dage_Window_endFrame(dage_Window* self))(void));
/// @brief Handle resize event
/// @note  Resizes composite buffer if needed
$extern fn_((dage_Window_handleResize(dage_Window* self, m_V2u32 new_size))(E$void)) $must_check;

/*========== Macros and Definitions =========================================*/

fn_((dage_Window_bindTarget(dage_Window* self, dage_TargetId target))(void)) {
    claim_assert_nonnull(self);
    self->target_id = target;
};

fn_((dage_Window_hasEvents(const dage_Window* self))(bool)) {
    claim_assert_nonnull(self);
    return ArrDeq_len(*self->event_que.as_raw) > 0;
};

fn_((dage_Window_isKeyPressed(const dage_Window* self, dage_KeyCode key))(bool)) {
    claim_assert_nonnull(self);
    return dage_InputState_isKeyPressed(&self->input, key);
};

fn_((dage_Window_isKeyHeld(const dage_Window* self, dage_KeyCode key))(bool)) {
    claim_assert_nonnull(self);
    return dage_InputState_isKeyHeld(&self->input, key);
};

fn_((dage_Window_getMousePos(const dage_Window* self))(m_V2i32)) {
    claim_assert_nonnull(self);
    return dage_InputState_getMousePos(&self->input);
};

fn_((dage_Window_getViewportCount(const dage_Window* self))(usize)) {
    claim_assert_nonnull(self);
    return ArrList_len(*self->viewports.as_raw);
};

fn_((dage_Window_getPixels(const dage_Window* self))(S_const$u32)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(self->composite_buf);
    let pixels = self->composite_buf->gird.items;
    /* Canvas grid items are color_RGBA which is u32-compatible */
    return P_prefix$((S_const$u32)(ptrCast$((const u32*)(pixels.ptr)))(pixels.len));
};

fn_((dage_Window_getSize(const dage_Window* self))(m_V2u32)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(self->composite_buf);
    return (m_V2u32){
        .x = self->composite_buf->gird.width,
        .y = self->composite_buf->gird.height,
    };
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_Window__included */
