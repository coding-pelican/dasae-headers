#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <dwmapi.h>
#include <shlobj.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "gdi32.lib")

// ============================================================================
// Data Structures
// ============================================================================

typedef struct {
    int left, top, right, bottom;
} Padding;

typedef struct {
    char fontFace[128];
    int fontSize;
    Padding padding;
} TerminalSettings;

typedef struct {
    HWND mainWindow;
    HWND contentArea;
    DWORD processId;
    int tabBarHeight;
    int cellWidth;
    int cellHeight;
    TerminalSettings settings;
    int currentZoomLevel; // Tracks zoom adjustments
} TerminalContext;

typedef struct {
    int charX, charY;
    int pixelX, pixelY;
    BOOL inContentArea;
    BOOL inPadding;
} TerminalPosition;

// ============================================================================
// Global State
// ============================================================================

static TerminalContext g_ctx = { 0 };
static HHOOK g_keyboardHook = NULL;
static HHOOK g_mouseHook = NULL;
static HWINEVENTHOOK g_winEventHook = NULL;
static volatile BOOL g_running = TRUE;

// ============================================================================
// Simple JSON Parser (Minimal, for settings.json)
// ============================================================================

typedef struct {
    const char* data;
    size_t pos;
    size_t len;
} JsonParser;

static void json_skip_whitespace(JsonParser* p) {
    while (p->pos < p->len) {
        char c = p->data[p->pos];
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            p->pos++;
        } else if (c == '/' && p->pos + 1 < p->len && p->data[p->pos + 1] == '/') {
            // Skip line comment
            while (p->pos < p->len && p->data[p->pos] != '\n') {
                p->pos++;
            }
        } else {
            break;
        }
    }
}

static BOOL json_match(JsonParser* p, const char* str) {
    size_t len = strlen(str);
    if (p->pos + len <= p->len && strncmp(p->data + p->pos, str, len) == 0) {
        p->pos += len;
        return TRUE;
    }
    return FALSE;
}

static BOOL json_parse_string(JsonParser* p, char* out, size_t outSize) {
    json_skip_whitespace(p);
    if (p->pos >= p->len || p->data[p->pos] != '"') {
        return FALSE;
    }
    p->pos++; // Skip opening quote

    size_t start = p->pos;
    while (p->pos < p->len && p->data[p->pos] != '"') {
        if (p->data[p->pos] == '\\' && p->pos + 1 < p->len) {
            p->pos += 2; // Skip escape sequence
        } else {
            p->pos++;
        }
    }

    size_t len = p->pos - start;
    if (len >= outSize) {
        len = outSize - 1;
    }
    strncpy(out, p->data + start, len);
    out[len] = '\0';

    if (p->pos < p->len) {
        p->pos++; // Skip closing quote
    }
    return TRUE;
}

static BOOL json_parse_number(JsonParser* p, int* out) {
    json_skip_whitespace(p);
    if (p->pos >= p->len) {
        return FALSE;
    }

    char* end;
    *out = (int)strtol(p->data + p->pos, &end, 10);
    if (end == p->data + p->pos) {
        return FALSE;
    }
    p->pos = end - p->data;
    return TRUE;
}

static void json_skip_value(JsonParser* p) {
    json_skip_whitespace(p);
    if (p->pos >= p->len) {
        return;
    }

    char c = p->data[p->pos];
    if (c == '"') {
        char dummy[256];
        json_parse_string(p, dummy, sizeof(dummy));
    } else if (c == '{') {
        int depth = 1;
        p->pos++;
        while (p->pos < p->len && depth > 0) {
            if (p->data[p->pos] == '{') {
                depth++;
            } else if (p->data[p->pos] == '}') {
                depth--;
            } else if (p->data[p->pos] == '"') {
                char dummy[256];
                json_parse_string(p, dummy, sizeof(dummy));
                continue;
            }
            p->pos++;
        }
    } else if (c == '[') {
        int depth = 1;
        p->pos++;
        while (p->pos < p->len && depth > 0) {
            if (p->data[p->pos] == '[') {
                depth++;
            } else if (p->data[p->pos] == ']') {
                depth--;
            } else if (p->data[p->pos] == '"') {
                char dummy[256];
                json_parse_string(p, dummy, sizeof(dummy));
                continue;
            }
            p->pos++;
        }
    } else {
        // Number, bool, null
        while (p->pos < p->len) {
            c = p->data[p->pos];
            if (c == ',' || c == '}' || c == ']' || c == ' ' || c == '\n' || c == '\r') {
                break;
            }
            p->pos++;
        }
    }
}

