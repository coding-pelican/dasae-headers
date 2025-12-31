/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    console.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-31 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows console support APIs
 * @details Manages character-mode console subsystems. Supports reading from and
 *          writing to console input/output buffers and customizing console properties.
 *
 * API Overview:
 * - Allocation: AllocConsole, FreeConsole, AttachConsole
 * - Input/Output: ReadConsole, WriteConsole
 * - Configuration: SetConsoleMode, GetConsoleMode, SetConsoleCursorPosition,
 *   SetConsoleTextAttribute, GetConsoleScreenBufferInfo, FillConsoleOutputCharacter,
 *   FillConsoleOutputAttribute
 *
 * @note Standard handles (GetStdHandle, SetStdHandle) are defined in proc.h via processenv.h.
 */
#ifndef os_windows_console__included
#define os_windows_console__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// Console API
/// ==================================================================================
/// Provides functions to manage the console window and input/output buffers.
///
/// Key APIs:
///
/// Console Window Management:
/// - BOOL AllocConsole(void);
///   Allocates a new console for the calling process.
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL FreeConsole(void);
///   Detaches the calling process from its console.
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL AttachConsole(DWORD dwProcessId);
///   Attaches the calling process to the console of the specified process.
///   @param dwProcessId Process ID to attach to, or \b ATTACH_PARENT_PROCESS (-1) for parent's console
///   @return \b TRUE on success, \b FALSE on failure
///
/// Console I/O:
/// - BOOL WriteConsole(HANDLE hConsoleOutput, const VOID* lpBuffer, DWORD nNumberOfCharsToWrite, LPDWORD lpNumberOfCharsWritten, LPVOID lpReserved);
///   Writes a character string to the console screen buffer.
///   @param hConsoleOutput Handle to console output (use \b GetStdHandle(STD_OUTPUT_HANDLE))
///   @param lpBuffer Buffer containing characters to write
///   @param nNumberOfCharsToWrite Number of characters to write
///   @param lpNumberOfCharsWritten Receives number of characters written (can be \b NULL)
///   @param lpReserved Reserved, must be \b NULL
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL ReadConsole(HANDLE hConsoleInput, LPVOID lpBuffer, DWORD nNumberOfCharsToRead, LPDWORD lpNumberOfCharsRead, LPVOID pInputControl);
///   Reads character input from the console input buffer.
///   @param hConsoleInput Handle to console input (use \b GetStdHandle(STD_INPUT_HANDLE))
///   @param lpBuffer Buffer to receive characters
///   @param nNumberOfCharsToRead Maximum number of characters to read
///   @param lpNumberOfCharsRead Receives number of characters read (can be \b NULL)
///   @param pInputControl Input control structure (can be \b NULL)
///   @return \b TRUE on success, \b FALSE on failure
///
/// Console Configuration:
/// - BOOL SetConsoleMode(HANDLE hConsoleHandle, DWORD dwMode);
///   Sets the input or output mode of a console.
///   @param hConsoleHandle Handle to console input or output
///   @param dwMode Mode flags: \b ENABLE_VIRTUAL_TERMINAL_PROCESSING (VT100 sequences),
///     \b ENABLE_PROCESSED_INPUT (process control chars), \b ENABLE_LINE_INPUT (wait for Enter)
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL GetConsoleMode(HANDLE hConsoleHandle, LPDWORD lpMode);
///   Retrieves the current input or output mode of a console.
///   @param hConsoleHandle Handle to console input or output
///   @param lpMode Receives current mode flags
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL SetConsoleCursorPosition(HANDLE hConsoleOutput, COORD dwCursorPosition);
///   Sets the cursor position in the console screen buffer.
///   @param hConsoleOutput Handle to console output
///   @param dwCursorPosition \b COORD structure: \code{.c} { SHORT X; SHORT Y; } \endcode
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL SetConsoleTextAttribute(HANDLE hConsoleOutput, WORD wAttributes);
///   Sets the foreground and background colors.
///   @param hConsoleOutput Handle to console output
///   @param wAttributes Bit flags: \b FOREGROUND_BLUE/GREEN/RED/INTENSITY, \b BACKGROUND_BLUE/GREEN/RED/INTENSITY
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL GetConsoleScreenBufferInfo(HANDLE hConsoleOutput, PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo);
///   Retrieves information about the console screen buffer (size, cursor position, attributes).
///
/// - BOOL FillConsoleOutputCharacter(HANDLE hConsoleOutput, TCHAR cCharacter, DWORD nLength, COORD dwWriteCoord, LPDWORD lpNumberOfCharsWritten);
///   Writes a character to the console screen buffer a specified number of times.
///
/// - BOOL FillConsoleOutputAttribute(HANDLE hConsoleOutput, WORD wAttribute, DWORD nLength, COORD dwWriteCoord, LPDWORD lpNumberOfAttrsWritten);
///   Sets the text and background color attributes for a specified number of character cells.
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/console/console-functions
#include <consoleapi.h>
/// ==================================================================================
/// Console Constants (winbase.h/wincon.h subset)
/// ==================================================================================
/// These constants are typically defined in winbase.h or wincon.h but are needed for
/// console operations. Defining them here avoids the massive winbase.h dependency.
///
/// AttachConsole Special Value:
#if !defined(ATTACH_PARENT_PROCESS)
#define ATTACH_PARENT_PROCESS ((DWORD) - 1)
#endif
/// Console Input Mode Flags (SetConsoleMode for input handles):
#if !defined(ENABLE_PROCESSED_INPUT)
#define ENABLE_PROCESSED_INPUT 0x0001
#endif
#if !defined(ENABLE_LINE_INPUT)
#define ENABLE_LINE_INPUT 0x0002
#endif
#if !defined(ENABLE_ECHO_INPUT)
#define ENABLE_ECHO_INPUT 0x0004
#endif
#if !defined(ENABLE_WINDOW_INPUT)
#define ENABLE_WINDOW_INPUT 0x0008
#endif
#if !defined(ENABLE_MOUSE_INPUT)
#define ENABLE_MOUSE_INPUT 0x0010
#endif
#if !defined(ENABLE_INSERT_MODE)
#define ENABLE_INSERT_MODE 0x0020
#endif
#if !defined(ENABLE_QUICK_EDIT_MODE)
#define ENABLE_QUICK_EDIT_MODE 0x0040
#endif
#if !defined(ENABLE_EXTENDED_FLAGS)
#define ENABLE_EXTENDED_FLAGS 0x0080
#endif
#if !defined(ENABLE_VIRTUAL_TERMINAL_INPUT)
#define ENABLE_VIRTUAL_TERMINAL_INPUT 0x0200
#endif
/// Console Output Mode Flags (SetConsoleMode for output handles):
#if !defined(ENABLE_PROCESSED_OUTPUT)
#define ENABLE_PROCESSED_OUTPUT 0x0001
#endif
#if !defined(ENABLE_WRAP_AT_EOL_OUTPUT)
#define ENABLE_WRAP_AT_EOL_OUTPUT 0x0002
#endif
#if !defined(ENABLE_VIRTUAL_TERMINAL_PROCESSING)
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#if !defined(DISABLE_NEWLINE_AUTO_RETURN)
#define DISABLE_NEWLINE_AUTO_RETURN 0x0008
#endif
#if !defined(ENABLE_LVB_GRID_WORLDWIDE)
#define ENABLE_LVB_GRID_WORLDWIDE 0x0010
#endif
/// Console Text Attributes (SetConsoleTextAttribute):
#if !defined(FOREGROUND_BLUE)
#define FOREGROUND_BLUE 0x0001
#endif
#if !defined(FOREGROUND_GREEN)
#define FOREGROUND_GREEN 0x0002
#endif
#if !defined(FOREGROUND_RED)
#define FOREGROUND_RED 0x0004
#endif
#if !defined(FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSITY 0x0008
#endif
#if !defined(BACKGROUND_BLUE)
#define BACKGROUND_BLUE 0x0010
#endif
#if !defined(BACKGROUND_GREEN)
#define BACKGROUND_GREEN 0x0020
#endif
#if !defined(BACKGROUND_RED)
#define BACKGROUND_RED 0x0040
#endif
#if !defined(BACKGROUND_INTENSITY)
#define BACKGROUND_INTENSITY 0x0080
#endif
/// Console Control Events (for SetConsoleCtrlHandler):
/// These are in consoleapi.h but included here for backward compatibility.
#if !defined(CTRL_C_EVENT)
#define CTRL_C_EVENT 0
#endif
#if !defined(CTRL_BREAK_EVENT)
#define CTRL_BREAK_EVENT 1
#endif
#if !defined(CTRL_CLOSE_EVENT)
#define CTRL_CLOSE_EVENT 2
#endif
#if !defined(CTRL_LOGOFF_EVENT)
#define CTRL_LOGOFF_EVENT 5
#endif
#if !defined(CTRL_SHUTDOWN_EVENT)
#define CTRL_SHUTDOWN_EVENT 6
#endif
/// Common Color Combinations (convenience macros):
#if !defined(FOREGROUND_WHITE)
#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#endif
#if !defined(BACKGROUND_WHITE)
#define BACKGROUND_WHITE (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
#endif
#pragma comment(lib, "kernel32.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_console__included */
