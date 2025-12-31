#include "dage/Runtime.h"
#include "dh/ArrList.h"

T_use_E$($set(mem_Err)(P$P$dage_Window));
T_use$((P$dage_Window)(
    ArrList_init,
    ArrList_fini,
    ArrList_addBackFixed,
    ArrList_pop,
    ArrList_removeSwap
));

/*========== Lifecycle ==========*/

fn_((dage_Runtime_init(dage_Runtime_Cfg cfg))(E$dage_Runtime) $scope) {
    cfg.backend = dage_Backend_ensureValid(cfg.backend);

    let gpa = mem_Allocator_ensureValid(cfg.gpa);
    dage_Runtime runtime = cleared();
    runtime.gpa = gpa;
    runtime.backend = cfg.backend;
    runtime.state.is_running = true;
    runtime.state.should_quit = false;
    runtime.next_window_id = 1; /* 0 is reserved/invalid */
    /* Initialize window list */
    runtime.windows = try_(ArrList_init$P$dage_Window(gpa, dage_Runtime_window_limit));

    return_ok(runtime);
} $unscoped_(fn);

fn_((dage_Runtime_fini(dage_Runtime* self))(void)) {
    claim_assert_nonnull(self);
    /* Destroy all windows */
    for_(($s(self->windows.items))(win_ptr) {
        let win = *win_ptr;
        /* Destroy backend target */
        if (win->target_id != dage_TargetId_invalid) {
            dage_Backend_destroyTarget(self->backend, win->target_id);
        }
        /* Free window */
        dage_Window_fini(win);
        mem_Allocator_destroy(self->gpa, u_anyP(win));
    });
    /* Free window list */
    ArrList_fini$P$dage_Window(&self->windows, self->gpa);
    /* Clear state */
    asg_lit((self)(cleared()));
};

/*========== Window Management ==========*/

errset_((dage_Runtime_CreateWindowErr)(
    WindowLimitReached,
    FailedToAddWindowToList
));

fn_((dage_Runtime_createWindow(dage_Runtime* self, dage_Runtime_WindowCfg cfg))(E$dage_WindowId) $guard) {
    claim_assert_nonnull(self);

    /* Check window limit */
    let win_ptr = catch_((ArrList_addBackFixed$P$dage_Window(&self->windows))($ignore, {
        return_err(dage_Runtime_CreateWindowErr_WindowLimitReached());
    }));
    errdefer_($ignore, ArrList_pop$P$dage_Window(&self->windows));

    /* Create backend target first - error boundary */
    let target_cfg = (dage_Target_Cfg){
        .width = cfg.size.x,
        .height = cfg.size.y,
        .scale = cfg.scale == 0.0f ? 1.0f : cfg.scale,
        .title = orelse_((cfg.title)(u8_l("Engine Window"))),
        .resizable = cfg.resizable,
        .borderless = cfg.borderless,
        .fullscreen = cfg.fullscreen,
    };

    let target_id = try_(dage_Backend_createTarget(self->backend, target_cfg));
    errdefer_($ignore, dage_Backend_destroyTarget(self->backend, target_id));

    /* Allocate window structure */
    let win = u_castP$((P$dage_Window)(try_(mem_Allocator_create(self->gpa, typeInfo$(dage_Window)))));
    errdefer_($ignore, mem_Allocator_destroy(self->gpa, u_anyP(win)));

    /* Initialize window */
    let window_cfg = (dage_Window_Cfg){
        .gpa = self->gpa,
        .size = cfg.size,
        .clear_color = cfg.clear_color,
    };
    *win = try_(dage_Window_init(window_cfg));
    errdefer_($ignore, dage_Window_fini(win));

    /* Assign IDs */
    let window_id = self->next_window_id++;
    win->id = window_id;
    dage_Window_bindTarget(win, target_id);

    /* Add to list */
    *win_ptr = win;

    return_ok(window_id);
} $unguarded_(fn);

