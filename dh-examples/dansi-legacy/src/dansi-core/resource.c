#include "dansi-core/resource.h"
#include <dh/io/Fixed.h>

$static fn_((dansi_resource__writeHexList(S_const$u8 names, io_Writer writer))(E$void) $scope) {
    $static let_(digits, S_const$u8) = u8_l("0123456789ABCDEF");
    for (usize i = 0; i < names.len; ++i) {
        let ch = *S_at((names)[i]);
        if (ch == u8_c(';')) {
            try_(io_Writer_writeByte(writer, ch));
            continue;
        }
        try_(io_Writer_writeByte(writer, *S_at((digits)[as$(usize)(ch / 16)])));
        try_(io_Writer_writeByte(writer, *S_at((digits)[as$(usize)(ch % 16)])));
    }
    return_ok({});
} $unscoped(fn);

$static fn_((dansi_resource__writeDCSHexList(S_const$u8 prefix, S_const$u8 names, io_Writer writer))(E$void) $scope) {
    try_(io_Writer_writeBytes(writer, u8_l(dansi_utils_dcs)));
    try_(io_Writer_writeBytes(writer, prefix));
    try_(dansi_resource__writeHexList(names, writer));
    try_(io_Writer_writeBytes(writer, u8_l(dansi_utils_st)));
    return_ok({});
} $unscoped(fn);

fn_((dansi_resource_requestValues(S_const$u8 names, S$u8 buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    catch_((dansi_resource_requestValuesWrite(names, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_resource_requestValuesWrite(S_const$u8 names, io_Writer writer))(E$void)) {
    return dansi_resource__writeDCSHexList(u8_l(dansi_utils_resource_request_values), names, writer);
};

fn_((dansi_resource_setTermcap(S_const$u8 name, S$u8 buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    catch_((dansi_resource_setTermcapWrite(name, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_resource_setTermcapWrite(S_const$u8 name, io_Writer writer))(E$void)) {
    return dansi_resource__writeDCSHexList(u8_l(dansi_utils_termcap_set), name, writer);
};

fn_((dansi_resource_requestTermcap(S_const$u8 names, S$u8 buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    catch_((dansi_resource_requestTermcapWrite(names, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_resource_requestTermcapWrite(S_const$u8 names, io_Writer writer))(E$void)) {
    return dansi_resource__writeDCSHexList(u8_l(dansi_utils_termcap_request), names, writer);
};
