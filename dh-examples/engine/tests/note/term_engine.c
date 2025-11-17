// Terminal Game Engine with Windows Terminal Metrics
//
// Architecture:
//   A (this process) - Game engine backend + WT metrics monitoring
//   B (Windows Terminal) - Display layer (spawned by A)
//   C (term_con_stub) - Console API bridge (child of WT, reads shared memory)
//
// A monitors B's window for metrics (size, font info from settings.json)
// C just renders the shared buffer and captures input
//
// Build:
//   clang term_engine.c -o term_engine.exe -luser32 -lgdi32
//   clang term_engine.c -DCONSTUB -o term_con_stub.exe -luser32 -lgdi32
//
// Run:
//   ./term_engine.exe [path_to_term_con_stub.exe]

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <tlhelp32.h>

// For _stricmp
#ifdef _WIN32
#include <string.h>
#endif

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

// Support both CONSOLE_STUB and CONSTUB defines
#if defined(CONSTUB) && !defined(CONSOLE_STUB)
#define CONSOLE_STUB
#endif

// ============================================================================
// Shared Memory Structure
// ============================================================================

#define SHARED_MEM_NAME    L"TerminalEngineSharedMem"
#define MAX_CONSOLE_WIDTH  512
#define MAX_CONSOLE_HEIGHT 256

typedef struct {
    // Screen buffer (A writes, C renders)
    CHAR_INFO screenBuffer[MAX_CONSOLE_HEIGHT][MAX_CONSOLE_WIDTH];
    SHORT bufferWidth;
    SHORT bufferHeight;
    COORD cursorPos;
    BOOL cursorVisible;

    // Terminal metrics (A calculates from WT window, C uses for reference)
    SHORT gridCols;
    SHORT gridRows;
    double cellWidth;
    double cellHeight;
    int contentAreaX; // Pixel offset to content area
    int contentAreaY; // Pixel offset (after title/tab bar)
    int paddingLeft;
    int paddingTop;
    int paddingRight;
    int paddingBottom;

    // Input events (C writes, A reads)
    INPUT_RECORD inputBuffer[512];
    volatile LONG inputCount;
    volatile LONG inputReadIndex;
    volatile LONG inputWriteIndex;

    // Control flags
    volatile BOOL shouldExit;
    volatile BOOL bufferDirty;
    volatile BOOL metricsUpdated;

    // Synchronization
    volatile LONG lock;
} SharedTerminalState;

// ============================================================================
// Console Stub (C) - Compile with -DCONSOLE_STUB
// ============================================================================

#ifdef CONSOLE_STUB

static HANDLE g_sharedMemHandle = NULL;
static SharedTerminalState* g_state = NULL;
static HANDLE g_consoleOut = NULL;
static HANDLE g_consoleIn = NULL;

static BOOL InitStub(void) {
    g_sharedMemHandle = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, SHARED_MEM_NAME);
    if (!g_sharedMemHandle) {
        return FALSE;
    }

    g_state = (SharedTerminalState*)MapViewOfFile(
        g_sharedMemHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedTerminalState)
    );
    if (!g_state) {
        CloseHandle(g_sharedMemHandle);
        return FALSE;
    }

    g_consoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    g_consoleIn = GetStdHandle(STD_INPUT_HANDLE);

    // Enable mouse input, window events
    DWORD mode = ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT;
    SetConsoleMode(g_consoleIn, mode);

    // Disable quick edit mode (interferes with mouse)
    DWORD outMode;
    GetConsoleMode(g_consoleOut, &outMode);
    outMode &= ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(g_consoleOut, outMode);

    // Hide cursor by default
    CONSOLE_CURSOR_INFO ci = { 1, FALSE };
    SetConsoleCursorInfo(g_consoleOut, &ci);

    return TRUE;
}

