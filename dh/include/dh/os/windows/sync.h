/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    sync.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-31 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows synchronization APIs
 * @details Provides synchronization primitives to coordinate execution among
 *          threads and processes. Includes support for atomic access and
 *          waitable objects.
 *
 * API Overview:
 * - Wait Functions: WaitForSingleObject, WaitForMultipleObjects, Sleep, SleepEx
 * - Critical Sections: InitializeCriticalSection, EnterCriticalSection, TryEnterCriticalSection,
 *   LeaveCriticalSection, DeleteCriticalSection
 * - Mutexes: CreateMutex, OpenMutex, ReleaseMutex
 * - Events: CreateEvent, OpenEvent, SetEvent, ResetEvent, PulseEvent [Deprecated]
 * - Semaphores: CreateSemaphore, OpenSemaphore, ReleaseSemaphore
 * - SRW Locks [Min: Vista]: InitializeSRWLock, AcquireSRWLockExclusive, ReleaseSRWLockExclusive,
 *   AcquireSRWLockShared, ReleaseSRWLockShared, TryAcquireSRWLockExclusive, TryAcquireSRWLockShared
 * - Barriers [Min: Win8]: InitializeSynchronizationBarrier, EnterSynchronizationBarrier,
 *   DeleteSynchronizationBarrier
 */
