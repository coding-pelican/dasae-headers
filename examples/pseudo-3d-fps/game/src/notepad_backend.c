#include "notepad_backend.h"

#include <stdio.h>
#include <shellapi.h>
#include <windows.h>

/*
NOTE: 테스트해볼 솔루션을 찾았다!!!
    Windows 11에서는 Edit이 아닌 RichEdit 컨트롤을 사용해야한다는 점.
    한번 해보자!

    winapi - SendMessage returns 0 when reading contents from notepad window using Python and Win32 API - Stack Overflow - https://stackoverflow.com/questions/72280096/sendmessage-returns-0-when-reading-contents-from-notepad-window-using-python-and

    Windows 11에서 텍스트를 메모장에 덤프하는 SendMessageByString API가 작동하지 않음-VBForums - https://www.vbforums.com/showthread.php?906994-SendMessageByString-API-to-dump-text-to-Notepad-not-working-in-Windows-11&s=bcd22b6cd77376c846df6ed31c584277
*/

static void NotepadBackend_destroy(engine_Platform* platform);
static void NotepadBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height);

using_Err$(HWND);
static Err$HWND NotepadBackend_launchNotepad(void) must_check;
static Err$HWND NotepadBackend_findNotepadWindow(void) must_check;
static Err$HWND NotepadBackend_getNotepadEditControl(HWND notepad) must_check;
static Err$void NotepadBackend_sendText(HWND notepad, const wchar* text) must_check;
// static HWND     NotepadBackend_setNotepadWindow(void);
// Using precomputed coefficients (R*0.299 + G*0.587 + B*0.114)
// Multiplied by 256 for fixed-point arithmetic
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

    // Launch and configure notepad
    backend->notepad_handle = catch (NotepadBackend_launchNotepad(), err, {
        free(buffer);
        free(backend);
        return_err(err);
    });

    // Send initial text
    catch (NotepadBackend_sendText(backend->notepad_handle, backend->buffer), err, {
        free(buffer);
        free(backend);
        return_err(err);
    });

    // Set up backend interface
    backend->base.type          = engine_RenderBackendType_custom;
    backend->base.presentBuffer = NotepadBackend_presentBuffer;
    backend->base.processEvents = null;
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
    static const u32   shade_levels  = (sizeof(shade_chars) / sizeof(wchar)) - 1;

    debug_assert_nonnull(platform);
    debug_assert_nonnull(data);
    engine_NotepadBackend* backend = (engine_NotepadBackend*)platform->backend;

    // Convert frame to ASCII
    backend->buffer_size = 0;
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

    HWND notepad = FindWindowW(L"Notepad", null);
    if (!notepad) {
        notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
    }

    if (notepad) {
        HWND edit = FindWindowExW(notepad, null, L"Edit", null);
        if (!edit) {
            edit = FindWindowExW(notepad, null, L"RichEditD2DPT", null);
        }

        if (edit) {
            SendMessageW(edit, WM_SETTEXT, 0, (LPARAM)backend->buffer);
        }
    } else {
        // No notepad window found, create new one
        // NotepadBackend_setNotepadWindow();
    }
}