static void Render(void) {
    if (!g_state->bufferDirty) {
        return;
    }

    COORD size = { g_state->bufferWidth, g_state->bufferHeight };
    COORD coord = { 0, 0 };
    SMALL_RECT region = { 0, 0, g_state->bufferWidth - 1, g_state->bufferHeight - 1 };

    WriteConsoleOutputW(g_consoleOut, (CHAR_INFO*)g_state->screenBuffer, size, coord, &region);

    if (g_state->cursorVisible) {
        SetConsoleCursorPosition(g_consoleOut, g_state->cursorPos);
    }

    g_state->bufferDirty = FALSE;
}

static void CaptureInput(void) {
    DWORD avail = 0;
    GetNumberOfConsoleInputEvents(g_consoleIn, &avail);
    if (avail == 0) {
        return;
    }

    INPUT_RECORD records[64];
    DWORD read = 0;
    ReadConsoleInputW(g_consoleIn, records, 64, &read);

    for (DWORD i = 0; i < read; i++) {
        LONG idx = InterlockedIncrement(&g_state->inputWriteIndex) - 1;
        g_state->inputBuffer[idx % 512] = records[i];
        InterlockedIncrement(&g_state->inputCount);
    }
}

int main(void) {
    if (!InitStub()) {
        return 1;
    }

    while (!g_state->shouldExit) {
        Render();
        CaptureInput();
        // Sleep(1);
    }

    UnmapViewOfFile(g_state);
    CloseHandle(g_sharedMemHandle);
    return 0;
}

#else
// ============================================================================
// Parent Process (A) - Game Engine with WT Monitoring
// ============================================================================

// ============================================================================
// Windows Terminal Settings Parser
// ============================================================================

typedef struct {
    char fontFace[128];
    int fontSize;
    double lineHeight;
    double cellWidthRatio;
    int paddingLeft, paddingTop, paddingRight, paddingBottom;
} WTSettings;

