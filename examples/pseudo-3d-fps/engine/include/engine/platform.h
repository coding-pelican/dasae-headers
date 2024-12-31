#ifndef ENGINE_PLATFORM_INCLUDED
#define ENGINE_PLATFORM_INCLUDED (1)

#include "common.h"
#include "color.h"

// engine_Platform-specific rendering backend types
typedef enum engine_RenderBackendType {
    engine_RenderBackendType_vt100,
    engine_RenderBackendType_win32_gdi,
    engine_RenderBackendType_xwindow,
    engine_RenderBackendType_directx,
    engine_RenderBackendType_opengl,
    engine_RenderBackendType_vulkan
} engine_RenderBackendType;

// engine_Platform-specific initialization parameters
typedef struct engine_PlatformParams {
    engine_RenderBackendType backend_type;
    anyptr                   native_window_handle; // Optional, platform-specific
    const char*              window_title;
    u32                      width;
    u32                      height;
} engine_PlatformParams;
using_Ptr$(engine_PlatformParams);
using_Err$(engine_PlatformParams);

// Forward declarations
typedef struct engine_Platform      engine_Platform;
typedef struct engine_RenderBackend engine_RenderBackend;

// engine_Platform interface
struct engine_Platform {
    engine_RenderBackend* backend;
    void (*destroy)(engine_Platform* platform);
    void (*process_events)(engine_Platform* platform);
    void (*present_buffer)(engine_Platform* platform, const Color* buffer, u32 width, u32 height);
};
using_Ptr$(engine_Platform);
using_Err$(engine_Platform);

// Create platform instance with specific backend
extern Err$Ptr$engine_Platform engine_Platform_create(const engine_PlatformParams* params) must_check;
extern void                    engine_Platform_destroy(engine_Platform* platform);

#endif // ENGINE_PLATFORM_INCLUDED
