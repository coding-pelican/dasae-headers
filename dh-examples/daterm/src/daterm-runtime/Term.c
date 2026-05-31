#include "daterm-runtime/Term.h"

fn_((daterm_Term_poll(daterm_Term self))(O$daterm_Event)) {
    return self.vtbl->pollFn(self.ctx);
};

fn_((daterm_Term_wait(daterm_Term self))(daterm_Event)) {
    return self.vtbl->waitFn(self.ctx);
};

fn_((daterm_Term_timedWait(daterm_Term self, time_Dur timeout))(Sched_TimeoutE$daterm_Event)) {
    return self.vtbl->timedWaitFn(self.ctx, timeout);
};

fn_((daterm_Term_reader(daterm_Term self))(io_Reader)) {
    return self.vtbl->readerFn(self.ctx);
};

fn_((daterm_Term_writer(daterm_Term self))(io_Writer)) {
    return self.vtbl->writerFn(self.ctx);
};

fn_((daterm_Term_queryScreenSize(daterm_Term self))(E$daterm_Size)) {
    return self.vtbl->queryScreenSizeFn(self.ctx);
};

fn_((daterm_Term_queryCursorPos(daterm_Term self))(E$daterm_Pos)) {
    return self.vtbl->queryCursorPosFn(self.ctx);
};
