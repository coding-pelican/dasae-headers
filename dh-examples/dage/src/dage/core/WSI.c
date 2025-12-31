#include "dage/core/WSI.h"
#include "dage/KeyCode.h"
#include "dage/MouseButton.h"

/*========== Internal Forward Declarations ==================================*/

$static fn_((WSI__findTarget(dage_core_WSI* self, dage_TargetId id))(O$P$dage_core_WSI_Target));
$static fn_((WSI__findFreeSlot(dage_core_WSI* self))(O$P$dage_core_WSI_Target));
$static fn_((WSI__pushEvent(dage_core_WSI_Target* target, dage_Event event))(void));

/* Unsupported platform fallbacks */
pp_if_(pp_true)(pp_then_(
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((WSI__unsupported_platformInit(P$dage_core_WSI self))(E$void));
    $attr($inline_always $maybe_unused)
    $static fn_((WSI__unsupported_platformFini(P$dage_core_WSI self))(void));
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((WSI__unsupported_createTarget(P$dage_core_WSI self, dage_Target_Cfg cfg))(E$dage_TargetId));
    $attr($inline_always $maybe_unused)
    $static fn_((WSI__unsupported_destroyTarget(P$dage_core_WSI self, dage_TargetId id))(void));
    $attr($inline_always $maybe_unused)
    $static fn_((WSI__unsupported_pumpEvents(P$dage_core_WSI self, S$dage_RawEvent out))(S$dage_RawEvent));
    $attr($inline_always $maybe_unused)
    $static fn_((WSI__unsupported_present(P$dage_core_WSI self, dage_TargetId id, S_const$u32 pixels, u32 w, u32 h))(void));
    $attr($inline_always $maybe_unused)
    $static fn_((WSI__unsupported_getTargetMetrics(P$dage_core_WSI self, dage_TargetId id))(dage_TargetMetrics));
));
/* Windows declarations */
pp_if_(plat_is_windows)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((WSI__windows_platformInit(P$dage_core_WSI self))(E$void));
    $attr($inline_always)
    $static fn_((WSI__windows_platformFini(P$dage_core_WSI self))(void));
    $attr($inline_always $must_check)
    $static fn_((WSI__windows_createTarget(P$dage_core_WSI self, dage_Target_Cfg cfg))(E$dage_TargetId));
    $attr($inline_always)
    $static fn_((WSI__windows_destroyTarget(P$dage_core_WSI self, dage_TargetId id))(void));
    $attr($inline_always)
    $static fn_((WSI__windows_pumpEvents(P$dage_core_WSI self, S$dage_RawEvent out))(S$dage_RawEvent));
    $attr($inline_always)
    $static fn_((WSI__windows_present(P$dage_core_WSI self, dage_TargetId id, S_const$u32 pixels, u32 w, u32 h))(void));
    $attr($inline_always)
    $static fn_((WSI__windows_getTargetMetrics(P$dage_core_WSI self, dage_TargetId id))(dage_TargetMetrics));
));
/* Linux/X11 declarations */
pp_if_(dage_core_WSI__use_x11)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((WSI__x11_platformInit(P$dage_core_WSI self))(E$void));
    $attr($inline_always)
    $static fn_((WSI__x11_platformFini(P$dage_core_WSI self))(void));
    $attr($inline_always $must_check)
    $static fn_((WSI__x11_createTarget(P$dage_core_WSI self, dage_Target_Cfg cfg))(E$dage_TargetId));
    $attr($inline_always)
    $static fn_((WSI__x11_destroyTarget(P$dage_core_WSI self, dage_TargetId id))(void));
    $attr($inline_always)
    $static fn_((WSI__x11_pumpEvents(P$dage_core_WSI self, S$dage_RawEvent out))(S$dage_RawEvent));
    $attr($inline_always)
    $static fn_((WSI__x11_present(P$dage_core_WSI self, dage_TargetId id, S_const$u32 pixels, u32 w, u32 h))(void));
    $attr($inline_always)
    $static fn_((WSI__x11_getTargetMetrics(P$dage_core_WSI self, dage_TargetId id))(dage_TargetMetrics));
));

/*========== Function Bindings (Compile-time Platform Selection) ============*/

$static let WSI__platformInit = pp_if_(plat_is_windows)(
    pp_then_(WSI__windows_platformInit),
    pp_else_(pp_if_(dage_core_WSI__use_x11)(
        pp_then_(WSI__x11_platformInit),
        pp_else_(WSI__unsupported_platformInit)
    )));
$static let WSI__platformFini = pp_if_(plat_is_windows)(
    pp_then_(WSI__windows_platformFini),
    pp_else_(pp_if_(dage_core_WSI__use_x11)(
        pp_then_(WSI__x11_platformFini),
        pp_else_(WSI__unsupported_platformFini)
    )));
$static let WSI__createTarget = pp_if_(plat_is_windows)(
    pp_then_(WSI__windows_createTarget),
    pp_else_(pp_if_(dage_core_WSI__use_x11)(
        pp_then_(WSI__x11_createTarget),
        pp_else_(WSI__unsupported_createTarget)
    )));
$static let WSI__destroyTarget = pp_if_(plat_is_windows)(
    pp_then_(WSI__windows_destroyTarget),
    pp_else_(pp_if_(dage_core_WSI__use_x11)(
        pp_then_(WSI__x11_destroyTarget),
        pp_else_(WSI__unsupported_destroyTarget)
    )));
$static let WSI__pumpEvents = pp_if_(plat_is_windows)(
    pp_then_(WSI__windows_pumpEvents),
    pp_else_(pp_if_(dage_core_WSI__use_x11)(
        pp_then_(WSI__x11_pumpEvents),
        pp_else_(WSI__unsupported_pumpEvents)
    )));
$static let WSI__present = pp_if_(plat_is_windows)(
    pp_then_(WSI__windows_present),
    pp_else_(pp_if_(dage_core_WSI__use_x11)(
        pp_then_(WSI__x11_present),
        pp_else_(WSI__unsupported_present)
    )));
