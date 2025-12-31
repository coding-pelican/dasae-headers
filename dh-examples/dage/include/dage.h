/**
 * @file    dage.h
 * @brief   dage - dasae abstract game engine (dage) / main include file
 * @details Include this single header to access all dage functionality.
 *
 * ## Quick Start
 * ```c
 * #include "dage.h"
 *
 * fn_((main(S$S_const$u8 args))(E$void) $guard) {
 *     let_ignore = args;
 *     // 1. Create backend (choose your implementation)
 *     var wsi = try_(dage_core_WSI_init(...));
 *     defer_(dage_core_WSI_fini(&wsi));
 *     let backend = dage_core_WSI_backend(wsi);
 *
 *     // 2. Create runtime with backend
 *     let runtime = try_(dage_Runtime_init((dage_Runtime_Cfg){
 *         .gpa = gpa,
 *         .backend = backend,
 *     }));
 *     defer_(dage_Runtime_fini(&runtime));
 *
 *     // 3. Create window
 *     let win_id = try_(dage_Runtime_createWindow(&runtime, ...));
 *
 *     // 4. Main loop
 *     while (!dage_Runtime_shouldQuit(&runtime)) {
 *         dage_Runtime_processEvents(&runtime);
 *
 *         // Your game logic here
 *         // - Query input: dage_Window_isKeyPressed(win, KEY_SPACE)
 *         // - Poll events: while_some((dage_Window_pollEvent(win))(e))
 *
 *         dage_Runtime_presentAll(&runtime);
 *         dage_Runtime_endFrame(&runtime);
 *     }
 *
 *     // 5. Cleanup
 *     return_ok({});
 * } $unguarded_(fn);
 * ```
 */
#ifndef dage__included
#define dage__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/* Common types and utilities */
#include "dage/common.h"

/* Event system */
#include "dage/Event.h"

/* Input types */
#include "dage/KeyCode.h"
#include "dage/MouseButton.h"
#include "dage/InputState.h"

/* Core systems */
#include "dage/Backend.h"
#include "dage/Canvas.h"
#include "dage/Viewport.h"
#include "dage/Window.h"
#include "dage/Runtime.h"

/* Backend implementations */
#if UNUSED_CODE
#include "dage/core/VT100.h"
#endif /* UNUSED_CODE */
#include "dage/core/WSI.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage__included */
