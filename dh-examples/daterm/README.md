# daterm - Terminal Control Library

Low-level terminal control library for C, designed as a foundation for TUI frameworks like `datui`.

## Design Philosophy

**daterm** provides a thin wrapper around ANSI escape sequences with two key design patterns:

1. **Dual API**: Most functions come in two variants:
   - String functions: Return static strings (convenient but no error handling)
   - Writer functions: Write to `io_Writer` (proper error handling)

2. **Zero Allocation**: All string functions use static buffers
   - No dynamic allocation
   - Thread-unsafe (by design for simplicity)
   - Suitable for single-threaded TUI applications

## Module Structure

```
daterm/
├── utils.h         - ANSI escape constants and formatting helpers
├── Color.h         - 256-color and RGB color support
├── Style.h         - Text attributes (bold, italic, underline, etc.)
├── Cursor.h        - Cursor movement, visibility, save/restore
├── Clear.h         - Screen and line clearing
├── Scroll.h        - Terminal scrolling
├── Term.h          - Raw mode, size, alternate screen, mouse tracking
├── Event.h         - Input event parsing (keyboard/mouse)
└── daterm.h        - Main header (includes all modules)
```

## Comparison with mibu (Zig)

| Feature          | mibu (Zig)     | daterm (C)     |
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

**daterm** is lower-level than **datui**:

- **daterm**: ANSI sequences, terminal control, event parsing
- **datui**: Widget system, layout, rendering, application framework

Think of it this way:
- `daterm` is like SDL's input/window handling
- `datui` is like a GUI framework built on top

## Usage Examples

### Basic Color

```c
// String API (simple but no error handling)
printf("%sRed text%s\n",
    daterm_color_fgRGB(255, 0, 0),
    daterm_color_resetAll());

// Writer API (with error handling)
try_(daterm_color_fgRGBWrite(writer, 255, 0, 0));
try_(io_Writer_print(writer, u8_l("Red text")));
try_(daterm_color_resetAllWrite(writer));
```

### Raw Mode and Events

```c
var term = try_(daterm_term_init());
defer_(daterm_term_fini(&term));

while (running) {
    let event = try_(daterm_event_next(&term, reader));
    match_(event) {
    pattern_((daterm_Event_key)(key)) {
        if (daterm_event_matchesChar(event, 'q', (daterm_Event_Modifiers){})) {
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
try_(daterm_cursor_goToWrite(writer, 1, 1));
try_(daterm_clear_allWrite(writer));

// Hide cursor for rendering
try_(daterm_cursor_hideWrite(writer));
defer_(daterm_cursor_showWrite(writer) catch {});
```

## Implementation Notes

### String Buffer Management

All string-returning functions use static buffers:

```c
// These share the same buffer - don't mix in same expression!
const char* s1 = daterm_cursor_goTo(10, 20);
const char* s2 = daterm_cursor_goUp(5);  // Overwrites s1's buffer!

// Safe: Immediate use
printf("%s", daterm_cursor_goTo(10, 20));

// Unsafe: Deferred use
const char* saved = daterm_cursor_goTo(10, 20);
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

**datui** should use **daterm** like this:

```c
// datui/src/datui-core/Runtime.c
#include <daterm.h>

fn_((ansi_setFg(io_Writer writer, color_RGBA color))(E$void)) {
    return daterm_color_fgRGBWrite(writer, color.r, color.g, color.b);
}

fn_((ansi_moveTo(io_Writer writer, u16 x, u16 y))(E$void)) {
    return daterm_cursor_goToWrite(writer, x + 1, y + 1);  // ANSI is 1-indexed
}
```

This keeps **datui** focused on high-level concerns (widgets, layout, rendering) while **daterm** handles low-level terminal control.

## Future Enhancements

Potential additions:
- [ ] More comprehensive 256-color palette constants
- [ ] Title/icon setting (OSC sequences)
- [ ] Hyperlinks (OSC 8)
- [ ] Synchronized updates (DEC private modes)
- [ ] Clipboard integration
- [ ] Window resize event detection
- [ ] UTF-8 width calculations

## License

See LICENSE file for details.
