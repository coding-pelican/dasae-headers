#include "../include/engine/platform.h"
#include "../include/engine/platform_backend.h"
#include "../include/engine/canvas.h"
#include "../include/engine/input.h"

#define DISPLAY_BUFFER_SIZE(width, height) ((width) * (height) * 32)

static void Win32ConsoleBackend_presentBuffer(engine_Platform* platform, const anyptr data, u32 width, u32 height) {
    engine_Win32ConsoleBackend* backend = (engine_Win32ConsoleBackend*)platform->backend;
    const engine_ColorRgba*     pixels  = (const engine_ColorRgba*)data;

    // Reset buffer
    backend->buffer_size = 0;

    // Reset cursor position
    const char* reset_cursor = "\033[H";
    memcpy(backend->buffer + backend->buffer_size, reset_cursor, strlen(reset_cursor));
    backend->buffer_size += strlen(reset_cursor);

    for (u32 y = 0; y < height; y += 2) {
        for (u32 x = 0; x < width; ++x) {
            // Get upper and lower pixels
            const engine_ColorRgba upper = pixels[y * width + x];
            const engine_ColorRgba lower = (y + 1 < height) ? pixels[(y + 1) * width + x] : (engine_ColorRgba){ 0 };

            // Find run length of identical color pairs
            usize run_length = 1;
            while ((x + run_length) < width) {
                const engine_ColorRgba next_upper = pixels[y * width + x + run_length];
                const engine_ColorRgba next_lower = (y + 1 < height) ? pixels[(y + 1) * width + x + run_length] : (engine_ColorRgba){ 0 };

                if (memcmp(&upper, &next_upper, sizeof(engine_ColorRgba)) != 0 || memcmp(&lower, &next_lower, sizeof(engine_ColorRgba)) != 0) {
                    break;
                }
                run_length++;
            }

            // Write color sequence
            backend->buffer_size += sprintf(
                backend->buffer + backend->buffer_size,
                "\033[38;2;%d;%d;%d;48;2;%d;%d;%dm",
                upper.r,
                upper.g,
                upper.b,
                lower.r,
                lower.g,
                lower.b
            );

            // Write half-blocks for the run
            for (usize i = 0; i < run_length; i++) {
                memcpy(backend->buffer + backend->buffer_size, "▀", strlen("▀"));
                backend->buffer_size += strlen("▀");
            }

            x += run_length - 1; // -1 because loop will increment
        }
        backend->buffer[backend->buffer_size++] = '\n';
    }

    // Reset colors
    const char* reset_colors = "\033[0m";
    memcpy(backend->buffer + backend->buffer_size, reset_colors, strlen(reset_colors));
    backend->buffer_size += strlen(reset_colors);

    // Write to console
    DWORD written = 0;
    WriteConsoleA(
        backend->console_handle,
        backend->buffer,
        (DWORD)backend->buffer_size,
        &written,
        null
    );
}

static void Win32ConsoleBackend_destroy(engine_Platform* platform) {
    engine_Win32ConsoleBackend* backend = (engine_Win32ConsoleBackend*)platform->backend;
    if (backend) {
        if (backend->buffer) {
            free(backend->buffer);
        }
        // Restore console state
        if (!backend->cursor_visible) {
            CONSOLE_CURSOR_INFO cursor_info = { 1, TRUE };
            SetConsoleCursorInfo(backend->console_handle, &cursor_info);
        }
        free(backend);
    }
    free(platform);
}

static void Win32ConsoleBackend_processEvents(engine_Platform* platform) {
    engine_Win32ConsoleBackend* backend      = (engine_Win32ConsoleBackend*)platform->backend;
    INPUT_RECORD                input_record = { 0 };
    DWORD                       events_read  = 0;

    while (PeekConsoleInput(backend->console_handle, &input_record, 1, &events_read) && events_read > 0) {
        ReadConsoleInput(backend->console_handle, &input_record, 1, &events_read);

        if (input_record.EventType == WINDOW_BUFFER_SIZE_EVENT) {
            // Simply acknowledge window resize - buffer will adapt automatically
            continue;
        }
    }

    // Update input state
    engine_Input_update();
}

ResErr_Ptr_engine_Platform engine_Platform_create(PtrConst_engine_PlatformParams params) {
    engine_Platform* const platform = (engine_Platform*)malloc(sizeof(engine_Platform));
    if (!platform) {
        return ResErr_Ptr_engine_Platform_err(Err_OutOfMemory);
    }

    const Ptr_engine_Platform self = Ptr_engine_Platform_from(platform);

    switch (params.addr->backend_type) {
    case engine_RenderBackendType_vt100: {
        engine_Win32ConsoleBackend* const backend = (engine_Win32ConsoleBackend*)malloc(sizeof(engine_Win32ConsoleBackend));
        if (!backend) {
            free(platform);
            return ResErr_Ptr_engine_Platform_err(Err_OutOfMemory);
        }

        // Initialize console backend
        HANDLE hConsole = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            null,
            CONSOLE_TEXTMODE_BUFFER,
            null
        );
        if (hConsole == INVALID_HANDLE_VALUE) {
            free(backend);
            free(platform);
            return ResErr_Ptr_engine_Platform_err(Err_AccessDenied);
        }

        // Configure console
        SetConsoleOutputCP(CP_UTF8);
        DWORD mode = 0;
        GetConsoleMode(hConsole, &mode);
        mode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_WINDOW_INPUT;
        SetConsoleMode(hConsole, mode);

        // Hide cursor
        CONSOLE_CURSOR_INFO cursor_info = { 1, false };
        SetConsoleCursorInfo(hConsole, &cursor_info);

        // Allocate string buffer for ANSI sequences
        backend->buffer_capacity = (usize)DISPLAY_BUFFER_SIZE((usize)params.addr->width, (usize)params.addr->height);
        backend->buffer          = malloc(backend->buffer_capacity);
        if (!backend->buffer) {
            CloseHandle(hConsole);
            free(backend);
            free(platform);
            return ResErr_Ptr_engine_Platform_err(Err_OutOfMemory);
        }

        backend->console_handle = hConsole;
        backend->buffer_size    = 0;
        backend->cursor_visible = false;

        // Set active screen buffer
        SetConsoleActiveScreenBuffer(hConsole);

        // Setup backend interface
        backend->base.type          = engine_RenderBackendType_vt100;
        backend->base.presentBuffer = Win32ConsoleBackend_presentBuffer;
        backend->base.processEvents = Win32ConsoleBackend_processEvents;
        backend->base.destroy       = Win32ConsoleBackend_destroy;

        platform->backend = &backend->base;
        return ResErr_Ptr_engine_Platform_ok(self);
    }

    case engine_RenderBackendType_win32_gdi:
    case engine_RenderBackendType_directx:
        free(platform);
        return ResErr_Ptr_engine_Platform_err(Err_NotImplemented);

    default:
        free(platform);
        return ResErr_Ptr_engine_Platform_err(Err_InvalidArgument);
    }
}

void engine_Platform_destroy(Ptr_engine_Platform platform) {
    if (!platform.addr) { return; }

    if (platform.addr->backend && platform.addr->backend->destroy) {
        platform.addr->backend->destroy(platform.addr);
    }
}
