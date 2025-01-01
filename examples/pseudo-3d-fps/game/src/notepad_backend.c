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

impl_Err(
    engine_NotepadBackendErr,
    FailedCreateNotepadWindow,
    NotFoundNotepadWindow,
    FailedAllocateBuffer
);

static HWND g_consoleWindow = NULL;

static void NotepadBackend_setNotepadWindow(void) {
    if (!g_consoleWindow) {
        g_consoleWindow = GetConsoleWindow();
    }

    // Launch notepad if it doesn't exist
    HWND notepad = FindWindowW(L"Notepad", NULL);
    if (!notepad) {
        notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
    }

    if (!notepad) {
        ShellExecuteW(NULL, L"open", L"notepad.exe", NULL, NULL, SW_SHOWNORMAL);
        Sleep(100); // Give it time to start
        notepad = FindWindowW(L"Notepad", NULL);
        if (!notepad) {
            notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
        }
    }

    if (notepad) {
        HWND edit = FindWindowExW(notepad, NULL, L"Edit", NULL);
        if (!edit) {
            edit = FindWindowExW(notepad, NULL, L"RichEditD2DPT", NULL);
        }
        if (!edit) {
            edit = FindWindowExW(notepad, NULL, L"RICHEDIT50W", NULL);
        }

        if (edit) {
            // Configure window and edit control
            HFONT hFont = CreateFontW(
                8,                       // Height
                0,                       // Width
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
                L"D2CodingLigature Nerd Font"
            );
            SendMessageW(edit, WM_SETFONT, (WPARAM)hFont, TRUE);

            // Make window always on top but not focusable
            SetWindowLongW(notepad, GWL_EXSTYLE, GetWindowLongW(notepad, GWL_EXSTYLE) | WS_EX_NOACTIVATE | WS_EX_TOPMOST);

            // Position window
            SetWindowPos(notepad, HWND_TOPMOST, 100, 100, 800, 600, SWP_SHOWWINDOW | SWP_NOACTIVATE);
        }
    }
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

static const wchar_t SHADE_CHARS[] = L" .',:!*|+%8#@█"; // More granular shading
#define SHADE_LEVELS (sizeof(SHADE_CHARS) / sizeof(wchar_t) - 1)

static inline u8 get_grayscale(const Color* color) {
    // Using precomputed coefficients (R*0.299 + G*0.587 + B*0.114)
    // Multiplied by 256 for fixed-point arithmetic
    return (u8)((color->r * 77 + color->g * 150 + color->b * 29) >> 8);
}

static void NotepadBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height) {
    debug_assert_nonnull(platform);
    debug_assert_nonnull(data);
    engine_NotepadBackend* backend = (engine_NotepadBackend*)platform->backend;

    // Convert frame to ASCII
    backend->buffer_size = 0;
    for (u32 y = 0; y < height; y += 2) {
        for (u32 x = 0; x < width; ++x) {
            const Color* upper = &data[y * width + x];
            const Color* lower = (y + 1 < height) ? &data[(y + 1) * width + x] : upper;

            u8 avg_intensity                        = (get_grayscale(upper) + get_grayscale(lower)) >> 1;
            u8 shade_idx                            = (avg_intensity * (SHADE_LEVELS - 1)) >> 8;
            backend->buffer[backend->buffer_size++] = SHADE_CHARS[shade_idx];
        }
        backend->buffer[backend->buffer_size++] = L'\r';
        backend->buffer[backend->buffer_size++] = L'\n';
    }
    backend->buffer[backend->buffer_size] = L'\0';

    // Write to file (async if needed)
    HANDLE hFile = CreateFileW(
        L"game_output.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, // Added for immediate disk write
        NULL
    );

    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD written = 0;
        WriteFile(hFile, backend->buffer, backend->buffer_size * sizeof(wchar), &written, NULL);
        CloseHandle(hFile);
    }

    // Direct update to Notepad
    HWND notepad = FindWindowW(L"Notepad", NULL);
    if (!notepad) {
        notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
    }
    if (!BringWindowToTop(notepad)) {
        printf("notepad: BringWindowToTop failed\n");
    }
    if (!BringWindowToTop(notepad)) {
        printf("notepad: BringWindowToTop failed\n");
    }

    HWND terminal = FindWindowW(L"main-notepad-debug", NULL);
    if (!terminal) {
        terminal = FindWindowW(L"ApplicationFrameWindow", L"main-notepad-debug");
    }
    if (!BringWindowToTop(terminal)) {
        printf("terminal: BringWindowToTop failed\n");
    }
    if (!BringWindowToTop(terminal)) {
        printf("terminal: BringWindowToTop failed\n");
    }

    if (!notepad) {
        NotepadBackend_setNotepadWindow();
        return;
    }

    // unreachable ...
    HWND edit = FindWindowExW(notepad, NULL, L"Edit", NULL);
    if (!edit) {
        edit = FindWindowExW(notepad, NULL, L"RichEditD2DPT", NULL);
    }
    if (!edit) {
        edit = FindWindowExW(notepad, NULL, L"RICHEDIT50W", NULL);
    }

    if (edit) {
        SendMessageW(edit, WM_SETTEXT, 0, (LPARAM)backend->buffer);
    }
}

