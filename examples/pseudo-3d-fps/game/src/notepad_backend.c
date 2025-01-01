#include <stdio.h>
#include <Richedit.h>
#include <shellapi.h>
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

static void NotepadBackend_destroy(engine_Platform* platform);
static void NotepadBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height);

using_Err$(HWND);
static void          DebugWindowInfo(HWND hwnd);
static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
static Err$HWND      NotepadBackend_launchNotepad(void) must_check;
static Err$HWND      NotepadBackend_findNotepadWindow(void) must_check;
static Err$HWND      NotepadBackend_getNotepadEditControl(HWND notepad) must_check;
static Err$void      NotepadBackend_sendTextToNotepad(HWND notepad, const wchar* text) must_check;
// Using precomputed coefficients (R*0.299 + G*0.587 + B*0.114)
// Multiplied by 256 for fixed-point arithmetic
force_inline u8      NotepadBackend_getGrayscale(const Color* color) {
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

// static void NotepadBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height) {
//     static const wchar shade_chars[] = L" .',:!*|+%8#@█"; // More granular shading
//     // static const wchar shade_chars[] = L" ░▒▓█"; // Using block elements for better spacing
//     static const u32   shade_levels  = (sizeof(shade_chars) / sizeof(wchar)) - 1;

//     debug_assert_nonnull(platform);
//     debug_assert_nonnull(data);
//     engine_NotepadBackend* backend = (engine_NotepadBackend*)platform->backend;

//     // Reset buffer
//     backend->buffer_size = 0;

//     // Convert frame to text art
//     for (u32 y = 0; y < height; y += 2) {
//         for (u32 x = 0; x < width; ++x) {
//             const Color* upper = &data[y * width + x];
//             const Color* lower = (y + 1 < height) ? &data[(y + 1) * width + x] : upper;

//             u8 avg_intensity = (NotepadBackend_getGrayscale(upper) + NotepadBackend_getGrayscale(lower)) >> 1;
//             u8 shade_idx     = (avg_intensity * (shade_levels - 1)) >> 8;

//             backend->buffer[backend->buffer_size++] = shade_chars[shade_idx];
//         }
//         backend->buffer[backend->buffer_size++] = L'\r';
//         backend->buffer[backend->buffer_size++] = L'\n';
//     }
//     backend->buffer[backend->buffer_size] = L'\0';

//     // Find or create notepad window
//     HWND notepad = FindWindowW(L"Notepad", NULL);
//     if (!notepad) {
//         notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
//     }

//     if (notepad) {
//         // Try to find the edit control directly
//         HWND edit = FindWindowExW(notepad, NULL, L"NotepadTextBox", NULL);
//         if (edit) {
//             // First clear existing content
//             SendMessageW(edit, WM_SETTEXT, 0, (LPARAM)L"");

//             // Wait a bit for the clear to take effect
//             Sleep(1);

//             // Then set new content
//             SendMessageW(edit, WM_SETTEXT, 0, (LPARAM)backend->buffer);
//         }
//     }
// }

// static void NotepadBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height) {
//     // static const wchar shade_chars[] = L" ░▒▓█";
//     static const wchar shade_chars[] = L" .',:!*|+%8#@█"; // More granular shading
//     static const u32   shade_levels  = (sizeof(shade_chars) / sizeof(wchar)) - 1;

//     debug_assert_nonnull(platform);
//     debug_assert_nonnull(data);
//     engine_NotepadBackend* backend = (engine_NotepadBackend*)platform->backend;

//     // Reset buffer
//     backend->buffer_size = 0;

//     printf("Converting frame: %dx%d\n", width, height);

//     // Convert frame to text art
//     for (u32 y = 0; y < height; y += 2) {
//         for (u32 x = 0; x < width; ++x) {
//             const Color* upper = &data[y * width + x];
//             const Color* lower = (y + 1 < height) ? &data[(y + 1) * width + x] : upper;

//             u8 avg_intensity = (NotepadBackend_getGrayscale(upper) + NotepadBackend_getGrayscale(lower)) >> 1;
//             u8 shade_idx     = (avg_intensity * (shade_levels - 1)) >> 8;

//             backend->buffer[backend->buffer_size++] = shade_chars[shade_idx];
//         }
//         backend->buffer[backend->buffer_size++] = L'\r';
//         backend->buffer[backend->buffer_size++] = L'\n';
//     }
//     backend->buffer[backend->buffer_size] = L'\0';

//     printf("Buffer size: %zu chars\n", backend->buffer_size);
//     printf("First 20 chars: ");
//     for (usize i = 0; i < 20 && i < backend->buffer_size; i++) {
//         printf("%C", backend->buffer[i]);
//     }
//     printf("\n");

//     // Find notepad window
//     HWND notepad = FindWindowW(L"Notepad", NULL);
//     if (!notepad) {
//         notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
//     }

//     if (!notepad) {
//         printf("Could not find notepad window!\n");
//         return;
//     }

//     // Get edit control
//     HWND edit = FindWindowExW(notepad, NULL, L"NotepadTextBox", NULL);
//     if (!edit) {
//         printf("Could not find edit control!\n");
//         return;
//     }

//     printf("Found notepad (%p) and edit control (%p)\n", (void*)notepad, (void*)edit);

//     // Try multiple methods to update text
//     // Method 1: Direct WM_SETTEXT
//     BOOL result = (BOOL)SendMessageW(edit, WM_SETTEXT, 0, (LPARAM)backend->buffer);
//     printf("WM_SETTEXT result: %d (error: %lu)\n", result, GetLastError());

//     if (!result) {
//         // Method 2: EM_SETSEL + EM_REPLACESEL
//         SendMessageW(edit, EM_SETSEL, 0, -1); // Select all
//         result = (BOOL)SendMessageW(edit, EM_REPLACESEL, TRUE, (LPARAM)backend->buffer);
//         printf("EM_REPLACESEL result: %d (error: %lu)\n", result, GetLastError());
//     }

//     // Force redraw
//     InvalidateRect(edit, NULL, TRUE);
//     UpdateWindow(edit);
// }

// static void NotepadBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height) {
//     debug_assert_nonnull(platform);
//     debug_assert_nonnull(data);
//     engine_NotepadBackend* backend = (engine_NotepadBackend*)platform->backend;

//     // Calculate required buffer size first
//     const size_t chars_per_line = width + 2; // chars + \r\n
//     const size_t total_lines    = height / 2;
//     const size_t required_size  = (chars_per_line * total_lines) + 1; // +1 for null terminator

//     // Ensure buffer is large enough
//     if (backend->buffer_capacity < required_size) {
//         printf("Buffer too small: %zu < %zu\n", backend->buffer_capacity, required_size);
//         return;
//     }

//     // Reset buffer
//     backend->buffer_size = 0;

//     // Build frame line by line to minimize memory fragmentation
//     for (u32 y = 0; y < height; y += 2) {
//         for (u32 x = 0; x < width; ++x) {
//             const Color* upper = &data[y * width + x];
//             const Color* lower = (y + 1 < height) ? &data[(y + 1) * width + x] : upper;

//             u8 avg_intensity = (NotepadBackend_getGrayscale(upper) + NotepadBackend_getGrayscale(lower)) >> 1;

//             // Even fewer characters to reduce memory usage
//             wchar ch;
//             if (avg_intensity > 192) {
//                 ch = L' ';
//             } else if (avg_intensity > 128) {
//                 ch = L'.';
//             } else if (avg_intensity > 64) {
//                 ch = L'o';
//             } else {
//                 ch = L'@';
//             }

//             backend->buffer[backend->buffer_size++] = ch;
//         }
//         backend->buffer[backend->buffer_size++] = L'\r';
//         backend->buffer[backend->buffer_size++] = L'\n';
//     }
//     backend->buffer[backend->buffer_size] = L'\0';

//     FILE* fp = _wfopen(L"game_output.txt", L"w, ccs=UTF-16LE");
//     if (fp) {
//         ignore fwprintf(fp, backend->buffer);
//         ignore fclose(fp);
//     }

//     // Find notepad window
//     HWND notepad = FindWindowW(L"Notepad", NULL);
//     if (!notepad) {
//         notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
//     }

//     if (notepad) {
//         HWND edit = FindWindowExW(notepad, NULL, L"NotepadTextBox", NULL);
//         if (edit) {
//             // Use EM_SETTEXTEX for better memory handling
//             SETTEXTEX text = {
//                 .flags    = ST_KEEPUNDO | ST_SELECTION, // Keep undo buffer and selection
//                 .codepage = 1200                        // UTF-16LE
//             };

//             // Update only if window exists and is valid
//             if (IsWindow(edit) && !IsWindowVisible(edit)) {
//                 SendMessageW(edit, EM_SETTEXTEX, (WPARAM)&text, (LPARAM)backend->buffer);
//             }
//         }
//     }
// }

// static Err$HWND NotepadBackend_launchNotepad(void) {
//     reserveReturn(Err$HWND);

//     // Create minimal initial file
//     FILE* fp = _wfopen(L"game_output.txt", L"w, ccs=UTF-16LE");
//     if (fp) {
//         ignore fwprintf(fp, L"Loading...\n");
//         ignore fclose(fp);
//     }

//     // Launch with specific window size
//     STARTUPINFOW        si = { 0 };
//     PROCESS_INFORMATION pi = { 0 };
//     si.cb                  = sizeof(si);
//     si.dwFlags             = STARTF_USESHOWWINDOW;
//     si.wShowWindow         = SW_SHOWNOACTIVATE;

//     WCHAR cmd[MAX_PATH * 2];
//     _snwprintf(cmd, MAX_PATH * 2, L"notepad.exe game_output.txt");

//     if (!CreateProcessW(NULL, cmd, NULL, NULL, FALSE,
//                         CREATE_NEW_CONSOLE | CREATE_SUSPENDED, // Start suspended to configure window
//                         NULL,
//                         NULL,
//                         &si,
//                         &pi)) {
//         return_err(NotepadBackendErr_err(NotepadBackendErrType_FailedCreateNotepadProcess));
//     }

//     // Resume the process
//     ResumeThread(pi.hThread);

//     // Wait for window
//     HWND  notepad   = NULL;
//     DWORD startTime = GetTickCount();

//     while (GetTickCount() - startTime < 5000) {
//         notepad = FindWindowW(L"Notepad", NULL);
//         if (!notepad) {
//             notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
//         }

//         if (notepad) {
//             HWND edit = FindWindowExW(notepad, NULL, L"NotepadTextBox", NULL);
//             if (edit) {
//                 // Configure window for minimal resource usage
//                 DWORD style = GetWindowLongW(notepad, GWL_STYLE);
//                 style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX); // Remove resizing
//                 SetWindowLongW(notepad, GWL_STYLE, style);

//                 SetWindowLongW(notepad, GWL_EXSTYLE, GetWindowLongW(notepad, GWL_EXSTYLE) | WS_EX_NOACTIVATE);

//                 // Position window
//                 SetWindowPos(notepad, HWND_TOPMOST, 100, 100, 800, 600, SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_FRAMECHANGED);

//                 // Clean up process handles
//                 CloseHandle(pi.hThread);
//                 CloseHandle(pi.hProcess);

//                 return_ok(notepad);
//             }
//         }
//         Sleep(100);
//     }

//     // Clean up on failure
//     CloseHandle(pi.hThread);
//     CloseHandle(pi.hProcess);
//     return_err(NotepadBackendErr_err(NotepadBackendErrType_FailedGetNotepadEditControl));
// }

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

static Err$HWND NotepadBackend_launchNotepad(void) {
    reserveReturn(Err$HWND);

    // Create initial file
    FILE* fp = _wfopen(L"game_output.txt", L"w");
    if (fp) {
        ignore fwprintf(fp, L"Loading...\n");
        ignore fclose(fp);
    }

    // Launch Notepad
    ShellExecuteW(NULL, L"open", L"notepad.exe", L"game_output.txt", NULL, SW_SHOW);
    Sleep(500); // Wait for window creation

    // Find the window
    HWND notepad = FindWindowW(L"Notepad", NULL);
    if (!notepad) {
        notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
    }
    if (!notepad) {
        return_err(NotepadBackendErr_err(NotepadBackendErrType_FailedCreateNotepadProcess));
    }

    // Position window and disable user input
    SetWindowPos(notepad, HWND_TOPMOST, 100, 100, 800, 600, SWP_SHOWWINDOW);
    EnableWindow(notepad, FALSE);

    return_ok(notepad);
}

static void DebugWindowInfo(HWND hwnd) {
    if (!hwnd) {
        return;
    }

    WCHAR class_name[256]  = cleared();
    WCHAR window_text[256] = cleared();

    GetClassNameW(hwnd, class_name, 256);
    GetWindowTextW(hwnd, window_text, 256);

    printf("Window Handle: %p\n", (void*)hwnd);
    printf("Class Name: %ls\n", class_name);
    printf("Window Text: %ls\n", window_text);
}

static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    unused(lParam);
    WCHAR class_name[256] = cleared();
    GetClassNameW(hwnd, class_name, 256);

    // Look for anything that might be notepad
    if (wcsstr(class_name, L"Notepad") || wcsstr(class_name, L"ApplicationFrame")) {
        printf("\nFound potential notepad window:\n");
        DebugWindowInfo(hwnd);

        // Enumerate child windows
        printf("Child windows:\n");
        EnumChildWindows(hwnd, EnumChildProc, 0);
    }
    return true;
}