static char* term_ReadFile(const char* path, size_t* outSize) {
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

// Simple JSON value extraction (no full parser needed)
static BOOL ExtractJsonString(const char* json, const char* key, char* out, size_t outSize) {
    char pattern[256];
    snprintf(pattern, sizeof(pattern), "\"%s\"", key);

    const char* keyPos = strstr(json, pattern);
    if (!keyPos) {
        return FALSE;
    }

    const char* colon = strchr(keyPos + strlen(pattern), ':');
    if (!colon) {
        return FALSE;
    }

    const char* quote1 = strchr(colon, '"');
    if (!quote1) {
        return FALSE;
    }

    quote1++;
    const char* quote2 = strchr(quote1, '"');
    if (!quote2) {
        return FALSE;
    }

    size_t len = quote2 - quote1;
    if (len >= outSize) {
        len = outSize - 1;
    }

    strncpy(out, quote1, len);
    out[len] = '\0';
    return TRUE;
}

static BOOL ExtractJsonInt(const char* json, const char* key, int* out) {
    char pattern[256];
    snprintf(pattern, sizeof(pattern), "\"%s\"", key);

    const char* keyPos = strstr(json, pattern);
    if (!keyPos) {
        return FALSE;
    }

    const char* colon = strchr(keyPos + strlen(pattern), ':');
    if (!colon) {
        return FALSE;
    }

    *out = atoi(colon + 1);
    return TRUE;
}

static BOOL ExtractJsonDouble(const char* json, const char* key, double* out) {
    char pattern[256];
    snprintf(pattern, sizeof(pattern), "\"%s\"", key);

    const char* keyPos = strstr(json, pattern);
    if (!keyPos) {
        return FALSE;
    }

    const char* colon = strchr(keyPos + strlen(pattern), ':');
    if (!colon) {
        return FALSE;
    }

    *out = atof(colon + 1);
    return TRUE;
}

static void ParsePaddingString(const char* str, int* left, int* top, int* right, int* bottom) {
    *left = *top = *right = *bottom = 8; // Default

    int values[4];
    int count = sscanf(str, "%d , %d , %d , %d", &values[0], &values[1], &values[2], &values[3]);

    if (count == 1) {
        *left = *top = *right = *bottom = values[0];
    } else if (count == 4) {
        *left = values[0];
        *top = values[1];
        *right = values[2];
        *bottom = values[3];
    }
}

static WTSettings LoadWTSettings(void) {
    WTSettings settings = {
        .fontFace = "Cascadia Mono",
        .fontSize = 12,
        .lineHeight = 1.2,
        .cellWidthRatio = 0.6,
        .paddingLeft = 8,
        .paddingTop = 8,
        .paddingRight = 8,
        .paddingBottom = 8
    };

    char* localAppData = getenv("LOCALAPPDATA");
    if (!localAppData) {
        return settings;
    }

    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s\\Microsoft\\Windows Terminal\\settings.json", localAppData);

    size_t fileSize;
    char* json = term_ReadFile(path, &fileSize);
    if (!json) {
        snprintf(path, MAX_PATH, "%s\\Packages\\Microsoft.WindowsTerminal_8wekyb3d8bbwe\\LocalState\\settings.json", localAppData);
        json = term_ReadFile(path, &fileSize);
    }
    if (!json) {
        return settings;
    }

    printf("[+] Loaded WT settings: %s\n", path);

    // Find "defaults" section
    const char* defaults = strstr(json, "\"defaults\"");
    if (defaults) {
        const char* obj = strchr(defaults, '{');
        if (obj) {
            // Extract font settings
            const char* fontSection = strstr(obj, "\"font\"");
            if (fontSection) {
                const char* fontObj = strchr(fontSection, '{');
                if (fontObj) {
                    char face[128];
                    if (ExtractJsonString(fontObj, "face", face, sizeof(face))) {
                        strcpy(settings.fontFace, face);
                        printf("    font.face: %s\n", settings.fontFace);
                    }
                    int size;
                    if (ExtractJsonInt(fontObj, "size", &size)) {
                        settings.fontSize = size;
                        printf("    font.size: %d\n", settings.fontSize);
                    }
                }
            }

            // Extract padding
            char paddingStr[64];
            if (ExtractJsonString(obj, "padding", paddingStr, sizeof(paddingStr))) {
                ParsePaddingString(paddingStr, &settings.paddingLeft, &settings.paddingTop, &settings.paddingRight, &settings.paddingBottom);
                printf("    padding: %d,%d,%d,%d\n", settings.paddingLeft, settings.paddingTop, settings.paddingRight, settings.paddingBottom);
            }
        }
    }

    free(json);
    return settings;
}

// ============================================================================
// Cell Size Calculation
// ============================================================================

static void CalculateCellSize(WTSettings* settings, double* cellWidth, double* cellHeight) {
    HDC screenDC = GetDC(NULL);
    int dpi = GetDeviceCaps(screenDC, LOGPIXELSY);
    ReleaseDC(NULL, screenDC);

    double fontPixels = (double)settings->fontSize * dpi / 72.0;
    *cellHeight = round(fontPixels * settings->lineHeight);
    *cellWidth = round(fontPixels * settings->cellWidthRatio);

    printf("[+] Cell size: %.0fx%.0f pixels (font: %dpt @ %d DPI)\n", *cellWidth, *cellHeight, settings->fontSize, dpi);
}

// ============================================================================
// Windows Terminal Window Discovery
// ============================================================================

typedef struct {
    HWND window;
    DWORD processId;
    int titleBarHeight;
} WTWindowInfo;

static BOOL CALLBACK FindWTWindow(HWND hwnd, LPARAM lParam) {
    WTWindowInfo* info = (WTWindowInfo*)lParam;

    char className[256];
    if (!GetClassNameA(hwnd, className, sizeof(className))) {
        return TRUE;
    }

    if (strstr(className, "CASCADIA_HOSTING_WINDOW_CLASS") && IsWindowVisible(hwnd)) {
        info->window = hwnd;
        GetWindowThreadProcessId(hwnd, &info->processId);
        return FALSE;
    }
    return TRUE;
}

static int DetectTitleBarHeight(HWND hwnd) {
    // Method 1: Check for child windows (content area offset)
    RECT childRect;
    HWND child = GetWindow(hwnd, GW_CHILD);
    int bestHeight = 0;

    while (child) {
        char childClass[256];
        if (GetClassNameA(child, childClass, sizeof(childClass))) {
            // Look for composition/content windows
            if (strstr(childClass, "Windows.UI.Composition") || strstr(childClass, "TermControl") || strstr(childClass, "InputSite")) {
                GetWindowRect(child, &childRect);
                POINT childTopLeft = { childRect.left, childRect.top };
                ScreenToClient(hwnd, &childTopLeft);
                // Title bar should be between 30-60 pixels typically
                if (childTopLeft.y >= 30 && childTopLeft.y <= 100) {
                    bestHeight = childTopLeft.y;
                    break;
                }
            }
        }
        child = GetWindow(child, GW_HWNDNEXT);
    }

    if (bestHeight > 0) {
        return bestHeight;
    }

    // Method 2: Use window frame metrics + standard WT title bar
    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);

    POINT clientTopLeft = { 0, 0 };
    ClientToScreen(hwnd, &clientTopLeft);

    int frameTop = clientTopLeft.y - windowRect.top;

    // Windows Terminal uses approximately 32-48px for title/tab bar
    // Common values: 32 (no tabs visible), 40 (with tabs), 48 (with search bar)
    // Default to 40px which covers most cases
    int titleBar = 40;

    // If frame offset is significant, it means standard window frame
    if (frameTop > 0) {
        // Standard frame window - title bar is inside client area
        titleBar = 40;
    }

    return titleBar;
}

