// dh-c run wt_monitor3.c --no-libdh --args="-luser32 -ldwmapi -lshell32 -lgdi32 -- "
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define COBJMACROS
#include <windows.h>
#include <dwmapi.h>
#define INITGUID
#include <initguid.h> // <---- required for MinGW/Clang
#include <dwrite.h>
#include <d2d1.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>
#include <math.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "ole32.lib")

// ============================================================================
// Data Structures
// ============================================================================

typedef struct {
    int left, top, right, bottom;
} Padding;

typedef struct {
    char fontFace[128];
    int fontSize;
    double lineHeight;
    double cellWidth;
    Padding padding;
} TerminalSettings;

typedef struct {
    HWND mainWindow;
    HWND contentArea;
    DWORD processId;
    HANDLE processHandle;
    HANDLE jobObject;
    HANDLE monitorThread;
    int tabBarHeight;
    double cellWidth; // Use double for precision
    double cellHeight;
    TerminalSettings settings;
    int currentZoomLevel;
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
static PROCESS_INFORMATION g_pi = { 0 };

// ============================================================================
// JSON Parser
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
        } else if (c == '/' && p->pos + 1 < p->len) {
            if (p->data[p->pos + 1] == '/') {
                p->pos += 2;
                while (p->pos < p->len && p->data[p->pos] != '\n') {
                    p->pos++;
                }
            } else if (p->data[p->pos + 1] == '*') {
                p->pos += 2;
                while (p->pos + 1 < p->len) {
                    if (p->data[p->pos] == '*' && p->data[p->pos + 1] == '/') {
                        p->pos += 2;
                        break;
                    }
                    p->pos++;
                }
            } else {
                break;
            }
        } else {
            break;
        }
    }
}

static BOOL json_parse_string(JsonParser* p, char* out, size_t outSize) {
    json_skip_whitespace(p);
    if (p->pos >= p->len || p->data[p->pos] != '"') {
        return FALSE;
    }
    p->pos++;

    size_t outPos = 0;
    while (p->pos < p->len && p->data[p->pos] != '"') {
        if (p->data[p->pos] == '\\' && p->pos + 1 < p->len) {
            p->pos++;
            if (outPos < outSize - 1) {
                out[outPos++] = p->data[p->pos];
            }
            p->pos++;
        } else {
            if (outPos < outSize - 1) {
                out[outPos++] = p->data[p->pos];
            }
            p->pos++;
        }
    }
    out[outPos] = '\0';
    if (p->pos < p->len && p->data[p->pos] == '"') {
        p->pos++;
    }
    return TRUE;
}

static BOOL json_parse_int(JsonParser* p, int* out) {
    json_skip_whitespace(p);
    char* end;
    *out = (int)strtol(p->data + p->pos, &end, 10);
    if (end == p->data + p->pos) {
        return FALSE;
    }
    p->pos = end - p->data;
    return TRUE;
}

