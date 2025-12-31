/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    auth.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-31 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows security, registry, and advanced API services
 * @details Provides functions from Advapi32.dll including user account management,
 *          registry access, cryptographic random number generation, and security.
 *
 * API Overview:
 * - User Info: GetUserName
 * - Registry: RegOpenKeyEx, RegQueryValueEx, RegSetValueEx, RegCloseKey,
 *   RegCreateKeyEx, RegDeleteKey, RegDeleteValue, RegEnumKeyEx, RegEnumValue
 * - Random: SystemFunction036 (RtlGenRandom)
 * - Security: OpenProcessToken, GetTokenInformation, AdjustTokenPrivileges
 */
#ifndef os_windows_auth__included
#define os_windows_auth__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// User Information API
/// ==================================================================================
/// Provides functions for retrieving user account information.
///
/// Key APIs:
///
/// - BOOL GetUserNameA(LPSTR lpBuffer, LPDWORD pcbBuffer);
///   Retrieves the name of the user associated with the current thread (ANSI).
///   @param lpBuffer Buffer to receive the user name
///   @param pcbBuffer On input, size of buffer. On output, size of name including null.
///   @return \b TRUE on success, \b FALSE on failure
///   @note Requires Advapi32.lib
///
/// - BOOL GetUserNameW(LPWSTR lpBuffer, LPDWORD pcbBuffer);
///   Retrieves the name of the user associated with the current thread (Unicode).
///   @param lpBuffer Buffer to receive the user name
///   @param pcbBuffer On input, size of buffer. On output, size of name including null.
///   @return \b TRUE on success, \b FALSE on failure
///
/// ==================================================================================
/// Registry API
/// ==================================================================================
/// Provides functions for reading and writing the Windows Registry.
///
/// Key APIs:
///
/// Opening/Creating Keys:
/// - LSTATUS RegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
///   Opens the specified registry key.
///   @param hKey Base key: \b HKEY_CLASSES_ROOT, \b HKEY_CURRENT_USER, \b HKEY_LOCAL_MACHINE, etc.
///   @param lpSubKey Name of the subkey to open
///   @param ulOptions Reserved, must be 0
///   @param samDesired Access rights: \b KEY_READ, \b KEY_WRITE, \b KEY_ALL_ACCESS
///   @param phkResult Receives handle to opened key
///   @return \b ERROR_SUCCESS on success, error code on failure
///
/// - LSTATUS RegCreateKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, const LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition);
///   Creates or opens a registry key.
///   @param hKey Base key
///   @param lpSubKey Name of the subkey to create/open
///   @param Reserved Reserved, must be 0
///   @param lpClass User-defined class type (\b NULL typically)
///   @param dwOptions Options: \b REG_OPTION_NON_VOLATILE (default), \b REG_OPTION_VOLATILE
///   @param samDesired Access rights
///   @param lpSecurityAttributes Security attributes (\b NULL for default)
///   @param phkResult Receives handle to key
///   @param lpdwDisposition Receives \b REG_CREATED_NEW_KEY or \b REG_OPENED_EXISTING_KEY
///   @return \b ERROR_SUCCESS on success
///
/// Reading Values:
/// - LSTATUS RegQueryValueExA(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
///   Retrieves the type and data for the specified value.
///   @param hKey Handle to an open key
///   @param lpValueName Name of value (\b NULL or empty for default value)
///   @param lpReserved Reserved, must be \b NULL
///   @param lpType Receives value type: \b REG_SZ, \b REG_DWORD, \b REG_BINARY, etc.
///   @param lpData Buffer to receive data (\b NULL to query size)
///   @param lpcbData On input, buffer size. On output, data size.
///   @return \b ERROR_SUCCESS on success
///
/// Writing Values:
/// - LSTATUS RegSetValueExA(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE* lpData, DWORD cbData);
///   Sets the data and type of a specified value.
///   @param hKey Handle to an open key
///   @param lpValueName Name of value to set
///   @param Reserved Reserved, must be 0
///   @param dwType Value type: \b REG_SZ, \b REG_DWORD, \b REG_BINARY, etc.
///   @param lpData Data to store
///   @param cbData Size of data in bytes
///   @return \b ERROR_SUCCESS on success
///
/// Deleting:
/// - LSTATUS RegDeleteKeyA(HKEY hKey, LPCSTR lpSubKey);
///   Deletes a subkey and its values.
///   @param hKey Handle to an open key
///   @param lpSubKey Name of subkey to delete
///   @return \b ERROR_SUCCESS on success
///
/// - LSTATUS RegDeleteValueA(HKEY hKey, LPCSTR lpValueName);
///   Removes a named value from a key.
///   @param hKey Handle to an open key
///   @param lpValueName Name of value to delete
///   @return \b ERROR_SUCCESS on success
///
/// Enumeration:
/// - LSTATUS RegEnumKeyExA(HKEY hKey, DWORD dwIndex, LPSTR lpName, LPDWORD lpcchName, LPDWORD lpReserved, LPSTR lpClass, LPDWORD lpcchClass, PFILETIME lpftLastWriteTime);
///   Enumerates subkeys of a key.
///   @param hKey Handle to an open key
///   @param dwIndex Index of subkey (0 for first, increment for each call)
///   @param lpName Buffer to receive subkey name
///   @param lpcchName On input, buffer size. On output, name length.
///   @return \b ERROR_SUCCESS on success, \b ERROR_NO_MORE_ITEMS when done
///
/// - LSTATUS RegEnumValueA(HKEY hKey, DWORD dwIndex, LPSTR lpValueName, LPDWORD lpcchValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
///   Enumerates values of a key.
///   @param hKey Handle to an open key
///   @param dwIndex Index of value (0 for first)
///   @param lpValueName Buffer to receive value name
///   @param lpcchValueName On input, buffer size. On output, name length.
///   @return \b ERROR_SUCCESS on success, \b ERROR_NO_MORE_ITEMS when done
///
/// Closing:
/// - LSTATUS RegCloseKey(HKEY hKey);
///   Closes a handle to the specified registry key.
///   @param hKey Handle to close
///   @return \b ERROR_SUCCESS on success
///
/// Value Types:
/// - \b REG_NONE (0): No defined value type
/// - \b REG_SZ (1): Null-terminated string
/// - \b REG_EXPAND_SZ (2): Null-terminated string with environment variable references
/// - \b REG_BINARY (3): Binary data
/// - \b REG_DWORD (4): 32-bit number
/// - \b REG_DWORD_BIG_ENDIAN (5): 32-bit big-endian number
/// - \b REG_MULTI_SZ (7): Array of null-terminated strings
/// - \b REG_QWORD (11): 64-bit number
///
/// ==================================================================================
/// Cryptographic Random API
/// ==================================================================================
/// Provides cryptographically secure random number generation.
///
/// Key APIs:
///
/// - BOOLEAN RtlGenRandom(PVOID RandomBuffer, ULONG RandomBufferLength);
///   Generates cryptographically random bytes. Also known as SystemFunction036.
///   @param RandomBuffer Buffer to receive random bytes
///   @param RandomBufferLength Number of random bytes to generate
///   @return \b TRUE on success, \b FALSE on failure
///   @note Declared as SystemFunction036 in ntsecapi.h. Use #define RtlGenRandom SystemFunction036.
///   @code{.c}
///     #define RtlGenRandom SystemFunction036
///     BYTE random[16];
///     RtlGenRandom(random, sizeof(random));
///   @endcode
///
/// ==================================================================================
/// Security Token API
/// ==================================================================================
/// Provides functions for managing process and thread security tokens.
///
/// Key APIs:
///
/// - BOOL OpenProcessToken(HANDLE ProcessHandle, DWORD DesiredAccess, PHANDLE TokenHandle);
///   Opens the access token associated with a process.
///   @param ProcessHandle Handle to the process
///   @param DesiredAccess Access rights: \b TOKEN_QUERY, \b TOKEN_ADJUST_PRIVILEGES, etc.
///   @param TokenHandle Receives the token handle
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL GetTokenInformation(HANDLE TokenHandle, TOKEN_INFORMATION_CLASS TokenInformationClass, LPVOID TokenInformation, DWORD TokenInformationLength, PDWORD ReturnLength);
///   Retrieves information about a token.
///   @param TokenHandle Handle to the token
///   @param TokenInformationClass Type of information to retrieve
///   @param TokenInformation Buffer to receive information
///   @param TokenInformationLength Size of buffer
///   @param ReturnLength Receives required size
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL AdjustTokenPrivileges(HANDLE TokenHandle, BOOL DisableAllPrivileges, PTOKEN_PRIVILEGES NewState, DWORD BufferLength, PTOKEN_PRIVILEGES PreviousState, PDWORD ReturnLength);
///   Enables or disables privileges in a token.
///   @param TokenHandle Handle to the token
///   @param DisableAllPrivileges \b TRUE to disable all privileges
///   @param NewState Privileges to enable/disable
///   @param BufferLength Size of PreviousState buffer
///   @param PreviousState Receives previous state (\b NULL if not needed)
///   @param ReturnLength Receives required size
///   @return \b TRUE on success, \b FALSE on failure
///   @note Call \b GetLastError to check for \b ERROR_NOT_ALL_ASSIGNED.
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/winreg/
/// https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/
#include <winreg.h>
/// ==================================================================================
/// Registry Constants
/// ==================================================================================
/// Defines registry value types, access rights, and options.
///
/// Registry Value Types:
#if !defined(REG_NONE)
#define REG_NONE 0
#endif
#if !defined(REG_SZ)
#define REG_SZ 1
#endif
#if !defined(REG_EXPAND_SZ)
#define REG_EXPAND_SZ 2
#endif
#if !defined(REG_BINARY)
#define REG_BINARY 3
#endif
#if !defined(REG_DWORD)
#define REG_DWORD 4
#endif
#if !defined(REG_DWORD_LITTLE_ENDIAN)
#define REG_DWORD_LITTLE_ENDIAN 4
#endif
#if !defined(REG_DWORD_BIG_ENDIAN)
#define REG_DWORD_BIG_ENDIAN 5
#endif
#if !defined(REG_LINK)
#define REG_LINK 6
#endif
#if !defined(REG_MULTI_SZ)
#define REG_MULTI_SZ 7
#endif
#if !defined(REG_RESOURCE_LIST)
#define REG_RESOURCE_LIST 8
#endif
#if !defined(REG_QWORD)
#define REG_QWORD 11
#endif
#if !defined(REG_QWORD_LITTLE_ENDIAN)
#define REG_QWORD_LITTLE_ENDIAN 11
#endif
/// Registry Key Access Rights:
#if !defined(KEY_QUERY_VALUE)
#define KEY_QUERY_VALUE 0x0001
#endif
#if !defined(KEY_SET_VALUE)
#define KEY_SET_VALUE 0x0002
#endif
#if !defined(KEY_CREATE_SUB_KEY)
#define KEY_CREATE_SUB_KEY 0x0004
#endif
#if !defined(KEY_ENUMERATE_SUB_KEYS)
#define KEY_ENUMERATE_SUB_KEYS 0x0008
#endif
#if !defined(KEY_NOTIFY)
#define KEY_NOTIFY 0x0010
#endif
#if !defined(KEY_CREATE_LINK)
#define KEY_CREATE_LINK 0x0020
#endif
#if !defined(KEY_WOW64_64KEY)
#define KEY_WOW64_64KEY 0x0100
#endif
#if !defined(KEY_WOW64_32KEY)
#define KEY_WOW64_32KEY 0x0200
#endif
#if !defined(KEY_READ)
#define KEY_READ 0x20019
#endif
#if !defined(KEY_WRITE)
#define KEY_WRITE 0x20006
#endif
#if !defined(KEY_EXECUTE)
#define KEY_EXECUTE 0x20019
#endif
#if !defined(KEY_ALL_ACCESS)
#define KEY_ALL_ACCESS 0xF003F
#endif
/// Registry Key Options:
#if !defined(REG_OPTION_NON_VOLATILE)
#define REG_OPTION_NON_VOLATILE 0x00000000
#endif
#if !defined(REG_OPTION_VOLATILE)
#define REG_OPTION_VOLATILE 0x00000001
#endif
#if !defined(REG_OPTION_CREATE_LINK)
#define REG_OPTION_CREATE_LINK 0x00000002
#endif
#if !defined(REG_OPTION_BACKUP_RESTORE)
#define REG_OPTION_BACKUP_RESTORE 0x00000004
#endif
#if !defined(REG_OPTION_OPEN_LINK)
#define REG_OPTION_OPEN_LINK 0x00000008
#endif
/// Registry Key Disposition (returned by RegCreateKeyEx):
#if !defined(REG_CREATED_NEW_KEY)
#define REG_CREATED_NEW_KEY 0x00000001
#endif
#if !defined(REG_OPENED_EXISTING_KEY)
#define REG_OPENED_EXISTING_KEY 0x00000002
#endif
/// ==================================================================================
/// Token Access Rights
/// ==================================================================================
/// Defines access rights for process and thread tokens.
#if !defined(TOKEN_ASSIGN_PRIMARY)
#define TOKEN_ASSIGN_PRIMARY 0x0001
#endif
#if !defined(TOKEN_DUPLICATE)
#define TOKEN_DUPLICATE 0x0002
#endif
#if !defined(TOKEN_IMPERSONATE)
#define TOKEN_IMPERSONATE 0x0004
#endif
#if !defined(TOKEN_QUERY)
#define TOKEN_QUERY 0x0008
#endif
#if !defined(TOKEN_QUERY_SOURCE)
#define TOKEN_QUERY_SOURCE 0x0010
#endif
#if !defined(TOKEN_ADJUST_PRIVILEGES)
#define TOKEN_ADJUST_PRIVILEGES 0x0020
#endif
#if !defined(TOKEN_ADJUST_GROUPS)
#define TOKEN_ADJUST_GROUPS 0x0040
#endif
#if !defined(TOKEN_ADJUST_DEFAULT)
#define TOKEN_ADJUST_DEFAULT 0x0080
#endif
#if !defined(TOKEN_ADJUST_SESSIONID)
#define TOKEN_ADJUST_SESSIONID 0x0100
#endif
#if !defined(TOKEN_READ)
#define TOKEN_READ 0x20008
#endif
#if !defined(TOKEN_WRITE)
#define TOKEN_WRITE 0x200E0
#endif
#if !defined(TOKEN_EXECUTE)
#define TOKEN_EXECUTE 0x20000
#endif
#if !defined(TOKEN_ALL_ACCESS)
#define TOKEN_ALL_ACCESS 0xF01FF
#endif
/// ==================================================================================
/// Privilege Constants
/// ==================================================================================
/// Defines privilege attribute flags for AdjustTokenPrivileges.
#if !defined(SE_PRIVILEGE_ENABLED_BY_DEFAULT)
#define SE_PRIVILEGE_ENABLED_BY_DEFAULT 0x00000001
#endif
#if !defined(SE_PRIVILEGE_ENABLED)
#define SE_PRIVILEGE_ENABLED 0x00000002
#endif
#if !defined(SE_PRIVILEGE_REMOVED)
#define SE_PRIVILEGE_REMOVED 0x00000004
#endif
#if !defined(SE_PRIVILEGE_USED_FOR_ACCESS)
#define SE_PRIVILEGE_USED_FOR_ACCESS 0x80000000
#endif
/// Registry Root Key Handles:
/// These are typically defined in winreg.h but included here for backward compatibility.
#if !defined(HKEY_CLASSES_ROOT)
#define HKEY_CLASSES_ROOT ((HKEY)(ULONG_PTR)((LONG)0x80000000))
#endif
#if !defined(HKEY_CURRENT_USER)
#define HKEY_CURRENT_USER ((HKEY)(ULONG_PTR)((LONG)0x80000001))
#endif
#if !defined(HKEY_LOCAL_MACHINE)
#define HKEY_LOCAL_MACHINE ((HKEY)(ULONG_PTR)((LONG)0x80000002))
#endif
#if !defined(HKEY_USERS)
#define HKEY_USERS ((HKEY)(ULONG_PTR)((LONG)0x80000003))
#endif
#if !defined(HKEY_PERFORMANCE_DATA)
#define HKEY_PERFORMANCE_DATA ((HKEY)(ULONG_PTR)((LONG)0x80000004))
#endif
#if !defined(HKEY_CURRENT_CONFIG)
#define HKEY_CURRENT_CONFIG ((HKEY)(ULONG_PTR)((LONG)0x80000005))
#endif
#if !defined(HKEY_DYN_DATA)
#define HKEY_DYN_DATA ((HKEY)(ULONG_PTR)((LONG)0x80000006))
#endif
/// Maximum Registry Key/Value Name Lengths:
#if !defined(MAX_PATH)
#define MAX_PATH 260
#endif
#pragma comment(lib, "advapi32.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_auth__included */