Err$Ptr$engine_RenderBackend NotepadBackend_create(void) {
    reserveReturn(Err$Ptr$engine_RenderBackend);

    let backend = (engine_NotepadBackend*)malloc(sizeof(engine_NotepadBackend));
    if (!backend) {
        return_err(engine_NotepadBackendErr_err(engine_NotepadBackendErrType_FailedCreateNotepadWindow));
    }
    memset(backend, 0, sizeof(engine_NotepadBackend));

    let capacity = 1024ull * 1024ull;
    let buffer   = (wchar*)malloc(capacity * sizeof(wchar));
    if (!buffer) {
        free(backend);
        return_err(engine_NotepadBackendErr_err(engine_NotepadBackendErrType_FailedAllocateBuffer));
    }

    backend->buffer          = buffer;
    backend->buffer_capacity = capacity;
    backend->buffer_size     = 0;

    backend->base.type          = engine_RenderBackendType_custom;
    backend->base.presentBuffer = NotepadBackend_presentBuffer;
    backend->base.processEvents = NULL;
    backend->base.destroy       = NotepadBackend_destroy;
    NotepadBackend_setNotepadWindow();

    return_ok(&backend->base);
}

// static void NotepadBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height) {
//     debug_assert_nonnull(platform);
//     debug_assert_nonnull(data);
//     engine_NotepadBackend* backend = (engine_NotepadBackend*)platform->backend;

//     // Find Notepad window
//     HWND notepad = NotepadBackend_findNotepadWindow();
//     if (!notepad) {
//         NotepadBackend_setNotepadWindow();
//         return;
//     }

//     // Find edit control
//     HWND edit = FindWindowExW(notepad, NULL, L"Edit", NULL);
//     if (!edit) {
//         edit = FindWindowExW(notepad, NULL, L"RichEditD2DPT", NULL);
//     }
//     if (!edit) {
//         edit = FindWindowExW(notepad, NULL, L"RICHEDIT50W", NULL);
//     }

//     if (!edit) {
//         printf("Failed to find edit control\n");
//         return;
//     }

//     // Create ASCII art
//     backend->buffer_size = 0;
//     for (u32 y = 0; y < height; y += 2) {
//         for (u32 x = 0; x < width; ++x) {
//             const Color* upper = &data[y * width + x];
//             const Color* lower = (y + 1 < height) ? &data[(y + 1) * width + x] : upper;

//             u8 avg_intensity                        = (get_grayscale(upper) + get_grayscale(lower)) >> 1;
//             u8 shade_idx                            = (avg_intensity * (SHADE_LEVELS - 1)) >> 8;
//             backend->buffer[backend->buffer_size++] = SHADE_CHARS[shade_idx];
//         }
//         backend->buffer[backend->buffer_size++] = L'\r';
//         backend->buffer[backend->buffer_size++] = L'\n';
//     }
//     backend->buffer[backend->buffer_size] = L'\0';

//     // Update Notepad content with debug info
//     static int frameCount = 0;
//     if (++frameCount % 60 == 0) { // Print debug info every 60 frames
//         NotepadBackend_printDebugInfo(notepad, edit);
//     }

//     // Send the content to Notepad
//     SendMessageW(edit, WM_SETTEXT, 0, (LPARAM)backend->buffer);
// }