static BOOL json_parse_double(JsonParser* p, double* out) {
    json_skip_whitespace(p);
    char* end;
    *out = strtod(p->data + p->pos, &end);
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
        p->pos++;
        while (p->pos < p->len && p->data[p->pos] != '"') {
            if (p->data[p->pos] == '\\') {
                p->pos++;
            }
            p->pos++;
        }
        if (p->pos < p->len) {
            p->pos++;
        }
    } else if (c == '{' || c == '[') {
        char open = c, close = (c == '{') ? '}' : ']';
        int depth = 1;
        p->pos++;
        while (p->pos < p->len && depth > 0) {
            char ch = p->data[p->pos];
            if (ch == open) {
                depth++;
            } else if (ch == close) {
                depth--;
            } else if (ch == '"') {
                p->pos++;
                while (p->pos < p->len && p->data[p->pos] != '"') {
                    if (p->data[p->pos] == '\\') {
                        p->pos++;
                    }
                    p->pos++;
                }
            }
            p->pos++;
        }
    } else {
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
    Padding p = { 8, 8, 8, 8 };
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
        .lineHeight = 1.2,
        .cellWidth = 0.6, // ADD THIS - Windows Terminal default
        .padding = { 8, 8, 8, 8 }
    };

    char* localAppData = getenv("LOCALAPPDATA");
    if (!localAppData) {
        return settings;
    }

    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s\\Microsoft\\Windows Terminal\\settings.json", localAppData);

    size_t fileSize;
    char* jsonData = read_file(path, &fileSize);
    if (!jsonData) {
        snprintf(path, MAX_PATH, "%s\\Packages\\Microsoft.WindowsTerminal_8wekyb3d8bbwe\\LocalState\\settings.json", localAppData);
        jsonData = read_file(path, &fileSize);
    }
    if (!jsonData) {
        return settings;
    }

    printf("[+] Loaded: %s\n", path);

    const char* defaultsStr = strstr(jsonData, "\"defaults\"");
    if (!defaultsStr) {
        free(jsonData);
        return settings;
    }

    const char* defaultsObj = strchr(defaultsStr, '{');
    if (!defaultsObj) {
        free(jsonData);
        return settings;
    }

    JsonParser parser = { .data = defaultsObj, .pos = 1, .len = fileSize - (defaultsObj - jsonData) };

    while (parser.pos < parser.len) {
        json_skip_whitespace(&parser);
        if (parser.data[parser.pos] == '}') {
            break;
        }

        char key[128];
        if (!json_parse_string(&parser, key, sizeof(key))) {
            break;
        }

        json_skip_whitespace(&parser);
        if (parser.data[parser.pos] != ':') {
            break;
        }
        parser.pos++;
        json_skip_whitespace(&parser);

        if (strcmp(key, "font") == 0 && parser.data[parser.pos] == '{') {
            parser.pos++;
            while (parser.pos < parser.len) {
                json_skip_whitespace(&parser);
                if (parser.data[parser.pos] == '}') {
                    parser.pos++;
                    break;
                }

                char fontKey[64];
                if (!json_parse_string(&parser, fontKey, sizeof(fontKey))) {
                    break;
                }
                json_skip_whitespace(&parser);
                if (parser.data[parser.pos] != ':') {
                    break;
                }
                parser.pos++;
                json_skip_whitespace(&parser);

                if (strcmp(fontKey, "face") == 0) {
                    json_parse_string(&parser, settings.fontFace, sizeof(settings.fontFace));
                    printf("  font.face: %s\n", settings.fontFace);
                } else if (strcmp(fontKey, "size") == 0) {
                    json_parse_int(&parser, &settings.fontSize);
                    printf("  font.size: %d\n", settings.fontSize);
                } else {
                    json_skip_value(&parser);
                }

                json_skip_whitespace(&parser);
                if (parser.data[parser.pos] == ',') {
                    parser.pos++;
                }
            }
        } else if (strcmp(key, "padding") == 0) {
            char paddingStr[64];
            if (json_parse_string(&parser, paddingStr, sizeof(paddingStr))) {
                settings.padding = parse_padding_string(paddingStr);
                printf("  padding: %s -> (%d,%d,%d,%d)\n", paddingStr, settings.padding.left, settings.padding.top, settings.padding.right, settings.padding.bottom);
            }
        } else {
            json_skip_value(&parser);
        }

        json_skip_whitespace(&parser);
        if (parser.data[parser.pos] == ',') {
            parser.pos++;
        }
    }

    free(jsonData);
    return settings;
}

// ============================================================================
// DirectWrite-based Cell Size Calculation (Accurate)
// ============================================================================

static double points_to_pixels(int points, int dpi) {
    // Standard conversion: pixels = points * dpi / 72
    return (double)points * dpi / 72.0;
}
static void calculate_cell_size(TerminalContext* ctx) {
    int effectiveSize = ctx->settings.fontSize + ctx->currentZoomLevel;
    if (effectiveSize < 1) {
        effectiveSize = 1;
    }

    // Get system DPI
    HDC screenDC = GetDC(NULL);
    int dpiY = GetDeviceCaps(screenDC, LOGPIXELSY);
    int dpiX = GetDeviceCaps(screenDC, LOGPIXELSX);
    ReleaseDC(NULL, screenDC);

    // Convert font size from points to pixels
    double fontSizeInPixels = (double)effectiveSize * dpiY / 72.0;

    // Cell dimensions using simple multipliers + rounding
    ctx->cellHeight = round(fontSizeInPixels * ctx->settings.lineHeight);
    ctx->cellWidth = round(fontSizeInPixels * ctx->settings.cellWidth);

    printf("[+] Cell size calculation (pure math):\n");
    printf("    Font: %dpt @ %d DPI = %.4f pixels\n", effectiveSize, dpiY, fontSizeInPixels);
    printf("    Height: %.4f * %.2f = %.4f-> %.0f pixels\n", fontSizeInPixels, ctx->settings.lineHeight, fontSizeInPixels * ctx->settings.lineHeight, ctx->cellHeight);
    printf("    Width:  %.4f * %.2f = %.4f-> %.0f pixels\n", fontSizeInPixels, ctx->settings.cellWidth, fontSizeInPixels * ctx->settings.cellWidth, ctx->cellWidth);
}

