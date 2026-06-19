#include "dansi-core/focus.h"
#include <dh/io/Fixed.h>

fn_((dansi_focus_enableTracking(dansi_focus_TrackingBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_focus_enableTrackingWrite(io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_focus_enableTrackingWrite(io_Writer writer))(E$void)) {
    return dansi_mode_enablePrivateWrite(u16_(dansi_mode_Private_focus_events), writer);
};

fn_((dansi_focus_disableTracking(dansi_focus_TrackingBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_focus_disableTrackingWrite(io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_focus_disableTrackingWrite(io_Writer writer))(E$void)) {
    return dansi_mode_disablePrivateWrite(u16_(dansi_mode_Private_focus_events), writer);
};