static Padding parse_padding_string(const char* str) {
    Padding p = { 8, 8, 8, 8 }; // Default

    // Format can be: "8" or "8, 8, 8, 8" (left, top, right, bottom)
    int values[4];
    int count = sscanf(str, "%d , %d , %d , %d", &values[0], &values[1], &values[2], &values[3]);

    if (count == 1) {
        p.left = p.top = p.right = p.bottom = values[0];
    } else if (count == 4) {
        p.left = values[0];
        p.top = values[1];
        p.right = values[2];
        p.bottom = values[3];
    }

    return p;
}

static BOOL json_find_in_object(JsonParser* p, const char* key, char* strOut, size_t strOutSize, int* intOut) {
    json_skip_whitespace(p);
    if (p->pos >= p->len || p->data[p->pos] != '{') {
        return FALSE;
    }
    p->pos++; // Skip '{'

    while (p->pos < p->len) {
        json_skip_whitespace(p);
        if (p->data[p->pos] == '}') {
            break;
        }

        // Parse key
        char currentKey[128];
        if (!json_parse_string(p, currentKey, sizeof(currentKey))) {
            break;
        }

        json_skip_whitespace(p);
        if (p->pos >= p->len || p->data[p->pos] != ':') {
            break;
        }
        p->pos++; // Skip ':'

        json_skip_whitespace(p);

        if (strcmp(currentKey, key) == 0) {
            // Found the key
            if (p->data[p->pos] == '"' && strOut) {
                return json_parse_string(p, strOut, strOutSize);
            } else if (intOut) {
                return json_parse_number(p, intOut);
            }
            return FALSE;
        }

        // Skip this value
        json_skip_value(p);

        json_skip_whitespace(p);
        if (p->pos < p->len && p->data[p->pos] == ',') {
            p->pos++;
        }
    }

    return FALSE;
}

// ============================================================================
// Windows Terminal Settings Reader
// ============================================================================

