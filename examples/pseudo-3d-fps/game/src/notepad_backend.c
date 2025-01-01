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
using_Err$(HWND);
static Err$HWND NotepadBackend_launchNotepad(void);

impl_Err(
    NotepadBackendErr,
    OutOfMemoryNotepadBacked,
    OutOfMemoryNotepadBackendBuffer,
    FailedCreateNotepadProcess,
    FailedIncreaseNotepadWorkingSet,
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

    let capacity = 160ull * 100ull;
    let buffer   = (wchar*)malloc(capacity * sizeof(wchar));
    if (!buffer) {
        free(backend);
        return_err(NotepadBackendErr_err(NotepadBackendErrType_OutOfMemoryNotepadBackendBuffer));
    }
    memset(buffer, 0, capacity * sizeof(wchar));

    backend->buffer          = buffer;
    backend->buffer_capacity = capacity;
    backend->buffer_size     = 0;

    // Create test message
    const wchar* test_msg = L"Hello, notepad!\n";
    wcscpy(backend->buffer, test_msg);
    backend->buffer_size = wcslen(test_msg);

    // Launch notepad and store handle
    backend->notepad_handle = catch (NotepadBackend_launchNotepad(), err, {
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

    if (backend->notepad_handle) {
        // Get the process ID we stored
        DWORD process_id = (DWORD)(DWORD_PTR)GetPropW(backend->notepad_handle, L"ProcessId");
        if (process_id) {
            // Get process handle and terminate it
            HANDLE h_process = OpenProcess(PROCESS_TERMINATE, FALSE, process_id);
            if (h_process) {
                TerminateProcess(h_process, 0);
                CloseHandle(h_process);
            }
        }
        RemovePropW(backend->notepad_handle, L"ProcessId");
    }

    // Delete the output file
    _wremove(L"game_output.txt");

    if (backend->buffer) {
        free(backend->buffer);
    }
    free(backend);
}

static void NotepadBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height) {
    static const wchar shade_chars[] = L" .',:!*|+8#@"; // More granular shading
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
            const Color* upper = &pixels[y * width + x];
            const Color* lower = (y + 1 < height) ? &pixels[(y + 1) * width + x] : upper;

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

static Err$HWND NotepadBackend_launchNotepad(void) {
    reserveReturn(Err$HWND);

    // Create initial file
    FILE* fp = _wfopen(L"game_output.txt", L"w");
    if (fp) {
        ignore fwprintf(fp, L"Loading...\n");
        ignore fclose(fp);
    }

    // Launch Notepad
    // ShellExecuteW(null, L"open", L"notepad.exe", L"game_output.txt", null, SW_SHOW);
    // Sleep(500); // Wait for window creation

    // Create process
    STARTUPINFOA        si = cleared();
    PROCESS_INFORMATION pi = cleared();
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcessA("C:\\Windows\\notepad.exe", NULL, NULL, NULL, FALSE,
                        CREATE_SUSPENDED, // Start suspended so we can adjust
                        NULL,
                        NULL,
                        &si,
                        &pi)) {
        printf("CreateProcess failed (%lu)\n", GetLastError());
        return_err(NotepadBackendErr_err(NotepadBackendErrType_FailedCreateNotepadProcess));
    }

    // Attempt to increase the Notepad process working set
    let min_ws = 100ull * 1024ull * 1024ull; // 100 MB
    let max_ws = 500ull * 1024ull * 1024ull; // 500 MB
    if (!SetProcessWorkingSetSize(pi.hProcess, min_ws, max_ws)) {
        printf("SetProcessWorkingSetSize failed (%lu)\n", GetLastError());
        return_err(NotepadBackendErr_err(NotepadBackendErrType_FailedIncreaseNotepadWorkingSet));
    }

    // Now resume the Notepad process
    ResumeThread(pi.hThread);

    // Wait until Notepad exits
    // WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // Find the window
    HWND notepad = FindWindowW(L"Notepad", null);
    if (!notepad) {
        printf("try 1: failed to find notepad window (Notepad)\n");
        notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
    }
    if (!notepad) {
        printf("try 2: failed to find notepad window (ApplicationFrameWindow, Notepad)\n");
        return_err(NotepadBackendErr_err(NotepadBackendErrType_FailedCreateNotepadProcess));
    }
    printf("Found notepad window\n");

    // Set up the font
    HWND edit = FindWindowExW(notepad, null, L"Edit", null);
    if (!edit) {
        printf("try 1: failed to find edit control (Edit)\n");
        edit = FindWindowExW(notepad, null, L"RichEditD2DPT", null);
    }
    if (!edit) {
        printf("try 2: failed to find edit control (RichEditD2DPT)\n");
        edit = FindWindowExW(notepad, null, L"RICHEDIT50W", null);
    }
    if (!edit) {
        printf("try 3: failed to find edit control (RICHEDIT50W)\n");
        edit = FindWindowExW(notepad, null, L"NotepadTextBox", null);
    }
    if (!edit) {
        printf("try 4: failed to find edit control (NotepadTextBox)\n");
    } else {
        printf("Found edit control\n");
        // Create font
        HFONT hFont = CreateFontW(
            14,                      // Height (4px)
            0,                       // Width (auto)
            0,                       // Escapement
            0,                       // Orientation
            FW_NORMAL,               // Weight
            FALSE,                   // Italic
            FALSE,                   // Underline
            FALSE,                   // StrikeOut
            DEFAULT_CHARSET,         // CharSet
            OUT_DEFAULT_PRECIS,      // OutPrecision
            CLIP_DEFAULT_PRECIS,     // ClipPrecision
            DEFAULT_QUALITY,         // Quality
            FIXED_PITCH | FF_MODERN, // Pitch and Family
            L"Consolas"              // Face Name
            // L"D2CodingLigature Nerd Font" // Face Name
        );
        if (hFont) {
            SendMessageW(edit, WM_SETFONT, (WPARAM)hFont, TRUE);
        } else {
            printf("failed to create font\n");
        }
    }

    // Calculate window size based on font metrics
    // HDC         hdc = GetDC(edit);
    // TEXTMETRICW tm;
    // GetTextMetricsW(hdc, &tm);
    // ReleaseDC(edit, hdc);

    // i32 char_width  = tm.tmAveCharWidth;
    // i32 char_height = tm.tmHeight + tm.tmExternalLeading;

    // // Set window size to fit content (add margins for window borders)
    // i32 window_width  = char_width * (80) * 2 + GetSystemMetrics(SM_CXFRAME) * 2;
    // i32 window_height = char_height * (50) * 2 + GetSystemMetrics(SM_CYFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
    // window_width /= 2;
    // window_height /= 2;

    // Position window and disable user input
    SetWindowPos(notepad, HWND_TOPMOST, 100, 100, 800, 600, SWP_SHOWWINDOW);
    ignore getchar(); // Wait for user input (Setup)
    EnableWindow(notepad, FALSE);

    // Store process ID for cleanup
    DWORD process_id = 0;
    GetWindowThreadProcessId(notepad, &process_id);
    SetPropW(notepad, L"ProcessId", (HANDLE)(DWORD_PTR)process_id); // NOLINT

    return_ok(notepad);
}
