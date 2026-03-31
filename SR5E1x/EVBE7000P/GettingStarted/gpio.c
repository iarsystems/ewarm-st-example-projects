/****************************************************************************
*
* Copyright (c) 2022 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0098 at www.st.com.
*
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
* EVALUATION ONLY - NOT FOR USE IN PRODUCTION
*****************************************************************************/
/**
 * @file    gpio.c
 * @brief   GPIO driver source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup GPIO
 * @ingroup SYSTEM
 * @{
 */

#include <gpio.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Local function prototypes                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void gpio_init(void) {

    uint32_t enr = 0U
#if defined(GPIOA)
      | RCC_AHB2LENR_GPIOA
#endif
#if defined(GPIOB)
      | RCC_AHB2LENR_GPIOB
#endif
#if defined(GPIOC)
      | RCC_AHB2LENR_GPIOC
#endif
#if defined(GPIOD)
      | RCC_AHB2LENR_GPIOD
#endif
#if defined(GPIOE)
      | RCC_AHB2LENR_GPIOE
#endif
#if defined(GPIOF)
      | RCC_AHB2LENR_GPIOF
#endif
#if defined(GPIOG)
      | RCC_AHB2LENR_GPIOG
#endif
#if defined(GPIOH)
      | RCC_AHB2LENR_GPIOH
#endif
#if defined(GPIOI)
      | RCC_AHB2LENR_GPIOI
#endif
      ;

    /* Enabling GPIO RCC clocks, reading back for synchronization.*/
    RCC->AHB2LENR |= enr;
    (void)RCC->AHB2LENR;
}

/** @} */
