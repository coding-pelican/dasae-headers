// dh-c run wt_monitor2.c --no-libdh --args="-luser32 -ldwmapi -lshell32 -lgdi32 -- "
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <dwmapi.h>
#include <shlobj.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>
#include <math.h>

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
    double lineHeight; // Multiplier (default 1.2)
    double cellWidth;  // Multiplier (default 0.6 of line height, effectively)
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

// ============================================================================
// Improved JSON Parser
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
                // Line comment
                p->pos += 2;
                while (p->pos < p->len && p->data[p->pos] != '\n') {
                    p->pos++;
                }
            } else if (p->data[p->pos + 1] == '*') {
                // Block comment
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
            char escaped = p->data[p->pos];
            if (outPos < outSize - 1) {
                switch (escaped) {
                case 'n':
                    out[outPos++] = '\n';
                    break;
                case 't':
                    out[outPos++] = '\t';
                    break;
                case 'r':
                    out[outPos++] = '\r';
                    break;
                case '\\':
                    out[outPos++] = '\\';
                    break;
                case '"':
                    out[outPos++] = '"';
                    break;
                default:
                    out[outPos++] = escaped;
                    break;
                }
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

static BOOL json_parse_number_int(JsonParser* p, int* out) {
    json_skip_whitespace(p);
    if (p->pos >= p->len) {
        return FALSE;
    }

    char* end;
    long val = strtol(p->data + p->pos, &end, 10);
    if (end == p->data + p->pos) {
        return FALSE;
    }

    *out = (int)val;
    p->pos = end - p->data;
    return TRUE;
}

static BOOL json_parse_number_double(JsonParser* p, double* out) {
    json_skip_whitespace(p);
    if (p->pos >= p->len) {
        return FALSE;
    }

    char* end;
    double val = strtod(p->data + p->pos, &end);
    if (end == p->data + p->pos) {
        return FALSE;
    }

    *out = val;
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
            if (p->data[p->pos] == '\\' && p->pos + 1 < p->len) {
                p->pos += 2;
            } else {
                p->pos++;
            }
        }
        if (p->pos < p->len) {
            p->pos++;
        }
    } else if (c == '{') {
        int depth = 1;
        p->pos++;
        while (p->pos < p->len && depth > 0) {
            char ch = p->data[p->pos];
            if (ch == '{') {
                depth++;
            } else if (ch == '}') {
                depth--;
            } else if (ch == '"') {
                p->pos++;
                while (p->pos < p->len && p->data[p->pos] != '"') {
                    if (p->data[p->pos] == '\\' && p->pos + 1 < p->len) {
                        p->pos += 2;
                    } else {
                        p->pos++;
                    }
                }
            }
            p->pos++;
        }
    } else if (c == '[') {
        int depth = 1;
        p->pos++;
        while (p->pos < p->len && depth > 0) {
            char ch = p->data[p->pos];
            if (ch == '[') {
                depth++;
            } else if (ch == ']') {
                depth--;
            } else if (ch == '"') {
                p->pos++;
                while (p->pos < p->len && p->data[p->pos] != '"') {
                    if (p->data[p->pos] == '\\' && p->pos + 1 < p->len) {
                        p->pos += 2;
                    } else {
                        p->pos++;
                    }
                }
            }
            p->pos++;
        }
    } else {
        // Number, bool, null
        while (p->pos < p->len) {
            c = p->data[p->pos];
            if (c == ',' || c == '}' || c == ']' || c == ' ' || c == '\n' || c == '\r' || c == '\t') {
                break;
            }
            p->pos++;
        }
    }
}

// Parse an object and call handler for each key-value pair
typedef void (*JsonObjectHandler)(const char* key, JsonParser* valueParser, void* userData);

