/******************** (C) COPYRIGHT 2005 STMicroelectronics **********************
* File Name          : 73x_lcd.c
* Author             : MCD Application Team
* Date First Issued  : 09/27/2005 :  V1.0
* Description        : This file provides all the LCD software functions.
**********************************************************************************
* History:
* 09/27/2005 :  V1.0
**********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*********************************************************************************/
/* Standard includes -----------------------------------------------------------*/
#include "73x_lcd.h"
#include "73x_lib.h"
/* Include of other module interface headers -----------------------------------*/
/* Local includes --------------------------------------------------------------*/
/* Private typedef -------------------------------------------------------------*/
/* Private define --------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------*/
vu16 LCDDelay = 0;
/* Private function prototypes -------------------------------------------------*/
// Define GPIO Init Structure */
GPIO_InitTypeDef GPIO_InitStructure;
/* Interface functions ---------------------------------------------------------*/
/* Private functions -----------------------------------------------------------*/


/*******************************************************************************
* Function Name  : LCD_Init
* Description    : Initializes the LCD driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Init(void)
{
  /* GPIO1 Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_GPIO1 , ENABLE);
  /* GPIO1 Configuration */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
  GPIO_InitStructure.GPIO_Pins = GPIO_PIN_7 |GPIO_PIN_8 | GPIO_PIN_9 |GPIO_PIN_10
                                |GPIO_PIN_11|GPIO_PIN_12 |GPIO_PIN_13;
  GPIO_Init(GPIO1, &GPIO_InitStructure);

  /* GPIO6 Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_GPIO6 , ENABLE);
  /* GPIO6 Configuration */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
  GPIO_InitStructure.GPIO_Pins = GPIO_PIN_0;
  GPIO_Init(GPIO6, &GPIO_InitStructure);
 GPIO_BitWrite(GPIO6, GPIO_PIN_0, Bit_SET);

  // Wait 40 ms
  LCD_Wait(4000);
  LCD_Send_Command_4bit(0x30);
  // Wait 6 ms
  LCD_Wait(600);
  LCD_Send_Command_4bit(0x30);
  // Wait 6 ms
  LCD_Wait(600);
  LCD_Send_Command_4bit(0x30);
  // Wait 6 ms
  LCD_Wait(600);
  LCD_Send_Command_4bit(0x20);
  // Wait 6 ms
  LCD_Wait(600);
  LCD_Send_Command_4bit(0x20);
  // Wait 6 ms
  LCD_Wait(600);
  LCD_Send_Command_4bit(0xF0);
  // Wait 6 ms
  LCD_Wait(600);
  LCD_Send_Command_4bit(0x00);
  // Wait 6 ms
  LCD_Wait(600);
  LCD_Send_Command_4bit(0x80);
  // Wait 6 ms
  LCD_Wait(600);
  LCD_Send_Command_4bit(0x00);
  // Wait 6 ms
  LCD_Wait(600);
  LCD_Send_Command_4bit(0x10);
  // Wait 6 ms
  LCD_Wait(600);
  LCD_Send_Command_4bit(0x00);
  // Wait 6 ms
  LCD_Wait(600);
  LCD_Send_Command_4bit(0x60);
  // Wait 6 ms
  LCD_Wait(600);
   }

/*******************************************************************************
* Function Name  : LCD_E_High
* Description    : Puts the LCD Enable line (E) high.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_E_High(void)
{
  GPIO_BitWrite(GPIO1, LCD_PIN_E, Bit_SET);
}

/*******************************************************************************
* Function Name  : LCD_E_Low
* Description    : Puts the LCD Enable line (E) low.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_E_Low(void)
{
  GPIO_BitWrite(GPIO1, LCD_PIN_E, Bit_RESET);
}

/*******************************************************************************
* Function name   : LCD_Send_Command_4bit
* Description     : Sends a command to the LCD.
* Input param     : Cmd: command to be sent.
* Output param    : None
* Return          : None
*******************************************************************************/
void LCD_Send_Command_4bit(u8 Cmd)
{
     // Reset RW bit
     GPIO_BitWrite(GPIO1, LCD_PIN_RW, Bit_RESET);

     // Reset RS bit
     GPIO_BitWrite(GPIO1, LCD_PIN_RS, Bit_RESET);

     // Set E bit
     GPIO_BitWrite(GPIO1, LCD_PIN_E, Bit_SET);

    LCD_Send_Cmd_Data(Cmd>>4);

    // Reset E bit
     GPIO_BitWrite(GPIO1, LCD_PIN_E, Bit_RESET);

     // Set E bit
    GPIO_BitWrite(GPIO1, LCD_PIN_E, Bit_SET);

    // Send the command
    LCD_Send_Cmd_Data(Cmd);

    // Reset E bit
    GPIO_BitWrite(GPIO1, LCD_PIN_E, Bit_RESET);

      // Set RS bit
    GPIO_BitWrite(GPIO1, LCD_PIN_RS, Bit_SET);

     // Set RW bit
    GPIO_BitWrite(GPIO1, LCD_PIN_RW, Bit_SET);
}

