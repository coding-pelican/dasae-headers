/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    crypt.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-31 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows cryptography APIs (Crypt32)
 * @details Provides functions for certificate management, data protection,
 *          and cryptographic operations using the Windows CryptoAPI.
 *
 * API Overview:
 * - Data Protection: CryptProtectData, CryptUnprotectData
 * - Base64: CryptBinaryToStringA, CryptStringToBinaryA
 * - Certificates: CertOpenStore, CertFindCertificateInStore, CertCloseStore,
 *   CertFreeCertificateContext, CertGetNameString
 * - Hash: CryptHashData, CryptCreateHash, CryptDestroyHash, CryptGetHashParam
 */
#ifndef os_windows_crypt__included
#define os_windows_crypt__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// Data Protection API (DPAPI)
/// ==================================================================================
/// Provides easy-to-use functions for encrypting and decrypting data using
/// user or machine credentials. Ideal for protecting sensitive configuration data.
///
/// Key APIs:
///
/// - BOOL CryptProtectData(DATA_BLOB* pDataIn, LPCWSTR szDataDescr, DATA_BLOB* pOptionalEntropy, PVOID pvReserved, CRYPTPROTECT_PROMPTSTRUCT* pPromptStruct, DWORD dwFlags, DATA_BLOB* pDataOut);
///   Encrypts data using user credentials.
///   @param pDataIn Pointer to DATA_BLOB containing data to encrypt
///   @param szDataDescr Description string (stored with encrypted data)
///   @param pOptionalEntropy Optional additional entropy (\b NULL for none)
///   @param pvReserved Reserved, must be \b NULL
///   @param pPromptStruct Optional UI prompt configuration (\b NULL for no UI)
///   @param dwFlags Protection flags:
///     - \b CRYPTPROTECT_LOCAL_MACHINE : Encrypt for any user on this machine
///     - \b CRYPTPROTECT_UI_FORBIDDEN : Fail if UI would be shown
///     - \b CRYPTPROTECT_AUDIT : Generate audit event
///   @param pDataOut Receives encrypted data (free with LocalFree)
///   @return \b TRUE on success, \b FALSE on failure
///   @code{.c}
///     DATA_BLOB plaintext = { strlen(secret), (BYTE*)secret };
///     DATA_BLOB encrypted;
///     CryptProtectData(&plaintext, L"My Secret", NULL, NULL, NULL, 0, &encrypted);
///     // Use encrypted.pbData, encrypted.cbData
///     LocalFree(encrypted.pbData);
///   @endcode
///
/// - BOOL CryptUnprotectData(DATA_BLOB* pDataIn, LPWSTR* ppszDataDescr, DATA_BLOB* pOptionalEntropy, PVOID pvReserved, CRYPTPROTECT_PROMPTSTRUCT* pPromptStruct, DWORD dwFlags, DATA_BLOB* pDataOut);
///   Decrypts data previously encrypted with CryptProtectData.
///   @param pDataIn Pointer to DATA_BLOB containing encrypted data
///   @param ppszDataDescr Receives description string (free with LocalFree, can be \b NULL)
///   @param pOptionalEntropy Same entropy used during encryption (\b NULL if none)
///   @param pvReserved Reserved, must be \b NULL
///   @param pPromptStruct Optional UI prompt configuration
///   @param dwFlags Decryption flags
///   @param pDataOut Receives decrypted data (free with LocalFree)
///   @return \b TRUE on success, \b FALSE on failure
///
/// DATA_BLOB Structure:
/// @code{.c}
/// typedef struct _CRYPTOAPI_BLOB {
///     DWORD cbData;   // Size in bytes
///     BYTE* pbData;   // Pointer to data
/// } DATA_BLOB, *PDATA_BLOB;
/// @endcode
///
/// ==================================================================================
/// Base64 Encoding API
/// ==================================================================================
/// Provides functions for converting between binary data and Base64 strings.
///
/// Key APIs:
///
/// - BOOL CryptBinaryToStringA(const BYTE* pbBinary, DWORD cbBinary, DWORD dwFlags, LPSTR pszString, DWORD* pcchString);
///   Converts binary data to a formatted string (Base64, hex, etc.).
///   @param pbBinary Binary data to convert
///   @param cbBinary Size of binary data
///   @param dwFlags Output format:
///     - \b CRYPT_STRING_BASE64 : Base64 without headers
///     - \b CRYPT_STRING_BASE64HEADER : Base64 with "-----BEGIN-----" headers
///     - \b CRYPT_STRING_HEX : Hexadecimal
///     - \b CRYPT_STRING_HEXRAW : Hex without spaces
///   @param pszString Buffer to receive string (\b NULL to query size)
///   @param pcchString On input, buffer size. On output, string length.
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL CryptStringToBinaryA(LPCSTR pszString, DWORD cchString, DWORD dwFlags, BYTE* pbBinary, DWORD* pcbBinary, DWORD* pdwSkip, DWORD* pdwFlags);
///   Converts a formatted string to binary data.
///   @param pszString String to convert
///   @param cchString String length (0 for null-terminated)
///   @param dwFlags Input format (same as CryptBinaryToStringA)
///   @param pbBinary Buffer to receive binary data (\b NULL to query size)
///   @param pcbBinary On input, buffer size. On output, data size.
///   @param pdwSkip Receives bytes skipped (can be \b NULL)
///   @param pdwFlags Receives detected format (can be \b NULL)
///   @return \b TRUE on success, \b FALSE on failure
///
/// ==================================================================================
/// Certificate Store API
/// ==================================================================================
/// Provides functions for managing certificate stores and certificates.
///
/// Key APIs:
///
/// Store Management:
/// - HCERTSTORE CertOpenStore(LPCSTR lpszStoreProvider, DWORD dwEncodingType, HCRYPTPROV_LEGACY hCryptProv, DWORD dwFlags, const void* pvPara);
///   Opens a certificate store.
///   @param lpszStoreProvider Store provider: \b CERT_STORE_PROV_SYSTEM_A, \b CERT_STORE_PROV_MEMORY
///   @param dwEncodingType Encoding type (usually \b X509_ASN_ENCODING | PKCS_7_ASN_ENCODING)
///   @param hCryptProv Legacy parameter, use 0
///   @param dwFlags Open flags: \b CERT_SYSTEM_STORE_CURRENT_USER, \b CERT_SYSTEM_STORE_LOCAL_MACHINE
///   @param pvPara Provider-specific (store name for system stores, e.g., "MY", "ROOT")
///   @return Store handle on success, \b NULL on failure
///   @code{.c}
///     HCERTSTORE hStore = CertOpenStore(
///         CERT_STORE_PROV_SYSTEM_A,
///         0, 0,
///         CERT_SYSTEM_STORE_CURRENT_USER,
///         "MY"  // Personal certificates
///     );
///   @endcode
///
/// - BOOL CertCloseStore(HCERTSTORE hCertStore, DWORD dwFlags);
///   Closes a certificate store.
///   @param hCertStore Handle to the store
///   @param dwFlags Close flags (usually 0)
///   @return \b TRUE on success
///
/// Certificate Lookup:
/// - PCCERT_CONTEXT CertFindCertificateInStore(HCERTSTORE hCertStore, DWORD dwCertEncodingType, DWORD dwFindFlags, DWORD dwFindType, const void* pvFindPara, PCCERT_CONTEXT pPrevCertContext);
///   Finds a certificate in a store.
///   @param hCertStore Handle to the store
///   @param dwCertEncodingType Encoding type
///   @param dwFindFlags Find flags (usually 0)
///   @param dwFindType Find type: \b CERT_FIND_SUBJECT_STR, \b CERT_FIND_ISSUER_STR, \b CERT_FIND_ANY
///   @param pvFindPara Search criteria (depends on dwFindType)
///   @param pPrevCertContext Previous result for enumeration (\b NULL for first)
///   @return Certificate context on success, \b NULL if not found
///   @note Call \b CertFreeCertificateContext on the returned context.
///
/// - BOOL CertFreeCertificateContext(PCCERT_CONTEXT pCertContext);
///   Frees a certificate context.
///   @param pCertContext Context to free
///   @return \b TRUE always
///
/// Certificate Information:
/// - DWORD CertGetNameStringA(PCCERT_CONTEXT pCertContext, DWORD dwType, DWORD dwFlags, void* pvTypePara, LPSTR pszNameString, DWORD cchNameString);
///   Retrieves the subject or issuer name from a certificate.
///   @param pCertContext Certificate context
///   @param dwType Name type: \b CERT_NAME_SIMPLE_DISPLAY_TYPE, \b CERT_NAME_FRIENDLY_DISPLAY_TYPE
///   @param dwFlags Flags (0 for subject, \b CERT_NAME_ISSUER_FLAG for issuer)
///   @param pvTypePara Type-specific parameter
///   @param pszNameString Buffer to receive name
///   @param cchNameString Buffer size
///   @return Number of characters copied
///
/// Common System Store Names:
/// - "MY": Personal certificates
/// - "ROOT": Trusted root CAs
/// - "CA": Intermediate CAs
/// - "Trust": Enterprise trust
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/dpapi/
/// https://learn.microsoft.com/en-us/windows/win32/api/wincrypt/
#include <wincrypt.h>
#pragma comment(lib, "crypt32.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_crypt__included */
