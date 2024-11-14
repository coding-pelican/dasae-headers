/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    scope_with.h
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


#ifndef SCOPE_WITH_INCLUDED
#define SCOPE_WITH_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include <dh/core.h>

/*========== Macros and Definitions =========================================*/

#define scope_with(INIT...) \
    IMPL_scope_with(pp_uniqueToken(_run_once), pp_uniqueToken(_init_once), INIT)

#define scope_if(INIT, COND) \
    IMPL_scope_if(pp_uniqueToken(_run_once), pp_uniqueToken(_init_once), INIT, COND)

#define scope_switch(INIT, COND) \
    IMPL_scope_switch(pp_uniqueToken(_run_once), pp_uniqueToken(_init_once), INIT, COND)

/*========== Macros Implementation ==========================================*/

/* NOLINTBEGIN */
#define IMPL_scope_with(RUN_ONCE, INIT_ONCE, INIT...)                        \
    for (bool RUN_ONCE = true, INIT_ONCE = true; RUN_ONCE; RUN_ONCE = false) \
        for (INIT; INIT_ONCE; INIT_ONCE = false)

#define IMPL_scope_if(RUN_ONCE, INIT_ONCE, INIT, COND)                       \
    for (bool RUN_ONCE = true, INIT_ONCE = true; RUN_ONCE; RUN_ONCE = false) \
        for (INIT; INIT_ONCE; INIT_ONCE = false)                             \
            if (COND)

#define IMPL_scope_switch(RUN_ONCE, INIT_ONCE, INIT, COND)                   \
    for (bool RUN_ONCE = true, INIT_ONCE = true; RUN_ONCE; RUN_ONCE = false) \
        for (INIT; INIT_ONCE; INIT_ONCE = false)                             \
            switch (COND)
/* NOLINTEND */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SCOPE_WITH_INCLUDED */
