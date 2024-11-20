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

#define ds_Vec_capacity(_T, _vec) \
    ds_Vec__capacity((ds_Vec*)(_vec))

#define ds_Vec_append(_T, _vec, _item) \
    ds_Vec__append((ds_Vec*)(_vec), (anyptr) & (_item))

#define ds_Vec_appendAssumeCapacity(_T, _vec, _item) \
    ds_Vec__appendAssumeCapacity((ds_Vec*)(_vec), (anyptr) & (_item))

#define ds_Vec_prepend(_T, _vec, _item) \
    ds_Vec__prepend((ds_Vec*)(_vec), (anyptr) & (_item))

#define ds_Vec_prependAssumeCapacity(_T, _vec, _item) \
    ds_Vec__prependAssumeCapacity((ds_Vec*)(_vec), (anyptr) & (_item))

#define ds_Vec_pop(_T, _vec) \
    (*(_T*)ds_Vec__pop((ds_Vec*)(_vec)))

#define ds_Vec_popOrNull(_T, _vec) \
    (_T*)ds_Vec__popOrNull((ds_Vec*)(_vec))

#define ds_Vec_shift(_T, _vec) \
    (*(_T*)ds_Vec__shift((ds_Vec*)(_vec)))

#define ds_Vec_shiftOrNull(_T, _vec) \
    (_T*)ds_Vec__shiftOrNull((ds_Vec*)(_vec))

#define ds_Vec_insert(_T, _vec, _idx, _item) \
    ds_Vec__insert((ds_Vec*)(_vec), _idx, (anyptr) & (_item))

#define ds_Vec_insertAssumeCapacity(_T, _vec, _idx, _item) \
    ds_Vec__insertAssumeCapacity((ds_Vec*)(_vec), _idx, (anyptr) & (_item))

#define ds_Vec_orderedRemove(_T, _vec, _idx) \
    (_T*)ds_Vec__orderedRemove((ds_Vec*)(_vec), (_idx))

#define ds_Vec_swapRemove(_T, _vec, _idx) \
    (_T*)ds_Vec__swapRemove((ds_Vec*)(_vec), (_idx))

#define ds_Vec_getFirst(_T, _vec) \
    (*(_T*)ds_Vec__getFirst((ds_Vec*)(_vec)))

#define ds_Vec_getFirstOrNull(_T, _vec) \
    (_T*)ds_Vec__getFirstOrNull((ds_Vec*)(_vec))

#define ds_Vec_getLast(_T, _vec) \
    (*(_T*)ds_Vec__getLast((ds_Vec*)(_vec)))

#define ds_Vec_getLastOrNull(_T, _vec) \
    (_T*)ds_Vec__getLastOrNull((ds_Vec*)(_vec))

/*========== Extern Constant and Variable Declarations ======================*/
/*========== Extern Function Prototypes =====================================*/

usize ds_Vec__capacity(ds_Vec* vec);

bool ds_Vec__append(ds_Vec* vec, anyptr item);
void ds_Vec__appendAssumeCapacity(ds_Vec* vec, anyptr item);
bool ds_Vec__prepend(ds_Vec* vec, anyptr item);
void ds_Vec__prependAssumeCapacity(ds_Vec* vec, anyptr item);

anyptr ds_Vec__pop(ds_Vec* vec);
anyptr ds_Vec__popOrNull(ds_Vec* vec);
anyptr ds_Vec__shift(ds_Vec* vec);
anyptr ds_Vec__shiftOrNull(ds_Vec* vec);

bool   ds_Vec__ensureCapacity(ds_Vec* vec, usize new_capacity);
bool   ds_Vec__ensurePreciseCapacity(ds_Vec* vec, usize new_capacity);
void   ds_Vec__expandToCapacity(ds_Vec* vec);
bool   ds_Vec__insert(ds_Vec* vec, usize index, anyptr item);
void   ds_Vec__insertAssumeCapacity(ds_Vec* vec, usize index, anyptr item);
anyptr ds_Vec__orderedRemove(ds_Vec* vec, usize index);
anyptr ds_Vec__swapRemove(ds_Vec* vec, usize index);

bool ds_Vec__resize(ds_Vec* vec, usize new_len);
void ds_Vec__shrinkAndFree(ds_Vec* vec, usize new_len);
void ds_Vec__shrinkRetainingCapacity(ds_Vec* vec, usize new_len);

anyptr ds_Vec__getFirst(ds_Vec* vec);
anyptr ds_Vec__getFirstOrNull(ds_Vec* vec);
anyptr ds_Vec__getLast(ds_Vec* vec);
anyptr ds_Vec__getLastOrNull(ds_Vec* vec);

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST


void TEST_defer_cleanupVec(void* vec) {
    f32ds_Vec_fini((f32ds_Vec*)vec);
}

/* Example usage of sophisticated version */
void TEST_defer_SophisticatedExample(void) {
    block_defer {
        f32ds_Vec vec = make(f32ds_Vec);
        f32ds_Vec_init(&vec, 4);
        defer(TEST_defer_cleanupVec, &vec);

        // Use vector...

        // Optional early return:
        bool some_condition = true;
        if (some_condition) {
            block_defer_return;
        }

        // More operations...
    }
}

#endif /* UNIT_TEST */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DS_VECTOR_INCLUDED  */
