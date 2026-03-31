/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name      : hid_mouse.c
 *    Description    : HID module
 *
 *    History :
 *    1. Date        : December 19, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
**************************************************************************/
#include "hid_mouse.h"

#pragma data_alignment=4
Int8U DataBuffer[sizeof(UsbHidDescriptor_t)];
#pragma data_alignment=4
MouseReport_t MouseReport = {0,0,0};


/*************************************************************************
 * Function Name: HidInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init HID Mouse
 *
 *************************************************************************/
void HidInit (void)
{
  MouseReport.Buttons = 0;
  MouseReport.X = 0;
  MouseReport.Y = 0;
  USB_UserFuncRegistering(UsbClassHidDescriptor,UsbUserGetDescriptor);
  USB_UserFuncRegistering(UsbClassHidRequest,UsbUserClass);
}

/*************************************************************************
 * Function Name: UsbClassHidConfigure
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: USB Class HID configure
 *
 *************************************************************************/
void * UsbClassHidConfigure (void * pArg)
{
  if((Int32U)pArg == 0)
  {
    if(UsbCoreReq(UsbCoreReqConfiquration) != 0)
    {
      // disable all class EPs
      USB_RealizeEp((USB_Endpoint_t)ReportEp ,
                     0,
                     EP_INTERRUPT,
                     EP_SLOT1);
      USB_UserFuncRegistering(NULL,ReportEp);
    }
  }
  else
  {
    // Realize Class EPs
    USB_RealizeEp((USB_Endpoint_t)ReportEp,
                   ReportEpMaxSize,
                   EP_INTERRUPT,
                   EP_SLOT1);
  }
  return(NULL);
}

/*************************************************************************
 * Function Name: UsbClassHidDescriptor
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: Implement GET DESCRIPTOR
 *
 *************************************************************************/
void * UsbClassHidDescriptor (void * pArg)
{
UsbEpCtrl_t * UsbEp = (UsbEpCtrl_t *) pArg;
UsbSetupPacket_t * pData = (UsbSetupPacket_t *) UsbEp->pData;
  if (pData->wIndex.Word == 0)
  {
    switch (pData->wValue.Hi)
    {
    case Hid:
      UsbEp->Counter = SIZE_OF_HID_DESC;
      UsbEp->pData = (pInt8U)UsbHidDescriptor;
      return((void *)UsbPass);
    case HidReport:
      UsbEp->Counter = SIZE_OF_HID_MOUSE_DESC;
      UsbEp->pData = (pInt8U)mouseDescriptor;
      return((void *)UsbPass);
    }
  }
  return((void *)UsbFault);
}

/*************************************************************************
 * Function Name: UsbClassHidRequest
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: Implement USB Class Hid requests
 *
 *************************************************************************/
void * UsbClassHidRequest (void * pArg)
{
UsbEpCtrl_t * UsbEp = (UsbEpCtrl_t *) pArg;
UsbSetupPacket_t *pData = (UsbSetupPacket_t *)UsbEp->pData;
  switch (pData->bRequest)
  {
  case HID_GET_REPORT:
    if((pData->wValue.Hi == 1) &&
       (pData->wIndex.Word == 1))
    {
      // Send report
      UsbEp->pData = (Int8U *)&MouseReport;
      UsbEp->Counter = ReportEpMaxSize;
      return((void *)UsbPass);
    }
    break;
  }
  return((void *)UsbFault);
}

/*************************************************************************
 * Function Name: HidSendReport
 * Parameters:  Int8U Buttons, Int8S X Int8S Y
 *
 * Return: Boolean
 *
 * Description: USB HID report send
 *
 *************************************************************************/
Boolean HidSendReport (Int8U Buttons, Int8S X, Int8S Y)
{
Int32U ReportEpSize;
  MouseReport.Buttons = Buttons;
  MouseReport.X = X;
  MouseReport.Y = Y;
  ReportEpSize = ReportEpMaxSize;
  return (USB_EpWrite((USB_Endpoint_t)ReportEp,(pInt8U)&MouseReport,&ReportEpSize) == USB_OK);
}
