#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

// Global state
static HWND g_terminalWindow = NULL;
static DWORD g_terminalPid = 0;
static HHOOK g_keyboardHook = NULL;
static HHOOK g_mouseHook = NULL;
static HWINEVENTHOOK g_winEventHook = NULL;
static volatile BOOL g_running = TRUE;

// Forward declarations
BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam);
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);
void CALLBACK WinEventCallback(
    HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd,
    LONG idObject, LONG idChild, DWORD idEventThread, DWORD dwmsEventTime
);
BOOL WINAPI ConsoleCtrlHandler(DWORD ctrlType);

typedef struct {
    DWORD targetPid;
    HWND foundWindow;
} EnumWindowsData;

// Find Windows Terminal window by process ID
HWND FindTerminalWindow(DWORD pid) {
    EnumWindowsData data = { .targetPid = pid, .foundWindow = NULL };

    // First try direct PID
    EnumWindows(EnumWindowsCallback, (LPARAM)&data);
    if (data.foundWindow) {
        return data.foundWindow;
    }

    // Windows Terminal spawns child processes, check those
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    PROCESSENTRY32W pe = { .dwSize = sizeof(pe) };
    if (Process32FirstW(snapshot, &pe)) {
        do {
            // Check if this is a child of our spawned process
            if (pe.th32ParentProcessID == pid) {
                data.targetPid = pe.th32ProcessID;
                EnumWindows(EnumWindowsCallback, (LPARAM)&data);
                if (data.foundWindow) {
                    g_terminalPid = pe.th32ProcessID;
                    break;
                }
            }
        } while (Process32NextW(snapshot, &pe));
    }

    CloseHandle(snapshot);
    return data.foundWindow;
}

BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam) {
    EnumWindowsData* data = (EnumWindowsData*)lParam;
    DWORD windowPid;

    GetWindowThreadProcessId(hwnd, &windowPid);
    if (windowPid != data->targetPid) {
        return TRUE;
    }

    // Check for Windows Terminal window class
    char className[256];
    if (GetClassNameA(hwnd, className, sizeof(className))) {
        // Windows Terminal uses this class name
        if (strstr(className, "CASCADIA_HOSTING_WINDOW_CLASS") || strstr(className, "Windows.UI.Core.CoreWindow")) {

            // Verify it's visible and has a title
            if (IsWindowVisible(hwnd)) {
                char title[256];
                if (GetWindowTextA(hwnd, title, sizeof(title)) > 0) {
                    data->foundWindow = hwnd;
                    return FALSE; // Stop enumeration
                }
            }
        }
    }
    return TRUE;
}

// Keyboard hook - captures all keyboard input when WT is focused
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* kbData = (KBDLLHOOKSTRUCT*)lParam;

        // Only process when our terminal window is in focus
        if (GetForegroundWindow() == g_terminalWindow) {
            const char* eventType = "";
            switch (wParam) {
            case WM_KEYDOWN:
                eventType = "KEY_DOWN";
                break;
            case WM_KEYUP:
                eventType = "KEY_UP";
                break;
            case WM_SYSKEYDOWN:
                eventType = "SYSKEY_DOWN";
                break;
            case WM_SYSKEYUP:
                eventType = "SYSKEY_UP";
                break;
            }

            // Get key name
            char keyName[64] = { 0 };
            UINT scanCode = MapVirtualKeyA(kbData->vkCode, MAPVK_VK_TO_VSC);
            GetKeyNameTextA(scanCode << 16, keyName, sizeof(keyName));

            printf("[KEYBOARD] %s: VK=0x%02lX Scan=0x%02lX Name='%s'", eventType, kbData->vkCode, kbData->scanCode, keyName);

            // Show modifier state
            if (kbData->flags & LLKHF_ALTDOWN) {
                printf(" [ALT]");
            }
            if (kbData->flags & LLKHF_EXTENDED) {
                printf(" [EXT]");
            }
            if (GetKeyState(VK_CONTROL) & 0x8000) {
                printf(" [CTRL]");
            }
            if (GetKeyState(VK_SHIFT) & 0x8000) {
                printf(" [SHIFT]");
            }
            printf("\n");

            // Example: Block specific keys (uncomment to test)
            // if (kbData->vkCode == VK_F12) return 1;  // Block F12
        }
    }
    return CallNextHookEx(g_keyboardHook, nCode, wParam, lParam);
}

