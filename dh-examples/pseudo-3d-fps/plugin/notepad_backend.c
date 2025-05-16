#include <windows.h>

#include "notepad_backend.h"
#include "engine/input.h"

#include <stdio.h>

// D2CodingLigature Nerd Font
/*
NOTE: 테스트해볼 솔루션을 찾았다!!!
    Windows 11에서는 Edit이 아닌 RichEdit 컨트롤을 사용해야한다는 점.
    한번 해보자!

    winapi - SendMessage returns 0 when reading contents from notepad window using Python and Win32 API - Stack Overflow - https://stackoverflow.com/questions/72280096/sendmessage-returns-0-when-reading-contents-from-notepad-window-using-python-and

    Windows 11에서 텍스트를 메모장에 덤프하는 SendMessageByString API가 작동하지 않음-VBForums - https://www.vbforums.com/showthread.php?906994-SendMessageByString-API-to-dump-text-to-Notepad-not-working-in-Windows-11&s=bcd22b6cd77376c846df6ed31c584277

---

    결국 Edit 등을 활용하는 방법으로는 실패하고 허무한 방법으로 해결했다...
*/

static void       NotepadBackend_destroy(engine_Platform* platform);
static void       NotepadBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height);
/**
 * Using precomputed coefficients (R*0.299 + G*0.587 + B*0.114)
 * Multiplied by 256 for fixed-point arithmetic
 */
$inline_always u8 NotepadBackend_getGrayscale(const Color* color) {
    return (u8)((color->r * 77 + color->g * 150 + color->b * 29) >> 8);
}
use_Err$(HWND);
static Err$HWND NotepadBackend_launchNotepad(void);

config_ErrSet(
    NotepadBackendErr,
    OutOfMemoryNotepadBacked,
    OutOfMemoryNotepadBackendBuffer,
    FailedCreateNotepadProcess,
    FailedIncreaseNotepadWorkingSet,
    FailedFindNotepadWindow,
    FailedFindNotepadEditControl,
    CantSendTextToNotepad,
);

Err$Ptr$engine_RenderBackend NotepadBackend_create(void) {
    scope_reserveReturn(Err$Ptr$engine_RenderBackend) {
        let backend = (engine_NotepadBackend*)malloc(sizeof(engine_NotepadBackend));
        if (!backend) {
            return_err(NotepadBackendErr_err(NotepadBackendErrCode_OutOfMemoryNotepadBacked));
        }
        errdefer(free(backend));
        memset(backend, 0, sizeof(engine_NotepadBackend));

        let capacity = 160ull * 100ull;
        let buffer   = (wchar*)malloc(capacity * sizeof(wchar));
        if (!buffer) {
            return_err(NotepadBackendErr_err(NotepadBackendErrCode_OutOfMemoryNotepadBackendBuffer));
        }
        errdefer(free(buffer));
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
            return_err(err);
        });

        // Set up backend interface
        backend->base = eval({
            var vt           = engine_RenderBackend_createNoOp();
            vt.type          = engine_RenderBackendType_custom;
            vt.destroy       = NotepadBackend_destroy;
            vt.presentBuffer = NotepadBackend_presentBuffer;
            eval_return vt;
        });
        return_ok(&backend->base);
    }
    scope_returnReserved;
}

static void NotepadBackend_destroy(engine_Platform* platform) {
    if (!platform || !platform->backend) {
        return;
    }
    engine_NotepadBackend* backend = (engine_NotepadBackend*)platform->backend;

    if (backend->notepad_handle) {
        // Get the stored handles
        HANDLE hProcess = (HANDLE)GetPropW(backend->notepad_handle, L"ProcessHandle");
        HANDLE hThread  = (HANDLE)GetPropW(backend->notepad_handle, L"ThreadHandle");

        if (hProcess) {
            TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
        }
        if (hThread) {
            CloseHandle(hThread);
        }

        RemovePropW(backend->notepad_handle, L"ProcessHandle");
        RemovePropW(backend->notepad_handle, L"ThreadHandle");
    }

    _wremove(L"game_output.txt");

    if (backend->buffer) {
        free(backend->buffer);
    }
    free(backend);
}