$static let WSI__getTargetMetrics = pp_if_(plat_is_windows)(
    pp_then_(WSI__windows_getTargetMetrics),
    pp_else_(pp_if_(dage_core_WSI__use_x11)(
        pp_then_(WSI__x11_getTargetMetrics),
        pp_else_(WSI__unsupported_getTargetMetrics)
    )));

/*========== VT Forward Declaration =========================================*/

$attr($must_check)
$static fn_((WSI_VT__createTarget(dage_Backend_Impl* ctx, dage_Target_Cfg cfg))(E$dage_TargetId));
$static fn_((WSI_VT__destroyTarget(dage_Backend_Impl* ctx, dage_TargetId id))(void));
$static fn_((WSI_VT__pumpEvents(dage_Backend_Impl* ctx, S$dage_RawEvent out))(S$dage_RawEvent));
$static fn_((WSI_VT__present(dage_Backend_Impl* ctx, dage_TargetId id, S_const$u32 pixels, u32 w, u32 h))(void));
$static fn_((WSI_VT__getTargetMetrics(dage_Backend_Impl* ctx, dage_TargetId id))(dage_TargetMetrics));

/*===========================================================================*/
/*                          COMMON IMPLEMENTATION                            */
/*===========================================================================*/

fn_((dage_core_WSI_init(dage_core_WSI_Cfg cfg))(E$P$dage_core_WSI) $guard) {
    let gpa = mem_Allocator_ensureValid(cfg.gpa);

    /* Allocate WSI instance via gpa (not malloc) */
    let wsi = u_castP$((P$dage_core_WSI)(try_(mem_Allocator_create(gpa, typeInfo$(dage_core_WSI)))));
    errdefer_($ignore, mem_Allocator_destroy(gpa, u_anyP(wsi)));

    /* Initialize fields */
    wsi->gpa = gpa;
    mem_setBytes0(mem_asBytes(&wsi->targets));
    wsi->target_count = 1;
    mem_setBytes0(mem_asBytes(&wsi->display));
    mem_setBytes0(mem_asBytes(&wsi->impl));
    wsi->initialized = false;

    /* Platform-specific initialization */
    try_(WSI__platformInit(wsi));

    wsi->initialized = true;
    return_ok(wsi);
} $unguarded_(fn);

fn_((dage_core_WSI_fini(dage_core_WSI** self))(void)) {
    claim_assert_nonnull(self);

    let wsi = *self;
    if (!wsi->initialized) {
        mem_Allocator_destroy(wsi->gpa, u_anyP(wsi));
        mem_setBytes0(mem_asBytes(self));
        return;
    }

    /* Destroy all active targets */
    for_(($s(A_ref(wsi->targets)))(target) {
        if (target->active) {
            WSI__destroyTarget(wsi, target->id);
        }
    });

    /* Platform-specific cleanup */
    WSI__platformFini(wsi);

    /* Free instance via gpa */
    let gpa = wsi->gpa;
    mem_Allocator_destroy(gpa, u_anyP(wsi));
    mem_setBytes0(mem_asBytes(self));
};

fn_((dage_core_WSI_backend(dage_core_WSI* self))(dage_Backend)) {
    claim_assert_nonnull(self);
    claim_assert(self->initialized);
    $static const dage_Backend_VT vt = {
        .createTarget = WSI_VT__createTarget,
        .destroyTarget = WSI_VT__destroyTarget,
        .pumpEvents = WSI_VT__pumpEvents,
        .present = WSI_VT__present,
        .getTargetMetrics = WSI_VT__getTargetMetrics,
    };
    return dage_Backend_ensureValid((dage_Backend){
        .ptr = ptrCast$((dage_Backend_Impl*)(self)),
        .vt = &vt,
    });
};

/*========== Native Handle Queries (Unified API) ============================*/

fn_((dage_core_WSI_getNativeHandle(dage_core_WSI* self, dage_TargetId id))(dage_core_WSI_Handle)) {
    claim_assert_nonnull(self);
    return ensureNonnull(unwrap_(WSI__findTarget(self, id))->handle);
};

fn_((dage_core_WSI_getNativeDisplay(dage_core_WSI* self))(dage_core_WSI_Display)) {
    claim_assert_nonnull(self);
    return ensureNonnull(self->display);
};

/*========== Helper Functions ===============================================*/

fn_((WSI__findTarget(dage_core_WSI* self, dage_TargetId id))(O$P$dage_core_WSI_Target) $scope) {
    for_(($s(A_ref(self->targets)))(target) {
        if (target->active && target->id == id) {
            return_some(target);
        }
    });
    return_none();
} $unscoped_(fn);

fn_((WSI__findFreeSlot(dage_core_WSI* self))(O$P$dage_core_WSI_Target) $scope) {
    for_(($s(A_ref(self->targets)))(target) {
        if (!target->active) {
            return_some(target);
        }
    });
    return_none();
} $unscoped_(fn);

fn_((WSI__pushEvent(dage_core_WSI_Target* target, dage_Event event))(void)) {
    if (target->pending_event_count < dage_core_WSI_max_pending_events) {
        *A_at((target->pending_events)[target->pending_event_count]) = event;
        target->pending_event_count += 1;
    }
};

/*========== VT Wrappers ====================================================*/

fn_((WSI_VT__createTarget(dage_Backend_Impl* ctx, dage_Target_Cfg cfg))(E$dage_TargetId)) {
    return WSI__createTarget((P$dage_core_WSI)ctx, cfg);
};

fn_((WSI_VT__destroyTarget(dage_Backend_Impl* ctx, dage_TargetId id))(void)) {
    WSI__destroyTarget((P$dage_core_WSI)ctx, id);
};

fn_((WSI_VT__pumpEvents(dage_Backend_Impl* ctx, S$dage_RawEvent out))(S$dage_RawEvent)) {
    return WSI__pumpEvents((P$dage_core_WSI)ctx, out);
};

