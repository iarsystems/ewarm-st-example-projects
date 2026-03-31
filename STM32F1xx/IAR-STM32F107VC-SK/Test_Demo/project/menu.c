/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : menu.c
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : This file includes the menu navigation driver for the
*                      STM32F103VE-SK demonstration.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>
#include "menu.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 MenuItemIndex = 0, nMenuLevel = 0;
u8* pszTitleprev;
u8 ItemNumb[MAX_MENU_LEVELS];
tMenuItem psMenuItem, psCurrentMenuItem;
tMenu psPrevMenu[MAX_MENU_LEVELS];
tMenu psNextMenu[MAX_MENU_LEVELS];
tMenu psCurrentMenu;
  u8* CurrentMenu;

struct sMenuItem
{
  u8* pszTitle;
  u8* pszTitleprev;
  tMenuFunc pfMenuFunc;
  tMenuFunc pfUpDownMenuFunc;
  tMenu psSubMenu;
};

struct sMenu
{
  u8* pszTitle;
  tMenuItem psItems;
  u8 nItems;
};



/*------------------------------ Menu level 4 -------------------------------*/

/*------------------------------ Menu level 3 -------------------------------*/
struct sMenuItem ContinueMenuItems[] = {{" LED         ","LED",LED_Test, IdleFunc},
                                        {" Temperature Sensor     ","Temperature Sensor",Temp_Sensor_Test, IdleFunc},
					{" Key Buttons         ","Key Buttons", Button_Test, IdleFunc},	
                                        {" Return       ","Return", ReturnFunc, IdleFunc}};

struct sMenu ContinueMenu = {" Periph Tests ", ContinueMenuItems, countof(ContinueMenuItems)};

/*------------------------------ Menu level 2 -------------------------------*/

struct sMenuItem CommunicationTestMenuItems[] ={{" USART1         ","USART1", USART1_Test, IdleFunc},
                                                {" USART2         ","USART2", USART2_Test, IdleFunc},
                                                {" CAN            ","CAN", CAN_Test, IdleFunc},
                                                {" Ethernet       ","Ethernet", Ethernet_Test, IdleFunc},
                                                {" Return       ","Return", ReturnFunc, IdleFunc}};

struct sMenu CommunicationTestMenu = {" Communication Periph", CommunicationTestMenuItems, countof(CommunicationTestMenuItems)};

struct sMenuItem PeriphTestsMenuItems[] = {                                        {  " Stepper Motor  ","Stepper Motor", Stepper_Motor_Test, IdleFunc    },
                                                                                   {  " Accelerometer         ","Accelerometer", Accelerometer_Test, IdleFunc    },
										   {  " Potentiometer         ","Potentiometer", Potentiometer_Test, IdleFunc},
										   {  " Buzzer         ","Buzzer", Buzzer_Test, IdleFunc},
                                                                                   {  " Audio In/Out         ","Buzzer", Sounds_Test, IdleFunc},
										   {  " Continue...  ","Continue", ContinueFunc, IdleFunc},
                                                                                   {  " Return       ","Return", ReturnFunc, IdleFunc}};									

struct sMenu PeriphTestsMenu = {"     Periph Tests   ", PeriphTestsMenuItems, countof(PeriphTestsMenuItems)};

/*------------------------------ Menu level 1 -------------------------------*/
struct sMenuItem MainMenuItems[] = {

  {" Gen. purpose Periph","Gen. purpose Periph", IdleFunc, IdleFunc, &PeriphTestsMenu},
  {"Communication Periph","Communication Periph", IdleFunc, IdleFunc, &CommunicationTestMenu}};
struct sMenu MainMenu = {"    Main Menu  ", MainMenuItems, countof(MainMenuItems)};//


/* Private function prototypes -----------------------------------------------*/

u8 ReadKey(void);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Menu_Init
* Description    : Initializes the navigation menu.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Menu_Init(void)
{
  DelayResolution100us(Dly);
  GLCD_PowerUpInit(0x0);
  GLCD_Backlight(BACKLIGHT_ON);
  GLCD_SetFont(&Terminal_9_12_6,0xF,0xFFF);
  psCurrentMenu = &MainMenu;
  psPrevMenu[nMenuLevel] = psCurrentMenu;
  psMenuItem = MainMenuItems;
}

