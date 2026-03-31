/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2014
 *
 *    File name   : usb_bsp.c
 *    Description : USB BSP Initializations for USB On-The-Go Host and
 *                  Device Library by ST. Adapted for IAR-STM32F429II-ACA board.
 *
 *    History :
 *    1. Date        : October, 2014
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "usb_bsp.h"
#include "usbd_conf.h"
#include "iar_stm32f429ii_aca.h"

/*************************************************************************
 * Function Name: USB_OTG_BSP_Init
 * Parameters: USB_OTG_CORE_HANDLE *pdev
 *
 * Return: none
 *
 * Description: Initalizes USB module pins
 *
 *************************************************************************/
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = OTG_HS_ID_PIN |
                                OTG_HS_DM_PIN |
                                OTG_HS_DP_PIN;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinAFConfig(OTG_HS_ID_GPIO_PORT,OTG_HS_ID_PIN_SOURCE,GPIO_AF_OTG2_FS);
  GPIO_PinAFConfig(OTG_HS_DM_GPIO_PORT,OTG_HS_DM_PIN_SOURCE,GPIO_AF_OTG2_FS);
  GPIO_PinAFConfig(OTG_HS_DP_GPIO_PORT,OTG_HS_DP_PIN_SOURCE,GPIO_AF_OTG2_FS);

  GPIO_InitStructure.GPIO_Pin = OTG_HS_VBUS_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(OTG_HS_VBUS_GPIO_PORT, &GPIO_InitStructure);

  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_OTG_HS, ENABLE) ;
}

/*************************************************************************
 * Function Name: USB_OTG_BSP_EnableInterrupt
 * Parameters: USB_OTG_CORE_HANDLE *pdev
 *
 * Return: none
 *
 * Description: Configures USB interrupts
 *
 *************************************************************************/
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
NVIC_InitTypeDef NVIC_InitStructure; 
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_EP1_OUT_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_EP1_IN_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

#endif
}

/*************************************************************************
 * Function Name: USB_OTG_BSP_uDelay
 * Parameters: const uint32_t usec
 *
 * Return: none
 *
 * Description: Delay in microseconds
 *
 *************************************************************************/
void USB_OTG_BSP_uDelay (const uint32_t usec)
{
  uint32_t count = 0;
  const uint32_t utime = (42 * usec);
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1);
}


/*************************************************************************
 * Function Name: USB_OTG_BSP_mDelay
 * Parameters: const uint32_t msec
 *
 * Return: none
 *
 * Description: Delay in miliseconds
 *
 *************************************************************************/
void USB_OTG_BSP_mDelay (const uint32_t msec)
{
  USB_OTG_BSP_uDelay(msec * 1000);
}

