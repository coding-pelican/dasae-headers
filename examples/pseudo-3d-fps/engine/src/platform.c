#include "../include/engine/platform.h"
#include "../include/engine/platform_backend.h"
#include "../include/engine/canvas.h"
#include "../include/engine/input.h"

#define Win32ConsoleBackend_calculateBufferSize(width, height) ((width) * (height) * 32)

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

    /* Reset cursor position */ {
        static const char* const reset_cursor = "\033[H";
        printf("%s", reset_cursor);
        // memcpy(backend->buffer + backend->buffer_size, reset_cursor, strlen(reset_cursor));
        // backend->buffer_size += strlen(reset_cursor);
    }
}

static void Win32ConsoleBackend_processEvents(engine_Platform* platform) {
    unused(platform);
    // engine_Win32ConsoleBackend* backend      = (engine_Win32ConsoleBackend*)platform->backend;
    // INPUT_RECORD                input_record = { 0 };
    // DWORD                       events_read  = 0;

    // while (PeekConsoleInput(backend->console_handle, &input_record, 1, &events_read) && events_read > 0) {
    //     ReadConsoleInput(backend->console_handle, &input_record, 1, &events_read);

    //     if (input_record.EventType == WINDOW_BUFFER_SIZE_EVENT) {
    //         // Simply acknowledge window resize - buffer will adapt automatically
    //         continue;
    //     }
    // }
}

static void Win32ConsoleBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height) {
    debug_assert_nonnull(platform);
    debug_assert_nonnull(data);
    let backend = (engine_Win32ConsoleBackend*)platform->backend;
    let pixels  = data;

    /* Reset buffer */ {
        backend->buffer_size = 0;
    }

    for (u32 y = 0; (y + 1) < height; y += 2) {
        for (u32 x = 0; x < width; ++x) {
            // Get upper and lower pixels
            const Color upper = pixels[y * width + x];
            const Color lower = pixels[(y + 1) * width + x];

            // clang-format off
            // Find run length of identical color pairs
            usize run_length = 1;
            while ((x + run_length) < width) {
                const Color next_upper = pixels[y * width + x + run_length];
                const Color next_lower = pixels[(y + 1) * width + x + run_length];

                if (memcmp(&upper.channels, &next_upper.channels, sizeof(Color)) != 0
                 || memcmp(&lower.channels, &next_lower.channels, sizeof(Color)) != 0) {
                    break;
                }
                run_length++;
            }

            // Write color sequence
            backend->buffer_size += sprintf(
                backend->buffer + backend->buffer_size,
                "\033[38;2;%d;%d;%d;48;2;%d;%d;%dm",
                upper.r, upper.g, upper.b,
                lower.r, lower.g, lower.b
            );
            // clang-format on

            // Write half-blocks for the run
            for (usize i = 0; i < run_length; ++i) {
                memcpy(backend->buffer + backend->buffer_size, "▀", strlen("▀"));
                backend->buffer_size += strlen("▀");
            }

            // NOTE: 으아 ㅠㅠㅠ 드디어 고쳐냈다 ㅠㅠ 길이를 1개 감소시키지 않았다고 화면이 와장창 깨지다니...
            x += run_length - 1; // -1 because loop will increment
        }
        backend->buffer[backend->buffer_size++] = '\n';
    }
    backend->buffer[--backend->buffer_size] = '\0';

    // /* Reset colors */ {
    //     const char* reset_colors = "\033[0m";
    //     memcpy(backend->buffer + backend->buffer_size, reset_colors, strlen(reset_colors));
    //     backend->buffer_size += strlen(reset_colors);
    // }

    // /* Clear console */ {
    //     HANDLE                     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //     COORD                      topLeft  = { 0, 0 };
    //     CONSOLE_SCREEN_BUFFER_INFO screen   = { 0 };
    //     DWORD                      written  = { 0 };

    //     GetConsoleScreenBufferInfo(hConsole, &screen);
    //     FillConsoleOutputCharacter(hConsole, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    //     FillConsoleOutputAttribute(hConsole, 0, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    //     // FillConsoleOutputAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    //     SetConsoleCursorPosition(hConsole, topLeft);
    // }

    /* Reset cursor position */ {
        static const char* const reset_cursor = "\033[H";
        printf("%s", reset_cursor);
        // memcpy(backend->buffer + backend->buffer_size, reset_cursor, strlen(reset_cursor));
        // backend->buffer_size += strlen(reset_cursor);
    }

    /* Write to console */ {
        DWORD written = 0;
        WriteConsoleA(
            backend->console_handle,
            backend->buffer,
            (DWORD)backend->buffer_size,
            &written,
            null
        );
    }
}