static BOOL json_parse_object(JsonParser* p, JsonObjectHandler handler, void* userData) {
    json_skip_whitespace(p);
    if (p->pos >= p->len || p->data[p->pos] != '{') {
        return FALSE;
    }
    p->pos++;

    while (p->pos < p->len) {
        json_skip_whitespace(p);
        if (p->data[p->pos] == '}') {
            p->pos++;
            return TRUE;
        }

        // Parse key
        char key[128];
        if (!json_parse_string(p, key, sizeof(key))) {
            return FALSE;
        }

        json_skip_whitespace(p);
        if (p->pos >= p->len || p->data[p->pos] != ':') {
            return FALSE;
        }
        p->pos++;

        json_skip_whitespace(p);

        // Call handler with current position pointing to value
        if (handler) {
            handler(key, p, userData);
        }

        // Skip the value if handler didn't consume it
        // We need to check if we're still at the same position
        json_skip_value(p);

        json_skip_whitespace(p);
        if (p->pos < p->len && p->data[p->pos] == ',') {
            p->pos++;
        }
    }

    return FALSE;
}

// ============================================================================
// Settings Parser
// ============================================================================

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

typedef struct {
    TerminalSettings* settings;
    BOOL foundFontFace;
    BOOL foundFontSize;
    BOOL foundLineHeight;
    BOOL foundCellWidth;
} FontParseContext;

static void font_object_handler(const char* key, JsonParser* p, void* userData) {
    FontParseContext* ctx = (FontParseContext*)userData;

    if (strcmp(key, "face") == 0) {
        json_skip_whitespace(p);
        if (p->data[p->pos] == '"') {
            size_t savedPos = p->pos;
            if (json_parse_string(p, ctx->settings->fontFace, sizeof(ctx->settings->fontFace))) {
                ctx->foundFontFace = TRUE;
                printf("    - font.face: %s\n", ctx->settings->fontFace);
            }
            // Reset position so skip_value works correctly
            // Actually, we consumed it, so we need to handle this differently
        }
    } else if (strcmp(key, "size") == 0) {
        json_skip_whitespace(p);
        size_t savedPos = p->pos;
        int size;
        if (json_parse_number_int(p, &size)) {
            ctx->settings->fontSize = size;
            ctx->foundFontSize = TRUE;
            printf("    - font.size: %d\n", size);
            p->pos = savedPos; // Let skip_value handle it
        }
    } else if (strcmp(key, "weight") == 0) {
        // Skip, we don't need this
    }
}

