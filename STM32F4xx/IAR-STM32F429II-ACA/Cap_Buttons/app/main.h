/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2014
 *
 *    File name   : main.h
 *    Description : Main module header
 *
 *    History :
 *    1. Date        : October 2014
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

/* application states */
typedef enum {
  stCapBut1Discharge,
  stCapBut1PrepareAndStart,
  stCapBut1Charging,
  stCapBut2Discharge,
  stCapBut2PrepareAndStart,
  stCapBut2Charging
} CapButtonsState_TypeDef;

