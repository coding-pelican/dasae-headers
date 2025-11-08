#include "dh/prl/types/Err.h"
#include "dh/mem/common.h"
#include "dh/io/common.h"
#include "dh/fs/File.h"

fn_((Err_domainToStr(Err self))(S_const$u8)) {
    return mem_spanZ0_const$u8(as$((const u8*)(self.vt->domainToCStr(self.ctx))));
}

fn_((Err_codeToStr(Err self))(S_const$u8)) {
    return mem_spanZ0_const$u8(as$((const u8*)(self.vt->codeToCStr(self.ctx))));
}

fn_((Err_print(Err self))(void)) {
    let out = fs_File_writer(io_getStdErr());
    catch_((io_Writer_println(
        out, u8_l("Program failed: {:e}"), self
    ))($ignore, claim_unreachable));
}
