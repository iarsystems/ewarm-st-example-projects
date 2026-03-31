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
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __WM8731_DRV_USR_H
#define __WM8731_DRV_USR_H

#include "iar_stm32f429ii_sk.h"
#include "i2c2_drv.h"

#define I2C_OPEN_FUNC()                   I2C2_Open()
#define I2C_SEND_FUNC(addr, pData, len)   I2C2_DataTransfer((addr<<1), (unsigned char*)pData, len)
#define I2C_CLOSE_FUNC()                  I2C2_Close()

#endif // __WM8731_DRV_USR_H