fn_((WSI_VT__present(dage_Backend_Impl* ctx, dage_TargetId id, S_const$u32 pixels, u32 w, u32 h))(void)) {
    WSI__present((P$dage_core_WSI)ctx, id, pixels, w, h);
};

fn_((WSI_VT__getTargetMetrics(dage_Backend_Impl* ctx, dage_TargetId id))(dage_TargetMetrics)) {
    return WSI__getTargetMetrics((P$dage_core_WSI)ctx, id);
};

/*========== Unsupported Platform ===========================================*/

fn_((WSI__unsupported_platformInit(P$dage_core_WSI self))(E$void) $scope) {
    let_ignore = self;
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((WSI__unsupported_platformFini(P$dage_core_WSI self))(void)) {
    let_ignore = self;
};

fn_((WSI__unsupported_createTarget(P$dage_core_WSI self, dage_Target_Cfg cfg))(E$dage_TargetId) $scope) {
    let_ignore = self;
    let_ignore = cfg;
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((WSI__unsupported_destroyTarget(P$dage_core_WSI self, dage_TargetId id))(void)) {
    let_ignore = self;
    let_ignore = id;
};

fn_((WSI__unsupported_pumpEvents(P$dage_core_WSI self, S$dage_RawEvent out))(S$dage_RawEvent)) {
    let_ignore = self;
    return S_slice((out)$r(0, 0));
};

fn_((WSI__unsupported_present(P$dage_core_WSI self, dage_TargetId id, S_const$u32 pixels, u32 w, u32 h))(void)) {
    let_ignore = self;
    let_ignore = id;
    let_ignore = pixels;
    let_ignore = w;
    let_ignore = h;
};

fn_((WSI__unsupported_getTargetMetrics(P$dage_core_WSI self, dage_TargetId id))(dage_TargetMetrics)) {
    let_ignore = self;
    let_ignore = id;
    return (dage_TargetMetrics){ .should_close = true };
};

/*===========================================================================*/
/*                           WINDOWS IMPLEMENTATION                          */
/*===========================================================================*/

#if plat_is_windows
/*----- Key Code Translation -----*/

$static fn_((WSI__windows_translateKeyCode(WPARAM vk))(dage_KeyCode)) {
    switch (vk) {
    case 'A': return dage_KeyCode_a;
    case 'B': return dage_KeyCode_b;
    case 'C': return dage_KeyCode_c;
    case 'D': return dage_KeyCode_d;
    case 'E': return dage_KeyCode_e;
    case 'F': return dage_KeyCode_f;
    case 'G': return dage_KeyCode_g;
    case 'H': return dage_KeyCode_h;
    case 'I': return dage_KeyCode_i;
    case 'J': return dage_KeyCode_j;
    case 'K': return dage_KeyCode_k;
    case 'L': return dage_KeyCode_l;
    case 'M': return dage_KeyCode_m;
    case 'N': return dage_KeyCode_n;
    case 'O': return dage_KeyCode_o;
    case 'P': return dage_KeyCode_p;
    case 'Q': return dage_KeyCode_q;
    case 'R': return dage_KeyCode_r;
    case 'S': return dage_KeyCode_s;
    case 'T': return dage_KeyCode_t;
    case 'U': return dage_KeyCode_u;
    case 'V': return dage_KeyCode_v;
    case 'W': return dage_KeyCode_w;
    case 'X': return dage_KeyCode_x;
    case 'Y': return dage_KeyCode_y;
    case 'Z': return dage_KeyCode_z;

    case '0': return dage_KeyCode_n0;
    case '1': return dage_KeyCode_n1;
    case '2': return dage_KeyCode_n2;
    case '3': return dage_KeyCode_n3;
    case '4': return dage_KeyCode_n4;
    case '5': return dage_KeyCode_n5;
    case '6': return dage_KeyCode_n6;
    case '7': return dage_KeyCode_n7;
    case '8': return dage_KeyCode_n8;
    case '9': return dage_KeyCode_n9;

    case VK_F1:  return dage_KeyCode_f1;
    case VK_F2:  return dage_KeyCode_f2;
    case VK_F3:  return dage_KeyCode_f3;
    case VK_F4:  return dage_KeyCode_f4;
    case VK_F5:  return dage_KeyCode_f5;
    case VK_F6:  return dage_KeyCode_f6;
    case VK_F7:  return dage_KeyCode_f7;
    case VK_F8:  return dage_KeyCode_f8;
    case VK_F9:  return dage_KeyCode_f9;
    case VK_F10: return dage_KeyCode_f10;
    case VK_F11: return dage_KeyCode_f11;
    case VK_F12: return dage_KeyCode_f12;

    case VK_UP:    return dage_KeyCode_arrow_up;
    case VK_DOWN:  return dage_KeyCode_arrow_down;
    case VK_LEFT:  return dage_KeyCode_arrow_left;
    case VK_RIGHT: return dage_KeyCode_arrow_right;

    case VK_LSHIFT:   return dage_KeyCode_shift_left;
    case VK_RSHIFT:   return dage_KeyCode_shift_right;
    case VK_LCONTROL: return dage_KeyCode_ctrl_left;
    case VK_RCONTROL: return dage_KeyCode_ctrl_right;
    case VK_LMENU:    return dage_KeyCode_alt_left;
    case VK_RMENU:    return dage_KeyCode_alt_right;

    case VK_SPACE:  return dage_KeyCode_space;
    case VK_RETURN: return dage_KeyCode_enter;
    case VK_ESCAPE: return dage_KeyCode_esc;
    case VK_TAB:    return dage_KeyCode_tab;
    case VK_BACK:   return dage_KeyCode_backspace;
    case VK_DELETE: return dage_KeyCode_delete;
    case VK_INSERT: return dage_KeyCode_insert;
    case VK_HOME:   return dage_KeyCode_home;
    case VK_END:    return dage_KeyCode_end;
    case VK_PRIOR:  return dage_KeyCode_page_up;
    case VK_NEXT:   return dage_KeyCode_page_down;

    default: return dage_KeyCode_unknown;
    }
};

$static fn_((WSI__windows_getKeyMods(void))(dage_KeyMods)) {
    dage_KeyMods mods = { .packed = 0 };
    mods.shift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    mods.ctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
    mods.alt = (GetKeyState(VK_MENU) & 0x8000) != 0;
    mods.caps_lock = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
    mods.num_lock = (GetKeyState(VK_NUMLOCK) & 0x0001) != 0;
    return mods;
};

/*----- Window Procedure -----*/

$static fn_((WSI__windows_findByHWND(P$dage_core_WSI self, HWND hwnd))(O$P$dage_core_WSI_Target) $scope) {
    claim_assert_nonnull(self);
    for_(($s(A_ref(self->targets)))(target) {
        if (target->active && target->handle == hwnd) {
            return_some(target);
        }
    });
    return_none();
} $unscoped_(fn);

$attr($stdcall)
$static fn_((WSI__windows_wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam))(LRESULT)) {
    let self = intToPtr$((P$dage_core_WSI)GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (self == null) {
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
#if UNUSED_CODE
    if_none(some$(O$P$dage_core_WSI, self)) {
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
#endif /* UNUSED_CODE */

    let target = orelse_((WSI__windows_findByHWND(self, hwnd))(
        return DefWindowProcW(hwnd, msg, wParam, lParam)
    ));
    switch (msg) {
    case WM_CLOSE: {
        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_close_request){}));
        target->metrics.should_close = true;
        return 0;
    }
    case WM_SIZE: {
        let new_w = LOWORD(lParam);
        let new_h = HIWORD(lParam);

        let resize = (dage_Event_Resize){
            .old_size = target->metrics.physical_size,
            .new_size = { .x = new_w, .y = new_h },
        };
        target->metrics.physical_size = resize.new_size;
        target->metrics.logical_size = resize.new_size;

        if (wParam == SIZE_MINIMIZED) {
            target->metrics.is_minimized = true;
            target->metrics.is_maximized = false;
            WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_minimize){}));
        } else if (wParam == SIZE_MAXIMIZED) {
            target->metrics.is_minimized = false;
            target->metrics.is_maximized = true;
            WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_maximize){}));
        } else if (wParam == SIZE_RESTORED) {
            if (target->metrics.is_minimized || target->metrics.is_maximized) {
                target->metrics.is_minimized = false;
                target->metrics.is_maximized = false;
                WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_restore){}));
            }
        }

        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_resize)(resize)));
        return 0;
    }
    case WM_MOVE: {
        let move = (dage_Event_Move){
            .old_pos = target->metrics.position,
            .new_pos = m_V2i32_of(intCast$((i16)(LOWORD(lParam))), intCast$((i16)(HIWORD(lParam)))),
        };
        target->metrics.position = move.new_pos;
        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_move)(move)));
        return 0;
    }
    case WM_SETFOCUS: {
        target->metrics.is_focused = true;
        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_focus){}));
        return 0;
    }
    case WM_KILLFOCUS: {
        target->metrics.is_focused = false;
        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_blur){}));
        return 0;
    }
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN: {
        let is_repeat = (lParam & 0x40000000) != 0;
        let key = (dage_Event_Key){
            .key = WSI__windows_translateKeyCode(wParam),
            .mods = WSI__windows_getKeyMods(),
        };
        if (is_repeat) {
            WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_key_repeat)(key)));
        } else {
            WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_key_down)(key)));
        }
        return 0;
    }
    case WM_KEYUP:
    case WM_SYSKEYUP: {
        let key = (dage_Event_Key){
            .key = WSI__windows_translateKeyCode(wParam),
            .mods = WSI__windows_getKeyMods(),
        };
        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_key_up)(key)));
        return 0;
    }
    case WM_LBUTTONDOWN: {
        let btn = (dage_Event_MouseButton){
            .button = dage_MouseButton_left,
            .mods = WSI__windows_getKeyMods(),
            .pos = m_V2i32_of(intCast$((i16)(LOWORD(lParam))), intCast$((i16)(HIWORD(lParam)))),
        };
        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_mouse_down)(btn)));
        SetCapture(hwnd);
        return 0;
    }
    case WM_LBUTTONUP: {
        let btn = (dage_Event_MouseButton){
            .button = dage_MouseButton_left,
            .mods = WSI__windows_getKeyMods(),
            .pos = m_V2i32_of(intCast$((i16)(LOWORD(lParam))), intCast$((i16)(HIWORD(lParam)))),
        };
        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_mouse_up)(btn)));
        ReleaseCapture();
        return 0;
    }
    case WM_RBUTTONDOWN: {
        let btn = (dage_Event_MouseButton){
            .button = dage_MouseButton_right,
            .mods = WSI__windows_getKeyMods(),
            .pos = m_V2i32_of(intCast$((i16)(LOWORD(lParam))), intCast$((i16)(HIWORD(lParam)))),
        };
        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_mouse_down)(btn)));
        return 0;
    }
    case WM_RBUTTONUP: {
        let btn = (dage_Event_MouseButton){
            .button = dage_MouseButton_right,
            .mods = WSI__windows_getKeyMods(),
            .pos = m_V2i32_of(intCast$((i16)(LOWORD(lParam))), intCast$((i16)(HIWORD(lParam)))),
        };
        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_mouse_up)(btn)));
        return 0;
    }
    case WM_MBUTTONDOWN: {
        let btn = (dage_Event_MouseButton){
            .button = dage_MouseButton_middle,
            .mods = WSI__windows_getKeyMods(),
            .pos = m_V2i32_of(intCast$((i16)(LOWORD(lParam))), intCast$((i16)(HIWORD(lParam)))),
        };
        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_mouse_down)(btn)));
        return 0;
    }
    case WM_MBUTTONUP: {
        let btn = (dage_Event_MouseButton){
            .button = dage_MouseButton_middle,
            .mods = WSI__windows_getKeyMods(),
            .pos = m_V2i32_of(intCast$((i16)(LOWORD(lParam))), intCast$((i16)(HIWORD(lParam)))),
        };
        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_mouse_up)(btn)));
        return 0;
    }
    case WM_MOUSEMOVE: {
        $static m_V2i32 s_last = m_V2i32_of_static(0, 0);
        let x = intCast$((i16)(LOWORD(lParam)));
        let y = intCast$((i16)(HIWORD(lParam)));
        let move = (dage_Event_MouseMove){
            .pos = m_V2i32_of(x, y),
            .delta = m_V2i32_of(x - s_last.x, y - s_last.y),
        };
        s_last = move.pos;
        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_mouse_move)(move)));
        return 0;
    }
    case WM_MOUSELEAVE: {
        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_mouse_leave){}));
        return 0;
    }
    case WM_MOUSEWHEEL: {
        let delta = GET_WHEEL_DELTA_WPARAM(wParam);
        let scroll = (dage_Event_Scroll){
            .delta = m_V2f32_of(0.0f, as$(f32)(delta) / 120.0f),
        };
        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_scroll)(scroll)));
        return 0;
    }
    case WM_MOUSEHWHEEL: {
        let delta = GET_WHEEL_DELTA_WPARAM(wParam);
        let scroll = (dage_Event_Scroll){
            .delta = m_V2f32_of(as$(f32)(delta) / 120.0f, 0.0f),
        };
        WSI__pushEvent(target, union_of$((dage_Event)(dage_Event_scroll)(scroll)));
        return 0;
    }
    default:
        break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
};

