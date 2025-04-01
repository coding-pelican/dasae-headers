---
title: API Reference
description: Complete API documentation for the dasae-headers library
created: 2025-04-01
updated: 2025-04-01
version: v0.1
target_version: pre-alpha
---

# API Reference

This section provides detailed documentation for all modules in the dasae-headers library.

## Data Structures

- [Fixed-Size Arrays (Arr)](./Arr.md) - Type-safe fixed-size arrays with bounds checking
- [Dynamic Array Lists (ArrList)](./ArrList.md) - Automatically resizing arrays
- Slices - View into arrays and other sequences
- Strings - UTF-8 string handling
- Hash Maps - Key-value associative arrays

## Memory Management

- Allocators - Custom memory allocators
- Memory Tracking - Leak detection and diagnostics
- Ownership - Control of memory lifetime
- Deferred Execution - Automated cleanup

## Error Handling

- Error Sets - Define custom error types
- Result Types - Safe error propagation
- Error Tracing - Track error origin
- Error Contexts - Additional error information

## Type System

- Type Information - Runtime type data
- Optional Types - Safe nullable values
- Union Enums - Type-safe unions
- Pattern Matching - Safe type discrimination

## Testing

- Test Framework - Unit and integration tests
- Assertions - Verify conditions during testing
- Benchmarking - Performance measurement

## Utility

- Formatting - String formatting
- Logging - Structured logging
- File I/O - Safe file operations
- Random - Secure random number generation
