/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : keys.h
 *    Description : keys include file
 *
 *    History :
 *    1. Date        : October 10, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#ifndef  __KEYS_H
#define  __KEYS_H

typedef union _Key_t
{
  Int32U Data;
  struct
  {
    Int32U PlayStop     : 1;
    Int32U Next         : 1;
  };
} Key_t, *pKey_t;

#define KeyPlayStopMask (1UL << 0)
#define KeyNextMask     (1UL << 1)

#define KEY_PRESS_DLY 2

/*************************************************************************
 * Function Name: KeyInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init key
 *
 *************************************************************************/
void KeyInit(void);

/*************************************************************************
 * Function Name: GetKeys
 * Parameters: none
 *
 * Return: none
 *
 * Description: Return pressed keys
 *
 *************************************************************************/
Key_t GetKeys (void);

/*************************************************************************
 * Function Name: GenreteKeyPress
 * Parameters: Int32U Keys
 *
 * Return: none
 *
 * Description: Generate key press
 *
 *************************************************************************/
void GenreteKeyPress (Int32U Keys);

/*************************************************************************
 * Function Name: KeyImpl
 * Parameters: none
 *
 * Return: none
 *
 * Description: Scan keys
 *
 *************************************************************************/
void KeyImpl (void);

#endif  /* __KEYS_H */