static void defaults_object_handler(const char* key, JsonParser* p, void* userData) {
    TerminalSettings* settings = (TerminalSettings*)userData;

    if (strcmp(key, "font") == 0) {
        json_skip_whitespace(p);
        if (p->data[p->pos] == '{') {
            printf("  Found 'font' object:\n");
            FontParseContext fontCtx = { .settings = settings };

            // Parse the font object
            size_t objStart = p->pos;
            p->pos++; // Skip '{'

            while (p->pos < p->len) {
                json_skip_whitespace(p);
                if (p->data[p->pos] == '}') {
                    p->pos++;
                    break;
                }

                char fontKey[64];
                if (!json_parse_string(p, fontKey, sizeof(fontKey))) {
                    break;
                }

                json_skip_whitespace(p);
                if (p->pos >= p->len || p->data[p->pos] != ':') {
                    break;
                }
                p->pos++;

                json_skip_whitespace(p);

                if (strcmp(fontKey, "face") == 0) {
                    if (json_parse_string(p, settings->fontFace, sizeof(settings->fontFace))) {
                        printf("    - face: %s\n", settings->fontFace);
                    }
                } else if (strcmp(fontKey, "size") == 0) {
                    int size;
                    if (json_parse_number_int(p, &size)) {
                        settings->fontSize = size;
                        printf("    - size: %d\n", size);
                    }
                } else if (strcmp(fontKey, "weight") == 0) {
                    json_skip_value(p);
                } else {
                    json_skip_value(p);
                }

                json_skip_whitespace(p);
                if (p->pos < p->len && p->data[p->pos] == ',') {
                    p->pos++;
                }
            }
        }
    } else if (strcmp(key, "padding") == 0) {
        json_skip_whitespace(p);
        if (p->data[p->pos] == '"') {
            char paddingStr[64];
            size_t savedPos = p->pos;
            if (json_parse_string(p, paddingStr, sizeof(paddingStr))) {
                settings->padding = parse_padding_string(paddingStr);
                printf("  Found 'padding': \"%s\" -> (%d, %d, %d, %d)\n", paddingStr, settings->padding.left, settings->padding.top, settings->padding.right, settings->padding.bottom);
                p->pos = savedPos;
            }
        }
    }
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
    // Windows Terminal default values
    TerminalSettings settings = {
        .fontFace = "Cascadia Mono",
        .fontSize = 12,
        .lineHeight = 1.2,
        .cellWidth = 0.6, // Not directly settable, but this is roughly the ratio
        .padding = { 8, 8, 8, 8 }
    };

    char* localAppData = getenv("LOCALAPPDATA");
    if (!localAppData) {
        printf("[!] LOCALAPPDATA not found, using defaults\n");
        return settings;
    }

    char path[MAX_PATH];

    // Try unpackaged location first
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

    printf("[+] Loaded: %s\n", path);
    printf("[*] Parsing settings...\n");

    // Find "profiles" section
    const char* profilesStr = strstr(jsonData, "\"profiles\"");
    if (!profilesStr) {
        printf("[!] 'profiles' section not found\n");
        free(jsonData);
        return settings;
    }

    // Find "defaults" within profiles
    const char* defaultsStr = strstr(profilesStr, "\"defaults\"");
    if (!defaultsStr) {
        printf("[!] 'defaults' section not found\n");
        free(jsonData);
        return settings;
    }

    // Find the opening brace of defaults object
    const char* defaultsObj = strchr(defaultsStr + strlen("\"defaults\""), '{');
    if (!defaultsObj) {
        printf("[!] 'defaults' object not found\n");
        free(jsonData);
        return settings;
    }

    printf("  Parsing 'profiles.defaults':\n");

    // Create parser starting at defaults object
    JsonParser parser = {
        .data = defaultsObj,
        .pos = 0,
        .len = fileSize - (defaultsObj - jsonData)
    };

    // Parse the defaults object
    parser.pos++; // Skip '{'

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
        if (parser.pos >= parser.len || parser.data[parser.pos] != ':') {
            break;
        }
        parser.pos++;

        json_skip_whitespace(&parser);

        if (strcmp(key, "font") == 0 && parser.data[parser.pos] == '{') {
            printf("  Found 'font' object:\n");
            parser.pos++; // Skip '{'

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
                if (parser.pos >= parser.len || parser.data[parser.pos] != ':') {
                    break;
                }
                parser.pos++;

                json_skip_whitespace(&parser);

                if (strcmp(fontKey, "face") == 0) {
                    if (json_parse_string(&parser, settings.fontFace, sizeof(settings.fontFace))) {
                        printf("    - face: \"%s\"\n", settings.fontFace);
                    }
                } else if (strcmp(fontKey, "size") == 0) {
                    int size;
                    if (json_parse_number_int(&parser, &size)) {
                        settings.fontSize = size;
                        printf("    - size: %d\n", size);
                    }
                } else if (strcmp(fontKey, "weight") == 0) {
                    json_skip_value(&parser);
                } else {
                    json_skip_value(&parser);
                }

                json_skip_whitespace(&parser);
                if (parser.pos < parser.len && parser.data[parser.pos] == ',') {
                    parser.pos++;
                }
            }
        } else if (strcmp(key, "padding") == 0) {
            if (parser.data[parser.pos] == '"') {
                char paddingStr[64];
                if (json_parse_string(&parser, paddingStr, sizeof(paddingStr))) {
                    settings.padding = parse_padding_string(paddingStr);
                    printf("  Found 'padding': \"%s\" -> (%d, %d, %d, %d)\n", paddingStr, settings.padding.left, settings.padding.top, settings.padding.right, settings.padding.bottom);
                }
            } else {
                json_skip_value(&parser);
            }
        } else {
            json_skip_value(&parser);
        }

        json_skip_whitespace(&parser);
        if (parser.pos < parser.len && parser.data[parser.pos] == ',') {
            parser.pos++;
        }
    }

    free(jsonData);

    printf("\n[+] Final settings:\n");
    printf("    Font: %s @ %dpt\n", settings.fontFace, settings.fontSize);
    printf("    Line height: %.2f\n", settings.lineHeight);
    printf("    Padding: (%d, %d, %d, %d)\n", settings.padding.left, settings.padding.top, settings.padding.right, settings.padding.bottom);

    return settings;
}