static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
    unused(lParam);
    WCHAR class_name[256] = cleared();
    GetClassNameW(hwnd, class_name, 256);
    printf("  Child window class: %ls\n", class_name);
    return true;
}

// static Err$HWND NotepadBackend_launchNotepad(void) {
//     reserveReturn(Err$HWND);

//     // Create an empty file for notepad to open
//     FILE* fp = _wfopen(L"game_output.txt", L"w");
//     if (fp) {
//         ignore fwprintf(fp, L"Starting...\n");
//         ignore fclose(fp);
//     }

//     // Launch notepad
//     HINSTANCE result = ShellExecuteW(
//         NULL,
//         L"open",
//         L"notepad.exe",
//         L"game_output.txt",
//         NULL,
//         SW_SHOWNOACTIVATE
//     );

//     if ((INT_PTR)result <= 32) {
//         return_err(NotepadBackendErr_err(NotepadBackendErrType_FailedCreateNotepadProcess));
//     }

//     // Wait for the window to be created
//     HWND  notepad   = null;
//     DWORD startTime = GetTickCount();

//     while (GetTickCount() - startTime < 5000) {
//         notepad = FindWindowW(L"Notepad", null);
//         if (!notepad) {
//             notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
//         }

//         if (notepad) {
//             HWND edit = try(NotepadBackend_getNotepadEditControl(notepad));
//             unused(edit); // We just want to verify we can get it

