#include "dh/io/Locked.h"

fn_((io_Locked_unlock(io_Locked self))(void)) {
    self = io_Locked_ensureValid(self);
    self.unlockFn(self.ctx);
};

fn_((io_Locked_Reader_from(io_Reader inner, io_Locked lock))(io_Locked_Reader)) {
    return (io_Locked_Reader){
        .inner = io_Reader_ensureValid(inner),
        .lock = io_Locked_ensureValid(lock),
    };
};
fn_((io_Locked_Reader_unlock(io_Locked_Reader self))(void)) {
    let lock = io_Locked_ensureValid(self.lock);
    io_Locked_unlock(lock);
};
fn_((io_Locked_reader(io_Locked_Reader self))(io_Reader)) {
    return io_Locked_ensureValid(self.lock), self.inner;
};

fn_((io_Locked_Writer_from(io_Writer inner, io_Locked lock))(io_Locked_Writer)) {
    return (io_Locked_Writer){
        .inner = io_Writer_ensureValid(inner),
        .lock = io_Locked_ensureValid(lock),
    };
};
fn_((io_Locked_Writer_unlock(io_Locked_Writer self))(void)) {
    let lock = io_Locked_ensureValid(self.lock);
    io_Locked_unlock(lock);
};
fn_((io_Locked_writer(io_Locked_Writer self))(io_Writer)) {
    return io_Locked_ensureValid(self.lock), self.inner;
};