// ============================================================================
// Terminal Context (Global State)
// ============================================================================

typedef struct {
    // Shared memory
    HANDLE sharedMemHandle;
    SharedTerminalState* state;

    // Windows Terminal monitoring
    WTSettings settings;
    WTWindowInfo wtWindow;
    double cellWidth;
    double cellHeight;

    // Process management
    PROCESS_INFORMATION stubProcess;
    HANDLE jobObject;
    HANDLE monitorThread;

    // Hooks for input interception (optional)
    HHOOK mouseHook;
    HWINEVENTHOOK winEventHook;

    volatile BOOL running;
} TerminalContext;

static TerminalContext g_ctx = { 0 };

// ============================================================================
// Metrics Update
// ============================================================================

static void UpdateMetrics(void) {
    if (!g_ctx.wtWindow.window) {
        return;
    }

    RECT clientRect;
    GetClientRect(g_ctx.wtWindow.window, &clientRect);

    int contentWidth = clientRect.right - g_ctx.settings.paddingLeft - g_ctx.settings.paddingRight;
    int contentHeight = clientRect.bottom - g_ctx.wtWindow.titleBarHeight - g_ctx.settings.paddingTop - g_ctx.settings.paddingBottom;

    int cols = (g_ctx.cellWidth > 0) ? (int)(contentWidth / g_ctx.cellWidth) : 80;
    int rows = (g_ctx.cellHeight > 0) ? (int)(contentHeight / g_ctx.cellHeight) : 24;

    // Clamp to buffer limits
    if (cols > MAX_CONSOLE_WIDTH) {
        cols = MAX_CONSOLE_WIDTH;
    }
    if (rows > MAX_CONSOLE_HEIGHT) {
        rows = MAX_CONSOLE_HEIGHT;
    }
    if (cols < 1) {
        cols = 1;
    }
    if (rows < 1) {
        rows = 1;
    }

    // Update shared state
    if (g_ctx.state->gridCols != cols || g_ctx.state->gridRows != rows) {
        g_ctx.state->gridCols = (SHORT)cols;
        g_ctx.state->gridRows = (SHORT)rows;
        g_ctx.state->bufferWidth = (SHORT)cols;
        g_ctx.state->bufferHeight = (SHORT)rows;
        g_ctx.state->cellWidth = g_ctx.cellWidth;
        g_ctx.state->cellHeight = g_ctx.cellHeight;
        g_ctx.state->contentAreaX = 0;
        g_ctx.state->contentAreaY = g_ctx.wtWindow.titleBarHeight;
        g_ctx.state->paddingLeft = g_ctx.settings.paddingLeft;
        g_ctx.state->paddingTop = g_ctx.settings.paddingTop;
        g_ctx.state->paddingRight = g_ctx.settings.paddingRight;
        g_ctx.state->paddingBottom = g_ctx.settings.paddingBottom;
        g_ctx.state->metricsUpdated = TRUE;

        printf("[METRICS] Grid: %dx%d, Cell: %.0fx%.0f\n", cols, rows, g_ctx.cellWidth, g_ctx.cellHeight);
    }
}

