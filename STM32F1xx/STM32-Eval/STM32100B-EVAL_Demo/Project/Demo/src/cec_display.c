/**
  ******************************************************************************
  * @file     cec_display.c
  * @author   MCD Application Team
  * @version  V1.0.0
  * @date     03/01/2010
  * @brief    It displyas the CEC demonstration messages on LCD .
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32100B_EVAL_Demo
  * @{
  */

/** @defgroup CEC_DISPLAY
  * @brief    It displyas the CEC demonstration messages on LCD .
  * @{
  */

/** @defgroup CEC_DISPLAY_Private_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup CEC_DISPLAY_Private_Defines
  * @{
  */
#define NUMBER_OF_CMD  12
/**
  * @}
  */

/** @defgroup CEC_DISPLAY_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup CEC_DISPLAY_Private_Variables
  * @{
  */
HDMI_CEC_Error TransmitStatus = HDMI_CEC_OK;
__IO uint8_t FollowerAddress = 0;
__IO uint8_t KeyValue = 0;
extern uint8_t Pressed_Key_Button;
extern __IO uint32_t ReceivedFrame, ReceiveStatus;
extern HDMI_CEC_Message HDMI_CEC_RX_MessageStructPrivate;
extern HDMI_CEC_Message HDMI_CEC_TX_MessageStructure;
extern __IO uint8_t MyLogicalAddress;
extern __IO uint16_t MyPhysicalAddress;
extern __IO uint8_t DeviceType;
extern uint8_t* HDMI_CEC_Follower_String[13][2];

/* CEC device commands */
uint8_t* HDMI_CEC_CMD_String[NUMBER_OF_CMD] =
  {
    "   FEATURE_ABORT    ",
    " GET CEC VERSION    ",
    " GIVE PHYSICAL ADR  ",
    "    GIVE OSD NAME   ",
    "   STANDBY VALUE    ",
    "   IMAGE VIEW ON    ",
    "    TEXT VIEW ON    ",
    "    RECORD OFF      ",
    " GIVE DEV VENDOR ID ",
    "     Volume Up      ",
    "   Volume Down      ",
    "   Volume Mute      "
  };

uint8_t HDMI_CEC_Follower_ADD[13] =
  {
    0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xF
  };

uint8_t HDMI_CEC_CMD_OP[NUMBER_OF_CMD] =
  {
    HDMI_CEC_OPCODE_FEATURE_ABORT, HDMI_CEC_OPCODE_GET_CEC_VERSION, HDMI_CEC_OPCODE_GIVE_PHYSICAL_ADDRESS, HDMI_CEC_OPCODE_GIVE_OSD_NAME,
    HDMI_CEC_OPCODE_STANDBY, HDMI_CEC_OPCODE_IMAGE_VIEW_ON, HDMI_CEC_OPCODE_TEXT_VIEW_ON, HDMI_CEC_OPCODE_RECORD_OFF, HDMI_CEC_OPCODE_GIVE_DEVICE_VENDOR_ID,
    HDMI_CEC_OPCODE_USER_CONTROL_PRESSED, 0x55, 0x66
  }; /* 0x55 = Volume Down, 0x66= Volume Mute*/


/**
  * @}
  */

/** @defgroup CEC_DISPLAY_Private_Function_Prototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup CEC_DISPLAY_Private_Functions
  * @{
  */

/**
  * @brief  DeInitialize the CEC Demo.
  * @param  None
  * @retval None
  */
void CEC_Display_DeInit(void)
{
  CEC_DeInit();
  I2C_DeInit(I2C2);
  GPIO_DeInit(GPIOB);
  RCC_DeInit();
}

/**
  * @brief  Initialize the CEC Demo.
  * @param  None
  * @retval None
  */
