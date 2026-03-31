/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : drv_touch_scr.h
 *    Description : Touch screen driver include file
 *
 *    History :
 *    1. Date        : August, 8 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : July, 24 2013
 *       Author      : Atanas Uzunov
 *       Description : Adapted for IAR-STM32F429II-SK
 *
 *    $Revision: #3 $
 **************************************************************************/

#ifndef __DRV_TOUCH_SCR_H
#define __DRV_TOUCH_SCR_H

#define TS_INTR_PRIORITY  2
#define TS_SETUP_DLY      200     // 200us
#define TS_SAMPLE_DLY     10000   // 10ms
#define TS_INIT_DLY       50000   // 50ms
#define TS_SAMPLES        64
#define IMAGE_WIDTH       C_GLCD_H_SIZE
#define IMAGE_HEIGHT      C_GLCD_V_SIZE

typedef union
{
  uint32_t XY;
  struct
  {
    uint16_t  X;
    uint16_t  Y;
  };
} ToushRes_t;

/*************************************************************************
 * Function Name: TIM5_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Sample timer interrupt handler
 *
 *************************************************************************/
void TIM5_IRQHandler (void);

/*************************************************************************
 * Function Name: EXTI9_5_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: On touch interrupt handler
 *
 *************************************************************************/
void EXTI9_5_IRQHandler (void);

/*************************************************************************
 * Function Name: TouchScrInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init Touch screen
 *
 *************************************************************************/
void TouchScrInit (void);

/*************************************************************************
 * Function Name: TouchScrGetStatus
 * Parameters: ToushRes_t * pData X,Y data
 *
 * Return: uint32_t 0 - untouched
 *                  1 - touched
 *
 * Description: Return current state of the touchscreen
 *
 *************************************************************************/
uint32_t TouchGet (ToushRes_t * pData);

#endif // __DRV_TOUCH_SCR_H
