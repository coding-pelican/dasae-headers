/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    thrd.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-31 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows thread management APIs
 * @details Manages threads within a process. Provides functions to create,
 *          suspend, resume, and query the state of execution threads.
 *
 * API Overview:
 * - Creation: CreateThread, ExitThread
 * - Identification: GetCurrentThread, GetCurrentThreadId, OpenThread
 * - Control: SuspendThread, ResumeThread, TerminateThread
 * - State: GetExitCodeThread, GetThreadPriority, SetThreadPriority
 * - TLS: TlsAlloc, TlsFree, TlsGetValue, TlsSetValue
 *
 * @note Sleep/SleepEx are in sync.h (synchapi.h).
 */
#ifndef os_windows_thrd__included
#define os_windows_thrd__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// Process and Thread API (Thread Focus)
/// ==================================================================================
/// Provides functions for creating and managing threads within a process.
///
/// Key APIs:
///
/// Thread Creation:
/// - HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
///   Creates a thread to execute within the address space of the calling process.
///   @param lpThreadAttributes Security attributes (\b NULL for default)
///   @param dwStackSize Stack size in bytes (0 for default, typically 1MB)
///   @param lpStartAddress Pointer to thread function: \code{.c} DWORD WINAPI ThreadProc(LPVOID lpParameter); \endcode
///   @param lpParameter Parameter passed to thread function
///   @param dwCreationFlags Creation flags:
///     - \b 0 : Thread runs immediately
///     - \b CREATE_SUSPENDED : Thread created in suspended state
///   @param lpThreadId Receives thread identifier (can be \b NULL)
///   @return Handle on success, \b NULL on failure
///
/// Thread Identification:
/// - HANDLE GetCurrentThread(void);
///   Retrieves a pseudo-handle to the current thread. Returns constant (-2) representing
///   the current thread. Use DuplicateHandle() to get a real handle.
///
/// - DWORD GetCurrentThreadId(void);
///   Retrieves the thread identifier of the calling thread. Returns the thread ID.
///
/// - HANDLE OpenThread(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwThreadId);
///   Opens an existing thread object.
///   @param dwDesiredAccess Access rights:
///     - \b THREAD_ALL_ACCESS : All possible access rights
///     - \b THREAD_QUERY_INFORMATION : Query information
///     - \b THREAD_SUSPEND_RESUME : Suspend/resume thread
///     - \b THREAD_TERMINATE : Terminate thread
///   @param bInheritHandle Whether handle is inheritable
///   @param dwThreadId Thread identifier
///   @return Handle on success, \b NULL on failure
///
/// Thread Control:
/// - DWORD SuspendThread(HANDLE hThread);
///   Suspends the specified thread.
///   @param hThread Handle to the thread
///   @return Previous suspend count, (DWORD)-1 on failure
///   @note Each \b SuspendThread must be matched with a \b ResumeThread.
///
/// - DWORD ResumeThread(HANDLE hThread);
///   Decrements a thread's suspend count.
///   @param hThread Handle to the thread
///   @return Previous suspend count, (DWORD)-1 on failure
///   @note When suspend count reaches zero, thread resumes execution.
///
/// - BOOL TerminateThread(HANDLE hThread, DWORD dwExitCode);
///   Terminates a thread.
///   @param hThread Handle to the thread
///   @param dwExitCode Exit code for the thread
///   @return \b TRUE on success, \b FALSE on failure
///   @warning Use with extreme caution - can leave resources in inconsistent state.
///   Prefer having threads exit naturally via return or \b ExitThread().
///
/// - VOID ExitThread(DWORD dwExitCode);
///   Ends the calling thread.
///   @param dwExitCode Exit code for the thread
///   @note This function does not return. Use this or return from thread function to exit thread cleanly.
///
/// Thread State:
/// - BOOL GetExitCodeThread(HANDLE hThread, LPDWORD lpExitCode);
///   Retrieves the exit code of the specified thread.
///   @param hThread Handle to the thread
///   @param lpExitCode Receives exit code (\b STILL_ACTIVE if thread is still running)
///   @return \b TRUE on success, \b FALSE on failure
///
/// - int GetThreadPriority(HANDLE hThread);
///   Retrieves the priority value for the specified thread.
///   @param hThread Handle to the thread
///   @return Priority value, \b THREAD_PRIORITY_ERROR_RETURN on failure
///   @note Priority values: \b THREAD_PRIORITY_IDLE, \b THREAD_PRIORITY_LOWEST, \b THREAD_PRIORITY_BELOW_NORMAL,
///   \b THREAD_PRIORITY_NORMAL, \b THREAD_PRIORITY_ABOVE_NORMAL, \b THREAD_PRIORITY_HIGHEST, \b THREAD_PRIORITY_TIME_CRITICAL
///
/// - BOOL SetThreadPriority(HANDLE hThread, int nPriority);
///   Sets the priority value for the specified thread.
///   @param hThread Handle to the thread
///   @param nPriority Priority value (see \b GetThreadPriority for values)
///   @return \b TRUE on success, \b FALSE on failure
///
/// Thread Local Storage (TLS):
/// - DWORD TlsAlloc(void);
///   Allocates a thread local storage (TLS) index.
///   @return Index on success, \b TLS_OUT_OF_INDEXES on failure
///   @note Each thread has its own copy of data stored at this index.
///
/// - BOOL TlsFree(DWORD dwTlsIndex);
///   Frees a TLS index.
///   @param dwTlsIndex TLS index to free
///   @return \b TRUE on success, \b FALSE on failure
///
/// - LPVOID TlsGetValue(DWORD dwTlsIndex);
///   Retrieves the value in the calling thread's TLS slot.
///   @param dwTlsIndex TLS index
///   @return Value on success, \b NULL on failure or if value is \b NULL
///   @note Use \b GetLastError() to distinguish between \b NULL value and error.
///
/// - BOOL TlsSetValue(DWORD dwTlsIndex, LPVOID lpTlsValue);
///   Stores a value in the calling thread's TLS slot.
///   @param dwTlsIndex TLS index
///   @param lpTlsValue Value to store
///   @return \b TRUE on success, \b FALSE on failure
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/
#include <processthreadsapi.h>
/// ==================================================================================
/// Thread Creation Flags (winbase.h subset)
/// ==================================================================================
/// These constants are typically defined in winbase.h but are needed for CreateThread.
/// Defining them here avoids the massive winbase.h dependency.
///
/// Creation Flags:
/// - CREATE_SUSPENDED: Thread is created in a suspended state (use ResumeThread to start)
/// - STACK_SIZE_PARAM_IS_A_RESERVATION: dwStackSize is the reserve size, not commit size
#if !defined(CREATE_SUSPENDED)
#define CREATE_SUSPENDED 0x00000004
#endif
#if !defined(STACK_SIZE_PARAM_IS_A_RESERVATION)
#define STACK_SIZE_PARAM_IS_A_RESERVATION 0x00010000
#endif
/// Thread Exit Code:
/// - STILL_ACTIVE: Thread is still running (returned by GetExitCodeThread)
#if !defined(STILL_ACTIVE)
#define STILL_ACTIVE 0x00000103L
#endif
/// TLS Constants:
/// - TLS_OUT_OF_INDEXES: TlsAlloc failed (no more TLS slots available)
#if !defined(TLS_OUT_OF_INDEXES)
#define TLS_OUT_OF_INDEXES 0xFFFFFFFF
#endif
/// Thread Priority Constants (for GetThreadPriority/SetThreadPriority):
/// These are only defined in winbase.h. The THREAD_BASE_PRIORITY_* constants
/// are in winnt.h, so we can define these safely.
#if !defined(THREAD_PRIORITY_LOWEST)
#define THREAD_PRIORITY_LOWEST THREAD_BASE_PRIORITY_MIN
#endif
#if !defined(THREAD_PRIORITY_BELOW_NORMAL)
#define THREAD_PRIORITY_BELOW_NORMAL (THREAD_PRIORITY_LOWEST + 1)
#endif
#if !defined(THREAD_PRIORITY_NORMAL)
#define THREAD_PRIORITY_NORMAL 0
#endif
#if !defined(THREAD_PRIORITY_ABOVE_NORMAL)
#define THREAD_PRIORITY_ABOVE_NORMAL (THREAD_PRIORITY_HIGHEST - 1)
#endif
#if !defined(THREAD_PRIORITY_HIGHEST)
#define THREAD_PRIORITY_HIGHEST THREAD_BASE_PRIORITY_MAX
#endif
#if !defined(THREAD_PRIORITY_TIME_CRITICAL)
#define THREAD_PRIORITY_TIME_CRITICAL THREAD_BASE_PRIORITY_LOWRT
#endif
#if !defined(THREAD_PRIORITY_IDLE)
#define THREAD_PRIORITY_IDLE THREAD_BASE_PRIORITY_IDLE
#endif
#if !defined(THREAD_PRIORITY_ERROR_RETURN)
#define THREAD_PRIORITY_ERROR_RETURN 0x7FFFFFFF
#endif
#pragma comment(lib, "kernel32.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_thrd__included */
