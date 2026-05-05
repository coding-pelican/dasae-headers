#include "MemMap.h"
#include "../Self.h"

fn_((fs_File_MemMap_init(
    fs_File file,
    fs_Self fs,
    usize len,
    fs_File_MemMap_Protection protection,
    bool undefined_contents,
    bool populate,
    u64 offset
))(fs_E$fs_File_MemMap) $scope) {
    let_ignore = file;
    let_ignore = fs;
    let_ignore = len;
    let_ignore = protection;
    let_ignore = undefined_contents;
    let_ignore = populate;
    let_ignore = offset;
    return_err(fs_E_Unsupported());
} $unscoped(fn);

fn_((fs_File_MemMap_fini(fs_File_MemMap* self, fs_Self fs))(void)) {
    let_ignore = self;
    let_ignore = fs;
};

fn_((fs_File_MemMap_setLen(fs_File_MemMap* self, fs_Self fs, usize new_len))(fs_E$void) $scope) {
    let_ignore = self;
    let_ignore = fs;
    let_ignore = new_len;
    return_err(fs_E_Unsupported());
} $unscoped(fn);

fn_((fs_File_MemMap_read(fs_File_MemMap* self, fs_Self fs))(fs_E$void) $scope) {
    let_ignore = self;
    let_ignore = fs;
    return_err(fs_E_Unsupported());
} $unscoped(fn);

fn_((fs_File_MemMap_write(fs_File_MemMap* self, fs_Self fs))(fs_E$void) $scope) {
    let_ignore = self;
    let_ignore = fs;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
