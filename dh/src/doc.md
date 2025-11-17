# DH-C DSL Documentation

## Implementation Summary

### fmt_common.c Completion Status

The formatting implementation has been completed with the following components:

1. **Format Specification Parsing** (`parseFormatSpec`)
   - Parses format strings like `{[index]:[fill][align][width].[precision][type][size]}`
   - Supports optional/result wrappers (? and !)
   - Validates format specifiers and returns proper errors

2. **Argument Collection** (`collectArg`, `collectArgValue`, `collectArgOptionalValue`, `collectArgErrorResultValue`)
   - Collects variadic arguments using `va_list`
   - Supports regular values, optional values (O$), and error results (E$)
   - Type-safe variant-based argument storage

3. **Value Formatting** (`formatArgValue`, `formatArgType`)
   - Formats all basic types (integers, floats, bools, strings, pointers, errors)
   - Handles optional and result wrappers
   - Applies format specifications (width, alignment, fill, precision)

4. **Specific Formatters**
   - `formatUnsigned`: Handles hex, octal, binary, decimal unsigned integers
   - `formatSigned`: Handles signed integers with proper sign extension
   - `formatFloat`: Handles floating point with precision, special values (NaN, Inf)
   - `formatBool`: Formats boolean as "true"/"false"
   - `formatPointer`: Formats pointers as "0x..." hex
   - `formatChar`: Formats single ASCII character
   - `formatUTF8`: Formats UTF-8 codepoint
   - `formatString`: Formats string slices and null-terminated strings
   - `formatError`: Formats error objects as "[Domain] Code"

5. **Main Formatting Function** (`fmt_format`, `fmt_formatVaArgs`)
   - Parses format string and collects all format specs
   - Builds argument descriptors and validates indices
   - Collects arguments from va_list
   - Outputs formatted text with proper escape handling ({{ and }})

### Key Design Decisions

- Used variant types for type-safe argument storage
- Separated formatting logic into small, focused functions
- Proper handling of optional types (prints "none" when absent)
- Proper handling of error results (prints error details when failed)
- Zero-copy string handling where possible
- Compile-time format string validation (indices, types)

# DH-C DSL Documentation

## Type System

### Optional Types (`O$`)
- Structure: `union { bool is_some; union { Void none; T some; } payload; }`
- Check: `isSome(opt)`, `isNone(opt)`
- Create: `some(value)`, `none()`
- Access:
  - Internally: `opt.payload.some` when `is_some` is true
  - Unwrap: `unwrap_(opt)` - panics if none
  - OrElse: `orelse_(opt, default_value)` - returns default value if none

### Error Result Types (`E$`)
- Structure: `union { bool is_ok; union { Err err; T ok; } payload; }`
- Check: `isOk(result)`, `isErr(result)`
- Create: `ok(value)`, `err(error_value)`
- Access:
  - Internally: `result.payload.ok` when `is_ok` is true, `result.payload.err` otherwise
  - Propagate: `try_(expr)` - returns error if expr is error
  - Handle: `catch_((expr)(e, handler))` - handles error with handler

### Variant Types
- Created with `variant_((Name)(Tag, Type)...)`
- Access tag: `variant.tag`
- Access payload: `variant.payload.$tag$TagName[0]` (note the `$tag` prefix and `[0]` suffix)
- Create: `union_of((TagName)(value))`
- Extract: `variant_extract(variant, TagName)`

## Memory and String Operations

### mem_lenZ0$u8
- Finds length of null-terminated string
- Returns `usize`

### Slice Operations
- `S_const$u8`: constant byte slice with `.ptr` and `.len`
- String literal: `u8_l("text")` creates `S_const$u8`
- Access element: `atS(slice, index)` or `sliceS(slice, range)` macros

## Assignment and Literals

### asg_lit
- Syntax: `asg_lit(&pointer, initializer_or_value)`
- Assigns literal value or initializer to dereferenced pointer
- Verifies pointer is non-null

### lit$
- Syntax: `lit$((Type)initializer)`
- Creates compound literal
- In C++, becomes `Type`; in C, becomes `(Type)initializer`

## Formatting Types

### Format Spec Fields
- `index`: O$u8 - argument index
- `fill`: O$u8 - fill character
- `align`: O$fmt_Align - alignment
- `width`: O$u8 - minimum width
- `precision`: O$u8 - decimal precision
- `type_prefix`: O$fmt_TypePrefix - optional/result wrapper
- `type`: fmt_Type - format type character
- `size`: fmt_Size - size modifier

### Type Mapping
Type characters map to `fmt_Type` enum:
- 'x', 'X' → hex (lower/upper)
- 'o' → octal
- 'b' → binary
- 'B' → boolean
- 'u' → unsigned
- 'd', 'i' → signed
- 'f', 'F' → float
- 'p' → pointer
- 'e' → error
- '0' → void
- 'c' → ASCII character
- 'C' → UTF-8 codepoint
- 'z' → null-terminated string
- 's' → slice string

Size modifiers map to `fmt_Size` enum:
- 'hh' → 8-bit
- 'h' → 16-bit
- (none) → 32-bit (default)
- 'l' → 64-bit
- 'z' → pointer size

## Variant Payload Access Pattern

For fmt_ArgValue variant:
```c
typedef variant_((fmt_ArgValue)(
    (fmt_ArgValue_void, Void),
    (fmt_ArgValue_bool, bool),
    (fmt_ArgValue_u8, u8),
    // ...
)) fmt_ArgValue;
```

Access patterns:
- Tag: `arg.tag`
- Payload for `fmt_ArgValue_u32`: `arg.payload.$tag$fmt_ArgValue_u32[0]`

Note the $ prefix: `$tag$<TagName>`