/*******************************************************************************
* Function Name  : Display Menu
* Description    : Displays the current menu.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DisplayMenu(void)
{
  vu32 Line = 0,index = 0, MenuItemIndex =0;
 index = 0;
 tMenuItem psMenuItem2;
 GLCD_print ("\f");
 GLCD_TextSetPos(1,(MenuItemIndex+1));
 GLCD_print( "%s\n ", psCurrentMenu->pszTitle);
 GLCD_SetFont(&Terminal_9_12_6,0xF,0xFFF);

  while(!(index >= (psCurrentMenu->nItems)))
  {
     psMenuItem2 = &(psCurrentMenu->psItems[index]);
     GLCD_TextSetPos(1,(Line + 3));
     GLCD_print( "%s\n ", psMenuItem2->pszTitle);
     index++;
     Line ++;
     GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
  }
  /* Get the current menu */
  psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
}

/*******************************************************************************
* Function Name  : SelFunc
* Description    : This function is executed when "SEL" push-buttton is pressed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SelFunc(void)
{
  psCurrentMenuItem = psMenuItem;

  if(psMenuItem->psSubMenu != NULL)
  {
   /* Update the current Item by the submenu */
   MenuItemIndex = 0;
   psCurrentMenu = psMenuItem->psSubMenu;
   psMenuItem = &(psCurrentMenu->psItems)[MenuItemIndex];
   DisplayMenu();
   nMenuLevel++;
   psPrevMenu[nMenuLevel] = psCurrentMenu;
  }

  psCurrentMenuItem->pfMenuFunc();
}

/*******************************************************************************
* Function Name  : UpFunc
* Description    : This function is executed when any of "UP" push-butttons
*                  is pressed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UpFunc(void)
{
 vu32 Line = 0;
  pszTitleprev = psMenuItem->pszTitle;

  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
   if(MenuItemIndex > 0)
   {
    GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
    GLCD_TextSetPos(1,(MenuItemIndex+3));
    GLCD_print( "%s\n ", psMenuItem->pszTitle);
    MenuItemIndex--;
    psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
    GLCD_SetFont(&Terminal_9_12_6,0xF,0xFFF);
    GLCD_TextSetPos(1,(MenuItemIndex+3));
    GLCD_print( "%s\n ", psMenuItem->pszTitle);
  }
   else
  {

    GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
    GLCD_TextSetPos(1,(MenuItemIndex+3));
    GLCD_print( "%s\n ", psMenuItem->pszTitle);
    MenuItemIndex = psCurrentMenu->nItems - 1;
    psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
    GLCD_SetFont(&Terminal_9_12_6,0xF,0xFFF);
    GLCD_TextSetPos(1,(MenuItemIndex+3));
    GLCD_print( "%s\n ", psMenuItem->pszTitle);

  }

  ItemNumb[nMenuLevel] = MenuItemIndex;
}

/*******************************************************************************
* Function Name  : DownFunc
* Description    : This function is executed when any of "Down" push-butttons
*                  is pressed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DownFunc(void)
{
  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];

  /* Test on the MenuItemIndex value before incrementing it */
  if(MenuItemIndex > ((psCurrentMenu->nItems)-2))
  {
    GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
    GLCD_TextSetPos(1,(MenuItemIndex+3));
    GLCD_print( "%s\n ", psMenuItem->pszTitle);
    MenuItemIndex = 0;
    psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
    GLCD_SetFont(&Terminal_9_12_6,0xF,0xFFF);
    GLCD_TextSetPos(1,(MenuItemIndex+3));
    GLCD_print( "%s\n ", psMenuItem->pszTitle);

  }
  else
  {
    GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
    GLCD_TextSetPos(1,(MenuItemIndex+3));
    GLCD_print( "%s\n ", psMenuItem->pszTitle);
    MenuItemIndex++;
    psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
    GLCD_SetFont(&Terminal_9_12_6,0xF,0xFFF);
    GLCD_TextSetPos(1,(MenuItemIndex+3));
    GLCD_print( "%s\n ", psMenuItem->pszTitle);

  }
  ItemNumb[nMenuLevel] = MenuItemIndex;
}

