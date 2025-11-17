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

// Terminal Game Engine - Improved
//
// Fixes:
// 1. FPS measurement with actual frame timing (not just counter)
// 2. Abstract input events (platform-agnostic)
// 3. Throttled window event processing
// 4. Clean separation between engine events and console internals

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

// High-resolution timer support (Windows 10 1803+)
#ifndef CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
#endif

#if defined(CONSTUB) && !defined(CONSOLE_STUB)
#define CONSOLE_STUB
#endif

// ============================================================================
// Abstract Input Events (Platform-Agnostic)
// ============================================================================

typedef enum {
    INPUT_EVENT_NONE = 0,
    INPUT_EVENT_KEY_DOWN,
    INPUT_EVENT_KEY_UP,
    INPUT_EVENT_MOUSE_BUTTON_DOWN,
    INPUT_EVENT_MOUSE_BUTTON_UP,
    INPUT_EVENT_MOUSE_MOVE,
    INPUT_EVENT_MOUSE_WHEEL,
    INPUT_EVENT_TEXT_INPUT
} InputEventType;

typedef enum {
    MOUSE_BUTTON_LEFT = 0,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_MIDDLE,
    MOUSE_BUTTON_X1,
    MOUSE_BUTTON_X2
} MouseButton;

// Abstract key codes (subset - extend as needed)
typedef enum {
    KEY_UNKNOWN = 0,
    KEY_ESCAPE = 27,
    KEY_SPACE = 32,
    KEY_ENTER = 13,
    KEY_TAB = 9,
    KEY_BACKSPACE = 8,
    KEY_DELETE = 127,
    KEY_UP = 256,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_HOME,
    KEY_END,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_INSERT,
    KEY_F1 = 290,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    // Letters and numbers use their ASCII values (A=65, 0=48, etc.)
} KeyCode;

typedef struct {
    InputEventType type;
    union {
        struct {
            KeyCode code;
            int scancode;
            BOOL shift;
            BOOL ctrl;
            BOOL alt;
        } key;
        struct {
            MouseButton button;
            int x; // Character position
            int y;
            int pixelX; // Pixel position within content
            int pixelY;
        } mouseButton;
        struct {
            int x;
            int y;
            int pixelX;
            int pixelY;
            int deltaX; // Movement since last event
            int deltaY;
        } mouseMove;
        struct {
            int delta; // Positive = up, negative = down
            int x;
            int y;
        } mouseWheel;
        struct {
            wchar_t character;
        } text;
    } data;
} InputEvent;

// ============================================================================
// Window Events (Platform-Agnostic)
// ============================================================================

typedef enum {
    WINDOW_EVENT_NONE = 0,
    WINDOW_EVENT_FOCUS_GAINED,
    WINDOW_EVENT_FOCUS_LOST,
    WINDOW_EVENT_MINIMIZED,
    WINDOW_EVENT_MAXIMIZED,
    WINDOW_EVENT_RESTORED,
    WINDOW_EVENT_MOVED,
    WINDOW_EVENT_RESIZED,
    WINDOW_EVENT_CLOSE_REQUESTED
} WindowEventType;

typedef struct {
    WindowEventType type;
    union {
        struct {
            int x, y;          // Window position (pixels)
            int width, height; // Window size (pixels)
        } geometry;
        struct {
            int cols, rows; // Grid size (characters)
        } resize;
    } data;
} WindowEvent;

// ============================================================================
// Shared Memory Structure
// ============================================================================

#define SHARED_MEM_NAME    L"TerminalEngineSharedMem"
#define MAX_CONSOLE_WIDTH  512
#define MAX_CONSOLE_HEIGHT 256

typedef struct {
    CHAR_INFO screenBuffer[MAX_CONSOLE_HEIGHT][MAX_CONSOLE_WIDTH];
    SHORT bufferWidth;
    SHORT bufferHeight;
    COORD cursorPos;
    BOOL cursorVisible;
    LARGE_INTEGER timestamp;
} FrameBuffer;

