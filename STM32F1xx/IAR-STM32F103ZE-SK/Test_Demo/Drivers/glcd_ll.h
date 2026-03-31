/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : glcd_ll.h
 *    Description : GLCD low level include file
 *
 *    History :
 *    1. Date        : July 19, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#ifndef __GLCD_LL_H
#define __GLCD_LL_H

#define BACKLIGHT_OFF     0x40
#define BACKLIGHT_ON      0x60
#define SSP_FIFO_SIZE     8

#define GLCD_SPI_CLK      1000    // [kHz]
// LCD controller reset pin - PB14
#define LCD_RST_PORT      GPIOB
#define LCD_RST_MASK      GPIO_Pin_14

// LCD controller CS pin - PC3
#define LCD_CS_PORT       GPIOC
#define LCD_CS_MASK       GPIO_Pin_3

// LCD controller SPI pins - PA5, PA6, PA7
#define LCD_SPI_PORT      GPIOA
#define LCD_SPI_SCLK_MASK GPIO_Pin_5
#define LCD_SPI_MISO_MASK GPIO_Pin_6
#define LCD_SPI_MOSI_MASK GPIO_Pin_7

#define GLCD_SPI_MOSI_H() LCD_SPI_PORT->BSRR = LCD_SPI_MOSI_MASK
#define GLCD_SPI_MOSI_L() LCD_SPI_PORT->BRR  = LCD_SPI_MOSI_MASK
#define GLCD_SPI_CLK_H()  LCD_SPI_PORT->BSRR = LCD_SPI_SCLK_MASK
#define GLCD_SPI_CLK_L()  LCD_SPI_PORT->BRR  = LCD_SPI_SCLK_MASK
#define GLCD_SPI_MIS0()   (0 != (LCD_SPI_PORT->IDR & LCD_SPI_MISO_MASK))

/*************************************************************************
 * Function Name: GLCD_SetReset
 * Parameters: Boolean State
 * Return: none
 *
 * Description: Set reset pin state
 *
 *************************************************************************/
void GLCD_SetReset (Boolean State);

/*************************************************************************
 * Function Name: GLCD_SetBacklight
 * Parameters: u8 Light
 * Return: none
 *
 * Description: Set backlight pin state
 *
 *************************************************************************/
void GLCD_Backlight (u8 Light);

/*************************************************************************
 * Function Name: GLCD_LLInit
 * Parameters: none
 * Return: none
 *
 * Description: Init Reset and Backlight control outputs
 *
 *************************************************************************/
void GLCD_LLInit (void);

/*************************************************************************
 * Function Name: LcdSpiChipSelect
 * Parameters: Boolean Select
 * Return: none
 *
 * Description: SPI Chip select control
 * Select = true  - Chip is enable
 * Select = false - Chip is disable
 *
 *************************************************************************/
void GLCD_SPI_ChipSelect (Boolean Select);

/*************************************************************************
 * Function Name: LcdSpiSetWordWidth
 * Parameters: u32 Width
 * Return: Boolean
 *
 * Description: Set SPI word width
 *
 *************************************************************************/
Boolean GLCD_SPI_SetWordWidth (u32 Width);

/*************************************************************************
 * Function Name: LcdSpiSetClockFreq
 * Parameters: u32 Frequency
 * Return: u32
 *
 * Description: Set SPI clock
 *
 *************************************************************************/
u32 GLCD_SPI_SetClockFreq (u32 Frequency);

/*************************************************************************
 * Function Name: GLCD_SPI_Init
 * Parameters: u32 Clk, u32 Width
 * Return: none
 *
 * Description: Init SPI
 *
 *************************************************************************/
void GLCD_SPI_Init(u32 Clk, u32 Width);

/*************************************************************************
 * Function Name: GLCD_SPI_TranserByte
 * Parameters: u32 Data
 * Return: u32
 *
 * Description: Transfer byte from SPI
 *
 *************************************************************************/
u32 GLCD_SPI_TranserByte (u32 Data);

/*************************************************************************
 * Function Name: GLCD_SPI_SendBlock
 * Parameters: pu8 pData, u32 Size
 *
 * Return: void
 *
 * Description: Write block of data to SPI
 *
 *************************************************************************/
void GLCD_SPI_SendBlock (pu8 pData, u32 Size);

/*************************************************************************
 * Function Name: GLCD_SPI_ReceiveBlock
 * Parameters: pu8 pData, u32 Size
 *
 * Return: void
 *
 * Description: Read block of data from SPI
 *
 *************************************************************************/
void GLCD_SPI_ReceiveBlock (pu8 pData, u32 Size);

#endif // __GLCD_LL_H