/*******************************************************************************
* Function Name  : ReturnFunc
* Description    : This function is executed when the "RETURN" menu is selected.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ReturnFunc(void)
{
  psMenuItem->pfUpDownMenuFunc();

  if(nMenuLevel == 0)
  {
    nMenuLevel++;
  }

  psCurrentMenu = psPrevMenu[nMenuLevel-1];
  psMenuItem = &psCurrentMenu->psItems[0];
  ItemNumb[nMenuLevel] = 0;
  MenuItemIndex = 0;
  nMenuLevel--;

  DisplayMenu();

}

/*******************************************************************************
* Function Name  : Continue Func
* Description    : This function is executed when the "Continue..." menu is selected.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ContinueFunc(void)
{
  psMenuItem->pfUpDownMenuFunc();
  psNextMenu[3] = &ContinueMenu;
  psCurrentMenu = psNextMenu[3];
  psMenuItem = &psCurrentMenu->psItems[0];
  ItemNumb[nMenuLevel] = 0;
  MenuItemIndex = 0;
  nMenuLevel++;

  DisplayMenu();
}

/*******************************************************************************
* Function Name  : IdleFunc
* Description    : Idle function.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void IdleFunc(void)
{
  /* Nothing to execute: return */
  return;
}

/*******************************************************************************
* Function Name  : IntExtOnOffConfig
* Description    : Enables or disables EXTI for the menu navigation keys :
*                  EXTI lines 14, 15 and 13 which correpond respectively
*                  to "UP", "SEL" and "DOWN".
* Input          : NewState: New state of the navigation keys. This parameter
*                  can be: ENABLE or DISABLE.
* Output         : None
* Return         : None
*******************************************************************************/
void IntExtOnOffConfig(FunctionalState NewState)
{
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Initializes the EXTI_InitStructure */
  EXTI_StructInit(&EXTI_InitStructure);

  /* Disable the EXTI line 12, 13 and 3 on falling edge */
  if(NewState == DISABLE)
  {
    EXTI_InitStructure.EXTI_Line = EXTI_Line12 | EXTI_Line13 | EXTI_Line3;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
  }
  /* Enable the EXTI line 12, 13 and 3 on falling edge */
  else
  {
    /* Clear the the EXTI line 13, 14 and 15 interrupt pending bit */
    EXTI_ClearITPendingBit(EXTI_Line12 | EXTI_Line13 | EXTI_Line3);

    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line = EXTI_Line12| EXTI_Line13 | EXTI_Line3;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
  }
}
/*******************************************************************************
* Function Name  : ReadKey
* Description    : Reads key from demoboard.
* Input          : None
* Output         : None
* Return         : Return RIGHT, LEFT, SEL, UP, DOWN or NOKEY
*******************************************************************************/
u8 ReadKey(void)
{
  /* "right" key is pressed */
  if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2))
  {
    return RIGHT;
  }	

  /* "left" key is pressed */
  if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7))
  {
    return LEFT;
  }

  /* "up" key is pressed */
  if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3))
  {
    return UP;
  }

  /* "down" key is pressed */
  if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13))
  {
    return DOWN;
  }

  /* "sel" key is pressed */
  if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12))
  {
    return SEL;
  }

  /* "Tamper" key is pressed */
  if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
  {
      GLCD_TextSetPos(0,6);
      GLCD_print("TAMPER Key pressed\n");
      GPIO_WriteBit(GPIOF, GPIO_Pin_7, Bit_SET);

    return  Tamper ;
  }

  /* "User" key is pressed */
  if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1))
  {
    return  User ;
  }

  else
  {
    return NOKEY;
  }
}

/*******************************************************************************
* Function Name  : ser_printf
* Description    : Write character to Serial Port.
* Input          : character, USART used
* Output         : None
* Return         : Return character
*******************************************************************************/
int ser_printf (USART_TypeDef* USARTx, int c) {

  while (!(USARTx->SR & USART_FLAG_TXE));
  USARTx->DR = (c & 0x1FF);

  return (c);
}

/*******************************************************************************
* Function Name  : ser_scanf
* Description    : Read character from Serial Port   (blocking read).
* Input          : USART used
* Output         : None
* Return         : Return int
*******************************************************************************/
int ser_scanf (USART_TypeDef* USARTx) {

  while (!(USARTx->SR & USART_FLAG_RXNE));

  return ((int)(USARTx->DR & 0x1FF));
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
