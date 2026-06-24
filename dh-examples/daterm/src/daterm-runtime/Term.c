#include "daterm-runtime/Term.h"

fn_((daterm_Term_poll(daterm_Term self))(O$daterm_Event)) {
    return self.vtbl->pollFn(self.ctx);
};

fn_((daterm_Term_wait(daterm_Term self))(Sched_Cancelable$daterm_Event)) {
    return self.vtbl->waitFn(self.ctx);
};

fn_((daterm_Term_waitTimed(daterm_Term self, time_Dur timeout))(daterm_Term_WaitE$daterm_Event)) {
    return self.vtbl->waitTimedFn(self.ctx, timeout);
};

fn_((daterm_Term_waitProtn(daterm_Term self))(daterm_Event)) {
    return self.vtbl->waitProtnFn(self.ctx);
};

fn_((daterm_Term_reader(daterm_Term self))(io_Reader)) {
    return self.vtbl->readerFn(self.ctx);
};

fn_((daterm_Term_writer(daterm_Term self))(io_Writer)) {
    return self.vtbl->writerFn(self.ctx);
};

fn_((daterm_Term_flush(daterm_Term self))(E$void)) {
    return self.vtbl->flushFn(self.ctx);
};

fn_((daterm_Term_caps(daterm_Term self))(daterm_TermCaps)) {
    return self.vtbl->capsFn(self.ctx);
};

fn_((daterm_Term_queryLocal(daterm_Term self, daterm_Query query))(E$daterm_Query_Result)) {
    return self.vtbl->queryLocalFn(self.ctx, query);
};

fn_((daterm_Term_queryNativeScreenCells(daterm_Term self))(E$daterm_CellSize) $scope) {
    let result = try_(daterm_Term_queryLocal(self, daterm_Query_native_screen_cells));
    $suppress_(switch_enum)(match_(result)) {
    patt_((daterm_Query_Result_size)(size)) return_ok(size) $end(patt);
    default_() return_err(E_cause$Unexpected()) $end(default);
    } $end(match);
} $unscoped(fn);

fn_((daterm_Term_queryCachedScreenCells(daterm_Term self))(E$daterm_CellSize) $scope) {
    let result = try_(daterm_Term_queryLocal(self, daterm_Query_cached_screen_cells));
    $suppress_(switch_enum)(match_(result)) {
    patt_((daterm_Query_Result_size)(size)) return_ok(size) $end(patt);
    default_() return_err(E_cause$Unexpected()) $end(default);
    } $end(match);
} $unscoped(fn);

fn_((daterm_Term_queryNativeCursorPos(daterm_Term self))(E$daterm_Pos) $scope) {
    let result = try_(daterm_Term_queryLocal(self, daterm_Query_native_cursor_pos));
    $suppress_(switch_enum)(match_(result)) {
    patt_((daterm_Query_Result_pos)(pos)) return_ok(pos) $end(patt);
    default_() return_err(E_cause$Unexpected()) $end(default);
    } $end(match);
} $unscoped(fn);

fn_((daterm_Term_runTxn(daterm_Term self, daterm_Txn txn))(daterm_Txn_E$Void)) {
    claim_assert_nonnull(txn.ctx);
    claim_assert_nonnull(txn.out);
    claim_assert_nonnull(txn.requestWriteFn);
    claim_assert_nonnull(txn.matchFn);
    return self.vtbl->runTxnFn(self.ctx, txn);
};