static void NotepadBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height) {
    // Option 1: Traditional ASCII art (22 levels)
    static const wchar shade_chars[] = L" .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";

    // // Option 2: Simple but effective (10 levels)
    // static const wchar shade_chars[] = L" .:-=+*#%@";

    // // Option 3: Dense shading (15 levels)
    // static const wchar shade_chars[] = L" ░▒▓█▄▀■◤◥◢◣▲▼●";

    // // Option 4: Mixed ASCII and blocks (16 levels)
    // static const wchar shade_chars[] = L" .',:;+=xX$░▒▓█▀■";

    // // Option 5: Detailed gradients (30 levels)
    // static const wchar shade_chars[] = L" `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$MB%&QN@";

    // // Option 6: Block-based only (8 levels)
    // static const wchar shade_chars[] = L"  ░░▒▒▓▓██";

    // // Option 7: Compact but readable (12 levels)
    // static const wchar shade_chars[] = L" ._-=+*:|%#@$";

    static const u32 shade_levels = (sizeof(shade_chars) / sizeof(wchar)) - 1;

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
        $ignore = fwrite(backend->buffer, sizeof(wchar), backend->buffer_size, fp);
        $ignore = fclose(fp);

        // Get console window handle
        HWND console = GetConsoleWindow();

        static engine_KeyCode input_key_list[] = {
            engine_KeyCode_a,
            engine_KeyCode_d,
            engine_KeyCode_s,
            engine_KeyCode_w,
            engine_KeyCode_arrow_left,
            engine_KeyCode_arrow_up,
            engine_KeyCode_arrow_right,
            engine_KeyCode_arrow_down
        };
        static const u32 input_key_count = sizeof(input_key_list) / sizeof(engine_KeyCode);

        bool has_any_held_key = false;
        for (u32 i = 0; i < input_key_count; ++i) {
            if (engine_Key_held(input_key_list[i])) {
                has_any_held_key = true;
                break;
            }
        }

        if (has_any_held_key) {
            // Brief focus switch to trigger Notepad refresh
            SetForegroundWindow(backend->notepad_handle);
            Sleep(10);
        }
        SetForegroundWindow(console);
    }
}

static Err$HWND NotepadBackend_launchNotepad(void) {
    reserveReturn(Err$HWND);

    // Create initial file
    FILE* fp = _wfopen(L"game_output.txt", L"w");
    if (fp) {
        $ignore = fwprintf(fp, L"Loading...\n");
        $ignore = fclose(fp);
    }

    // Create process
    STARTUPINFOW        si = cleared();
    PROCESS_INFORMATION pi = cleared();
    si.cb                  = sizeof(si);
    si.dwFlags             = STARTF_USESHOWWINDOW;
    si.wShowWindow         = SW_SHOW;

    wchar cmd[MAX_PATH * 2] = cleared();
    _snwprintf(cmd, MAX_PATH * 2, L"notepad.exe game_output.txt");

    if (!CreateProcessW(null, cmd, null, null, false,
                        CREATE_SUSPENDED, // Start suspended
                        null,
                        null,
                        &si,
                        &pi)) {
        printf("CreateProcess failed (%lu)\n", GetLastError());
        return_err(NotepadBackendErr_err(NotepadBackendErrCode_FailedCreateNotepadProcess));
    }

    // Increase working set
    let min_ws = 100ull * 1024ull * 1024ull; // 100 MB
    let max_ws = 500ull * 1024ull * 1024ull; // 500 MB
    if (!SetProcessWorkingSetSize(pi.hProcess, min_ws, max_ws)) {
        printf("SetProcessWorkingSetSize failed (%lu)\n", GetLastError());
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return_err(NotepadBackendErr_err(NotepadBackendErrCode_FailedIncreaseNotepadWorkingSet));
    }

    // Resume the process
    ResumeThread(pi.hThread);

    // Wait for window creation
    HWND  notepad   = null;
    HWND  edit      = null;
    DWORD startTime = GetTickCount();

    printf("Waiting for notepad window...\n");
    while (GetTickCount() - startTime < 5000) { // 5 second timeout
        Sleep(100);

        if (!notepad) {
            notepad = FindWindowW(L"Notepad", null);
            if (!notepad) {
                printf("try 1: failed to find notepad window (Notepad)\n");
                notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
            }
            if (!notepad) {
                printf("try 2: failed to find notepad window (ApplicationFrameWindow, Notepad)\n");
                continue;
            }
            printf("Found notepad window\n");
        }

        if (!edit) {
            edit = FindWindowExW(notepad, null, L"Edit", null);
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
                continue;
            }
            printf("Found edit control\n");
        }

        break;
    }

    if (!notepad) {
        printf("Failed to find notepad window\n");
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return_err(NotepadBackendErr_err(NotepadBackendErrCode_FailedFindNotepadWindow));
    }

    if (!edit) {
        printf("Failed to find edit control\n");
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return_err(NotepadBackendErr_err(NotepadBackendErrCode_FailedFindNotepadEditControl));
    }

    // Set font
    HFONT hFont = CreateFontW(
        14,                      // Height
        0,                       // Width (auto)
        0,                       // Escapement
        0,                       // Orientation
        FW_NORMAL,               // Weight
        false,                   // Italic
        false,                   // Underline
        false,                   // StrikeOut
        DEFAULT_CHARSET,         // CharSet
        OUT_DEFAULT_PRECIS,      // OutPrecision
        CLIP_DEFAULT_PRECIS,     // ClipPrecision
        DEFAULT_QUALITY,         // Quality
        FIXED_PITCH | FF_MODERN, // Pitch and Family
        L"Consolas"              // Face Name
    );
    if (hFont) {
        SendMessageW(edit, WM_SETFONT, (WPARAM)hFont, true);
    }
    // Configure window
    SetWindowPos(notepad, HWND_TOPMOST, 100, 100, 800, 600, SWP_SHOWWINDOW);
    $ignore = getchar(); // user setting step
    // EnableWindow(notepad, false);

    // Store process handles for cleanup
    SetPropW(notepad, L"ProcessHandle", (HANDLE)pi.hProcess);
    SetPropW(notepad, L"ThreadHandle", (HANDLE)pi.hThread);

    return_ok(notepad);
}