//             // Configure window
//             SetWindowLongW(notepad, GWL_EXSTYLE, GetWindowLongW(notepad, GWL_EXSTYLE) | WS_EX_NOACTIVATE);

//             SetWindowPos(notepad, HWND_TOPMOST, 100, 100, 800, 600, SWP_SHOWWINDOW | SWP_NOACTIVATE);

//             return_ok(notepad);
//         }

//         Sleep(100);
//     }

//     return_err(NotepadBackendErr_err(NotepadBackendErrType_FailedGetNotepadEditControl));
// }

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

typedef struct {
    HWND* edit_hwnd;
    bool  found;
    int   depth;
} EnumData;

static void EnumChildWindowsRecursive(HWND hwnd, EnumData* data) {
    if (!hwnd || data->found) {
        return;
    }

    WCHAR class_name[256] = { 0 };
    GetClassNameW(hwnd, class_name, 256);

    // Print with indentation to show hierarchy
    for (int i = 0; i < data->depth; i++) {
        printf("  ");
    }
    printf("Window class: %ls (handle: %p)\n", class_name, (void*)hwnd);

    // Check if this is an edit control
    if (wcscmp(class_name, L"NotepadTextBox") == 0 || wcscmp(class_name, L"RichEditD2DPT") == 0) {
        *data->edit_hwnd = hwnd;
        data->found      = true;
        return;
    }

    // Recursively enumerate child windows
    data->depth++;
    HWND child = GetWindow(hwnd, GW_CHILD);
    while (child && !data->found) {
        EnumChildWindowsRecursive(child, data);
        child = GetWindow(child, GW_HWNDNEXT);
    }
    data->depth--;
}