/*----- Platform Implementation -----*/

errset_((WSI_PlatformInitErr)(FailedRegisterWindowClass));
fn_((WSI__windows_platformInit(P$dage_core_WSI self))(E$void) $scope) {
    self->display = GetModuleHandleW(0);

    WNDCLASSEXW wc = {
        .cbSize = sizeOf$(WNDCLASSEXW),
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = WSI__windows_wndProc,
        .hInstance = self->display,
        .hCursor = LoadCursor(0, IDC_ARROW),
        .lpszClassName = L"dh_dage_WSI",
    };

    if (RegisterClassExW(&wc) == 0) {
        return_err(WSI_PlatformInitErr_FailedRegisterWindowClass());
    }

    self->impl.window_class_registered = true;
    return_ok({});
} $unscoped_(fn);

fn_((WSI__windows_platformFini(P$dage_core_WSI self))(void)) {
    if (self->impl.window_class_registered) {
        UnregisterClassW(L"dh_dage_WSI", self->display);
        self->impl.window_class_registered = false;
    }
};

errset_((WSI_CreateTargetErr)(
    MaximumTargetsReached,
    FailedCreateWindow
));
fn_((WSI__windows_createTarget(P$dage_core_WSI self, dage_Target_Cfg cfg))(E$dage_TargetId) $scope) {
    let target = orelse_((WSI__findFreeSlot(self))(
        return_err(WSI_CreateTargetErr_MaximumTargetsReached())
    ));

    DWORD style = WS_OVERLAPPEDWINDOW;
    if (!cfg.resizable) { style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX); }
    if (cfg.borderless) { style = WS_POPUP; }

    var rect = (RECT){ .left = 0, .top = 0, .right = as$(LONG)(cfg.width), .bottom = as$(LONG)(cfg.height) };
    AdjustWindowRect(&rect, style, false);


    var_(title_wide, A$$(256, wchar_t)) = A_init(L"Engine Window");
    if (cfg.title.len > 0) {
        let limit_len = prim_min(cfg.title.len, A_len(title_wide) - 1);
        let title_utf8 = S_prefix((cfg.title)limit_len);
        let written = MultiByteToWideChar(
            CP_UTF8, 0,
            ptrCast$((const char*)title_utf8.ptr), intCast$((i32)title_utf8.len),
            A_ptr(title_wide), intCast$((i32)A_len(title_wide))
        );
        if (written > 0) {
            *A_at((title_wide)[written]) = L'\0';
        }
    }

    let hwnd = CreateWindowExW(
        0, L"dh_dage_WSI", A_ptr(title_wide), style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        0, 0, self->display, 0
    );
    if (hwnd == 0) {
        return_err(WSI_CreateTargetErr_FailedCreateWindow());
    }

    SetWindowLongPtrW(hwnd, GWLP_USERDATA, intCast$((LONG_PTR)ptrToInt(self)));

    let id = self->target_count;
    self->target_count += 1;

    mem_setBytes0(mem_asBytes(target));
    target->id = id;
    target->active = true;
    target->handle = hwnd;
    target->device = GetDC(hwnd);
    target->metrics = (dage_TargetMetrics){
        .physical_size = { .x = cfg.width, .y = cfg.height },
        .logical_size = { .x = cfg.width, .y = cfg.height },
        .dpi_scale = 0.0f < cfg.scale ? cfg.scale : 1.0f,
    };
    target->image.bmi = (BITMAPINFO){
        .bmiHeader = {
            .biSize = sizeOf$(BITMAPINFOHEADER),
            .biWidth = as$(LONG)(cfg.width),
            .biHeight = -as$(LONG)(cfg.height),
            .biPlanes = 1,
            .biBitCount = 32,
            .biCompression = BI_RGB,
        },
    };

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return_ok(id);
} $unscoped_(fn);

