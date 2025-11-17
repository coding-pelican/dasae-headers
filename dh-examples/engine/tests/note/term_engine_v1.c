// Enhanced Terminal Game Engine
//
// Features:
//   1. Double-buffered shared memory with atomic swap
//   2. Accurate title bar height detection (UI Automation + fallbacks)
//   3. Window focus/minimize/maximize/mouse wheel/movement tracking
//   4. FPS measurement
//
// Build:
//   clang term_engine_enhanced.c -o term_engine.exe -luser32 -lgdi32 -lole32 -loleaut32 -static -O3 -flto
//   clang term_engine_enhanced.c -DCONSTUB -o term_engine_con_stub.exe -luser32 -lgdi32 -static -O3 -flto
//
// Run:
//   ./term_engine term_engine_con_stub
//
// Dependencies: Minimal - uses COM/UI Automation which is built into Windows 11

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define COBJMACROS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <tlhelp32.h>

#define INITGUID
#include <initguid.h> // <---- required for MinGW/Clang
#ifndef CONSOLE_STUB
#include <uiautomation.h>
#include <oleauto.h>
#endif

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#ifndef CONSOLE_STUB
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#endif

#if defined(CONSTUB) && !defined(CONSOLE_STUB)
#define CONSOLE_STUB
#endif

// ============================================================================
// Shared Memory Structure - Double Buffered
// ============================================================================

#define SHARED_MEM_NAME    L"TerminalEngineSharedMem"
#define MAX_CONSOLE_WIDTH  512
#define MAX_CONSOLE_HEIGHT 256

// Single frame buffer
typedef struct {
    CHAR_INFO screenBuffer[MAX_CONSOLE_HEIGHT][MAX_CONSOLE_WIDTH];
    SHORT bufferWidth;
    SHORT bufferHeight;
    COORD cursorPos;
    BOOL cursorVisible;
    LARGE_INTEGER timestamp; // When this frame was completed
} FrameBuffer;

// Window state events
typedef enum {
    WINDOW_EVENT_NONE = 0,
    WINDOW_EVENT_FOCUS_GAINED,
    WINDOW_EVENT_FOCUS_LOST,
    WINDOW_EVENT_MINIMIZED,
    WINDOW_EVENT_MAXIMIZED,
    WINDOW_EVENT_RESTORED,
    WINDOW_EVENT_MOVED,
    WINDOW_EVENT_RESIZED
} WindowEventType;

typedef struct {
    WindowEventType type;
    LARGE_INTEGER timestamp;
    union {
        struct {
            int x, y, width, height;
        } rect;       // For moved/resized
        BOOL focused; // For focus events
    } data;
} WindowEvent;

typedef struct {
    // === Double-buffered frame data ===
    FrameBuffer frames[2];
    volatile LONG currentReadBuffer; // Which buffer the stub should read (0 or 1)
    volatile LONG writeInProgress;   // Lock for atomic swap

    // === Terminal metrics ===
    SHORT gridCols;
    SHORT gridRows;
    double cellWidth;
    double cellHeight;
    int contentAreaX;
    int contentAreaY;
    int paddingLeft;
    int paddingTop;
    int paddingRight;
    int paddingBottom;

    // === Input events (circular buffer) ===
    INPUT_RECORD inputBuffer[512];
    volatile LONG inputCount;
    volatile LONG inputReadIndex;
    volatile LONG inputWriteIndex;

    // === Window events (circular buffer) ===
    WindowEvent windowEvents[64];
    volatile LONG windowEventCount;
    volatile LONG windowEventReadIndex;
    volatile LONG windowEventWriteIndex;

    // === Mouse wheel state ===
    volatile LONG mouseWheelDelta; // Accumulated wheel delta

    // === Control flags ===
    volatile BOOL shouldExit;
    volatile BOOL metricsUpdated;

    // === FPS tracking ===
    volatile LONG frameCount;
    volatile LARGE_INTEGER lastFpsTime;
    volatile float currentFps;

    // === Synchronization ===
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
static LONG g_lastRenderedBuffer = -1;

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

    DWORD mode = ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT;
    SetConsoleMode(g_consoleIn, mode);

    DWORD outMode;
    GetConsoleMode(g_consoleOut, &outMode);
    outMode &= ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(g_consoleOut, outMode);

    CONSOLE_CURSOR_INFO ci = { 1, FALSE };
    SetConsoleCursorInfo(g_consoleOut, &ci);

    return TRUE;
}

