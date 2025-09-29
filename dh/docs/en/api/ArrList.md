---
title: Dynamic Array Lists (ArrList)
description: Documentation for the ArrList module that provides dynamic resizable arrays
created: 2025-04-01
updated: 2025-04-01
version: v0.1
target_version: pre-alpha
---

# Dynamic Array Lists (ArrList)

## Overview

The `ArrList` module provides a dynamic array implementation that automatically resize as elements are added or removed. It is based on Zig's `ArrayList` (similar to C++'s `std::vector` or Rust's `Vec<T>`), and offers memory-safe and convenient operations for managing elements in collections.

## Features

- Automatic memory management and resizing
- Type-safe operations
- Bounds checking
- Efficient append/prepend operations
- Insertion and removal at arbitrary positions
- Memory capacity management

## Types

### Base Structure

```c
typedef struct ArrList {
    meta_Sli      items;     // Slice containing the elements
    usize         cap;       // Current capacity
    mem_Allocator allocator; // Memory allocator to use
} ArrList;
```

### Typed Variant

```c
typedef union ArrList$T {
    ArrList base[1];
    struct {
        TypeInfo      type;      // Type information
        Sli$T         items;     // Typed slice of elements
        usize         cap;       // Current capacity
        mem_Allocator allocator; // Memory allocator
    };
} ArrList$T;
```

### Declaration

```c
// Declare and implement in one step
use_ArrList$(i32);  // Creates a typed array list for integers

// Separate declaration and implementation
decl_ArrList$(f32);
impl_ArrList$(f32);

// Type aliases
ArrList$i32 myList;  // Typed array list for integers
```

## Initialization

```c
// Initialize an empty list
ArrList list = ArrList_init(typeInfo$(i32), heap_allocator);

// Typed initialization
ArrList$i32 typed_list = type$(ArrList$i32,
    ArrList_init(typeInfo$(i32), heap_allocator));

// Initialize with capacity
Err$ArrList result = ArrList_initCap(typeInfo$(f32), heap_allocator, 10);
if_ok(result, list) {
    // Use list
}
```

## Basic Operations

### Adding Elements

```c
// Add to the end
try_(ArrList_append(list.base, &value));

// Add multiple elements to the end
try_(ArrList_appendSli(list.base, slice));

// Add to the front
try_(ArrList_prepend(list.base, &value));

// Add and get pointer to new element
meta_Ptr item = try_(ArrList_addBackOne(list.base));
```

### Removing Elements

```c
// Remove from the end
ArrList_pop(list.base);

// Remove and get item from the end
Opt$meta_Ptr opt_item = ArrList_popOrNull(list.base);

// Remove from the front
ArrList_shift(list.base);

// Remove at specific index (preserving order)
meta_Ptr removed = ArrList_removeOrdered(list.base, 2);

// Remove at specific index (fast, doesn't preserve order)
meta_Ptr removed = ArrList_removeSwap(list.base, 2);
```

### Capacity Management

```c
// Ensure total capacity
try_(ArrList_ensureTotalCap(list.base, 20));

// Resize the list
try_(ArrList_resize(list.base, 10));

// Shrink and free excess memory
ArrList_shrinkAndFree(list.base, 5);

// Clear but keep capacity
ArrList_clearRetainingCap(list.base);

// Clear and free all memory
ArrList_clearAndFree(list.base);
```

## Resource Management

Remember to free resources when done:

```c
// Finalize and release resources
ArrList_fini(list.base);
```

## Examples

### Basic Usage

```c
#include "dh/main.h"
#include "dh/ArrList.h"

fn_(dh_main(Sli$Sli_const$u8 args), Err$void $guard) {
    // Initialize array list
    ArrList$i32 list = type$(ArrList$i32,
        ArrList_init(typeInfo$(i32), heap_allocator));

    // Resource cleanup when function exits
    defer_(ArrList_fini(list.base));

    // Add elements
    i32 values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        try_(ArrList_append(list.base, &values[i]));
    }

    // Print elements
    printf("List contents: ");
    for_slice (list.items, item) {
        printf("%d ", *item);
    }
    printf("\n");

    // Remove an element
    ArrList_removeOrdered(list.base, 2);

    // Print updated list
    printf("After removal: ");
    for_slice (list.items, item) {
        printf("%d ", *item);
    }
    printf("\n");

    return_ok({});
} $unguarded;
```