static char* read_file(const char* path, size_t* outSize) {
    FILE* f = fopen(path, "rb");
    if (!f) {
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* data = (char*)malloc(size + 1);
    if (!data) {
        fclose(f);
        return NULL;
    }

    fread(data, 1, size, f);
    data[size] = '\0';
    fclose(f);

    *outSize = size;
    return data;
}

static TerminalSettings load_terminal_settings(void) {
    TerminalSettings settings = {
        .fontFace = "Cascadia Mono",
        .fontSize = 12,
        .padding = { 8, 8, 8, 8 }
    };

    char path[MAX_PATH];

    // Try packaged version first (Microsoft Store install)
    char* localAppData = getenv("LOCALAPPDATA");
    if (!localAppData) {
        return settings;
    }

    // Try unpackaged location first (more common for developers)
    snprintf(path, MAX_PATH, "%s\\Microsoft\\Windows Terminal\\settings.json", localAppData);

    size_t fileSize;
    char* jsonData = read_file(path, &fileSize);

    if (!jsonData) {
        // Try packaged location
        snprintf(path, MAX_PATH, "%s\\Packages\\Microsoft.WindowsTerminal_8wekyb3d8bbwe\\LocalState\\settings.json", localAppData);
        jsonData = read_file(path, &fileSize);
    }

    if (!jsonData) {
        printf("[!] Could not read settings.json, using defaults\n");
        return settings;
    }

    printf("[*] Loaded settings from: %s\n", path);

    JsonParser parser = { .data = jsonData, .pos = 0, .len = fileSize };

    // Find "profiles" -> "defaults" section
    const char* profilesDefaults = strstr(jsonData, "\"defaults\"");
    if (profilesDefaults) {
        // Find the object after "defaults"
        const char* objStart = strchr(profilesDefaults, '{');
        if (objStart) {
            JsonParser defaultsParser = {
                .data = objStart,
                .pos = 0,
                .len = fileSize - (objStart - jsonData)
            };

            // Look for font settings
            char fontFaceStr[128];
            if (json_find_in_object(&defaultsParser, "fontFace", fontFaceStr, sizeof(fontFaceStr), NULL)) {
                strncpy(settings.fontFace, fontFaceStr, sizeof(settings.fontFace) - 1);
            }

            defaultsParser.pos = 0;
            int fontSize;
            if (json_find_in_object(&defaultsParser, "fontSize", NULL, 0, &fontSize)) {
                settings.fontSize = fontSize;
            }

            defaultsParser.pos = 0;
            char paddingStr[64];
            if (json_find_in_object(&defaultsParser, "padding", paddingStr, sizeof(paddingStr), NULL)) {
                settings.padding = parse_padding_string(paddingStr);
            }
        }
    }

    free(jsonData);

    printf("[+] Settings: font='%s' size=%d padding=(%d,%d,%d,%d)\n", settings.fontFace, settings.fontSize, settings.padding.left, settings.padding.top, settings.padding.right, settings.padding.bottom);

    return settings;
}

// ============================================================================
// Font Metrics Calculation
// ============================================================================

static void calculate_cell_size(TerminalContext* ctx) {
    HDC hdc = GetDC(NULL);

    // Convert point size to logical units
    int logPixelsY = GetDeviceCaps(hdc, LOGPIXELSY);
    int fontHeight = -MulDiv(ctx->settings.fontSize + ctx->currentZoomLevel, logPixelsY, 72);

    HFONT font = CreateFontA(
        fontHeight,
        0, 0, 0,
        FW_NORMAL,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        FIXED_PITCH | FF_MODERN,
        ctx->settings.fontFace
    );

    if (font) {
        HFONT oldFont = SelectObject(hdc, font);

        TEXTMETRICA tm;
        GetTextMetricsA(hdc, &tm);

        ctx->cellWidth = tm.tmAveCharWidth;
        ctx->cellHeight = tm.tmHeight;

        SelectObject(hdc, oldFont);
        DeleteObject(font);
    } else {
        // Fallback estimation
        ctx->cellWidth = (ctx->settings.fontSize + ctx->currentZoomLevel) * 7 / 10;
        ctx->cellHeight = (ctx->settings.fontSize + ctx->currentZoomLevel) * 4 / 3;
    }

    ReleaseDC(NULL, hdc);

    printf("[+] Cell size: %dx%d pixels (zoom level: %+d)\n", ctx->cellWidth, ctx->cellHeight, ctx->currentZoomLevel);
}

// ============================================================================
// Window Discovery
// ============================================================================

typedef struct {
    DWORD targetPid;
    HWND foundWindow;
} EnumWindowsData;

static BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam) {
    EnumWindowsData* data = (EnumWindowsData*)lParam;
    DWORD windowPid;

    GetWindowThreadProcessId(hwnd, &windowPid);
    if (windowPid != data->targetPid) {
        return TRUE;
    }

    char className[256];
    if (GetClassNameA(hwnd, className, sizeof(className))) {
        if (strstr(className, "CASCADIA_HOSTING_WINDOW_CLASS") || strstr(className, "Windows.UI.Core.CoreWindow")) {
            if (IsWindowVisible(hwnd)) {
                char title[256];
                if (GetWindowTextA(hwnd, title, sizeof(title)) > 0) {
                    data->foundWindow = hwnd;
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

static BOOL CALLBACK FindContentAreaCallback(HWND hwnd, LPARAM lParam) {
    TerminalContext* ctx = (TerminalContext*)lParam;

    char className[256];
    if (!GetClassNameA(hwnd, className, sizeof(className))) {
        return TRUE;
    }

    // Look for the actual content rendering surface
    if (strstr(className, "Windows.UI.Composition.DesktopWindowContentBridge")) {
        RECT rect;
        GetWindowRect(hwnd, &rect);

        POINT topLeft = { rect.left, rect.top };
        ScreenToClient(ctx->mainWindow, &topLeft);

        // Content area should be below the tab bar
        if (topLeft.y > 20) { // Reasonable threshold for tab bar
            ctx->contentArea = hwnd;
            ctx->tabBarHeight = topLeft.y;
            return FALSE;
        }
    }

    return TRUE;
}

static HWND find_terminal_window(DWORD pid, TerminalContext* ctx) {
    EnumWindowsData data = { .targetPid = pid, .foundWindow = NULL };

    EnumWindows(EnumWindowsCallback, (LPARAM)&data);
    if (data.foundWindow) {
        ctx->processId = pid;
        return data.foundWindow;
    }

    // Check child processes (WT broker architecture)
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    PROCESSENTRY32W pe = { .dwSize = sizeof(pe) };
    if (Process32FirstW(snapshot, &pe)) {
        do {
            if (pe.th32ParentProcessID == pid) {
                data.targetPid = pe.th32ProcessID;
                EnumWindows(EnumWindowsCallback, (LPARAM)&data);
                if (data.foundWindow) {
                    ctx->processId = pe.th32ProcessID;
                    break;
                }
            }
        } while (Process32NextW(snapshot, &pe));
    }

    CloseHandle(snapshot);
    return data.foundWindow;
}

static void detect_tab_bar_height(TerminalContext* ctx) {
    // Try to find content area child window
    EnumChildWindows(ctx->mainWindow, FindContentAreaCallback, (LPARAM)ctx);

    if (!ctx->contentArea) {
        // Fallback: estimate based on Windows 11 defaults
        // Tab bar is typically 32-40 pixels
        ctx->tabBarHeight = 32;

        // Try using DWM extended frame bounds
        RECT extendedFrame;
        if (SUCCEEDED(DwmGetWindowAttribute(ctx->mainWindow, DWMWA_EXTENDED_FRAME_BOUNDS, &extendedFrame, sizeof(extendedFrame)))) {
            RECT clientRect;
            GetClientRect(ctx->mainWindow, &clientRect);

            // The difference might give us a hint
            int diff = (extendedFrame.bottom - extendedFrame.top) - (clientRect.bottom - clientRect.top);
            if (diff > 0 && diff < 100) {
                // Reasonable adjustment
            }
        }
    }

    printf("[+] Tab bar height: %d pixels\n", ctx->tabBarHeight);
}

// ============================================================================
// Position Conversion
// ============================================================================

static TerminalPosition screen_to_terminal_pos(POINT screenPt, TerminalContext* ctx) {
    TerminalPosition pos = { 0 };

    POINT clientPt = screenPt;
    ScreenToClient(ctx->mainWindow, &clientPt);

    // Check if in tab bar
    if (clientPt.y < ctx->tabBarHeight) {
        pos.inContentArea = FALSE;
        pos.pixelX = clientPt.x;
        pos.pixelY = clientPt.y;
        return pos;
    }

    // Convert to content area coordinates
    int contentX = clientPt.x;
    int contentY = clientPt.y - ctx->tabBarHeight;

    // Remove padding
    int textX = contentX - ctx->settings.padding.left;
    int textY = contentY - ctx->settings.padding.top;

    // Check bounds
    RECT clientRect;
    GetClientRect(ctx->mainWindow, &clientRect);

    int contentWidth = clientRect.right - ctx->settings.padding.left - ctx->settings.padding.right;
    int contentHeight = clientRect.bottom - ctx->tabBarHeight - ctx->settings.padding.top - ctx->settings.padding.bottom;

    if (textX < 0 || textY < 0 || textX >= contentWidth || textY >= contentHeight) {
        pos.inPadding = TRUE;
        pos.inContentArea = FALSE;
        pos.pixelX = textX;
        pos.pixelY = textY;
        return pos;
    }

    pos.inContentArea = TRUE;
    pos.inPadding = FALSE;
    pos.pixelX = textX;
    pos.pixelY = textY;

    // Calculate character position
    if (ctx->cellWidth > 0 && ctx->cellHeight > 0) {
        pos.charX = textX / ctx->cellWidth;
        pos.charY = textY / ctx->cellHeight;
    }

    return pos;
}

// ============================================================================
// Event Hooks
// ============================================================================

static LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* kb = (KBDLLHOOKSTRUCT*)lParam;

        if (GetForegroundWindow() == g_ctx.mainWindow) {
            BOOL ctrlDown = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
            BOOL shiftDown = (GetKeyState(VK_SHIFT) & 0x8000) != 0;

            // Detect font size changes (Ctrl+Plus, Ctrl+Minus, Ctrl+0)
            if (ctrlDown && wParam == WM_KEYDOWN) {
                int oldZoom = g_ctx.currentZoomLevel;
                BOOL zoomChanged = FALSE;

                if (kb->vkCode == VK_OEM_PLUS || kb->vkCode == VK_ADD) {
                    // Ctrl+Plus: Increase font size
                    g_ctx.currentZoomLevel += 2; // WT typically changes by 2pt
                    zoomChanged = TRUE;
                    printf("[FONT] SIZE INCREASED: %d -> %d (zoom: %+d)\n", g_ctx.settings.fontSize + oldZoom, g_ctx.settings.fontSize + g_ctx.currentZoomLevel, g_ctx.currentZoomLevel);
                } else if (kb->vkCode == VK_OEM_MINUS || kb->vkCode == VK_SUBTRACT) {
                    // Ctrl+Minus: Decrease font size
                    g_ctx.currentZoomLevel -= 2;
                    zoomChanged = TRUE;
                    printf("[FONT] SIZE DECREASED: %d -> %d (zoom: %+d)\n", g_ctx.settings.fontSize + oldZoom, g_ctx.settings.fontSize + g_ctx.currentZoomLevel, g_ctx.currentZoomLevel);
                } else if (kb->vkCode == '0' || kb->vkCode == VK_NUMPAD0) {
                    // Ctrl+0: Reset font size
                    g_ctx.currentZoomLevel = 0;
                    zoomChanged = TRUE;
                    printf("[FONT] SIZE RESET to %d (zoom: 0)\n", g_ctx.settings.fontSize);
                }

                if (zoomChanged) {
                    calculate_cell_size(&g_ctx);
                }
            }

            // Log regular key events
            if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
                char keyName[64] = { 0 };
                UINT scanCode = MapVirtualKeyA(kb->vkCode, MAPVK_VK_TO_VSC);
                GetKeyNameTextA(scanCode << 16, keyName, sizeof(keyName));

                printf("[KEY] DOWN: VK=0x%02lX '%s'", kb->vkCode, keyName);
                if (ctrlDown) {
                    printf(" [CTRL]");
                }
                if (shiftDown) {
                    printf(" [SHIFT]");
                }
                if (kb->flags & LLKHF_ALTDOWN) {
                    printf(" [ALT]");
                }
                printf("\n");
            }
        }
    }
    return CallNextHookEx(g_keyboardHook, nCode, wParam, lParam);
}

static LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        MSLLHOOKSTRUCT* ms = (MSLLHOOKSTRUCT*)lParam;
        POINT pt = ms->pt;

        HWND hwndUnderCursor = WindowFromPoint(pt);
        HWND rootWindow = GetAncestor(hwndUnderCursor, GA_ROOT);

        if (rootWindow == g_ctx.mainWindow || hwndUnderCursor == g_ctx.mainWindow) {
            TerminalPosition tpos = screen_to_terminal_pos(pt, &g_ctx);

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
                short delta = (short)HIWORD(ms->mouseData);
                BOOL ctrlDown = (GetKeyState(VK_CONTROL) & 0x8000) != 0;

                if (ctrlDown) {
                    // Ctrl+Scroll also changes font size in WT
                    int oldZoom = g_ctx.currentZoomLevel;
                    if (delta > 0) {
                        g_ctx.currentZoomLevel += 2;
                        printf("[FONT] SIZE INCREASED (scroll): %d -> %d\n", g_ctx.settings.fontSize + oldZoom, g_ctx.settings.fontSize + g_ctx.currentZoomLevel);
                    } else {
                        g_ctx.currentZoomLevel -= 2;
                        printf("[FONT] SIZE DECREASED (scroll): %d -> %d\n", g_ctx.settings.fontSize + oldZoom, g_ctx.settings.fontSize + g_ctx.currentZoomLevel);
                    }
                    calculate_cell_size(&g_ctx);
                } else {
                    printf("[MOUSE] WHEEL: delta=%d\n", delta);
                }
                break;
            }
            }

            if (eventType) {
                if (tpos.inContentArea) {
                    printf("[MOUSE] %s: pixel(%d,%d) char(%d,%d)\n", eventType, tpos.pixelX, tpos.pixelY, tpos.charX, tpos.charY);
                } else if (tpos.inPadding) {
                    printf("[MOUSE] %s: in padding area pixel(%d,%d)\n", eventType, tpos.pixelX, tpos.pixelY);
                } else {
                    printf("[MOUSE] %s: in tab bar\n", eventType);
                }
            }
        }
    }
    return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
}

