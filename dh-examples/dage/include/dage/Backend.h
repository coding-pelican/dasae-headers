/**
 * @file    Backend.h
 * @brief   Abstract backend interface for dage
 * @details Pure V-Table interface pattern (like mem_Allocator).
 *          Backend only manages render targets - knows nothing about dage_Window.
 *
 * @note    External extension point - users can implement custom backends
 *          (VT100, WSI, Vulkan, Notepad, etc.) without modifying engine core.
 *
 * ## Design Principles
 * 1. Backend is a "render target manager" - creates surfaces, pumps OS events, presents pixels
 * 2. Engine objects (Window, Canvas) are unknown to Backend - only TargetId matters
 * 3. Event routing is pull-based (pumpEvents returns array, Runtime dispatches)
 * 4. Always RGBA32 pixel format - Backend handles conversion internally
 */
#ifndef dage_Backend__included
#define dage_Backend__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dage/common.h"
#include "dage/Event.h"

/*========== Macros and Declarations ========================================*/

/*========== Forward Declarations ==========*/

/// @brief Opaque backend implementation context
typedef struct dage_Backend_Impl dage_Backend_Impl;

/*========== Target Types ==========*/

/// @brief Render target identifier
/// @details Opaque handle to a platform-specific surface (HWND, X11 Window, ncurses screen, etc.)
///          Only meaningful to the Backend that created it.
typedef u32 dage_TargetId;
T_use_prl$(dage_TargetId);
#define dage_TargetId_invalid ((dage_TargetId)0)

/// @brief Target creation configuration
typedef struct dage_Target_Cfg {
    u32 width;
    u32 height;
    f32 scale; /* DPI scale hint (1.0 = native) */
    S_const$u8 title; /* Window title (for windowed targets) */
    bool resizable;
    bool borderless;
    bool fullscreen;
} dage_Target_Cfg;

/// @brief Target metrics (SSOT for platform state)
/// @details Only Backend knows the true state - this is the single source of truth.
typedef struct dage_TargetMetrics {
    m_V2i32 position; /* Screen-space position */
    m_V2u32 physical_size; /* Physical pixels */
    m_V2u32 logical_size; /* Logical units (may differ with DPI) */
    f32 dpi_scale;
    bool is_focused;
    bool is_minimized;
    bool is_maximized;
    bool is_fullscreen;
    bool should_close; /* Close requested by user/system */
} dage_TargetMetrics;

/*========== Raw Event (Backend -> Runtime) ==========*/

/// @brief Event packet with target association
/// @details Backend generates these, Runtime dispatches to appropriate Window.
typedef struct dage_RawEvent {
    dage_TargetId target_id; /* Which target generated this event */
    dage_Event base; /* The event data */
} dage_RawEvent;
T_use_prl$(dage_RawEvent);

/*========== Backend V-Table ==========*/

/// @brief Backend virtual function table
/// @details Implement this to create a custom backend.
///          All functions receive ctx as first parameter (like C++ this).
typedef struct dage_Backend_VT {
    /*=== Target Management ===*/

    /// @brief Create a new render target
    /// @param ctx   Backend context
    /// @param cfg   Target configuration
    /// @return      Target ID on success, error on failure (OS resource allocation)
    /// @note        Error boundary - may fail due to OS resource limits
    $attr($must_check)
    fn_(((*createTarget)(dage_Backend_Impl* ctx, dage_Target_Cfg cfg))(E$dage_TargetId));

    /// @brief Destroy a render target
    /// @param ctx   Backend context
    /// @param id    Target to destroy
    /// @note        No error - invalid ID is programmer error (assert)
    fn_(((*destroyTarget)(dage_Backend_Impl* ctx, dage_TargetId id))(void));

    /*=== Event System (Pull Model) ===*/

    /// @brief Pump OS events into buffer
    /// @param ctx         Backend context
    /// @param out_events  Caller-provided buffer (slice with capacity = len)
    /// @return            Slice of events actually written
    /// @note              Never fails - empty slice is valid (no events)
    ///                    out_events must be valid slice (assert on null)
    fn_(((*pumpEvents)(dage_Backend_Impl* ctx, S$dage_RawEvent out_events))(S$dage_RawEvent));

    /*=== Presentation ===*/

    /// @brief Present pixels to target
    /// @param ctx     Backend context
    /// @param id      Target to present to
    /// @param pixels  RGBA32 pixel data (engine standard format)
    /// @param w, h    Dimensions (assert: pixels.len == w * h)
    /// @note          Never fails - Backend handles format conversion internally
    ///                (e.g., VT100 converts RGBA → ASCII art)
    fn_(((*present)(dage_Backend_Impl* ctx, dage_TargetId id, S_const$u32 pixels, u32 w, u32 h))(void));

    /*=== Queries ===*/

    /// @brief Get target metrics
    /// @param ctx   Backend context
    /// @param id    Target to query
    /// @return      Current metrics (SSOT)
    /// @note        Never fails - invalid ID is programmer error (assert)
    fn_(((*getTargetMetrics)(dage_Backend_Impl* ctx, dage_TargetId id))(dage_TargetMetrics));
} dage_Backend_VT;