/* static void Win32ConsoleBackend_presentBuffer(engine_Platform* platform, const anyptr data, u32 width, u32 height) {
    engine_Win32ConsoleBackend* backend = (engine_Win32ConsoleBackend*)platform->backend;
    const engine_ColorRgba*     pixels  = (const engine_ColorRgba*)data;

    backend->buffer_size = 0;

    // Process two rows at a time using block characters
    for (usize y = 0; (y + 1) < height; y += 2) {
        for (usize x = 0; x < width; ++x) {
            const engine_ColorRgba upper = pixels[x + (y * width)];
            const engine_ColorRgba lower = pixels[x + ((y + 1) * width)];

            // Find run length of identical color pairs
            usize run_length = 1;
            while ((x + run_length) < width) {
                const engine_ColorRgba next_upper = pixels[(x + run_length) + (y * width)];
                const engine_ColorRgba next_lower = pixels[(x + run_length) + ((y + 1) * width)];

                // clang-format off
                if (memcmp(&upper, &next_upper, sizeof(engine_ColorRgba)) != 0 ||
                    memcmp(&lower, &next_lower, sizeof(engine_ColorRgba)) != 0) {
                    break;
                }
                run_length++;
            }

            // Write ANSI color sequence
            backend->buffer_size += sprintf(
                backend->buffer + backend->buffer_size,
                "\033[38;2;%d;%d;%d;48;2;%d;%d;%d;m",
                upper.r, upper.g, upper.b,
                lower.r, lower.g, lower.b
            );
            // clang-format on

            // Append the block character '▀' runLength times
            static const char* const block_char        = "▀"; // Multibyte character
            static const usize       block_char_length = strlen(block_char);

            // Write block characters for the run
            for (usize i = 0; i < run_length; ++i) {
                memcpy(backend->buffer + backend->buffer_size, block_char, block_char_length);
                backend->buffer_size += block_char_length;
            }
            x += run_length - 1;
        }
        backend->buffer[backend->buffer_size++] = '\n';
    }

    printf("\033[H"); // Reset cursor position
    WriteConsoleA(backend->console_handle, backend->buffer, (DWORD)backend->buffer_size, NULL, NULL);
} */

Err$Ptr$engine_Platform engine_Platform_create(const engine_PlatformParams* params) {
    reserveReturn(Err$Ptr$engine_Platform);
    engine_Platform* const platform = (engine_Platform*)malloc(sizeof(engine_Platform));
    if (!platform) {
        return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
    }

    switch (params->backend_type) {
    case engine_RenderBackendType_vt100: {
        engine_Win32ConsoleBackend* const backend = (engine_Win32ConsoleBackend*)malloc(sizeof(engine_Win32ConsoleBackend));
        if (!backend) {
            free(platform);
            return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
        }

        // Initialize console backend
        // HANDLE hConsole = CreateConsoleScreenBuffer(
        //     GENERIC_READ | GENERIC_WRITE,
        //     FILE_SHARE_READ | FILE_SHARE_WRITE,
        //     null,
        //     CONSOLE_TEXTMODE_BUFFER,
        //     null
        // );
        // if (hConsole == INVALID_HANDLE_VALUE) {
        //     free(backend);
        //     free(platform);
        //     return_err(engine_PlatformErr_err(engine_PlatformErrType_AccessDenied));
        // }
        HANDLE     hConsole          = GetStdHandle(STD_OUTPUT_HANDLE);
        SMALL_RECT windowSizeInitial = (SMALL_RECT){ 0, 0, 1, 1 };
        SetConsoleWindowInfo(hConsole, TRUE, &windowSizeInitial);

        // void SetConsoleScreenBuffer()
        COORD dwSize = (COORD){ (SHORT)params->width, (SHORT)params->height };
        SetConsoleScreenBufferSize(hConsole, dwSize);

        SMALL_RECT windowSize = (SMALL_RECT){ 0, 0, (SHORT)(params->width), (SHORT)(params->height) };
        SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

        // Configure console
        SetConsoleOutputCP(CP_UTF8);
        DWORD mode = 0;
        GetConsoleMode(hConsole, &mode);
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
        // mode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_WINDOW_INPUT;
        SetConsoleMode(hConsole, mode);

        // Hide cursor
        CONSOLE_CURSOR_INFO cursor_info = { 1, false };
        SetConsoleCursorInfo(hConsole, &cursor_info);

        // Allocate string buffer for ANSI sequences
        backend->buffer_capacity = (usize)Win32ConsoleBackend_calculateBufferSize((usize)params->width, (usize)params->height);
        backend->buffer          = malloc(backend->buffer_capacity);
        if (!backend->buffer) {
            // CloseHandle(hConsole);
            free(backend);
            free(platform);
            return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
        }

        backend->console_handle = hConsole;
        backend->buffer_size    = 0;
        backend->cursor_visible = false;

        // Set active screen buffer
        SetConsoleActiveScreenBuffer(hConsole);

        // Setup backend interface
        backend->base.type          = engine_RenderBackendType_vt100;
        backend->base.destroy       = Win32ConsoleBackend_destroy;
        backend->base.processEvents = Win32ConsoleBackend_processEvents;
        backend->base.presentBuffer = Win32ConsoleBackend_presentBuffer;

        platform->backend = &backend->base;
        return_ok(platform);
    }

    case engine_RenderBackendType_win32_gdi:
    case engine_RenderBackendType_directx:
        free(platform);
        return_err(engine_PlatformErr_err(engine_PlatformErrType_NotImplemented));

    case engine_RenderBackendType_custom:
        if (!params->custom_data) {
            free(platform);
            return_err(engine_PlatformErr_err(engine_PlatformErrType_InvalidArgument));
        }
        platform->backend = params->custom_data;
        return_ok(platform);

    default:
        free(platform);
        return_err(engine_PlatformErr_err(engine_PlatformErrType_InvalidArgument));
    }
}

void engine_Platform_destroy(engine_Platform* platform) {
    if (!platform) { return; }
    if (platform->backend && platform->backend->destroy) {
        platform->backend->destroy(platform);
    }
}
