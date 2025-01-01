#include <windows.h>
#include <stdio.h>
#include <wchar.h>

// A basic color struct for demonstration
typedef struct Color {
    unsigned char r, g, b, a;
} Color;

// Minimal grayscale helper
static inline unsigned char getGrayscale(const Color* c) {
    // R*0.299 + G*0.587 + B*0.114 ~= c->r*77 + c->g*150 + c->b*29 >> 8
    return (unsigned char)((c->r * 77 + c->g * 150 + c->b * 29) >> 8);
}

static HWND s_notepadHwnd = NULL; // We'll store Notepad's main window

// 1) Create or overwrite a file named "game_output.txt"
static void writeFileDemo(const wchar_t* text) {
    FILE* fp = _wfopen(L"game_output.txt", L"w");
    if (fp) {
        fputws(text, fp);
        fclose(fp);
    } else {
        wprintf(L"Failed to open game_output.txt for writing.\n");
    }
}

// 2) Launch (or reuse) Notepad with "game_output.txt"
static void launchNotepad(void) {
    // If we haven't already, launch Notepad to open that file
    if (!s_notepadHwnd || !IsWindow(s_notepadHwnd)) {
        // Open notepad with that file
        HINSTANCE inst = ShellExecuteW(NULL, L"open", L"notepad.exe", L"game_output.txt", NULL, SW_SHOW);
        if ((INT_PTR)inst <= 32) {
            wprintf(L"ShellExecuteW failed. Code = %ld\n", (long)inst);
            return;
        }

        // Wait up to 3 seconds for the Notepad window
        DWORD start = GetTickCount();
        for (;;) {
            HWND wnd = FindWindowW(L"Notepad", NULL);
            if (!wnd) {
                // Try Windows 11 store version class name
                wnd = FindWindowW(L"ApplicationFrameWindow", L"Notepad");
            }
            if (wnd) {
                s_notepadHwnd = wnd;
                break;
            }
            if (GetTickCount() - start > 3000) {
                wprintf(L"Timed out waiting for Notepad window.\n");
                break;
            }
            Sleep(100);
        }
    }
}

// 3) Find the Notepad's edit or rich-edit control and set a normal font
static void setupNotepadFont(void) {
    if (!s_notepadHwnd || !IsWindow(s_notepadHwnd)) {
        return;
    }

    // Attempt to find the edit control
    HWND edit = FindWindowExW(s_notepadHwnd, NULL, L"Edit", NULL);
    if (!edit) {
        edit = FindWindowExW(s_notepadHwnd, NULL, L"RichEditD2DPT", NULL);
    }
    if (!edit) {
        edit = FindWindowExW(s_notepadHwnd, NULL, L"RICHEDIT50W", NULL);
    }

    if (!edit) {
        wprintf(L"Could not find an Edit/RichEdit control in Notepad.\n");
        return;
    }

    // Create a reasonable font (14 px Consolas)
    HFONT hFont = CreateFontW(
        14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, L"Consolas"
    );
    if (!hFont) {
        wprintf(L"CreateFontW failed!\n");
        return;
    }

    // Assign the font to the edit control
    SendMessageW(edit, WM_SETFONT, (WPARAM)hFont, TRUE);

    // Optional: bring Notepad to front and resize
    SetWindowPos(s_notepadHwnd, HWND_TOP, 100, 100, 800, 600, SWP_SHOWWINDOW);
}

// 4) Convert a small “image” (fake) to ASCII and write it to file
static void presentBufferDemo(const Color* data, int width, int height) {
    static const wchar_t SHADE_CHARS[] = L" .',:!*|+%8#@█";
    static const int     SHADE_LEVELS  = (sizeof(SHADE_CHARS) / sizeof(wchar_t)) - 1;

    if (!data) {
        return;
    }

    // We'll store in a local buffer
    static wchar_t buffer[8192];
    int            index = 0;

    // Convert the pixel data into ASCII lines
    for (int y = 0; y < height; y += 2) {
        for (int x = 0; x < width; x++) {
            const Color*  upper      = &data[y * width + x];
            const Color*  lower      = (y + 1 < height) ? &data[(y + 1) * width + x] : upper;
            unsigned char avg        = (unsigned char)((getGrayscale(upper) + getGrayscale(lower)) >> 1);
            unsigned char shadeIndex = (unsigned char)((avg * (SHADE_LEVELS - 1)) >> 8);
            buffer[index++]          = SHADE_CHARS[shadeIndex];
        }
        buffer[index++] = L'\r';
        buffer[index++] = L'\n';
        if (index > (int)(sizeof(buffer) / sizeof(buffer[0])) - 3) {
            // Prevent overflow
            break;
        }
    }
    buffer[index] = L'\0';

    // Write ASCII to file
    writeFileDemo(buffer);
}

int main(void) {
    // 1) Write an initial file
    writeFileDemo(L"Initial hello...\n");

    // 2) Launch Notepad to show that file
    launchNotepad();

    // 3) Set up a normal font
    setupNotepadFont();

    // For demonstration, create a small “image”
    Color fakeImage[80 * 20];
    for (int i = 0; i < 80 * 20; i++) {
        // Some gradient pattern
        int x          = i % 80;
        int y          = i / 80;
        fakeImage[i].r = (unsigned char)(x * 3);
        fakeImage[i].g = (unsigned char)(y * 12);
        fakeImage[i].b = 128;
        fakeImage[i].a = 255;
    }

    // 4) Convert that “image” to ASCII and write it to file
    presentBufferDemo(fakeImage, 80, 20);

    wprintf(L"ASCII data written to file.\n");
    wprintf(L"Please switch to Notepad and press F5 to reload.\n");
    wprintf(L"Press ENTER to quit.\n");
    getchar();

    return 0;
}
