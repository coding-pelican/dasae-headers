/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    struct.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef CORE_PRIM_STRUCT_INCLUDED
#define CORE_PRIM_STRUCT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "int.h"

/*========== Macros and Definitions =========================================*/

#define typedef_struct(Alias)   \
    typedef struct Alias Alias; \
    struct Alias

#define initial(_Inital...)     IMPL_initial(_Inital)
#define cleared()               IMPL_cleared()
#define make(_T, _Inital...)    IMPL_make(_T, _Inital)
#define makeCleared(_T)         IMPL_makeCleared(_T)
#define create(_T, _Initial...) IMPL_create(_T, _Initial)
#define createCleared(_T)       IMPL_createCleared(_T)
#define createFrom(_T, _var...) IMPL_createFrom(_T, _var)

#define getContainer(_ptr_member, _TContainer)                        \
    /**                                                               \
     * Get struct pointer from member pointer.                        \
     * this for _ptr_member same as _MemberName.                      \
     *                                                                \
     * _ptr_member: the pointer that point struct's member.           \
     *            it's the member address offset from struct address. \
     */                                                               \
    IMPL_getContainer(_ptr_member, _TContainer)

#define getContainerByFieldName(_ptr_member, _TContainer, _MemberName) \
    /**                                                                \
     * Get struct pointer from member pointer with _MemberName.        \
     * this for _ptr_member not same as _MemberName.                   \
     *                                                                 \
     * _ptr_member: the pointer that point struct's member.            \
     *            it's the member address offset from struct address.  \
     */                                                                \
    IMPL_getContainerByFieldName(_ptr_member, _TContainer, _MemberName)

#define reserveReturn(_T) \
    IMPL_reserveReturn(_T)

#define getReservedReturn() \
    IMPL_getReservedReturn()

#define setReservedReturn(_val_return...) \
    IMPL_setReservedReturn(_val_return)

/*========== Macros Implementation ==========================================*/

#define IMPL_initial(_Inital...) \
    { _Inital }

#define IMPL_cleared() \
    { 0 }

// NOLINTBEGIN(bugprone-macro-parentheses)
#define IMPL_make(_T, _Inital...) \
    ((_T){ _Inital })

#define IMPL_makeCleared(_T) \
    ((_T){ 0 })

#define IMPL_create(_T, _Inital...) \
    ((_T[1]){ (_T){ _Inital } })

#define IMPL_createCleared(_T) \
    ((_T[1]){ (_T){ 0 } })

#define IMPL_createFrom(_T, _var...) \
    ((_T[1]){ _var })
// NOLINTEND(bugprone-macro-parentheses)

#define IMPL_getContainer(_ptr_member, _TContainer) \
    ((Parent_TContainerType*)((u8*)(_ptr_member) - offsetof(_TContainer, _ptr_member)))

#define IMPL_getContainerByFieldName(_ptr_member, _TContainer, _MemberName) \
    ((_TContainer*)((u8*)(_ptr_member) - offsetof(_TContainer, _MemberName)))

#define IMPL_reserveReturn(_T) \
    _T _reserved_return = cleared()

#define IMPL_getReservedReturn() \
    (_reserved_return)

#define IMPL_setReservedReturn(_val_return...) \
    (getReservedReturn() = (_val_return))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_STRUCT_INCLUDED */
