#include "internal.h"
#include "dal-c-ext/dir.h"
#include "dal-c-ext/file.h"
#include "dal-c-ext/path.h"
#include "dal-c-ext/proc.h"
#include "dal-c-ext/str.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define DAL_C_ARCHIVE_MARKER ".dh-c-archive"

typedef struct dal_c_Sha256 {
    uint32_t state[8];
    uint64_t bit_count;
    unsigned char block[64];
    size_t block_len;
} dal_c_Sha256;

static uint32_t dal_c__sha256RotateRight(uint32_t value, unsigned shift) {
    return (value >> shift) | (value << (32u - shift));
}

static void dal_c__sha256Transform(dal_c_Sha256* ctx, const unsigned char block[64]) {
    static const uint32_t constants[64] = {
        0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u,
        0x3956c25bu, 0x59f111f1u, 0x923f82a4u, 0xab1c5ed5u,
        0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u,
        0x72be5d74u, 0x80deb1feu, 0x9bdc06a7u, 0xc19bf174u,
        0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu,
        0x2de92c6fu, 0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau,
        0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u,
        0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u,
        0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu, 0x53380d13u,
        0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u,
        0xa2bfe8a1u, 0xa81a664bu, 0xc24b8b70u, 0xc76c51a3u,
        0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u,
        0x19a4c116u, 0x1e376c08u, 0x2748774cu, 0x34b0bcb5u,
        0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
        0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u,
        0x90befffau, 0xa4506cebu, 0xbef9a3f7u, 0xc67178f2u,
    };
    uint32_t words[64] = { 0 };
    for (int i = 0; i < 16; ++i) {
        const size_t offset = (size_t)i * 4u;
        words[i] = ((uint32_t)block[offset] << 24u)
                 | ((uint32_t)block[offset + 1u] << 16u)
                 | ((uint32_t)block[offset + 2u] << 8u)
                 | (uint32_t)block[offset + 3u];
    }
    for (int i = 16; i < 64; ++i) {
        const uint32_t x = words[i - 15];
        const uint32_t y = words[i - 2];
        const uint32_t s0 = dal_c__sha256RotateRight(x, 7u)
                          ^ dal_c__sha256RotateRight(x, 18u)
                          ^ (x >> 3u);
        const uint32_t s1 = dal_c__sha256RotateRight(y, 17u)
                          ^ dal_c__sha256RotateRight(y, 19u)
                          ^ (y >> 10u);
        words[i] = words[i - 16] + s0 + words[i - 7] + s1;
    }

    uint32_t a = ctx->state[0];
    uint32_t b = ctx->state[1];
    uint32_t c = ctx->state[2];
    uint32_t d = ctx->state[3];
    uint32_t e = ctx->state[4];
    uint32_t f = ctx->state[5];
    uint32_t g = ctx->state[6];
    uint32_t h = ctx->state[7];
    for (int i = 0; i < 64; ++i) {
        const uint32_t sum1 = dal_c__sha256RotateRight(e, 6u)
                            ^ dal_c__sha256RotateRight(e, 11u)
                            ^ dal_c__sha256RotateRight(e, 25u);
        const uint32_t choose = (e & f) ^ ((~e) & g);
        const uint32_t temp1 = h + sum1 + choose + constants[i] + words[i];
        const uint32_t sum0 = dal_c__sha256RotateRight(a, 2u)
                            ^ dal_c__sha256RotateRight(a, 13u)
                            ^ dal_c__sha256RotateRight(a, 22u);
        const uint32_t majority = (a & b) ^ (a & c) ^ (b & c);
        const uint32_t temp2 = sum0 + majority;
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }
    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
}

static void dal_c__sha256Init(dal_c_Sha256* ctx) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->state[0] = 0x6a09e667u;
    ctx->state[1] = 0xbb67ae85u;
    ctx->state[2] = 0x3c6ef372u;
    ctx->state[3] = 0xa54ff53au;
    ctx->state[4] = 0x510e527fu;
    ctx->state[5] = 0x9b05688cu;
    ctx->state[6] = 0x1f83d9abu;
    ctx->state[7] = 0x5be0cd19u;
}

