#include "dh/fs/File/MemMap.h"

#if plat_is_windows
#include "dh/sys/api/windows/file.h"
#elif plat_is_linux
#include "dh/sys/call/linux.h"
#endif

#if plat_is_windows
$static fn_((fs__File_MemMap_windowsPageProtect(fs_File_MemMap_Protection protection))(DWORD)) {
    if (protection.execute) {
        return protection.write ? PAGE_EXECUTE_READWRITE : PAGE_EXECUTE_READ;
    }
    return protection.write ? PAGE_READWRITE : PAGE_READONLY;
}

$static fn_((fs__File_MemMap_windowsViewAccess(fs_File_MemMap_Protection protection))(DWORD)) {
    DWORD access = 0;
    if (protection.read) access |= FILE_MAP_READ;
    if (protection.write) access |= FILE_MAP_WRITE;
    if (protection.execute) access |= FILE_MAP_EXECUTE;
    return access == 0 ? FILE_MAP_READ : access;
}
#elif plat_is_linux
$static fn_((fs__File_MemMap_linuxProtect(fs_File_MemMap_Protection protection))(sys_call_linux_word)) {
    sys_call_linux_word prot = 0;
    if (protection.read) prot |= sys_call_linux_PROT_READ;
    if (protection.write) prot |= sys_call_linux_PROT_WRITE;
    if (protection.execute) prot |= sys_call_linux_PROT_EXEC;
    return prot == 0 ? sys_call_linux_PROT_NONE : prot;
}
#endif

$static fn_((fs__File_MemMap_unmap(fs_File_MemMap* self))(void)) {
    if (self->mem.ptr == null || self->mem.len == 0) return;
#if plat_is_windows
    let_ignore = UnmapViewOfFile(self->mem.ptr);
    if (self->mapping != null) {
        CloseHandle(self->mapping);
        self->mapping = null;
    }
#elif plat_is_linux
    let_ignore = sys_call_linux_munmap(self->mem.ptr, self->mem.len);
#endif
    self->mem = l$((S$u8)cleared());
}

$static fn_((fs__File_MemMap_remap(fs_File_MemMap* self, usize len))(fs_E$void) $guard) {
    if (len == 0) {
        fs__File_MemMap_unmap(self);
        return_ok({});
    }
#if plat_is_windows
    let end = self->offset + len;
    let mapping = CreateFileMappingA(
        self->file.handle,
        null,
        fs__File_MemMap_windowsPageProtect(self->protection),
        as$(DWORD)(end >> 32),
        as$(DWORD)(end & 0xFFFFFFFFu),
        null
    );
    if (mapping == null) return_err(E_cause$fs_Unsupported());
    errdefer_($ignore, CloseHandle(mapping));
    let view = as$(u8*)(MapViewOfFile(
        mapping,
        fs__File_MemMap_windowsViewAccess(self->protection),
        as$(DWORD)(self->offset >> 32),
        as$(DWORD)(self->offset & 0xFFFFFFFFu),
        len
    ));
    if (view == null) return_err(E_cause$fs_Unsupported());
    self->mapping = mapping;
    self->mem = P_prefix$((S$u8)(view)(len));
#elif plat_is_linux
    let mapped = sys_call_linux_mmap(
        null,
        len,
        fs__File_MemMap_linuxProtect(self->protection),
        sys_call_linux_MAP_SHARED,
        self->file.handle,
        self->offset
    );
    if (sys_call_linux_syscall_isErr(mapped)) return_err(E_cause$fs_Unsupported());
    self->mem = P_prefix$((S$u8)(intToPtr$((P$raw)(mapped)))(len));
#else
    let_ignore = self;
    let_ignore = len;
    return_err(E_cause$fs_Unsupported());
#endif
    return_ok({});
} $unguarded(fn);

fn_((fs_File_MemMap_init(
    fs_File file,
    usize len,
    fs_File_MemMap_Protection protection,
    bool undefined_contents,
    bool populate,
    u64 offset
))(fs_E$fs_File_MemMap) $scope) {
    let_ignore = undefined_contents;
    let_ignore = populate;
    var_(self, fs_File_MemMap) = {
        .file = file,
        .offset = offset,
        .mem = cleared(),
        .protection = protection,
        .mapping = null,
    };

    if (protection.write && len > 0) {
        let end = offset + len;
        let stat = try_(fs_File_stat(file));
        if (stat.size < end) try_(fs_File_setLen(file, end));
    }

    try_(fs__File_MemMap_remap(&self, len));
    return_ok(self);
} $unscoped(fn);

fn_((fs_File_MemMap_fini(fs_File_MemMap* self))(void)) {
    if (!isNonnull(self)) return;
    fs__File_MemMap_unmap(self);
};

fn_((fs_File_MemMap_setLen(fs_File_MemMap* self, usize new_len))(fs_E$void) $scope) {
    claim_assert_nonnull(self);
    fs__File_MemMap_unmap(self);
    try_(fs_File_setLen(self->file, self->offset + new_len));
    try_(fs__File_MemMap_remap(self, new_len));
    return_ok({});
} $unscoped(fn);

fn_((fs_File_MemMap_read(fs_File_MemMap* self))(fs_E$void) $scope) {
    claim_assert_nonnull(self);
#if plat_is_windows
    return_ok({});
#elif plat_is_linux
    if (self->mem.ptr != null && self->mem.len > 0) {
        if (sys_call_linux_msync(self->mem.ptr, self->mem.len, sys_call_linux_MS_INVALIDATE) != 0) return_err(E_cause$fs_ReadFailed());
    }
    return_ok({});
#else
    let_ignore = self;
    return_err(E_cause$fs_Unsupported());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_File_MemMap_write(fs_File_MemMap* self))(fs_E$void) $scope) {
    claim_assert_nonnull(self);
#if plat_is_windows
    if (self->mem.ptr != null && self->mem.len > 0 && !FlushViewOfFile(self->mem.ptr, self->mem.len)) {
        return_err(E_cause$fs_WriteFailed());
    }
#elif plat_is_linux
    if (self->mem.ptr != null && self->mem.len > 0) {
        if (sys_call_linux_msync(self->mem.ptr, self->mem.len, sys_call_linux_MS_SYNC) != 0) return_err(E_cause$fs_WriteFailed());
    }
#else
    let_ignore = self;
    return_err(E_cause$fs_Unsupported());
#endif
    return_ok({});
} $unscoped(fn);