/*========== Backend Object (Fat Pointer) ==========*/

/// @brief Backend instance
/// @details Fat pointer pattern - ctx holds implementation data, vt holds operations.
///          Similar to mem_Allocator design in dh.
typedef struct dage_Backend {
    dage_Backend_Impl* ptr;
    const dage_Backend_VT* vt;
} dage_Backend;
T_use_prl$(dage_Backend);

/*========== Public API (Wrappers) ==========*/

/// @brief Check if backend is valid
$attr($inline_always)
$static fn_((dage_Backend_isValid(dage_Backend self))(bool));
/// @brief Assert backend is valid
$attr($inline_always)
$static fn_((dage_Backend_assertValid(dage_Backend_Impl* ptr, const dage_Backend_VT* vt))(void));
/// @brief Ensure backend is valid
$attr($inline_always)
$static fn_((dage_Backend_ensureValid(dage_Backend self))(dage_Backend));

/// @brief Create a render target
/// @note  Error boundary - may fail
$attr($must_check)
$extern fn_((dage_Backend_createTarget(dage_Backend self, dage_Target_Cfg cfg))(E$dage_TargetId));
/// @brief Destroy a render target
$extern fn_((dage_Backend_destroyTarget(dage_Backend self, dage_TargetId id))(void));
/// @brief Pump events into caller-provided buffer
/// @param self        Backend instance
/// @param out_events  Buffer to fill (capacity = out_events.len)
/// @return            Slice of events written
$extern fn_((dage_Backend_pumpEvents(dage_Backend self, S$dage_RawEvent out_events))(S$dage_RawEvent));
/// @brief Present RGBA pixels to target
$extern fn_((dage_Backend_present(dage_Backend self, dage_TargetId id, S_const$u32 pixels, u32 w, u32 h))(void));
/// @brief Get target metrics
$extern fn_((dage_Backend_getTargetMetrics(dage_Backend self, dage_TargetId id))(dage_TargetMetrics));

/*========== Convenience API (Allocating Variants) ==========*/

/// @brief Result of allocating pumpEvents
typedef struct dage_Backend_PumpedEvents {
    S$dage_RawEvent items; /* Allocated events (caller must free) */
    usize cap; /* Actual capacity of the allocated events */
} dage_Backend_PumpedEvents;
T_use_E$(dage_Backend_PumpedEvents);

/// @brief Pump events with dynamic allocation
/// @param self      Backend instance
/// @param gpa       Allocator for events
/// @param cap_hint  Suggested capacity (0 = default 128)
/// @return          Allocated events + capacity (caller owns memory)
/// @note            Error boundary - allocation may fail
$attr($must_check)
$extern fn_((dage_Backend_pumpEventsAlloc(dage_Backend self, mem_Allocator gpa, usize cap_hint))(E$dage_Backend_PumpedEvents));
/// @brief Free pumped events
$extern fn_((dage_Backend_PumpedEvents_free(dage_Backend_PumpedEvents* self, mem_Allocator gpa))(void));

/*========== Macros and Definitions =========================================*/

fn_((dage_Backend_isValid(dage_Backend self))(bool)) {
    return self.ptr != null
        && self.vt != null
        && self.vt->createTarget != null
        && self.vt->destroyTarget != null
        && self.vt->pumpEvents != null
        && self.vt->present != null
        && self.vt->getTargetMetrics != null;
};

fn_((dage_Backend_assertValid(dage_Backend_Impl* ptr, const dage_Backend_VT* vt))(void)) {
    claim_assert_nonnull(ptr);
    claim_assert_nonnull(vt);
    claim_assert_nonnull(vt->createTarget);
    claim_assert_nonnull(vt->destroyTarget);
    claim_assert_nonnull(vt->pumpEvents);
    claim_assert_nonnull(vt->present);
    claim_assert_nonnull(vt->getTargetMetrics);
};

fn_((dage_Backend_ensureValid(dage_Backend self))(dage_Backend)) {
    return dage_Backend_assertValid(self.ptr, self.vt), self;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_Backend__included */
