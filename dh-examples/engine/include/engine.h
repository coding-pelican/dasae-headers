#ifndef engine__included
#define engine__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "engine/common.h"
#include "engine/utils.h"

#include "engine/Color.h"
#include "engine/Canvas.h"
#include "engine/UnicodeCanvas.h"

#include "engine/Backend.h"
#include "engine/Window.h"

#include "engine/KeyButtonStates.h"
#include "engine/KeyCode.h"
#include "engine/Keyboard.h"
#include "engine/MouseButton.h"
#include "engine/Mouse.h"

#include "engine/Input.h"

#include "engine/core/VT100.h"
#define engine_core_VT100Ex_comp_enabled 1
#if engine_core_VT100Ex_comp_enabled
#include "engine/core/VT100Ex.h"
#endif /* engine_core_VT100Ex_comp_enabled */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* engine__included */
