#include "engine/utils.h"

#if plat_windows
#include "dh/os/windows.h"

fn_((engine_utils_getch(void))(u8)) {
    let console_input_handle = GetStdHandle(STD_INPUT_HANDLE);
    if (console_input_handle == INVALID_HANDLE_VALUE) { return 0; }

    DWORD original_console_mode = 0;
    if (!GetConsoleMode(console_input_handle, &original_console_mode)) { return 0; }

    let new_console_mode = original_console_mode & (~ENABLE_ECHO_INPUT) & (~ENABLE_LINE_INPUT);
    if (!SetConsoleMode(console_input_handle, new_console_mode)) { return 0; }

    u8 character_buffer = 0;
    DWORD characters_read = 0;
    if (!ReadConsoleA(console_input_handle, &character_buffer, 1, &characters_read, null)
        || characters_read != 1) {
        character_buffer = 0;
    }
    SetConsoleMode(console_input_handle, original_console_mode);
    return character_buffer;
}

fn_((engine_utils_kbhit(void))(bool)) {
    let console_input_handle = GetStdHandle(STD_INPUT_HANDLE);
    if (console_input_handle == INVALID_HANDLE_VALUE) { return false; }

    DWORD total_events = 0;
    if (!GetNumberOfConsoleInputEvents(console_input_handle, &total_events)) { return false; }
    if (total_events <= 1) { return false; }

    INPUT_RECORD event_buffer[128] = {};
    DWORD events_read = 0;
    if (!PeekConsoleInput(console_input_handle, event_buffer, 128, &events_read)) { return false; }
    for (DWORD event_index = 0; event_index < events_read; ++event_index) {
        if (event_buffer[event_index].EventType == KEY_EVENT
            && event_buffer[event_index].Event.KeyEvent.bKeyDown) {
            return true;
        }
    }
    return false;
}

#else /* plat_unix */
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

fn_((engine_utils_getch(void))(u8)) {
    struct termios original_terminal_attrs = {};
    if (tcgetattr(STDIN_FILENO, &original_terminal_attrs) < 0) { return 0; }

    let new_terminal_attrs = original_terminal_attrs;
    new_terminal_attrs.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal_attrs) < 0) { return 0; }

    u8 character_buffer = 0;
    if (read(STDIN_FILENO, &character_buffer, 1) < 0) { character_buffer = 0; }
    tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal_attrs);
    return character_buffer;
}

fn_((engine_utils_kbhit(void))(bool)) {
    struct termios original_terminal_attrs = {};
    if (tcgetattr(STDIN_FILENO, &original_terminal_attrs) < 0) { return false; }

    let new_terminal_attrs = original_terminal_attrs;
    new_terminal_attrs.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal_attrs) < 0) {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal_attrs);
        return false;
    }

    let original_file_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (original_file_flags < 0) {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal_attrs);
        return false;
    }
    if (fcntl(STDIN_FILENO, F_SETFL, original_file_flags | O_NONBLOCK) < 0) {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal_attrs);
        return false;
    }

    let character_from_input = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal_attrs);
    fcntl(STDIN_FILENO, F_SETFL, original_file_flags);
    if (character_from_input != EOF) {
        ungetc(character_from_input, stdin);
        return true;
    }
    return false;
}
#endif /* plat_unix */