### Dynamic Collection

```c
fn_(processDynamicData(Sli$i32 input), Err$Sli$i32 $guard) {
    // Initialize with allocator
    ArrList$i32 results = type$(ArrList$i32,
        ArrList_init(typeInfo$(i32), heap_allocator));

    // Clean up resources on function exit
    defer_(ArrList_fini(results.base));

    // Process input data
    for_slice (input, item) {
        if (*item % 2 == 0) {
            // Process even numbers
            i32 processed = *item * 2;
            try_(ArrList_append(results.base, &processed));
        }
    }

    // Convert to owned slice and return (caller takes ownership)
    meta_Sli owned = try_(ArrList_toOwnedSli(results.base));
    return_ok(meta_cast$(Sli$i32, owned));
} $unguarded;
```

## API Reference

### Type Management

- `use_ArrList$(T)` - Declare and implement typed array list
- `decl_ArrList$(T)` - Declare typed array list
- `impl_ArrList$(T)` - Implement typed array list
- `ArrList$(T)` - Create array list type
- `ArrList$$(T)` - Create anonymous array list
- `ArrList_anonCast$(T_ArrList, var_anon...)` - Convert anonymous array list to specific type

### Initialization/Finalization

- `ArrList_init(TypeInfo type, mem_Allocator allocator)` - Initialize an array list
- `ArrList_initCap(TypeInfo type, mem_Allocator allocator, usize cap)` - Initialize with capacity
- `ArrList_fini(ArrList* self)` - Finalize an array list

### Conversion

- `ArrList_toOwnedSli(ArrList* self)` - Convert to owned slice
- `ArrList_fromOwnedSli(mem_Allocator allocator, meta_Sli slice)` - Create from owned slice
- `ArrList_clone(const ArrList* self)` - Clone an array list

### Capacity Management

- `ArrList_ensureTotalCap(ArrList* self, usize new_cap)` - Ensure total capacity
- `ArrList_ensureTotalCapPrecise(ArrList* self, usize new_cap)` - Ensure precise capacity
- `ArrList_ensureUnusedCap(ArrList* self, usize additional)` - Ensure unused capacity

### Size Operations

- `ArrList_resize(ArrList* self, usize new_len)` - Resize to new length
- `ArrList_shrinkAndFree(ArrList* self, usize new_len)` - Shrink and free excess memory
- `ArrList_shrinkRetainingCap(ArrList* self, usize new_len)` - Shrink without freeing
- `ArrList_expandToCap(ArrList* self)` - Expand to full capacity

### Adding Elements

- `ArrList_append(ArrList* self, meta_Ptr item)` - Add to end
- `ArrList_appendSli(ArrList* self, meta_Sli items)` - Add slice to end
- `ArrList_prepend(ArrList* self, meta_Ptr item)` - Add to front
- `ArrList_prependSli(ArrList* self, meta_Sli items)` - Add slice to front
- `ArrList_addBackOne(ArrList* self)` - Add one to end and get pointer
- `ArrList_addFrontOne(ArrList* self)` - Add one to front and get pointer

### Removing Elements

- `ArrList_pop(ArrList* self)` - Remove from end
- `ArrList_popOrNull(ArrList* self)` - Remove and return from end
- `ArrList_shift(ArrList* self)` - Remove from front
- `ArrList_shiftOrNull(ArrList* self)` - Remove and return from front
- `ArrList_removeOrdered(ArrList* self, usize index)` - Remove at index preserving order
- `ArrList_removeSwap(ArrList* self, usize index)` - Remove at index by swapping

### Clearing

- `ArrList_clearRetainingCap(ArrList* self)` - Clear but keep capacity
- `ArrList_clearAndFree(ArrList* self)` - Clear and free all memory
