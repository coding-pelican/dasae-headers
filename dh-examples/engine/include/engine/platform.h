#ifndef ENGINE_PLATFORM_INCLUDED
#define ENGINE_PLATFORM_INCLUDED (1)

#include "common.h"
#include "color.h"

// engine_Platform-specific rendering backend types
typedef enum engine_RenderBackendType {
    engine_RenderBackendType_null  = 0,
    engine_RenderBackendType_vt100 = 0,
    engine_RenderBackendType_win32_gdi,
    engine_RenderBackendType_xwindow,
    engine_RenderBackendType_directx,
    engine_RenderBackendType_opengl,
    engine_RenderBackendType_vulkan,
    engine_RenderBackendType_custom
} engine_RenderBackendType;

// engine_Platform-specific initialization parameters
typedef struct engine_PlatformParams {
    engine_RenderBackendType backend_type;
    anyptr                   native_window_handle; // Optional, platform-specific
    const char*              window_title;
    u32                      width;
    u32                      height;
    Color                    default_color; // if alpha is not 0xFF, it will be used default color
    anyptr                   custom_data;   // Optional, platform-specific
} engine_PlatformParams;
use_Ptr$(engine_PlatformParams);
use_Err$(engine_PlatformParams);

// engine_Platform-specific error types
use_Err(
    engine_PlatformErr,
    AccessDenied
);

// engine_Platform-specific rendering backend interface
typedef struct engine_RenderBackend engine_RenderBackend;

// engine_Platform interface
typedef struct engine_Platform {
    engine_RenderBackend* backend;
} engine_Platform;
use_Ptr$(engine_Platform);
use_Err$(engine_Platform);

// Create platform instance with specific backend
extern Err$Ptr$engine_Platform engine_Platform_create(const engine_PlatformParams* params) must_check;
extern void                    engine_Platform_destroy(engine_Platform* platform);

#endif /* ENGINE_PLATFORM_INCLUDED */