static void CALLBACK WinEventCallback(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD idEventThread, DWORD dwmsEventTime) {
    if (hwnd != g_ctx.mainWindow || idObject != OBJID_WINDOW) {
        return;
    }

    switch (event) {
    case EVENT_OBJECT_LOCATIONCHANGE: {
        RECT windowRect, clientRect;
        GetWindowRect(hwnd, &windowRect);
        GetClientRect(hwnd, &clientRect);

        int windowW = windowRect.right - windowRect.left;
        int windowH = windowRect.bottom - windowRect.top;
        int clientW = clientRect.right - clientRect.left;
        int clientH = clientRect.bottom - clientRect.top;

        // Calculate character grid size
        int contentW = clientW - g_ctx.settings.padding.left - g_ctx.settings.padding.right;
        int contentH = clientH - g_ctx.tabBarHeight - g_ctx.settings.padding.top - g_ctx.settings.padding.bottom;

        int cols = (g_ctx.cellWidth > 0) ? contentW / g_ctx.cellWidth : 0;
        int rows = (g_ctx.cellHeight > 0) ? contentH / g_ctx.cellHeight : 0;

        printf("[WINDOW] RESIZE: %dx%d (client: %dx%d) grid: %dx%d chars\n", windowW, windowH, clientW, clientH, cols, rows);
        break;
    }
    case EVENT_SYSTEM_FOREGROUND:
        printf("[WINDOW] ACTIVATED\n");
        break;
    case EVENT_SYSTEM_MINIMIZESTART:
        printf("[WINDOW] MINIMIZED\n");
        break;
    case EVENT_SYSTEM_MINIMIZEEND:
        printf("[WINDOW] RESTORED\n");
        break;
    }
}