typedef struct {
    // Double-buffered frames
    FrameBuffer frames[2];
    volatile LONG currentReadBuffer;
    volatile LONG writeInProgress;

    // Terminal metrics
    SHORT gridCols;
    SHORT gridRows;
    double cellWidth;
    double cellHeight;
    int contentAreaY; // Title bar height
    int paddingLeft;
    int paddingTop;
    int paddingRight;
    int paddingBottom;

    // Raw console input (stub writes, engine reads and translates)
    INPUT_RECORD rawInputBuffer[512];
    volatile LONG rawInputCount;
    volatile LONG rawInputReadIndex;
    volatile LONG rawInputWriteIndex;

    // Control flags
    volatile BOOL shouldExit;
    volatile BOOL metricsUpdated;

    // Synchronization
    volatile LONG lock;
} SharedTerminalState;

// ============================================================================
// Console Stub - Minimal (only handles raw I/O)
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
    LONG bufferIndex = InterlockedCompareExchange(&g_state->currentReadBuffer, 0, 0);

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
        LONG idx = InterlockedIncrement(&g_state->rawInputWriteIndex) - 1;
        g_state->rawInputBuffer[idx % 512] = records[i];
        InterlockedIncrement(&g_state->rawInputCount);
    }
}

int main(void) {
    if (!InitStub()) {
        return 1;
    }

    while (!g_state->shouldExit) {
        Render();
        CaptureInput();
        Sleep(1);
    }

    UnmapViewOfFile(g_state);
    CloseHandle(g_sharedMemHandle);
    return 0;
}

#else
// ============================================================================
// Engine Side
// ============================================================================

typedef struct {
    char fontFace[128];
    int fontSize;
    double lineHeight;
    double cellWidthRatio;
    int paddingLeft, paddingTop, paddingRight, paddingBottom;
} WTSettings;

typedef struct {
    HWND window;
    DWORD processId;
    int titleBarHeight;
} WTWindowInfo;

// FPS tracking with proper frame timing
typedef struct {
    LARGE_INTEGER frequency;
    LARGE_INTEGER lastFrameTime;
    LARGE_INTEGER lastFpsUpdateTime;
    double frameTimeAccumulator;
    int frameCount;
    float currentFps;
    float avgFrameTimeMs;
} FpsTracker;

typedef struct {
    HANDLE sharedMemHandle;
    SharedTerminalState* state;

    WTSettings settings;
    WTWindowInfo wtWindow;
    double cellWidth;
    double cellHeight;

    PROCESS_INFORMATION stubProcess;
    HANDLE jobObject;
    HANDLE monitorThread;

    HHOOK mouseHook;
    HWINEVENTHOOK winEventHook;

    LONG currentWriteBuffer;

    // Abstract event queues
    InputEvent inputQueue[256];
    volatile LONG inputQueueCount;
    volatile LONG inputQueueReadIdx;
    volatile LONG inputQueueWriteIdx;

    WindowEvent windowQueue[64];
    volatile LONG windowQueueCount;
    volatile LONG windowQueueReadIdx;
    volatile LONG windowQueueWriteIdx;

    // Mouse wheel accumulator
    int mouseWheelAccumulator;

    // FPS tracking
    FpsTracker fps;

    // Focus state
    BOOL hasFocus;

    // Throttling for location change events
    LARGE_INTEGER lastLocationChangeTime;

    volatile BOOL running;
} TerminalContext;

static TerminalContext g_ctx = { 0 };

// Forward declarations
static void UpdateMetrics(void);
static int DetectTitleBarHeight(HWND hwnd);

// ============================================================================
// Settings Parser (simplified - keeping same logic)
// ============================================================================

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

static void ParsePaddingString(const char* str, int* l, int* t, int* r, int* b) {
    *l = *t = *r = *b = 8;
    int v[4];
    int c = sscanf(str, "%d , %d , %d , %d", &v[0], &v[1], &v[2], &v[3]);
    if (c == 1) {
        *l = *t = *r = *b = v[0];
    } else if (c == 4) {
        *l = v[0];
        *t = v[1];
        *r = v[2];
        *b = v[3];
    }
}

