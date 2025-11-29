#include "dh/prl/types/Err.h"
#include "dh/mem/common.h"
#include "dh/io/common.h"
#include "dh/fs/File.h"

fn_((Err_domainToStr(Err self))(S_const$u8)) {
    claim_assert(self.ctx != 0);
    claim_assert_nonnull(self.vt);
    return mem_spanZ0$u8(Err_domainToStrZ(self));
}

fn_((Err_codeToStr(Err self))(S_const$u8)) {
    claim_assert(self.ctx != 0);
    claim_assert_nonnull(self.vt);
    return mem_spanZ0$u8(Err_codeToStrZ(self));
}

fn_((Err_print(Err self))(void)) {
    claim_assert(self.ctx != 0);
    claim_assert_nonnull(self.vt);
    let out = fs_File_writer(io_getStdErr());
    catch_((io_Writer_println(
        out, u8_l("Program failed: {:e}"), self
    ))($ignore, claim_unreachable));
}