fn_((dage_Runtime_destroyWindow(dage_Runtime* self, dage_WindowId id))(void)) {
    claim_assert_nonnull(self);
    /* Find window index */
    let win_idx = eval_(usize $scope)(for_(($s(self->windows.items), $rf(0))(win_ptr, idx) {
        let win = *win_ptr;
        if (win->id != id) { continue; }
        $break_(idx);
    })) eval_(else)({
        claim_unreachable; /* Window not found - programmer error */
    }) $unscoped_(eval);

    /* Remove from list (swap with last) */
    let win = ArrList_removeSwap$P$dage_Window(&self->windows, win_idx);
    /* Destroy backend target */
    if (win->target_id != dage_TargetId_invalid) {
        dage_Backend_destroyTarget(self->backend, win->target_id);
    }
    /* Free window */
    dage_Window_fini(win);
    mem_Allocator_destroy(self->gpa, u_anyP(win));
};

fn_((dage_Runtime_getWindow(dage_Runtime* self, dage_WindowId id))(O$P$dage_Window) $scope) {
    claim_assert_nonnull(self);
    for_(($s(self->windows.items))(win_ptr) {
        let win = *win_ptr;
        if (win->id == id) { return_some(win); }
    });
    return_none();
} $unscoped_(fn);

fn_((dage_Runtime_findWindowByTarget(dage_Runtime* self, dage_TargetId target_id))(O$P$dage_Window) $scope) {
    claim_assert_nonnull(self);
    for_(($s(self->windows.items))(win_ptr) {
        let win = *win_ptr;
        if (win->target_id == target_id) { return_some(win); }
    });
    return_none();
} $unscoped_(fn);

/*========== Main Loop ==========*/

fn_((dage_Runtime_processEvents(dage_Runtime* self))(void)) {
    claim_assert_nonnull(self);

    /* Stack-allocated event buffer - zero allocation */
    var_(events_buf, A$$(dage_Runtime_event_buffer_size, dage_RawEvent)) = cleared();
    let out_slice = A_ref$((S$dage_RawEvent)events_buf);

    /* Pump events from backend */
    let events = dage_Backend_pumpEvents(self->backend, out_slice);

    /* Dispatch events to windows */
    for_(($s(events))(raw) {
        if_some((dage_Runtime_findWindowByTarget(self, raw->target_id))(win)) {
            /* Push event to window's queue */
            dage_Window_pushEvent(win, raw->base);

            /* Update input state immediately */
            dage_InputState_applyEvent(&win->input, &raw->base);

            /* Handle special events */
            match_(raw->base) {
            case_((dage_Event_close_request)) {
                /* Default behavior: request quit if any window closes */
                /* User can override by consuming the event first */
                self->state.should_quit = true;
            } $end(case);
            pattern_((dage_Event_resize)(on_resize)) {
                /* Handle resize - ignore errors (best effort) */
                catch_((dage_Window_handleResize(win, on_resize->new_size))(
                    $ignore, /* Resize failed - continue anyway */
                ));
            } $end(pattern);
            default_() {
                /* Unknown event - ignore */
            } $end(default);
            } $end(match);
        };
    });
};

fn_((dage_Runtime_presentAll(dage_Runtime* self))(void)) {
    claim_assert_nonnull(self);
    for_(($s(self->windows.items))(win_ptr) {
        let win = *win_ptr;
        /* Skip if target is invalid or minimized */
        if (win->target_id == dage_TargetId_invalid) { continue; }

        let metrics = dage_Backend_getTargetMetrics(self->backend, win->target_id);
        if (metrics.is_minimized) { continue; }

        /* Composite viewports */
        dage_Window_composite(win);

        /* Present to backend */
        let pixels = dage_Window_getPixels(win);
        let size = dage_Window_getSize(win);
        dage_Backend_present(
            self->backend,
            win->target_id,
            pixels,
            size.x,
            size.y
        );
    });
};

fn_((dage_Runtime_endFrame(dage_Runtime* self))(void)) {
    claim_assert_nonnull(self);
    for_(($s(self->windows.items))(win_ptr) {
        dage_Window_endFrame(*win_ptr);
    });
};