// Err$Ptr$engine_RenderBackend NotepadBackend_create(void) {
//     reserveReturn(Err$Ptr$engine_RenderBackend);

//     let backend = (engine_NotepadBackend*)malloc(sizeof(engine_NotepadBackend));
//     if (!backend) {
//         return_err(engine_NotepadBackendErr_err(engine_NotepadBackendErrType_FailedCreateNotepadWindow));
//     }
//     memset(backend, 0, sizeof(engine_NotepadBackend));

//     let capacity = 1024ull * 1024ull;
//     let buffer   = (wchar*)malloc(capacity * sizeof(wchar));
//     if (!buffer) {
//         free(backend);
//         return_err(engine_NotepadBackendErr_err(engine_NotepadBackendErrType_FailedAllocateBuffer));
//     }

//     backend->buffer          = buffer;
//     backend->buffer_capacity = capacity;
//     backend->buffer_size     = 0;

//     backend->base.type          = engine_RenderBackendType_custom;
//     backend->base.presentBuffer = NotepadBackend_presentBuffer;
//     backend->base.processEvents = NULL;
//     backend->base.destroy       = NotepadBackend_destroy;

//     NotepadBackend_setNotepadWindow();
//     return_ok(&backend->base);
// }

// Err$Ptr$engine_RenderBackend NotepadBackend_create(void) {
//     reserveReturn(Err$Ptr$engine_RenderBackend);

//     // Allocate backend structure
//     let backend = (engine_NotepadBackend*)malloc(sizeof(engine_NotepadBackend));
//     if (!backend) {
//         return_err(engine_NotepadBackendErr_err(engine_NotepadBackendErrType_FailedCreateNotepadWindow));
//     }

//     // Initialize to zero
//     memset(backend, 0, sizeof(engine_NotepadBackend));

//     // Find or create notepad window
//     backend->notepad_handle = NotepadBackend_findOrCreateNotepad();
//     if (!backend->notepad_handle) {
//         free(backend);
//         return_err(engine_NotepadBackendErr_err(engine_NotepadBackendErrType_NotFoundNotepadWindow));
//     }

//     // Allocate text buffer
//     let capacity = 1024ull * 1024ull; // 2MB initial
//     let buffer   = malloc(capacity * sizeof(wchar));
//     if (!buffer) {
//         free(backend);
//         return_err(engine_NotepadBackendErr_err(engine_NotepadBackendErrType_FailedAllocateBuffer));
//     }

//     // Initialize buffer state
//     backend->buffer          = buffer; // Add this line to assign the allocated buffer
//     backend->buffer_capacity = capacity;
//     backend->buffer_size     = 0;

//     // Setup render backend interface
//     backend->base.type          = engine_RenderBackendType_custom;
//     backend->base.presentBuffer = NotepadBackend_presentBuffer;
//     backend->base.processEvents = NotepadBackend_processEvents;
//     backend->base.destroy       = NotepadBackend_destroy;

//     return_ok(&backend->base);
// }



// static void NotepadBackend_debug(engine_NotepadBackend* backend) {
//     printf("Notepad handle: %p\n", (void*)backend->notepad_handle);
//     HWND edit = FindWindowEx(backend->notepad_handle, null, "Edit", null);
//     printf("Edit control handle: %p\n", (void*)edit);
//     printf("Buffer size: %zu\n", backend->buffer_size);
//     printf("First few characters: ");
//     for (usize i = 0; i < 10 && i < backend->buffer_size; ++i) {
//         printf("%c", backend->buffer[i]);
//     }
//     printf("...\n");

//     // Check for last error
//     DWORD error = GetLastError();
//     if (error != 0) {
//         char errorMsg[256] = cleared();
//         FormatMessageA(
//             FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
//             null,
//             error,
//             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//             errorMsg,
//             sizeof(errorMsg),
//             null
//         );
//         printf("Last error: %lu - %s\n", error, errorMsg);
//     }
// }

// static HWND NotepadBackend_findOrCreateNotepad(void) {
//     // Try finding existing Notepad window first
//     HWND notepad = FindWindowW(L"Notepad", null); // Try classic way
//     if (!notepad) {
//         notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad"); // Try Win11 way
//     }

