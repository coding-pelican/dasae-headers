#include "dh/io/Locked.h"

/*========== External Definitions ===========================================*/

$static var_(io_Locked__ctx_noop, Void) $undefined_static;
let_(io_Locked_noop, io_Locked) = {
    .ctx = &io_Locked__ctx_noop,
    .unlockFn = io_Locked_noUnlock,
};
fn_((io_Locked_noUnlock(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
};

$static var_(io_Locked__ctx_failing, Void) $undefined_static;
let_(io_Locked_failing, io_Locked) = {
    .ctx = &io_Locked__ctx_failing,
    .unlockFn = io_Locked_unreachableUnlock,
};
fn_((io_Locked_unreachableUnlock(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable;
};

fn_((io_Locked_unlock(io_Locked* self))(void)) {
    self = ensureNonnull(self);
    let lock = io_Locked_ensureValid(move(self));
    lock.unlockFn(lock.ctx);
};

fn_((io_Locked_Reader_adopt(io_Reader inner, io_Locked* lock))(io_Locked_Reader)) {
    return (io_Locked_Reader){
        .inner = io_Reader_ensureValid(inner),
        .lock = io_Locked_ensureValid(move(ensureNonnull(lock))),
    };
};
fn_((io_Locked_Reader_unlock(io_Locked_Reader* self))(void)) {
    self = ensureNonnull(self);
    self->inner = l0$((io_Reader));
    io_Locked_unlock(&self->lock);
};
fn_((io_Locked_reader(const io_Locked_Reader* self))(io_Reader)) {
    self = ensureNonnull(self);
    return io_Locked_ensureValid(self->lock), io_Reader_ensureValid(self->inner);
};

fn_((io_Locked_Writer_adopt(io_Writer inner, io_Locked* lock))(io_Locked_Writer)) {
    return (io_Locked_Writer){
        .inner = io_Writer_ensureValid(inner),
        .lock = io_Locked_ensureValid(move(ensureNonnull(lock))),
    };
};
fn_((io_Locked_Writer_unlock(io_Locked_Writer* self))(void)) {
    self = ensureNonnull(self);
    self->inner = l0$((io_Writer));
    io_Locked_unlock(&self->lock);
};
fn_((io_Locked_writer(const io_Locked_Writer* self))(io_Writer)) {
    self = ensureNonnull(self);
    return io_Locked_ensureValid(self->lock), io_Writer_ensureValid(self->inner);
};
