/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    scope.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-14 (date of creation)
 * @updated 2024-11-14 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef SCOPE_INCLUDED
#define SCOPE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "core.h"

/*========== Macros and Definitions =========================================*/

#define scope_with(INIT...) \
    IMPL_scope_with(pp_uniqueToken(run_once), pp_uniqueToken(init_once), INIT)

#define scope_if(INIT, COND) \
    IMPL_scope_if(pp_uniqueToken(run_once), pp_uniqueToken(init_once), INIT, COND)

#define scope_switch(INIT, COND) \
    IMPL_scope_switch(pp_uniqueToken(run_once), pp_uniqueToken(init_once), INIT, COND)

/*========== Macros Implementation ==========================================*/

// NOLINTBEGIN
#define IMPL_scope_with(_run_once, _init_once, INIT...)                          \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
        for (INIT; _init_once; _init_once = false)

#define IMPL_scope_if(_run_once, _init_once, INIT, COND)                         \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
        for (INIT; _init_once; _init_once = false)                               \
            if (COND)

#define IMPL_scope_switch(_run_once, _init_once, INIT, COND)                     \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
        for (INIT; _init_once; _init_once = false)                               \
            switch (COND)
// NOLINTEND

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SCOPE_INCLUDED */
