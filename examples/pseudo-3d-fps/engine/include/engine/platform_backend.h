// platform_backend.h (Include this before platform.h)
#ifndef ENGINE_PLATFORM_BACKEND_H
#define ENGINE_PLATFORM_BACKEND_H

#include "platform.h"
#include "common.h"

#include <windows.h>

// Complete render backend interface
struct engine_RenderBackend {
    engine_RenderBackendType type;
    void (*presentBuffer)(struct engine_Platform* platform, const anyptr buffer, u32 width, u32 height);
    void (*processEvents)(struct engine_Platform* platform);
    void (*processInput)(void);
    void (*destroy)(struct engine_Platform* platform);
};

// Win32 console backend
typedef struct engine_Win32ConsoleBackend {
    engine_RenderBackend base;
    HANDLE               console_handle;
    char*                buffer;
    usize                buffer_size;
    usize                buffer_capacity;
    bool                 cursor_visible;
} engine_Win32ConsoleBackend;

#endif // ENGINE_PLATFORM_BACKEND_H
