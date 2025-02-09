// Scheduled to be DEPRECATED
#ifndef ENGINE_PLATFORM_BACKEND_INCLUDED
#define ENGINE_PLATFORM_BACKEND_INCLUDED (1)

#include "platform.h"
#include "common.h"
#include "window.h"

// Complete render backend interface
struct engine_RenderBackend {
    engine_RenderBackendType type;
    void (*destroy)(struct engine_Platform* platform);
    void (*processEvents)(struct engine_Platform* platform);
    void (*presentBuffer)(struct engine_Platform* platform, const Color* buffer, u32 width, u32 height);
    Opt$engine_WindowMetrics (*getWindowMetrics)(struct engine_Platform* platform);
};
use_Ptr$(engine_RenderBackend);
use_Err$(engine_RenderBackend);
extern engine_RenderBackend     engine_RenderBackend_createNoOp(void);
extern void                     engine_RenderBackend_noDestroy(struct engine_Platform* platform);
extern void                     engine_RenderBackend_noProcessEvents(struct engine_Platform* platform);
extern void                     engine_RenderBackend_noPresentBuffer(struct engine_Platform* platform, const Color* buffer, u32 width, u32 height);
extern Opt$engine_WindowMetrics engine_RenderBackend_noGetWindowMetrics(struct engine_Platform* platform);

#endif // ENGINE_PLATFORM_BACKEND_INCLUDED