// ============================================================================
// Process Lifecycle Management
// ============================================================================

static DWORD WINAPI ProcessMonitorThread(LPVOID param) {
    TerminalContext* ctx = (TerminalContext*)param;

    // Wait for child process to terminate
    DWORD result = WaitForSingleObject(ctx->processHandle, INFINITE);

    if (result == WAIT_OBJECT_0) {
        DWORD exitCode;
        GetExitCodeProcess(ctx->processHandle, &exitCode);
        printf("\n[!] Windows Terminal terminated (exit code: %lu)\n", exitCode);

        // Signal main thread to exit
        g_running = FALSE;
        PostQuitMessage(0);
    }

    return 0;
}

static HANDLE CreateJobObjectForChild(void) {
    HANDLE job = CreateJobObjectW(NULL, NULL);
    if (!job) {
        return NULL;
    }

    // Configure job to terminate all processes when job handle is closed
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jobInfo = { 0 };
    jobInfo.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;

    if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation, &jobInfo, sizeof(jobInfo))) {
        CloseHandle(job);
        return NULL;
    }

    return job;
}

static BOOL SpawnTerminalProcess(TerminalContext* ctx) {
    STARTUPINFOW si = { .cb = sizeof(si) };

    ctx->jobObject = CreateJobObjectForChild();
    if (!ctx->jobObject) {
        printf("[!] Failed to create job object (error: %lu)\n", GetLastError());
    } else {
        printf("[+] Job object created (child will terminate with parent)\n");
    }

    wchar_t cmdLine[] = L"wt.exe";
    if (!CreateProcessW(NULL, cmdLine, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &g_pi)) {
        printf("[!] Failed to spawn wt.exe (error: %lu)\n", GetLastError());
        return FALSE;
    }

    if (ctx->jobObject) {
        if (!AssignProcessToJobObject(ctx->jobObject, g_pi.hProcess)) {
            printf("[!] Failed to assign process to job (error: %lu)\n", GetLastError());
        }
    }

    ResumeThread(g_pi.hThread);
    ctx->processHandle = g_pi.hProcess;
    printf("[+] Process created (PID: %lu)\n", g_pi.dwProcessId);

    // DON'T start monitor thread here - wait until we find the actual terminal window

    return TRUE;
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
        if (strstr(className, "CASCADIA_HOSTING_WINDOW_CLASS")) {
            if (IsWindowVisible(hwnd)) {
                data->foundWindow = hwnd;
                return FALSE;
            }
        }
    }
    return TRUE;
}

// ============================================================================
// Window Discovery (Improved - finds ANY Windows Terminal window)
// ============================================================================

static BOOL CALLBACK EnumAllWindowsCallback(HWND hwnd, LPARAM lParam) {
    TerminalContext* ctx = (TerminalContext*)lParam;

    // Skip if we already found one
    if (ctx->mainWindow) {
        return FALSE;
    }

    char className[256];
    if (!GetClassNameA(hwnd, className, sizeof(className))) {
        return TRUE;
    }

    // Check for Windows Terminal window class
    if (strstr(className, "CASCADIA_HOSTING_WINDOW_CLASS")) {
        if (IsWindowVisible(hwnd)) {
            DWORD windowPid;
            GetWindowThreadProcessId(hwnd, &windowPid);

            // Found a Windows Terminal window - use it
            ctx->mainWindow = hwnd;
            ctx->processId = windowPid;
            return FALSE; // Stop enumeration
        }
    }

    return TRUE;
}

