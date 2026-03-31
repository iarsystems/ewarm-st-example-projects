/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : drv_hd44780_l.c
 *    Description : STR912F HD44780 low level I/O function driver
 *
 *    History :
 *    1. Date        : August 10, 2006
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
GPIO_InitTypeDef  GPIO_InitStructure;

  // Enable GPIO clocks
  SCU_APBPeriphClockConfig(__GPIO8 | __GPIO9, ENABLE);
  // Release GPIO reset
  SCU_APBPeriphReset(__GPIO8 | __GPIO9, DISABLE);

  // IO configure
  SCU->GPIOEMI &= ~1;
  SCU->GPIOTYPE[9] &=  ~(LCD_RS | LCD_E | LCD_RW | LCD_LIGHT);
  GPIO9->DDR |= LCD_RS | LCD_E | LCD_RW | LCD_LIGHT;

  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1;
  GPIO_InitStructure.GPIO_Pin = LCD_DATA;
  GPIO_Init(GPIO8, &GPIO_InitStructure);

  // RS Output - Low
  HD44780SetRS(0);
  // E  Output - Low
  HD44780SetE(0);
#if HD4780_WR > 0
  // WR Output - Low
  HD44780SetRW(0);
#endif
  // Backlight off
  LCD_LIGHT_OFF();
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
  GPIO_WriteBit(GPIO9,LCD_RS,Data?Bit_SET:Bit_RESET);
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
  GPIO_WriteBit(GPIO9,LCD_E,Data?Bit_SET:Bit_RESET);
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
  // Write Data
#if HD4780_WR > 0
GPIO_InitTypeDef  GPIO_InitStructure;
  HD44780SetRW(0);
  // Set Direction
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1;
  GPIO_InitStructure.GPIO_Pin = LCD_DATA;
  GPIO_Init(GPIO8, &GPIO_InitStructure);
#endif
  GPIO_Write(GPIO8,Data);
  // Clock E
  HD44780SetE(1);
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
  GPIO_WriteBit(GPIO9,LCD_RW,Data?Bit_SET:Bit_RESET);
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
GPIO_InitTypeDef  GPIO_InitStructure;
  // Set Direction
  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1;
  GPIO_InitStructure.GPIO_Pin = LCD_DATA;
  GPIO_Init(GPIO8, &GPIO_InitStructure);
  HD44780SetRW(1);
  // Read Data
  HD44780SetE(1);
  HD44780_BUS_DLY();
  Data = GPIO_Read(GPIO8);
  HD44780SetE(0);
  // Clock E
  return Data;
}
#endif