void CEC_Display_Init(void)
{
  /* Select the device type */

  DeviceType = HDMI_CEC_TUNER;

  /* CEC Initialization */
  HDMI_CEC_Init();

  /* for Unregistered Device */
  if (MyLogicalAddress == 0xF)
  {
    /* Clear the LCD */
    LCD_Clear(LCD_COLOR_BLUE);
    LCD_SetBackColor(LCD_COLOR_BLACK);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    CEC_Display(LCD_LINE_5, "Unregistered Device");
    CEC_Display_DeInit();
    while (1);
  }

  /* Display initialization */
  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  CEC_Display(LCD_LINE_0, "CEC device is confi-");
  CEC_Display(LCD_LINE_1, "gured as Tuner      ");
  CEC_Display(LCD_LINE_2, "and initialized     ");
  CEC_Display(LCD_LINE_3, "correctly           ");
  /* Display physical and logical addesses */
  CEC_Display(LCD_LINE_4, "Logical Addr: ");
  LCD_SetTextColor(LCD_COLOR_RED);
  CEC_Display_LCD_DisplayString(LCD_LINE_4, LCD_COLUMN_13, "0x");
  CEC_Display_DisplayHexToChar (MyLogicalAddress, LCD_LINE_4, LCD_COLUMN_15);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  CEC_Display(LCD_LINE_5, "Physical Addr:");
  LCD_SetTextColor(LCD_COLOR_RED);
  CEC_Display_LCD_DisplayString(LCD_LINE_5, LCD_COLUMN_14, "0x");
  CEC_Display_DisplayHexToChar (MyPhysicalAddress >> 8, LCD_LINE_5, LCD_COLUMN_16);
  CEC_Display_DisplayHexToChar (MyPhysicalAddress & 0xFF, LCD_LINE_5, LCD_COLUMN_18);

  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_SetBackColor(LCD_COLOR_BLACK);
  CEC_Display(LCD_LINE_8, "Press SEL button to ");
  CEC_Display(LCD_LINE_9, "  enter CEC menu    ");
  /* press SEL button to enter CEC menu */
  while ((Menu_ReadKey() != SEL)&& (Pressed_Key_Button == 0))
  {}
  if(Pressed_Key_Button)return;

  /* Display CEC menu */
  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_SetBackColor(LCD_COLOR_BLACK);
  CEC_Display(LCD_LINE_0, "Receive:            ");
  CEC_Display(LCD_LINE_7, "Send Status:        ");
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetBackColor(LCD_COLOR_RED);
}

/**
  * @brief  Select the device type
  * @param  None
  * @retval None
  */
void CEC_Display_SelectDeviceType(void)
{
  do
  {
    KeyValue = Menu_ReadKey();
  }
  while ((!KeyValue) || (KeyValue == SEL));


  switch (KeyValue)
  {
    case LEFT:
      DeviceType = HDMI_CEC_RECORDING;
      break;
    case RIGHT:
      DeviceType = HDMI_CEC_TUNER;
      break;
    case UP:
      DeviceType = HDMI_CEC_PLAYBACK;
      break;
    case DOWN:
      DeviceType = HDMI_CEC_AUDIOSYSTEM;
      break;
  }
}

/**
  * @brief  Display the available CEC commands.
  * @param  None
  * @retval None
  */
uint8_t CEC_Display_CECAvailableCommands(void)
{
  uint32_t KeyState = 0, opcode = 0;
  int8_t i = 0;
  uint8_t sendcount = 0;

  /* select the follower address */
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLACK);
  
  TransmitStatus = HDMI_CEC_OK;

  CEC_Display(LCD_LINE_8, "Select Follower ADDR");

  FollowerAddress = CEC_Display_SelectFollowerAddress();

  while (Menu_ReadKey() != NOKEY)
  {}

  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLACK);
  CEC_Display(LCD_LINE_8, " Select CEC Command ");

  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);

  CEC_Display(LCD_LINE_9, HDMI_CEC_CMD_String[i]);
  opcode = HDMI_CEC_CMD_OP[i];

  while (KeyState != SEL)
  {
    KeyState = Menu_ReadKey();

    if (Pressed_Key_Button != 0)
    {
      return 0;
    }

    if (KeyState == LEFT)
    {
      if (i == 0)
      {
        i = NUMBER_OF_CMD;
      }
      i--;
      CEC_Display(LCD_LINE_9, HDMI_CEC_CMD_String[i]);
      opcode = HDMI_CEC_CMD_OP[i];
    }
    else if (KeyState == RIGHT)
    {
      if (i == (NUMBER_OF_CMD - 1))
      {
        i = -1;
      }
      i++;
      CEC_Display(LCD_LINE_9, HDMI_CEC_CMD_String[i]);
      opcode = HDMI_CEC_CMD_OP[i];
    }
    else if (KeyState == SEL)
    {
      /* Send CEC Message */
      HDMI_CEC_TX_MessageStructure.Header = ((MyLogicalAddress << 4) | FollowerAddress);
      HDMI_CEC_TX_MessageStructure.Opcode = opcode;
      HDMI_CEC_TX_MessageStructure.TxMessageLength = 0x0;
      if (opcode == HDMI_CEC_OPCODE_USER_CONTROL_PRESSED)
      {
        HDMI_CEC_TX_MessageStructure.Operande[0] = 0x41;
        HDMI_CEC_TX_MessageStructure.TxMessageLength = 0x1;
      }
      if (opcode == 0x55)
      {
        HDMI_CEC_TX_MessageStructure.Opcode = HDMI_CEC_OPCODE_USER_CONTROL_PRESSED;
        HDMI_CEC_TX_MessageStructure.Operande[0] = 0x42;
        HDMI_CEC_TX_MessageStructure.TxMessageLength = 0x1;
      }
      if (opcode == 0x66)
      {
        HDMI_CEC_TX_MessageStructure.Opcode = HDMI_CEC_OPCODE_USER_CONTROL_PRESSED;
        HDMI_CEC_TX_MessageStructure.Operande[0] = 0x43;
        HDMI_CEC_TX_MessageStructure.TxMessageLength = 0x1;
      }
      TransmitStatus = HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructure);

      /*retransmit message until 5 time */
      while ((TransmitStatus != HDMI_CEC_OK) && sendcount < 0x5)
      {
        sendcount++;
        TransmitStatus = HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructure);
      }


      if (TransmitStatus == HDMI_CEC_OK)
      {
        LCD_SetBackColor(LCD_COLOR_GREEN);
        CEC_Display(LCD_LINE_7, "Send Status: Succeed");
      }
      else
      {
        LCD_SetBackColor(LCD_COLOR_RED);
        CEC_Display(LCD_LINE_7, "Send Status: Fail   ");
      }
    }
    else if (KeyState == NOKEY)
    {}

    /* Display the received messages */
    while (ReceivedFrame)
    {
      CEC_Display_ReceiveInfoDisplay();
      /* Get the physical and logical address  of new connected devices*/
      HDMI_CEC_CommandCallBack();
    }
  }
  return 0;
}

