---
title: Fixed-Size Arrays (Arr)
description: Documentation for the Arr module that provides type-safe fixed-size arrays with bounds checking
created: 2025-04-01
updated: 2025-04-01
version: v0.1
target_version: pre-alpha
---

# Fixed-Size Arrays (Arr)

## Overview

The `Arr` module provides type-safe fixed-size arrays with bounds checking and convenient operations. Unlike standard C arrays, `Arr` arrays offer safety guarantees and a more expressive API.

## Features

- Compile-time size and element type checking
- Automatic bounds checking in debug mode
- Constant (immutable) and mutable array variants
- Rich slicing operations
- Convenient iteration patterns
- Zero-overhead abstraction in release builds

## Types

### Declaration

Arrays can be declared and implemented in several ways:

```c
// Declare and implement in one step
use_Arr$(3, i32);  // Creates a fixed-size array of 3 integers

// Declare without implementation
decl_Arr$(4, f32);

// Implement a previously declared type
impl_Arr$(4, f32);
```

### Type Aliases

```c
// Create a constant (immutable) array type
Arr_const$(3, i32) constArr;  // Constant array of 3 integers

// Create a mutable array type
Arr$(5, f32) arr;  // Mutable array of 5 floats

// For typedefs (anonymous array types)
typedef Arr$$(4, u8) ByteQuad;  // Type definition for a 4-byte array
```

## Initialization

Arrays can be initialized in several ways:

```c
// Zero initialization
Arr$(3, i32) arr1 = Arr_zero();  // {0, 0, 0}

// Value initialization
Arr$(3, i32) arr2 = Arr_init({ 1, 2, 3 });  // {1, 2, 3}

// Anonymous array initialization
var anon1 = Arr_zero$(Arr$$(3, i32));  // Anonymous array with zeros
var anon2 = Arr_init$(Arr$$(3, i32), { 1, 2, 3 });  // With values

// Type conversion
var typed1 = Arr_anonCast$(Arr$3$i32, anon1);
var typed2 = Arr_anonCast$(Arr$(3, i32), anon2);
```

## Basic Operations

### Array Information

```c
// Get array length
usize len = Arr_len(arr);  // Returns the number of elements
```

### Element Access

```c
// Get pointer to element (bounds-checked)
i32* ptr = Arr_at(arr, 1);

// Get element value (bounds-checked)
i32 val = Arr_getAt(arr, 1);

// Set element value (bounds-checked)
Arr_setAt(arr, 1, 5);  // arr = {1, 5, 3}
```

## Iteration

Arrays can be iterated in several ways:

```c
// Forward iteration
for_array (arr, item) {
    printf("%d ", *item);  // Prints: 1 2 3
}

// Forward iteration with index
for_array_indexed (arr, item, idx) {
    printf("[%zu]=%d ", idx, *item);  // Prints: [0]=1 [1]=2 [2]=3
}

// Reverse iteration
for_array_rev (arr, item) {
    printf("%d ", *item);  // Prints: 3 2 1
}

// Reverse iteration with index
for_array_rev_indexed (arr, item, idx) {
    printf("[%zu]=%d ", idx, *item);  // Prints: [2]=3 [1]=2 [0]=1
}
```

## Slicing Operations

Arrays can be sliced to create views of portions of the array:

```c
// Get slice from begin to end
Sli$i32 slice = Arr_slice(arr, (1, 2));  // Slice from index 1 to 2 (exclusive)

// Type-specific slice
Sli$i32 typed_slice = Arr_slice$(Sli$i32, arr, (1, 2));

// Get slice from beginning to index
Sli$i32 prefix = Arr_prefix(arr, 2);  // First 2 elements

// Get slice from index to end
Sli$i32 suffix = Arr_suffix(arr, 1);  // Last 2 elements
```

## Complete Example

```c
#include "dh/main.h"
#include "dh/Arr.h"

fn_scope(dh_main(Sli$Str_const args), Err$void) {
    // Declare and initialize a fixed-size array
    Arr$(3, i32) numbers = Arr_init({ 10, 20, 30 });

    // Get length
    printf("Array length: %zu\n", Arr_len(numbers));

    // Access elements
    printf("Second element: %d\n", Arr_getAt(numbers, 1));

    // Modify elements
    Arr_setAt(numbers, 1, 25);

    // Iteration
    printf("Array elements: ");
    for_array (numbers, item) {
        printf("%d ", *item);
    }
    printf("\n");

    // Create a slice
    Sli$i32 slice = Arr_slice(numbers, (0, 2));
    printf("Slice elements: ");
    for_slice (slice, item) {
        printf("%d ", *item);
    }
    printf("\n");

    return_ok({});
} unscoped;
```

## API Reference

### Type Declarations

- `use_Arr$(N, T)` - Declare and implement array type in one step
- `decl_Arr$(N, T)` - Declare array type without implementation
- `impl_Arr$(N, T)` - Implement previously declared array type

### Type Aliases

- `Arr_const$(N, T)` - Create a constant array type with N elements of type T
- `Arr$(N, T)` - Create a mutable array type with N elements of type T
- `Arr_const$$(N, T)` - Create anonymous constant array type
- `Arr$$(N, T)` - Create anonymous mutable array type
- `Arr_anonCast$(T_Arr, var_anon...)` - Convert anonymous array to specific type

### Initialization

- `Arr_zero()` - Initialize array with zeros
- `Arr_zero$(T_Arr)` - Initialize specific array type with zeros
- `Arr_init(_Initial...)` - Initialize array with specified values
- `Arr_init$(T_Arr, _Initial...)` - Initialize specific array type with values

### Operations

- `Arr_len(var_self...)` - Get number of elements in array
- `Arr_at(var_self, usize_index...)` - Get pointer to element at index (bounds-checked)
- `Arr_getAt(var_self, usize_index...)` - Get value of element at index (bounds-checked)
- `Arr_setAt(var_self, usize_index, val_item...)` - Set value at index (bounds-checked)

### Iteration

- `for_array(var_arr, _Iter_item)` - Forward iteration over elements
- `for_array_indexed(var_arr, _Iter_item, _Iter_index)` - Forward iteration with index
- `for_array_rev(var_arr, _Iter_item)` - Reverse iteration over elements
- `for_array_rev_indexed(var_arr, _Iter_item, _Iter_index)` - Reverse iteration with index
