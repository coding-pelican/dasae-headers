// Vector
typedef struct Vec {
    Slice  items;
    usize  capacity;
    anyptr allocator; // TODO: Define allocator interface
} Vec;