static HWND find_terminal_window(DWORD launcherPid, TerminalContext* ctx) {
    // First, try to find ANY existing Windows Terminal window
    // This handles the case where wt.exe reuses an existing instance
    ctx->mainWindow = NULL;
    EnumWindows(EnumAllWindowsCallback, (LPARAM)ctx);

    if (ctx->mainWindow) {
        printf("[+] Found existing Windows Terminal window (PID: %lu)\n", ctx->processId);
        return ctx->mainWindow;
    }

    // If not found, look for child processes of the launcher
    EnumWindowsData data = { .targetPid = launcherPid, .foundWindow = NULL };
    EnumWindows(EnumWindowsCallback, (LPARAM)&data);

    if (data.foundWindow) {
        ctx->processId = launcherPid;
        return data.foundWindow;
    }

    // Search child processes
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    PROCESSENTRY32W pe = { .dwSize = sizeof(pe) };
    if (Process32FirstW(snapshot, &pe)) {
        do {
            if (pe.th32ParentProcessID == launcherPid) {
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

    if (data.foundWindow) {
        ctx->mainWindow = data.foundWindow;
    }

    return data.foundWindow;
}

// Add this callback function before detect_tab_bar_height
static BOOL CALLBACK FindContentAreaCallback(HWND hwnd, LPARAM lParam) {
    TerminalContext* ctx = (TerminalContext*)lParam;

    char className[256];
    if (!GetClassNameA(hwnd, className, sizeof(className))) {
        return TRUE;
    }

    // Windows Terminal content classes
    if (strstr(className, "Windows.UI.Composition.DesktopWindowContentBridge") || strstr(className, "Windows.UI.Input.InputSite.WindowClass") || strstr(className, "TermControl")) {

        RECT childRect;
        if (GetWindowRect(hwnd, &childRect)) {
            POINT topLeft = { childRect.left, childRect.top };
            ScreenToClient(ctx->mainWindow, &topLeft);

            if (topLeft.y > 0 && topLeft.y < 200) {
                ctx->contentArea = hwnd;
                ctx->tabBarHeight = topLeft.y;
                printf("[+] Found content area: title bar is %ld pixels\n", topLeft.y);
                return FALSE;
            }
        }
    }

    return TRUE;
}

// Add these includes at the top
#include <uiautomation.h>
#include <oleauto.h>

#pragma comment(lib, "oleaut32.lib")

// Function to get title bar height using UI Automation
static int get_titlebar_height_uia(HWND hwnd) {
    HRESULT hr;
    IUIAutomation* automation = NULL;
    IUIAutomationElement* rootElement = NULL;
    IUIAutomationElement* titleBarElement = NULL;
    IUIAutomationCondition* condition = NULL;
    RECT titleBarRect = { 0 };
    int height = 0;

    // Initialize COM if not already done
    hr = CoCreateInstance(&CLSID_CUIAutomation, NULL, CLSCTX_INPROC_SERVER, &IID_IUIAutomation, (void**)&automation);
    if (FAILED(hr)) {
        printf("[!] Failed to create UI Automation instance\n");
        return 0;
    }

    // Get the automation element for the window
    hr = IUIAutomation_ElementFromHandle(automation, hwnd, &rootElement);
    if (FAILED(hr)) {
        printf("[!] Failed to get element from handle\n");
        goto cleanup;
    }

    // Create condition to find the title bar
    // Title bars typically have ControlType = TitleBar (UIA_TitleBarControlTypeId = 50037)
    VARIANT varProp;
    VariantInit(&varProp);
    varProp.vt = VT_I4;
    varProp.lVal = UIA_TitleBarControlTypeId;

    hr = IUIAutomation_CreatePropertyCondition(automation, UIA_ControlTypePropertyId, varProp, &condition);
    VariantClear(&varProp);

    if (FAILED(hr)) {
        printf("[!] Failed to create property condition\n");
        goto cleanup;
    }

    // Find the title bar element
    hr = IUIAutomationElement_FindFirst(rootElement, TreeScope_Descendants, condition, &titleBarElement);
    if (FAILED(hr) || !titleBarElement) {
        printf("[!] Title bar element not found\n");
        goto cleanup;
    }

    // Get the bounding rectangle of the title bar
    hr = IUIAutomationElement_get_CurrentBoundingRectangle(titleBarElement, &titleBarRect);
    if (SUCCEEDED(hr)) {
        height = titleBarRect.bottom - titleBarRect.top;
        printf("[+] Title bar UIA rect: (%ld,%ld)-(%ld,%ld), height=%d\n", titleBarRect.left, titleBarRect.top, titleBarRect.right, titleBarRect.bottom, height);
    }

cleanup:
    if (titleBarElement) {
        IUIAutomationElement_Release(titleBarElement);
    }
    if (condition) {
        IUIAutomationCondition_Release(condition);
    }
    if (rootElement) {
        IUIAutomationElement_Release(rootElement);
    }
    if (automation) {
        IUIAutomation_Release(automation);
    }

    return height;
}

// Alternative: Find by searching child elements directly
// Alternative: Find by searching child elements directly
static int get_titlebar_height_by_children(HWND hwnd) {
    HRESULT hr;
    IUIAutomation* automation = NULL;
    IUIAutomationElement* windowElement = NULL;
    IUIAutomationElementArray* children = NULL;
    int height = 0;

    hr = CoCreateInstance(&CLSID_CUIAutomation, NULL, CLSCTX_INPROC_SERVER, &IID_IUIAutomation, (void**)&automation);
    if (FAILED(hr)) {
        return 0;
    }

    hr = IUIAutomation_ElementFromHandle(automation, hwnd, &windowElement);
    if (FAILED(hr)) {
        goto cleanup;
    }

    // Get window position for relative calculations
    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);

    // Get all child elements
    IUIAutomationCondition* trueCondition = NULL;
    hr = IUIAutomation_CreateTrueCondition(automation, &trueCondition);
    if (SUCCEEDED(hr)) {
        hr = IUIAutomationElement_FindAll(windowElement, TreeScope_Children, trueCondition, &children);
        if (SUCCEEDED(hr) && children) {
            int count = 0;
            IUIAutomationElementArray_get_Length(children, &count);

            printf("[DEBUG] Found %d child elements\n", count);

            for (int i = 0; i < count; i++) {
                IUIAutomationElement* child = NULL;
                if (SUCCEEDED(IUIAutomationElementArray_GetElement(children, i, &child))) {
                    BSTR name = NULL;
                    CONTROLTYPEID controlType = 0;
                    RECT rect = { 0 };

                    IUIAutomationElement_get_CurrentName(child, &name);
                    IUIAutomationElement_get_CurrentControlType(child, &controlType);
                    IUIAutomationElement_get_CurrentBoundingRectangle(child, &rect);

                    int elementHeight = rect.bottom - rect.top;
                    int relativeTop = rect.top - windowRect.top;

                    wprintf(L"[DEBUG]   Child %d: type=%d, name=%s, rect=(%ld,%ld,%ld,%ld), height=%d, relTop=%d\n", i, controlType, name ? name : L"(null)", rect.left, rect.top, rect.right, rect.bottom, elementHeight, relativeTop);

                    // Check if this is the title bar:
                    // - First few children (i < 3)
                    // - Control type is Pane (50033) or TitleBar (50037)
                    // - Height between 30-50 pixels (typical title bar range)
                    // - Positioned near the top of the window (relative < 10)
                    if (i < 3 && (controlType == 50033 || controlType == 50037) && elementHeight >= 30 && elementHeight <= 50 && relativeTop < 10) {

                        height = elementHeight;
                        printf("[+] Found title bar (Child %d): height=%d\n", i, height);

                        if (name) {
                            SysFreeString(name);
                        }
                        IUIAutomationElement_Release(child);
                        break; // Found it, stop searching
                    }

                    if (name) {
                        SysFreeString(name);
                    }
                    IUIAutomationElement_Release(child);
                }
            }
        }
        if (trueCondition) {
            IUIAutomationCondition_Release(trueCondition);
        }
        if (children) {
            IUIAutomationElementArray_Release(children);
        }
    }

cleanup:
    if (windowElement) {
        IUIAutomationElement_Release(windowElement);
    }
    if (automation) {
        IUIAutomation_Release(automation);
    }

    return height;
}

// Updated detect function
static void detect_tab_bar_height(TerminalContext* ctx) {
    printf("[*] Detecting title bar height...\n");

    // Method 1: Try UI Automation
    int uiaHeight = get_titlebar_height_uia(ctx->mainWindow);
    if (uiaHeight > 0 && uiaHeight < 200) {
        ctx->tabBarHeight = uiaHeight;
        printf("[+] Title bar height: %d pixels (from UI Automation)\n", ctx->tabBarHeight);
        return;
    }

    // Method 2: Try finding by child elements
    int childHeight = get_titlebar_height_by_children(ctx->mainWindow);
    if (childHeight > 0 && childHeight < 200) {
        ctx->tabBarHeight = childHeight;
        printf("[+] Title bar height: %d pixels (from child elements)\n", ctx->tabBarHeight);
        return;
    }

    // Method 3: Enumerate child windows
    printf("[*] Searching for content area child windows...\n");
    EnumChildWindows(ctx->mainWindow, FindContentAreaCallback, (LPARAM)ctx);

    if (ctx->tabBarHeight > 0) {
        printf("[+] Title bar height: %d pixels (from content area position)\n", ctx->tabBarHeight);
        return;
    }

    // Fallback: Calculate from window metrics
    printf("[!] Could not detect title bar, using fallback calculation\n");
    RECT windowRect;
    GetWindowRect(ctx->mainWindow, &windowRect);

    POINT clientTopLeft = { 0, 0 };
    ClientToScreen(ctx->mainWindow, &clientTopLeft);

    int borderOffset = clientTopLeft.y - windowRect.top;
    ctx->tabBarHeight = borderOffset + 40; // Windows Terminal typically uses 40px

    printf("[+] Title bar height: %d pixels (estimated)\n", ctx->tabBarHeight);
}

// ============================================================================
// Position Conversion (Using floating-point for accuracy)
// ============================================================================

static TerminalPosition screen_to_terminal_pos(POINT screenPt, TerminalContext* ctx) {
    TerminalPosition pos = { 0 };

    POINT clientPt = screenPt;
    ScreenToClient(ctx->mainWindow, &clientPt);

    if (clientPt.y < ctx->tabBarHeight) {
        pos.inContentArea = FALSE;
        pos.pixelX = clientPt.x;
        pos.pixelY = clientPt.y;
        return pos;
    }

    // Content area coordinates (after tab bar)
    double contentX = (double)clientPt.x;
    double contentY = (double)(clientPt.y - ctx->tabBarHeight);

    // Text area coordinates (after padding)
    double textX = contentX - ctx->settings.padding.left;
    double textY = contentY - ctx->settings.padding.top;

    pos.pixelX = (int)textX;
    pos.pixelY = (int)textY;

    RECT clientRect;
    GetClientRect(ctx->mainWindow, &clientRect);

    double maxTextX = clientRect.right - ctx->settings.padding.left - ctx->settings.padding.right;
    double maxTextY = clientRect.bottom - ctx->tabBarHeight - ctx->settings.padding.top - ctx->settings.padding.bottom;

    if (textX < 0 || textY < 0 || textX >= maxTextX || textY >= maxTextY) {
        pos.inPadding = TRUE;
        pos.inContentArea = FALSE;
        return pos;
    }

    pos.inContentArea = TRUE;
    pos.inPadding = FALSE;

    // Simple floor division - no offset needed
    if (ctx->cellWidth > 0 && ctx->cellHeight > 0) {
        pos.charX = (int)floor(textX / ctx->cellWidth);
        pos.charY = (int)floor(textY / ctx->cellHeight);
    }

    return pos;
}

// ============================================================================
// Event Hooks
// ============================================================================

static LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && GetForegroundWindow() == g_ctx.mainWindow) {
        KBDLLHOOKSTRUCT* kb = (KBDLLHOOKSTRUCT*)lParam;
        BOOL ctrlDown = (GetKeyState(VK_CONTROL) & 0x8000) != 0;

        if (ctrlDown && wParam == WM_KEYDOWN) {
            int oldSize = g_ctx.settings.fontSize + g_ctx.currentZoomLevel;
            BOOL changed = FALSE;

            if (kb->vkCode == VK_OEM_PLUS || kb->vkCode == VK_ADD) {
                g_ctx.currentZoomLevel += 2;
                changed = TRUE;
            } else if (kb->vkCode == VK_OEM_MINUS || kb->vkCode == VK_SUBTRACT) {
                g_ctx.currentZoomLevel -= 2;
                changed = TRUE;
            } else if (kb->vkCode == '0') {
                g_ctx.currentZoomLevel = 0;
                changed = TRUE;
            }

            if (changed) {
                int newSize = g_ctx.settings.fontSize + g_ctx.currentZoomLevel;
                printf("[FONT] %dpt -> %dpt\n", oldSize, newSize);
                calculate_cell_size(&g_ctx);
            }
        }

        if (wParam == WM_KEYDOWN) {
            char keyName[64] = { 0 };
            GetKeyNameTextA(MapVirtualKeyA(kb->vkCode, MAPVK_VK_TO_VSC) << 16, keyName, 64);
            printf("[KEY] %s", keyName);
            if (ctrlDown) {
                printf(" +Ctrl");
            }
            if (GetKeyState(VK_SHIFT) & 0x8000) {
                printf(" +Shift");
            }
            if (kb->flags & LLKHF_ALTDOWN) {
                printf(" +Alt");
            }
            printf("\n");
        }
    }
    return CallNextHookEx(g_keyboardHook, nCode, wParam, lParam);
}

static LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        MSLLHOOKSTRUCT* ms = (MSLLHOOKSTRUCT*)lParam;
        HWND root = GetAncestor(WindowFromPoint(ms->pt), GA_ROOT);

        if (root == g_ctx.mainWindow) {
            TerminalPosition tpos = screen_to_terminal_pos(ms->pt, &g_ctx);

            if (wParam == WM_LBUTTONDOWN) {
                if (tpos.inContentArea) {
                    // Show both pixel and character position with sub-cell info
                    double subCellX = fmod(tpos.pixelX, g_ctx.cellWidth);
                    double subCellY = fmod(tpos.pixelY, g_ctx.cellHeight);
                    printf("[CLICK] char(%d,%d) pixel(%d,%d) subcell(%.1f,%.1f)\n", tpos.charX, tpos.charY, tpos.pixelX, tpos.pixelY, subCellX, subCellY);
                } else if (tpos.inPadding) {
                    printf("[CLICK] padding pixel(%d,%d)\n", tpos.pixelX, tpos.pixelY);
                } else {
                    printf("[CLICK] tab bar\n");
                }
            } else if (wParam == WM_MOUSEWHEEL) {
                short delta = (short)HIWORD(ms->mouseData);
                if (GetKeyState(VK_CONTROL) & 0x8000) {
                    int oldSize = g_ctx.settings.fontSize + g_ctx.currentZoomLevel;
                    g_ctx.currentZoomLevel += (delta > 0) ? 2 : -2;
                    int newSize = g_ctx.settings.fontSize + g_ctx.currentZoomLevel;
                    printf("[FONT] scroll: %dpt -> %dpt\n", oldSize, newSize);
                    calculate_cell_size(&g_ctx);
                }
            }
        }
    }
    return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
}