static void dal_c__sha256Update(dal_c_Sha256* ctx, const unsigned char* data, size_t len) {
    if (!ctx || (!data && len != 0u)) { return; }
    ctx->bit_count += (uint64_t)len * 8u;
    while (len > 0u) {
        const size_t available = sizeof(ctx->block) - ctx->block_len;
        const size_t take = len < available ? len : available;
        memcpy(ctx->block + ctx->block_len, data, take);
        ctx->block_len += take;
        data += take;
        len -= take;
        if (ctx->block_len == sizeof(ctx->block)) {
            dal_c__sha256Transform(ctx, ctx->block);
            ctx->block_len = 0u;
        }
    }
}

static void dal_c__sha256Final(dal_c_Sha256* ctx, unsigned char digest[32]) {
    ctx->block[ctx->block_len++] = 0x80u;
    if (ctx->block_len > 56u) {
        while (ctx->block_len < sizeof(ctx->block)) { ctx->block[ctx->block_len++] = 0u; }
        dal_c__sha256Transform(ctx, ctx->block);
        ctx->block_len = 0u;
    }
    while (ctx->block_len < 56u) { ctx->block[ctx->block_len++] = 0u; }
    for (int i = 7; i >= 0; --i) {
        ctx->block[ctx->block_len++] = (unsigned char)(ctx->bit_count >> ((unsigned)i * 8u));
    }
    dal_c__sha256Transform(ctx, ctx->block);
    for (int i = 0; i < 8; ++i) {
        digest[(size_t)i * 4u] = (unsigned char)(ctx->state[i] >> 24u);
        digest[(size_t)i * 4u + 1u] = (unsigned char)(ctx->state[i] >> 16u);
        digest[(size_t)i * 4u + 2u] = (unsigned char)(ctx->state[i] >> 8u);
        digest[(size_t)i * 4u + 3u] = (unsigned char)ctx->state[i];
    }
}

char* dal_c__archiveHashFile(const char* path) {
    FILE* fp = path ? fopen(path, "rb") : NULL;
    if (!fp) { return NULL; }
    const size_t buffer_size = 16384u;
    unsigned char* buffer = (unsigned char*)malloc(buffer_size);
    if (!buffer) {
        (void)fclose(fp);
        return NULL;
    }
    dal_c_Sha256 ctx;
    dal_c__sha256Init(&ctx);
    size_t read_count = 0u;
    while ((read_count = fread(buffer, 1u, buffer_size, fp)) > 0u) {
        dal_c__sha256Update(&ctx, buffer, read_count);
    }
    if (ferror(fp) != 0) {
        free(buffer);
        (void)fclose(fp);
        return NULL;
    }
    free(buffer);
    (void)fclose(fp);
    unsigned char digest[32];
    dal_c__sha256Final(&ctx, digest);
    char* result = (char*)malloc(72u);
    if (!result) { return NULL; }
    memcpy(result, "sha256:", 7u);
    for (size_t i = 0; i < sizeof(digest); ++i) {
        static const char hex[] = "0123456789abcdef";
        result[7u + i * 2u] = hex[digest[i] >> 4u];
        result[8u + i * 2u] = hex[digest[i] & 0x0fu];
    }
    result[71] = '\0';
    return result;
}

static bool dal_c__archiveEndsWithIgnoreCase(const char* value, const char* suffix) {
    if (!value || !suffix) { return false; }
    size_t value_len = strcspn(value, "?#");
    const size_t suffix_len = strlen(suffix);
    if (value_len < suffix_len) { return false; }
    const char* tail = value + value_len - suffix_len;
    for (size_t i = 0; i < suffix_len; ++i) {
        char lhs = tail[i];
        char rhs = suffix[i];
        if (lhs >= 'A' && lhs <= 'Z') { lhs = (char)(lhs - 'A' + 'a'); }
        if (rhs >= 'A' && rhs <= 'Z') { rhs = (char)(rhs - 'A' + 'a'); }
        if (lhs != rhs) { return false; }
    }
    return true;
}