fn_((WSI__windows_destroyTarget(P$dage_core_WSI self, dage_TargetId id))(void)) {
    let target = orelse_((WSI__findTarget(self, id))(return));
    ReleaseDC(target->handle, target->device);
    DestroyWindow(target->handle);
    mem_setBytes0(mem_asBytes(target));
};

fn_((WSI__windows_pumpEvents(P$dage_core_WSI self, S$dage_RawEvent out))(S$dage_RawEvent)) {
    MSG msg = cleared();
    while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    usize count = 0;
    for (usize i = 0; i < dage_core_WSI_max_targets && count < out.len; ++i) {
        let target = A_at((self->targets)[i]);
        if (!target->active) { continue; }
        for (usize j = 0; j < target->pending_event_count && count < out.len; ++j) {
            asg_lit((S_at((out)[count]))({
                .target_id = target->id,
                .base = *A_at((target->pending_events)[j]),
            }));
            count += 1;
        }
        target->pending_event_count = 0;
    }

    return S_slice((out)$r(0, count));
};

fn_((WSI__windows_present(P$dage_core_WSI self, dage_TargetId id, S_const$u32 pixels, u32 w, u32 h))(void)) {
    let target = orelse_((WSI__findTarget(self, id))(return));
    if (target->device == 0) { return; }

    if (target->image.bmi.bmiHeader.biWidth != as$(LONG)(w) || target->image.bmi.bmiHeader.biHeight != -as$(LONG)(h)) {
        target->image.bmi.bmiHeader.biWidth = as$(LONG)(w);
        target->image.bmi.bmiHeader.biHeight = -as$(LONG)(h);
    }

    StretchDIBits(
        target->device,
        0, 0, intCast$((int)(target->metrics.physical_size.x)), intCast$((int)(target->metrics.physical_size.y)),
        0, 0, intCast$((int)(w)), intCast$((int)(h)),
        pixels.ptr,
        &target->image.bmi,
        DIB_RGB_COLORS,
        SRCCOPY
    );
};