//     if (notepad) {
//         // For Windows 11, we need to find the actual edit control differently
//         HWND edit = null;

//         // Try different approaches to find the edit control
//         edit = FindWindowExW(notepad, null, L"Edit", null); // Classic
//         if (!edit) {
//             edit = FindWindowExW(notepad, null, L"RichEditD2DPT", null); // Win11
//         }
//         if (!edit) {
//             edit = FindWindowExW(notepad, null, L"RICHEDIT50W", null); // Another possibility
//         }

//         if (edit) {
//             printf("Found existing notepad with edit control\n");
//             return notepad;
//         }
//     }

//     // Launch new Notepad process
//     WCHAR systemDir[MAX_PATH];
//     GetSystemDirectoryW(systemDir, MAX_PATH);
//     WCHAR notepadPath[MAX_PATH];
//     _snwprintf(notepadPath, MAX_PATH, L"%s\\notepad.exe", systemDir);

//     STARTUPINFOW        si = { 0 };
//     PROCESS_INFORMATION pi = { 0 };
//     si.cb                  = sizeof(si);

//     if (!CreateProcessW(
//             notepadPath,
//             null,
//             null,
//             null,
//             false,
//             0,
//             null,
//             null,
//             &si,
//             &pi
//         )) {
//         printf("Failed to create notepad process: %lu\n", GetLastError());
//         return null;
//     }

//     // Wait for the window with message pump
//     notepad         = null;
//     HWND  edit      = null;
//     DWORD startTime = GetTickCount();

//     while (GetTickCount() - startTime < 5000) { // 5 second timeout
//         // Try both classic and Win11 window classes
//         notepad = FindWindowW(L"Notepad", null);
//         if (!notepad) {
//             notepad = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
//         }

//         if (notepad) {
//             // Try different edit control classes
//             edit = FindWindowExW(notepad, null, L"Edit", null);
//             if (!edit) {
//                 edit = FindWindowExW(notepad, null, L"RichEditD2DPT", null);
//             }
//             if (!edit) {
//                 edit = FindWindowExW(notepad, null, L"RICHEDIT50W", null);
//             }

//             if (edit) {
//                 printf("Successfully created new notepad with edit control\n");
//                 break;
//             }
//         }

//         Sleep(100);
//         MSG msg;
//         while (PeekMessageW(&msg, null, 0, 0, PM_REMOVE)) {
//             TranslateMessage(&msg);
//             DispatchMessageW(&msg);
//         }
//     }

//     CloseHandle(pi.hProcess);
//     CloseHandle(pi.hThread);

//     if (!notepad || !edit) {
//         printf("Failed to find notepad window or edit control after creation\n");
//         return null;
//     }

//     // Configure window
//     ShowWindow(notepad, SW_SHOWNORMAL);
//     SetForegroundWindow(notepad);

//     printf("Notepad handle: %p, Edit handle: %p\n", (void*)notepad, (void*)edit);
//     return notepad;
// }

// static void NotepadBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height) {
//     debug_assert_nonnull(platform);
//     debug_assert_nonnull(data);
//     engine_NotepadBackend* backend = (engine_NotepadBackend*)platform->backend;
//     const Color*           pixels  = data;

//     // Get Edit control with multiple attempts
//     HWND edit = FindWindowExW(backend->notepad_handle, null, L"Edit", null);
//     if (!edit) {
//         edit = FindWindowExW(backend->notepad_handle, null, L"RichEditD2DPT", null);
//     }
//     if (!edit) {
//         edit = FindWindowExW(backend->notepad_handle, null, L"RICHEDIT50W", null);
//     }

//     if (!edit) {
//         printf("Could not find Edit control!\n");
//         return;
//     }

//     // Reset buffer
//     backend->buffer_size = 0;

//     // Write a simple test pattern first
//     const char test[] = "Test Pattern\r\n";
//     memcpy(backend->buffer, test, sizeof(test));
//     backend->buffer_size                  = sizeof(test) - 1;
//     backend->buffer[backend->buffer_size] = '\0';

//     // Try multiple ways to set the text
//     BOOL result = false;

//     // Method 1: Direct SetWindowText
//     result = SetWindowTextW(edit, L"Test Pattern\r\n");
//     if (!result) {
//         printf("SetWindowTextW failed: %lu\n", GetLastError());
//     }

