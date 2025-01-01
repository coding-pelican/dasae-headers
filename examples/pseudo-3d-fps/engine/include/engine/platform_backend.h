#ifndef ENGINE_PLATFORM_BACKEND_INCLUDED
#define ENGINE_PLATFORM_BACKEND_INCLUDED (1)

#include "platform.h"
#include "common.h"

// Complete render backend interface
struct engine_RenderBackend {
    engine_RenderBackendType type;
    void (*destroy)(struct engine_Platform* platform);
    void (*processEvents)(struct engine_Platform* platform);
    void (*presentBuffer)(struct engine_Platform* platform, const Color* buffer, u32 width, u32 height);
};
using_Ptr$(engine_RenderBackend);
using_Err$(engine_RenderBackend);

#include <windows.h>

// Win32 console backend
typedef struct engine_Win32ConsoleBackend {
    engine_RenderBackend base;
    HANDLE               console_handle;
    char*                buffer;
    usize                buffer_size;
    usize                buffer_capacity;
    bool                 cursor_visible;
} engine_Win32ConsoleBackend;

#endif // ENGINE_PLATFORM_BACKEND_INCLUDED