// ============================================================================
// Win Event Hook (Optional - for resize monitoring)
// ============================================================================

static void CALLBACK WinEventCallback(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD thread, DWORD time) {
    if (hwnd != g_ctx.wtWindow.window) {
        return;
    }

    if (event == EVENT_OBJECT_LOCATIONCHANGE && idObject == OBJID_WINDOW) {
        UpdateMetrics();
    }
}

// ============================================================================
// Mouse Hook (Optional - for precise coordinate mapping)
// ============================================================================

typedef struct {
    int charX, charY;
    int pixelX, pixelY;
    BOOL inContent;
} TerminalPos;

static TerminalPos ScreenToTerminalPos(POINT screenPt) {
    TerminalPos pos = { 0 };

    POINT clientPt = screenPt;
    ScreenToClient(g_ctx.wtWindow.window, &clientPt);

    // Check if in title bar
    if (clientPt.y < g_ctx.wtWindow.titleBarHeight) {
        pos.inContent = FALSE;
        return pos;
    }

    // Convert to content area coordinates
    double contentX = (double)clientPt.x - g_ctx.settings.paddingLeft;
    double contentY = (double)(clientPt.y - g_ctx.wtWindow.titleBarHeight) - g_ctx.settings.paddingTop;

    pos.pixelX = (int)contentX;
    pos.pixelY = (int)contentY;

    // Check bounds
    double maxX = g_ctx.state->gridCols * g_ctx.cellWidth;
    double maxY = g_ctx.state->gridRows * g_ctx.cellHeight;

    if (contentX < 0 || contentY < 0 || contentX >= maxX || contentY >= maxY) {
        pos.inContent = FALSE;
        return pos;
    }

    pos.inContent = TRUE;
    pos.charX = (int)floor(contentX / g_ctx.cellWidth);
    pos.charY = (int)floor(contentY / g_ctx.cellHeight);

    return pos;
}

static LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        MSLLHOOKSTRUCT* ms = (MSLLHOOKSTRUCT*)lParam;
        HWND root = GetAncestor(WindowFromPoint(ms->pt), GA_ROOT);

        if (root == g_ctx.wtWindow.window && wParam == WM_LBUTTONDOWN) {
            TerminalPos tpos = ScreenToTerminalPos(ms->pt);
            if (tpos.inContent) {
                // Inject mouse event into shared input buffer
                INPUT_RECORD rec = { 0 };
                rec.EventType = MOUSE_EVENT;
                rec.Event.MouseEvent.dwMousePosition.X = (SHORT)tpos.charX;
                rec.Event.MouseEvent.dwMousePosition.Y = (SHORT)tpos.charY;
                rec.Event.MouseEvent.dwButtonState = FROM_LEFT_1ST_BUTTON_PRESSED;
                rec.Event.MouseEvent.dwEventFlags = 0;

                LONG idx = InterlockedIncrement(&g_ctx.state->inputWriteIndex) - 1;
                g_ctx.state->inputBuffer[idx % 512] = rec;
                InterlockedIncrement(&g_ctx.state->inputCount);
            }
        }
    }
    return CallNextHookEx(g_ctx.mouseHook, nCode, wParam, lParam);
}

// ============================================================================
// Process Monitor Thread
// ============================================================================