//     // Method 2: Send WM_SETTEXT message
//     if (!result) {
//         result = (BOOL)SendMessageW(edit, WM_SETTEXT, 0, (LPARAM)L"Test Pattern\r\n");
//         if (!result) {
//             printf("SendMessage WM_SETTEXT failed: %lu\n", GetLastError());
//         }
//     }

//     // Force redraw
//     InvalidateRect(edit, null, TRUE);
//     UpdateWindow(edit);
// }

// static wchar NotepadBackend_getShadeChar(const Color* upper, const Color* lower) {
//     // Calculate weighted grayscale values for both pixels
//     u8 upper_intensity = (u8)(((f32)upper->r * 0.299f + (f32)upper->g * 0.587f + (f32)upper->b * 0.114f));
//     u8 lower_intensity = (u8)(((f32)lower->r * 0.299f + (f32)lower->g * 0.587f + (f32)lower->b * 0.114f));

//     // Average the intensities
//     u8 avg_intensity = (upper_intensity + lower_intensity) / 2;

//     // Extended ASCII shade mapping
//     if (avg_intensity > 230) {
//         return ' '; // Brightest (empty space)
//     }
//     if (avg_intensity > 200) {
//         return '.';
//     }
//     if (avg_intensity > 170) {
//         return '-';
//     }
//     if (avg_intensity > 140) {
//         return ':';
//     }
//     if (avg_intensity > 110) {
//         return '=';
//     }
//     if (avg_intensity > 80) {
//         return '+';
//     }
//     if (avg_intensity > 50) {
//         return '#';
//     }
//     if (avg_intensity > 20) {
//         return '@';
//     }
//     return L'█'; // Darkest
// }

// static void NotepadBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height) {
//     debug_assert_nonnull(platform);
//     debug_assert_nonnull(data);
//     let backend = (engine_NotepadBackend*)platform->backend;
//     let pixels  = data;

//     /* Reset buffer */
//     backend->buffer_size = 0;

//     // Convert pixel data to ASCII art
//     for (u32 y = 0; y < height; y += 2) {
//         for (u32 x = 0; x < width; ++x) {
//             const Color* upper = &pixels[y * width + x];
//             const Color* lower = (y + 1 < height) ? &pixels[(y + 1) * width + x] : upper;

//             let shade                               = NotepadBackend_getShadeChar(upper, lower);
//             backend->buffer[backend->buffer_size++] = shade;
//         }
//         backend->buffer[backend->buffer_size++] = '\r';
//         backend->buffer[backend->buffer_size++] = '\n';
//     }
//     backend->buffer[backend->buffer_size] = '\0'; // Ensure null termination

//     // Find the Edit control in Notepad
//     HWND edit = FindWindowEx(backend->notepad_handle, null, "Edit", null);
//     if (!edit) {
//         // If we can't find the edit control, try to find Notepad again
//         backend->notepad_handle = NotepadBackend_findOrCreateNotepad();
//         if (backend->notepad_handle) {
//             edit = FindWindowEx(backend->notepad_handle, null, "Edit", null);
//         }
//         if (!edit) {
//             return; // Still couldn't find it
//         }
//     }

//     // Try both methods of sending text
//     SetWindowText(edit, (char*)backend->buffer);               // Try direct method first
//     SendMessage(edit, WM_SETTEXT, 0, (LPARAM)backend->buffer); // Try SendMessage as backup

//     // Force a redraw
//     InvalidateRect(edit, null, true);
//     UpdateWindow(edit);

//     NotepadBackend_debug(backend);
// }

// static void NotepadBackend_processEvents(engine_Platform* platform) {
//     debug_assert_nonnull(platform);
//     engine_NotepadBackend* backend = (engine_NotepadBackend*)platform->backend;

//     // Check if notepad window still exists
//     if (!IsWindow(backend->notepad_handle)) {
//         backend->notepad_handle = NotepadBackend_findOrCreateNotepad();
//     }

//     // Process Windows messages
//     MSG msg = cleared();
//     while (PeekMessage(&msg, backend->notepad_handle, 0, 0, PM_REMOVE)) {
//         TranslateMessage(&msg);
//         DispatchMessage(&msg);
//     }
// }