fn_((WSI__windows_getTargetMetrics(P$dage_core_WSI self, dage_TargetId id))(dage_TargetMetrics) $scope) {
    let target = orelse_((WSI__findTarget(self, id))(
        return_({ .should_close = true })
    ));
    return_(target->metrics);
} $unscoped_(fn);
#endif /* plat_is_windows */

/*===========================================================================*/
/*                          LINUX/X11 IMPLEMENTATION                         */
/*===========================================================================*/

#if dage_core_WSI__use_x11
/*----- Key Code Translation -----*/

$static fn_((WSI__x11_translateKey(KeySym keysym))(dage_KeyCode)) {
    switch (keysym) {
    case XK_a:
    case XK_A: return dage_KeyCode_a;
    case XK_b:
    case XK_B: return dage_KeyCode_b;
    case XK_c:
    case XK_C: return dage_KeyCode_c;
    case XK_d:
    case XK_D: return dage_KeyCode_d;
    case XK_e:
    case XK_E: return dage_KeyCode_e;
    case XK_f:
    case XK_F: return dage_KeyCode_f;
    case XK_g:
    case XK_G: return dage_KeyCode_g;
    case XK_h:
    case XK_H: return dage_KeyCode_h;
    case XK_i:
    case XK_I: return dage_KeyCode_i;
    case XK_j:
    case XK_J: return dage_KeyCode_j;
    case XK_k:
    case XK_K: return dage_KeyCode_k;
    case XK_l:
    case XK_L: return dage_KeyCode_l;
    case XK_m:
    case XK_M: return dage_KeyCode_m;
    case XK_n:
    case XK_N: return dage_KeyCode_n;
    case XK_o:
    case XK_O: return dage_KeyCode_o;
    case XK_p:
    case XK_P: return dage_KeyCode_p;
    case XK_q:
    case XK_Q: return dage_KeyCode_q;
    case XK_r:
    case XK_R: return dage_KeyCode_r;
    case XK_s:
    case XK_S: return dage_KeyCode_s;
    case XK_t:
    case XK_T: return dage_KeyCode_t;
    case XK_u:
    case XK_U: return dage_KeyCode_u;
    case XK_v:
    case XK_V: return dage_KeyCode_v;
    case XK_w:
    case XK_W: return dage_KeyCode_w;
    case XK_x:
    case XK_X: return dage_KeyCode_x;
    case XK_y:
    case XK_Y: return dage_KeyCode_y;
    case XK_z:
    case XK_Z: return dage_KeyCode_z;

    case XK_0: return dage_KeyCode_n0;
    case XK_1: return dage_KeyCode_n1;
    case XK_2: return dage_KeyCode_n2;
    case XK_3: return dage_KeyCode_n3;
    case XK_4: return dage_KeyCode_n4;
    case XK_5: return dage_KeyCode_n5;
    case XK_6: return dage_KeyCode_n6;
    case XK_7: return dage_KeyCode_n7;
    case XK_8: return dage_KeyCode_n8;
    case XK_9: return dage_KeyCode_n9;

    case XK_space:     return dage_KeyCode_space;
    case XK_Escape:    return dage_KeyCode_esc;
    case XK_Return:    return dage_KeyCode_enter;
    case XK_Tab:       return dage_KeyCode_tab;
    case XK_BackSpace: return dage_KeyCode_backspace;

    case XK_Up:    return dage_KeyCode_arrow_up;
    case XK_Down:  return dage_KeyCode_arrow_down;
    case XK_Left:  return dage_KeyCode_arrow_left;
    case XK_Right: return dage_KeyCode_arrow_right;

    case XK_Shift_L:   return dage_KeyCode_shift_left;
    case XK_Shift_R:   return dage_KeyCode_shift_right;
    case XK_Control_L: return dage_KeyCode_ctrl_left;
    case XK_Control_R: return dage_KeyCode_ctrl_right;
    case XK_Alt_L:     return dage_KeyCode_alt_left;
    case XK_Alt_R:     return dage_KeyCode_alt_right;

    case XK_F1:  return dage_KeyCode_f1;
    case XK_F2:  return dage_KeyCode_f2;
    case XK_F3:  return dage_KeyCode_f3;
    case XK_F4:  return dage_KeyCode_f4;
    case XK_F5:  return dage_KeyCode_f5;
    case XK_F6:  return dage_KeyCode_f6;
    case XK_F7:  return dage_KeyCode_f7;
    case XK_F8:  return dage_KeyCode_f8;
    case XK_F9:  return dage_KeyCode_f9;
    case XK_F10: return dage_KeyCode_f10;
    case XK_F11: return dage_KeyCode_f11;
    case XK_F12: return dage_KeyCode_f12;

    case XK_Delete:    return dage_KeyCode_delete;
    case XK_Insert:    return dage_KeyCode_insert;
    case XK_Home:      return dage_KeyCode_home;
    case XK_End:       return dage_KeyCode_end;
    case XK_Page_Up:   return dage_KeyCode_page_up;
    case XK_Page_Down: return dage_KeyCode_page_down;

    default: return dage_KeyCode_unknown;
    }
}

