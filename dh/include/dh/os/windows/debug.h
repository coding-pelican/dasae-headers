/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    debug.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-31 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows debugging APIs
 * @details Provides functions for debugging applications, including debugger
 *          detection, debug output, breakpoints, and process debugging.
 *
 * API Overview:
 * - Detection: IsDebuggerPresent, CheckRemoteDebuggerPresent
 * - Output: OutputDebugStringA, OutputDebugStringW
 * - Breakpoints: DebugBreak, DebugBreakProcess
 * - Process Debugging: DebugActiveProcess, DebugActiveProcessStop,
 *   ContinueDebugEvent, WaitForDebugEvent, WaitForDebugEventEx [Min: Win10]
 */
#ifndef os_windows_debug__included
#define os_windows_debug__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// Debug API
/// ==================================================================================
/// Provides functions for debugging applications, including debugger detection,
/// debug output, breakpoints, and process debugging capabilities.
///
/// Key APIs:
///
/// Debugger Detection:
/// - BOOL IsDebuggerPresent(void);
///   Determines whether the calling process is being debugged by a user-mode debugger.
///   @return \b TRUE if the process is being debugged, \b FALSE otherwise
///   @note Only detects user-mode debuggers (e.g., Visual Studio). Kernel debuggers
///   are not detected. Can be bypassed by malware.
///
/// - BOOL CheckRemoteDebuggerPresent(HANDLE hProcess, PBOOL pbDebuggerPresent);
///   Determines whether the specified process is being debugged.
///   @param hProcess Handle to the process (use \b GetCurrentProcess() for current)
///   @param pbDebuggerPresent Receives \b TRUE if debugger is attached, \b FALSE otherwise
///   @return \b TRUE on success, \b FALSE on failure
///   @note Can check other processes if you have appropriate access rights.
///
/// Debug Output:
/// - VOID OutputDebugStringA(LPCSTR lpOutputString);
///   Sends a string to the debugger for display (ANSI version).
///   @param lpOutputString Null-terminated string to output
///   @note If no debugger is attached, the string is discarded.
///   Use \b DebugView (Sysinternals) to capture output without a debugger.
///
/// - VOID OutputDebugStringW(LPCWSTR lpOutputString);
///   Sends a string to the debugger for display (Unicode version).
///   @param lpOutputString Null-terminated wide string to output
///
/// Breakpoints:
/// - VOID DebugBreak(void);
///   Causes a breakpoint exception to occur in the current process.
///   @note If a debugger is attached, it will break. Otherwise, the process will
///   crash with an unhandled exception unless a structured exception handler catches it.
///   Use \b __debugbreak() intrinsic for inline breakpoints in MSVC.
///
/// - BOOL DebugBreakProcess(HANDLE Process);
///   Causes a breakpoint exception in the specified process.
///   @param Process Handle to the process
///   @return \b TRUE on success, \b FALSE on failure
///   @note Requires \b PROCESS_ALL_ACCESS or equivalent rights.
///
/// Process Debugging (Advanced):
/// - BOOL DebugActiveProcess(DWORD dwProcessId);
///   Enables a debugger to attach to an active process and debug it.
///   @param dwProcessId Process identifier of the process to debug
///   @return \b TRUE on success, \b FALSE on failure
///   @note Requires appropriate privileges. The debugger will receive debug events.
///
/// - BOOL DebugActiveProcessStop(DWORD dwProcessId);
///   Stops the debugger from debugging the specified process.
///   @param dwProcessId Process identifier of the process to stop debugging
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL WaitForDebugEvent(LPDEBUG_EVENT lpDebugEvent, DWORD dwMilliseconds);
///   Waits for a debugging event to occur in a process being debugged.
///   @param lpDebugEvent Pointer to \b DEBUG_EVENT structure to receive event info
///   @param dwMilliseconds Timeout in milliseconds (\b INFINITE for no timeout)
///   @return \b TRUE if event occurred, \b FALSE on timeout or error
///
/// - BOOL WaitForDebugEventEx(LPDEBUG_EVENT lpDebugEvent, DWORD dwMilliseconds);  [Min: Win10]
///   Extended version of WaitForDebugEvent with improved performance.
///   @param lpDebugEvent Pointer to \b DEBUG_EVENT structure
///   @param dwMilliseconds Timeout in milliseconds
///   @return \b TRUE if event occurred, \b FALSE on timeout or error
///   @note Requires Windows 10 or later. Preferred over \b WaitForDebugEvent.
///
/// - BOOL ContinueDebugEvent(DWORD dwProcessId, DWORD dwThreadId, DWORD dwContinueStatus);
///   Enables a debugger to continue a thread that reported a debugging event.
///   @param dwProcessId Process identifier
///   @param dwThreadId Thread identifier
///   @param dwContinueStatus Continue status:
///     - \b DBG_CONTINUE : Exception handled, continue execution
///     - \b DBG_EXCEPTION_NOT_HANDLED : Exception not handled, pass to exception handlers
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL DebugSetProcessKillOnExit(BOOL KillOnExit);
///   Sets whether the debuggee should be terminated when the debugger exits.
///   @param KillOnExit \b TRUE to terminate debuggee on exit, \b FALSE to detach
///   @return \b TRUE on success, \b FALSE on failure
///
/// DEBUG_EVENT Structure:
/// @code{.c}
/// typedef struct _DEBUG_EVENT {
///     DWORD dwDebugEventCode;    // Event type
///     DWORD dwProcessId;         // Process ID
///     DWORD dwThreadId;          // Thread ID
///     union {
///         EXCEPTION_DEBUG_INFO Exception;
///         CREATE_THREAD_DEBUG_INFO CreateThread;
///         CREATE_PROCESS_DEBUG_INFO CreateProcessInfo;
///         EXIT_THREAD_DEBUG_INFO ExitThread;
///         EXIT_PROCESS_DEBUG_INFO ExitProcess;
///         LOAD_DLL_DEBUG_INFO LoadDll;
///         UNLOAD_DLL_DEBUG_INFO UnloadDll;
///         OUTPUT_DEBUG_STRING_INFO DebugString;
///         RIP_INFO RipInfo;
///     } u;
/// } DEBUG_EVENT;
/// @endcode
///
/// Debug Event Codes:
/// - \b EXCEPTION_DEBUG_EVENT : Exception occurred
/// - \b CREATE_THREAD_DEBUG_EVENT : Thread created
/// - \b CREATE_PROCESS_DEBUG_EVENT : Process created
/// - \b EXIT_THREAD_DEBUG_EVENT : Thread exited
/// - \b EXIT_PROCESS_DEBUG_EVENT : Process exited
/// - \b LOAD_DLL_DEBUG_EVENT : DLL loaded
/// - \b UNLOAD_DLL_DEBUG_EVENT : DLL unloaded
/// - \b OUTPUT_DEBUG_STRING_EVENT : OutputDebugString called
/// - \b RIP_EVENT : System debugging error
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/debugapi/
#include <debugapi.h>
/// ==================================================================================
/// Debug Constants (winbase.h subset)
/// ==================================================================================
/// These constants are typically defined in winbase.h but are needed for debug operations.
/// Defining them here avoids the massive winbase.h dependency.
///
/// ContinueDebugEvent Status Codes:
#if !defined(DBG_CONTINUE)
#define DBG_CONTINUE ((DWORD)0x00010002L)
#endif
#if !defined(DBG_EXCEPTION_NOT_HANDLED)
#define DBG_EXCEPTION_NOT_HANDLED ((DWORD)0x80010001L)
#endif
#if !defined(DBG_REPLY_LATER)
#define DBG_REPLY_LATER ((DWORD)0x40010001L)
#endif
#if !defined(DBG_TERMINATE_THREAD)
#define DBG_TERMINATE_THREAD ((DWORD)0x40010003L)
#endif
#if !defined(DBG_TERMINATE_PROCESS)
#define DBG_TERMINATE_PROCESS ((DWORD)0x40010004L)
#endif
#if !defined(DBG_CONTROL_C)
#define DBG_CONTROL_C ((DWORD)0x40010005L)
#endif
#if !defined(DBG_PRINTEXCEPTION_C)
#define DBG_PRINTEXCEPTION_C ((DWORD)0x40010006L)
#endif
#if !defined(DBG_RIPEXCEPTION)
#define DBG_RIPEXCEPTION ((DWORD)0x40010007L)
#endif
#if !defined(DBG_CONTROL_BREAK)
#define DBG_CONTROL_BREAK ((DWORD)0x40010008L)
#endif
#if !defined(DBG_COMMAND_EXCEPTION)
#define DBG_COMMAND_EXCEPTION ((DWORD)0x40010009L)
#endif
/// Debug Event Codes (DEBUG_EVENT.dwDebugEventCode):
#if !defined(EXCEPTION_DEBUG_EVENT)
#define EXCEPTION_DEBUG_EVENT 1
#endif
#if !defined(CREATE_THREAD_DEBUG_EVENT)
#define CREATE_THREAD_DEBUG_EVENT 2
#endif
#if !defined(CREATE_PROCESS_DEBUG_EVENT)
#define CREATE_PROCESS_DEBUG_EVENT 3
#endif
#if !defined(EXIT_THREAD_DEBUG_EVENT)
#define EXIT_THREAD_DEBUG_EVENT 4
#endif
#if !defined(EXIT_PROCESS_DEBUG_EVENT)
#define EXIT_PROCESS_DEBUG_EVENT 5
#endif
#if !defined(LOAD_DLL_DEBUG_EVENT)
#define LOAD_DLL_DEBUG_EVENT 6
#endif
#if !defined(UNLOAD_DLL_DEBUG_EVENT)
#define UNLOAD_DLL_DEBUG_EVENT 7
#endif
#if !defined(OUTPUT_DEBUG_STRING_EVENT)
#define OUTPUT_DEBUG_STRING_EVENT 8
#endif
#if !defined(RIP_EVENT)
#define RIP_EVENT 9
#endif
#pragma comment(lib, "kernel32.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_debug__included */
