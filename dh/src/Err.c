#include "dh/Err.h"
#include "dh/Str.h"
#include "dh/io/common.h"
#include "dh/fs/File.h"
#include "dh/io/Writer.h"

fn_((Err_domainToStr(Err self))(Sli_const$u8)) {
    return Str_viewZ(as$((const u8*)(self.vt->domainToCStr(self.ctx))));
}

fn_((Err_codeToStr(Err self))(Sli_const$u8)) {
    return Str_viewZ(as$((const u8*)(self.vt->codeToCStr(self.ctx))));
}

fn_((Err_print(Err self))(void)) {
    let domain = Err_domainToStr(self);
    let code   = Err_codeToStr(self);

    let out = fs_File_writer(io_getStdErr());
    catch_((io_Writer_print(
        out, u8_l("Program failed: [{:s}] {:s}({:d})\n"),
        domain, code, self.ctx
    ))($ignore, claim_unreachable));
}