static Err$HWND NotepadBackend_getNotepadEditControl(HWND notepad) {
    reserveReturn(Err$HWND);
    if (!notepad) {
        return_err(NotepadBackendErr_err(NotepadBackendErrType_InvalidArgument));
    }

    // Print parent window info
    WCHAR class_name[256]  = { 0 };
    WCHAR window_text[256] = { 0 };
    GetClassNameW(notepad, class_name, 256);
    GetWindowTextW(notepad, window_text, 256);
    printf("\nSearching for edit control in window:\n");
    printf("Parent class: %ls\n", class_name);
    printf("Parent text: %ls\n", window_text);

    HWND     edit_hwnd = NULL;
    EnumData data      = {
             .edit_hwnd = &edit_hwnd,
             .found     = false,
             .depth     = 0
    };

    printf("Enumerating child windows:\n");
    EnumChildWindowsRecursive(notepad, &data);

    if (!data.found || !edit_hwnd) {
        printf("Could not find edit control through enumeration\n");

        // Try direct approach one more time
        HWND direct_edit = NULL;
        HWND parent      = notepad;
        int  attempts    = 0;

        while (parent && attempts < 10) {
            direct_edit = FindWindowExW(parent, NULL, L"NotepadTextBox", NULL);
            if (!direct_edit) {
                direct_edit = FindWindowExW(parent, NULL, L"RichEditD2DPT", NULL);
            }
            if (direct_edit) {
                break;
            }

            parent = GetWindow(parent, GW_CHILD);
            attempts++;
        }

        if (direct_edit) {
            printf("Found edit control through direct search: %p\n", (void*)direct_edit);
            return_ok(direct_edit);
        }

        return_err(NotepadBackendErr_err(NotepadBackendErrType_FailedGetNotepadEditControl));
    }

    printf("Found edit control: %p\n", (void*)edit_hwnd);
    return_ok(edit_hwnd);
}

