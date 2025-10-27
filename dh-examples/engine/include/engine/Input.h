#ifndef ENGINE_INPUT_INCLUDED
#define ENGINE_INPUT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "engine/common.h"
#include "engine/KeyButtonStates.h"

#include "engine/KeyCode.h"
#include "engine/Keyboard.h"

#include "engine/MouseButton.h"
#include "engine/Mouse.h"

#include "engine/Backend.h"

typedef union engine_InputKeyModes {
    struct {
        u8 shift     : 1;
        u8 control   : 1;
        u8 alt       : 1;
        u8 caps_lock : 1;
        u8 num_lock  : 1;
        u8 padding_  : 3;
    };
    u8 packed;
} engine_InputKeyModes;

typedef struct engine_InputEvent_KeyboardKey {
    engine_KeyCode       key;   /**< The key that was pressed/released/held */
    engine_InputKeyModes modes; /**< The modifiers that were enabled */
} engine_InputEvent_KeyboardKey;

typedef struct engine_InputEvent_MouseButton {
    Vec2i                pos;
    engine_MouseButton   button; /**< The mouse button that was pressed/released/held */
    engine_InputKeyModes modes;  /**< The modifiers that were enabled */
} engine_InputEvent_MouseButton;

typedef struct engine_InputEvent_MouseMotion {
    Vec2i pos; /**< The new position of the mouse cursor */
} engine_InputEvent_MouseMotion;

typedef struct engine_InputEvent_MouseScroll {
    Vec2f delta; /**< The amount the mouse wheel was scrolled */
} engine_InputEvent_MouseScroll;

typedef variant_(
    (engine_InputEvent),
    (engine_InputEvent_key_press, engine_InputEvent_KeyboardKey),
    (engine_InputEvent_key_hold, engine_InputEvent_KeyboardKey),
    (engine_InputEvent_key_release, engine_InputEvent_KeyboardKey),
    (engine_InputEvent_mouse_press, engine_InputEvent_MouseButton),
    (engine_InputEvent_mouse_hold, engine_InputEvent_MouseButton),
    (engine_InputEvent_mouse_release, engine_InputEvent_MouseButton),
    (engine_InputEvent_mouse_motion, engine_InputEvent_MouseMotion),
    (engine_InputEvent_mouse_scroll, engine_InputEvent_MouseScroll)
) engine_InputEvent;
use_P$(engine_InputEvent);
use_S$(engine_InputEvent);
use_O$(engine_InputEvent);
use_E$(engine_InputEvent);

/// Buffer for input events between updates
typedef struct engine_InputEventBuffer engine_InputEventBuffer;
#define engine_InputEventBuffer_size /* Circular buffer for input event stack */ (32)
struct engine_InputEventBuffer {
    var_(events, A$$(engine_InputEventBuffer_size, engine_InputEvent));
    var_(head, i32);
    var_(tail, i32);
    var_(count, i32);
};

/// Input system structure
typedef struct engine_Input {
    var_(event_buffer, engine_InputEventBuffer);
    var_(keyboard, engine_Keyboard) $like_ptr;
    var_(mouse, engine_Mouse) $like_ptr;
    var_(backend, O$engine_Backend);
    var_(allocator, mem_Allocator);
} engine_Input;
use_P$(engine_Input);
use_S$(engine_Input);
use_O$(engine_Input);
use_E$(engine_Input);

/* Input management ==========================================================*/

extern fn_((engine_Input_init(mem_Allocator allocator))(E$P$engine_Input)) $must_check;
extern fn_((engine_Input_fini(engine_Input* self))(void));
extern fn_((engine_Input_update(engine_Input* self))(E$void)) $must_check;

/* Event buffer management ===================================================*/

extern fn_((engine_InputEventBuffer_push(engine_Input* self, engine_InputEvent event))(void));
extern fn_((engine_InputEventBuffer_pop(engine_Input* self))(O$engine_InputEvent));
extern fn_((engine_InputEventBuffer_peek(engine_Input* self))(O$engine_InputEvent));
extern fn_((engine_InputEventBuffer_clear(engine_Input* self))(void));


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_INPUT_INCLUDED */