/**
  * @brief  Select follower address.
  * @param  None
  * @retval The follower address
  */
uint8_t CEC_Display_SelectFollowerAddress(void)
{
  uint32_t KeyState = 0, address = 0;
  int8_t i = 0;

  while (Menu_ReadKey() != NOKEY)
  {}

  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);

  do
  {
    i++;
  }
  while (*(uint8_t *)(HDMI_CEC_Follower_String[i][1]) == '0');

  CEC_Display(LCD_LINE_9, HDMI_CEC_Follower_String[i][0]);
  address = HDMI_CEC_Follower_ADD[i];

  while (KeyState != SEL)
  {
    KeyState = Menu_ReadKey();

    if (Pressed_Key_Button != 0)
    {
      return 0;
    }

    if (KeyState == LEFT)
    {
      do
      {
        if (i == 0)
        {
          i = 13;
        }
        i--;
      }
      while (*(uint8_t *)(HDMI_CEC_Follower_String[i][1]) == '0');

      CEC_Display(LCD_LINE_9, HDMI_CEC_Follower_String[i][0]);
      address = HDMI_CEC_Follower_ADD[i];
    }
    else if (KeyState == RIGHT)
    {

      do
      {
        if (i == 12)
        {
          i = -1;
        }
        i++;
      }
      while (*(uint8_t *)(HDMI_CEC_Follower_String[i][1]) == '0');

      CEC_Display(LCD_LINE_9, HDMI_CEC_Follower_String[i][0]);
      address = HDMI_CEC_Follower_ADD[i];
    }
    else if (KeyState == SEL)
    {
      address = HDMI_CEC_Follower_ADD[i];
    }
    else if (KeyState == NOKEY)
    {}
    /* Display the received messages */
    while (ReceivedFrame)
    {
      CEC_Display_ReceiveInfoDisplay();
      /* Get the physical and logical address  of new connected devices*/
      HDMI_CEC_CommandCallBack();
    }
  }
  return address;
}

/**
  * @brief  Displays the string on the required row and Column
  * @param  Row: the Line where to display the character shape.
  * @param  Column: start Column address.
  * @param  *ptr: pointer to string to display on LCD.
  * @retval None
  */
void CEC_Display_LCD_DisplayString(uint8_t Row, uint16_t Column, uint8_t *ptr)
{
  uint32_t i = 0;
  uint16_t refcolumn = Column;

  /* Send the string character by character on lCD */
  while ( (*ptr != 0) && (i < 20))
  {
    /* Display one character on LCD */
    LCD_DisplayChar(Row, refcolumn, *ptr);
    /* Decrement the COLUMN position by 16 */
    refcolumn -= 16;
    /* Point on the next character */
    ptr++;
    /* Increment the character counter */
    i++;
  }
}

/**
  * @brief  Displays a data byte on the LCD in Hex format.
  * @param  Data: the data to be displayed.
  * @param  Line: the line where the data will be displayed.
  * @param  COLUMN: the COLUMN where the data will be displayed.
  * @retval None
  */
void CEC_Display_DisplayHexToChar (uint8_t Data, uint16_t Line, uint16_t COLUMN)
{
  LCD_DisplayChar(Line, (COLUMN - 16), CEC_Display_HexToChar(Data));
  LCD_DisplayChar(Line, COLUMN, CEC_Display_HexToChar(Data >> 4));
}

