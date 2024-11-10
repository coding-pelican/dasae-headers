/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    vector.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-05 (date of creation)
 * @updated 2024-11-05 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/ds
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef DS_VECTOR_INCLUDED
#define DS_VECTOR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "container.h"

/*========== Forward Declarations ===========================================*/
/*========== Macros and Definitions =========================================*/

#define Vector_capacity(_T, _vec) \
    Vector__capacity((Vector*)(_vec))

#define Vector_append(_T, _vec, _item) \
    Vector__append((Vector*)(_vec), (anyptr) & (_item))

#define Vector_appendAssumeCapacity(_T, _vec, _item) \
    Vector__appendAssumeCapacity((Vector*)(_vec), (anyptr) & (_item))

#define Vector_prepend(_T, _vec, _item) \
    Vector__prepend((Vector*)(_vec), (anyptr) & (_item))

#define Vector_prependAssumeCapacity(_T, _vec, _item) \
    Vector__prependAssumeCapacity((Vector*)(_vec), (anyptr) & (_item))

#define Vector_pop(_T, _vec) \
    (*(_T*)Vector__pop((Vector*)(_vec)))

#define Vector_popOrNull(_T, _vec) \
    (_T*)Vector__popOrNull((Vector*)(_vec))

#define Vector_shift(_T, _vec) \
    (*(_T*)Vector__shift((Vector*)(_vec)))

#define Vector_shiftOrNull(_T, _vec) \
    (_T*)Vector__shiftOrNull((Vector*)(_vec))

#define Vector_insert(_T, _vec, _idx, _item) \
    Vector__insert((Vector*)(_vec), _idx, (anyptr) & (_item))

#define Vector_insertAssumeCapacity(_T, _vec, _idx, _item) \
    Vector__insertAssumeCapacity((Vector*)(_vec), _idx, (anyptr) & (_item))

#define Vector_orderedRemove(_T, _vec, _idx) \
    (_T*)Vector__orderedRemove((Vector*)(_vec), (_idx))

#define Vector_swapRemove(_T, _vec, _idx) \
    (_T*)Vector__swapRemove((Vector*)(_vec), (_idx))

#define Vector_getFirst(_T, _vec) \
    (*(_T*)Vector__getFirst((Vector*)(_vec)))

#define Vector_getFirstOrNull(_T, _vec) \
    (_T*)Vector__getFirstOrNull((Vector*)(_vec))

#define Vector_getLast(_T, _vec) \
    (*(_T*)Vector__getLast((Vector*)(_vec)))

#define Vector_getLastOrNull(_T, _vec) \
    (_T*)Vector__getLastOrNull((Vector*)(_vec))

/*========== Extern Constant and Variable Declarations ======================*/
/*========== Extern Function Prototypes =====================================*/

usize Vector__capacity(Vector* vec);

bool Vector__append(Vector* vec, anyptr item);
void Vector__appendAssumeCapacity(Vector* vec, anyptr item);
bool Vector__prepend(Vector* vec, anyptr item);
void Vector__prependAssumeCapacity(Vector* vec, anyptr item);

anyptr Vector__pop(Vector* vec);
anyptr Vector__popOrNull(Vector* vec);
anyptr Vector__shift(Vector* vec);
anyptr Vector__shiftOrNull(Vector* vec);

bool   Vector__ensureCapacity(Vector* vec, usize new_capacity);
bool   Vector__ensurePreciseCapacity(Vector* vec, usize new_capacity);
void   Vector__expandToCapacity(Vector* vec);
bool   Vector__insert(Vector* vec, usize index, anyptr item);
void   Vector__insertAssumeCapacity(Vector* vec, usize index, anyptr item);
anyptr Vector__orderedRemove(Vector* vec, usize index);
anyptr Vector__swapRemove(Vector* vec, usize index);

bool Vector__resize(Vector* vec, usize new_len);
void Vector__shrinkAndFree(Vector* vec, usize new_len);
void Vector__shrinkRetainingCapacity(Vector* vec, usize new_len);

anyptr Vector__getFirst(Vector* vec);
anyptr Vector__getFirstOrNull(Vector* vec);
anyptr Vector__getLast(Vector* vec);
anyptr Vector__getLastOrNull(Vector* vec);

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST


void TEST_defer_cleanupVec(void* vec) {
    f32Vector_fini((f32Vector*)vec);
}

/* Example usage of sophisticated version */
void TEST_defer_SophisticatedExample(void) {
    defer_scope {
        f32Vector vec = make(f32Vector);
        f32Vector_init(&vec, 4);
        defer(TEST_defer_cleanupVec, &vec);

        // Use vector...

        // Optional early return:
        bool some_condition = true;
        if (some_condition) {
            defer_scope_return;
        }

        // More operations...
    }
}

#endif /* UNIT_TEST */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DS_VECTOR_INCLUDED  */
