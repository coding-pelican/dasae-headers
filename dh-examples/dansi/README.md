# dansi - Terminal Control Library

Low-level terminal control library for C, designed as a foundation for TUI frameworks like `datui`.

## Design Philosophy

**dansi** provides a thin wrapper around ANSI escape sequences with two key design patterns:

1. **Dual API**: Most functions come in two variants:
   - String functions: Return static strings (convenient but no error handling)
   - Writer functions: Write to `io_Writer` (proper error handling)

2. **Zero Allocation**: All string functions use static buffers
   - No dynamic allocation
   - Thread-unsafe (by design for simplicity)
   - Suitable for single-threaded TUI applications

## Module Structure

```
dansi/
├── utils.h         - ANSI escape constants and formatting helpers
├── Color.h         - 256-color and RGB color support
├── Style.h         - Text attributes (bold, italic, underline, etc.)
├── Cursor.h        - Cursor movement, visibility, save/restore
├── Clear.h         - Screen and line clearing
├── Scroll.h        - Terminal scrolling
├── Term.h          - Raw mode, size, alternate screen, mouse tracking
├── Event.h         - Input event parsing (keyboard/mouse)
└── dansi.h        - Main header (includes all modules)
```

## Comparison with mibu (Zig)

| Feature          | mibu (Zig)     | dansi (C)     |
| ---------------- | -------------- | -------------- |
| Color Support    | ✓              | ✓              |
| Cursor Control   | ✓              | ✓              |
| Raw Mode         | ✓              | ✓              |
| Event Parsing    | ✓              | ✓              |
| Mouse Tracking   | ✓              | ✓              |
| Alternate Screen | ✓              | ✓              |
| Allocation       | Zero           | Zero           |
| Error Handling   | Zig errors     | DH-C E$ monad  |
| Platform Support | Cross-platform | Cross-platform |

## Key Differences from datui

**dansi** is lower-level than **datui**:

- **dansi**: ANSI sequences, terminal control, event parsing
- **datui**: Widget system, layout, rendering, application framework

Think of it this way:
- `dansi` is like SDL's input/window handling
- `datui` is like a GUI framework built on top

## Usage Examples

### Basic Color

```c
// String API (simple but no error handling)
printf("%sRed text%s\n",
    dansi_color_fgRGB(255, 0, 0),
    dansi_color_resetAll());

// Writer API (with error handling)
try_(dansi_color_fgRGBWrite(writer, 255, 0, 0));
try_(io_Writer_print(writer, u8_l("Red text")));
try_(dansi_color_resetAllWrite(writer));
```

### Raw Mode and Events

```c
var term = try_(dansi_term_init());
defer_(dansi_term_fini(&term));

while (running) {
    let event = try_(dansi_event_next(&term, reader));
    match_(event) {
    pattern_((dansi_Event_key)(key)) {
        if (dansi_event_matchesChar(event, 'q', (dansi_Event_Modifiers){})) {
            running = false;
        }
    } $end(pattern);
    default_() break $end(default);
    } $end(match);
}
```

### Cursor and Clearing

```c
// Move cursor and clear screen
try_(dansi_cursor_goToWrite(writer, 1, 1));
try_(dansi_clear_allWrite(writer));

// Hide cursor for rendering
try_(dansi_cursor_hideWrite(writer));
defer_(dansi_cursor_showWrite(writer) catch {});
```

## Implementation Notes

### String Buffer Management

All string-returning functions use static buffers:

```c
// These share the same buffer - don't mix in same expression!
const char* s1 = dansi_cursor_goTo(10, 20);
const char* s2 = dansi_cursor_goUp(5);  // Overwrites s1's buffer!

// Safe: Immediate use
printf("%s", dansi_cursor_goTo(10, 20));

// Unsafe: Deferred use
const char* saved = dansi_cursor_goTo(10, 20);
// ... other calls ...
printf("%s", saved);  // Buffer may have been overwritten!
```

### Platform Differences

Terminal control varies by platform:

**POSIX (Linux/macOS)**:
- Raw mode via `termios`
- ANSI sequences work directly
- Event parsing from stdin

**Windows**:
- Raw mode via `SetConsoleMode`
- Virtual Terminal Sequences (VTS) must be enabled
- Different event APIs (could use Windows Console API or VTS)

## Integration with datui

**datui** should use **dansi** like this:

```c
// datui/src/datui-core/Runtime.c
#include <dansi.h>

fn_((ansi_setFg(io_Writer writer, color_RGBA color))(E$void)) {
    return dansi_color_fgRGBWrite(writer, color.r, color.g, color.b);
}

fn_((ansi_moveTo(io_Writer writer, u16 x, u16 y))(E$void)) {
    return dansi_cursor_goToWrite(writer, x + 1, y + 1);  // ANSI is 1-indexed
}
```

This keeps **datui** focused on high-level concerns (widgets, layout, rendering) while **dansi** handles low-level terminal control.

## Future Enhancements

Potential additions:
- [x] More comprehensive 256-color palette constants
- [x] Title/icon setting (OSC sequences)
- [x] Hyperlinks (OSC 8)
- [x] Synchronized updates (DEC private modes)
<!-- - [ ] Clipboard integration -->
<!-- - [ ] Window resize event detection -->
<!-- - [ ] UTF-8 width calculations -->

## License

See LICENSE file for details.