static void CALLBACK WinEventCallback(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD thread, DWORD time) {
    if (hwnd != g_ctx.mainWindow || idObject != OBJID_WINDOW) {
        return;
    }

    if (event == EVENT_OBJECT_LOCATIONCHANGE) {
        RECT r;
        GetClientRect(hwnd, &r);
        int contentW = r.right - g_ctx.settings.padding.left - g_ctx.settings.padding.right;
        int contentH = r.bottom - g_ctx.tabBarHeight - g_ctx.settings.padding.top - g_ctx.settings.padding.bottom;
        int cols = (g_ctx.cellWidth > 0) ? (int)(contentW / g_ctx.cellWidth) : 0;
        int rows = (g_ctx.cellHeight > 0) ? (int)(contentH / g_ctx.cellHeight) : 0;
        printf("[RESIZE] %ldx%ld grid:%dx%d\n", r.right, r.bottom, cols, rows);
    }
}

// ============================================================================
// Cleanup
// ============================================================================

static void cleanup(void) {
    printf("[*] Cleaning up...\n");

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

    // Signal monitor thread to stop
    g_running = FALSE;

    // Wait for monitor thread (with timeout)
    if (g_ctx.monitorThread) {
        WaitForSingleObject(g_ctx.monitorThread, 2000);
        CloseHandle(g_ctx.monitorThread);
        g_ctx.monitorThread = NULL;
    }

    // Terminate terminal process if still running
    if (g_ctx.processHandle) {
        DWORD exitCode;
        if (GetExitCodeProcess(g_ctx.processHandle, &exitCode) && exitCode == STILL_ACTIVE) {
            printf("[*] Terminating Windows Terminal (PID: %lu)...\n", g_ctx.processId);
            TerminateProcess(g_ctx.processHandle, 0);
            WaitForSingleObject(g_ctx.processHandle, 1000);
        }
        CloseHandle(g_ctx.processHandle);
        g_ctx.processHandle = NULL;
    }

    // Close launcher handles
    if (g_pi.hThread) {
        CloseHandle(g_pi.hThread);
        g_pi.hThread = NULL;
    }
    if (g_pi.hProcess) {
        CloseHandle(g_pi.hProcess);
        g_pi.hProcess = NULL;
    }

    // Close job object LAST - this will kill all processes in the job
    if (g_ctx.jobObject) {
        printf("[*] Closing job object (will terminate all child processes)...\n");
        CloseHandle(g_ctx.jobObject);
        g_ctx.jobObject = NULL;
    }

    printf("[+] Cleanup complete\n");
}

