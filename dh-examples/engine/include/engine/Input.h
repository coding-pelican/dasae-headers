#ifndef engine_Input__included
#define engine_Input__included 1
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

#include "dh/ArrQue.h"

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
    engine_KeyCode key;         /**< The key that was pressed/released/held */
    engine_InputKeyModes modes; /**< The modifiers that were enabled */
} engine_InputEvent_KeyboardKey;

typedef struct engine_InputEvent_MouseButton {
    m_V2i32 pos;
    engine_MouseButton button;  /**< The mouse button that was pressed/released/held */
    engine_InputKeyModes modes; /**< The modifiers that were enabled */
} engine_InputEvent_MouseButton;

typedef struct engine_InputEvent_MouseMotion {
    m_V2i32 pos; /**< The new position of the mouse cursor */
} engine_InputEvent_MouseMotion;

typedef struct engine_InputEvent_MouseScroll {
    m_V2f32 delta; /**< The amount the mouse wheel was scrolled */
} engine_InputEvent_MouseScroll;

typedef variant_((engine_InputEvent)(
    (engine_InputEvent_key_press, engine_InputEvent_KeyboardKey),
    (engine_InputEvent_key_hold, engine_InputEvent_KeyboardKey),
    (engine_InputEvent_key_release, engine_InputEvent_KeyboardKey),
    (engine_InputEvent_mouse_press, engine_InputEvent_MouseButton),
    (engine_InputEvent_mouse_hold, engine_InputEvent_MouseButton),
    (engine_InputEvent_mouse_release, engine_InputEvent_MouseButton),
    (engine_InputEvent_mouse_motion, engine_InputEvent_MouseMotion),
    (engine_InputEvent_mouse_scroll, engine_InputEvent_MouseScroll)
)) engine_InputEvent;
prl_T_use$(engine_InputEvent);

T_use_ArrQue$(engine_InputEvent);
#define engine_Input_event_queue_len /* Circular buffer for input event stack */ (32)
/// Input system structure
typedef struct engine_Input {
    var_(event_queue, ArrQue$engine_InputEvent) $like_ref; // Circular buffer for input events between updates
    var_(keyboard, engine_Keyboard) $like_ref;
    var_(mouse, engine_Mouse) $like_ref;
    var_(backend, O$engine_Backend);
    var_(gpa, mem_Allocator);
} engine_Input;
T_use_P$(engine_Input);
T_use_S$(engine_Input);
T_use_O$(engine_Input);
T_use_E$(P$engine_Input);

/* Input management ==========================================================*/

extern fn_((engine_Input_init(mem_Allocator gpa))(E$P$engine_Input)) $must_check;
extern fn_((engine_Input_fini(engine_Input* self))(void));
extern fn_((engine_Input_update(engine_Input* self))(E$void)) $must_check;

/* Event buffer management ===================================================*/

extern fn_((engine_Input_clearEvent(engine_Input* self))(void));
extern fn_((engine_Input_enqueEvent(engine_Input* self, engine_InputEvent event))(void));
extern fn_((engine_Input_peekEvent(engine_Input* self))(O$engine_InputEvent));
extern fn_((engine_Input_dequeEvent(engine_Input* self))(O$engine_InputEvent));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* engine_Input__included */
