/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : mmc_ll_SPI2.h
 *    Description : Low level MMC SPI diver include file
 *
 *    History :
 *    1. Date        : March 19, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#ifndef  __SD_LL_SPI2_H
#define  __SD_LL_SPI2_H

extern void DelayResolution100us(Int32U Delay);

#if SPI_DMA_ENA > 0 && !defined(DMA_ERRATA)
/*************************************************************************
 * Function Name: SPI2_DmaHandler
 * Parameters: none
 * Return: none
 *
 * Description: Interrupt handler of the SPI2 DMA.
 *
 *************************************************************************/
void SPI2_RxDmaHandler(void);

/*************************************************************************
 * Function Name: SPI2_DmaHandler
 * Parameters: none
 * Return: none
 *
 * Description: Interrupt handler of the SPI2 DMA.
 *
 *************************************************************************/
void SPI2_TxDmaHandler(void);
#endif // SPI_DMA_ENA > 0 && !defined(DMA_ERRATA)

/*************************************************************************
 * Function Name: SdChipSelect
 * Parameters: Boolean Select
 * Return: none
 *
 * Description: Mmc Chip select control
 * Select = true  - Chip is enable
 * Select = false - Chip is disable
 *
 *************************************************************************/
void SdChipSelect (Boolean Select);

/*************************************************************************
 * Function Name: SdPresent
 * Parameters: none
 * Return: Boolean - true cart present
 *                 - false cart no present
 *
 * Description: Mmc present check
 *
 *************************************************************************/
Boolean SdPresent (void);

/*************************************************************************
 * Function Name: SdWriteProtect
 * Parameters: none
 * Return: Boolean - true cart is protected
 *                 - false cart no protected
 *
 * Description: Mmc Write protect check
 *
 *************************************************************************/
Boolean SdWriteProtect (void);

/*************************************************************************
 * Function Name: SdSetClockFreq
 * Parameters: Int32U Frequency
 * Return: Int32U
 *
 * Description: Set SPI ckl frequency
 *
 *************************************************************************/
Int32U SdSetClockFreq (Int32U Frequency);

/*************************************************************************
 * Function Name: SdInit
 * Parameters: none
 * Return: none
 *
 * Description: Init SPI, Cart Present, Write Protect and Chip select pins
 *
 *************************************************************************/
void SdInit (void);

/*************************************************************************
 * Function Name: SdTranserByte
 * Parameters: Int8U ch
 * Return: Int8U
 *
 * Description: Transfer byte by SPI
 *
 *************************************************************************/
Int8U SdTranserByte (Int8U ch);

/*************************************************************************
 * Function Name: SdSendBlock
 * Parameters: pInt8U pData, Int32U Size
 *
 * Return: none
 *
 * Description: Send block by SPI
 *
 *************************************************************************/
void SdSendBlock (pInt8U pData, Int32U Size);

/*************************************************************************
 * Function Name: SdReceiveBlock
 * Parameters: pInt8U pData, Int32U Size
 *
 * Return: none
 *
 * Description: Read block by SPI
 *
 *************************************************************************/
void SdReceiveBlock (pInt8U pData, Int32U Size);

/*************************************************************************
 * Function Name: SdDly_1ms
 * Parameters: Int32U Delay
 * Return: none
 *
 * Description: Delay [msec]
 *
 *************************************************************************/
void SdDly_1ms (Int32U Delay);

/*************************************************************************
 * Function Name: SdPowerOn
 * Parameters: none
 * Return: none
 *
 * Description: Set power off state
 *
 *************************************************************************/
void SdPowerOn (void);

/*************************************************************************
 * Function Name: SdPowerOff
 * Parameters: none
 * Return: none
 *
 * Description: Set power off state
 *
 *************************************************************************/
void SdPowerOff (void);


#endif  // __MMC_LL_SPI2_H