$static fn_((WSI__x11_findByWindow(P$dage_core_WSI self, u64 window))(O$P$dage_core_WSI_Target)) {
    for (usize i = 0; i < dage_core_WSI_max_targets; ++i) {
        let target = A_at(self->targets[i]);
        if (target->active && target->handle == window) {
            return some$(O$P$dage_core_WSI_Target, target);
        }
    }
    return none$(O$P$dage_core_WSI_Target);
}

/*----- Platform Implementation -----*/

fn_((WSI__x11_platformInit(P$dage_core_WSI self))(E$void) $guard) {
    Display* dpy = XOpenDisplay(0);
    if (dpy == 0) {
        return_err(Err_Custom("Failed to open X11 display"));
    }

    self->display = (P$raw)dpy;
    self->impl.screen = DefaultScreen(dpy);
    self->impl.wm_delete_window = (u64)XInternAtom(dpy, "WM_DELETE_WINDOW", False);

    return_ok({});
} $unguarded_(fn);

fn_((WSI__x11_platformFini(P$dage_core_WSI self))(void)) {
    if (self->display != cleared()) {
        XCloseDisplay((Display*)self->display);
        self->display = cleared();
    }
}

fn_((WSI__x11_createTarget(P$dage_core_WSI self, dage_Target_Cfg cfg))(E$dage_TargetId) $guard) {
    let target_opt = WSI__findFreeSlot(self);
    if_none(target_opt) {
        return_err(Err_Custom("Maximum targets reached"));
    }
    let target = unwrap_(target_opt);

    Display* dpy = (Display*)self->display;
    Window root = RootWindow(dpy, self->impl.screen);

    XSetWindowAttributes attrs = {
        .event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask
                    | PointerMotionMask | EnterWindowMask | LeaveWindowMask
                    | StructureNotifyMask | FocusChangeMask,
    };

    Window window = XCreateWindow(
        dpy, root,
        0, 0, cfg.width, cfg.height,
        0, CopyFromParent, InputOutput, CopyFromParent,
        CWEventMask, &attrs
    );

    if (cfg.title.ptr != cleared() && cfg.title.len > 0) {
        XStoreName(dpy, window, (const char*)cfg.title.ptr);
    }

    Atom wm_delete = (Atom)self->impl.wm_delete_window;
    XSetWMProtocols(dpy, window, &wm_delete, 1);

    GC gc = XCreateGC(dpy, window, 0, 0);

    /* Allocate pixel buffer via gpa (NOT malloc) */
    usize buffer_len = (usize)cfg.width * cfg.height;
    let alloc_result = mem_Allocator_alloc(self->gpa, typeInfo$(u32), buffer_len);
    if_err(alloc_result) {
        XFreeGC(dpy, gc);
        XDestroyWindow(dpy, window);
        return_err(Err_Custom("Failed to allocate pixel buffer"));
    }
    let pixels = u_castS$((S$u32)(unwrap_ok_(alloc_result)));

    XImage* ximage = XCreateImage(
        dpy,
        DefaultVisual(dpy, self->impl.screen),
        DefaultDepth(dpy, self->impl.screen),
        ZPixmap, 0, (char*)pixels.ptr,
        cfg.width, cfg.height, 32, 0
    );

    if (ximage == 0) {
        mem_Allocator_free(self->gpa, u_anyS(pixels));
        XFreeGC(dpy, gc);
        XDestroyWindow(dpy, window);
        return_err(Err_Custom("Failed to create XImage"));
    }

    let id = self->target_count;
    self->target_count += 1;

    *target = (dage_core_WSI_Target){
        .id = id,
        .active = true,
        .handle = (u64)window,
        .device = (u64)gc,
        .image = {
            .ximage = (P$raw)ximage,
            .pixels = pixels,
        },
        .metrics = {
            .physical_size = { { { cfg.width, cfg.height } } },
            .logical_size = { { { cfg.width, cfg.height } } },
            .dpi_scale = cfg.scale > 0.0f ? cfg.scale : 1.0f,
        },
        .pending_event_count = 0,
    };

    XMapWindow(dpy, window);
    XFlush(dpy);

    return_ok(id);
} $unguarded_(fn);

fn_((WSI__x11_destroyTarget(P$dage_core_WSI self, dage_TargetId id))(void)) {
    let target_opt = WSI__findTarget(self, id);
    if_none(target_opt) { return; }
    let target = unwrap_(target_opt);

    Display* dpy = (Display*)self->display;

    if (target->image.ximage != cleared()) {
        /* Detach pixel buffer before destroying XImage (XImage doesn't own it) */
        ((XImage*)target->image.ximage)->data = 0;
        XDestroyImage((XImage*)target->image.ximage);
    }
    if (target->image.pixels.ptr != cleared()) {
        /* Free pixel buffer via gpa (NOT free) */
        mem_Allocator_free(self->gpa, u_anyS(target->image.pixels));
    }
    if (target->device != 0) {
        XFreeGC(dpy, (GC)target->device);
    }
    if (target->handle != 0) {
        XDestroyWindow(dpy, (Window)target->handle);
    }

    *target = cleared();
}