/*******************************************************************************
* Function Name  : LCD_SendData_4bit
* Description    : Sends data to the LCD.
* Input          : Data: data to be displayed.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_SendData_4bit(u8 Data)
{

     // Set RS bit
     GPIO_BitWrite(GPIO1, LCD_PIN_RS, Bit_SET);

      // Reset RW bit
     GPIO_BitWrite(GPIO1, LCD_PIN_RW, Bit_RESET);

     // Set E bit
     GPIO_BitWrite(GPIO1, LCD_PIN_E, Bit_SET);

     // Data on GPIO_MSB
     LCD_Send_Cmd_Data(Data >>4);


      // Reset E bit
     GPIO_BitWrite(GPIO1, LCD_PIN_E, Bit_RESET);


      // Set E bit
     GPIO_BitWrite(GPIO1, LCD_PIN_E, Bit_SET);

     // Data on GPIO_MSB
     LCD_Send_Cmd_Data(Data);


      // Reset E bit
     GPIO_BitWrite(GPIO1, LCD_PIN_E, Bit_RESET);

      // Reset RS bit
     GPIO_BitWrite(GPIO1, LCD_PIN_RS, Bit_RESET);

      // Reset RW bit
     GPIO_BitWrite(GPIO1, LCD_PIN_RW, Bit_SET);
}

/*******************************************************************************
* Function name   : LCD_Send_Cmd_Data
* Description     : Send a command or Data On GPIO1_MSB.
* Input param     : Cmd: command/Data to be sent.
* Output param    : None
* Return          : None
*******************************************************************************/
void  LCD_Send_Cmd_Data(u8 Cmd)
{

       if((Cmd & 0x01)==0x01)
        GPIO_BitWrite(GPIO1, GPIO_PIN_8, Bit_SET);
       else
       GPIO_BitWrite(GPIO1, GPIO_PIN_8 , Bit_RESET);

       if((Cmd & 0x02)==0x02)
        GPIO_BitWrite(GPIO1, GPIO_PIN_9, Bit_SET);
       else
       GPIO_BitWrite(GPIO1, GPIO_PIN_9 , Bit_RESET);

       if((Cmd & 0x04)==0x04)
        GPIO_BitWrite(GPIO1, GPIO_PIN_10, Bit_SET);
       else
       GPIO_BitWrite(GPIO1, GPIO_PIN_10 , Bit_RESET);

       if((Cmd & 0x08)==0x08)
        GPIO_BitWrite(GPIO1, GPIO_PIN_11, Bit_SET);
       else
       GPIO_BitWrite(GPIO1, GPIO_PIN_11 , Bit_RESET);


}




