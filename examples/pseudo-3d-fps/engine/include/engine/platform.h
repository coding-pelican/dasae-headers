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

/* typedef union engine_ColorRgb {
    u8 channels[3]; // Rgb channel components in [0,255]
    struct {
        u8 r; // Red channel in [0,255]
        u8 g; // Green channel in [0,255]
        u8 b; // Blue channel in [0,255]
    };
} engine_ColorRgb;

typedef union engine_ColorRgba {
    u8 channels[4]; // RGBA channel components in [0,255]
    struct {
        union {
            engine_ColorRgb rgb; // RGB channel components in [0,255]
            struct {
                u8 r; // Red channel in [0,255]
                u8 g; // Green channel in [0,255]
                u8 b; // Blue channel in [0,255]
            };
        };
        u8 a; // Alpha channel in [0,255]
    };
    u32 packed;
} engine_ColorRgba;

typedef struct engine_ColorIndexed {
    u8 ascii_char;
    u8 index; // Index in color palette
} engine_ColorIndexed;

// Abstract color type that can be interpreted differently by different backends
typedef union engine_ColorValue {
    engine_ColorRgba    rgba;
    engine_ColorIndexed indexed;
} engine_ColorValue; */

// engine_Platform-specific initialization parameters
typedef struct engine_PlatformParams {
    engine_RenderBackendType backend_type;
    anyptr                   native_window_handle; // Optional, platform-specific
    const char*              window_title;
    u32                      width;
    u32                      height;
} engine_PlatformParams;
using_Ptr$(engine_PlatformParams);
using_Err$(Ptr$engine_PlatformParams);

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
using_Err$(Ptr$engine_Platform);

// Create platform instance with specific backend
extern Err$Ptr$engine_Platform engine_Platform_create(const engine_PlatformParams* params) must_check;
extern void                    engine_Platform_destroy(engine_Platform* platform);

#endif // ENGINE_PLATFORM_INCLUDED
