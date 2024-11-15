/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    display.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-15 (date of creation)
 * @updated 2024-11-15 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/terminal
 * @prefix  Display
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef DISPLAY_INCLUDED
#define DISPLAY_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


/*========== Includes =======================================================*/
/*========== Macros and Definitions =========================================*/

#define Display_refresh_rate (30.0)
#define Display_delta_time   (1.0 / Display_refresh_rate)

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DISPLAY_INCLUDED */