// Mouse hook - captures mouse events over the terminal window
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        MSLLHOOKSTRUCT* msData = (MSLLHOOKSTRUCT*)lParam;

        // Check if mouse is over our terminal window
        POINT pt = msData->pt;
        HWND hwndUnderCursor = WindowFromPoint(pt);

        // Check if it's our window or a child of it
        if (hwndUnderCursor == g_terminalWindow || GetAncestor(hwndUnderCursor, GA_ROOT) == g_terminalWindow) {

            // Convert to client coordinates
            POINT clientPt = pt;
            ScreenToClient(g_terminalWindow, &clientPt);

            const char* eventType = NULL;
            switch (wParam) {
            case WM_LBUTTONDOWN:
                eventType = "LEFT_DOWN";
                break;
            case WM_LBUTTONUP:
                eventType = "LEFT_UP";
                break;
            case WM_RBUTTONDOWN:
                eventType = "RIGHT_DOWN";
                break;
            case WM_RBUTTONUP:
                eventType = "RIGHT_UP";
                break;
            case WM_MBUTTONDOWN:
                eventType = "MIDDLE_DOWN";
                break;
            case WM_MBUTTONUP:
                eventType = "MIDDLE_UP";
                break;
            case WM_MOUSEWHEEL: {
                short delta = (short)HIWORD(msData->mouseData);
                printf("[MOUSE] WHEEL: delta=%d at client(%ld, %ld)\n", delta, clientPt.x, clientPt.y);
                break;
            }
            case WM_MOUSEMOVE:
                // Uncomment to see mouse movement (very verbose!)
                // printf("[MOUSE] MOVE: client(%ld, %ld)\n", clientPt.x, clientPt.y);
                break;
            }

            if (eventType) {
                printf("[MOUSE] %s at client(%ld, %ld) screen(%ld, %ld)\n", eventType, clientPt.x, clientPt.y, pt.x, pt.y);
            }
        }
    }
    return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
}

// Window event hook - captures resize, move, focus changes
void CALLBACK WinEventCallback(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD idEventThread, DWORD dwmsEventTime) {
    // Only handle events for our window
    if (hwnd != g_terminalWindow) {
        return;
    }
    if (idObject != OBJID_WINDOW) {
        return;
    }

    switch (event) {
    case EVENT_OBJECT_LOCATIONCHANGE: {
        RECT windowRect, clientRect;
        GetWindowRect(hwnd, &windowRect);
        GetClientRect(hwnd, &clientRect);

        printf("[WINDOW] RESIZE/MOVE: Window(%ldx%ld) Client(%ldx%ld) Position(%ld, %ld)\n", windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, windowRect.left, windowRect.top);
        break;
    }
    case EVENT_SYSTEM_FOREGROUND:
        printf("[WINDOW] ACTIVATED (got focus)\n");
        break;
    case EVENT_OBJECT_FOCUS:
        printf("[WINDOW] FOCUS changed\n");
        break;
    case EVENT_SYSTEM_MINIMIZESTART:
        printf("[WINDOW] MINIMIZED\n");
        break;
    case EVENT_SYSTEM_MINIMIZEEND:
        printf("[WINDOW] RESTORED from minimize\n");
        break;
    }
}

// Ctrl+C handler
BOOL WINAPI ConsoleCtrlHandler(DWORD ctrlType) {
    if (ctrlType == CTRL_C_EVENT || ctrlType == CTRL_BREAK_EVENT) {
        printf("\nShutting down...\n");
        g_running = FALSE;
        PostQuitMessage(0);
        return TRUE;
    }
    return FALSE;
}

