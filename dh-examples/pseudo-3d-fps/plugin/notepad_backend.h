#ifndef ENGINE_PLUGIN_NOTEPAD_BACKEND
#define ENGINE_PLUGIN_NOTEPAD_BACKEND (1)

#include "engine/platform_backend.h"

typedef struct engine_NotepadBackend {
    engine_RenderBackend base;
    HWND                 notepad_handle;
    wchar*               buffer;
    usize                buffer_size;
    usize                buffer_capacity;
} engine_NotepadBackend;

extern Err$Ptr$engine_RenderBackend NotepadBackend_create(void) must_check;

#endif /* ENGINE_PLUGIN_NOTEPAD_BACKEND */