static DWORD WINAPI ProcessMonitorThread(LPVOID param) {
    // Wait for WT window to close
    while (g_ctx.running) {
        if (!IsWindow(g_ctx.wtWindow.window)) {
            printf("[!] Windows Terminal window closed\n");
            g_ctx.running = FALSE;
            break;
        }
        Sleep(100);
    }
    return 0;
}

// ============================================================================
// Setup & Cleanup
// ============================================================================

static BOOL CreateSharedMem(void) {
    g_ctx.sharedMemHandle = CreateFileMappingW(
        INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
        sizeof(SharedTerminalState), SHARED_MEM_NAME
    );
    if (!g_ctx.sharedMemHandle) {
        return FALSE;
    }

    g_ctx.state = (SharedTerminalState*)MapViewOfFile(
        g_ctx.sharedMemHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedTerminalState)
    );
    if (!g_ctx.state) {
        CloseHandle(g_ctx.sharedMemHandle);
        return FALSE;
    }

    memset(g_ctx.state, 0, sizeof(SharedTerminalState));
    return TRUE;
}

static BOOL SpawnWTWithStub(const char* stubPath) {
    g_ctx.jobObject = CreateJobObjectW(NULL, NULL);
    if (g_ctx.jobObject) {
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION info = { 0 };
        info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        SetInformationJobObject(g_ctx.jobObject, JobObjectExtendedLimitInformation, &info, sizeof(info));
    }

    char cmdLine[MAX_PATH * 2];
    snprintf(cmdLine, sizeof(cmdLine), "wt.exe new-tab --title \"Game\" -- \"%s\"", stubPath);

    STARTUPINFOA si = { .cb = sizeof(si) };
    if (!CreateProcessA(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &g_ctx.stubProcess)) {
        return FALSE;
    }

    if (g_ctx.jobObject) {
        AssignProcessToJobObject(g_ctx.jobObject, g_ctx.stubProcess.hProcess);
    }

    return TRUE;
}

static BOOL FindWTWindowWithRetry(void) {
    for (int i = 0; i < 50 && g_ctx.running; i++) {
        Sleep(100);
        g_ctx.wtWindow.window = NULL;
        EnumWindows(FindWTWindow, (LPARAM)&g_ctx.wtWindow);
        if (g_ctx.wtWindow.window) {
            g_ctx.wtWindow.titleBarHeight = DetectTitleBarHeight(g_ctx.wtWindow.window);
            return TRUE;
        }
    }
    return FALSE;
}

static void Cleanup(void) {
    g_ctx.running = FALSE;

    if (g_ctx.state) {
        g_ctx.state->shouldExit = TRUE;
    }

    if (g_ctx.mouseHook) {
        UnhookWindowsHookEx(g_ctx.mouseHook);
    }
    if (g_ctx.winEventHook) {
        UnhookWinEvent(g_ctx.winEventHook);
    }

    if (g_ctx.monitorThread) {
        WaitForSingleObject(g_ctx.monitorThread, 1000);
        CloseHandle(g_ctx.monitorThread);
    }

    if (g_ctx.stubProcess.hProcess) {
        TerminateProcess(g_ctx.stubProcess.hProcess, 0);
        CloseHandle(g_ctx.stubProcess.hProcess);
        CloseHandle(g_ctx.stubProcess.hThread);
    }

    if (g_ctx.state) {
        UnmapViewOfFile(g_ctx.state);
    }
    if (g_ctx.sharedMemHandle) {
        CloseHandle(g_ctx.sharedMemHandle);
    }
    if (g_ctx.jobObject) {
        CloseHandle(g_ctx.jobObject);
    }
}

static BOOL WINAPI CtrlHandler(DWORD type) {
    if (type == CTRL_C_EVENT) {
        g_ctx.running = FALSE;
        PostQuitMessage(0);
        return TRUE;
    }
    return FALSE;
}

// ============================================================================
// Game Engine API
// ============================================================================

