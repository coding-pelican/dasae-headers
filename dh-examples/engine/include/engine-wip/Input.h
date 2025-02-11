#ifndef ENGINE_INPUT_INCLUDED
#define ENGINE_INPUT_INCLUDED (1)

#include "engine-wip/common.h"
#include "engine-wip/KeyButtonStates.h"

#include "engine-wip/KeyCode.h"
#include "engine-wip/Keyboard.h"

#include "engine-wip/MouseButton.h"
#include "engine-wip/Mouse.h"

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

enum {
    engine_InputEventTag_none = -1,

    engine_InputEventTag_key_press = 0,
    engine_InputEventTag_key_hold,
    engine_InputEventTag_key_release,
    engine_InputEventTag_mouse_press,
    engine_InputEventTag_mouse_hold,
    engine_InputEventTag_mouse_release,
    engine_InputEventTag_mouse_motion,
    engine_InputEventTag_mouse_scroll,

    engine_InputEventTag_count
};
typedef i8 engine_InputEventTag;

typedef struct engine_InputEvent {
    engine_InputEventTag tag; /**< The type of event */
    union {
        engine_InputEvent_KeyboardKey key_press;
        engine_InputEvent_KeyboardKey key_hold;
        engine_InputEvent_KeyboardKey key_release;
        engine_InputEvent_MouseButton mouse_press;
        engine_InputEvent_MouseButton mouse_hold;
        engine_InputEvent_MouseButton mouse_release;
        engine_InputEvent_MouseMotion mouse_motion;
        engine_InputEvent_MouseScroll mouse_scroll;
    };
} engine_InputEvent;
use_Sli$(engine_InputEvent);
use_Opt$(engine_InputEvent);
use_Err$(engine_InputEvent);

/// Buffer for input events between updates
typedef struct engine_InputEventBuffer engine_InputEventBuffer;
#define engine_InputEventBuffer_size /* Circular buffer for input event stack */ (32)
struct engine_InputEventBuffer {
    engine_InputEvent events[engine_InputEventBuffer_size];
    i32               head;
    i32               tail;
    i32               count;
};

/// Input system structure
typedef struct engine_Input {
    engine_InputEventBuffer event_buffer;
    engine_Keyboard         keyboard;
    engine_Mouse            mouse;
    mem_Allocator           allocator;
} engine_Input;
use_Sli$(engine_Input);
use_Opt$(engine_Input);
use_Err$(engine_Input);

/// Core functions
extern Err$Ptr$engine_Input engine_Input_init(mem_Allocator allocator) must_check;
extern void                 engine_Input_fini(engine_Input* self);
extern void                 engine_Input_update(engine_Input* self);

/// Management event buffer
extern void                  engine_InputEventBuffer_push(engine_Input* self, engine_InputEvent event);
extern Opt$engine_InputEvent engine_InputEventBuffer_pop(engine_Input* self);
extern Opt$engine_InputEvent engine_InputEventBuffer_peek(engine_Input* self);
extern void                  engine_InputEventBuffer_clear(engine_Input* self);

#endif /* ENGINE_INPUT_INCLUDED */
