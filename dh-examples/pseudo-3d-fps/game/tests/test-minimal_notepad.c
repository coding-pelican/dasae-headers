#include <windows.h>
#include <stdio.h>

// Some debug printing macros for convenience
#define PRINT_LAST_ERROR(msg)                                       \
    do {                                                            \
        DWORD e_ = GetLastError();                                  \
        fprintf(stderr, "%s: error %lu\n", msg, (unsigned long)e_); \
    } while (0)

// Wait up to 5 seconds for Notepad to appear, find the main window, and get the edit child.
// Returns the HWND of the main Notepad window, or NULL on failure.
HWND LaunchAndFindNotepad(void) {
    STARTUPINFOW        si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    // 1) Launch notepad.exe with no arguments
    if (!CreateProcessW(L"notepad.exe", // Application name
                        NULL,           // Command line
                        NULL,
                        NULL,  // Process & thread security
                        FALSE, // Inherit handles
                        0,     // Creation flags
                        NULL,  // Environment
                        NULL,  // Current directory
                        &si,
                        &pi)) {
        PRINT_LAST_ERROR("CreateProcessW(notepad.exe) failed");
        return NULL;
    }

    // 2) Wait for Notepad's window to appear (up to 5 seconds)
    HWND        notepadHwnd = NULL;
    const DWORD startTime   = GetTickCount();
    for (;;) {
        // Try the classic Notepad class first
        notepadHwnd = FindWindowW(L"Notepad", NULL);
        if (!notepadHwnd) {
            // On Windows 10/11 store-based Notepad, it's sometimes "ApplicationFrameWindow"
            notepadHwnd = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
        }
        if (notepadHwnd) {
            break; // We found a matching window
        }
        if ((GetTickCount() - startTime) > 5000) {
            // Timed out after 5 seconds
            fprintf(stderr, "Timed out: Notepad main window not found.\n");
            break;
        }
        Sleep(100);
    }

    // Close the process/thread handles; Notepad keeps running
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return notepadHwnd;
}

// Find the Edit or RichEdit control in the given Notepad window
HWND FindNotepadEditControl(HWND notepadHwnd) {
    if (!IsWindow(notepadHwnd)) {
        fprintf(stderr, "Invalid Notepad window handle.\n");
        return NULL;
    }

    // We try multiple class names:
    HWND edit = FindWindowExW(notepadHwnd, NULL, L"Edit", NULL);
    if (!edit) {
        edit = FindWindowExW(notepadHwnd, NULL, L"RichEditD2DPT", NULL);
    }
    if (!edit) {
        edit = FindWindowExW(notepadHwnd, NULL, L"RICHEDIT50W", NULL);
    }
    if (!edit) {
        fprintf(stderr, "Could not find an Edit/RichEdit control in Notepad.\n");
    }
    return edit;
}

int main(void) {
    printf("Launching Notepad (no arguments)...\n");
    HWND notepadHwnd = LaunchAndFindNotepad();
    if (!notepadHwnd) {
        fprintf(stderr, "Failed to launch or find Notepad window.\n");
        return 1;
    }

    // Optional: bring the window to the front and resize
    SetWindowPos(notepadHwnd, HWND_TOPMOST, 100, 100, 800, 600, SWP_SHOWWINDOW);

    // 3) Find the Edit control
    HWND edit = FindNotepadEditControl(notepadHwnd);
    if (!edit) {
        fprintf(stderr, "Unable to proceed without Edit control.\n");
        return 2;
    }

    // 4) Optionally set a monospaced font
    HFONT hFont = CreateFontW(
        14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, (FIXED_PITCH | FF_MODERN), L"Consolas"
    );
    SendMessageW(edit, WM_SETFONT, (WPARAM)hFont, TRUE);

    // 5) Mark the Edit control read-only so the user can't modify it
    SendMessageW(edit, EM_SETREADONLY, TRUE, 0);

    // 6) Now set some text directly into Notepad
    const wchar_t* textToShow = L"Hello from ChatGPT!\r\n"
                                L"This Notepad is read-only.\r\n"
                                L"No user edits allowed.\r\n";
    if (!SetWindowTextW(edit, textToShow)) {
        PRINT_LAST_ERROR("SetWindowTextW failed");
    } else {
        printf("Successfully wrote text to Notepad's edit control.\n");
    }

    // 7) Keep the console open so we can see the messages
    printf("Notepad should be visible. Press ENTER to quit.\n");
    getchar();

    return 0;
}