static BOOL WINAPI ConsoleCtrlHandler(DWORD ctrlType) {
    if (ctrlType == CTRL_C_EVENT || ctrlType == CTRL_BREAK_EVENT) {
        printf("\n[!] Ctrl+C received\n");
        g_running = FALSE;
        PostQuitMessage(0);
        return TRUE;
    }
    return FALSE;
}

// ============================================================================
// Main
// ============================================================================

int main(void) {
    printf("=== Windows Terminal Monitor v6 ===\n\n");

    // Initialize COM for DirectWrite
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

    printf("[*] Loading settings...\n");
    g_ctx.settings = load_terminal_settings();
    g_ctx.currentZoomLevel = 0;

    printf("\n[*] Calculating cell size...\n");
    calculate_cell_size(&g_ctx);

    printf("\n[*] Spawning Windows Terminal...\n");
    if (!SpawnTerminalProcess(&g_ctx)) {
        CoUninitialize();
        return 1;
    }

    printf("[*] Waiting for window...\n");
    for (int i = 0; i < 50 && g_running; i++) {
        Sleep(100);
        g_ctx.mainWindow = find_terminal_window(g_pi.dwProcessId, &g_ctx);
        if (g_ctx.mainWindow) {
            break;
        }
    }
    if (!g_ctx.mainWindow) {
        printf("[!] Window not found\n");
        cleanup();
        CoUninitialize();
        return 1;
    }

    char title[256];
    GetWindowTextA(g_ctx.mainWindow, title, sizeof(title));
    printf("[+] Found: %s (PID: %lu)\n", title, g_ctx.processId);

    // Update process handle to the ACTUAL terminal process
    if (g_ctx.processId != g_pi.dwProcessId) {
        printf("[*] Terminal is child process, updating handle...\n");
        CloseHandle(g_ctx.processHandle); // Close launcher handle

        g_ctx.processHandle = OpenProcess(
            SYNCHRONIZE | PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE,
            FALSE,
            g_ctx.processId
        );
        if (!g_ctx.processHandle) {
            printf("[!] Failed to open terminal process handle (error: %lu)\n", GetLastError());
            cleanup();
            CoUninitialize();
            return 1;
        }
        printf("[+] Updated to monitor PID %lu\n", g_ctx.processId);

        // CRITICAL: Assign the actual terminal process to the job object
        // This ensures it gets terminated when the job object is closed
        if (g_ctx.jobObject) {
            if (AssignProcessToJobObject(g_ctx.jobObject, g_ctx.processHandle)) {
                printf("[+] Assigned terminal process to job object\n");
            } else {
                printf("[!] Failed to assign terminal to job (error: %lu)\n", GetLastError());
                // Continue anyway - we'll still try to terminate it manually
            }
        }
    } else {
        // Same process - job object already assigned
        printf("[+] Terminal is same process as launcher\n");
    }

    // Start monitor thread
    g_ctx.monitorThread = CreateThread(NULL, 0, ProcessMonitorThread, &g_ctx, 0, NULL);
    if (!g_ctx.monitorThread) {
        printf("[!] Failed to create monitor thread\n");
    } else {
        printf("[+] Process monitor thread started\n");
    }

    detect_tab_bar_height(&g_ctx);

    printf("\n[*] Installing hooks...\n");
    g_keyboardHook = SetWindowsHookExA(WH_KEYBOARD_LL, KeyboardHookProc, NULL, 0);
    g_mouseHook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);
    g_winEventHook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_OBJECT_LOCATIONCHANGE, NULL, WinEventCallback, g_ctx.processId, 0, WINEVENT_OUTOFCONTEXT);

    printf("[%c] Keyboard  [%c] Mouse  [%c] Window\n", g_keyboardHook ? '+' : '!', g_mouseHook ? '+' : '!', g_winEventHook ? '+' : '!');

    printf("\n=== Ready ===\n");
    printf("- Click in terminal to see character position\n");
    printf("- Ctrl+C here or close terminal to exit\n\n");

    MSG msg;
    while (g_running && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    cleanup();
    CoUninitialize();

    return 0;
}
