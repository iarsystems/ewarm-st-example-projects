/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : usbd_usr.h
 *    Description : USB Device Callback Functions for USB On-The-Go Host and
 *                  Device Library by ST.
 *
 *    History :
 *    1. Date        : October 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "usbd_usr.h"
#include "usbd_ioreq.h"
#include "usb_conf.h"
#include "includes.h"

#define LED_USB_CONFIGURED    LED1

USBD_Usr_cb_TypeDef USR_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,
};

/*************************************************************************
 * Function Name: USBD_USR_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Callback function on USB init.
 *
 *************************************************************************/
void USBD_USR_Init(void)
{
}

/*************************************************************************
 * Function Name: USBD_USR_DeviceReset
 * Parameters: none
 *
 * Return: none
 *
 * Description: Callback function on USB device reset.
 *
 *************************************************************************/
void USBD_USR_DeviceReset(uint8_t speed)
{

}

/*************************************************************************
 * Function Name: USBD_USR_DeviceConfigured
 * Parameters: none
 *
 * Return: none
 *
 * Description: Callback when USB device is configured
 *
 *************************************************************************/
void USBD_USR_DeviceConfigured(void)
{
  STM_LEDOn(LED_USB_CONFIGURED);
}


/*************************************************************************
 * Function Name: USBD_USR_DeviceConnected
 * Parameters: none
 *
 * Return: none
 *
 * Description: Callback when device is connected
 *
 *************************************************************************/
void USBD_USR_DeviceConnected(void)
{

}

/*************************************************************************
 * Function Name: USBD_USR_DeviceDisconnected
 * Parameters: none
 *
 * Return: none
 *
 * Description: Callback when device is disconnected
 *
 *************************************************************************/
void USBD_USR_DeviceDisconnected(void)
{
  STM_LEDOff(LED_USB_CONFIGURED);
}

/*************************************************************************
 * Function Name: USBD_USR_DeviceSuspended
 * Parameters: none
 *
 * Return: none
 *
 * Description: Callback when device is suspended
 *
 *************************************************************************/
void USBD_USR_DeviceSuspended(void)
{

}

/*************************************************************************
 * Function Name: USBD_USR_DeviceResumed
 * Parameters: none
 *
 * Return: none
 *
 * Description: Callback when device is resumed
 *
 *************************************************************************/
void USBD_USR_DeviceResumed(void)
{
  
}
