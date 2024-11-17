#ifndef WINDOW_INCLUDE
#define WINDOW_INCLUDE (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dh/core.h"

// Enums
typedef enum Window_EUpdateState {
    Window_EUpdateState_OK             = 0,
    Window_EUpdateState_EXIT           = -1,
    Window_EUpdateState_INVALID_WINDOW = -2,
    Window_EUpdateState_INVALID_BUFFER = -3,
    Window_EUpdateState_INTERNAL_ERROR = -4,
} Window_EUpdateState;

typedef struct Window {
    anyptr specific;
    anyptr user_data;

    // mfb_active_func       active_func;
    // mfb_resize_func       resize_func;
    // mfb_close_func        close_func;
    // mfb_keyboard_func     keyboard_func;
    // mfb_char_input_func   char_input_func;
    // mfb_mouse_button_func mouse_btn_func;
    // mfb_mouse_move_func   mouse_move_func;
    // mfb_mouse_scroll_func mouse_wheel_func;

    u32 window_width;
    u32 window_height;

    u32 dst_offset_x;
    u32 dst_offset_y;
    u32 dst_width;
    u32 dst_height;
    f32 factor_x;
    f32 factor_y;
    f32 factor_width;
    f32 factor_height;

    anyptr draw_buffer;
    u32    buffer_width;
    u32    buffer_height;
    u32    buffer_stride;

    i32 mouse_pos_x;
    i32 mouse_pos_y;
    f32 mouse_wheel_x;
    f32 mouse_wheel_y;
    u8  mouse_button_status[8];
    u8  key_status[512];
    u32 mod_keys;

    bool is_active;
    bool is_initialized;

    bool close;
} Window;

// Create a window that is used to display the buffer sent into the Window_update function, returns 0 if fails
Window* Window_init(Window* w, const char* title, u32 width, u32 height);
Window* Window_withFlags(Window* w, u32 flags);
// Close the window
Window* Window_fini(Window* window);

// Update the display
// Input buffer is assumed to be a 32-bit buffer of the size given in the open call
// Will return a negative status if something went wrong or the user want to exit
// Also updates the window events
Window_EUpdateState Window_update(Window* window, anyptr buffer);

// Only updates the window events
Window_EUpdateState Window_updateEvents(Window* window);

// Set user data
void   Window_set_user_data(Window* window, anyptr user_data);
anyptr Window_get_user_data(Window* window);

// Set viewport (useful when resize)
bool Window_set_viewport(Window* window, u32 offset_x, u32 offset_y, u32 width, u32 height);
// Let Window to calculate the best fit from your framebuffer original size
bool Window_set_viewport_best_fit(Window* window, u32 old_width, u32 old_height);

// DPI
// [Deprecated]: Probably a better name will be Window_get_monitor_scale
void Window_monitor_dpi(Window* window, f32* dpi_x, f32* dpi_y);
// Use this instead
void Window_monitor_scale(Window* window, f32* scale_x, f32* scale_y);

// Callbacks
// void Window_set_active_callback(Window* window, Window_active_func callback);
// void Window_set_resize_callback(Window* window, Window_resize_func callback);
// void Window_set_close_callback(Window* window, Window_close_func callback);
// void Window_set_keyboard_callback(Window* window, Window_keyboard_func callback);
// void Window_set_char_input_callback(Window* window, Window_char_input_func callback);
// void Window_set_mouse_button_callback(Window* window, Window_mouse_button_func callback);
// void Window_set_mouse_move_callback(Window* window, Window_mouse_move_func callback);
// void Window_set_mouse_scroll_callback(Window* window, Window_mouse_scroll_func callback);

// Getters
// const char* Window_get_key_name(EKey key);

bool      Window_is_active(Window* window);
u32       Window_width(Window* window);
u32       Window_height(Window* window);
int       Window_mouse_x(Window* window);             // Last mouse pos X
int       Window_mouse_y(Window* window);             // Last mouse pos Y
f32       Window_mouse_scroll_x(Window* window);      // Mouse wheel X as a sum. When you call this function it resets.
f32       Window_mouse_scroll_y(Window* window);      // Mouse wheel Y as a sum. When you call this function it resets.
const u8* Window_mouse_button_buffer(Window* window); // One byte for every button. Press (1), Release 0. (up to 8 buttons)
const u8* Window_key_buffer(Window* window);          // One byte for every key. Press (1), Release 0.

// FPS
void Window_set_target_fps(u32 fps);
u32  Window_get_target_gps(void);
bool Window_waitSync(Window* window);

// Timer
Window_timer* Window_timer_create(void);
void          Window_timer_destroy(Window_timer* tmr);
void          Window_timer_reset(Window_timer* tmr);
double        Window_timer_now(Window_timer* tmr);
double        Window_timer_delta(Window_timer* tmr);
double        Window_timer_get_frequency(void);
double        Window_timer_get_resolution(void);


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* COLOR_INCLUDED */
