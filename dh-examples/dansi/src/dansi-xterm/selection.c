#include "dansi-xterm/selection.h"
#include <dh/io/Fixed.h>

$static fn_((dansi_xterm_selection__rawWrite(
    u16 cmd, S_const$u8 targets, S_const$u8 data, io_Writer out
))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_osc_7bit_prefix)));
    try_(io_Writer_print(out, u8_l("{:uh}" dansi_osc_cmd_sep), cmd));
    try_(io_Writer_writeBytes(out, targets));
    try_(io_Writer_writeByte(out, dansi_xterm_selection_sep_byte));
    try_(io_Writer_writeBytes(out, data));
    return dansi_Seq_EOS_write(dansi_Seq_EOS_st_7bit, out);
} $unscoped(fn);

$static fn_((dansi_xterm_selection__targetsRaw(dansi_xterm_selection_Targets targets, S$u8 raw))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(raw));
    if (targets.clipboard) try_(io_Writer_writeByte(io_Fixed_writer(&writing), dansi_xterm_selection_target_clipboard_byte));
    if (targets.primary) try_(io_Writer_writeByte(io_Fixed_writer(&writing), dansi_xterm_selection_target_primary_byte));
    if (targets.secondary) try_(io_Writer_writeByte(io_Fixed_writer(&writing), dansi_xterm_selection_target_secondary_byte));
    if (targets.select) try_(io_Writer_writeByte(io_Fixed_writer(&writing), dansi_xterm_selection_target_select_byte));
    if (targets.cut0) try_(io_Writer_writeByte(io_Fixed_writer(&writing), dansi_xterm_selection_target_cut0_byte));
    if (targets.cut1) try_(io_Writer_writeByte(io_Fixed_writer(&writing), dansi_xterm_selection_target_cut1_byte));
    if (targets.cut2) try_(io_Writer_writeByte(io_Fixed_writer(&writing), dansi_xterm_selection_target_cut2_byte));
    if (targets.cut3) try_(io_Writer_writeByte(io_Fixed_writer(&writing), dansi_xterm_selection_target_cut3_byte));
    if (targets.cut4) try_(io_Writer_writeByte(io_Fixed_writer(&writing), dansi_xterm_selection_target_cut4_byte));
    if (targets.cut5) try_(io_Writer_writeByte(io_Fixed_writer(&writing), dansi_xterm_selection_target_cut5_byte));
    if (targets.cut6) try_(io_Writer_writeByte(io_Fixed_writer(&writing), dansi_xterm_selection_target_cut6_byte));
    if (targets.cut7) try_(io_Writer_writeByte(io_Fixed_writer(&writing), dansi_xterm_selection_target_cut7_byte));
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
    var raw_buf = (A$$(dansi_xterm_selection_target_count, u8)){};
    let raw = try_(dansi_xterm_selection_targetsToRaw(targets, A_ref$((S$u8)(raw_buf))));
    return dansi_xterm_selection_setRaw(raw.as_const, data_base64, buf);
} $unscoped(fn);

fn_((dansi_xterm_selection_setWrite(
    dansi_xterm_selection_Targets targets, S_const$u8 data_base64, io_Writer out
))(E$void) $scope) {
    var raw_buf = (A$$(dansi_xterm_selection_target_count, u8)){};
    let raw = try_(dansi_xterm_selection_targetsToRaw(targets, A_ref$((S$u8)(raw_buf))));
    return dansi_xterm_selection_setRawWrite(raw.as_const, data_base64, out);
} $unscoped(fn);

fn_((dansi_xterm_selection_query(
    dansi_xterm_selection_Targets targets, S$u8 buf
))(E$S$u8) $scope) {
    var raw_buf = (A$$(dansi_xterm_selection_target_count, u8)){};
    let raw = try_(dansi_xterm_selection_targetsToRaw(targets, A_ref$((S$u8)(raw_buf))));
    return dansi_xterm_selection_queryRaw(raw.as_const, buf);
} $unscoped(fn);

fn_((dansi_xterm_selection_queryWrite(
    dansi_xterm_selection_Targets targets, io_Writer out
))(E$void) $scope) {
    var raw_buf = (A$$(dansi_xterm_selection_target_count, u8)){};
    let raw = try_(dansi_xterm_selection_targetsToRaw(targets, A_ref$((S$u8)(raw_buf))));
    return dansi_xterm_selection_queryRawWrite(raw.as_const, out);
} $unscoped(fn);

fn_((dansi_xterm_selection_clear(
    dansi_xterm_selection_Targets targets, S$u8 buf
))(E$S$u8) $scope) {
    var raw_buf = (A$$(dansi_xterm_selection_target_count, u8)){};
    let raw = try_(dansi_xterm_selection_targetsToRaw(targets, A_ref$((S$u8)(raw_buf))));
    return dansi_xterm_selection_clearRaw(raw.as_const, buf);
} $unscoped(fn);

fn_((dansi_xterm_selection_clearWrite(
    dansi_xterm_selection_Targets targets, io_Writer out
))(E$void) $scope) {
    var raw_buf = (A$$(dansi_xterm_selection_target_count, u8)){};
    let raw = try_(dansi_xterm_selection_targetsToRaw(targets, A_ref$((S$u8)(raw_buf))));
    return dansi_xterm_selection_clearRawWrite(raw.as_const, out);
} $unscoped(fn);

fn_((dansi_xterm_selection_setRaw(S_const$u8 targets, S_const$u8 data_base64, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_xterm_selection_setRawWrite(targets, data_base64, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_xterm_selection_setRawWrite(S_const$u8 targets, S_const$u8 data_base64, io_Writer out))(E$void)) {
    return dansi_xterm_selection__rawWrite(dansi_xterm_selection_cmd_u16, targets, data_base64, out);
};

fn_((dansi_xterm_selection_queryRaw(S_const$u8 targets, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_xterm_selection_queryRawWrite(targets, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_xterm_selection_queryRawWrite(S_const$u8 targets, io_Writer out))(E$void)) {
    return dansi_xterm_selection__rawWrite(
        dansi_xterm_selection_cmd_u16, targets, u8_l(dansi_xterm_selection_query_marker), out
    );
};

fn_((dansi_xterm_selection_clearRaw(S_const$u8 targets, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_xterm_selection_clearRawWrite(targets, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_xterm_selection_clearRawWrite(S_const$u8 targets, io_Writer out))(E$void)) {
    return dansi_xterm_selection__rawWrite(dansi_xterm_selection_cmd_u16, targets, u8_l(""), out);
};
