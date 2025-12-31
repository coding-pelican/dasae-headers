/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    file.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-31 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows file input/output APIs
 * @details Handles file input and output operations. Provides low-level access to
 *          the file system for creating, reading, writing, and managing files and
 *          streams. Supports synchronous and asynchronous (overlapped) I/O via
 *          OVERLAPPED structure.
 *
 * API Overview:
 * - Creation: CreateFile
 * - I/O: ReadFile, WriteFile, FlushFileBuffers
 * - Positioning: SetFilePointer, SetFilePointerEx
 * - Information: GetFileSizeEx, GetFileSize, GetFileAttributes, SetFileAttributes
 * - Management: DeleteFile, MoveFile, CopyFile
 *
 * @note For asynchronous file I/O, you need both file.h and io.h:
 *       - file.h: CreateFile (with FILE_FLAG_OVERLAPPED), ReadFile, WriteFile
 *       - io.h: GetOverlappedResult, CreateIoCompletionPort, GetQueuedCompletionStatus
 *       See "Asynchronous File I/O Example" section below for usage.
 */
#ifndef os_windows_file__included
#define os_windows_file__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// File Management API
/// ==================================================================================
/// Provides functions for file creation, reading, writing, and attribute management.
///
/// Key APIs:
///
/// File Creation and Opening:
/// - HANDLE CreateFile(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
///   Creates or opens a file or I/O device.
///   @param lpFileName Path to the file or device
///   @param dwDesiredAccess Access mode:
///     - \b GENERIC_READ : Read access
///     - \b GENERIC_WRITE : Write access
///     - \b GENERIC_READ | \b GENERIC_WRITE : Read and write access
///   @param dwShareMode Sharing mode:
///     - \b 0 : No sharing
///     - \b FILE_SHARE_READ : Allow read sharing
///     - \b FILE_SHARE_WRITE : Allow write sharing
///     - \b FILE_SHARE_DELETE : Allow delete sharing
///   @param lpSecurityAttributes Security attributes (\b NULL for default)
///   @param dwCreationDisposition How to create/open:
///     - \b CREATE_NEW : Create new file, fail if exists
///     - \b CREATE_ALWAYS : Create new file, overwrite if exists
///     - \b OPEN_EXISTING : Open existing file, fail if not exists
///     - \b OPEN_ALWAYS : Open existing file, create if not exists
///     - \b TRUNCATE_EXISTING : Open and truncate, fail if not exists
///   @param dwFlagsAndAttributes File attributes:
///     - \b FILE_ATTRIBUTE_NORMAL : Normal file
///     - \b FILE_ATTRIBUTE_READONLY : Read-only file
///     - \b FILE_ATTRIBUTE_HIDDEN : Hidden file
///     - \b FILE_FLAG_OVERLAPPED : Enable asynchronous I/O
///     - \b FILE_FLAG_NO_BUFFERING : No buffering
///   @param hTemplateFile Template file handle (\b NULL for none)
///   @return Handle on success, \b INVALID_HANDLE_VALUE on failure
///
/// File I/O:
/// - BOOL ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
///   Reads data from a file.
///   @param hFile Handle to the file
///   @param lpBuffer Buffer to receive the data
///   @param nNumberOfBytesToRead Number of bytes to read
///   @param lpNumberOfBytesRead Receives the number of bytes read (\b NULL for overlapped I/O)
///   @param lpOverlapped OVERLAPPED structure for asynchronous I/O (\b NULL for synchronous)
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
///   Writes data to a file.
///   @param hFile Handle to the file
///   @param lpBuffer Buffer containing data to write
///   @param nNumberOfBytesToWrite Number of bytes to write
///   @param lpNumberOfBytesWritten Receives the number of bytes written (\b NULL for overlapped I/O)
///   @param lpOverlapped OVERLAPPED structure for asynchronous I/O (\b NULL for synchronous)
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL FlushFileBuffers(HANDLE hFile);
///   Flushes the buffers of a specified file and causes all buffered data to be written.
///   @param hFile Handle to the file
///   @return \b TRUE on success, \b FALSE on failure
///
/// File Positioning:
/// - DWORD SetFilePointer(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod);
///   Moves the file pointer of the specified file.
///   @param hFile Handle to the file
///   @param lDistanceToMove Low 32 bits of distance to move
///   @param lpDistanceToMoveHigh Pointer to high 32 bits of distance (\b NULL for 32-bit files)
///   @param dwMoveMethod Starting point:
///     - \b FILE_BEGIN : Start of file
///     - \b FILE_CURRENT : Current position
///     - \b FILE_END : End of file
///   @return New pointer position (low 32 bits)
///
/// - BOOL SetFilePointerEx(HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod);
///   64-bit version of SetFilePointer.
///   @param hFile Handle to the file
///   @param liDistanceToMove Distance to move (use \b .QuadPart member)
///   @param lpNewFilePointer Receives new file pointer position (\b NULL if not needed)
///   @param dwMoveMethod Starting point (same as \b SetFilePointer)
///   @return \b TRUE on success, \b FALSE on failure
///   @note LARGE_INTEGER is a union: { struct { DWORD LowPart; LONG HighPart; }; LONGLONG QuadPart; }
///
/// File Information:
/// - BOOL GetFileSizeEx(HANDLE hFile, PLARGE_INTEGER lpFileSize);
///   Retrieves the size of the specified file.
///   @param hFile Handle to the file
///   @param lpFileSize Receives the file size (use \b .QuadPart member)
///   @return \b TRUE on success, \b FALSE on failure
///
/// - DWORD GetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh);
///   32-bit version of GetFileSizeEx.
///   @param hFile Handle to the file
///   @param lpFileSizeHigh Receives high 32 bits of size (\b NULL for files < 4GB)
///   @return Low 32 bits of file size, 0xFFFFFFFF on failure (check \b GetLastError)
///
/// - DWORD GetFileAttributes(LPCSTR lpFileName);
///   Retrieves file system attributes.
///   @param lpFileName Path to the file or directory
///   @return Attributes on success, \b INVALID_FILE_ATTRIBUTES on failure
///
/// - BOOL SetFileAttributes(LPCSTR lpFileName, DWORD dwFileAttributes);
///   Sets the attributes for a file or directory.
///   @param lpFileName Path to the file or directory
///   @param dwFileAttributes Attributes to set
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL DeleteFile(LPCSTR lpFileName);
///   Deletes an existing file.
///   @param lpFileName Path to the file to delete
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL MoveFile(LPCSTR lpExistingFileName, LPCSTR lpNewFileName);
///   Moves or renames a file or directory.
///   @param lpExistingFileName Path to the existing file
///   @param lpNewFileName New path for the file
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL CopyFile(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists);
///   Copies an existing file to a new file.
///   @param lpExistingFileName Path to the existing file
///   @param lpNewFileName Path for the new file
///   @param bFailIfExists \b TRUE to fail if destination exists, \b FALSE to overwrite
///   @return \b TRUE on success, \b FALSE on failure
///
/// ==================================================================================
/// Asynchronous File I/O Example
/// ==================================================================================
/// To use async file I/O, you need both file.h (for file operations) and io.h
/// (for completion handling). Here's a complete example:
///
/// @code{.c}
/// #include "file.h"   // CreateFile, ReadFile, WriteFile, FILE_FLAG_OVERLAPPED
/// #include "io.h"     // GetOverlappedResult, CreateIoCompletionPort
/// #include "handle.h" // CloseHandle, GetLastError
/// #include "sync.h"   // CreateEvent, WaitForSingleObject, INFINITE
///
/// // Method 1: Using Events (Simple)
/// void asyncReadWithEvent(void) {
///     // Open file with overlapped flag
///     HANDLE hFile = CreateFile("data.bin", GENERIC_READ, 0, NULL,
///                               OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
///     if (hFile == INVALID_HANDLE_VALUE) return;
///
///     // Create event for completion notification
///     OVERLAPPED ov = {0};
///     ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
///
///     // Start async read
///     char buffer[4096];
///     BOOL result = ReadFile(hFile, buffer, sizeof(buffer), NULL, &ov);
///     if (!result && GetLastError() == ERROR_IO_PENDING) {
///         // Wait for completion
///         WaitForSingleObject(ov.hEvent, INFINITE);
///
///         // Get result
///         DWORD bytesRead;
///         GetOverlappedResult(hFile, &ov, &bytesRead, FALSE);
///     }
///
///     CloseHandle(ov.hEvent);
///     CloseHandle(hFile);
/// }
///
/// // Method 2: Using IOCP (High-Performance Servers)
/// void asyncReadWithIOCP(void) {
///     // Create completion port
///     HANDLE iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
///
///     // Open file and associate with IOCP
///     HANDLE hFile = CreateFile("data.bin", GENERIC_READ, 0, NULL,
///                               OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
///     CreateIoCompletionPort(hFile, iocp, (ULONG_PTR)hFile, 0);
///
///     // Start async read
///     OVERLAPPED ov = {0};
///     char buffer[4096];
///     ReadFile(hFile, buffer, sizeof(buffer), NULL, &ov);
///
///     // Wait for completion on IOCP
///     DWORD bytesRead;
///     ULONG_PTR key;
///     LPOVERLAPPED pov;
///     GetQueuedCompletionStatus(iocp, &bytesRead, &key, &pov, INFINITE);
///
///     CloseHandle(hFile);
///     CloseHandle(iocp);
/// }
/// @endcode
///
/// Key Points:
/// - Always open files with \b FILE_FLAG_OVERLAPPED for async I/O
/// - OVERLAPPED.Offset/OffsetHigh specify the file position (no SetFilePointer needed)
/// - Check for \b ERROR_IO_PENDING after ReadFile/WriteFile returns FALSE
/// - Use \b GetOverlappedResult (io.h) to retrieve operation results
/// - For high-performance scenarios, use I/O Completion Ports (IOCP)
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/
#include <fileapi.h>
/// ==================================================================================
/// File Constants (winbase.h subset)
/// ==================================================================================
/// These constants are typically defined in winbase.h but are needed for file operations.
/// Defining them here avoids the massive winbase.h dependency.
///
/// Creation Disposition (dwCreationDisposition for CreateFile):
#if !defined(CREATE_NEW)
#define CREATE_NEW 1
#endif
#if !defined(CREATE_ALWAYS)
#define CREATE_ALWAYS 2
#endif
#if !defined(OPEN_EXISTING)
#define OPEN_EXISTING 3
#endif
#if !defined(OPEN_ALWAYS)
#define OPEN_ALWAYS 4
#endif
#if !defined(TRUNCATE_EXISTING)
#define TRUNCATE_EXISTING 5
#endif
/// File Share Modes (dwShareMode for CreateFile):
#if !defined(FILE_SHARE_READ)
#define FILE_SHARE_READ 0x00000001
#endif
#if !defined(FILE_SHARE_WRITE)
#define FILE_SHARE_WRITE 0x00000002
#endif
#if !defined(FILE_SHARE_DELETE)
#define FILE_SHARE_DELETE 0x00000004
#endif
/// File Attributes (dwFlagsAndAttributes for CreateFile):
#if !defined(FILE_ATTRIBUTE_READONLY)
#define FILE_ATTRIBUTE_READONLY 0x00000001
#endif
#if !defined(FILE_ATTRIBUTE_HIDDEN)
#define FILE_ATTRIBUTE_HIDDEN 0x00000002
#endif
#if !defined(FILE_ATTRIBUTE_SYSTEM)
#define FILE_ATTRIBUTE_SYSTEM 0x00000004
#endif
#if !defined(FILE_ATTRIBUTE_DIRECTORY)
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#endif
#if !defined(FILE_ATTRIBUTE_ARCHIVE)
#define FILE_ATTRIBUTE_ARCHIVE 0x00000020
#endif
#if !defined(FILE_ATTRIBUTE_NORMAL)
#define FILE_ATTRIBUTE_NORMAL 0x00000080
#endif
#if !defined(FILE_ATTRIBUTE_TEMPORARY)
#define FILE_ATTRIBUTE_TEMPORARY 0x00000100
#endif
/// File Flags (dwFlagsAndAttributes for CreateFile):
#if !defined(FILE_FLAG_WRITE_THROUGH)
#define FILE_FLAG_WRITE_THROUGH 0x80000000
#endif
#if !defined(FILE_FLAG_OVERLAPPED)
#define FILE_FLAG_OVERLAPPED 0x40000000
#endif
#if !defined(FILE_FLAG_NO_BUFFERING)
#define FILE_FLAG_NO_BUFFERING 0x20000000
#endif
#if !defined(FILE_FLAG_RANDOM_ACCESS)
#define FILE_FLAG_RANDOM_ACCESS 0x10000000
#endif
#if !defined(FILE_FLAG_SEQUENTIAL_SCAN)
#define FILE_FLAG_SEQUENTIAL_SCAN 0x08000000
#endif
#if !defined(FILE_FLAG_DELETE_ON_CLOSE)
#define FILE_FLAG_DELETE_ON_CLOSE 0x04000000
#endif
#if !defined(FILE_FLAG_BACKUP_SEMANTICS)
#define FILE_FLAG_BACKUP_SEMANTICS 0x02000000
#endif
#if !defined(FILE_FLAG_POSIX_SEMANTICS)
#define FILE_FLAG_POSIX_SEMANTICS 0x01000000
#endif
/// File Seek Methods (dwMoveMethod for SetFilePointer):
#if !defined(FILE_BEGIN)
#define FILE_BEGIN 0
#endif
#if !defined(FILE_CURRENT)
#define FILE_CURRENT 1
#endif
#if !defined(FILE_END)
#define FILE_END 2
#endif
/// Invalid File Attributes (return value from GetFileAttributes on error):
#if !defined(INVALID_FILE_ATTRIBUTES)
#define INVALID_FILE_ATTRIBUTES ((DWORD) - 1)
#endif
/// Invalid Set File Pointer (return value from SetFilePointer on error):
#if !defined(INVALID_SET_FILE_POINTER)
#define INVALID_SET_FILE_POINTER ((DWORD) - 1)
#endif
/// MoveFileEx Flags (dwFlags for MoveFileEx):
#if !defined(MOVEFILE_REPLACE_EXISTING)
#define MOVEFILE_REPLACE_EXISTING 0x00000001
#endif
#if !defined(MOVEFILE_COPY_ALLOWED)
#define MOVEFILE_COPY_ALLOWED 0x00000002
#endif
#if !defined(MOVEFILE_DELAY_UNTIL_REBOOT)
#define MOVEFILE_DELAY_UNTIL_REBOOT 0x00000004
#endif
#if !defined(MOVEFILE_WRITE_THROUGH)
#define MOVEFILE_WRITE_THROUGH 0x00000008
#endif
#if !defined(MOVEFILE_CREATE_HARDLINK)
#define MOVEFILE_CREATE_HARDLINK 0x00000010
#endif
#if !defined(MOVEFILE_FAIL_IF_NOT_TRACKABLE)
#define MOVEFILE_FAIL_IF_NOT_TRACKABLE 0x00000020
#endif
/// LockFileEx Flags (dwFlags for LockFileEx):
#if !defined(LOCKFILE_FAIL_IMMEDIATELY)
#define LOCKFILE_FAIL_IMMEDIATELY 0x00000001
#endif
#if !defined(LOCKFILE_EXCLUSIVE_LOCK)
#define LOCKFILE_EXCLUSIVE_LOCK 0x00000002
#endif
#pragma comment(lib, "kernel32.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_file__included */
