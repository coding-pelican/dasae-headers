/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    proc.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-31 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows process management APIs
 * @details Manages processes and their environments. Controls the creation,
 *          termination, and parameterization of application processes.
 *
 * API Overview:
 * - Life Cycle: CreateProcess, TerminateProcess, ExitProcess
 * - Identification: GetCurrentProcess, GetCurrentProcessId, OpenProcess
 * - Environment: GetEnvironmentStrings, FreeEnvironmentStrings, GetCommandLine,
 *   GetEnvironmentVariable, SetEnvironmentVariable, GetCurrentDirectory, SetCurrentDirectory
 * - Standard Handles: GetStdHandle, SetStdHandle
 * - Topology: GetProcessGroupAffinity, SetProcessGroupAffinity, GetNumaNodeProcessorMask,
 *   GetNumaProcessorNode
 */
#ifndef os_windows_proc__included
#define os_windows_proc__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// Process and Thread API (Process Focus)
/// ==================================================================================
/// Provides functions for creating and managing processes.
///
/// Key APIs:
///
/// Process Creation:
/// - BOOL CreateProcess(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFO lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
///   Creates a new process and its primary thread.
///   @param lpApplicationName Full path to executable (\b NULL to use \b lpCommandLine)
///   @param lpCommandLine Command line string (can include application name)
///   @param lpProcessAttributes Security attributes for process (\b NULL for default)
///   @param lpThreadAttributes Security attributes for thread (\b NULL for default)
///   @param bInheritHandles Whether child inherits parent's handles
///   @param dwCreationFlags Creation flags: \b CREATE_NEW_CONSOLE, \b CREATE_SUSPENDED, \b DETACHED_PROCESS, \b CREATE_NO_WINDOW
///   @param lpEnvironment Environment block (\b NULL to inherit)
///   @param lpCurrentDirectory Working directory (\b NULL for parent's directory)
///   @param lpStartupInfo \b STARTUPINFO structure with window/console settings
///   @param lpProcessInformation Receives \b PROCESS_INFORMATION: \code{.c}
///     typedef struct _PROCESS_INFORMATION {
///         HANDLE hProcess;      // Process handle
///         HANDLE hThread;        // Primary thread handle
///         DWORD dwProcessId;     // Process ID
///         DWORD dwThreadId;      // Thread ID
///     } PROCESS_INFORMATION; \endcode
///   @return \b TRUE on success, \b FALSE on failure
///
/// Process Identification:
/// - HANDLE GetCurrentProcess(void);
///   Retrieves a pseudo-handle to the current process.
///   @return Constant (-1) representing current process. Use \b DuplicateHandle() to get a real handle.
///
/// - DWORD GetCurrentProcessId(void);
///   Retrieves the process identifier of the calling process.
///   @return Process ID (DWORD)
///
/// - HANDLE OpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
///   Opens an existing local process object.
///   @param dwDesiredAccess Access rights: \b PROCESS_ALL_ACCESS, \b PROCESS_QUERY_INFORMATION,
///     \b PROCESS_TERMINATE, \b PROCESS_VM_READ
///   @param bInheritHandle Whether handle is inheritable
///   @param dwProcessId Process identifier
///   @return Handle on success, \b NULL on failure
///
/// Process Termination:
/// - BOOL TerminateProcess(HANDLE hProcess, UINT uExitCode);
///   Terminates the specified process and all its threads.
///   @param hProcess Handle to the process
///   @param uExitCode Exit code for the process
///   @return \b TRUE on success, \b FALSE on failure
///   @note Use \b ExitProcess() for current process, \b TerminateProcess() for other processes.
///
/// - VOID ExitProcess(UINT uExitCode);
///   Ends the calling process and all its threads.
///   @param uExitCode Exit code for the process
///   @note This function does not return.
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/
#include <processthreadsapi.h>
/// ==================================================================================
/// Process Environment API
/// ==================================================================================
/// Provides functions for managing the process environment, including environment
/// variables, command line, and standard handles.
///
/// Key APIs (Environment Functions):
///
/// - LPCH GetEnvironmentStrings(void);
///   Retrieves the environment block for the current process.
///   @return Pointer to environment block. Format: "VAR1=value1\0VAR2=value2\0\0"
///   @note Free with \b FreeEnvironmentStrings.
///
/// - BOOL FreeEnvironmentStrings(LPCH lpszEnvironmentBlock);
///   Frees a block of environment strings.
///   @param lpszEnvironmentBlock Pointer to environment block from \b GetEnvironmentStrings
///   @return \b TRUE on success, \b FALSE on failure
///
/// - LPSTR GetCommandLineA(void);
///   Retrieves the command-line string for the current process (ANSI).
///   @return Pointer to command line string
///
/// - LPWSTR GetCommandLineW(void);
///   Retrieves the command-line string for the current process (Unicode).
///   @return Pointer to command line wide string
///
/// - DWORD GetEnvironmentVariableA(LPCSTR lpName, LPSTR lpBuffer, DWORD nSize);
///   Retrieves the value of the specified environment variable (ANSI).
///   @param lpName Name of the environment variable
///   @param lpBuffer Buffer to receive the value
///   @param nSize Size of the buffer in characters
///   @return Number of characters stored (excluding null), 0 on failure
///
/// - BOOL SetEnvironmentVariableA(LPCSTR lpName, LPCSTR lpValue);
///   Sets the value of an environment variable (ANSI).
///   @param lpName Name of the environment variable
///   @param lpValue Value to set (\b NULL to delete the variable)
///   @return \b TRUE on success, \b FALSE on failure
///
/// - DWORD GetCurrentDirectoryA(DWORD nBufferLength, LPSTR lpBuffer);
///   Retrieves the current directory for the current process (ANSI).
///   @param nBufferLength Size of the buffer in characters
///   @param lpBuffer Buffer to receive the current directory path
///   @return Number of characters written (excluding null), 0 on failure
///
/// - BOOL SetCurrentDirectoryA(LPCSTR lpPathName);
///   Changes the current directory for the current process (ANSI).
///   @param lpPathName Path to the new current directory
///   @return \b TRUE on success, \b FALSE on failure
///
/// Standard Handle Functions (also used by console):
/// - HANDLE GetStdHandle(DWORD nStdHandle);
///   Retrieves a handle to the specified standard device.
///   @param nStdHandle Device type: \b STD_INPUT_HANDLE (-10), \b STD_OUTPUT_HANDLE (-11), \b STD_ERROR_HANDLE (-12)
///   @return Handle on success, \b INVALID_HANDLE_VALUE on failure
///
/// - BOOL SetStdHandle(DWORD nStdHandle, HANDLE hHandle);
///   Sets the handle for the specified standard device.
///   @param nStdHandle Device type
///   @param hHandle New handle for the device
///   @return \b TRUE on success, \b FALSE on failure
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/processenv/
#include <processenv.h>
/// ==================================================================================
/// Process Topology API
/// ==================================================================================
/// Provides functions to manage processor groups and NUMA nodes. Useful for systems
/// with more than 64 logical processors or NUMA (Non-Uniform Memory Access) architectures.
///
/// Key APIs:
///
/// - BOOL GetProcessGroupAffinity(HANDLE hProcess, PUSHORT GroupCount, PUSHORT GroupArray);  [Min: Win7]
///   Retrieves the processor group affinity of the specified process.
///   @param hProcess Handle to the process (\b NULL for current process)
///   @param GroupCount Input: Size of GroupArray, Output: Number of groups
///   @param GroupArray Array to receive processor group numbers
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL SetProcessGroupAffinity(HANDLE hProcess, const USHORT* GroupArray, USHORT GroupCount);  [Min: Win7]
///   Sets the processor group affinity for the threads of the specified process.
///   @param hProcess Handle to the process (\b NULL for current process)
///   @param GroupArray Array of processor group numbers
///   @param GroupCount Number of groups in array
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL GetNumaNodeProcessorMask(USHORT Node, PULONGLONG ProcessorMask);
///   Retrieves the processor mask for the specified NUMA node.
///   @param Node NUMA node number
///   @param ProcessorMask Receives the processor mask
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL GetNumaProcessorNode(UCHAR Processor, PUSHORT NodeNumber);
///   Retrieves the NUMA node number for the specified processor.
///   @param Processor Processor number
///   @param NodeNumber Receives the NUMA node number
///   @return \b TRUE on success, \b FALSE on failure
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/processtopologyapi/
#include <processtopologyapi.h>
/// ==================================================================================
/// Process Constants (winbase.h subset)
/// ==================================================================================
/// These constants are typically defined in winbase.h but are needed for process operations.
/// Defining them here avoids the massive winbase.h dependency.
///
/// Standard Handle Identifiers (for GetStdHandle/SetStdHandle):
#if !defined(STD_INPUT_HANDLE)
#define STD_INPUT_HANDLE ((DWORD) - 10)
#endif
#if !defined(STD_OUTPUT_HANDLE)
#define STD_OUTPUT_HANDLE ((DWORD) - 11)
#endif
#if !defined(STD_ERROR_HANDLE)
#define STD_ERROR_HANDLE ((DWORD) - 12)
#endif
/// Process Creation Flags (dwCreationFlags for CreateProcess):
#if !defined(DEBUG_PROCESS)
#define DEBUG_PROCESS 0x00000001
#endif
#if !defined(DEBUG_ONLY_THIS_PROCESS)
#define DEBUG_ONLY_THIS_PROCESS 0x00000002
#endif
#if !defined(CREATE_SUSPENDED)
#define CREATE_SUSPENDED 0x00000004
#endif
#if !defined(DETACHED_PROCESS)
#define DETACHED_PROCESS 0x00000008
#endif
#if !defined(CREATE_NEW_CONSOLE)
#define CREATE_NEW_CONSOLE 0x00000010
#endif
#if !defined(NORMAL_PRIORITY_CLASS)
#define NORMAL_PRIORITY_CLASS 0x00000020
#endif
#if !defined(IDLE_PRIORITY_CLASS)
#define IDLE_PRIORITY_CLASS 0x00000040
#endif
#if !defined(HIGH_PRIORITY_CLASS)
#define HIGH_PRIORITY_CLASS 0x00000080
#endif
#if !defined(REALTIME_PRIORITY_CLASS)
#define REALTIME_PRIORITY_CLASS 0x00000100
#endif
#if !defined(CREATE_NEW_PROCESS_GROUP)
#define CREATE_NEW_PROCESS_GROUP 0x00000200
#endif
#if !defined(CREATE_UNICODE_ENVIRONMENT)
#define CREATE_UNICODE_ENVIRONMENT 0x00000400
#endif
#if !defined(CREATE_NO_WINDOW)
#define CREATE_NO_WINDOW 0x08000000
#endif
#if !defined(BELOW_NORMAL_PRIORITY_CLASS)
#define BELOW_NORMAL_PRIORITY_CLASS 0x00004000
#endif
#if !defined(ABOVE_NORMAL_PRIORITY_CLASS)
#define ABOVE_NORMAL_PRIORITY_CLASS 0x00008000
#endif
/// STARTUPINFO Flags (dwFlags for STARTUPINFO):
/// Used with CreateProcess to specify which STARTUPINFO fields are valid.
#if !defined(STARTF_USESHOWWINDOW)
#define STARTF_USESHOWWINDOW 0x00000001
#endif
#if !defined(STARTF_USESIZE)
#define STARTF_USESIZE 0x00000002
#endif
#if !defined(STARTF_USEPOSITION)
#define STARTF_USEPOSITION 0x00000004
#endif
#if !defined(STARTF_USECOUNTCHARS)
#define STARTF_USECOUNTCHARS 0x00000008
#endif
#if !defined(STARTF_USEFILLATTRIBUTE)
#define STARTF_USEFILLATTRIBUTE 0x00000010
#endif
#if !defined(STARTF_RUNFULLSCREEN)
#define STARTF_RUNFULLSCREEN 0x00000020
#endif
#if !defined(STARTF_FORCEONFEEDBACK)
#define STARTF_FORCEONFEEDBACK 0x00000040
#endif
#if !defined(STARTF_FORCEOFFFEEDBACK)
#define STARTF_FORCEOFFFEEDBACK 0x00000080
#endif
#if !defined(STARTF_USESTDHANDLES)
#define STARTF_USESTDHANDLES 0x00000100
#endif
#if !defined(STARTF_USEHOTKEY)
#define STARTF_USEHOTKEY 0x00000200
#endif
#if !defined(STARTF_TITLEISLINKNAME)
#define STARTF_TITLEISLINKNAME 0x00000800
#endif
#if !defined(STARTF_TITLEISAPPID)
#define STARTF_TITLEISAPPID 0x00001000
#endif
#if !defined(STARTF_PREVENTPINNING)
#define STARTF_PREVENTPINNING 0x00002000
#endif
/// ShowWindow Commands (wShowWindow in STARTUPINFO):
/// These are typically in winuser.h but needed for process creation.
#if !defined(SW_HIDE)
#define SW_HIDE 0
#endif
#if !defined(SW_SHOWNORMAL)
#define SW_SHOWNORMAL 1
#endif
#if !defined(SW_SHOWMINIMIZED)
#define SW_SHOWMINIMIZED 2
#endif
#if !defined(SW_SHOWMAXIMIZED)
#define SW_SHOWMAXIMIZED 3
#endif
#if !defined(SW_SHOWNOACTIVATE)
#define SW_SHOWNOACTIVATE 4
#endif
#if !defined(SW_SHOW)
#define SW_SHOW 5
#endif
#if !defined(SW_MINIMIZE)
#define SW_MINIMIZE 6
#endif
#if !defined(SW_SHOWMINNOACTIVE)
#define SW_SHOWMINNOACTIVE 7
#endif
#if !defined(SW_SHOWNA)
#define SW_SHOWNA 8
#endif
#if !defined(SW_RESTORE)
#define SW_RESTORE 9
#endif
#if !defined(SW_SHOWDEFAULT)
#define SW_SHOWDEFAULT 10
#endif
#if !defined(SW_FORCEMINIMIZE)
#define SW_FORCEMINIMIZE 11
#endif
#pragma comment(lib, "kernel32.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_proc__included */
