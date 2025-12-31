/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    base.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-13 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows base definitions and fundamental types
 * @details Provides the fundamental data types, constants, and macros that form
 *          the foundation of the Windows API. This header acts as the minimal
 *          entry point for type definitions (such as HANDLE, DWORD, BOOL) without
 *          including the entire Windows SDK.
 *
 * API Overview:
 * - Types: BOOL, BYTE, WORD, DWORD, LONG, HANDLE, LPVOID, LPCSTR, LPSTR
 * - Structures: CRITICAL_SECTION, SECURITY_ATTRIBUTES, FILETIME, SYSTEMTIME, OVERLAPPED
 * - Platform Types: INT_PTR, UINT_PTR, SIZE_T, SSIZE_T, DWORD_PTR
 * - Error Codes: ERROR_SUCCESS, ERROR_FILE_NOT_FOUND, E_FAIL, HRESULT_FROM_WIN32
 */
#ifndef os_windows_base__included
#define os_windows_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
/// ==================================================================================
/// Build Configuration
/// ==================================================================================
/// Provides Windows target version configuration and build options.
/// Defines _WIN32_WINNT, NTDDI_VERSION, WIN32_LEAN_AND_MEAN, NOMINMAX, and UNICODE.
/// See cfg.h for detailed version constants and configuration options.
#include "cfg.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// Minimal Windows Definitions
/// ==================================================================================
/// Defines the most fundamental Windows data types used throughout the API.
///
/// Key Types:
/// - BOOL: Boolean type (int, TRUE=1, FALSE=0)
/// - BYTE: 8-bit unsigned integer (unsigned char)
/// - WORD: 16-bit unsigned integer (unsigned short)
/// - DWORD: 32-bit unsigned integer (unsigned long)
/// - LONG: 32-bit signed integer (long)
/// - FLOAT: 32-bit floating point (float)
/// - UINT: Unsigned integer (unsigned int)
/// - LPVOID: Pointer to void (void*)
/// - LPCVOID: Pointer to constant void (const void*)
/// - PUCHAR, PUSHORT, PULONG, PFLOAT: Pointers to basic types
///
/// Key Constants:
/// - TRUE: Boolean true (1)
/// - FALSE: Boolean false (0)
/// - MAX_PATH: Maximum path length (260)
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/minwindef/
#include <minwindef.h>
/// ==================================================================================
/// API Set Infrastructure
/// ==================================================================================
/// Provides the API set contract mechanism for Windows API versioning and
/// compatibility. These headers define the infrastructure used by Windows
/// to map API sets to their implementing DLLs.
///
/// - apiset.h: Core API set definitions and macros
/// - apisetcconv.h: Calling convention definitions for API sets
///
/// @note These are internal infrastructure headers; their types are rarely
/// used directly in application code but are required for proper API resolution.
#include <apiset.h>
#include <apisetcconv.h>
/// ==================================================================================
/// Platform-Dependent Type Safety
/// ==================================================================================
/// Defines types for pointer precision to ensure safety across 32-bit (x86) and
/// 64-bit (x64/ARM64) architectures.
///
/// Key Types:
/// - INT_PTR: Signed integer type for pointer precision (32-bit: int, 64-bit: __int64)
/// - UINT_PTR: Unsigned integer type for pointer precision
/// - LONG_PTR: Signed long type for pointer precision
/// - ULONG_PTR: Unsigned long type for pointer precision
/// - SIZE_T: Unsigned integer type for size
/// - SSIZE_T: Signed integer type for size
/// - DWORD_PTR: Unsigned integer type matching pointer size
///
/// Usage: Use these types when performing pointer arithmetic or storing pointer
/// values as integers to ensure correct behavior across architectures.
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types
#include <basetsd.h>
/// ==================================================================================
/// Windows Error Codes
/// ==================================================================================
/// Defines system error codes (ERROR_*) and HRESULT values used for error reporting.
///
/// Common Error Codes (ERROR_*):
/// - ERROR_SUCCESS (0): Operation completed successfully
/// - ERROR_INVALID_FUNCTION (1): Incorrect function
/// - ERROR_FILE_NOT_FOUND (2): File not found
/// - ERROR_PATH_NOT_FOUND (3): Path not found
/// - ERROR_TOO_MANY_OPEN_FILES (4): Too many open files
/// - ERROR_ACCESS_DENIED (5): Access denied
/// - ERROR_INVALID_HANDLE (6): Invalid handle
/// - ERROR_NOT_ENOUGH_MEMORY (8): Insufficient memory
/// - ERROR_INVALID_DATA (13): Invalid data
/// - ERROR_OUTOFMEMORY (14): Out of memory
/// - ERROR_INVALID_PARAMETER (87): Invalid parameter
/// - ERROR_INSUFFICIENT_BUFFER (122): Buffer too small
/// - ERROR_ALREADY_EXISTS (183): File already exists
/// - ERROR_MORE_DATA (234): More data available
/// - ERROR_NO_MORE_ITEMS (259): No more items
/// - ERROR_TIMEOUT (1460): Timeout period expired
///
/// HRESULT Values:
/// - S_OK (0): Success
/// - S_FALSE (1): Success with warning
/// - E_FAIL (0x80004005): Unspecified failure
/// - E_INVALIDARG (0x80070057): Invalid argument
/// - E_OUTOFMEMORY (0x8007000E): Out of memory
/// - E_NOTIMPL (0x80004001): Not implemented
/// - E_NOINTERFACE (0x80004002): Interface not supported
/// - E_POINTER (0x80004003): Invalid pointer
/// - E_UNEXPECTED (0x8000FFFF): Unexpected failure
///
/// Macros for HRESULT:
/// - SUCCEEDED(hr): TRUE if HRESULT indicates success
/// - FAILED(hr): TRUE if HRESULT indicates failure
/// - HRESULT_FROM_WIN32(err): Convert Win32 error to HRESULT
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes
#include <winerror.h>
/// ==================================================================================
/// Windows NT Core Types
/// ==================================================================================
/// Defines Windows NT kernel types, access rights, and low-level structures.
///
/// Key Types:
/// - VOID, CHAR, SHORT, LONG, LONGLONG: Basic C types with explicit sizes
/// - WCHAR: Wide character (16-bit Unicode)
/// - HANDLE: Generic handle to kernel objects
/// - PVOID, LPVOID: Pointer to void
/// - LPCSTR, LPSTR: Pointers to ANSI strings
/// - LPCWSTR, LPWSTR: Pointers to Unicode strings
///
/// Access Rights Constants:
/// - GENERIC_READ, GENERIC_WRITE, GENERIC_EXECUTE, GENERIC_ALL
/// - DELETE, READ_CONTROL, WRITE_DAC, WRITE_OWNER, SYNCHRONIZE
///
/// Memory Protection Constants:
/// - PAGE_NOACCESS, PAGE_READONLY, PAGE_READWRITE
/// - PAGE_EXECUTE, PAGE_EXECUTE_READ, PAGE_EXECUTE_READWRITE
/// - MEM_COMMIT, MEM_RESERVE, MEM_RELEASE, MEM_FREE
///
/// Key Structures:
/// - LARGE_INTEGER: 64-bit integer (union of QuadPart and LowPart/HighPart)
/// - ULARGE_INTEGER: Unsigned 64-bit integer
/// - LUID: Locally unique identifier
/// - LIST_ENTRY: Doubly-linked list entry
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/winnt/
#include <winnt.h>
/// ==================================================================================
/// Windows Extended Definitions
/// ==================================================================================
/// Extends minwindef.h with additional handle types and geometric structures.
///
/// Handle Types:
/// - HWND: Handle to a window
/// - HDC: Handle to a device context
/// - HBRUSH: Handle to a brush
/// - HPEN: Handle to a pen
/// - HFONT: Handle to a font
/// - HICON: Handle to an icon
/// - HCURSOR: Handle to a cursor
/// - HBITMAP: Handle to a bitmap
/// - HRGN: Handle to a region
/// - HMENU: Handle to a menu
/// - HINSTANCE: Handle to an instance/module
/// - HMODULE: Handle to a module (same as HINSTANCE)
///
/// Color Type:
/// - COLORREF: 32-bit color value (0x00BBGGRR format)
/// - RGB(r,g,b): Macro to create COLORREF
///
/// Geometric Structures:
/// - POINT: 2D point { LONG x; LONG y; }
/// - POINTL: 2D point with LONG members
/// - RECT: Rectangle { LONG left, top, right, bottom; }
/// - RECTL: Rectangle with LONG members
/// - SIZE: Dimensions { LONG cx; LONG cy; }
/// - SIZEL: Size with LONG members
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/windef/
#include <windef.h>
/// ==================================================================================
/// Windows Base Structures & Security
/// ==================================================================================
/// Provides core structures for synchronization, security, and file operations
/// that are essential for the kernel API.
///
/// Key Structures:
/// - CRITICAL_SECTION: Lightweight synchronization primitive for thread exclusion
///   typedef struct _CRITICAL_SECTION {
///       void* DebugInfo;
///       LONG LockCount;
///       LONG RecursionCount;
///       HANDLE OwningThread;
///       HANDLE LockSemaphore;
///       ULONG_PTR SpinCount;
///   } CRITICAL_SECTION;
///
/// - SECURITY_ATTRIBUTES: Security descriptor for object creation
///   typedef struct _SECURITY_ATTRIBUTES {
///       DWORD nLength;
///       LPVOID lpSecurityDescriptor;
///       BOOL bInheritHandle;
///   } SECURITY_ATTRIBUTES;
///
/// - FILETIME: 64-bit time value (100-nanosecond intervals since 1601-01-01)
///   typedef struct _FILETIME {
///       DWORD dwLowDateTime;
///       DWORD dwHighDateTime;
///   } FILETIME;
///
/// - SYSTEMTIME: Date and time in human-readable format
///   typedef struct _SYSTEMTIME {
///       WORD wYear;
///       WORD wMonth;
///       WORD wDayOfWeek;
///       WORD wDay;
///       WORD wHour;
///       WORD wMinute;
///       WORD wSecond;
///       WORD wMilliseconds;
///   } SYSTEMTIME;
///
/// - OVERLAPPED: Structure for asynchronous I/O operations
///   typedef struct _OVERLAPPED {
///       ULONG_PTR Internal;
///       ULONG_PTR InternalHigh;
///       union {
///           struct {
///               DWORD Offset;
///               DWORD OffsetHigh;
///           };
///           PVOID Pointer;
///       };
///       HANDLE hEvent;
///   } OVERLAPPED;
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/
#include <minwinbase.h>

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_base__included */