// ============================================================================
// Font Metrics Calculation
// ============================================================================

static void calculate_cell_size(TerminalContext* ctx) {
    HDC hdc = GetDC(NULL);

    int effectiveFontSize = ctx->settings.fontSize + ctx->currentZoomLevel;
    if (effectiveFontSize < 1) {
        effectiveFontSize = 1;
    }

    int logPixelsY = GetDeviceCaps(hdc, LOGPIXELSY);
    int fontHeight = -MulDiv(effectiveFontSize, logPixelsY, 72);

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

        // Base cell size from font metrics
        int baseWidth = tm.tmAveCharWidth;
        int baseHeight = tm.tmHeight;

        // Apply line height multiplier
        ctx->cellHeight = (int)(baseHeight * ctx->settings.lineHeight);

        // Cell width is typically the average character width
        // Windows Terminal uses the font's actual metrics
        ctx->cellWidth = baseWidth;

        SelectObject(hdc, oldFont);
        DeleteObject(font);

        printf("[+] Cell size: %dx%d pixels (base: %dx%d, line height: %.2f, zoom: %+d)\n", ctx->cellWidth, ctx->cellHeight, baseWidth, baseHeight, ctx->settings.lineHeight, ctx->currentZoomLevel);
    } else {
        // Fallback estimation based on typical monospace font ratios
        double ptToPixel = logPixelsY / 72.0;
        ctx->cellHeight = (int)(effectiveFontSize * ptToPixel * ctx->settings.lineHeight);
        ctx->cellWidth = (int)(ctx->cellHeight * ctx->settings.cellWidth);

        printf("[!] Font '%s' not found, using estimated cell size: %dx%d\n", ctx->settings.fontFace, ctx->cellWidth, ctx->cellHeight);
    }

    ReleaseDC(NULL, hdc);
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

    if (strstr(className, "Windows.UI.Composition.DesktopWindowContentBridge")) {
        RECT rect;
        GetWindowRect(hwnd, &rect);

        POINT topLeft = { rect.left, rect.top };
        ScreenToClient(ctx->mainWindow, &topLeft);

        if (topLeft.y > 20) {
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
    EnumChildWindows(ctx->mainWindow, FindContentAreaCallback, (LPARAM)ctx);

    if (!ctx->contentArea) {
        ctx->tabBarHeight = 32;
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

    if (clientPt.y < ctx->tabBarHeight) {
        pos.inContentArea = FALSE;
        pos.pixelX = clientPt.x;
        pos.pixelY = clientPt.y;
        return pos;
    }

    int contentX = clientPt.x;
    int contentY = clientPt.y - ctx->tabBarHeight;

    int textX = contentX - ctx->settings.padding.left;
    int textY = contentY - ctx->settings.padding.top;

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

            if (ctrlDown && wParam == WM_KEYDOWN) {
                int oldZoom = g_ctx.currentZoomLevel;
                BOOL zoomChanged = FALSE;

                if (kb->vkCode == VK_OEM_PLUS || kb->vkCode == VK_ADD) {
                    g_ctx.currentZoomLevel += 2;
                    zoomChanged = TRUE;
                    printf("[FONT] INCREASED: %dpt -> %dpt (zoom: %+d)\n", g_ctx.settings.fontSize + oldZoom, g_ctx.settings.fontSize + g_ctx.currentZoomLevel, g_ctx.currentZoomLevel);
                } else if (kb->vkCode == VK_OEM_MINUS || kb->vkCode == VK_SUBTRACT) {
                    g_ctx.currentZoomLevel -= 2;
                    zoomChanged = TRUE;
                    printf("[FONT] DECREASED: %dpt -> %dpt (zoom: %+d)\n", g_ctx.settings.fontSize + oldZoom, g_ctx.settings.fontSize + g_ctx.currentZoomLevel, g_ctx.currentZoomLevel);
                } else if (kb->vkCode == '0' || kb->vkCode == VK_NUMPAD0) {
                    g_ctx.currentZoomLevel = 0;
                    zoomChanged = TRUE;
                    printf("[FONT] RESET: %dpt (zoom: 0)\n", g_ctx.settings.fontSize);
                }

                if (zoomChanged) {
                    calculate_cell_size(&g_ctx);
                }
            }

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
                    int oldZoom = g_ctx.currentZoomLevel;
                    if (delta > 0) {
                        g_ctx.currentZoomLevel += 2;
                        printf("[FONT] INCREASED (scroll): %dpt -> %dpt\n", g_ctx.settings.fontSize + oldZoom, g_ctx.settings.fontSize + g_ctx.currentZoomLevel);
                    } else {
                        g_ctx.currentZoomLevel -= 2;
                        printf("[FONT] DECREASED (scroll): %dpt -> %dpt\n", g_ctx.settings.fontSize + oldZoom, g_ctx.settings.fontSize + g_ctx.currentZoomLevel);
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

        int contentW = clientW - g_ctx.settings.padding.left - g_ctx.settings.padding.right;
        int contentH = clientH - g_ctx.tabBarHeight - g_ctx.settings.padding.top - g_ctx.settings.padding.bottom;

        int cols = (g_ctx.cellWidth > 0) ? contentW / g_ctx.cellWidth : 0;
        int rows = (g_ctx.cellHeight > 0) ? contentH / g_ctx.cellHeight : 0;

        printf("[WINDOW] RESIZE: %dx%d client:%dx%d grid:%dx%d\n", windowW, windowH, clientW, clientH, cols, rows);
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
// Control
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
    }
    if (g_mouseHook) {
        UnhookWindowsHookEx(g_mouseHook);
    }
    if (g_winEventHook) {
        UnhookWinEvent(g_winEventHook);
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

    printf("[*] Loading Windows Terminal settings...\n");
    g_ctx.settings = load_terminal_settings();
    g_ctx.currentZoomLevel = 0;

    printf("\n[*] Calculating initial cell size...\n");
    calculate_cell_size(&g_ctx);

    SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

    printf("\n[*] Spawning Windows Terminal...\n");

    wchar_t cmdLine[] = L"wt.exe";
    if (!CreateProcessW(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("[!] Failed to spawn wt.exe (error: %lu)\n", GetLastError());
        return 1;
    }

    printf("[+] Process created (PID: %lu)\n", pi.dwProcessId);
    printf("[*] Waiting for window...\n");

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

    detect_tab_bar_height(&g_ctx);

    printf("\n[*] Installing hooks...\n");

    g_keyboardHook = SetWindowsHookExA(WH_KEYBOARD_LL, KeyboardHookProc, NULL, 0);
    printf("[%c] Keyboard hook\n", g_keyboardHook ? '+' : '!');

    g_mouseHook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);
    printf("[%c] Mouse hook\n", g_mouseHook ? '+' : '!');

    g_winEventHook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_OBJECT_LOCATIONCHANGE, NULL, WinEventCallback, g_ctx.processId, 0, WINEVENT_OUTOFCONTEXT);
    printf("[%c] Window event hook\n", g_winEventHook ? '+' : '!');

    printf("\n=== Monitoring Started ===\n");
    printf("Ctrl+Plus/Minus = font size | Ctrl+0 = reset | Ctrl+C here = quit\n\n");

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
