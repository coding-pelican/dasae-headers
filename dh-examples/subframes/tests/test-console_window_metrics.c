#include "dh/core.h"
#include "dh/os/windows.h"

#include <stdio.h>

void printConsoleWindowMetrics(void) {
    let con_output      = GetStdHandle(STD_OUTPUT_HANDLE);
    let con_buf_info    = eval({
        var info = makeCleared$(CONSOLE_SCREEN_BUFFER_INFO);
        GetConsoleScreenBufferInfo(con_output, &info);
        eval_return info;
    });
    let con_font_info   = eval({
        var info = make$(CONSOLE_FONT_INFOEX, .cbSize = sizeOf$(CONSOLE_FONT_INFOEX));
        GetCurrentConsoleFontEx(con_output, false, &info);
        eval_return info;
    });
    let con_window      = GetConsoleWindow();
    let con_rect        = eval({
        var rect = makeCleared(RECT);
        GetWindowRect(con_window, &rect);
        eval_return rect;
    });
    let con_client_rect = eval({
        var rect = makeCleared(RECT);
        GetClientRect(con_window, &rect);
        eval_return rect;
    });
    let con_place       = eval({
        var place = make$(WINDOWPLACEMENT);
        GetWindowPlacement(con_window, &place);
        eval_return place;
    });
    let fg_window       = GetForegroundWindow();
    let fg_rect         = eval({
        var rect = makeCleared(RECT);
        GetWindowRect(fg_window, &rect);
        eval_return rect;
    });
    let fg_client_rect  = eval({
        var rect = makeCleared(RECT);
        GetClientRect(fg_window, &rect);
        eval_return rect;
    });
    let fg_place        = eval({
        var place = make$(WINDOWPLACEMENT);
        GetWindowPlacement(fg_window, &place);
        eval_return place;
    });
    let fg_mouse_pos    = eval({
        var pos = makeCleared$(POINT);
        GetCursorPos(&pos);
        eval_return pos;
    });

    printf("con_output: %p\n", con_output);

    printf("  con_buf_info:\n");
    printf("    dwSize: (%d, %d)\n", con_buf_info.dwSize.X, con_buf_info.dwSize.Y);
    printf("    dwCursorPosition: (%d, %d)\n", con_buf_info.dwCursorPosition.X, con_buf_info.dwCursorPosition.Y);
    printf("    srWindow: {%d, %d} - {%d, %d}\n", con_buf_info.srWindow.Left, con_buf_info.srWindow.Top, con_buf_info.srWindow.Right, con_buf_info.srWindow.Bottom);

    printf("  con_font_info:\n");
    printf("    cbSize: %lu\n", con_font_info.cbSize);
    printf("    dwFontSize: (%d, %d)\n", con_font_info.dwFontSize.X, con_font_info.dwFontSize.Y);
    printf("    FontFamily: %d\n", con_font_info.FontFamily);
    printf("    FontWeight: %d\n", con_font_info.FontWeight);
    printf("    FaceName: '%S'\n", con_font_info.FaceName);

    printf("  con_window:\n");
    printf("    hwnd: %p\n", con_window);
    printf("    rect: {%ld, %ld} - {%ld, %ld}\n", con_rect.left, con_rect.top, con_rect.right, con_rect.bottom);
    printf("    client_rect: {%ld, %ld} - {%ld, %ld}\n", con_client_rect.left, con_client_rect.top, con_client_rect.right, con_client_rect.bottom);
    printf("    place:\n");
    printf("      min: (%ld, %ld)\n", con_place.ptMinPosition.x, con_place.ptMinPosition.y);
    printf("      max: (%ld, %ld)\n", con_place.ptMaxPosition.x, con_place.ptMaxPosition.y);
    printf("      normal: {%ld, %ld} - {%ld, %ld}\n", con_place.rcNormalPosition.left, con_place.rcNormalPosition.top, con_place.rcNormalPosition.right, con_place.rcNormalPosition.bottom);

    printf("  fg_window:\n");
    printf("    hwnd: %p\n", fg_window);
    printf("    rect: {%ld, %ld} - {%ld, %ld}\n", fg_rect.left, fg_rect.top, fg_rect.right, fg_rect.bottom);
    printf("    client_rect: {%ld, %ld} - {%ld, %ld}\n", fg_client_rect.left, fg_client_rect.top, fg_client_rect.right, fg_client_rect.bottom);
    printf("    place:\n");
    printf("      min: (%ld, %ld)\n", fg_place.ptMinPosition.x, fg_place.ptMinPosition.y);
    printf("      max: (%ld, %ld)\n", fg_place.ptMaxPosition.x, fg_place.ptMaxPosition.y);
    printf("      normal: {%ld, %ld} - {%ld, %ld}\n", fg_place.rcNormalPosition.left, fg_place.rcNormalPosition.top, fg_place.rcNormalPosition.right, fg_place.rcNormalPosition.bottom);

    printf("  fg_mouse_pos:\n");
    printf("    dwSize (%ld, %ld)\n", fg_mouse_pos.x, fg_mouse_pos.y);

    printf("\n");
}

int main(void) {
    while (true) {
        if (GetAsyncKeyState(VK_ESCAPE)) {
            break;
        }
        if (GetAsyncKeyState('I')) {
            printConsoleWindowMetrics();
        }
        Sleep(100);
    }
    ignore getchar();
    return 0;
}
