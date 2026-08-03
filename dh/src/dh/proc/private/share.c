#include "share.h"
#include "dh/io/Fixed.h"
#include "dh/mem/common.h"
#include "dh/unicode.h"

/*========== Internal Declarations ==========================================*/

$attr($must_check)
$static fn_((proc__cmdLineAppendQuoted(io_Writer out, S_const$u8 arg))(io_WriteE$void));

/*========== External Definitions ===========================================*/

fn_((proc__growScratch(S$u8* self, mem_Alctr gpa))(mem_E$void) $scope) {
    claim_assert_nonnull(self);
    let new_len = orelse_((usize_mulChkd(self->len, usize_(2)))(return_err(E_cause$OutOfMemory())));
    asg_l((self)(try_(mem_Alctr_reallocBytes($trace gpa, *self, new_len))));
    return_ok({});
} $unscoped(fn);

fn_((proc__cmdLineWTF16(mem_Alctr gpa, S$S_const$u8 argv))(proc__cmdLineWTF16_E$S$u16) $guard) {
    if (argv.len == 0) return_err(E_cause$proc_InvalidName());

    var_(cap, usize) = 0;
    for_(($rf(0), $s(argv))(i, arg)) {
        let expanded = orelse_((usize_mulChkd(arg->len, usize_(2)))(
            return_err(E_cause$proc_InvalidName())
        ));
        cap = orelse_((usize_addChkd(cap, expanded))(
            return_err(E_cause$proc_InvalidName())
        ));
        cap = orelse_((usize_addChkd(cap, i == 0 ? usize_(2) : usize_(3)))(
            return_err(E_cause$proc_InvalidName())
        ));
    } $end(for);

    let utf8_mem = try_(mem_Alctr_allocBytes($trace gpa, cap));
    defer_(mem_Alctr_freeBytes($trace gpa, utf8_mem));
    var fixed = io_Fixed_Writer_from(io_Fixed_writing(utf8_mem));
    for_(($rf(0), $s(argv))(i, arg)) {
        if (i != 0) catch_((io_Writer_writeByte(io_Fixed_writer(&fixed), u8_c(' ')))(
            $ignore, return_err(E_cause$proc_InvalidName())
        ));
        catch_((proc__cmdLineAppendQuoted(io_Fixed_writer(&fixed), *arg))(
            $ignore, return_err(E_cause$proc_InvalidName())
        ));
    } $end(for);

    let utf8 = io_Fixed_written(fixed.stream).as_const;
    let wide_len = unicode_wtf8ToWTF16Len(utf8);
    let alloc_len = orelse_((usize_addChkd(wide_len, usize_(1)))(
        return_err(E_cause$OutOfMemory())
    ));
    let wide = u_castS$((S$u16)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(u16), alloc_len))));
    errdefer_($ignore, mem_Alctr_free($trace gpa, u_anyS(wide)));
    let converted = catch_((unicode_wtf8ToWTF16Within(utf8, S_prefix((wide)(wide_len))))(
        $ignore, return_err(E_cause$proc_InvalidName())
    ));
    claim_assert(converted.len == wide_len);
    *S_at((wide)[wide_len]) = 0;
    return_ok(wide);
} $unguarded(fn);

fn_((proc__envWTF16(mem_Alctr gpa, proc_Env inherited, O$proc_cmd_Env override))(proc__envWTF16_E$S$u16) $guard) {
    var items = try_(proc_NativeStrList_fromEnv(gpa, inherited, override));
    defer_(proc_NativeStrList_fini(&items, gpa));

    let item_count = proc_NativeStrList_len(items);
    var_(len, usize) = item_count == 0 ? usize_(2) : usize_(1);
    for_(($rt(item_count))(i)) {
        let item_len = unicode_wtf8ToWTF16Len(proc_NativeStrList_at(items, i));
        len = orelse_((usize_addChkd(
            len,
            orelse_((usize_addChkd(item_len, usize_(1)))(
                return_err(E_cause$OutOfMemory())
            ))
        ))(return_err(E_cause$OutOfMemory())));
    } $end(for);

    let bytes = u_castS$((S$u16)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(u16), len))));
    errdefer_($ignore, mem_Alctr_free($trace gpa, u_anyS(bytes)));

    var_(pos, usize) = 0;
    for_(($rt(item_count))(i)) {
        let item = proc_NativeStrList_at(items, i);
        let item_len = unicode_wtf8ToWTF16Len(item);
        let end = orelse_((usize_addChkd(pos, item_len))(return_err(E_cause$OutOfMemory())));
        let converted = catch_((unicode_wtf8ToWTF16Within(item, S_slice((bytes)$r(pos, end))))(
            $ignore, return_err(E_cause$proc_InvalidName())
        ));
        claim_assert(converted.len == item_len);
        pos = end;
        *S_at((bytes)[pos++]) = 0;
    } $end(for);

    if (item_count == 0) *S_at((bytes)[pos++]) = 0;
    *S_at((bytes)[pos++]) = 0;
    claim_assert(pos == len);
    return_ok(bytes);
} $unguarded(fn);

/*========== Internal Definitions ===========================================*/

fn_((proc__cmdLineAppendQuoted(io_Writer out, S_const$u8 arg))(io_WriteE$void) $scope) {
    let needs_quote = arg.len == 0
                   || isSome(mem_findFirstAnyBytes(arg, u8_l(" \t\"")));
    if (!needs_quote) return_(io_Writer_writeBytes(out, arg));

    try_(io_Writer_writeByte(out, u8_c('"')));
    var_(slash_count, usize) = 0;
    for_(($s(arg))(ch)) {
        if (*ch == u8_c('\\')) {
            ++slash_count;
            continue;
        }
        try_(io_Writer_writeByteN(
            out, u8_c('\\'),
            *ch == u8_c('"') ? slash_count * 2 + 1 : slash_count
        ));
        try_(io_Writer_writeByte(out, *ch));
        slash_count = 0;
    } $end(for);
    try_(io_Writer_writeByteN(out, u8_c('\\'), slash_count * 2));
    try_(io_Writer_writeByte(out, u8_c('"')));
    return_ok({});
} $unscoped(fn);