// ============================================================================
// Cleanup and Control
// ============================================================================

static BOOL WINAPI ConsoleCtrlHandler(DWORD ctrlType) {
    if (ctrlType == CTRL_C_EVENT || ctrlType == CTRL_BREAK_EVENT) {
        printf("\n[*] Shutting down...\n");
        g_running = FALSE;
        PostQuitMessage(0);
        return TRUE;
    }
    return FALSE;
}

static void cleanup(PROCESS_INFORMATION* pi) {
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

// ============================================================================
// Main
// ============================================================================

int main(int argc, char* argv[]) {
    PROCESS_INFORMATION pi = { 0 };
    STARTUPINFOW si = { .cb = sizeof(si) };

    printf("=== Windows Terminal Event Monitor ===\n\n");

    // Load settings
    printf("[*] Loading Windows Terminal settings...\n");
    g_ctx.settings = load_terminal_settings();
    g_ctx.currentZoomLevel = 0;
    calculate_cell_size(&g_ctx);

    SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

    // Spawn Windows Terminal
    printf("\n[*] Spawning Windows Terminal...\n");

    wchar_t cmdLine[] = L"wt.exe";
    if (!CreateProcessW(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("[!] Failed to spawn wt.exe (error: %lu)\n", GetLastError());
        return 1;
    }

    printf("[+] Process created (PID: %lu)\n", pi.dwProcessId);
    printf("[*] Waiting for window...\n");

    // Wait for window
    for (int i = 0; i < 30; i++) {
        Sleep(200);
        g_ctx.mainWindow = find_terminal_window(pi.dwProcessId, &g_ctx);
        if (g_ctx.mainWindow) {
            break;
        }
        printf(".");
    }
    printf("\n");

    if (!g_ctx.mainWindow) {
        printf("[!] Could not find Windows Terminal window\n");
        cleanup(&pi);
        return 1;
    }

    char windowTitle[256];
    GetWindowTextA(g_ctx.mainWindow, windowTitle, sizeof(windowTitle));
    printf("[+] Found: HWND=0x%p PID=%lu '%s'\n", (void*)g_ctx.mainWindow, g_ctx.processId, windowTitle);

    // Detect layout
    detect_tab_bar_height(&g_ctx);

    // Install hooks
    printf("\n[*] Installing hooks...\n");

    g_keyboardHook = SetWindowsHookExA(WH_KEYBOARD_LL, KeyboardHookProc, NULL, 0);
    if (g_keyboardHook) {
        printf("[+] Keyboard hook installed\n");
    } else {
        printf("[!] Keyboard hook failed (error: %lu)\n", GetLastError());
    }

    g_mouseHook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);
    if (g_mouseHook) {
        printf("[+] Mouse hook installed\n");
    } else {
        printf("[!] Mouse hook failed (error: %lu)\n", GetLastError());
    }

    g_winEventHook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND,
        EVENT_OBJECT_LOCATIONCHANGE,
        NULL,
        WinEventCallback,
        g_ctx.processId,
        0,
        WINEVENT_OUTOFCONTEXT
    );
    if (g_winEventHook) {
        printf("[+] Window event hook installed\n");
    } else {
        printf("[!] Window event hook failed (error: %lu)\n", GetLastError());
    }

    printf("\n=== Monitoring Started ===\n");
    printf("- Type in Windows Terminal to see keyboard events\n");
    printf("- Click in content area to see character positions\n");
    printf("- Use Ctrl+Plus/Minus or Ctrl+Scroll to change font size\n");
    printf("- Resize window to see grid dimensions\n");
    printf("- Press Ctrl+C HERE to stop\n\n");

    // Message loop
    MSG msg;
    while (g_running && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    printf("[*] Cleaning up...\n");
    cleanup(&pi);
    printf("[+] Done.\n");

    return 0;
}