void TermSetCell(int x, int y, wchar_t ch, WORD attr) {
    if (x >= 0 && x < g_ctx.state->bufferWidth && y >= 0 && y < g_ctx.state->bufferHeight) {
        g_ctx.state->screenBuffer[y][x].Char.UnicodeChar = ch;
        g_ctx.state->screenBuffer[y][x].Attributes = attr;
    }
}

void TermClear(wchar_t ch, WORD attr) {
    for (int y = 0; y < g_ctx.state->bufferHeight; y++) {
        for (int x = 0; x < g_ctx.state->bufferWidth; x++) {
            g_ctx.state->screenBuffer[y][x].Char.UnicodeChar = ch;
            g_ctx.state->screenBuffer[y][x].Attributes = attr;
        }
    }
}

void TermPresent(void) {
    g_ctx.state->bufferDirty = TRUE;
}

BOOL TermPollInput(INPUT_RECORD* out) {
    if (g_ctx.state->inputCount <= 0) {
        return FALSE;
    }

    LONG idx = InterlockedIncrement(&g_ctx.state->inputReadIndex) - 1;
    *out = g_ctx.state->inputBuffer[idx % 512];
    InterlockedDecrement(&g_ctx.state->inputCount);
    return TRUE;
}

void TermGetSize(int* cols, int* rows) {
    *cols = g_ctx.state->gridCols;
    *rows = g_ctx.state->gridRows;
}

BOOL TermWasResized(void) {
    BOOL r = g_ctx.state->metricsUpdated;
    g_ctx.state->metricsUpdated = FALSE;
    return r;
}

// ============================================================================
// Main
// ============================================================================