// static Err$void NotepadBackend_sendTextToNotepad(HWND notepad, const wchar* text) {
//     reserveReturn(Err$void);
//     if (!notepad || !text) {
//         return_err(NotepadBackendErr_err(NotepadBackendErrType_InvalidArgument));
//     }

//     HWND edit = try(NotepadBackend_getNotepadEditControl(notepad));

//     // Try different message types that might work
//     BOOL result = FALSE;

//     // Method 1: WM_SETTEXT
//     result = (BOOL)SendMessageW(edit, WM_SETTEXT, 0, (LPARAM)text);
//     if (!result) {
//         // Method 2: EM_REPLACESEL
//         result = (BOOL)SendMessageW(edit, EM_REPLACESEL, TRUE, (LPARAM)text);
//     }
//     if (!result) {
//         // Method 3: EM_SETTEXTEX
//         SETTEXTEX stx = {
//             .flags    = ST_DEFAULT,
//             .codepage = CP_ACP
//         };
//         result = SendMessageW(edit, EM_SETTEXTEX, (WPARAM)&stx, (LPARAM)text);
//     }

//     if (!result) {
//         printf("Failed to send text to notepad. Error: %lu\n", GetLastError());
//         return_err(NotepadBackendErr_err(NotepadBackendErrType_CantSendTextToNotepad));
//     }

//     return_ok({});
// }

static Err$void NotepadBackend_sendTextToNotepad(HWND notepad, const wchar* text) {
    reserveReturn(Err$void);
    if (!notepad || !text) {
        return_err(NotepadBackendErr_err(NotepadBackendErrType_InvalidArgument));
    }

    HWND edit = try(NotepadBackend_getNotepadEditControl(notepad));

    // Clear first
    SendMessageW(edit, WM_SETTEXT, 0, (LPARAM)L"");
    Sleep(1); // Small delay

    // Then set new text
    if (!SendMessageW(edit, WM_SETTEXT, 0, (LPARAM)text)) {
        printf("Failed to send text to notepad. Error: %lu\n", GetLastError());
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
