#include "dh/fs/File/MemMap.h"

#if plat_is_windows
#include "dh/os/windows/file.h"
#include <malloc.h>
#else
#include <malloc.h>
#include <sys/mman.h>
#endif

T_alias$((fs__File_MemMap_Section)(struct fs__File_MemMap_Section {
    var_(protection, fs_File_MemMap_Protection);
#if plat_is_windows
    var_(mapping, HANDLE);
#endif
}));

$static fn_((fs__File_MemMap_sectionAlloc(fs_File_MemMap_Protection protection))(fs__File_MemMap_Section*)) {
    let section = as$(fs__File_MemMap_Section*)(malloc(sizeOf$(fs__File_MemMap_Section)));
    if (section == null) return null;
    *section = (fs__File_MemMap_Section){
        .protection = protection,
#if plat_is_windows
        .mapping = null,
#endif
    };
    return section;
}

$static fn_((fs__File_MemMap_sectionFree(fs__File_MemMap_Section* section))(void)) {
    if (section != null) free(section);
}

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
#else
$static fn_((fs__File_MemMap_posixProtect(fs_File_MemMap_Protection protection))(i32)) {
    i32 prot = 0;
    if (protection.read) prot |= PROT_READ;
    if (protection.write) prot |= PROT_WRITE;
    if (protection.execute) prot |= PROT_EXEC;
    return prot == 0 ? PROT_NONE : prot;
}
#endif

$static fn_((fs__File_MemMap_unmap(fs_File_MemMap* self))(void)) {
    if (self->mem.ptr == null || self->mem.len == 0) return;
#if plat_is_windows
    let section = as$(fs__File_MemMap_Section*)(self->section);
    let_ignore = UnmapViewOfFile(self->mem.ptr);
    if (section != null && section->mapping != null) {
        CloseHandle(section->mapping);
        section->mapping = null;
    }
#else
    let_ignore = munmap(self->mem.ptr, self->mem.len);
#endif
    self->mem = (S$u8){ .ptr = null, .len = 0 };
}

$static fn_((fs__File_MemMap_remap(fs_File_MemMap* self, usize len))(fs_E$void) $scope) {
    let section = as$(fs__File_MemMap_Section*)(ensureNonnull(self->section));
    if (len == 0) {
        fs__File_MemMap_unmap(self);
        return_ok({});
    }
#if plat_is_windows
    let end = self->offset + len;
    let mapping = CreateFileMappingA(
        self->file.handle,
        null,
        fs__File_MemMap_windowsPageProtect(section->protection),
        as$(DWORD)(end >> 32),
        as$(DWORD)(end & 0xFFFFFFFFu),
        null
    );
    if (mapping == null) return_err(E_cause$UnsupportedFS());
    let view = as$(u8*)(MapViewOfFile(
        mapping,
        fs__File_MemMap_windowsViewAccess(section->protection),
        as$(DWORD)(self->offset >> 32),
        as$(DWORD)(self->offset & 0xFFFFFFFFu),
        len
    ));
    if (view == null) {
        CloseHandle(mapping);
        return_err(E_cause$UnsupportedFS());
    }
    section->mapping = mapping;
    self->mem = (S$u8){ .ptr = view, .len = len };
#else
    let view = as$(u8*)(mmap(
        null,
        len,
        fs__File_MemMap_posixProtect(section->protection),
        MAP_SHARED,
        self->file.handle,
        as$(off_t)(self->offset)
    ));
    if (view == MAP_FAILED) return_err(E_cause$UnsupportedFS());
    self->mem = (S$u8){ .ptr = view, .len = len };
#endif
    return_ok({});
} $unscoped(fn);

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
        .mem = { .ptr = null, .len = 0 },
        .section = null,
    };
    let section = fs__File_MemMap_sectionAlloc(protection);
    if (section == null) return_err(E_cause$NoSpaceLeftFS());
    self.section = section;

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
    fs__File_MemMap_sectionFree(as$(fs__File_MemMap_Section*)(self->section));
    self->section = null;
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
#else
    if (self->mem.ptr != null && self->mem.len > 0) {
        if (msync(self->mem.ptr, self->mem.len, MS_INVALIDATE) != 0) return_err(E_cause$ReadFailedFS());
    }
    return_ok({});
#endif
} $unscoped(fn);

fn_((fs_File_MemMap_write(fs_File_MemMap* self))(fs_E$void) $scope) {
    claim_assert_nonnull(self);
#if plat_is_windows
    if (self->mem.ptr != null && self->mem.len > 0 && !FlushViewOfFile(self->mem.ptr, self->mem.len)) {
        return_err(E_cause$WriteFailedFS());
    }
#else
    if (self->mem.ptr != null && self->mem.len > 0) {
        if (msync(self->mem.ptr, self->mem.len, MS_SYNC) != 0) return_err(E_cause$WriteFailedFS());
    }
#endif
    return_ok({});
} $unscoped(fn);