int main(int argc, char* argv[]) {
    printf("=== Terminal Game Engine ===\n\n");

    // Handle stub path - add .exe if not present and convert to absolute path
    char stubPathBuf[MAX_PATH];
    char absolutePath[MAX_PATH];
    const char* stubPath;

    if (argc > 1) {
        const char* arg = argv[1];
        size_t len = strlen(arg);
        // Check if .exe extension is missing
        if (len < 4 || _stricmp(arg + len - 4, ".exe") != 0) {
            snprintf(stubPathBuf, sizeof(stubPathBuf), "%s.exe", arg);
            stubPath = stubPathBuf;
        } else {
            stubPath = arg;
        }
    } else {
        stubPath = "console_stub.exe";
    }

    // Convert to absolute path (Windows Terminal runs from different directory)
    if (!GetFullPathNameA(stubPath, MAX_PATH, absolutePath, NULL)) {
        printf("[!] Failed to resolve absolute path for: %s\n", stubPath);
        return 1;
    }
    stubPath = absolutePath;
    printf("[*] Stub path: %s\n", stubPath);

    // Verify stub exists
    if (GetFileAttributesA(stubPath) == INVALID_FILE_ATTRIBUTES) {
        printf("[!] Stub executable not found: %s\n", stubPath);
        printf("    Please compile with: clang %s -DCONSTUB -o %s -luser32 -lgdi32\n", __FILE__, stubPath);
        return 1;
    }

    g_ctx.running = TRUE;
    SetConsoleCtrlHandler(CtrlHandler, TRUE);

    // 1. Load Windows Terminal settings
    printf("[*] Loading Windows Terminal settings...\n");
    g_ctx.settings = LoadWTSettings();
    CalculateCellSize(&g_ctx.settings, &g_ctx.cellWidth, &g_ctx.cellHeight);

    // 2. Create shared memory
    printf("[*] Creating shared memory...\n");
    if (!CreateSharedMem()) {
        printf("[!] Failed to create shared memory\n");
        return 1;
    }

    // 3. Spawn Windows Terminal with console stub
    printf("[*] Spawning Windows Terminal...\n");
    if (!SpawnWTWithStub(stubPath)) {
        printf("[!] Failed to spawn Windows Terminal: %lu\n", GetLastError());
        Cleanup();
        return 1;
    }
    printf("[+] Launched (PID: %lu)\n", g_ctx.stubProcess.dwProcessId);

    // 4. Find Windows Terminal window
    printf("[*] Waiting for Windows Terminal window...\n");
    if (!FindWTWindowWithRetry()) {
        printf("[!] Could not find Windows Terminal window\n");
        Cleanup();
        return 1;
    }
    printf("[+] Found WT window (PID: %lu, TitleBar: %dpx)\n", g_ctx.wtWindow.processId, g_ctx.wtWindow.titleBarHeight);

    // 5. Initial metrics calculation
    UpdateMetrics();

    // 6. Setup hooks (optional - for precise input/resize monitoring)
    g_ctx.winEventHook = SetWinEventHook(
        EVENT_OBJECT_LOCATIONCHANGE, EVENT_OBJECT_LOCATIONCHANGE,
        NULL, WinEventCallback, g_ctx.wtWindow.processId, 0, WINEVENT_OUTOFCONTEXT
    );
    g_ctx.mouseHook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);

    printf("[+] Hooks: WinEvent=%s, Mouse=%s\n", g_ctx.winEventHook ? "OK" : "FAIL", g_ctx.mouseHook ? "OK" : "FAIL");

    // 7. Start process monitor
    g_ctx.monitorThread = CreateThread(NULL, 0, ProcessMonitorThread, NULL, 0, NULL);

    printf("\n=== Ready ===\n");
    printf("Grid: %dx%d, Cell: %.0fx%.0f\n", g_ctx.state->gridCols, g_ctx.state->gridRows, g_ctx.cellWidth, g_ctx.cellHeight);
    printf("Press Ctrl+C or close terminal to exit\n\n");

    // 8. Demo game loop
    int frame = 0;
    int px = 5, py = 5;

    while (g_ctx.running) {
        // Process Windows messages (for hooks)
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                g_ctx.running = FALSE;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Handle resize
        if (TermWasResized()) {
            printf("[RESIZE] %dx%d\n", g_ctx.state->gridCols, g_ctx.state->gridRows);
        }

        // Process input
        INPUT_RECORD input;
        while (TermPollInput(&input)) {
            if (input.EventType == KEY_EVENT && input.Event.KeyEvent.bKeyDown) {
                switch (input.Event.KeyEvent.wVirtualKeyCode) {
                case VK_UP:
                    py = (py > 0) ? py - 1 : py;
                    break;
                case VK_DOWN:
                    py = (py < g_ctx.state->gridRows - 1) ? py + 1 : py;
                    break;
                case VK_LEFT:
                    px = (px > 0) ? px - 1 : px;
                    break;
                case VK_RIGHT:
                    px = (px < g_ctx.state->gridCols - 1) ? px + 1 : px;
                    break;
                case VK_ESCAPE:
                    g_ctx.running = FALSE;
                    break;
                }
            } else if (input.EventType == MOUSE_EVENT) {
                if (input.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
                    px = input.Event.MouseEvent.dwMousePosition.X;
                    py = input.Event.MouseEvent.dwMousePosition.Y;
                    printf("[CLICK] (%d,%d)\n", px, py);
                }
            }
        }

        // Render
        TermClear(L' ', 0x07);

        // Border
        for (int x = 0; x < g_ctx.state->gridCols; x++) {
            TermSetCell(x, 0, L'=', 0x0A);
            TermSetCell(x, g_ctx.state->gridRows - 1, L'=', 0x0A);
        }

        // Player
        TermSetCell(px, py, L'@', 0x0C);

        // Info
        wchar_t info[128];
        swprintf(info, 128, L"Frame:%d Pos:(%d,%d) Grid:%dx%d", frame, px, py, g_ctx.state->gridCols, g_ctx.state->gridRows);
        for (int i = 0; info[i] && i + 2 < g_ctx.state->gridCols; i++) {
            TermSetCell(2 + i, 1, info[i], 0x0F);
        }

        TermPresent();

        frame++;
        Sleep(16);
    }

    Cleanup();
    printf("[+] Exited cleanly\n");
    return 0;
}

#endif
