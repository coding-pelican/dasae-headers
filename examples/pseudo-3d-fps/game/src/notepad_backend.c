#include <stdio.h>
#include <windows.h>

#include "notepad_backend.h"

/*
NOTE: 테스트해볼 솔루션을 찾았다!!!
    Windows 11에서는 Edit이 아닌 RichEdit 컨트롤을 사용해야한다는 점.
    한번 해보자!

    winapi - SendMessage returns 0 when reading contents from notepad window using Python and Win32 API - Stack Overflow - https://stackoverflow.com/questions/72280096/sendmessage-returns-0-when-reading-contents-from-notepad-window-using-python-and

    Windows 11에서 텍스트를 메모장에 덤프하는 SendMessageByString API가 작동하지 않음-VBForums - https://www.vbforums.com/showthread.php?906994-SendMessageByString-API-to-dump-text-to-Notepad-not-working-in-Windows-11&s=bcd22b6cd77376c846df6ed31c584277

---

    결국 Edit 등을 활용하는 방법으로는 실패하고 허무한 방법으로 해결했다...
*/

static void     NotepadBackend_destroy(engine_Platform* platform);
static void     NotepadBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height);
/**
 * Using precomputed coefficients (R*0.299 + G*0.587 + B*0.114)
 * Multiplied by 256 for fixed-point arithmetic
 */
force_inline u8 NotepadBackend_getGrayscale(const Color* color) {
    return (u8)((color->r * 77 + color->g * 150 + color->b * 29) >> 8);
}

impl_Err(
    NotepadBackendErr,
    OutOfMemoryNotepadBacked,
    OutOfMemoryNotepadBackendBuffer,
    FailedCreateNotepadProcess,
    FailedFindNotepadWindow,
    FailedGetNotepadEditControl,
    CantSendTextToNotepad,
);

Err$Ptr$engine_RenderBackend NotepadBackend_create(void) {
    reserveReturn(Err$Ptr$engine_RenderBackend);

    let backend = (engine_NotepadBackend*)malloc(sizeof(engine_NotepadBackend));
    if (!backend) {
        return_err(NotepadBackendErr_err(NotepadBackendErrType_OutOfMemoryNotepadBacked));
    }
    memset(backend, 0, sizeof(engine_NotepadBackend));

    let capacity = 1024ull * 1024ull;
    let buffer   = (wchar*)malloc(capacity * sizeof(wchar));
    if (!buffer) {
        free(backend);
        return_err(NotepadBackendErr_err(NotepadBackendErrType_OutOfMemoryNotepadBackendBuffer));
    }

    backend->buffer          = buffer;
    backend->buffer_capacity = capacity;
    backend->buffer_size     = 0;

    // Create test message
    const wchar* test_msg = L"Hello, notepad!\n";
    wcscpy(backend->buffer, test_msg);
    backend->buffer_size = wcslen(test_msg);

    // Launch notepad and store handle
    backend->notepad_handle = try(NotepadBackend_launchNotepad());

    // Send initial text
    catch (NotepadBackend_sendTextToNotepad(backend->notepad_handle, backend->buffer), err, {
        free(buffer);
        free(backend);
        return_err(err);
    });

    // Set up backend interface
    backend->base.type          = engine_RenderBackendType_custom;
    backend->base.presentBuffer = NotepadBackend_presentBuffer;
    backend->base.processEvents = NULL;
    backend->base.destroy       = NotepadBackend_destroy;

    return_ok(&backend->base);
}

static void NotepadBackend_destroy(engine_Platform* platform) {
    if (!platform || !platform->backend) {
        return;
    }
    engine_NotepadBackend* backend = (engine_NotepadBackend*)platform->backend;

    if (backend->buffer) {
        free(backend->buffer);
    }
    free(backend);
}

static void NotepadBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height) {
    static const wchar shade_chars[] = L" .',:!*|+%8#@█"; // More granular shading
    // static const wchar shade_chars[] = L" ░▒▓█"; // Using block elements for better spacing
    static const u32   shade_levels  = (sizeof(shade_chars) / sizeof(wchar)) - 1;

    debug_assert_nonnull(platform);
    debug_assert_nonnull(data);

    let backend = (engine_NotepadBackend*)platform->backend;
    let pixels  = data;

    // Reset buffer
    backend->buffer_size = 0;

    // Convert frame to text art
    for (u32 y = 0; y < height; y += 2) {
        for (u32 x = 0; x < width; ++x) {
            const Color* upper = &data[y * width + x];
            const Color* lower = (y + 1 < height) ? &data[(y + 1) * width + x] : upper;

            u8 avg_intensity = (NotepadBackend_getGrayscale(upper) + NotepadBackend_getGrayscale(lower)) >> 1;
            u8 shade_idx     = (avg_intensity * (shade_levels - 1)) >> 8;

            backend->buffer[backend->buffer_size++] = shade_chars[shade_idx];
        }
        backend->buffer[backend->buffer_size++] = L'\r';
        backend->buffer[backend->buffer_size++] = L'\n';
    }
    backend->buffer[backend->buffer_size] = L'\0';

    // Write to file
    FILE* fp = _wfopen(L"game_output.txt", L"w");
    if (fp) {
        ignore fwrite(backend->buffer, sizeof(wchar), backend->buffer_size, fp);
        ignore fclose(fp);

        // Get console window handle
        HWND console = GetConsoleWindow();

        // Brief focus switch to trigger Notepad refresh
        SetForegroundWindow(backend->notepad_handle);
        Sleep(10);
        SetForegroundWindow(console);
    }
}
