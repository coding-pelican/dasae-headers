#include "Atom.h"
#include "../Self.h"

fn_((fs_File_Atom_fini(fs_File_Atom* self, fs_Self fs))(void)) {
    if (!isNonnull(self)) return;
    if (self->file_open) {
        fs_File_close(self->file, fs);
        self->file_open = false;
    }
    if (self->close_dir_on_fini) {
        fs_Dir_close(&self->dir, fs);
        self->close_dir_on_fini = false;
    }
};

fn_((fs_File_Atom_link(fs_File_Atom* self, fs_Self fs))(E$void) $scope) {
    claim_assert_nonnull(self);
    let_ignore = fs;
    return_err(fs_E_Unsupported());
} $unscoped(fn);

fn_((fs_File_Atom_replace(fs_File_Atom* self, fs_Self fs))(E$void) $scope) {
    claim_assert_nonnull(self);
    let_ignore = fs;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