static void Render(void) {
    // Check which buffer to read (atomic read)
    LONG bufferIndex = InterlockedCompareExchange(&g_state->currentReadBuffer, 0, 0);

    // Only render if buffer changed
    if (bufferIndex == g_lastRenderedBuffer) {
        return;
    }

    const FrameBuffer* frame = &g_state->frames[bufferIndex];

    COORD size = { frame->bufferWidth, frame->bufferHeight };
    COORD coord = { 0, 0 };
    SMALL_RECT region = { 0, 0, frame->bufferWidth - 1, frame->bufferHeight - 1 };

    WriteConsoleOutputW(g_consoleOut, (CHAR_INFO*)frame->screenBuffer, size, coord, &region);

    if (frame->cursorVisible) {
        SetConsoleCursorPosition(g_consoleOut, frame->cursorPos);
    }

    g_lastRenderedBuffer = bufferIndex;
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
        // Handle mouse wheel specially
        if (records[i].EventType == MOUSE_EVENT && (records[i].Event.MouseEvent.dwEventFlags & MOUSE_WHEELED)) {
            short delta = (short)HIWORD(records[i].Event.MouseEvent.dwButtonState);
            InterlockedAdd(&g_state->mouseWheelDelta, delta);
        }

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
        Sleep(1); // Small sleep to prevent busy-waiting
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
    *left = *top = *right = *bottom = 8;
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

static WTSettings term_loadWTSettings(void) {
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

    const char* defaults = strstr(json, "\"defaults\"");
    if (defaults) {
        const char* obj = strchr(defaults, '{');
        if (obj) {
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

static void term_calculateCellSize(WTSettings* settings, double* cellWidth, double* cellHeight) {
    HDC screenDC = GetDC(NULL);
    int dpi = GetDeviceCaps(screenDC, LOGPIXELSY);
    ReleaseDC(NULL, screenDC);

    double fontPixels = (double)settings->fontSize * dpi / 72.0;
    *cellHeight = round(fontPixels * settings->lineHeight);
    *cellWidth = round(fontPixels * settings->cellWidthRatio);

    printf("[+] Cell size: %.0fx%.0f pixels (font: %dpt @ %d DPI)\n", *cellWidth, *cellHeight, settings->fontSize, dpi);
}

// ============================================================================
// Title Bar Height Detection (UI Automation - Windows 11 compatible)
// ============================================================================

static int GetTitleBarHeightByChildren(HWND hwnd) {
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
        IUIAutomation_Release(automation);
        return 0;
    }

    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);

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

                    // Check for title bar: Pane (50033) or TitleBar (50037), height 30-50px, near top
                    if (i < 3 && (controlType == 50033 || controlType == 50037) && elementHeight >= 30 && elementHeight <= 50 && relativeTop < 10) {
                        height = elementHeight;
                        printf("[+] Found title bar (Child %d): height=%d\n", i, height);
                        if (name) {
                            SysFreeString(name);
                        }
                        IUIAutomationElement_Release(child);
                        break;
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

    if (windowElement) {
        IUIAutomationElement_Release(windowElement);
    }
    if (automation) {
        IUIAutomation_Release(automation);
    }

    return height;
}

static int GetTitleBarHeightUIA(HWND hwnd) {
    HRESULT hr;
    IUIAutomation* automation = NULL;
    IUIAutomationElement* rootElement = NULL;
    IUIAutomationElement* titleBarElement = NULL;
    IUIAutomationCondition* condition = NULL;
    int height = 0;

    hr = CoCreateInstance(&CLSID_CUIAutomation, NULL, CLSCTX_INPROC_SERVER, &IID_IUIAutomation, (void**)&automation);
    if (FAILED(hr)) {
        return 0;
    }

    hr = IUIAutomation_ElementFromHandle(automation, hwnd, &rootElement);
    if (FAILED(hr)) {
        goto cleanup;
    }

    VARIANT varProp;
    VariantInit(&varProp);
    varProp.vt = VT_I4;
    varProp.lVal = UIA_TitleBarControlTypeId;

    hr = IUIAutomation_CreatePropertyCondition(automation, UIA_ControlTypePropertyId, varProp, &condition);
    VariantClear(&varProp);
    if (FAILED(hr)) {
        goto cleanup;
    }

    hr = IUIAutomationElement_FindFirst(rootElement, TreeScope_Descendants, condition, &titleBarElement);
    if (SUCCEEDED(hr) && titleBarElement) {
        RECT titleBarRect;
        hr = IUIAutomationElement_get_CurrentBoundingRectangle(titleBarElement, &titleBarRect);
        if (SUCCEEDED(hr)) {
            height = titleBarRect.bottom - titleBarRect.top;
            printf("[+] Title bar UIA: height=%d\n", height);
        }
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

static int DetectTitleBarHeight(HWND hwnd) {
    // Method 1: UI Automation - most accurate
    int height = GetTitleBarHeightUIA(hwnd);
    if (height > 0 && height < 200) {
        return height;
    }

    // Method 2: Child element analysis
    height = GetTitleBarHeightByChildren(hwnd);
    if (height > 0 && height < 200) {
        return height;
    }

    // Method 3: Child window enumeration
    HWND child = GetWindow(hwnd, GW_CHILD);
    while (child) {
        char childClass[256];
        if (GetClassNameA(child, childClass, sizeof(childClass))) {
            if (strstr(childClass, "Windows.UI.Composition") || strstr(childClass, "TermControl") || strstr(childClass, "InputSite")) {
                RECT childRect;
                GetWindowRect(child, &childRect);
                POINT childTopLeft = { childRect.left, childRect.top };
                ScreenToClient(hwnd, &childTopLeft);
                if (childTopLeft.y >= 30 && childTopLeft.y <= 100) {
                    return childTopLeft.y;
                }
            }
        }
        child = GetWindow(child, GW_HWNDNEXT);
    }

    // Method 4: Fallback estimate
    return 40;
}

// ============================================================================
// Terminal Context (Global State)
// ============================================================================

typedef struct {
    HWND window;
    DWORD processId;
    int titleBarHeight;
} WTWindowInfo;

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

    // Hooks
    HHOOK mouseHook;
    HWINEVENTHOOK winEventHook;

    // Double buffering
    LONG currentWriteBuffer; // Which buffer we're writing to (0 or 1)

    // FPS tracking
    LARGE_INTEGER perfFrequency;
    LARGE_INTEGER lastFpsCalcTime;
    LONG framesSinceLastCalc;

    volatile BOOL running;
} TerminalContext;

static TerminalContext g_ctx = { 0 };

// ============================================================================
// Double-Buffered Frame Writing
// ============================================================================

static FrameBuffer* GetWriteBuffer(void) {
    return &g_ctx.state->frames[g_ctx.currentWriteBuffer];
}

static void term_swapBuffers(void) {
    // Mark write as in progress
    InterlockedExchange(&g_ctx.state->writeInProgress, 1);

    // Get timestamp
    LARGE_INTEGER timestamp;
    QueryPerformanceCounter(&timestamp);
    GetWriteBuffer()->timestamp = timestamp;

    // Atomic swap - flip the read buffer index
    LONG newReadBuffer = g_ctx.currentWriteBuffer;
    InterlockedExchange(&g_ctx.state->currentReadBuffer, newReadBuffer);

    // Switch our write buffer to the other one
    g_ctx.currentWriteBuffer = 1 - g_ctx.currentWriteBuffer;

    // Mark write as complete
    InterlockedExchange(&g_ctx.state->writeInProgress, 0);

    // Update frame count for FPS
    InterlockedIncrement(&g_ctx.state->frameCount);
    g_ctx.framesSinceLastCalc++;
}

// ============================================================================
// FPS Measurement
// ============================================================================

static void term_updateFps(void) {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    double elapsed = (double)(now.QuadPart - g_ctx.lastFpsCalcTime.QuadPart) / (double)g_ctx.perfFrequency.QuadPart;

    if (elapsed >= 1.0) { // Update every second
        float fps = (float)g_ctx.framesSinceLastCalc / (float)elapsed;
        g_ctx.state->currentFps = fps;
        g_ctx.framesSinceLastCalc = 0;
        g_ctx.lastFpsCalcTime = now;
    }
}

static float term_getFPS(void) {
    return g_ctx.state->currentFps;
}

// ============================================================================
// Window Event Tracking
// ============================================================================

static void term_pushWindowEvent(WindowEventType type) {
    WindowEvent evt = { 0 };
    evt.type = type;
    QueryPerformanceCounter(&evt.timestamp);

    if (type == WINDOW_EVENT_MOVED || type == WINDOW_EVENT_RESIZED) {
        RECT rect;
        GetWindowRect(g_ctx.wtWindow.window, &rect);
        evt.data.rect.x = rect.left;
        evt.data.rect.y = rect.top;
        evt.data.rect.width = rect.right - rect.left;
        evt.data.rect.height = rect.bottom - rect.top;
    }

    LONG idx = InterlockedIncrement(&g_ctx.state->windowEventWriteIndex) - 1;
    g_ctx.state->windowEvents[idx % 64] = evt;
    InterlockedIncrement(&g_ctx.state->windowEventCount);
}

static BOOL term_pollWindowEvent(WindowEvent* out) {
    if (g_ctx.state->windowEventCount <= 0) {
        return FALSE;
    }

    LONG idx = InterlockedIncrement(&g_ctx.state->windowEventReadIndex) - 1;
    *out = g_ctx.state->windowEvents[idx % 64];
    InterlockedDecrement(&g_ctx.state->windowEventCount);
    return TRUE;
}

static LONG term_getMouseWheelDelta(void) {
    return InterlockedExchange(&g_ctx.state->mouseWheelDelta, 0);
}

// ============================================================================
// Metrics Update
// ============================================================================

static void term_updateMetrics(void) {
    if (!g_ctx.wtWindow.window) {
        return;
    }

    RECT clientRect;
    GetClientRect(g_ctx.wtWindow.window, &clientRect);

    int contentWidth = clientRect.right - g_ctx.settings.paddingLeft - g_ctx.settings.paddingRight;
    int contentHeight = clientRect.bottom - g_ctx.wtWindow.titleBarHeight - g_ctx.settings.paddingTop - g_ctx.settings.paddingBottom;

    int cols = (g_ctx.cellWidth > 0) ? (int)(contentWidth / g_ctx.cellWidth) : 80;
    int rows = (g_ctx.cellHeight > 0) ? (int)(contentHeight / g_ctx.cellHeight) : 24;

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

    if (g_ctx.state->gridCols != cols || g_ctx.state->gridRows != rows) {
        g_ctx.state->gridCols = (SHORT)cols;
        g_ctx.state->gridRows = (SHORT)rows;

        // Update both frame buffers
        g_ctx.state->frames[0].bufferWidth = (SHORT)cols;
        g_ctx.state->frames[0].bufferHeight = (SHORT)rows;
        g_ctx.state->frames[1].bufferWidth = (SHORT)cols;
        g_ctx.state->frames[1].bufferHeight = (SHORT)rows;

        g_ctx.state->cellWidth = g_ctx.cellWidth;
        g_ctx.state->cellHeight = g_ctx.cellHeight;
        g_ctx.state->contentAreaX = 0;
        g_ctx.state->contentAreaY = g_ctx.wtWindow.titleBarHeight;
        g_ctx.state->paddingLeft = g_ctx.settings.paddingLeft;
        g_ctx.state->paddingTop = g_ctx.settings.paddingTop;
        g_ctx.state->paddingRight = g_ctx.settings.paddingRight;
        g_ctx.state->paddingBottom = g_ctx.settings.paddingBottom;
        g_ctx.state->metricsUpdated = TRUE;

        term_pushWindowEvent(WINDOW_EVENT_RESIZED);
        printf("[METRICS] Grid: %dx%d, Cell: %.0fx%.0f\n", cols, rows, g_ctx.cellWidth, g_ctx.cellHeight);
    }
}

// ============================================================================
// Win Event Hook (Extended for focus/minimize/maximize/movement)
// ============================================================================

static void CALLBACK WinEventCallback(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD thread, DWORD time) {
    if (hwnd != g_ctx.wtWindow.window) {
        return;
    }

    switch (event) {
    case EVENT_OBJECT_LOCATIONCHANGE:
        if (idObject == OBJID_WINDOW) {
            term_updateMetrics();
            term_pushWindowEvent(WINDOW_EVENT_MOVED);
        }
        break;

    case EVENT_SYSTEM_FOREGROUND:
        term_pushWindowEvent(WINDOW_EVENT_FOCUS_GAINED);
        printf("[FOCUS] Window gained focus\n");
        break;

    case EVENT_SYSTEM_MINIMIZESTART:
        term_pushWindowEvent(WINDOW_EVENT_MINIMIZED);
        printf("[WINDOW] Minimized\n");
        break;

    case EVENT_SYSTEM_MINIMIZEEND:
        term_pushWindowEvent(WINDOW_EVENT_RESTORED);
        printf("[WINDOW] Restored from minimize\n");
        break;

    case EVENT_OBJECT_STATECHANGE:
        if (idObject == OBJID_WINDOW) {
            WINDOWPLACEMENT wp = { .length = sizeof(wp) };
            if (GetWindowPlacement(hwnd, &wp)) {
                if (wp.showCmd == SW_SHOWMAXIMIZED) {
                    term_pushWindowEvent(WINDOW_EVENT_MAXIMIZED);
                    printf("[WINDOW] Maximized\n");
                } else if (wp.showCmd == SW_SHOWNORMAL) {
                    term_pushWindowEvent(WINDOW_EVENT_RESTORED);
                    printf("[WINDOW] Restored\n");
                }
            }
        }
        break;
    }
}

// Also track focus loss through GetForegroundWindow changes
static void term_checkFocusChanged(void) {
    static BOOL wasFocused = FALSE;
    BOOL isFocused = (GetForegroundWindow() == g_ctx.wtWindow.window);

    if (isFocused != wasFocused) {
        if (!isFocused) {
            term_pushWindowEvent(WINDOW_EVENT_FOCUS_LOST);
            printf("[FOCUS] Window lost focus\n");
        }
        wasFocused = isFocused;
    }
}

// ============================================================================
// Mouse Hook (with wheel support)
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

    if (clientPt.y < g_ctx.wtWindow.titleBarHeight) {
        pos.inContent = FALSE;
        return pos;
    }

    double contentX = (double)clientPt.x - g_ctx.settings.paddingLeft;
    double contentY = (double)(clientPt.y - g_ctx.wtWindow.titleBarHeight) - g_ctx.settings.paddingTop;

    pos.pixelX = (int)contentX;
    pos.pixelY = (int)contentY;

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

        if (root == g_ctx.wtWindow.window) {
            TerminalPos tpos = ScreenToTerminalPos(ms->pt);

            if (wParam == WM_LBUTTONDOWN && tpos.inContent) {
                INPUT_RECORD rec = { 0 };
                rec.EventType = MOUSE_EVENT;
                rec.Event.MouseEvent.dwMousePosition.X = (SHORT)tpos.charX;
                rec.Event.MouseEvent.dwMousePosition.Y = (SHORT)tpos.charY;
                rec.Event.MouseEvent.dwButtonState = FROM_LEFT_1ST_BUTTON_PRESSED;
                rec.Event.MouseEvent.dwEventFlags = 0;

                LONG idx = InterlockedIncrement(&g_ctx.state->inputWriteIndex) - 1;
                g_ctx.state->inputBuffer[idx % 512] = rec;
                InterlockedIncrement(&g_ctx.state->inputCount);
            } else if (wParam == WM_MOUSEWHEEL) {
                short delta = (short)HIWORD(ms->mouseData);
                InterlockedAdd(&g_ctx.state->mouseWheelDelta, delta);
                printf("[WHEEL] delta=%d\n", delta);
            }
        }
    }
    return CallNextHookEx(g_ctx.mouseHook, nCode, wParam, lParam);
}

// ============================================================================
// Process Monitor Thread
// ============================================================================

static DWORD WINAPI ProcessMonitorThread(LPVOID param) {
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

static BOOL term_createSharedMem(void) {
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

    // Initialize double buffering
    g_ctx.currentWriteBuffer = 0;
    g_ctx.state->currentReadBuffer = 0;

    return TRUE;
}

static BOOL term_spawnWTWithStub(const char* stubPath) {
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

static BOOL term_findWTWindowWithRetry(void) {
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

void term_setCell(int x, int y, wchar_t ch, WORD attr) {
    FrameBuffer* frame = GetWriteBuffer();
    if (x >= 0 && x < frame->bufferWidth && y >= 0 && y < frame->bufferHeight) {
        frame->screenBuffer[y][x].Char.UnicodeChar = ch;
        frame->screenBuffer[y][x].Attributes = attr;
    }
}

void term_clear(wchar_t ch, WORD attr) {
    FrameBuffer* frame = GetWriteBuffer();
    for (int y = 0; y < frame->bufferHeight; y++) {
        for (int x = 0; x < frame->bufferWidth; x++) {
            frame->screenBuffer[y][x].Char.UnicodeChar = ch;
            frame->screenBuffer[y][x].Attributes = attr;
        }
    }
}

void term_present(void) {
    term_swapBuffers();
    term_updateFps();
}

BOOL term_pollInput(INPUT_RECORD* out) {
    if (g_ctx.state->inputCount <= 0) {
        return FALSE;
    }

    LONG idx = InterlockedIncrement(&g_ctx.state->inputReadIndex) - 1;
    *out = g_ctx.state->inputBuffer[idx % 512];
    InterlockedDecrement(&g_ctx.state->inputCount);
    return TRUE;
}

void term_getSize(int* cols, int* rows) {
    *cols = g_ctx.state->gridCols;
    *rows = g_ctx.state->gridRows;
}

BOOL term_wasResized(void) {
    BOOL r = g_ctx.state->metricsUpdated;
    g_ctx.state->metricsUpdated = FALSE;
    return r;
}

// ============================================================================
// Main
// ============================================================================

int main(int argc, char* argv[]) {
    printf("=== Terminal Game Engine (Enhanced) ===\n\n");

    // Initialize COM for UI Automation
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    // Initialize performance counter
    QueryPerformanceFrequency(&g_ctx.perfFrequency);
    QueryPerformanceCounter(&g_ctx.lastFpsCalcTime);

    char stubPathBuf[MAX_PATH];
    char absolutePath[MAX_PATH];
    const char* stubPath;

    if (argc > 1) {
        const char* arg = argv[1];
        size_t len = strlen(arg);
        if (len < 4 || _stricmp(arg + len - 4, ".exe") != 0) {
            snprintf(stubPathBuf, sizeof(stubPathBuf), "%s.exe", arg);
            stubPath = stubPathBuf;
        } else {
            stubPath = arg;
        }
    } else {
        stubPath = "console_stub.exe";
    }

    if (!GetFullPathNameA(stubPath, MAX_PATH, absolutePath, NULL)) {
        printf("[!] Failed to resolve absolute path for: %s\n", stubPath);
        CoUninitialize();
        return 1;
    }
    stubPath = absolutePath;
    printf("[*] Stub path: %s\n", stubPath);

    if (GetFileAttributesA(stubPath) == INVALID_FILE_ATTRIBUTES) {
        printf("[!] Stub executable not found: %s\n", stubPath);
        printf("    Please compile with: clang %s -DCONSTUB -o %s -luser32 -lgdi32\n", __FILE__, stubPath);
        CoUninitialize();
        return 1;
    }

    g_ctx.running = TRUE;
    SetConsoleCtrlHandler(CtrlHandler, TRUE);

    printf("[*] Loading Windows Terminal settings...\n");
    g_ctx.settings = term_loadWTSettings();
    term_calculateCellSize(&g_ctx.settings, &g_ctx.cellWidth, &g_ctx.cellHeight);

    printf("[*] Creating shared memory (double-buffered)...\n");
    if (!term_createSharedMem()) {
        printf("[!] Failed to create shared memory\n");
        CoUninitialize();
        return 1;
    }

    printf("[*] Spawning Windows Terminal...\n");
    if (!term_spawnWTWithStub(stubPath)) {
        printf("[!] Failed to spawn Windows Terminal: %lu\n", GetLastError());
        Cleanup();
        CoUninitialize();
        return 1;
    }
    printf("[+] Launched (PID: %lu)\n", g_ctx.stubProcess.dwProcessId);

    printf("[*] Waiting for Windows Terminal window...\n");
    if (!term_findWTWindowWithRetry()) {
        printf("[!] Could not find Windows Terminal window\n");
        Cleanup();
        CoUninitialize();
        return 1;
    }
    printf("[+] Found WT window (PID: %lu, TitleBar: %dpx)\n", g_ctx.wtWindow.processId, g_ctx.wtWindow.titleBarHeight);

    term_updateMetrics();

    // Setup extended event hooks for focus/minimize/maximize/movement
    g_ctx.winEventHook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND, EVENT_OBJECT_STATECHANGE,
        NULL, WinEventCallback, g_ctx.wtWindow.processId, 0, WINEVENT_OUTOFCONTEXT
    );
    g_ctx.mouseHook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);

    printf("[+] Hooks: WinEvent=%s, Mouse=%s\n", g_ctx.winEventHook ? "OK" : "FAIL", g_ctx.mouseHook ? "OK" : "FAIL");

    g_ctx.monitorThread = CreateThread(NULL, 0, ProcessMonitorThread, NULL, 0, NULL);

    printf("\n=== Ready ===\n");
    printf("Grid: %dx%d, Cell: %.0fx%.0f\n", g_ctx.state->gridCols, g_ctx.state->gridRows, g_ctx.cellWidth, g_ctx.cellHeight);
    printf("Features: Double-buffering, FPS tracking, Window events, Mouse wheel\n");
    printf("Press Ctrl+C or close terminal to exit\n\n");

    // Demo game loop
    int frame = 0;
    int px = 5, py = 5;

    while (g_ctx.running) {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                g_ctx.running = FALSE;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        term_checkFocusChanged();

        if (term_wasResized()) {
            printf("[RESIZE] %dx%d\n", g_ctx.state->gridCols, g_ctx.state->gridRows);
        }

        // Process window events
        WindowEvent wevt;
        while (term_pollWindowEvent(&wevt)) {
            // Handle window events as needed
        }

        // Handle mouse wheel
        LONG wheelDelta = term_getMouseWheelDelta();
        if (wheelDelta != 0) {
            printf("[WHEEL_TOTAL] delta=%ld\n", wheelDelta);
        }

        INPUT_RECORD input;
        while (term_pollInput(&input)) {
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

        // Render to write buffer
        term_clear(L' ', 0x07);

        for (int x = 0; x < g_ctx.state->gridCols; x++) {
            term_setCell(x, 0, L'=', 0x0A);
            term_setCell(x, g_ctx.state->gridRows - 1, L'=', 0x0A);
        }

        term_setCell(px, py, L'@', 0x0C);

        wchar_t info[128];
        swprintf(info, 128, L"Frame:%d Pos:(%d,%d) Grid:%dx%d FPS:%.1f", frame, px, py, g_ctx.state->gridCols, g_ctx.state->gridRows, term_getFPS());
        for (int i = 0; info[i] && i + 2 < g_ctx.state->gridCols; i++) {
            term_setCell(2 + i, 1, info[i], 0x0F);
        }

        // Swap buffers (atomic)
        term_present();

        frame++;
        Sleep(8);
    }

    Cleanup();
    CoUninitialize();
    printf("[+] Exited cleanly\n");
    return 0;
}

#endif
