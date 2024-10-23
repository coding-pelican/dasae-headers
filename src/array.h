#ifndef ARRAY_INCLUDED
#define ARRAY_INCLUDED (1)


#include "primitive_types.h"


typedef struct Array Array;
struct Array {
    void* data_;
    usize length_;
};
/**
 * Marked Array element type.
 * Type: element data type
 */
#define Array(_Type) Array
/**
 * Construct Array composite literal.
 *
 * Type       : element data type
 * length     : elements count
 * ...        : each elements
 */
#define Array_Make(_Type, _length, ...) (           \
    (Array[1]){                                     \
        .data_   = (_Type[_length]){ __VA_ARGS__ }, \
        .length_ = _length,                         \
    }                                               \
)
/**
 * Construct Array composite literal.
 *
 * data   : elements data
 * length : elements count
 */
#define Array_WithData(_data, _length) (                 \
    (Array[1]){ .data_ = (_data), .length_ = (_length) } \
)
/**
 * Init constant Array.
 *
 * Type       : element data type
 * length     : elements count
 * ...        : each elements
 */
#define Array_Init(_Type, _length, ...)             \
    {                                               \
        .data_   = (_Type[_length]){ __VA_ARGS__ }, \
        .length_ = _length,                         \
    }
/**
 * Access Array element.
 *
 * Type       : element data type
 * array      : Array ptr
 * index      : element index of array
 */
#define Array_At(_Type, _array, _index) \
    ((Ptr(_Type))(_array)->data_ + (_index))
/**
 * Get element at index.
 *
 * Type       : element data type
 * array      : Array ptr
 * index      : element index of array
 */
#define Array_Get(_Type, _array, _index) \
    ((Ptr(_Type))(_array)->data_)[_index]
/**
 * Set element at index.
 *
 * Type       : element data type
 * array      : Array ptr
 * index      : element index of array
 * element    : element value
 */
#define Array_Set(_Type, _array, _index, _element) \
    Array_Get(_Type, _array, _index) = _element
/**
 * Array struct space and elements space in one malloc.
 * the data ptr store the offset of malloc return address.
 *
 * elementTypeSize: sizeof element type
 * length         : elements count
 */
Array* Array_New(int elementTypeSize, int length);
#endif /* ARRAY_INCLUDED */
