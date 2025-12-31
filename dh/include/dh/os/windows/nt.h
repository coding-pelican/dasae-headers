/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    nt.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-31 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows Native API (NTAPI) definitions
 * @details Provides access to low-level NT kernel functions. These are internal
 *          Windows APIs that may change between versions. Use with caution.
 *
 * API Overview:
 * - Status: NTSTATUS type and macros (NT_SUCCESS, NT_ERROR)
 * - Strings: UNICODE_STRING, ANSI_STRING structures
 * - Objects: OBJECT_ATTRIBUTES structure
 * - Files: NtCreateFile, NtReadFile, NtWriteFile, NtClose
 * - Process: NtQueryInformationProcess, NtQuerySystemInformation
 *
 * @warning These APIs are semi-documented and may change between Windows versions.
 *          Prefer Win32 APIs when possible.
 */
#ifndef os_windows_nt__included
#define os_windows_nt__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// NT Status Codes
/// ==================================================================================
/// NTSTATUS is the return type for most NT functions. It encodes success, warning,
/// and error conditions in a 32-bit value.
///
/// Status Code Macros:
/// - NT_SUCCESS(Status): Returns TRUE if Status indicates success (>= 0)
/// - NT_INFORMATION(Status): Returns TRUE if Status is informational
/// - NT_WARNING(Status): Returns TRUE if Status is a warning
/// - NT_ERROR(Status): Returns TRUE if Status indicates an error
///
/// Common Status Codes:
/// - \b STATUS_SUCCESS (0x00000000): Operation completed successfully
/// - \b STATUS_PENDING (0x00000103): Operation is pending
/// - \b STATUS_ACCESS_DENIED (0xC0000022): Access denied
/// - \b STATUS_INVALID_PARAMETER (0xC000000D): Invalid parameter
/// - \b STATUS_NO_MEMORY (0xC0000017): Insufficient memory
/// - \b STATUS_OBJECT_NAME_NOT_FOUND (0xC0000034): Object name not found
/// - \b STATUS_BUFFER_TOO_SMALL (0xC0000023): Buffer too small
///
/// Converting NTSTATUS to Win32 Error:
/// Use RtlNtStatusToDosError() or map common values manually.
///
/// ==================================================================================
/// NT String Types
/// ==================================================================================
/// NT uses counted strings rather than null-terminated strings.
///
/// UNICODE_STRING Structure:
/// @code{.c}
/// typedef struct _UNICODE_STRING {
///     USHORT Length;         // Length in bytes (not including null if present)
///     USHORT MaximumLength;  // Size of Buffer in bytes
///     PWSTR  Buffer;         // Pointer to wide-char buffer
/// } UNICODE_STRING, *PUNICODE_STRING;
/// @endcode
///
/// ANSI_STRING Structure:
/// @code{.c}
/// typedef struct _ANSI_STRING {
///     USHORT Length;         // Length in bytes (not including null)
///     USHORT MaximumLength;  // Size of Buffer in bytes
///     PSTR   Buffer;         // Pointer to char buffer
/// } ANSI_STRING, *PANSI_STRING;
/// @endcode
///
/// ==================================================================================
/// OBJECT_ATTRIBUTES Structure
/// ==================================================================================
/// Used to specify object properties when creating or opening objects.
///
/// @code{.c}
/// typedef struct _OBJECT_ATTRIBUTES {
///     ULONG Length;                    // Size of this structure
///     HANDLE RootDirectory;            // Handle to root directory (or NULL)
///     PUNICODE_STRING ObjectName;      // Object name
///     ULONG Attributes;                // Object attributes flags
///     PVOID SecurityDescriptor;        // Security descriptor
///     PVOID SecurityQualityOfService;  // Security QOS
/// } OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;
/// @endcode
///
/// Attribute Flags:
/// - \b OBJ_INHERIT (0x00000002): Handle is inheritable
/// - \b OBJ_PERMANENT (0x00000010): Object is permanent
/// - \b OBJ_EXCLUSIVE (0x00000020): Exclusive access
/// - \b OBJ_CASE_INSENSITIVE (0x00000040): Case-insensitive name lookup
/// - \b OBJ_KERNEL_HANDLE (0x00000200): Kernel-mode handle
///
/// InitializeObjectAttributes Macro:
/// @code{.c}
/// #define InitializeObjectAttributes(p, n, a, r, s) { \
///     (p)->Length = sizeof(OBJECT_ATTRIBUTES); \
///     (p)->RootDirectory = r; \
///     (p)->Attributes = a; \
///     (p)->ObjectName = n; \
///     (p)->SecurityDescriptor = s; \
///     (p)->SecurityQualityOfService = NULL; \
/// }
/// @endcode
///
/// ==================================================================================
/// NT File Operations
/// ==================================================================================
/// Low-level file operations. Prefer CreateFile/ReadFile/WriteFile when possible.
///
/// Key APIs (from ntdll.dll):
///
/// - NTSTATUS NtCreateFile(PHANDLE FileHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock, PLARGE_INTEGER AllocationSize, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions, PVOID EaBuffer, ULONG EaLength);
///   Creates or opens a file.
///   @return NTSTATUS code
///   @note Use CreateFile() for most scenarios.
///
/// - NTSTATUS NtReadFile(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext, PIO_STATUS_BLOCK IoStatusBlock, PVOID Buffer, ULONG Length, PLARGE_INTEGER ByteOffset, PULONG Key);
///   Reads from a file.
///   @return NTSTATUS code
///
/// - NTSTATUS NtWriteFile(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext, PIO_STATUS_BLOCK IoStatusBlock, PVOID Buffer, ULONG Length, PLARGE_INTEGER ByteOffset, PULONG Key);
///   Writes to a file.
///   @return NTSTATUS code
///
/// - NTSTATUS NtClose(HANDLE Handle);
///   Closes an object handle.
///   @return NTSTATUS code
///   @note Use CloseHandle() for Win32 handles.
///
/// ==================================================================================
/// NT Process/System Information
/// ==================================================================================
///
/// - NTSTATUS NtQueryInformationProcess(HANDLE ProcessHandle, PROCESSINFOCLASS ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);
///   Retrieves information about a process.
///   @param ProcessHandle Handle to the process
///   @param ProcessInformationClass Type of information to retrieve
///   @param ProcessInformation Buffer to receive information
///   @param ProcessInformationLength Size of buffer
///   @param ReturnLength Receives actual size needed
///   @return NTSTATUS code
///
/// - NTSTATUS NtQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
///   Retrieves system information.
///   @param SystemInformationClass Type of information to retrieve
///   @param SystemInformation Buffer to receive information
///   @param SystemInformationLength Size of buffer
///   @param ReturnLength Receives actual size needed
///   @return NTSTATUS code
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntddk/
///
/// @note To use NT functions, load them dynamically from ntdll.dll or use
/// the Windows Driver Kit (WDK) headers.
#include <winternl.h>
/// ==================================================================================
/// NT Status Constants (winternl.h subset)
/// ==================================================================================
/// Defines common NTSTATUS codes used by NT functions.
///
/// Success Codes:
#if !defined(STATUS_SUCCESS)
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#endif
#if !defined(STATUS_PENDING)
#define STATUS_PENDING ((NTSTATUS)0x00000103L)
#endif
#if !defined(STATUS_WAIT_0)
#define STATUS_WAIT_0 ((NTSTATUS)0x00000000L)
#endif
/// Warning Codes:
#if !defined(STATUS_BUFFER_OVERFLOW)
#define STATUS_BUFFER_OVERFLOW ((NTSTATUS)0x80000005L)
#endif
/// Error Codes:
#if !defined(STATUS_UNSUCCESSFUL)
#define STATUS_UNSUCCESSFUL ((NTSTATUS)0xC0000001L)
#endif
#if !defined(STATUS_NOT_IMPLEMENTED)
#define STATUS_NOT_IMPLEMENTED ((NTSTATUS)0xC0000002L)
#endif
#if !defined(STATUS_INVALID_INFO_CLASS)
#define STATUS_INVALID_INFO_CLASS ((NTSTATUS)0xC0000003L)
#endif
#if !defined(STATUS_INFO_LENGTH_MISMATCH)
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
#endif
#if !defined(STATUS_ACCESS_VIOLATION)
#define STATUS_ACCESS_VIOLATION ((NTSTATUS)0xC0000005L)
#endif
#if !defined(STATUS_INVALID_HANDLE)
#define STATUS_INVALID_HANDLE ((NTSTATUS)0xC0000008L)
#endif
#if !defined(STATUS_INVALID_PARAMETER)
#define STATUS_INVALID_PARAMETER ((NTSTATUS)0xC000000DL)
#endif
#if !defined(STATUS_NO_MEMORY)
#define STATUS_NO_MEMORY ((NTSTATUS)0xC0000017L)
#endif
#if !defined(STATUS_ACCESS_DENIED)
#define STATUS_ACCESS_DENIED ((NTSTATUS)0xC0000022L)
#endif
#if !defined(STATUS_BUFFER_TOO_SMALL)
#define STATUS_BUFFER_TOO_SMALL ((NTSTATUS)0xC0000023L)
#endif
#if !defined(STATUS_OBJECT_NAME_INVALID)
#define STATUS_OBJECT_NAME_INVALID ((NTSTATUS)0xC0000033L)
#endif
#if !defined(STATUS_OBJECT_NAME_NOT_FOUND)
#define STATUS_OBJECT_NAME_NOT_FOUND ((NTSTATUS)0xC0000034L)
#endif
#if !defined(STATUS_OBJECT_NAME_COLLISION)
#define STATUS_OBJECT_NAME_COLLISION ((NTSTATUS)0xC0000035L)
#endif
#if !defined(STATUS_OBJECT_PATH_NOT_FOUND)
#define STATUS_OBJECT_PATH_NOT_FOUND ((NTSTATUS)0xC000003AL)
#endif
/// Status Code Macros:
#if !defined(NT_SUCCESS)
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif
#if !defined(NT_INFORMATION)
#define NT_INFORMATION(Status) ((((ULONG)(Status)) >> 30) == 1)
#endif
#if !defined(NT_WARNING)
#define NT_WARNING(Status) ((((ULONG)(Status)) >> 30) == 2)
#endif
#if !defined(NT_ERROR)
#define NT_ERROR(Status) ((((ULONG)(Status)) >> 30) == 3)
#endif
/// ==================================================================================
/// Object Attribute Flags (winternl.h subset)
/// ==================================================================================
/// Defines flags used in OBJECT_ATTRIBUTES.Attributes field.
#if !defined(OBJ_INHERIT)
#define OBJ_INHERIT 0x00000002L
#endif
#if !defined(OBJ_PERMANENT)
#define OBJ_PERMANENT 0x00000010L
#endif
#if !defined(OBJ_EXCLUSIVE)
#define OBJ_EXCLUSIVE 0x00000020L
#endif
#if !defined(OBJ_CASE_INSENSITIVE)
#define OBJ_CASE_INSENSITIVE 0x00000040L
#endif
#if !defined(OBJ_OPENIF)
#define OBJ_OPENIF 0x00000080L
#endif
#if !defined(OBJ_OPENLINK)
#define OBJ_OPENLINK 0x00000100L
#endif
#if !defined(OBJ_KERNEL_HANDLE)
#define OBJ_KERNEL_HANDLE 0x00000200L
#endif
#if !defined(OBJ_FORCE_ACCESS_CHECK)
#define OBJ_FORCE_ACCESS_CHECK 0x00000400L
#endif
#if !defined(OBJ_VALID_ATTRIBUTES)
#define OBJ_VALID_ATTRIBUTES 0x000007F2L
#endif
/// InitializeObjectAttributes Macro:
#if !defined(InitializeObjectAttributes)
#define InitializeObjectAttributes(p, n, a, r, s) \
    { \
        (p)->Length = sizeof(OBJECT_ATTRIBUTES); \
        (p)->RootDirectory = r; \
        (p)->Attributes = a; \
        (p)->ObjectName = n; \
        (p)->SecurityDescriptor = s; \
        (p)->SecurityQualityOfService = NULL; \
    }
#endif
#pragma comment(lib, "ntdll.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_nt__included */