static WTSettings LoadWTSettings(void) {
    WTSettings s = {
        .fontFace = "Cascadia Mono", .fontSize = 12, .lineHeight = 1.2, .cellWidthRatio = 0.6, .paddingLeft = 8, .paddingTop = 8, .paddingRight = 8, .paddingBottom = 8
    };

    char* localAppData = getenv("LOCALAPPDATA");
    if (!localAppData) {
        return s;
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
        return s;
    }

    printf("[+] Loaded: %s\n", path);

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
                        strcpy(s.fontFace, face);
                        printf("    font.face: %s\n", s.fontFace);
                    }
                    int size;
                    if (ExtractJsonInt(fontObj, "size", &size)) {
                        s.fontSize = size;
                        printf("    font.size: %d\n", s.fontSize);
                    }
                }
            }
            char paddingStr[64];
            if (ExtractJsonString(obj, "padding", paddingStr, sizeof(paddingStr))) {
                ParsePaddingString(paddingStr, &s.paddingLeft, &s.paddingTop, &s.paddingRight, &s.paddingBottom);
                printf("    padding: %d,%d,%d,%d\n", s.paddingLeft, s.paddingTop, s.paddingRight, s.paddingBottom);
            }
        }
    }

    free(json);
    return s;
}

static void CalculateCellSize(WTSettings* settings, double* w, double* h) {
    HDC screenDC = GetDC(NULL);
    int dpi = GetDeviceCaps(screenDC, LOGPIXELSY);
    ReleaseDC(NULL, screenDC);

    double fontPixels = (double)settings->fontSize * dpi / 72.0;
    *h = round(fontPixels * settings->lineHeight);
    *w = round(fontPixels * settings->cellWidthRatio);

    printf("[+] Cell: %.0fx%.0f px (font: %dpt @ %d DPI)\n", *w, *h, settings->fontSize, dpi);
}

