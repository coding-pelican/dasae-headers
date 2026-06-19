#include "dansi-core/key.h"
#include <dh/io/Fixed.h>
#include <dh/io/common.h>

fn_((dansi_key_setModifyResource(dansi_key_ModifyResource resource, u8 value, dansi_key_OptionBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_key_setModifyResourceWrite(resource, value, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_key_setModifyResourceWrite(dansi_key_ModifyResource resource, u8 value, io_Writer writer))(E$void)) {
    return io_Writer_print(
        writer,
        u8_l(dansi_key_setModifyResource_static("{:uhh}", "{:uhh}")),
        as$(u8)(resource),
        value
    );
};

fn_((dansi_key_resetModifyResource(dansi_key_ModifyResource resource, dansi_key_OptionBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_key_resetModifyResourceWrite(resource, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_key_resetModifyResourceWrite(dansi_key_ModifyResource resource, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_key_resetModifyResource_static("{:uhh}")), as$(u8)(resource));
};

fn_((dansi_key_disableModifyResource(dansi_key_ModifyResource resource, dansi_key_OptionBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_key_disableModifyResourceWrite(resource, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_key_disableModifyResourceWrite(dansi_key_ModifyResource resource, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_key_disableModifyResource_static("{:uhh}")), as$(u8)(resource));
};

fn_((dansi_key_queryModifyResource(dansi_key_ModifyResource resource, dansi_key_OptionBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_key_queryModifyResourceWrite(resource, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_key_queryModifyResourceWrite(dansi_key_ModifyResource resource, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_key_queryModifyResource_static("{:uhh}")), as$(u8)(resource));
};

fn_((dansi_key_setFormatResource(dansi_key_FormatResource resource, u8 value, dansi_key_OptionBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_key_setFormatResourceWrite(resource, value, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_key_setFormatResourceWrite(dansi_key_FormatResource resource, u8 value, io_Writer writer))(E$void)) {
    return io_Writer_print(
        writer,
        u8_l(dansi_key_setFormatResource_static("{:uhh}", "{:uhh}")),
        as$(u8)(resource),
        value
    );
};

fn_((dansi_key_resetFormatResource(dansi_key_FormatResource resource, dansi_key_OptionBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_key_resetFormatResourceWrite(resource, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_key_resetFormatResourceWrite(dansi_key_FormatResource resource, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_key_resetFormatResource_static("{:uhh}")), as$(u8)(resource));
};

fn_((dansi_key_queryFormatResource(dansi_key_FormatResource resource, dansi_key_OptionBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_key_queryFormatResourceWrite(resource, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_key_queryFormatResourceWrite(dansi_key_FormatResource resource, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_key_queryFormatResource_static("{:uhh}")), as$(u8)(resource));
};

fn_((dansi_key_enableModifyOtherKeys(u8 level, dansi_key_ModifyOtherKeysBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_key_enableModifyOtherKeysWrite(level, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_key_enableModifyOtherKeysWrite(u8 level, io_Writer writer))(E$void)) {
    return io_Writer_print(
        writer,
        u8_l(dansi_utils_csi ">" dansi_utils_key_modify_other_keys dansi_utils_sep "{:uhh}" dansi_utils_key_set_modify_options),
        level
    );
};

fn_((dansi_key_disableModifyOtherKeysWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, u8_l(dansi_key_disableModifyOtherKeys_static()));
};

fn_((dansi_key_enableCSIUFormat(dansi_key_CSIUFormatBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_key_enableCSIUFormatWrite(io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_key_enableCSIUFormatWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, u8_l(dansi_key_enableCSIUFormat_static()));
};

fn_((dansi_key_disableCSIUFormatWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, u8_l(dansi_key_disableCSIUFormat_static()));
};

fn_((dansi_key_enableEnhanced(void))(S_const$u8)) {
    return u8_l(dansi_key_enableEnhanced_static());
};

fn_((dansi_key_enableEnhancedWrite(io_Writer writer))(E$void) $scope) {
    try_(dansi_key_enableModifyOtherKeysWrite(dansi_key_ModifyOtherKeys_level2, writer));
    try_(dansi_key_enableCSIUFormatWrite(writer));
    return_ok({});
} $unscoped(fn);

fn_((dansi_key_disableEnhanced(void))(S_const$u8)) {
    return u8_l(dansi_key_disableEnhanced_static());
};

fn_((dansi_key_disableEnhancedWrite(io_Writer writer))(E$void) $scope) {
    catch_((dansi_key_disableCSIUFormatWrite(writer))($ignore, $do_nothing));
    catch_((dansi_key_disableModifyOtherKeysWrite(writer))($ignore, $do_nothing));
    return_ok({});
} $unscoped(fn);
