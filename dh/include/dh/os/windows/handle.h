/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    handle.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-31 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows handle management and error reporting APIs
 * @details Manages kernel object handles and system error reporting. Provides
 *          mechanisms to close abstract handles and retrieve extended error
 *          information for failed API operations. Essential for resource cleanup
 *          and debugging.
 *
 * API Overview:
 * - Handle Manipulation: CloseHandle, DuplicateHandle, GetHandleInformation, SetHandleInformation
 * - Error Handling: GetLastError, SetLastError, FormatMessage, RaiseException
 */
#ifndef os_windows_handle__included
#define os_windows_handle__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// Error Handling API
/// ==================================================================================
/// Provides functions to retrieve error codes and error messages from the system.
///
/// Key APIs:
///
/// - DWORD GetLastError(void);
///   Retrieves the calling thread's last-error code value. The last-error code is maintained
///   per-thread. Call immediately after a failed API call.
///   @return The last-error code value. Common codes:
///     - \b ERROR_SUCCESS (0): Operation succeeded
///     - \b ERROR_FILE_NOT_FOUND (2): File not found
///     - \b ERROR_ACCESS_DENIED (5): Access denied
///     - \b ERROR_INVALID_HANDLE (6): Invalid handle
///     - \b ERROR_NOT_ENOUGH_MEMORY (8): Insufficient memory
///     - \b ERROR_INVALID_PARAMETER (87): Invalid parameter
///     - \b ERROR_ALREADY_EXISTS (183): File already exists
///
/// - VOID SetLastError(DWORD dwErrCode);
///   Sets the last-error code for the calling thread. Useful for custom error reporting.
///   @param dwErrCode Error code to set
///
/// - DWORD FormatMessage(DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, LPSTR lpBuffer, DWORD nSize, va_list* Arguments);
///   Formats a message string from a message definition.
///   @param dwFlags Format options:
///     - \b FORMAT_MESSAGE_ALLOCATE_BUFFER : Allocate buffer automatically
///     - \b FORMAT_MESSAGE_FROM_SYSTEM : Get message from system
///     - \b FORMAT_MESSAGE_IGNORE_INSERTS : Ignore insert sequences
///   @param lpSource Message source (\b NULL for system messages)
///   @param dwMessageId Message identifier (error code from \b GetLastError)
///   @param dwLanguageId Language identifier (0 for default)
///   @param lpBuffer Buffer to receive formatted message
///   @param nSize Size of buffer (or 0 if \b FORMAT_MESSAGE_ALLOCATE_BUFFER)
///   @param Arguments Optional array of message insert values
///   @return Number of characters stored (excluding null terminator), 0 on failure
///
/// - VOID RaiseException(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, const ULONG_PTR* lpArguments);
///   Raises an exception in the calling thread. The exception can be handled by structured
///   exception handling (__try/__except) or C++ exception handling.
///   @param dwExceptionCode Exception code (e.g., \b EXCEPTION_ACCESS_VIOLATION)
///   @param dwExceptionFlags Exception flags (0 for continuable)
///   @param nNumberOfArguments Number of arguments
///   @param lpArguments Array of arguments
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/debug/error-handling-functions
#include <errhandlingapi.h>
/// ==================================================================================
/// Handle Manipulation API
/// ==================================================================================
/// Manages open handles to kernel objects (files, threads, mutexes, etc.).
///
/// Key APIs:
///
/// - BOOL CloseHandle(HANDLE hObject);
///   Closes an open object handle. This decrements the object's handle count and
///   may cause the object to be destroyed if the count reaches zero.
///   @param hObject Handle to close
///   @return \b TRUE on success, \b FALSE on failure
///   @note Always close handles when done to prevent resource leaks.
///
/// - BOOL DuplicateHandle(HANDLE hSourceProcessHandle, HANDLE hSourceHandle, HANDLE hTargetProcessHandle, LPHANDLE lpTargetHandle, DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwOptions);
///   Duplicates an object handle so it can be used by another process or with different access rights.
///   @param hSourceProcessHandle Handle to process containing source handle
///   @param hSourceHandle Handle to duplicate
///   @param hTargetProcessHandle Handle to process that will receive duplicate (\b NULL for current)
///   @param lpTargetHandle Receives the duplicate handle
///   @param dwDesiredAccess Desired access rights (0 to duplicate with same access)
///   @param bInheritHandle Whether handle is inheritable
///   @param dwOptions Options:
///     - \b DUPLICATE_CLOSE_SOURCE : Close source handle after duplication
///     - \b DUPLICATE_SAME_ACCESS : Duplicate with same access rights
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL GetHandleInformation(HANDLE hObject, LPDWORD lpdwFlags);
///   Retrieves certain properties of an object handle.
///   @param hObject Handle to query
///   @param lpdwFlags Receives flags:
///     - \b HANDLE_FLAG_INHERIT : Handle is inheritable
///     - \b HANDLE_FLAG_PROTECT_FROM_CLOSE : Handle is protected from close
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL SetHandleInformation(HANDLE hObject, DWORD dwMask, DWORD dwFlags);
///   Sets certain properties of an object handle.
///   @param hObject Handle to modify
///   @param dwMask Mask of flags to change
///   @param dwFlags New flag values
///   @return \b TRUE on success, \b FALSE on failure
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/handleapi/
#include <handleapi.h>
/// ==================================================================================
/// Handle Constants (winbase.h subset)
/// ==================================================================================
/// These constants are typically defined in winbase.h but are needed for handle operations.
/// Defining them here avoids the massive winbase.h dependency.
///
/// DuplicateHandle Options:
#if !defined(DUPLICATE_CLOSE_SOURCE)
#define DUPLICATE_CLOSE_SOURCE 0x00000001
#endif
#if !defined(DUPLICATE_SAME_ACCESS)
#define DUPLICATE_SAME_ACCESS 0x00000002
#endif
/// Handle Flags (GetHandleInformation/SetHandleInformation):
#if !defined(HANDLE_FLAG_INHERIT)
#define HANDLE_FLAG_INHERIT 0x00000001
#endif
#if !defined(HANDLE_FLAG_PROTECT_FROM_CLOSE)
#define HANDLE_FLAG_PROTECT_FROM_CLOSE 0x00000002
#endif
/// FormatMessage Flags:
#if !defined(FORMAT_MESSAGE_ALLOCATE_BUFFER)
#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
#endif
#if !defined(FORMAT_MESSAGE_IGNORE_INSERTS)
#define FORMAT_MESSAGE_IGNORE_INSERTS 0x00000200
#endif
#if !defined(FORMAT_MESSAGE_FROM_STRING)
#define FORMAT_MESSAGE_FROM_STRING 0x00000400
#endif
#if !defined(FORMAT_MESSAGE_FROM_HMODULE)
#define FORMAT_MESSAGE_FROM_HMODULE 0x00000800
#endif
#if !defined(FORMAT_MESSAGE_FROM_SYSTEM)
#define FORMAT_MESSAGE_FROM_SYSTEM 0x00001000
#endif
#if !defined(FORMAT_MESSAGE_ARGUMENT_ARRAY)
#define FORMAT_MESSAGE_ARGUMENT_ARRAY 0x00002000
#endif
#if !defined(FORMAT_MESSAGE_MAX_WIDTH_MASK)
#define FORMAT_MESSAGE_MAX_WIDTH_MASK 0x000000FF
#endif
/// Invalid Handle Value:
/// Standard return value indicating an invalid handle (e.g., from CreateFile on failure).
#if !defined(INVALID_HANDLE_VALUE)
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR) - 1)
#endif
/// Common Error Codes (from winerror.h, for convenience):
/// These are typically defined in winerror.h but included here for reference.
#if !defined(ERROR_SUCCESS)
#define ERROR_SUCCESS 0L
#endif
#if !defined(ERROR_INVALID_FUNCTION)
#define ERROR_INVALID_FUNCTION 1L
#endif
#if !defined(ERROR_FILE_NOT_FOUND)
#define ERROR_FILE_NOT_FOUND 2L
#endif
#if !defined(ERROR_PATH_NOT_FOUND)
#define ERROR_PATH_NOT_FOUND 3L
#endif
#if !defined(ERROR_TOO_MANY_OPEN_FILES)
#define ERROR_TOO_MANY_OPEN_FILES 4L
#endif
#if !defined(ERROR_ACCESS_DENIED)
#define ERROR_ACCESS_DENIED 5L
#endif
#if !defined(ERROR_INVALID_HANDLE)
#define ERROR_INVALID_HANDLE 6L
#endif
#if !defined(ERROR_NOT_ENOUGH_MEMORY)
#define ERROR_NOT_ENOUGH_MEMORY 8L
#endif
#if !defined(ERROR_INVALID_PARAMETER)
#define ERROR_INVALID_PARAMETER 87L
#endif
#if !defined(ERROR_INSUFFICIENT_BUFFER)
#define ERROR_INSUFFICIENT_BUFFER 122L
#endif
#if !defined(ERROR_ALREADY_EXISTS)
#define ERROR_ALREADY_EXISTS 183L
#endif
#if !defined(ERROR_NO_MORE_ITEMS)
#define ERROR_NO_MORE_ITEMS 259L
#endif
#if !defined(ERROR_TIMEOUT)
#define ERROR_TIMEOUT 1460L
#endif
#pragma comment(lib, "kernel32.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_handle__included */