#ifndef os_windows_sync__included
#define os_windows_sync__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// Synchronization API
/// ==================================================================================
/// Provides primitives for coordinating execution among threads and processes.
///
/// Key APIs:
///
/// Wait Functions:
/// - DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
///   Waits until the specified object is in the signaled state or the time-out interval elapses.
///   @param hHandle Handle to object (mutex, event, semaphore, thread, process, etc.)
///   @param dwMilliseconds Timeout in milliseconds (\b INFINITE for no timeout, 0 for test only)
///   @return \b WAIT_OBJECT_0 (signaled), \b WAIT_TIMEOUT (timeout), \b WAIT_FAILED (error)
///
/// - DWORD WaitForMultipleObjects(DWORD nCount, const HANDLE* lpHandles, BOOL bWaitAll, DWORD dwMilliseconds);
///   Waits until one or all of the specified objects are in the signaled state.
///   @param nCount Number of handles (1 to \b MAXIMUM_WAIT_OBJECTS)
///   @param lpHandles Array of object handles
///   @param bWaitAll \b TRUE to wait for all, \b FALSE to wait for any
///   @param dwMilliseconds Timeout in milliseconds
///   @return \b WAIT_OBJECT_0 to \b WAIT_OBJECT_0+nCount-1 (which object), \b WAIT_TIMEOUT, \b WAIT_FAILED
///
/// Sleep Functions:
/// - VOID Sleep(DWORD dwMilliseconds);
///   Suspends the execution of the current thread for the specified interval.
///   @param dwMilliseconds Sleep time in milliseconds (0 to yield to other threads)
///   @note \b Sleep(0) yields to other threads of equal priority.
///
/// - DWORD SleepEx(DWORD dwMilliseconds, BOOL bAlertable);
///   Suspends the current thread until I/O completion callback, an APC is queued, or timeout.
///   @param dwMilliseconds Sleep time in milliseconds (\b INFINITE for no timeout)
///   @param bAlertable \b TRUE to return early for APCs, \b FALSE otherwise
///   @return 0 if timeout, \b WAIT_IO_COMPLETION if alertable I/O completed
///
/// Critical Sections (Fast, Process-Local):
/// - VOID InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
///   Initializes a critical section object.
///   @param lpCriticalSection Pointer to CRITICAL_SECTION structure
///   @note Must be called before use.
///
/// - VOID EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
///   Waits for ownership of the critical section. Blocks if another thread owns it.
///   @param lpCriticalSection Pointer to CRITICAL_SECTION structure
///
/// - BOOL TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
///   Attempts to enter a critical section without blocking.
///   @param lpCriticalSection Pointer to CRITICAL_SECTION structure
///   @return \b TRUE if entered, \b FALSE otherwise
///
/// - VOID LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
///   Releases ownership of the critical section.
///   @param lpCriticalSection Pointer to CRITICAL_SECTION structure
///   @note Must be called once per \b EnterCriticalSection.
///
/// - VOID DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
///   Deletes a critical section object.
///   @param lpCriticalSection Pointer to CRITICAL_SECTION structure
///   @note Must be called when done.
///
/// Mutexes (Named, Cross-Process):
/// - HANDLE CreateMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName);
///   Creates or opens a mutex object.
///   @param lpMutexAttributes Security attributes (\b NULL for default)
///   @param bInitialOwner \b TRUE if calling thread should own mutex initially
///   @param lpName Name of mutex (\b NULL for unnamed, or name for cross-process sharing)
///   @return Handle on success, \b NULL on failure
///
/// - HANDLE OpenMutex(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCSTR lpName);
///   Opens an existing named mutex object.
///   @param dwDesiredAccess Desired access rights
///   @param bInheritHandle Whether handle is inheritable
///   @param lpName Name of the mutex
///   @return Handle on success, \b NULL on failure
///
/// - BOOL ReleaseMutex(HANDLE hMutex);
///   Releases ownership of the specified mutex object.
///   @param hMutex Handle to the mutex
///   @return \b TRUE on success, \b FALSE on failure
///
/// Events (Signaling):
/// - HANDLE CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName);
///   Creates or opens an event object.
///   @param lpEventAttributes Security attributes (\b NULL for default)
///   @param bManualReset \b TRUE for manual-reset event, \b FALSE for auto-reset event
///   @param bInitialState \b TRUE if initially signaled
///   @param lpName Name of event (\b NULL for unnamed)
///   @return Handle on success, \b NULL on failure
///
/// - HANDLE OpenEvent(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCSTR lpName);
///   Opens an existing named event object.
///   @param dwDesiredAccess Desired access rights
///   @param bInheritHandle Whether handle is inheritable
///   @param lpName Name of the event
///   @return Handle on success, \b NULL on failure
///
/// - BOOL SetEvent(HANDLE hEvent);
///   Sets the specified event object to the signaled state.
///   @param hEvent Handle to the event
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL ResetEvent(HANDLE hEvent);
///   Sets the specified event object to the nonsignaled state.
///   @param hEvent Handle to the event
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL PulseEvent(HANDLE hEvent);  [Deprecated]
///   Sets the event to signaled, releases waiting threads, then resets to nonsignaled.
///   @param hEvent Handle to the event
///   @return \b TRUE on success, \b FALSE on failure
///   @deprecated This function is unreliable. Use \b SetEvent with manual-reset events instead.
///
/// Semaphores (Counting):
/// - HANDLE CreateSemaphore(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCSTR lpName);
///   Creates or opens a semaphore object.
///   @param lpSemaphoreAttributes Security attributes (\b NULL for default)
///   @param lInitialCount Initial count (must be >= 0 and <= lMaximumCount)
///   @param lMaximumCount Maximum count
///   @param lpName Name of semaphore (\b NULL for unnamed)
///   @return Handle on success, \b NULL on failure
///
/// - HANDLE OpenSemaphore(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCSTR lpName);
///   Opens an existing named semaphore object.
///   @param dwDesiredAccess Desired access rights
///   @param bInheritHandle Whether handle is inheritable
///   @param lpName Name of the semaphore
///   @return Handle on success, \b NULL on failure
///
/// - BOOL ReleaseSemaphore(HANDLE hSemaphore, LONG lReleaseCount, LPLONG lpPreviousCount);
///   Increases the count of the semaphore by the specified amount.
///   @param hSemaphore Handle to semaphore
///   @param lReleaseCount Amount to increase count (must be > 0)
///   @param lpPreviousCount Receives previous count (can be \b NULL)
///   @return \b TRUE on success, \b FALSE on failure
///
/// Slim Reader/Writer Locks [Min: Vista]:
/// @note SRW Locks require Windows Vista or later. Lightweight alternative to CRITICAL_SECTION
/// for reader/writer scenarios. No cleanup required (no DeleteSRWLock function).
///
/// - VOID InitializeSRWLock(PSRWLOCK SRWLock);
///   Initializes a slim reader/writer lock. Must be called before use.
///   @param SRWLock Pointer to SRWLOCK structure
///
/// - VOID AcquireSRWLockExclusive(PSRWLOCK SRWLock);
///   Acquires the lock in exclusive mode (for writing). Blocks until acquired.
///   @param SRWLock Pointer to SRWLOCK structure
///
/// - VOID ReleaseSRWLockExclusive(PSRWLOCK SRWLock);
///   Releases the lock that was acquired in exclusive mode.
///   @param SRWLock Pointer to SRWLOCK structure
///
/// - VOID AcquireSRWLockShared(PSRWLOCK SRWLock);
///   Acquires the lock in shared mode (for reading). Multiple readers allowed.
///   @param SRWLock Pointer to SRWLOCK structure
///
/// - VOID ReleaseSRWLockShared(PSRWLOCK SRWLock);
///   Releases the lock that was acquired in shared mode.
///   @param SRWLock Pointer to SRWLOCK structure
///
/// - BOOL TryAcquireSRWLockExclusive(PSRWLOCK SRWLock);  [Min: Win7]
///   Attempts to acquire the lock in exclusive mode without blocking.
///   @param SRWLock Pointer to SRWLOCK structure
///   @return \b TRUE if acquired, \b FALSE otherwise
///   @note Requires Windows 7 or later.
///
/// - BOOL TryAcquireSRWLockShared(PSRWLOCK SRWLock);  [Min: Win7]
///   Attempts to acquire the lock in shared mode without blocking.
///   @param SRWLock Pointer to SRWLOCK structure
///   @return \b TRUE if acquired, \b FALSE otherwise
///   @note Requires Windows 7 or later.
///
/// Synchronization Barriers [Min: Win8]:
/// @note Synchronization Barriers require Windows 8 or later.
///
/// - BOOL InitializeSynchronizationBarrier(LPSYNCHRONIZATION_BARRIER lpBarrier, LONG lTotalThreads, LONG lSpinCount);
///   Initializes a synchronization barrier.
///   @param lpBarrier Pointer to barrier structure
///   @param lTotalThreads Number of threads that must reach barrier
///   @param lSpinCount Spin count (0 for default)
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL EnterSynchronizationBarrier(LPSYNCHRONIZATION_BARRIER lpBarrier, DWORD dwFlags);
///   Enters the synchronization barrier. Blocks until all threads have entered.
///   @param lpBarrier Pointer to barrier structure
///   @param dwFlags Reserved, must be 0
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL DeleteSynchronizationBarrier(LPSYNCHRONIZATION_BARRIER lpBarrier);
///   Deletes a synchronization barrier.
///   @param lpBarrier Pointer to barrier structure
///   @return \b TRUE on success, \b FALSE on failure
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/synchapi/
#include <synchapi.h>
/// ==================================================================================
/// Wait Constants (winbase.h subset)
/// ==================================================================================
/// These constants are typically defined in winbase.h but are needed for wait functions.
/// Defining them here avoids the massive winbase.h dependency.
///
/// Wait Result Codes:
/// - WAIT_OBJECT_0: Object is signaled
/// - WAIT_ABANDONED: Mutex was abandoned (owner terminated without releasing)
/// - WAIT_TIMEOUT: Timeout elapsed
/// - WAIT_FAILED: Function failed (call GetLastError)
///
/// Timeout Value:
/// - INFINITE: Wait forever (no timeout)
#if !defined(INFINITE)
#define INFINITE 0xFFFFFFFF
#endif
#if !defined(WAIT_OBJECT_0)
#define WAIT_OBJECT_0 0x00000000L
#endif
#if !defined(WAIT_ABANDONED)
#define WAIT_ABANDONED 0x00000080L
#endif
#if !defined(WAIT_ABANDONED_0)
#define WAIT_ABANDONED_0 0x00000080L
#endif
#if !defined(WAIT_TIMEOUT)
#define WAIT_TIMEOUT 0x00000102L
#endif
#if !defined(WAIT_FAILED)
#define WAIT_FAILED 0xFFFFFFFF
#endif
#if !defined(WAIT_IO_COMPLETION)
#define WAIT_IO_COMPLETION 0x000000C0L
#endif
/// ==================================================================================
/// Waitable Timer Constants (winbase.h/winnt.h subset)
/// ==================================================================================
/// Timer creation flags for CreateWaitableTimerEx.
///
/// - CREATE_WAITABLE_TIMER_MANUAL_RESET: Manual-reset timer (stays signaled until reset)
/// - CREATE_WAITABLE_TIMER_HIGH_RESOLUTION: High-resolution timer (Win10+, ~0.5ms precision)
#if !defined(CREATE_WAITABLE_TIMER_MANUAL_RESET)
#define CREATE_WAITABLE_TIMER_MANUAL_RESET 0x00000001
#endif
#if !defined(CREATE_WAITABLE_TIMER_HIGH_RESOLUTION)
#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
#endif
/// Timer access rights (from winnt.h, but may not be available in all SDK versions)
#if !defined(TIMER_ALL_ACCESS)
#define TIMER_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x3)
#endif
#if !defined(TIMER_QUERY_STATE)
#define TIMER_QUERY_STATE 0x0001
#endif
#if !defined(TIMER_MODIFY_STATE)
#define TIMER_MODIFY_STATE 0x0002
#endif
/// Synchronization Barrier Flags (dwFlags for EnterSynchronizationBarrier):
/// These require Windows 8 or later but may be missing in older SDK versions.
#if !defined(SYNCHRONIZATION_BARRIER_FLAGS_SPIN_ONLY)
#define SYNCHRONIZATION_BARRIER_FLAGS_SPIN_ONLY 0x01
#endif
#if !defined(SYNCHRONIZATION_BARRIER_FLAGS_BLOCK_ONLY)
#define SYNCHRONIZATION_BARRIER_FLAGS_BLOCK_ONLY 0x02
#endif
#if !defined(SYNCHRONIZATION_BARRIER_FLAGS_NO_DELETE)
#define SYNCHRONIZATION_BARRIER_FLAGS_NO_DELETE 0x04
#endif
/// Condition Variable Flags:
#if !defined(CONDITION_VARIABLE_LOCKMODE_SHARED)
#define CONDITION_VARIABLE_LOCKMODE_SHARED 0x1
#endif
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "synchronization.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_sync__included */