static const char* dal_c__archiveSuffix(const char* archive) {
    static const char* const suffixes[] = {
        ".tar.gz", ".tar.xz", ".tar.bz2", ".tar.zst",
        ".tgz", ".txz", ".tbz2", ".tzst", ".tar", ".zip", NULL,
    };
    for (int i = 0; suffixes[i]; ++i) {
        if (dal_c__archiveEndsWithIgnoreCase(archive, suffixes[i])) { return suffixes[i]; }
    }
    return NULL;
}

static bool dal_c__archiveDownload(const char* archive, const char* output) {
    if (path_isFile(archive)) { return file_copy(archive, output); }
    const char* curl[] = {
        "curl", "--fail", "--location", "--silent", "--show-error",
        "--output", output, "--", archive, NULL,
    };
    if (proc_run(curl, true) == 0 && path_isFile(output)) { return true; }
    (void)remove(output);
    const char* wget[] = { "wget", "--quiet", "--output-document", output, "--", archive, NULL };
    if (proc_run(wget, true) == 0 && path_isFile(output)) { return true; }
    (void)remove(output);
    return false;
}

static bool dal_c__archiveExtract(const char* archive, const char* destination, const char* suffix) {
    if (!dir_createRecur(destination)) { return false; }
    const char* tar[] = { "tar", "-xf", archive, "-C", destination, NULL };
    if (proc_run(tar, false) == 0) { return true; }
    if (str_eql(suffix, ".zip")) {
        if (!dir_removeRecur(destination) || !dir_createRecur(destination)) { return false; }
        const char* unzip[] = { "unzip", "-q", "-o", archive, "-d", destination, NULL };
        if (proc_run(unzip, false) == 0) { return true; }
    }
    return false;
}

static void dal_c__archiveFreeEntries(char** entries, int count) {
    if (!entries) { return; }
    for (int i = 0; i < count; ++i) { free(entries[i]); }
    free(entries);
}

static bool dal_c__archiveMovePath(const char* source, const char* destination) {
#ifdef _WIN32
    return MoveFileExA(source, destination, MOVEFILE_WRITE_THROUGH) != 0;
#else
    return rename(source, destination) == 0;
#endif
}

static bool dal_c__archiveReplaceDirectory(const char* materialized, const char* extraction_root, const char* destination) {
    char* backup = str_format("%s.previous", destination);
    if (!backup) { return false; }
    if (path_isDir(backup)) { (void)dir_removeRecur(backup); }
    else if (path_isFile(backup)) { (void)remove(backup); }

    bool moved_old = false;
    if (path_isDir(destination) || path_isFile(destination)) {
        if (!dal_c__archiveMovePath(destination, backup)) {
            free(backup);
            return false;
        }
        moved_old = true;
    }
    if (!dal_c__archiveMovePath(materialized, destination)) {
        if (moved_old) { (void)dal_c__archiveMovePath(backup, destination); }
        free(backup);
        return false;
    }
    if (moved_old) {
        if (path_isDir(backup)) { (void)dir_removeRecur(backup); }
        else { (void)remove(backup); }
    }
    if (!str_eql(materialized, extraction_root) && path_isDir(extraction_root)) {
        (void)dir_removeRecur(extraction_root);
    }
    free(backup);
    return true;
}

char* dal_c__archiveReadRevision(const char* source_dir) {
    if (!source_dir || !path_isDir(source_dir)) { return NULL; }
    char* marker = path_join(source_dir, DAL_C_ARCHIVE_MARKER);
    char* text = marker ? file_read(marker) : NULL;
    free(marker);
    if (!text) { return NULL; }
    char* end = strpbrk(text, "\r\n");
    if (end) { *end = '\0'; }
    if (!str_startsWith(text, "sha256:") || strlen(text) != 71u) {
        free(text);
        return NULL;
    }
    return text;
}

