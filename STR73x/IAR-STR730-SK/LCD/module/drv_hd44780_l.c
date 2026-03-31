/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : drv_hd44780_l.c
 *    Description : I/O function
 *
 *    History :
 *    1. Date        : June 13, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
**************************************************************************/
#include "drv_hd44780_l.h"

/*************************************************************************
 * Function Name: HD44780_IO_Init
 * Parameters: none
 * Return: none
 * Description: Init IO ports directions and level
 *
 *************************************************************************/
void HD44780_IO_Init (void)
{
GPIO_InitTypeDef GPIO_InitStructure;
#if BOARD_REV == 1
  /* GPIO Initialization */
  /* GPIO2, GPIO5, GPIO6 Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_GPIO2, ENABLE);
  CFG_PeripheralClockConfig(CFG_CLK_GPIO5, ENABLE);
  CFG_PeripheralClockConfig(CFG_CLK_GPIO6, ENABLE);
#else
  /* GPIO2, GPIO6 Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_GPIO2, ENABLE);
  CFG_PeripheralClockConfig(CFG_CLK_GPIO6, ENABLE);
#endif

  /* LCD Data Port Configuration */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
  GPIO_InitStructure.GPIO_Pins = (LCD_DATA << LCD_DATA_SHIFT);
  GPIO_Init (LCD_DATA_PORT, &GPIO_InitStructure);

  /* LCD Ctrl IO Configuration */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
  GPIO_InitStructure.GPIO_Pins = LCD_RS;
  GPIO_Init (LCD_RS_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pins = LCD_E;
  GPIO_Init (LCD_E_PORT,  &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pins = LCD_LIGHT;
  GPIO_Init (LCD_LIGHT_PORT, &GPIO_InitStructure);

#if HD4780_WR > 0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
  GPIO_InitStructure.GPIO_Pins = LCD_RW;
  GPIO_Init (LCD_RW_PORT, &GPIO_InitStructure);
#endif // HD4780_WR > 0

  // LCD backlight off
  LCD_LIGHT_OFF();
  // RS Ouput - Low
  HD44780SetRS(0);
  // E  Ouput - Low
  HD44780SetE(0);

#if HD4780_WR > 0
  // WR Ouput - Low
  HD44780SetRW(0);
#endif
}

/*************************************************************************
 * Function Name: HD44780SetRS
 * Parameters: Boolean Data
 * Return: none
 * Description: Set RS signal
 *
 *************************************************************************/
void HD44780SetRS (Boolean Data)
{
  GPIO_BitWrite(LCD_RS_PORT,LCD_RS,Data?Bit_SET:Bit_RESET);
}

/*************************************************************************
 * Function Name: HD44780SetE
 * Parameters: Boolean Data
 * Return: none
 * Description: Set E signal
 *
 *************************************************************************/
void HD44780SetE (Boolean Data)
{
  GPIO_BitWrite(LCD_E_PORT,LCD_E,Data?Bit_SET:Bit_RESET);
}

/*************************************************************************
 * Function Name: HD44780WrIO
 * Parameters: Int8U Data
 * Return: none
 * Description: Write to HD44780 I/O
 *
 *************************************************************************/
void HD44780WrIO (Int8U Data)
{
GPIO_InitTypeDef GPIO_InitStructure;
  // Set Direction
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
  GPIO_InitStructure.GPIO_Pins = LCD_DATA << LCD_DATA_SHIFT;
  GPIO_Init (LCD_DATA_PORT, &GPIO_InitStructure);

#if HD4780_WR > 0
  HD44780SetRW(0);
#endif
  // Write Data
  // Clock E
  HD44780SetE(1);

  Data = (GPIO_WordRead(LCD_DATA_PORT) & ~(LCD_DATA << LCD_DATA_SHIFT)) | ((Data << LCD_DATA_SHIFT) & (LCD_DATA << LCD_DATA_SHIFT));
  GPIO_WordWrite(LCD_DATA_PORT,Data);

  HD44780_BUS_DLY();
  HD44780SetE(0);
}

#if HD4780_WR > 0
/*************************************************************************
 * Function Name: HD44780SetRW
 * Parameters: Boolean Data
 * Return: none
 * Description: Set R/W signal
 *
 *************************************************************************/
void HD44780SetRW (Boolean Data)
{
  GPIO_BitWrite(LCD_RW_PORT,LCD_RW,Data?Bit_SET:Bit_RESET);
}

/*************************************************************************
 * Function Name: HD44780RdIO
 * Parameters: none
 * Return: Int8U
 * Description: Read from HD44780 I/O
 *
 *************************************************************************/
Int8U HD44780RdIO (void)
{
Int8U Data;
GPIO_InitTypeDef GPIO_InitStructure;
  // Set Direction
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_TRI_TTL;
  GPIO_InitStructure.GPIO_Pins = LCD_DATA << LCD_DATA_SHIFT;
  GPIO_Init (LCD_DATA_PORT, &GPIO_InitStructure);

  HD44780SetRW(1);
  // Read Data
  HD44780SetE(1);
  HD44780_BUS_DLY();
  Data = ( GPIO_WordRead(LCD_DATA_PORT) >> LCD_DATA_SHIFT) & LCD_DATA;
  HD44780SetE(0);
  // Clock E
  return Data;
}
#endif
