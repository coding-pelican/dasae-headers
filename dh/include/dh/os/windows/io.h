/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    io.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-31 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows I/O completion and device control APIs
 * @details Provides functions for I/O Completion Ports (IOCP), asynchronous I/O,
 *          and device control. Essential for high-performance server applications.
 *
 * API Overview:
 * - Completion Ports: CreateIoCompletionPort, GetQueuedCompletionStatus,
 *   GetQueuedCompletionStatusEx [Min: Vista], PostQueuedCompletionStatus
 * - Async I/O: GetOverlappedResult, GetOverlappedResultEx [Min: Win8],
 *   CancelIo, CancelIoEx [Min: Vista], CancelSynchronousIo [Min: Vista]
 * - Device Control: DeviceIoControl
 *
 * @note For asynchronous file I/O, this header provides the completion handling
 *       functions. You also need:
 *       - file.h: CreateFile (with FILE_FLAG_OVERLAPPED), ReadFile, WriteFile
 *       - sync.h: CreateEvent, WaitForSingleObject (for event-based completion)
 *       See file.h for complete async file I/O examples.
 *
 * @see file.h for CreateFile, ReadFile, WriteFile and async I/O examples
 * @see sync.h for event creation and wait functions
 */
#ifndef os_windows_io__included
#define os_windows_io__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// I/O Completion Ports API
/// ==================================================================================
/// Provides high-performance asynchronous I/O using completion ports. IOCP is the
/// foundation for scalable Windows server applications.
///
/// Key APIs:
///
/// Completion Port Management:
/// - HANDLE CreateIoCompletionPort(HANDLE FileHandle, HANDLE ExistingCompletionPort, ULONG_PTR CompletionKey, DWORD NumberOfConcurrentThreads);
///   Creates an I/O completion port or associates a handle with an existing port.
///   @param FileHandle Handle to associate (\b INVALID_HANDLE_VALUE to create new port)
///   @param ExistingCompletionPort Existing port to associate with (\b NULL for new port)
///   @param CompletionKey Per-handle key returned with completions
///   @param NumberOfConcurrentThreads Max threads processing completions (0 = num processors)
///   @return Handle to completion port, \b NULL on failure
///   @code{.c}
///     // Create new completion port
///     HANDLE iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
///     // Associate file handle with port
///     CreateIoCompletionPort(hFile, iocp, (ULONG_PTR)context, 0);
///   @endcode
///
/// - BOOL GetQueuedCompletionStatus(HANDLE CompletionPort, LPDWORD lpNumberOfBytesTransferred, PULONG_PTR lpCompletionKey, LPOVERLAPPED* lpOverlapped, DWORD dwMilliseconds);
///   Dequeues a completion packet from the specified I/O completion port.
///   @param CompletionPort Handle to the completion port
///   @param lpNumberOfBytesTransferred Receives bytes transferred
///   @param lpCompletionKey Receives completion key
///   @param lpOverlapped Receives pointer to OVERLAPPED structure
///   @param dwMilliseconds Timeout (\b INFINITE for no timeout)
///   @return \b TRUE on success, \b FALSE on failure or timeout
///   @note If returns \b FALSE with lpOverlapped != \b NULL, operation failed.
///   If returns \b FALSE with lpOverlapped == \b NULL, timeout or port error.
///
/// - BOOL GetQueuedCompletionStatusEx(HANDLE CompletionPort, LPOVERLAPPED_ENTRY lpCompletionPortEntries, ULONG ulCount, PULONG ulNumEntriesRemoved, DWORD dwMilliseconds, BOOL fAlertable);  [Min: Vista]
///   Dequeues multiple completion packets from the specified port.
///   @param CompletionPort Handle to the completion port
///   @param lpCompletionPortEntries Array to receive completion entries
///   @param ulCount Maximum number of entries to dequeue
///   @param ulNumEntriesRemoved Receives number of entries dequeued
///   @param dwMilliseconds Timeout
///   @param fAlertable Whether to return for APCs
///   @return \b TRUE on success, \b FALSE on failure
///   @note More efficient than calling GetQueuedCompletionStatus multiple times.
///
/// - BOOL PostQueuedCompletionStatus(HANDLE CompletionPort, DWORD dwNumberOfBytesTransferred, ULONG_PTR dwCompletionKey, LPOVERLAPPED lpOverlapped);
///   Posts a completion packet to an I/O completion port.
///   @param CompletionPort Handle to the completion port
///   @param dwNumberOfBytesTransferred Value to return as bytes transferred
///   @param dwCompletionKey Value to return as completion key
///   @param lpOverlapped Value to return as overlapped pointer
///   @return \b TRUE on success, \b FALSE on failure
///   @note Useful for signaling worker threads to exit.
///
/// Asynchronous I/O Results:
/// - BOOL GetOverlappedResult(HANDLE hFile, LPOVERLAPPED lpOverlapped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait);
///   Retrieves the result of an overlapped operation.
///   @param hFile Handle to the file, pipe, or device
///   @param lpOverlapped Pointer to OVERLAPPED structure from the operation
///   @param lpNumberOfBytesTransferred Receives bytes transferred
///   @param bWait \b TRUE to wait for completion, \b FALSE to return immediately
///   @return \b TRUE on success, \b FALSE on failure or pending
///
/// - BOOL GetOverlappedResultEx(HANDLE hFile, LPOVERLAPPED lpOverlapped, LPDWORD lpNumberOfBytesTransferred, DWORD dwMilliseconds, BOOL bAlertable);  [Min: Win8]
///   Retrieves the result with timeout support.
///   @param hFile Handle to the file, pipe, or device
///   @param lpOverlapped Pointer to OVERLAPPED structure
///   @param lpNumberOfBytesTransferred Receives bytes transferred
///   @param dwMilliseconds Timeout in milliseconds
///   @param bAlertable Whether to return for APCs
///   @return \b TRUE on success, \b FALSE on failure or timeout
///
/// I/O Cancellation:
/// - BOOL CancelIo(HANDLE hFile);
///   Cancels all pending I/O operations issued by the calling thread.
///   @param hFile Handle to the file
///   @return \b TRUE on success, \b FALSE on failure
///   @note Only cancels operations from the calling thread.
///
/// - BOOL CancelIoEx(HANDLE hFile, LPOVERLAPPED lpOverlapped);  [Min: Vista]
///   Cancels pending I/O operations for a specific overlapped structure.
///   @param hFile Handle to the file
///   @param lpOverlapped Overlapped structure to cancel (\b NULL to cancel all)
///   @return \b TRUE on success, \b FALSE on failure
///   @note Preferred over \b CancelIo for better control.
///
/// - BOOL CancelSynchronousIo(HANDLE hThread);  [Min: Vista]
///   Cancels pending synchronous I/O operations for a thread.
///   @param hThread Handle to the thread
///   @return \b TRUE on success, \b FALSE on failure
///
/// ==================================================================================
/// Async File I/O with IOCP Example
/// ==================================================================================
/// IOCP enables scalable asynchronous I/O for servers handling thousands of
/// concurrent operations. This example shows a simple IOCP worker pattern:
///
/// @code{.c}
/// #include "file.h"   // CreateFile, ReadFile, FILE_FLAG_OVERLAPPED
/// #include "io.h"     // CreateIoCompletionPort, GetQueuedCompletionStatus
/// #include "thrd.h"   // CreateThread
/// #include "handle.h" // CloseHandle
///
/// typedef struct {
///     OVERLAPPED ov;      // Must be first member
///     char buffer[4096];
///     HANDLE hFile;
/// } IoContext;
///
/// HANDLE g_iocp;
///
/// // Worker thread - processes completed I/O operations
/// DWORD WINAPI WorkerThread(LPVOID param) {
///     DWORD bytesTransferred;
///     ULONG_PTR completionKey;
///     LPOVERLAPPED pOverlapped;
///
///     while (TRUE) {
///         BOOL result = GetQueuedCompletionStatus(
///             g_iocp, &bytesTransferred, &completionKey,
///             &pOverlapped, INFINITE);
///
///         if (!result && pOverlapped == NULL) break; // Port closed
///         if (completionKey == 0) break; // Exit signal
///
///         // Get our context structure
///         IoContext* ctx = (IoContext*)pOverlapped;
///
///         // Process completed data
///         ProcessData(ctx->buffer, bytesTransferred);
///
///         // Start next read or cleanup
///         free(ctx);
///     }
///     return 0;
/// }
///
/// void StartAsyncRead(HANDLE hFile) {
///     IoContext* ctx = calloc(1, sizeof(IoContext));
///     ctx->hFile = hFile;
///     ctx->ov.Offset = 0;  // Read from beginning
///
///     // Start async read - completion goes to IOCP
///     ReadFile(hFile, ctx->buffer, sizeof(ctx->buffer), NULL, &ctx->ov);
/// }
///
/// int main(void) {
///     // Create IOCP with worker threads = number of processors
///     g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
///
///     // Create worker threads
///     HANDLE worker = CreateThread(NULL, 0, WorkerThread, NULL, 0, NULL);
///
///     // Open file and associate with IOCP
///     HANDLE hFile = CreateFile("data.bin", GENERIC_READ, 0, NULL,
///                               OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
///     CreateIoCompletionPort(hFile, g_iocp, (ULONG_PTR)hFile, 0);
///
///     // Start async operations
///     StartAsyncRead(hFile);
///
///     // Cleanup: post exit signal and wait for worker
///     PostQueuedCompletionStatus(g_iocp, 0, 0, NULL);
///     WaitForSingleObject(worker, INFINITE);
///
///     CloseHandle(hFile);
///     CloseHandle(g_iocp);
///     return 0;
/// }
/// @endcode
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/ioapiset/
#include <ioapiset.h>
/// ==================================================================================
/// Device I/O Control API
/// ==================================================================================
/// Provides direct device control capabilities.
///
/// Key APIs:
///
/// - BOOL DeviceIoControl(HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped);
///   Sends a control code to a device driver.
///   @param hDevice Handle to the device
///   @param dwIoControlCode Control code (IOCTL_*)
///   @param lpInBuffer Input buffer
///   @param nInBufferSize Input buffer size
///   @param lpOutBuffer Output buffer
///   @param nOutBufferSize Output buffer size
///   @param lpBytesReturned Receives bytes returned
///   @param lpOverlapped Overlapped structure for async (\b NULL for sync)
///   @return \b TRUE on success, \b FALSE on failure
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/ioapiset/nf-ioapiset-deviceiocontrol
#pragma comment(lib, "kernel32.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_io__included */
