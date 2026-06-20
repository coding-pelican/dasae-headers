#include "dansi-xterm/selection.h"
#include <dh/io/Fixed.h>

$static fn_((dansi_xterm_selection__rawWrite(
    u16 cmd, S_const$u8 targets, S_const$u8 data, io_Writer out
))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l("\x1b]")));
    try_(io_Writer_print(out, u8_l("{:uh};"), cmd));
    try_(io_Writer_writeBytes(out, targets));
    try_(io_Writer_writeByte(out, u8_c(';')));
    try_(io_Writer_writeBytes(out, data));
    return dansi_Seq_EOS_write(dansi_Seq_EOS_st_7bit, out);
} $unscoped(fn);

$static fn_((dansi_xterm_selection__targetsRaw(dansi_xterm_selection_Targets targets, S$u8 raw))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(raw));
    if (targets.clipboard) try_(io_Writer_writeByte(io_Fixed_writer(&writing), u8_c('c')));
    if (targets.primary) try_(io_Writer_writeByte(io_Fixed_writer(&writing), u8_c('p')));
    if (targets.secondary) try_(io_Writer_writeByte(io_Fixed_writer(&writing), u8_c('q')));
    if (targets.select) try_(io_Writer_writeByte(io_Fixed_writer(&writing), u8_c('s')));
    if (targets.cut0) try_(io_Writer_writeByte(io_Fixed_writer(&writing), u8_c('0')));
    if (targets.cut1) try_(io_Writer_writeByte(io_Fixed_writer(&writing), u8_c('1')));
    if (targets.cut2) try_(io_Writer_writeByte(io_Fixed_writer(&writing), u8_c('2')));
    if (targets.cut3) try_(io_Writer_writeByte(io_Fixed_writer(&writing), u8_c('3')));
    if (targets.cut4) try_(io_Writer_writeByte(io_Fixed_writer(&writing), u8_c('4')));
    if (targets.cut5) try_(io_Writer_writeByte(io_Fixed_writer(&writing), u8_c('5')));
    if (targets.cut6) try_(io_Writer_writeByte(io_Fixed_writer(&writing), u8_c('6')));
    if (targets.cut7) try_(io_Writer_writeByte(io_Fixed_writer(&writing), u8_c('7')));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_xterm_selection_targetsToRaw(
    dansi_xterm_selection_Targets targets, S$u8 buf
))(E$S$u8)) {
    return dansi_xterm_selection__targetsRaw(targets, buf);
};

fn_((dansi_xterm_selection_set(
    dansi_xterm_selection_Targets targets, S_const$u8 data_base64, S$u8 buf
))(E$S$u8) $scope) {
    var raw_buf = (A$$(12, u8)){};
    let raw = try_(dansi_xterm_selection_targetsToRaw(targets, A_ref$((S$u8)(raw_buf))));
    return dansi_xterm_selection_setRaw(raw.as_const, data_base64, buf);
} $unscoped(fn);

fn_((dansi_xterm_selection_setWrite(
    dansi_xterm_selection_Targets targets, S_const$u8 data_base64, io_Writer out
))(E$void) $scope) {
    var raw_buf = (A$$(12, u8)){};
    let raw = try_(dansi_xterm_selection_targetsToRaw(targets, A_ref$((S$u8)(raw_buf))));
    return dansi_xterm_selection_setRawWrite(raw.as_const, data_base64, out);
} $unscoped(fn);

fn_((dansi_xterm_selection_query(
    dansi_xterm_selection_Targets targets, S$u8 buf
))(E$S$u8) $scope) {
    var raw_buf = (A$$(12, u8)){};
    let raw = try_(dansi_xterm_selection_targetsToRaw(targets, A_ref$((S$u8)(raw_buf))));
    return dansi_xterm_selection_queryRaw(raw.as_const, buf);
} $unscoped(fn);

fn_((dansi_xterm_selection_queryWrite(
    dansi_xterm_selection_Targets targets, io_Writer out
))(E$void) $scope) {
    var raw_buf = (A$$(12, u8)){};
    let raw = try_(dansi_xterm_selection_targetsToRaw(targets, A_ref$((S$u8)(raw_buf))));
    return dansi_xterm_selection_queryRawWrite(raw.as_const, out);
} $unscoped(fn);

fn_((dansi_xterm_selection_clear(
    dansi_xterm_selection_Targets targets, S$u8 buf
))(E$S$u8) $scope) {
    var raw_buf = (A$$(12, u8)){};
    let raw = try_(dansi_xterm_selection_targetsToRaw(targets, A_ref$((S$u8)(raw_buf))));
    return dansi_xterm_selection_clearRaw(raw.as_const, buf);
} $unscoped(fn);

fn_((dansi_xterm_selection_clearWrite(
    dansi_xterm_selection_Targets targets, io_Writer out
))(E$void) $scope) {
    var raw_buf = (A$$(12, u8)){};
    let raw = try_(dansi_xterm_selection_targetsToRaw(targets, A_ref$((S$u8)(raw_buf))));
    return dansi_xterm_selection_clearRawWrite(raw.as_const, out);
} $unscoped(fn);

fn_((dansi_xterm_selection_setRaw(S_const$u8 targets, S_const$u8 data_base64, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_xterm_selection_setRawWrite(targets, data_base64, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_xterm_selection_setRawWrite(S_const$u8 targets, S_const$u8 data_base64, io_Writer out))(E$void)) {
    return dansi_xterm_selection__rawWrite(52, targets, data_base64, out);
};

fn_((dansi_xterm_selection_queryRaw(S_const$u8 targets, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_xterm_selection_queryRawWrite(targets, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_xterm_selection_queryRawWrite(S_const$u8 targets, io_Writer out))(E$void)) {
    return dansi_xterm_selection__rawWrite(52, targets, u8_l("?"), out);
};

fn_((dansi_xterm_selection_clearRaw(S_const$u8 targets, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_xterm_selection_clearRawWrite(targets, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_xterm_selection_clearRawWrite(S_const$u8 targets, io_Writer out))(E$void)) {
    return dansi_xterm_selection__rawWrite(52, targets, u8_l(""), out);
};