// ============================================================================
// Title Bar Detection (UI Automation)
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

            for (int i = 0; i < count && i < 3; i++) {
                IUIAutomationElement* child = NULL;
                if (SUCCEEDED(IUIAutomationElementArray_GetElement(children, i, &child))) {
                    CONTROLTYPEID controlType = 0;
                    RECT rect = { 0 };
                    IUIAutomationElement_get_CurrentControlType(child, &controlType);
                    IUIAutomationElement_get_CurrentBoundingRectangle(child, &rect);

                    int elementHeight = rect.bottom - rect.top;
                    int relativeTop = rect.top - windowRect.top;

                    if ((controlType == 50033 || controlType == 50037) && elementHeight >= 30 && elementHeight <= 50 && relativeTop < 10) {
                        height = elementHeight;
                        IUIAutomationElement_Release(child);
                        break;
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

static int DetectTitleBarHeight(HWND hwnd) {
    int height = GetTitleBarHeightByChildren(hwnd);
    if (height > 0 && height < 200) {
        return height;
    }

    // Fallback: child window enumeration
    HWND child = GetWindow(hwnd, GW_CHILD);
    while (child) {
        char childClass[256];
        if (GetClassNameA(child, childClass, sizeof(childClass))) {
            if (strstr(childClass, "Windows.UI.Composition") || strstr(childClass, "TermControl")) {
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

    return 40; // Default fallback
}

// ============================================================================
// Abstract Event Queue Management
// ============================================================================

static void PushInputEvent(InputEvent* evt) {
    LONG idx = InterlockedIncrement(&g_ctx.inputQueueWriteIdx) - 1;
    g_ctx.inputQueue[idx % 256] = *evt;
    InterlockedIncrement(&g_ctx.inputQueueCount);
}

static void PushWindowEvent(WindowEvent* evt) {
    LONG idx = InterlockedIncrement(&g_ctx.windowQueueWriteIdx) - 1;
    g_ctx.windowQueue[idx % 64] = *evt;
    InterlockedIncrement(&g_ctx.windowQueueCount);
}

// Convert Windows virtual key code to abstract key code
static KeyCode VirtualKeyToKeyCode(WORD vk) {
    if (vk >= 'A' && vk <= 'Z') {
        return (KeyCode)vk;
    }
    if (vk >= '0' && vk <= '9') {
        return (KeyCode)vk;
    }

    switch (vk) {
    case VK_ESCAPE:
        return KEY_ESCAPE;
    case VK_SPACE:
        return KEY_SPACE;
    case VK_RETURN:
        return KEY_ENTER;
    case VK_TAB:
        return KEY_TAB;
    case VK_BACK:
        return KEY_BACKSPACE;
    case VK_DELETE:
        return KEY_DELETE;
    case VK_UP:
        return KEY_UP;
    case VK_DOWN:
        return KEY_DOWN;
    case VK_LEFT:
        return KEY_LEFT;
    case VK_RIGHT:
        return KEY_RIGHT;
    case VK_HOME:
        return KEY_HOME;
    case VK_END:
        return KEY_END;
    case VK_PRIOR:
        return KEY_PAGE_UP;
    case VK_NEXT:
        return KEY_PAGE_DOWN;
    case VK_INSERT:
        return KEY_INSERT;
    case VK_F1:
        return KEY_F1;
    case VK_F2:
        return KEY_F2;
    case VK_F3:
        return KEY_F3;
    case VK_F4:
        return KEY_F4;
    case VK_F5:
        return KEY_F5;
    case VK_F6:
        return KEY_F6;
    case VK_F7:
        return KEY_F7;
    case VK_F8:
        return KEY_F8;
    case VK_F9:
        return KEY_F9;
    case VK_F10:
        return KEY_F10;
    case VK_F11:
        return KEY_F11;
    case VK_F12:
        return KEY_F12;
    default:
        return KEY_UNKNOWN;
    }
}

// Translate raw console input to abstract events
static void TranslateRawInput(void) {
    while (g_ctx.state->rawInputCount > 0) {
        LONG idx = InterlockedIncrement(&g_ctx.state->rawInputReadIndex) - 1;
        INPUT_RECORD raw = g_ctx.state->rawInputBuffer[idx % 512];
        InterlockedDecrement(&g_ctx.state->rawInputCount);

        InputEvent evt = { 0 };

        switch (raw.EventType) {
        case KEY_EVENT: {
            evt.type = raw.Event.KeyEvent.bKeyDown ? INPUT_EVENT_KEY_DOWN : INPUT_EVENT_KEY_UP;
            evt.data.key.code = VirtualKeyToKeyCode(raw.Event.KeyEvent.wVirtualKeyCode);
            evt.data.key.scancode = raw.Event.KeyEvent.wVirtualScanCode;
            evt.data.key.shift = (raw.Event.KeyEvent.dwControlKeyState & SHIFT_PRESSED) != 0;
            evt.data.key.ctrl = (raw.Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0;
            evt.data.key.alt = (raw.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) != 0;
            PushInputEvent(&evt);

            // Also generate text input for printable characters
            if (raw.Event.KeyEvent.bKeyDown && raw.Event.KeyEvent.uChar.UnicodeChar >= 32) {
                InputEvent textEvt = { 0 };
                textEvt.type = INPUT_EVENT_TEXT_INPUT;
                textEvt.data.text.character = raw.Event.KeyEvent.uChar.UnicodeChar;
                PushInputEvent(&textEvt);
            }
            break;
        }

        case MOUSE_EVENT: {
            DWORD flags = raw.Event.MouseEvent.dwEventFlags;
            DWORD buttons = raw.Event.MouseEvent.dwButtonState;

            if (flags & MOUSE_WHEELED) {
                evt.type = INPUT_EVENT_MOUSE_WHEEL;
                evt.data.mouseWheel.delta = (short)HIWORD(buttons);
                evt.data.mouseWheel.x = raw.Event.MouseEvent.dwMousePosition.X;
                evt.data.mouseWheel.y = raw.Event.MouseEvent.dwMousePosition.Y;
                g_ctx.mouseWheelAccumulator += evt.data.mouseWheel.delta;
                PushInputEvent(&evt);
            } else if (flags == 0) { // Button press/release
                // This is simplified - you'd need state tracking for proper button events
                if (buttons & FROM_LEFT_1ST_BUTTON_PRESSED) {
                    evt.type = INPUT_EVENT_MOUSE_BUTTON_DOWN;
                    evt.data.mouseButton.button = MOUSE_BUTTON_LEFT;
                    evt.data.mouseButton.x = raw.Event.MouseEvent.dwMousePosition.X;
                    evt.data.mouseButton.y = raw.Event.MouseEvent.dwMousePosition.Y;
                    PushInputEvent(&evt);
                }
                if (buttons & RIGHTMOST_BUTTON_PRESSED) {
                    evt.type = INPUT_EVENT_MOUSE_BUTTON_DOWN;
                    evt.data.mouseButton.button = MOUSE_BUTTON_RIGHT;
                    evt.data.mouseButton.x = raw.Event.MouseEvent.dwMousePosition.X;
                    evt.data.mouseButton.y = raw.Event.MouseEvent.dwMousePosition.Y;
                    PushInputEvent(&evt);
                }
            } else if (flags & MOUSE_MOVED) {
                evt.type = INPUT_EVENT_MOUSE_MOVE;
                evt.data.mouseMove.x = raw.Event.MouseEvent.dwMousePosition.X;
                evt.data.mouseMove.y = raw.Event.MouseEvent.dwMousePosition.Y;
                PushInputEvent(&evt);
            }
            break;
        }
        }
    }
}

// ============================================================================
// FPS Tracking (Improved - measures actual frame time)
// ============================================================================

static void InitFpsTracker(FpsTracker* fps) {
    QueryPerformanceFrequency(&fps->frequency);
    QueryPerformanceCounter(&fps->lastFrameTime);
    fps->lastFpsUpdateTime = fps->lastFrameTime;
    fps->frameTimeAccumulator = 0;
    fps->frameCount = 0;
    fps->currentFps = 0;
    fps->avgFrameTimeMs = 0;
}

static void UpdateFpsTracker(FpsTracker* fps) {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    // Calculate this frame's time
    double frameTime = (double)(now.QuadPart - fps->lastFrameTime.QuadPart) / (double)fps->frequency.QuadPart;
    fps->lastFrameTime = now;

    fps->frameTimeAccumulator += frameTime;
    fps->frameCount++;

    // Update FPS display every 0.5 seconds for more responsive updates
    double timeSinceUpdate = (double)(now.QuadPart - fps->lastFpsUpdateTime.QuadPart) / (double)fps->frequency.QuadPart;

    if (timeSinceUpdate >= 0.5) {
        fps->currentFps = (float)(fps->frameCount / timeSinceUpdate);
        fps->avgFrameTimeMs = (float)(fps->frameTimeAccumulator / fps->frameCount * 1000.0);

        fps->frameTimeAccumulator = 0;
        fps->frameCount = 0;
        fps->lastFpsUpdateTime = now;
    }
}

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

        g_ctx.state->frames[0].bufferWidth = (SHORT)cols;
        g_ctx.state->frames[0].bufferHeight = (SHORT)rows;
        g_ctx.state->frames[1].bufferWidth = (SHORT)cols;
        g_ctx.state->frames[1].bufferHeight = (SHORT)rows;

        g_ctx.state->cellWidth = g_ctx.cellWidth;
        g_ctx.state->cellHeight = g_ctx.cellHeight;
        g_ctx.state->contentAreaY = g_ctx.wtWindow.titleBarHeight;
        g_ctx.state->paddingLeft = g_ctx.settings.paddingLeft;
        g_ctx.state->paddingTop = g_ctx.settings.paddingTop;
        g_ctx.state->paddingRight = g_ctx.settings.paddingRight;
        g_ctx.state->paddingBottom = g_ctx.settings.paddingBottom;
        g_ctx.state->metricsUpdated = TRUE;

        WindowEvent evt = { .type = WINDOW_EVENT_RESIZED };
        evt.data.resize.cols = cols;
        evt.data.resize.rows = rows;
        PushWindowEvent(&evt);

        printf("[RESIZE] Grid: %dx%d\n", cols, rows);
    }
}

// ============================================================================
// Win Event Hook (with throttling)
// ============================================================================

static void CALLBACK WinEventCallback(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD thread, DWORD time) {
    if (hwnd != g_ctx.wtWindow.window) {
        return;
    }

    WindowEvent evt = { 0 };

    switch (event) {
    case EVENT_OBJECT_LOCATIONCHANGE:
        if (idObject == OBJID_WINDOW) {
            // Throttle location change events (max 60 per second)
            LARGE_INTEGER now;
            QueryPerformanceCounter(&now);
            double elapsed = (double)(now.QuadPart - g_ctx.lastLocationChangeTime.QuadPart) / (double)g_ctx.fps.frequency.QuadPart;
            if (elapsed < 0.016) {
                return; // Skip if less than 16ms since last
            }
            g_ctx.lastLocationChangeTime = now;

            UpdateMetrics();

            RECT rect;
            GetWindowRect(hwnd, &rect);
            evt.type = WINDOW_EVENT_MOVED;
            evt.data.geometry.x = rect.left;
            evt.data.geometry.y = rect.top;
            evt.data.geometry.width = rect.right - rect.left;
            evt.data.geometry.height = rect.bottom - rect.top;
            PushWindowEvent(&evt);
        }
        break;

    case EVENT_SYSTEM_FOREGROUND:
        if (!g_ctx.hasFocus) {
            g_ctx.hasFocus = TRUE;
            evt.type = WINDOW_EVENT_FOCUS_GAINED;
            PushWindowEvent(&evt);
        }
        break;

    case EVENT_SYSTEM_MINIMIZESTART:
        evt.type = WINDOW_EVENT_MINIMIZED;
        PushWindowEvent(&evt);
        break;

    case EVENT_SYSTEM_MINIMIZEEND:
        evt.type = WINDOW_EVENT_RESTORED;
        PushWindowEvent(&evt);
        break;

    case EVENT_OBJECT_STATECHANGE:
        if (idObject == OBJID_WINDOW) {
            WINDOWPLACEMENT wp = { .length = sizeof(wp) };
            if (GetWindowPlacement(hwnd, &wp)) {
                if (wp.showCmd == SW_SHOWMAXIMIZED) {
                    evt.type = WINDOW_EVENT_MAXIMIZED;
                    PushWindowEvent(&evt);
                }
            }
        }
        break;
    }
}

static void CheckFocusLoss(void) {
    BOOL currentlyFocused = (GetForegroundWindow() == g_ctx.wtWindow.window);
    if (g_ctx.hasFocus && !currentlyFocused) {
        g_ctx.hasFocus = FALSE;
        WindowEvent evt = { .type = WINDOW_EVENT_FOCUS_LOST };
        PushWindowEvent(&evt);
    }
}

// ============================================================================
// Mouse Hook (simplified - just for additional input capture)
// ============================================================================

static LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    // The console stub already captures mouse input, so this hook is optional
    // You might use it for global mouse tracking or window-drag detection
    return CallNextHookEx(g_ctx.mouseHook, nCode, wParam, lParam);
}

// ============================================================================
// Double-Buffered Rendering
// ============================================================================

static FrameBuffer* GetWriteBuffer(void) {
    return &g_ctx.state->frames[g_ctx.currentWriteBuffer];
}

static void term_SwapBuffers(void) {
    InterlockedExchange(&g_ctx.state->writeInProgress, 1);

    LARGE_INTEGER timestamp;
    QueryPerformanceCounter(&timestamp);
    GetWriteBuffer()->timestamp = timestamp;

    LONG newReadBuffer = g_ctx.currentWriteBuffer;
    InterlockedExchange(&g_ctx.state->currentReadBuffer, newReadBuffer);

    g_ctx.currentWriteBuffer = 1 - g_ctx.currentWriteBuffer;

    InterlockedExchange(&g_ctx.state->writeInProgress, 0);
}

// ============================================================================
// Process Management
// ============================================================================

static DWORD WINAPI ProcessMonitorThread(LPVOID param) {
    while (g_ctx.running) {
        if (!IsWindow(g_ctx.wtWindow.window)) {
            printf("[!] Window closed\n");
            WindowEvent evt = { .type = WINDOW_EVENT_CLOSE_REQUESTED };
            PushWindowEvent(&evt);
            g_ctx.running = FALSE;
            break;
        }
        Sleep(100);
    }
    return 0;
}

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
    g_ctx.currentWriteBuffer = 0;
    g_ctx.state->currentReadBuffer = 0;

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
// High-Resolution Sleep (avoids Windows Sleep() granularity issues)
// ============================================================================

static void SleepHighRes(double milliseconds) {
    // Try high-resolution waitable timer (Windows 10 1803+)
    HANDLE timer = CreateWaitableTimerExW(
        NULL, NULL,
        CREATE_WAITABLE_TIMER_HIGH_RESOLUTION,
        TIMER_ALL_ACCESS
    );

    if (timer) {
        LARGE_INTEGER dueTime;
        // Negative value = relative time in 100-nanosecond intervals
        dueTime.QuadPart = (LONGLONG)(milliseconds * -10000.0);

        if (SetWaitableTimer(timer, &dueTime, 0, NULL, NULL, FALSE)) {
            WaitForSingleObject(timer, INFINITE);
        } else {
            // Fallback if SetWaitableTimer fails
            Sleep((DWORD)milliseconds);
        }
        CloseHandle(timer);
    } else {
        // Fallback for older Windows: use timeBeginPeriod
        // This requires linking with winmm.lib
        Sleep((DWORD)milliseconds);
    }
}

// Alternative: Spin-wait for very precise timing (uses CPU)
static void SleepPrecise(double milliseconds) {
    LARGE_INTEGER freq, start, now;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    double targetTicks = (milliseconds / 1000.0) * (double)freq.QuadPart;

    // Hybrid: sleep for most of the time, then spin for precision
    if (milliseconds > 2.0) {
        SleepHighRes(milliseconds - 1.0); // Sleep most of the time
    }

    // Spin-wait for the remaining time
    do {
        QueryPerformanceCounter(&now);
    } while ((double)(now.QuadPart - start.QuadPart) < targetTicks);
}

// ============================================================================
// Public Engine API (Abstract, Platform-Agnostic)
// ============================================================================

void Term_SetCell(int x, int y, wchar_t ch, WORD attr) {
    FrameBuffer* frame = GetWriteBuffer();
    if (x >= 0 && x < frame->bufferWidth && y >= 0 && y < frame->bufferHeight) {
        frame->screenBuffer[y][x].Char.UnicodeChar = ch;
        frame->screenBuffer[y][x].Attributes = attr;
    }
}

void Term_Clear(wchar_t ch, WORD attr) {
    FrameBuffer* frame = GetWriteBuffer();
    for (int y = 0; y < frame->bufferHeight; y++) {
        for (int x = 0; x < frame->bufferWidth; x++) {
            frame->screenBuffer[y][x].Char.UnicodeChar = ch;
            frame->screenBuffer[y][x].Attributes = attr;
        }
    }
}

void Term_Present(void) {
    term_SwapBuffers();
    UpdateFpsTracker(&g_ctx.fps);
}

BOOL Term_PollInput(InputEvent* out) {
    TranslateRawInput(); // Convert any pending raw input

    if (g_ctx.inputQueueCount <= 0) {
        return FALSE;
    }

    LONG idx = InterlockedIncrement(&g_ctx.inputQueueReadIdx) - 1;
    *out = g_ctx.inputQueue[idx % 256];
    InterlockedDecrement(&g_ctx.inputQueueCount);
    return TRUE;
}

BOOL Term_PollWindowEvent(WindowEvent* out) {
    if (g_ctx.windowQueueCount <= 0) {
        return FALSE;
    }

    LONG idx = InterlockedIncrement(&g_ctx.windowQueueReadIdx) - 1;
    *out = g_ctx.windowQueue[idx % 64];
    InterlockedDecrement(&g_ctx.windowQueueCount);
    return TRUE;
}

void Term_GetSize(int* cols, int* rows) {
    *cols = g_ctx.state->gridCols;
    *rows = g_ctx.state->gridRows;
}

float Term_GetFPS(void) {
    return g_ctx.fps.currentFps;
}

float Term_GetFrameTimeMs(void) {
    return g_ctx.fps.avgFrameTimeMs;
}

int Term_GetMouseWheelDelta(void) {
    int delta = g_ctx.mouseWheelAccumulator;
    g_ctx.mouseWheelAccumulator = 0;
    return delta;
}

// ============================================================================
// Main
// ============================================================================

int main(int argc, char* argv[]) {
    printf("=== Terminal Game Engine v2 ===\n\n");

    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    InitFpsTracker(&g_ctx.fps);

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
        printf("[!] Failed to resolve path: %s\n", stubPath);
        CoUninitialize();
        return 1;
    }
    stubPath = absolutePath;
    printf("[*] Stub: %s\n", stubPath);

    if (GetFileAttributesA(stubPath) == INVALID_FILE_ATTRIBUTES) {
        printf("[!] Not found: %s\n", stubPath);
        CoUninitialize();
        return 1;
    }

    g_ctx.running = TRUE;
    SetConsoleCtrlHandler(CtrlHandler, TRUE);

    printf("[*] Loading settings...\n");
    g_ctx.settings = LoadWTSettings();
    CalculateCellSize(&g_ctx.settings, &g_ctx.cellWidth, &g_ctx.cellHeight);

    printf("[*] Creating shared memory...\n");
    if (!CreateSharedMem()) {
        printf("[!] Failed to create shared memory\n");
        CoUninitialize();
        return 1;
    }

    printf("[*] Spawning terminal...\n");
    if (!SpawnWTWithStub(stubPath)) {
        printf("[!] Failed to spawn: %lu\n", GetLastError());
        Cleanup();
        CoUninitialize();
        return 1;
    }
    printf("[+] Launched PID: %lu\n", g_ctx.stubProcess.dwProcessId);

    printf("[*] Finding window...\n");
    if (!FindWTWindowWithRetry()) {
        printf("[!] Window not found\n");
        Cleanup();
        CoUninitialize();
        return 1;
    }
    printf("[+] Found window (TitleBar: %dpx)\n", g_ctx.wtWindow.titleBarHeight);

    UpdateMetrics();

    g_ctx.winEventHook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND, EVENT_OBJECT_STATECHANGE,
        NULL, WinEventCallback, g_ctx.wtWindow.processId, 0, WINEVENT_OUTOFCONTEXT
    );

    g_ctx.monitorThread = CreateThread(NULL, 0, ProcessMonitorThread, NULL, 0, NULL);

    printf("\n=== Ready ===\n");
    printf("Grid: %dx%d, Cell: %.0fx%.0f\n", g_ctx.state->gridCols, g_ctx.state->gridRows, g_ctx.cellWidth, g_ctx.cellHeight);
    printf("Press Ctrl+C or close terminal to exit\n\n");

    int px = 5, py = 5;
    int frameNum = 0;

    while (g_ctx.running) {
        // Process Windows messages (limited)
        MSG msg;
        int msgCount = 0;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) && msgCount < 10) {
            if (msg.message == WM_QUIT) {
                g_ctx.running = FALSE;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            msgCount++;
        }

        CheckFocusLoss();

        // Process window events
        WindowEvent wevt;
        while (Term_PollWindowEvent(&wevt)) {
            switch (wevt.type) {
            case WINDOW_EVENT_RESIZED:
                printf("[EVENT] Resized: %dx%d\n", wevt.data.resize.cols, wevt.data.resize.rows);
                break;
            case WINDOW_EVENT_FOCUS_GAINED:
                printf("[EVENT] Focus gained\n");
                break;
            case WINDOW_EVENT_FOCUS_LOST:
                printf("[EVENT] Focus lost\n");
                break;
            default:
                break;
            }
        }

        // Process input (abstract)
        InputEvent input;
        while (Term_PollInput(&input)) {
            if (input.type == INPUT_EVENT_KEY_DOWN) {
                switch (input.data.key.code) {
                case KEY_UP:
                    py = (py > 0) ? py - 1 : py;
                    break;
                case KEY_DOWN:
                    py = (py < g_ctx.state->gridRows - 1) ? py + 1 : py;
                    break;
                case KEY_LEFT:
                    px = (px > 0) ? px - 1 : px;
                    break;
                case KEY_RIGHT:
                    px = (px < g_ctx.state->gridCols - 1) ? px + 1 : px;
                    break;
                case KEY_ESCAPE:
                    g_ctx.running = FALSE;
                    break;
                default:
                    break;
                }
            } else if (input.type == INPUT_EVENT_MOUSE_BUTTON_DOWN) {
                px = input.data.mouseButton.x;
                py = input.data.mouseButton.y;
                printf("[CLICK] (%d,%d)\n", px, py);
            } else if (input.type == INPUT_EVENT_MOUSE_WHEEL) {
                printf("[WHEEL] delta=%d\n", input.data.mouseWheel.delta);
            }
        }

        // Render
        Term_Clear(L' ', 0x07);

        for (int x = 0; x < g_ctx.state->gridCols; x++) {
            Term_SetCell(x, 0, L'=', 0x0A);
            Term_SetCell(x, g_ctx.state->gridRows - 1, L'=', 0x0A);
        }

        Term_SetCell(px, py, L'@', 0x0C);

        wchar_t info[128];
        swprintf(info, 128, L"Frame:%d Pos:(%d,%d) Grid:%dx%d FPS:%.1f (%.2fms)", frameNum, px, py, g_ctx.state->gridCols, g_ctx.state->gridRows, Term_GetFPS(), Term_GetFrameTimeMs());
        for (int i = 0; info[i] && i + 2 < g_ctx.state->gridCols; i++) {
            Term_SetCell(2 + i, 1, info[i], 0x0F);
        }

        Term_Present();
        frameNum++;

        SleepHighRes(16.0); // High-resolution ~60 FPS target
        // Or use: SleepPrecise(16.667);  // More precise but uses some CPU
    }

    Cleanup();
    CoUninitialize();
    printf("[+] Exited\n");
    return 0;
}

#endif
