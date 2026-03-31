/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name      : hid_mouse.h
 *    Description    : HID mouse device header file
 *
 *    History :
 *    1. Date        : December 19, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
**************************************************************************/
#include <includes.h>

#ifndef __HID_MOUSE_H
#define __HID_MOUSE_H

extern const Int16U mouseDescriptor[];
extern const Int8U UsbHidDescriptor[];

#define SIZE_OF_HID_MOUSE_DESC  0x32
#define SIZE_OF_HID_DESC        sizeof(UsbHidDescriptor_t)

#pragma pack(1)

typedef struct
{
  Int8U Buttons;
  Int8U X;
  Int8U Y;
} MouseReport_t;

#pragma pack()

/*************************************************************************
 * Function Name: HidInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init HID Mouse
 *
 *************************************************************************/
void HidInit (void);

/*************************************************************************
 * Function Name: UsbClassHidConfigure
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: USB Class HID configure
 *
 *************************************************************************/
void * UsbClassHidConfigure (void * pArg);

/*************************************************************************
 * Function Name: UsbClassHidDescriptor
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: Implement GET DESCRIPTOR
 *
 *************************************************************************/
void * UsbClassHidDescriptor (void * pArg);

/*************************************************************************
 * Function Name: UsbClassHidRequest
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: Implement USB Class Hid requests
 *
 *************************************************************************/
void * UsbClassHidRequest (void * pArg);

/*************************************************************************
 * Function Name: HidSendReport
 * Parameters:  Int8U Buttons, Int8S X Int8S Y
 *
 * Return: Boolean
 *
 * Description: USB HID report send
 *
 *************************************************************************/
Boolean HidSendReport (Int8U Buttons, Int8S X, Int8S Y);

#endif //__HID_MOUSE_H