bool dal_c__archiveMaterializationMatches(const char* source_dir, const char* locked_revision, char** reason_out) {
    if (reason_out) { *reason_out = NULL; }
    char* actual = dal_c__archiveReadRevision(source_dir);
    const bool ok = actual && locked_revision && str_eql(actual, locked_revision);
    if (!ok && reason_out) {
        *reason_out = str_format(
            "archive materialization does not match lock.dh: materialized=%s locked=%s",
            actual ? actual : "(missing)", locked_revision ? locked_revision : "(missing)"
        );
    }
    free(actual);
    return ok;
}

bool dal_c__archiveAcquire(
    const char* archive,
    const char* source_dir,
    const char* expected_revision,
    char** resolved_revision_out,
    char** reason_out
) {
    if (resolved_revision_out) { *resolved_revision_out = NULL; }
    if (reason_out) { *reason_out = NULL; }
    const char* suffix = dal_c__archiveSuffix(archive);
    if (!archive || !source_dir || !suffix) {
        if (reason_out) {
            *reason_out = strdup("unsupported archive format; expected .zip, .tar, .tar.gz, .tgz, .tar.xz, .txz, .tar.bz2, .tbz2, .tar.zst, or .tzst");
        }
        return false;
    }

    char* download_path = str_format("%s.download%s", source_dir, suffix);
    char* extraction_root = str_format("%s.extract", source_dir);
    if (!download_path || !extraction_root) {
        free(extraction_root);
        free(download_path);
        return false;
    }
    if (path_isFile(download_path)) { (void)remove(download_path); }
    if (path_isDir(extraction_root)) { (void)dir_removeRecur(extraction_root); }

    bool ok = false;
    if (!dal_c__archiveDownload(archive, download_path)) {
        if (reason_out) { *reason_out = str_format("failed to download archive `%s`; install curl or wget", archive); }
        goto cleanup;
    }
    char* revision = dal_c__archiveHashFile(download_path);
    if (!revision) {
        if (reason_out) { *reason_out = strdup("failed to calculate archive SHA-256"); }
        goto cleanup;
    }
    if (expected_revision && !str_eql(revision, expected_revision)) {
        if (reason_out) {
            *reason_out = str_format(
                "downloaded archive differs from lock.dh: downloaded=%s locked=%s",
                revision, expected_revision
            );
        }
        free(revision);
        goto cleanup;
    }
    if (!dal_c__archiveExtract(download_path, extraction_root, suffix)) {
        if (reason_out) {
            *reason_out = str_format(
                "failed to extract `%s`; tar is required, and .zip also accepts unzip as a fallback",
                archive
            );
        }
        free(revision);
        goto cleanup;
    }

    int entry_count = 0;
    char** entries = dir_listEntries(extraction_root, &entry_count);
    if (entry_count == 0) {
        if (reason_out) { *reason_out = strdup("archive extracted no files"); }
        dal_c__archiveFreeEntries(entries, entry_count);
        free(revision);
        goto cleanup;
    }
    const char* materialized = extraction_root;
    if (entry_count == 1 && path_isDir(entries[0])) { materialized = entries[0]; }
    char* marker = path_join(materialized, DAL_C_ARCHIVE_MARKER);
    char* marker_text = str_format("%s\n", revision);
    if (!marker || !marker_text || !file_writeAtomic(marker, marker_text)
        || !dal_c__archiveReplaceDirectory(materialized, extraction_root, source_dir)) {
        if (reason_out) { *reason_out = strdup("failed to materialize extracted archive"); }
        free(marker_text);
        free(marker);
        dal_c__archiveFreeEntries(entries, entry_count);
        free(revision);
        goto cleanup;
    }
    free(marker_text);
    free(marker);
    dal_c__archiveFreeEntries(entries, entry_count);
    if (resolved_revision_out) { *resolved_revision_out = revision; }
    else { free(revision); }
    ok = true;

cleanup:
    if (path_isFile(download_path)) { (void)remove(download_path); }
    if (!ok && path_isDir(extraction_root)) { (void)dir_removeRecur(extraction_root); }
    free(extraction_root);
    free(download_path);
    return ok;
}