/**
  * @brief  Converts a data (four bits) to a character to be displayed  in Hex format.
  * @param  Data: byte to be converted.
  * @retval None
  */
uint8_t CEC_Display_HexToChar(uint8_t Data)
{
  Data &= 0xF;

  switch (Data)
  {
    case 0xA:
      return 0x41;
    case 0xB:
      return 0x42;
    case 0xC:
      return 0x43;
    case 0xD:
      return 0x44;
    case 0xE:
      return 0x45;
    case 0xF:
      return 0x46;
    default:
      return (Data + 0x30);
  }
}

/**
  * @brief  Displays the content of a buffer on the LCD in Hex format.
  * @param  DataBuffer: a pointer to the buffer to be displayed.
  * @param  BufferSize: the size of the buffer in byte.
  * @param  Line: the line where the data will be displayed.
  * @retval None
  */
void CEC_Display_DisplayBuffer(uint8_t *DataBuffer, uint8_t BufferSize, uint16_t Line)
{
  uint8_t i;
  uint16_t column = LCD_COLUMN_5;
  uint16_t line = Line;

  CEC_Display_LCD_DisplayString(Line, LCD_COLUMN_0, "                    ");
  CEC_Display_LCD_DisplayString(Line + 24, LCD_COLUMN_0, "                    ");
  CEC_Display_LCD_DisplayString(Line + 48, LCD_COLUMN_0, "                    ");
  CEC_Display_LCD_DisplayString(Line, LCD_COLUMN_0, "Data:");

  for (i = 0; i < BufferSize; i++)
  {
    CEC_Display_DisplayHexToChar(*(DataBuffer++), line, column);
    column -= 32;
    LCD_DisplayChar(line, column, ',');
    if (i == 4 || i == 0xA)
    {
      line += 24;
      column = LCD_COLUMN_0;
    }
    column -= 16;
  }
}

/**
  * @brief  Displays the CEC messages on either LCD or Hyperterminal.
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  *ptr: pointer to string to display on LCD or Hyperterminal.
  * @retval None
  */
void CEC_Display (uint8_t Line, uint8_t *ptr)
{
  uint8_t *pointer = ptr;
  uint8_t line = Line;
  LCD_DisplayStringLine(line,  pointer);
}

/**
  * @brief  Displays the CEC received messages on the required row and COLUMN
  * @param  None
  * @retval None
  */
void CEC_Display_ReceiveInfoDisplay(void)
{
  /* If a frame has been received */
  if (ReceivedFrame)
  {
    ReceivedFrame = 0;
    LCD_SetBackColor(LCD_COLOR_BLUE);

    /* Check if the frame has been received correctly */
    if (ReceiveStatus)
    {
      LCD_SetBackColor(LCD_COLOR_GREEN);
      CEC_Display_LCD_DisplayString(LCD_LINE_0, LCD_COLUMN_10, "Succeeded");
    }
    else /* The receive was failed */
    {
      LCD_SetBackColor(LCD_COLOR_RED);
      CEC_Display_LCD_DisplayString(LCD_LINE_0, LCD_COLUMN_10, " Failed  ");
    }
    /* Display the receive status, the initiator address, the received data
    frame and  the number of received bytes  */
    LCD_SetBackColor(LCD_COLOR_BLUE);
    CEC_Display_LCD_DisplayString(LCD_LINE_1, LCD_COLUMN_0, "Sender Address = ");
    CEC_Display_DisplayHexToChar ((uint8_t)((HDMI_CEC_RX_MessageStructPrivate.Header >> 0x4) & 0x0F), LCD_LINE_1, LCD_COLUMN_17);
    CEC_Display_LCD_DisplayString(LCD_LINE_2, LCD_COLUMN_0, "Number of bytes:    ");
    CEC_Display_DisplayHexToChar ((uint8_t)(HDMI_CEC_RX_MessageStructPrivate.RxMessageLength + 2), LCD_LINE_2, LCD_COLUMN_17);
    CEC_Display_LCD_DisplayString(LCD_LINE_3, LCD_COLUMN_0, "Message Opcode:     ");
    CEC_Display_DisplayHexToChar ((uint8_t)HDMI_CEC_RX_MessageStructPrivate.Opcode, LCD_LINE_3, LCD_COLUMN_17);
    CEC_Display_DisplayBuffer((uint8_t*)HDMI_CEC_RX_MessageStructPrivate.Operande, (HDMI_CEC_RX_MessageStructPrivate.RxMessageLength), LCD_LINE_4);

  }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
  
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