void Cleanup(PROCESS_INFORMATION* pi) {
    if (g_keyboardHook) {
        UnhookWindowsHookEx(g_keyboardHook);
        g_keyboardHook = NULL;
    }
    if (g_mouseHook) {
        UnhookWindowsHookEx(g_mouseHook);
        g_mouseHook = NULL;
    }
    if (g_winEventHook) {
        UnhookWinEvent(g_winEventHook);
        g_winEventHook = NULL;
    }
    if (pi) {
        CloseHandle(pi->hThread);
        CloseHandle(pi->hProcess);
    }
}

int main(void) {
    PROCESS_INFORMATION pi = { 0 };
    STARTUPINFOW si = { .cb = sizeof(si) };

    printf("=== Windows Terminal Event Monitor ===\n\n");

    // Set up Ctrl+C handler
    SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

    // Spawn Windows Terminal
    printf("[*] Spawning Windows Terminal...\n");

    wchar_t cmdLine[] = L"wt.exe";
    if (!CreateProcessW(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("[!] Failed to spawn wt.exe (error: %lu)\n", GetLastError());
        printf("    Make sure Windows Terminal is installed.\n");
        return 1;
    }

    printf("[+] Process created (PID: %lu)\n", pi.dwProcessId);
    printf("[*] Waiting for window to appear...\n");

    // Wait for the window to be created
    // Windows Terminal has a complex startup, may need a few seconds
    for (int attempt = 0; attempt < 20; attempt++) {
        Sleep(250);
        g_terminalWindow = FindTerminalWindow(pi.dwProcessId);
        if (g_terminalWindow) {
            break;
        }
        printf(".");
    }
    printf("\n");

    if (!g_terminalWindow) {
        printf("[!] Could not find Windows Terminal window\n");
        printf("    The window might be using a different class name.\n");
        Cleanup(&pi);
        return 1;
    }

    // Get window info
    char windowTitle[256];
    GetWindowTextA(g_terminalWindow, windowTitle, sizeof(windowTitle));
    printf("[+] Found window: HWND=0x%p PID=%lu Title='%s'\n", (void*)g_terminalWindow, g_terminalPid, windowTitle);

    // Install keyboard hook (global, low-level)
    printf("[*] Installing keyboard hook...\n");
    g_keyboardHook = SetWindowsHookExA(WH_KEYBOARD_LL, KeyboardHookProc, NULL, 0);
    if (!g_keyboardHook) {
        printf("[!] Failed to install keyboard hook (error: %lu)\n", GetLastError());
    } else {
        printf("[+] Keyboard hook installed\n");
    }

    // Install mouse hook (global, low-level)
    printf("[*] Installing mouse hook...\n");
    g_mouseHook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);
    if (!g_mouseHook) {
        printf("[!] Failed to install mouse hook (error: %lu)\n", GetLastError());
    } else {
        printf("[+] Mouse hook installed\n");
    }

    // Install window event hook (for resize, focus, etc.)
    printf("[*] Installing window event hook...\n");
    g_winEventHook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND,     // Min event
        EVENT_OBJECT_LOCATIONCHANGE, // Max event
        NULL,                        // No DLL
        WinEventCallback,
        g_terminalPid, // Specific process
        0,             // All threads
        WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
    );
    if (!g_winEventHook) {
        printf("[!] Failed to install window event hook (error: %lu)\n", GetLastError());
    } else {
        printf("[+] Window event hook installed\n");
    }

    printf("\n=== Monitoring Started ===\n");
    printf("- Click on Windows Terminal and type to see keyboard events\n");
    printf("- Move/click mouse over the terminal to see mouse events\n");
    printf("- Resize or move the window to see window events\n");
    printf("- Press Ctrl+C in THIS window to stop monitoring\n\n");

    // Message loop - required for hooks to work
    MSG msg;
    while (g_running && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    printf("\n[*] Cleaning up...\n");
    Cleanup(&pi);
    printf("[+] Done.\n");

    return 0;
}
