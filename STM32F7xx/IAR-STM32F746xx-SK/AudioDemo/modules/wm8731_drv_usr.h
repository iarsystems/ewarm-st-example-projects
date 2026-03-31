/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : wm8731_drv_usr.h
 *    Description : Wolfson WM8731 Audio Codec driver user definitions
 *
 *    History :
 *    1. Date        : October, 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: #1 $
 **************************************************************************/

#ifndef __WM8731_DRV_USR_H
#define __WM8731_DRV_USR_H

#include "iar_stm32f746xx_sk.h"

#define I2C_OPEN_FUNC()                   
#define I2C_SEND_FUNC(addr, pData, len)   HAL_I2C_Master_Transmit(hi2c, (addr<<1), (unsigned char*)pData, len, 1000)
#define I2C_CLOSE_FUNC()             
#define SUCCESS_RESULT   (HAL_OK)
#define FAIL_RESULT      (!HAL_OK)

#endif // __WM8731_DRV_USR_H