/*******************************************************************************
* Function Name  : LCD_SendString
* Description    : Displays a string on the LCD.
* Input          : - pBuffer: pointer to the buffer containing the data to be
*                    displayed on the LCD.
*                  - Line: specifies the line where the string will be displayed.
*                  - Column: specifies the start column from where the string will
*                    be displayed.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_SendString(u8 *pBuffer, u8 Line, u8 Column)
{
  u8 len = 0, count = 0;

  len = strlen(( const char *)pBuffer);

  if((len < (LCD_HALF_LENGTH + 2 - Column)))
  {
    LCD_SetPosCur(Line, Column);

    for(count =0; count < len; count++)
    {
      LCD_SendData_4bit(pBuffer[count]);
    }
  }
  else
  {
    LCD_SetPosCur(Line, Column);

    for(count = 0; count <= (LCD_HALF_LENGTH-Column); count++)
    {
      LCD_SendData_4bit(pBuffer[count]);
    }

    LCD_SetPosCur(2, 1);

    for(count = (LCD_HALF_LENGTH-Column+1); count < len; count++)
    {
      LCD_SendData_4bit(pBuffer[count]);
    }
  }
}

/*******************************************************************************
* Function Name  : LCD_SendStringByStep
* Description    : Displays a string on the LCD by step.
* Input          : - pBuffer: pointer to the buffer containing the data to be
*                    displayed on the LCD.
*                  - Line: specifies the line where the string will be displayed.
*                  - Column: specifies the start column from where the string will
*                    be displayed.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_SendStringByStep(u8 *pBuffer, u8 Line, u8 Column)
{
  u8 len = 0, count = 0;

  len = strlen(( const char *)pBuffer);

  if(len < (LCD_HALF_LENGTH + 2 - Column))
  {
    LCD_SetPosCur(Line, Column);

    for(count =0; count < len; count++)
    {
      // wait 200 ms
    LCD_Wait(20000);
      LCD_SendData_4bit(pBuffer[count]);
    }
  }
  else
  {
    LCD_SetPosCur(Line, Column);

    for(count = 0; count <= (LCD_HALF_LENGTH-Column); count++)
    {
      // wait 200 ms
    LCD_Wait(20000);
      LCD_SendData_4bit(pBuffer[count]);
    }

    LCD_SetPosCur(2, 1);

    for(count = (LCD_HALF_LENGTH-Column+1); count < len; count++)
    {
      // wait 200 ms
   LCD_Wait(20000);
      LCD_SendData_4bit(pBuffer[count]);
    }
  }
}

/*******************************************************************************
* Function Name  : LCD_SendStringPinPong
* Description    : Displays a string on the LCD as ping pong.
* Input          : - pBuffer: pointer to the buffer containing the data to be
*                    displayed on the LCD.
*                  - Line: specifies the line where the string will be displayed.
*                  - nTime: number of ping pong.
* Output         :
* Return         : None
*******************************************************************************/
void LCD_SendStringPinPong(u8 *pBuffer, u8 Line, u32 nTime)
{
  u8 len = 0, index = 1;
  bool Direction = TRUE;

  len = strlen(( const char *)pBuffer);

  while(nTime--)
  {
    if(Direction)
    {
      LCD_SendString(pBuffer, Line, index);
      // wait 200 ms
       LCD_Wait(20000);
      index++;
      LCD_LineClear(Line);
      if(index == (LCD_HALF_LENGTH - len) + 1)
      {
        Direction = FALSE;
      }
    }
    else
    {
      LCD_SendString(pBuffer, Line, index);
      // wait 200 ms
       LCD_Wait(20000);
      index--;
      LCD_LineClear(Line);
      if(index == 1)
      {
        Direction = TRUE;
      }
    }
  }
}

/*******************************************************************************
* Function Name  : LCD_SetPosCur
* Description    : Sets the line and column position of the LCD cursor.
* Input          : - Line: specifies the cursor's line position.
*                  - Column: specifies cursor's column position.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_SetPosCur(u8 Line, u8 Column)
{
  if(Line == 1)
  {
    Column = Column - 1;
  }
  else
  {
    Column = Column -1  + 64;
  }

  LCD_Send_Command_4bit(Column | 0x80);
}

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : Clears the LCD display.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Clear(void)
{
  /* reset RS bit */
  GPIO_BitWrite(GPIO1, LCD_PIN_RS, Bit_RESET);

  /* send "DISPLAY CLEAR COMMAND" command */
  LCD_Send_Command_4bit(LCD_CMD_CLR_DISP);

}

/*******************************************************************************
* Function Name  : LCD_LineClear
* Description    : Clears the LCD specified line.
* Input          : Line: line to be cleared.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_LineClear(u8 Line)
{
  u8 j = 0;

  /* reset RS bit */
  GPIO_BitWrite(GPIO1, LCD_PIN_RS, Bit_RESET);

  LCD_SetPosCur(Line, 1);

  /* set RS bit */
  GPIO_BitWrite(GPIO1, LCD_PIN_RS, Bit_SET);

  /* write LCD_HALF_LENGTH spaces to clear the line */
  for(j = LCD_HALF_LENGTH; j != 0; j--)
  {
    LCD_SendData_4bit(' ');
  }
}

/*******************************************************************************
* Function Name  : LCD_DigitDisplay
* Description    : Displays a digit on the LCD.
* Input          : - Digit: digit to be displayed.
*                  - Line: specifies the line where the digit will be displayed.
*                  - Column: specifies the start column from where the digit will
*                    be displayed.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DigitDisplay(u8 Digit, u8 Line, u8 Column)
{
  LCD_SetPosCur(Line, Column);

  LCD_SendData_4bit(Digit + 48);
}

/*******************************************************************************
* Function Name  : LCD_Wait
* Description    : Inserts a delay.
* Input          : nTime: specifies the time-out value.
* Output         : LCDDelay: incremented each 125 µs in TB1 interrupt handler
*                  routine and used to define the time-out value.
* Return         : None
*******************************************************************************/
void LCD_Wait(u32 nTime)
{
  LCDDelay = 0;

  while(LCDDelay < nTime)
  {
    LCDDelay++;
  }
}


/******************* (C) COPYRIGHT 2005 STMicroelectronics *****END OF FILE****/