fn_((WSI__x11_pumpEvents(P$dage_core_WSI self, S$dage_RawEvent out))(S$dage_RawEvent)) {
    Display* dpy = (Display*)self->display;
    usize count = 0;

    while (XPending(dpy) > 0 && count < out.len) {
        XEvent xev;
        XNextEvent(dpy, &xev);

        let target_opt = WSI__x11_findByWindow(self, (u64)xev.xany.window);
        if_none(target_opt) { continue; }
        let target = unwrap_(target_opt);

        dage_Event event;
        bool has_event = true;

        switch (xev.type) {
        case KeyPress: {
            KeySym keysym = XLookupKeysym(&xev.xkey, 0);
            event = union_of$((dage_Event)(dage_Event_key_down))((dage_Event_Key){
                .key = WSI__x11_translateKey(keysym),
            });
        } break;

        case KeyRelease: {
            KeySym keysym = XLookupKeysym(&xev.xkey, 0);
            event = union_of$((dage_Event)(dage_Event_key_up))((dage_Event_Key){
                .key = WSI__x11_translateKey(keysym),
            });
        } break;

        case ButtonPress: {
            if (xev.xbutton.button == Button4) {
                event = union_of$((dage_Event)(dage_Event_scroll))((dage_Event_Scroll){
                    .delta = { { { 0.0f, 1.0f } } },
                });
            } else if (xev.xbutton.button == Button5) {
                event = union_of$((dage_Event)(dage_Event_scroll))((dage_Event_Scroll){
                    .delta = { { { 0.0f, -1.0f } } },
                });
            } else {
                dage_MouseButton btn = dage_MouseButton_left;
                if (xev.xbutton.button == Button2) btn = dage_MouseButton_middle;
                if (xev.xbutton.button == Button3) btn = dage_MouseButton_right;
                event = union_of$((dage_Event)(dage_Event_mouse_down))((dage_Event_MouseButton){
                    .button = btn,
                    .pos = { { { xev.xbutton.x, xev.xbutton.y } } },
                });
            }
        } break;

        case ButtonRelease: {
            if (xev.xbutton.button == Button4 || xev.xbutton.button == Button5) {
                has_event = false;
            } else {
                dage_MouseButton btn = dage_MouseButton_left;
                if (xev.xbutton.button == Button2) btn = dage_MouseButton_middle;
                if (xev.xbutton.button == Button3) btn = dage_MouseButton_right;
                event = union_of$((dage_Event)(dage_Event_mouse_up))((dage_Event_MouseButton){
                    .button = btn,
                    .pos = { { { xev.xbutton.x, xev.xbutton.y } } },
                });
            }
        } break;

        case MotionNotify: {
            event = union_of$((dage_Event)(dage_Event_mouse_move))((dage_Event_MouseMove){
                .pos = { { { xev.xmotion.x, xev.xmotion.y } } },
            });
        } break;

        case EnterNotify: {
            event = union_of$((dage_Event)(dage_Event_mouse_enter))();
        } break;

        case LeaveNotify: {
            event = union_of$((dage_Event)(dage_Event_mouse_leave))();
        } break;

        case ConfigureNotify: {
            let new_w = (u32)xev.xconfigure.width;
            let new_h = (u32)xev.xconfigure.height;

            if (new_w != target->metrics.physical_size.x || new_h != target->metrics.physical_size.y) {
                event = union_of$((dage_Event)(dage_Event_resize))((dage_Event_Resize){
                    .old_size = target->metrics.physical_size,
                    .new_size = { { { new_w, new_h } } },
                });
                target->metrics.physical_size = (m_V2u32){ { { new_w, new_h } } };
                target->metrics.logical_size = (m_V2u32){ { { new_w, new_h } } };
            } else {
                has_event = false;
            }
        } break;

        case FocusIn: {
            target->metrics.is_focused = true;
            event = union_of$((dage_Event)(dage_Event_focus))();
        } break;

        case FocusOut: {
            target->metrics.is_focused = false;
            event = union_of$((dage_Event)(dage_Event_blur))();
        } break;

        case ClientMessage: {
            if ((u64)xev.xclient.data.l[0] == self->impl.wm_delete_window) {
                target->metrics.should_close = true;
                event = union_of$((dage_Event)(dage_Event_close_request))();
            } else {
                has_event = false;
            }
        } break;

        default:
            has_event = false;
            break;
        }

        if (has_event) {
            *S_at(out[count]) = (dage_RawEvent){
                .target_id = target->id,
                .base = event,
            };
            count += 1;
        }
    }

    return S_slice((out)$r(0, count));
}

fn_((WSI__x11_present(P$dage_core_WSI self, dage_TargetId id, S_const$u32 pixels, u32 w, u32 h))(void)) {
    let target_opt = WSI__findTarget(self, id);
    if_none(target_opt) { return; }
    let target = unwrap_(target_opt);

    if (target->image.ximage == cleared()) { return; }

    Display* dpy = (Display*)self->display;

    /* RGBA -> BGRA conversion */
    usize pixel_count = prim_min((usize)(w * h), target->image.pixels.len);
    for (usize i = 0; i < pixel_count; ++i) {
        u32 rgba = *S_at(pixels[i]);
        u8 r = (rgba >> 0) & 0xFF;
        u8 g = (rgba >> 8) & 0xFF;
        u8 b = (rgba >> 16) & 0xFF;
        u8 a = (rgba >> 24) & 0xFF;
        *S_at(target->image.pixels[i]) = (a << 24) | (r << 16) | (g << 8) | b;
    }

    XPutImage(
        dpy, (Window)target->handle, (GC)target->device,
        (XImage*)target->image.ximage,
        0, 0, 0, 0, w, h
    );
    XFlush(dpy);
}

fn_((WSI__x11_getTargetMetrics(P$dage_core_WSI self, dage_TargetId id))(dage_TargetMetrics)) {
    let target_opt = WSI__findTarget(self, id);
    if_none(target_opt) {
        return (dage_TargetMetrics){ .should_close = true };
    }
    return unwrap_(target_opt)->metrics;
}
#endif /* dage_core_WSI__use_x11 */
