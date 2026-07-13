#include "dh/thrd/Waker.h"

fn_((thrd_Waker_wake(thrd_Waker self))(void)) {
    self = thrd_Waker_ensureValid(self);
    self.wakeFn(self.ctx);
};