static Err$HWND NotepadBackend_launchNotepad(void) {
    reserveReturn(Err$HWND);
    // Launch notepad process
    WCHAR system_dir[MAX_PATH] = cleared();
    GetSystemDirectoryW(system_dir, MAX_PATH);
    WCHAR notepad_path[MAX_PATH * 2] = cleared();
    _snwprintf(notepad_path, MAX_PATH * 2, L"%s\\notepad.exe", system_dir);

    STARTUPINFOW        startup = cleared();
    PROCESS_INFORMATION process = cleared();
    startup.cb                  = sizeof(startup);

    if (!CreateProcessW(notepad_path, null, null, null, false, CREATE_NO_WINDOW, null, null, &startup, &process)) {
        return_err(NotepadBackendErr_err(NotepadBackendErrType_FailedCreateNotepadProcess));
    }

    // Wait for the window to be created
    Err$HWND notepad_result = cleared();
    Err$HWND edit_result    = cleared();
    DWORD    startTime      = GetTickCount();

    while (GetTickCount() - startTime < 5000) { // 5 second timeout
        // Try to find the notepad window
        notepad_result = NotepadBackend_findNotepadWindow();
        if (notepad_result.is_err) {
            Sleep(100);
            continue;
        }

        // Try to get the edit control
        edit_result = NotepadBackend_getNotepadEditControl(notepad_result.ok);
        if (edit_result.is_err) {
            Sleep(100);
            continue;
        }

        // Window and edit control found
        printf("Successfully created notepad with edit control\n");
        break;
    }

    CloseHandle(process.hProcess);
    CloseHandle(process.hThread);

    let notepad = try(notepad_result);
    let edit    = try(edit_result);
    unused(edit);

    // Configure window
    SetWindowPos(notepad, HWND_TOPMOST, 100, 100, 800, 600, SWP_SHOWWINDOW);

    // Make edit control read-only
    // SendMessageW(edit, EM_SETREADONLY, true, 0);
    return_ok(notepad);
}

static Err$HWND NotepadBackend_findNotepadWindow(void) {
    reserveReturn(Err$HWND);
    HWND notepad = FindWindowW(L"Notepad", null);
    if (!notepad) {
        notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
    }
    if (!notepad) {
        return_err(NotepadBackendErr_err(NotepadBackendErrType_FailedFindNotepadWindow));
    }
    return_ok(notepad);
}

static Err$HWND NotepadBackend_getNotepadEditControl(HWND notepad) {
    reserveReturn(Err$HWND);
    if (!notepad) {
        return_err(NotepadBackendErr_err(NotepadBackendErrType_InvalidArgument));
    }
    HWND edit = FindWindowExW(notepad, null, L"Edit", null); // Win10
    if (!edit) {
        edit = FindWindowExW(notepad, null, L"RichEditD2DPT", null); // Win11
    }
    if (!edit) {
        return_err(NotepadBackendErr_err(NotepadBackendErrType_FailedGetNotepadEditControl));
    }
    return_ok(edit);
}

static Err$void NotepadBackend_sendText(HWND notepad, const wchar* text) {
    reserveReturn(Err$void);
    if (!notepad) {
        return_err(NotepadBackendErr_err(NotepadBackendErrType_InvalidArgument));
    }
    if (!text) {
        return_err(NotepadBackendErr_err(NotepadBackendErrType_InvalidArgument));
    }
    HWND edit = try(NotepadBackend_getNotepadEditControl(notepad));
    if (SendMessageW(edit, WM_SETTEXT, 255, (LPARAM)text)) {
        return_err(NotepadBackendErr_err(NotepadBackendErrType_CantSendTextToNotepad));
    }
    return_ok({});
}

// static HWND NotepadBackend_setNotepadWindow(void) {
//     HWND notepad = FindWindowW(L"Notepad", null);
//     if (!notepad) {
//         notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
//     }

//     if (notepad) {
//         // Try different edit controls for Win10/Win11
//         HWND edit = FindWindowExW(notepad, null, L"Edit", null); // Win10
//         if (!edit) {
//             edit = FindWindowExW(notepad, null, L"RichEditD2DPT", null); // Win11
//         }

//         if (edit) {
//             SendMessageW(edit, EM_SETREADONLY, true, 0); // Make read-only

//             // Configure window
//             SetWindowLongW(notepad, GWL_EXSTYLE, GetWindowLongW(notepad, GWL_EXSTYLE) | WS_EX_NOACTIVATE);
//             SetWindowPos(notepad, HWND_TOPMOST, 100, 100, 800, 600, SWP_SHOWWINDOW | SWP_NOACTIVATE);

//             return notepad;
//         }
//     }
//     return null;
// }
