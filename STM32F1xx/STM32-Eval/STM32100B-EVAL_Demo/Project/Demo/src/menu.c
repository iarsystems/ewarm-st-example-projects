/**
  ******************************************************************************
  * @file     menu.c
  * @author   MCD Application Team
  * @version  V1.0.0
  * @date     03/01/2010
  * @brief    This file includes the menu navigation driver for the STM3210B-EVAL demonstration.
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

/** @defgroup MENU
  * @brief    This file includes the menu navigation driver for the STM3210B-EVAL 
  *           demonstration.
  * @{
  */

/** @defgroup MENU_Private_Types
  * @{
  */
typedef void (* tMenuFunc)(void);
typedef struct sMenuItem * tMenuItem;
typedef struct sMenu * tMenu;
/**
  * @}
  */


/** @defgroup MENU_Private_Defines
  * @{
  */
#define NUMBER_OF_ICONS      10
#define TEMPERATURE_THYS     31
#define TEMPERATURE_TOS      32
/**
  * @}
  */

/** @defgroup MENU_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup MENU_Private_Variables
  * @{
  */
uint8_t MenuItemIndex = 0, nMenuLevel = 0;
uint8_t ItemNumb[MAX_MENU_LEVELS];
uint8_t NumberOfFiles = 0;
uint32_t bmplen = 0;
extern uint8_t Wavebuffer[];
char* DirectoryFiles[MAX_BMP_FILES];
tMenuItem psMenuItem, psCurrentMenuItem;
tMenu psPrevMenu[MAX_MENU_LEVELS];
tMenu psCurrentMenu;
extern VOLINFO  vi;
extern DIRINFO  di;
extern uint8_t sector[SECTOR_SIZE];
uint8_t Pressed_Key_Button = 0;
extern __IO uint32_t WaveDataLength;

static uint8_t TempCelsiusDisplay[]     = "       +abc.d C     ";
uint8_t TempFahrenheitDisplay[]  = "       +abc.d F     ";
static int32_t TempValue = 0, TempValueCelsius = 0, TempValueFahrenheit = 0;
__IO uint8_t SMbusAlertOccurred = 0, CEC_Menu = 0;
struct sMenuItem
{
  uint8_t* pszTitle;
  tMenuFunc pfMenuFunc;
  tMenuFunc pfUpDownMenuFunc;
  tMenu psSubMenu;
};
struct sMenu
{
  uint8_t* pszTitle;
  tMenuItem psItems;
  uint8_t nItems;
};
const uint8_t* Images1Addr[2] = {"STFILES/STLOGO.BMP", "STFILES/HELP.BMP"};
const uint8_t* Images2Addr[2] = {"STFILES/ALARM.BMP", "STFILES/WATCH.BMP"};
const uint8_t* IconsAddr[10] =
  {
   "STFILES/ICON1.BMP", 
   "STFILES/ICON2.BMP", 
   "STFILES/ICON3.BMP", 
   "STFILES/ICON4.BMP", 
   "STFILES/ICON5.BMP",
   "STFILES/ICON6.BMP", 
   "STFILES/ICON7.BMP", 
   "STFILES/ICON8.BMP", 
   "STFILES/ICON9.BMP", 
   "STFILES/ICON10.BMP"
  };
const uint8_t* SlideAddr[14] =
  {
   "STFILES/SLIDE1.BMP", 
   "STFILES/SLIDE2.BMP", 
   "STFILES/SLIDE3.BMP", 
   "STFILES/SLIDE4.BMP", 
   "STFILES/SLIDE5.BMP",
   "STFILES/SLIDE6.BMP", 
   "STFILES/SLIDE7.BMP", 
   "STFILES/SLIDE8.BMP", 
   "STFILES/SLIDE9.BMP", 
   "STFILES/SLIDE10.BMP",
   "STFILES/SLIDE11.BMP", 
   "STFILES/SLIDE12.BMP", 
   "STFILES/SLIDE13.BMP", 
   "STFILES/SLIDE14.BMP"
  };

const uint32_t SlidesSpeech[14] =
  {
    0x000000, 0x031A00, 0x06A200, 0x079000, 0x0A7600, 0x104000, 0x132600, 
    0x176600, 0x1B9C00, 0x1D8200, 0x21F400, 0x24CE00, 0x28E400, 0x2BB200
   };

const uint32_t DelayTiming[14] = {2650, 3020, 800, 2480, 4950, 2480, 3640, 3600, 1630, 3800, 2520, 3500, 2400, 1770};

const uint8_t SlidesCheck[6] = {0x42, 0x4D, 0x42, 0x58, 0x02, 0x00};
const uint8_t Icons64Check[6] = {0x42, 0x4D, 0x42, 0x20, 0x00, 0x00};
const uint8_t Icons128Check[6] = {0x42, 0x4D, 0x42, 0x80, 0x00, 0x00};

const uint8_t DemoTitle[20] = "STM32 DAC Audio Demo";
const uint8_t CmdTitle0[20] = " Playing Wave files ";

/*------------------------------ Menu level 4 -------------------------------*/
/*------------------------------ Menu level 3 -------------------------------*/
struct sMenuItem LOWPOWERSTOPMenuItems[] =
  {
    {"    Exit: EXTI      ", LowPower_EnterSTOPMode_EXTI, Menu_IdleFunc
    },
    {"  Exit: RTC Alarm   ", LowPower_EnterSTOPMode_RTCAlarm, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu LOWPOWERSTOPMenu =
  {"     STOP Mode      ", LOWPOWERSTOPMenuItems, countof(LOWPOWERSTOPMenuItems)
  };
struct sMenuItem LOWPOWERSTANDBYMenuItems[] =
  {
    {"  Exit: Wakeup Pin  ", LowPower_EnterSTANDBYMode_WAKEUP, Menu_IdleFunc
    },
    {"  Exit: RTC Alarm   ", LowPower_EnterSTANDBYMode_RTCAlarm, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu LOWPOWERSTANDBYMenu =
  {"   STANDBY Mode     ", LOWPOWERSTANDBYMenuItems, countof(LOWPOWERSTANDBYMenuItems)
  };
struct sMenuItem SettingTimeMenuItems[] =
  {
    {"       Adjust       ", Calendar_TimeAdjust, Menu_IdleFunc
    },
    {"        Show        ", Calendar_TimeShow, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu TimeMenu =
  {"        Time        ", SettingTimeMenuItems, countof(SettingTimeMenuItems)
  };
struct sMenuItem SettingDateMenuItems[] =
  {
    {"       Adjust       ", Calendar_DateAdjust, Menu_IdleFunc
    },
    {"        Show        ", Calendar_DateShow, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu DateMenu =
  {"        Date        ", SettingDateMenuItems, countof(SettingDateMenuItems)
  };
struct sMenuItem SettingAlarmMenuItems[] =
  {
    {"       Adjust       ", Calendar_AlarmAdjust, Menu_IdleFunc
    },
    {"        Show        ", Calendar_AlarmShow, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu AlarmMenu =
  {"        Alarm       ", SettingAlarmMenuItems, countof(SettingAlarmMenuItems)
  };
/*------------------------------ Menu level 2 -------------------------------*/
struct sMenuItem AboutMenuItems[] =
  {
    {"       About        ", Menu_AboutFunc, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu AboutMenu =
  {"        About       ", AboutMenuItems, countof(AboutMenuItems)
  };
struct sMenuItem HelpMenuItems[] =
  {
    {"       Start        ", Menu_HelpFunc, Menu_IdleFunc
    },
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu HelpMenu =
  {"        Help        ", HelpMenuItems, countof(HelpMenuItems)
  };
struct sMenuItem CECDisplayMenuItems[] =
  {
    {"     HDMI CEC       ", Menu_CECDisplayFunc, Menu_IdleFunc
    },
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu CECDisplayMenu =
  {"     HDMI CEC       ", CECDisplayMenuItems, countof(CECDisplayMenuItems)
  };
struct sMenuItem TempSensorMenuItems[] =
  {
    {"    Temperature     ", Menu_ThermometerFun, Menu_IdleFunc
    },
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu TempSensorMenu =
  {"    Thermometer     ", TempSensorMenuItems, countof(TempSensorMenuItems)
  };

struct sMenuItem IDDMeasureMenuItems[] =
  {
    {"    IDD RUN Mode    ", IDD_Measurement_Run, Menu_IdleFunc
    },
    {"    IDD STOP Mode   ", IDD_Measurement_Stop, Menu_IdleFunc},
    {"   IDD SLEEP Mode   ", IDD_Measurement_Sleep, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };

struct sMenu IDDMeasureMenu =
  {"      IDD Measure   ", IDDMeasureMenuItems, countof(IDDMeasureMenuItems)
  };
struct sMenuItem LowPowerMenuItems[] =
  {
    {"        STOP        ", Menu_IdleFunc, Menu_IdleFunc, &LOWPOWERSTOPMenu
    },
    {"       STANDBY      ", Menu_IdleFunc, Menu_IdleFunc, &LOWPOWERSTANDBYMenu},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu LowPowerMenu =
  {"    Low Power Mode  ", LowPowerMenuItems, countof(LowPowerMenuItems)
  };
struct sMenuItem WavePlayerMenuItems[] =
  {
    {"      WavePlayer    ", Menu_WavePlayerFunc, Menu_IdleFunc
    },
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu WavePlayerMenu =
  {"      WavePlayer    ", WavePlayerMenuItems, countof(WavePlayerMenuItems)
  };
struct sMenuItem ImagesViewerMenuItems[] =
  {
    {"    Images Viewer   ", Menu_ImagesViewerFunc, Menu_IdleFunc
    },
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu ImagesViewerMenu =
  {"    Images Viewer   ", ImagesViewerMenuItems, countof(ImagesViewerMenuItems)
  };
struct sMenuItem CalendarMenuItems[] =
  {
    {"        Time        ", Menu_IdleFunc, Menu_IdleFunc, &TimeMenu},
    {"        Date        ", Menu_IdleFunc, Menu_IdleFunc, &DateMenu},
    {"        Alarm       ", Menu_IdleFunc, Menu_IdleFunc, &AlarmMenu},
    {"        Return      ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu CalendarMenu =
  {"       Calendar     ", CalendarMenuItems, countof(CalendarMenuItems)
  };
struct sMenuItem ProdPresMenuItems[] =
  {
    {"       Start        ", Menu_ProductPres, Menu_IdleFunc
    },
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu ProdPresMenu =
  {"Product Presentation", ProdPresMenuItems, countof(ProdPresMenuItems)
  };
/*------------------------------ Menu level 1 -------------------------------*/
struct sMenuItem MainMenuItems[] =
  {
    {"Product Presentation", Menu_IdleFunc, Menu_IdleFunc, &ProdPresMenu
    },
    {"       Calendar     ", Menu_IdleFunc, Menu_IdleFunc, &CalendarMenu},
    {"    Images Viewer   ", Menu_IdleFunc, Menu_IdleFunc, &ImagesViewerMenu},
    {"      WavePlayer    ", Menu_IdleFunc, Menu_IdleFunc, &WavePlayerMenu},
    {"    Low Power Mode  ", Menu_IdleFunc, Menu_IdleFunc, &LowPowerMenu},
    {"    IDD Measure     ", Menu_IdleFunc, Menu_IdleFunc, &IDDMeasureMenu},
    {"    Thermometer     ", Menu_IdleFunc, Menu_IdleFunc, &TempSensorMenu},
    {"     HDMI CEC       ", Menu_IdleFunc, Menu_IdleFunc, &CECDisplayMenu},
    {"        Help        ", Menu_IdleFunc, Menu_IdleFunc, &HelpMenu},
    {"        About       ", Menu_IdleFunc, Menu_IdleFunc, &AboutMenu}
  };
struct sMenu MainMenu =
  {"     Main menu      ", MainMenuItems, countof(MainMenuItems)
  };
/**
  * @}
  */

/** @defgroup MENU_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup MENU_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the navigation menu.
  * @param  None
  * @retval None
  */
void Menu_Init(void)
{
  psCurrentMenu = &MainMenu;
  psPrevMenu[nMenuLevel] = psCurrentMenu;
  psMenuItem = MainMenuItems;
}


/**
  * @brief  Displays the current menu.
  * @param  None
  * @retval None
  */
void Menu_DisplayMenu(void)
{
  uint32_t Line = 0, index = 0;

  tMenuItem psMenuItem2;
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  /* Clear the LCD Screen */
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(Line, psCurrentMenu->pszTitle);
  Line += 24;
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  while (!(index >= (psCurrentMenu->nItems)))
  {
    psMenuItem2 = &(psCurrentMenu->psItems[index]);
    LCD_DisplayStringLine(Line, psMenuItem2->pszTitle);
    index++;
    Line += 24;
  }
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_GREEN);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  /* Get the current menu */
  psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
}

/**
  * @brief  This function is executed when "SEL" push-buttton is pressed.
  * @param  None
  * @retval None
  */
void Menu_SelFunc(void)
{
  psCurrentMenuItem = psMenuItem;
  if (psMenuItem->psSubMenu != '\0')
  {
    /* Update the current Item by the submenu */
    MenuItemIndex = 0;
    psCurrentMenu = psMenuItem->psSubMenu;
    psMenuItem = &(psCurrentMenu->psItems)[MenuItemIndex];
    Menu_DisplayMenu();
    nMenuLevel++;
    psPrevMenu[nMenuLevel] = psCurrentMenu;
  }
  psCurrentMenuItem->pfMenuFunc();
}


/**
  * @brief  This function is executed when any of "UP" push-butttons is pressed.
  * @param  None
  * @retval None
  */
void Menu_UpFunc(void)
{
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
  if (MenuItemIndex > 0)
  {
    MenuItemIndex--;
  }
  else
  {
    MenuItemIndex = psCurrentMenu->nItems - 1;
  }
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_GREEN);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
  ItemNumb[nMenuLevel] = MenuItemIndex;
}

/**
  * @brief  This function is executed when any of "Down" push-butttons is pressed.
  * @param  None
  * @retval None
  */
void Menu_DownFunc(void)
{
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);

  /* Test on the MenuItemIndex value before incrementing it */
  if (MenuItemIndex >= ((psCurrentMenu->nItems) - 1))
  {
    MenuItemIndex = 0;
  }
  else
  {
    MenuItemIndex++;
  }
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_GREEN);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  /* Get the current menu */
  psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
  ItemNumb[nMenuLevel] = MenuItemIndex;
}

/**
  * @brief  This function is executed when the "RETURN" menu is selected.
  * @param  None
  * @retval None
  */
void Menu_ReturnFunc(void)
{
  psMenuItem->pfUpDownMenuFunc();
  if (nMenuLevel == 0)
  {
    nMenuLevel++;
  }
  psCurrentMenu = psPrevMenu[nMenuLevel-1];
  psMenuItem = &psCurrentMenu->psItems[0];
  ItemNumb[nMenuLevel] = 0;
  MenuItemIndex = 0;
  nMenuLevel--;
  if (nMenuLevel != 0)
  {
    Menu_DisplayMenu();
  }
  else
  {
    Menu_ShowMenuIcons();
  }
}

/**
  * @brief  Reads key from demoboard.
  * @param  None
  * @retval Return RIGHT, LEFT, SEL, UP, DOWN or NOKEY
  */
uint8_t Menu_ReadKey(void)
{
  /* "right" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_RIGHT))
  {
    while (STM_EVAL_PBGetState(BUTTON_RIGHT) == Bit_RESET);
    return RIGHT;
  }
  /* "left" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_LEFT))
  {
    while (STM_EVAL_PBGetState(BUTTON_LEFT) == Bit_RESET);
    return LEFT;
  }
  /* "up" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_UP))
  {
    while (STM_EVAL_PBGetState(BUTTON_UP) == Bit_RESET);
    return UP;
  }
  /* "down" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_DOWN))
  {
    while (STM_EVAL_PBGetState(BUTTON_DOWN) == Bit_RESET);
    return DOWN;
  }
  /* "sel" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_SEL))
  {
    while (STM_EVAL_PBGetState(BUTTON_SEL) == Bit_RESET);
    return SEL;
  }
  /* No key is pressed */
  else
  {
    return NOKEY;
  }
}

/**
  * @brief  Idle function.
  * @param  None
  * @retval None
  */
void Menu_IdleFunc(void)
{
  /* Nothing to execute: return */
  return;
}

/**
  * @brief  Display menu icons.
  * @param  None
  * @retval None
  */
void Menu_DisplayIcons(void)
{
  uint32_t i = 0, j = 0, l = 0,  iconline = 0, iconcolumn = 0;

  iconline = 98;
  iconcolumn = 290 ;

  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 4; j++)
    {
      LCD_SetDisplayWindow(iconline, iconcolumn, 64, 64);
      Storage_OpenReadFile("STFILES", (uint8_t *)IconsAddr[l], (uint32_t)&Wavebuffer, &bmplen);
      iconcolumn -= 65;
      l++;
      if (l == NUMBER_OF_ICONS )
      {
        return;
      }
    }
    iconline += 65;
    iconcolumn = 290;
  }
}

/**
  * @brief  Show the main menu icon.
  * @param  None
  * @retval None
  */
void Menu_ShowMenuIcons(void)
{
  uint32_t MyKey = 0, i = 0;
  uint16_t IconRect[12][2] = {{34, 290}, {34, 225}, {34, 160}, {34, 95},
                              {99, 290}, {99, 225}, {99, 160}, {99, 95},
                              {164, 290}, {164, 225}, {164, 160}, {164, 95}};

  /* Disable the JoyStick interrupts */
  Demo_IntExtOnOffConfig(DISABLE);

  while (Menu_ReadKey() != NOKEY)
  {}

  /* Initializes the Menu state machine */
  Menu_Init();

  MenuItemIndex = 0;

  /* Clear*/
  LCD_Clear(LCD_COLOR_WHITE);

  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);

  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);

  LCD_DisplayStringLine(LCD_LINE_0, psMenuItem->pszTitle);

  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);

  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);

  /* Displays Icons */
  Menu_DisplayIcons();

  /* Disable LCD Window mode */
  LCD_WindowModeDisable();


  LCD_DrawRect(IconRect[0][0], IconRect[0][1], 64, 65);


  /* Endless loop */
  while (1)
  {
    /* Check which key is pressed */
    MyKey = Menu_ReadKey();

    /* If "UP" pushbutton is pressed */
    if (MyKey == UP)
    {
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);

      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);

      if (i <= 3)
      {
        i += 8;
        if (i >= NUMBER_OF_ICONS)
        {
          i = (NUMBER_OF_ICONS- 1);
        }
      }
      else
      {
        i -= 4;
      }
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_BLUE);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);

      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      /* Test on the MenuItemIndex value before incrementing it */
      if (MenuItemIndex <= 3)
      {
        MenuItemIndex += 8;
        if (MenuItemIndex >= NUMBER_OF_ICONS)
        {
          MenuItemIndex = (NUMBER_OF_ICONS- 1);
        }
      }
      else
      {
        MenuItemIndex -= 4;
      }
      /* Get the current menu */
      psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
      LCD_DisplayStringLine(LCD_LINE_0, psMenuItem->pszTitle);
      ItemNumb[nMenuLevel] = MenuItemIndex;
    }
    /* If "DOWN" pushbutton is pressed */
    if (MyKey == DOWN)
    {
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);
      if (i >= 8)
      {
        i -= 8;
      }
      else
      {
        i += 4;
        if (i >= NUMBER_OF_ICONS)
        {
          i = (NUMBER_OF_ICONS- 1);
        }
      }
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_BLUE);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);

      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      /* Test on the MenuItemIndex value before incrementing it */
      if (MenuItemIndex >= 8)
      {
        MenuItemIndex -= 8;
      }
      else
      {
        MenuItemIndex += 4;
        if (MenuItemIndex >= NUMBER_OF_ICONS)
        {
          MenuItemIndex = (NUMBER_OF_ICONS- 1);
        }
      }
      /* Get the current menu */
      psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
      LCD_DisplayStringLine(LCD_LINE_0, psMenuItem->pszTitle);
      ItemNumb[nMenuLevel] = MenuItemIndex;
    }
    /* If "LEFT" pushbutton is pressed */
    if (MyKey == LEFT)
    {
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);
      if (i == 0)
      {
        i = (NUMBER_OF_ICONS- 1);
      }
      else
      {
        i--;
      }
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_BLUE);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);

      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      if (MenuItemIndex > 0)
      {
        MenuItemIndex--;
      }
      else
      {
        MenuItemIndex = psCurrentMenu->nItems - 1;
      }

      psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
      LCD_DisplayStringLine(LCD_LINE_0, psMenuItem->pszTitle);
      ItemNumb[nMenuLevel] = MenuItemIndex;
    }
    /* If "RIGHT" pushbutton is pressed */
    if (MyKey == RIGHT)
    {
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);
      if (i == (NUMBER_OF_ICONS- 1))
      {
        i = 0x00;
      }
      else
      {
        i++;
      }
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_BLUE);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);

      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      /* Test on the MenuItemIndex value before incrementing it */
      if (MenuItemIndex >= ((psCurrentMenu->nItems) - 1))
      {
        MenuItemIndex = 0;
      }
      else
      {
        MenuItemIndex++;
      }
      /* Get the current menu */
      psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
      LCD_DisplayStringLine(LCD_LINE_0, psMenuItem->pszTitle);
      ItemNumb[nMenuLevel] = MenuItemIndex;
    }
    /* If "SEL" pushbutton is pressed */
    if (MyKey == SEL)
    {
      Menu_SelFunc();
      Demo_IntExtOnOffConfig(ENABLE);
      return;
    }
  }
}

/**
  * @brief CEC display function
  * @param  None
  * @retval : None
  */
void Menu_CECDisplayFunc(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;  
  ErrorStatus HSEStartUpStatus;

  CEC_Menu = 1;
  /* Disable the JoyStick Interrupts */
  Demo_IntExtOnOffConfig(DISABLE);

  /* Initializes the EXTI_InitStructure */
  EXTI_StructInit(&EXTI_InitStructure);
  /* Clear the the EXTI line 9 interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_Line9);

  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable the EXTI9_5_IRQn Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  Pressed_Key_Button = 0;

  /* Display initialization */
  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  CEC_Display(LCD_LINE_0, "CEC Initializing.   ");
  CEC_Display(LCD_LINE_1, "Please wait...      ");

  /* Initialize the CEC Demo */
  CEC_Display_Init();

  while (Pressed_Key_Button == 0)
  {
    CEC_Display_CECAvailableCommands();
  }
  Pressed_Key_Button = 0;
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* DeInitialize the CEC peripheral */
  CEC_DeInit();

  /* Restore the System Clock */
  /* Select HSI as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
  /* Disable PLL */
  RCC_PLLCmd(DISABLE);
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  
  if (HSEStartUpStatus == SUCCESS)
  {
	/* Configure the PLL Source */
	RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {}
  }

  CEC_Menu = 0;
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  /* Display the previous menu */
  Menu_DisplayMenu();
  /* UP Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource8);
  /* Enable the JoyStick interrupts */
  Demo_IntExtOnOffConfig(ENABLE);
}

/**
  * @brief  Display the STM32 introduction.
  * @param  None
  * @retval None
  */
void Menu_STM32Intro(void)
{
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  Storage_OpenReadFile("STFILES", "STFILES/STLOGO.BMP", (uint32_t)&Wavebuffer, &bmplen);
  Demo_Delay(100);

  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  Storage_OpenReadFile("STFILES", "STFILES/SLIDE1.BMP", (uint32_t)&Wavebuffer, &bmplen);
  Demo_Delay(100);

  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  Storage_OpenReadFile("STFILES", "STFILES/VALUE.BMP", (uint32_t)&Wavebuffer, &bmplen);
  Demo_Delay(100);  
}

/**
  * @brief  Display the Help menu.
  * @param  None
  * @retval None
  */
void Menu_HelpFunc(void)
{
  Demo_IntExtOnOffConfig(DISABLE);
  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  Storage_OpenReadFile("STFILES", "STFILES/HELP.BMP", (uint32_t)&Wavebuffer, &bmplen);
  while (Menu_ReadKey() == NOKEY)
  {}
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_0, "UP, DOWN, RIGHT and ");
  LCD_DisplayStringLine(LCD_LINE_1, "LEFT push-buttons   ");
  LCD_DisplayStringLine(LCD_LINE_2, "perform circular    ");
  LCD_DisplayStringLine(LCD_LINE_3, "navigation in the   ");
  LCD_DisplayStringLine(LCD_LINE_4, "main menu, current  ");
  LCD_DisplayStringLine(LCD_LINE_5, "menu items. SEL     ");
  LCD_DisplayStringLine(LCD_LINE_6, "push-button selects ");
  LCD_DisplayStringLine(LCD_LINE_7, "the current item. UP");
  LCD_DisplayStringLine(LCD_LINE_8, "and DOWN perform    ");
  LCD_DisplayStringLine(LCD_LINE_9, "vertical navigation.");

  while (Menu_ReadKey() == NOKEY)
  {}
  LCD_Clear(LCD_COLOR_WHITE);
  Menu_DisplayMenu();
  Demo_IntExtOnOffConfig(ENABLE);
}
/**
  * @brief  Display the Images stored on the SDcard.
  * @param  None
  * @retval None
  */
void Menu_ImagesViewerFunc(void)
{
  uint8_t str[20], KeyState = 0;
  int8_t i = 0;

  LCD_Clear(LCD_COLOR_WHITE);
  Demo_IntExtOnOffConfig(DISABLE);

  while (Menu_ReadKey() != NOKEY)
  {}

  for (i = 0; i < MAX_BMP_FILES; i++)
  {
    DirectoryFiles[i] = malloc(11); /** Initialize the DirectoryFiles pointers (heap) */
  }

  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);

  NumberOfFiles = Storage_GetDirectoryFiles (DirectoryFiles); /** Get the BMP file names on root directory  Maximum 10 files */
  i = 0;

  if(NumberOfFiles == 0)
  {
    for (i = 0; i < MAX_BMP_FILES; i++)
    {
      free(DirectoryFiles[i]);
    }
    LCD_Clear(LCD_COLOR_WHITE);
    /* Set the Back Color */
    LCD_SetBackColor(LCD_COLOR_BLACK);
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DisplayStringLine(LCD_LINE_0, "No Bitmap files...  ");
    LCD_DisplayStringLine(LCD_LINE_1, "Exit:  Push JoyStick");
    while (Menu_ReadKey() == NOKEY)
    {}    
    LCD_Clear(LCD_COLOR_WHITE);
    Menu_DisplayMenu();
    Demo_IntExtOnOffConfig(ENABLE);
    return;    
  }
  while (i < NumberOfFiles)
  {
    sprintf ((char*)str, "USER/%-11.11s", DirectoryFiles[i]); /** Format the string */
    if(Storage_CheckBitmapFile("USER", str, (uint32_t)&Wavebuffer, &bmplen) == 0)
    {
      LCD_SetDisplayWindow(239, 0x13F, 240, 320);
      Storage_OpenReadFile("USER", str, (uint32_t)&Wavebuffer, &bmplen);
      KeyState = Demo_DelayJoyStick(100);
      if (KeyState == LEFT)
      {
        if (i == 0)
        {
          i = NumberOfFiles;
        }
        i--;
      }
      else if (KeyState == RIGHT)
      {
        if (i == (NumberOfFiles - 1))
        {
          i = -1;
        }
        i++;
      }
      else if (KeyState == SEL)
      {
        i = NumberOfFiles;
      }
      else if (KeyState == NOKEY)
      {
        i++;
      }
    }
    else
    {
      LCD_Clear(LCD_COLOR_WHITE);
      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      /* Set the Text LCD_COLOR_WHITE */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_ClearLine(LCD_LINE_3);
      LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *) str);
      LCD_DisplayStringLine(LCD_LINE_4, "Bitmap file is not  ");
      LCD_DisplayStringLine(LCD_LINE_5, "supported.          ");      
      LCD_DisplayStringLine(LCD_LINE_6, "Press JoyStick to   ");
      LCD_DisplayStringLine(LCD_LINE_7, "continue.           ");       
      KeyState = Demo_DelayJoyStick(100);
      if (KeyState == LEFT)
      {
        if (i == 0)
        {
          i = NumberOfFiles;
        }
        i--;
      }
      else if (KeyState == RIGHT)
      {
        if (i == (NumberOfFiles - 1))
        {
          i = -1;
        }
        i++;
      }
      else if (KeyState == SEL)
      {
        i = NumberOfFiles;
      }
      else if (KeyState == NOKEY)
      {
        i++;
      }
    }
  }

  for (i = 0; i < MAX_BMP_FILES; i++)
  {
    free(DirectoryFiles[i]);
  }

  LCD_Clear(LCD_COLOR_WHITE);
  Menu_DisplayMenu();
  Demo_IntExtOnOffConfig(ENABLE);
}


/**
  * @brief  Play wave files stored on the SDcard.
  * @param  None
  * @retval None
  */
void Menu_WavePlayerFunc(void)
{
  uint8_t str[20], KeyState = 0;

  int8_t i = 0;

  LCD_Clear(LCD_COLOR_WHITE);

  Demo_IntExtOnOffConfig(DISABLE);
  while (Menu_ReadKey() != NOKEY)
  {}

  for (i = 0; i < MAX_WAVE_FILES; i++)
  {
    DirectoryFiles[i] = malloc(11); /** Initialize the DirectoryFiles pointers (heap) */
  }

  NumberOfFiles = Storage_GetDirectoryWaveFiles (DirectoryFiles); /** Get the BMP file names on root directory  Maximum 10 files */

  if(NumberOfFiles == 0)
  {
    for (i = 0; i < MAX_BMP_FILES; i++)
    {
      free(DirectoryFiles[i]);
    }
    LCD_Clear(LCD_COLOR_WHITE);
    /* Set the Back Color */
    LCD_SetBackColor(LCD_COLOR_BLACK);
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DisplayStringLine(LCD_LINE_0, "No wave files...    ");
    LCD_DisplayStringLine(LCD_LINE_1, "Exit:  Push JoyStick");
    while (Menu_ReadKey() == NOKEY)
    {}    
    LCD_Clear(LCD_COLOR_WHITE);
    Menu_DisplayMenu();
    Demo_IntExtOnOffConfig(ENABLE);    
    return;
  }

  /* Set the text and the background color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_BLACK);
  LCD_Clear(LCD_COLOR_WHITE);

  /* Display the Titles */
  LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)DemoTitle);
  LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)CmdTitle0);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_3, "SEL  -> Play        ");
  LCD_DisplayStringLine(LCD_LINE_4, "DOWN -> Return      ");
  LCD_DisplayStringLine(LCD_LINE_5, "LEFT -> Next Wave   ");
  LCD_DisplayStringLine(LCD_LINE_6, "RIGHT->Previous Wave");
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_ClearLine(LCD_LINE_7);
  
  i = 0;
  LCD_SetBackColor(LCD_COLOR_GREEN);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_8, "                    ");

  sprintf ((char*)str, "USER/%-11.11s", DirectoryFiles[0]); /** Format the string */
  LCD_DisplayStringLine(LCD_LINE_8, (uint8_t *) str);

  while (i < NumberOfFiles)
  {
    KeyState = Menu_ReadKey();
    if (KeyState == LEFT)
    {
      if (i == 0)
      {
        i = NumberOfFiles;
      }
      i--;
      LCD_SetBackColor(LCD_COLOR_GREEN);
      LCD_DisplayStringLine(LCD_LINE_8, "                    ");
      sprintf ((char*)str, "USER/%-11.11s", DirectoryFiles[i]); /** Format the string */
      LCD_DisplayStringLine(LCD_LINE_8, (uint8_t *) str);
    }
    else if (KeyState == RIGHT)
    {
      if (i == (NumberOfFiles - 1))
      {
        i = -1;
      }
      i++;
      LCD_SetBackColor(LCD_COLOR_GREEN);
      LCD_DisplayStringLine(LCD_LINE_8, "                    ");
      sprintf ((char*)str, "USER/%-11.11s", DirectoryFiles[i]); /** Format the string */
      LCD_DisplayStringLine(LCD_LINE_8, (uint8_t *) str);
    }
    else if (KeyState == DOWN)
    {
      i = NumberOfFiles;
    }
    else if (KeyState == SEL)
    {
      /* Set the text and the background color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      LCD_DisplayStringLine(LCD_LINE_3, "SEL  PAUSE LEFT  BWD");
      LCD_DisplayStringLine(LCD_LINE_4, "DOWN STOP  RIGHT FWD");
      LCD_SetBackColor(LCD_COLOR_WHITE);
      LCD_ClearLine(LCD_LINE_2);
      LCD_ClearLine(LCD_LINE_5);
      LCD_SetTextColor(LCD_COLOR_RED);
      LCD_SetBackColor(LCD_COLOR_BLUE);
      LCD_ClearLine(LCD_LINE_7);
      LCD_DisplayStringLine(LCD_LINE_6, "       Playing      ");
      LCD_SetBackColor(LCD_COLOR_GREEN);
      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_DisplayStringLine(LCD_LINE_8, "                    ");
      sprintf ((char*)str, "USER/%-11.11s", DirectoryFiles[i]); /** Format the string */
      LCD_DisplayStringLine(LCD_LINE_8, (uint8_t *) str);
      KeyState = WavePlayerMenu_Start("USER", str, &bmplen);
      if (KeyState == NOKEY)
      {
        if(Get_WaveFileStatus() != Valid_WAVE_File)
        {
          /* Set the Back Color */
          LCD_SetBackColor(LCD_COLOR_BLUE);
          /* Set the Text LCD_COLOR_WHITE */
          LCD_SetTextColor(LCD_COLOR_WHITE);
          LCD_ClearLine(LCD_LINE_3);
          LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *) str);
          LCD_DisplayStringLine(LCD_LINE_4, "Wave file is not    ");
          LCD_DisplayStringLine(LCD_LINE_5, "supported.          ");      
          LCD_DisplayStringLine(LCD_LINE_6, "Press JoyStick to   ");
          LCD_DisplayStringLine(LCD_LINE_7, "continue.           ");
          while (Menu_ReadKey() != NOKEY) 
          {           
          }
          KeyState = Menu_ReadKey();
          while (KeyState == NOKEY)
          {
            KeyState = Menu_ReadKey();            
          }
          if (KeyState == LEFT)
          {
            if (i == 0)
            {
              i = NumberOfFiles;
            }
            i--;
            LCD_SetBackColor(LCD_COLOR_GREEN);
            LCD_DisplayStringLine(LCD_LINE_8, "                    ");
            sprintf ((char*)str, "USER/%-11.11s", DirectoryFiles[i]); /** Format the string */
            LCD_DisplayStringLine(LCD_LINE_8, (uint8_t *) str);
          }
          else if (KeyState == RIGHT)
          {
            if (i == (NumberOfFiles - 1))
            {
              i = -1;
            }
            i++;
            LCD_SetBackColor(LCD_COLOR_GREEN);
            LCD_DisplayStringLine(LCD_LINE_8, "                    ");
            sprintf ((char*)str, "USER/%-11.11s", DirectoryFiles[i]); /** Format the string */
            LCD_DisplayStringLine(LCD_LINE_8, (uint8_t *) str);
          }
          else if (KeyState == DOWN)
          {
            i = NumberOfFiles;
          }
          LCD_SetTextColor(LCD_COLOR_WHITE);
          LCD_SetBackColor(LCD_COLOR_BLUE);
          LCD_DisplayStringLine(LCD_LINE_3, "SEL  -> Play        ");
          LCD_DisplayStringLine(LCD_LINE_4, "DOWN -> Exit        ");
          LCD_DisplayStringLine(LCD_LINE_5, "LEFT -> Next Wave   ");
          LCD_DisplayStringLine(LCD_LINE_6, "RIGHT->Previous Wave");
          LCD_ClearLine(LCD_LINE_7);            
        }
        else
        {
          i++;
        }
      }
      KeyState = NOKEY;
    }
  }
  for (i = 0; i < MAX_WAVE_FILES; i++)
  {
    free(DirectoryFiles[i]);
  }

  LCD_Clear(LCD_COLOR_WHITE);
  Menu_DisplayMenu();
  Demo_IntExtOnOffConfig(ENABLE);
}


/**
  * @brief  Display the About menu.
  * @param  None
  * @retval None
  */
void Menu_AboutFunc(void)
{
  LCD_Clear(LCD_COLOR_WHITE);
  Demo_IntExtOnOffConfig(DISABLE);
  while (Menu_ReadKey() != NOKEY)
  {}
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_ClearLine(LCD_LINE_0);
  LCD_DisplayStringLine(LCD_LINE_1, "   STM32100B-EVAL   ");
  LCD_DisplayStringLine(LCD_LINE_2, "   Demonstration    ");
  LCD_DisplayStringLine(LCD_LINE_3, "   Version 1.0.0    ");
  LCD_DisplayStringLine(LCD_LINE_4, "     03/01/2010     ");
  LCD_ClearLine(LCD_LINE_5);
  LCD_DisplayStringLine(LCD_LINE_6, "MCD Application Team");
  LCD_DisplayStringLine(LCD_LINE_7, "   COPYRIGHT 2010   ");
  LCD_DisplayStringLine(LCD_LINE_8, " STMicroelectronics ");
  LCD_DisplayStringLine(LCD_LINE_9, "  www.st.com/stm32  ");
  while (Menu_ReadKey() == NOKEY)
  {}
  LCD_Clear(LCD_COLOR_WHITE);
  Menu_DisplayMenu();
  Demo_IntExtOnOffConfig(ENABLE);
}

/**
  * @brief  Display the About menu.
  * @param  None
  * @retval None
  */
void Menu_ProductPres(void)
{
  __IO uint32_t index = 0, KeyState = 0;
  Demo_IntExtOnOffConfig(DISABLE);

  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  Storage_OpenReadFile("STFILES", "STFILES/VALUE.BMP", (uint32_t)&Wavebuffer, &bmplen);
  Demo_Delay(100); 
  
  while (Menu_ReadKey() != NOKEY)
  {}

  LCD_Clear(LCD_COLOR_WHITE);
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);

  LCD_ClearLine(LCD_LINE_0);
  LCD_DisplayStringLine(LCD_LINE_1, " Press SEL to start ");
  LCD_ClearLine(LCD_LINE_2);
  LCD_ClearLine(LCD_LINE_3);
  LCD_DisplayStringLine(LCD_LINE_4, " When presentation  ");
  LCD_DisplayStringLine(LCD_LINE_5, "starts use RIGHT and");
  LCD_DisplayStringLine(LCD_LINE_6, "LEFT to go to the   ");
  LCD_DisplayStringLine(LCD_LINE_7, "next/previous slide ");
  LCD_DisplayStringLine(LCD_LINE_8, "and SEL to exit     ");
  LCD_ClearLine(LCD_LINE_9);
  while (Menu_ReadKey() != SEL)
  {}
  while (Menu_ReadKey() != NOKEY)
  {}
  /* Enable the SysTick Counter */
  SysTick->CTRL |= SysTick_CTRL_ENABLE;
  /* Start Wave audio playing */
  if (WavePlayer_Start() ==0)
  {
  while (index < 14)
  {
  
    WavePlayer_Pause();
    LCD_SetDisplayWindow(239, 0x13F, 240, 320);
    /* Slide nÝ: index */
    Storage_OpenReadFile("STFILES", (uint8_t *)SlideAddr[index], (uint32_t)&Wavebuffer, &bmplen);
    WavePlayer_RePlay();

    /* Clear the SysTick Counter */
    SysTick->VAL = (uint32_t)0x0;

    KeyState = Demo_DelayJoyStick(DelayTiming[index]);
    if (KeyState == LEFT)
    {
      if (index == 0)
      {
        index = 1;
      }
      index--;
      WavePointerUpdate(SlidesSpeech[index]);
    }
    else if (KeyState == RIGHT)
    {
      if (index != 13)
      {
        index++;
        WavePointerUpdate(SlidesSpeech[index]);
      }
    else
    {
      WavePlayer_Stop();
            index = 15;
    }

    }
    else if (KeyState == SEL)
    {
      WavePlayer_Stop();
      index = 15;
    }
    else if (KeyState == NOKEY)
    {
      index++;
    }
  }
  }
  /* Disable the SysTick Counter */
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
  /* Clear the SysTick Counter */
  SysTick->VAL = (uint32_t)0x0;

  LCD_Clear(LCD_COLOR_BLACK);
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLACK);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_0, "  End of slide show ");
  LCD_DisplayStringLine(LCD_LINE_1, "Exit:  Push JoyStick");
  while (Menu_ReadKey() == NOKEY)
  {}
  LCD_Clear(LCD_COLOR_WHITE);
  Menu_DisplayMenu();
  Demo_IntExtOnOffConfig(ENABLE);
}

/**
  * @brief  Checks if the bitmapfiles (slides + icons) are already loaded in the 
  *         SD Card FLASH.
  * @param  None
  * @retval None
  */
uint32_t Menu_CheckBitmapFiles(void)
{
  uint32_t err = 0;
  uint8_t pbuffer[512];
  FILEINFO fi;
  uint32_t i = 0x100;
  uint8_t j = 0;

  LCD_Clear(LCD_COLOR_WHITE);
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);

  if (SD_Detect() == SD_NOT_PRESENT)
  {
    LCD_DisplayStringLine(LCD_LINE_1, "Please insert SDCard");
    while (SD_Detect() == SD_NOT_PRESENT)
    {}

    LCD_DisplayStringLine(LCD_LINE_1, "   SDCard detected  ");
    Demo_Delay(200);
  }

  /*------------------- FAT Initialization -------------------------------*/
  do
  {
    err = Storage_Init();                         /** SDCARD Initialisation for DosFs                            */
  }
  while ((err != 0) && (i-- != 0));
  
  /* SD Card not formatted */
  if(i == 0)
  {
    LCD_DisplayStringLine(LCD_LINE_1, "SDCard not formatted");
    LCD_DisplayStringLine(LCD_LINE_1, "Reprogram your card.");
    while (1)
    {}  
  }

  if (DFS_OpenDir(&vi, "", &di))
  {
    return 1;
  }
  if (DFS_OpenDir(&vi, "STFILES", &di))
  {
    return 2;
  }
  /* Check icons if available on SD card under STFILES directory*/
  for (j = 0; j < 10; j++)
  {
    if (DFS_OpenFile(&vi, (uint8_t *)IconsAddr[j], DFS_READ, sector, &fi))
    {
      return 3;
    }

    DFS_ReadFile(&fi, sector, pbuffer, &i, SECTOR_SIZE);
    if (Menu_Buffercmp((uint8_t *)Icons64Check, pbuffer, 6) != 0)
    {
      return 4;
    }
  }
  /* Check Slides if available on SD card under STFILES directory*/
  for (j = 0; j < 14; j++)
  {
    if (DFS_OpenFile(&vi, (uint8_t *)SlideAddr[j], DFS_READ, sector, &fi))
    {
      return 3;
    }

    DFS_ReadFile(&fi, sector, pbuffer, &i, SECTOR_SIZE);
    if (Menu_Buffercmp((uint8_t *)SlidesCheck, pbuffer, 6) != 0)
    {
      return 4;
    }
  }
  /* Check Alarm and Watch images if available on SD card under STFILES directory*/
  for (j = 0;j < 2;j++)
  {
    if (DFS_OpenFile(&vi, (uint8_t *)Images2Addr[j], DFS_READ, sector, &fi))
    {
      return 3;
    }

    DFS_ReadFile(&fi, sector, pbuffer, &i, SECTOR_SIZE);
    if (Menu_Buffercmp((uint8_t *)Icons128Check, pbuffer, 6) != 0)
    {
      return 4;
    }
  }

  /* Check HELP and STLOGO images if available on SD card under STFILES directory*/
  for (j = 0; j < 2; j++)
  {
    if (DFS_OpenFile(&vi, (uint8_t *)Images1Addr[j], DFS_READ, sector, &fi))
    {
      return 3;
    }

    DFS_ReadFile(&fi, sector, pbuffer, &i, SECTOR_SIZE);
    if (Menu_Buffercmp((uint8_t *)SlidesCheck, pbuffer, 6) != 0)
    {
      return 4;
    }
  }

  return 0;
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length.
  * @retval  0: pBuffer1 identical to pBuffer2
  *          1: pBuffer1 differs from pBuffer2
  */
uint8_t Menu_Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  Displays the temperature in Celsius and fahrenheit degree.
  * @param  None
  * @retval None
  */
void Menu_ThermometerFun(void)
{
  uint32_t i = 0, pressedkey = 0;

  while (Menu_ReadKey() != NOKEY)
  {}

  /* Initialize the Temperature Sensor */
  LM75_Init();

  if (LM75_GetStatus() == SUCCESS)
  {
    /* Disable the JoyStick Interrupts */
    Demo_IntExtOnOffConfig(DISABLE);
    /* Clear the LCD */
    LCD_Clear(LCD_COLOR_WHITE);

    /* Set the Icon display window */
    LCD_SetDisplayWindow(170, 190, 64, 64);

    /* Display the Thermometer icon */
    Storage_OpenReadFile("STFILES", "STFILES/ICON7.BMP", (uint32_t)&Wavebuffer, &bmplen);
    /* Disable LCD Window mode */
    LCD_WindowModeDisable();
    /* Set the Back Color */
    LCD_SetBackColor(LCD_COLOR_BLUE);
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_GREEN);
    LCD_DisplayStringLine(LCD_LINE_0, "     Temperature    ");

    /* Wait until no key is pressed */
    while (Menu_ReadKey() != NOKEY)
    {}

    I2C_ClearITPendingBit(I2C1, I2C_IT_SMBALERT);
    SMbusAlertOccurred = 0;

    /* Wait until a key is beiing pressed */
    while (pressedkey == NOKEY)
    {
      /* Initialize the Temperature Sensor */
      LM75_Init();

      /* Configure the Temperature sensor device STLM75:
           - Thermostat mode Interrupt
           - Fault tolerance: 00
         */
      LM75_WriteConfReg(0x02);

      /* Configure the THYS and TOS inorder to use the SMbus alert interrupt */
      LM75_WriteReg(LM75_REG_THYS, TEMPERATURE_THYS << 8);  /*31ÝC*/
      LM75_WriteReg(LM75_REG_TOS, TEMPERATURE_TOS << 8);   /*32ÝC*/

      /* Get double of Temperature value */
      TempValue = LM75_ReadTemp();
      if (TempValue <= 256)
      {
        /* Positive temperature measured */
        TempCelsiusDisplay[7] = '+';
        /* Initialize the temperature sensor value*/
        TempValueCelsius = TempValue;
      }
      else
      {
        /* Negative temperature measured */
        TempCelsiusDisplay[7] = '-';
        /* Remove temperature value sign */
        TempValueCelsius = 0x200 - TempValue;
      }

      /* Calculate temperature digits in ÝC */
      if ((TempValueCelsius & 0x01) == 0x01)
      {
        TempCelsiusDisplay[12] = 0x05 + 0x30;
        TempFahrenheitDisplay[12] = 0x05 + 0x30;
      }
      else
      {
        TempCelsiusDisplay[12] = 0x00 + 0x30;
        TempFahrenheitDisplay[12] = 0x00 + 0x30;
      }

      TempValueCelsius >>= 1;

      TempCelsiusDisplay[8] = (TempValueCelsius / 100) + 0x30;
      TempCelsiusDisplay[9] = ((TempValueCelsius % 100) / 10) + 0x30;
      TempCelsiusDisplay[10] = ((TempValueCelsius % 100) % 10) + 0x30;

      if (TempValue > 256)
      {
        if (((9 * TempValueCelsius) / 5) <= 32)
        {
          /* Convert temperature ÝC to Fahrenheit */
          TempValueFahrenheit = abs (32 - ((9 * TempValueCelsius) / 5));

          /* Calculate temperature digits in ÝF */
          TempFahrenheitDisplay[8] = (TempValueFahrenheit / 100) + 0x30;
          TempFahrenheitDisplay[9] = ((TempValueFahrenheit % 100) / 10) + 0x30;
          TempFahrenheitDisplay[10] = ((TempValueFahrenheit % 100) % 10) + 0x30;
          /* Positive temperature measured */
          TempFahrenheitDisplay[7] = '+';
        }
        else
        {
          /* Convert temperature ÝC to Fahrenheit */
          TempValueFahrenheit = abs(((9 * TempValueCelsius) / 5) - 32);
          /* Calculate temperature digits in ÝF */
          TempFahrenheitDisplay[8] = (TempValueFahrenheit / 100) + 0x30;
          TempFahrenheitDisplay[9] = ((TempValueFahrenheit % 100) / 10) + 0x30;
          TempFahrenheitDisplay[10] = ((TempValueFahrenheit % 100) % 10) + 0x30;

          /* Negative temperature measured */
          TempFahrenheitDisplay[7] = '-';
        }
      }
      else
      {
        /* Convert temperature ÝC to Fahrenheit */
        TempValueFahrenheit = ((9 * TempValueCelsius) / 5) + 32;

        /* Calculate temperature digits in ÝF */
        TempFahrenheitDisplay[8] = (TempValueFahrenheit / 100) + 0x30;
        TempFahrenheitDisplay[9] = ((TempValueFahrenheit % 100) / 10) + 0x30;
        TempFahrenheitDisplay[10] = ((TempValueFahrenheit % 100) % 10) + 0x30;

        /* Positive temperature measured */
        TempFahrenheitDisplay[7] = '+';
      }

      /* Display Fahrenheit value on LCD */
      for (i = 0; i < 20; i++)
      {
        LCD_DisplayChar(LCD_LINE_7, (319 - (16 * i)), TempCelsiusDisplay[i]);

        LCD_DisplayChar(LCD_LINE_8, (319 - (16 * i)), TempFahrenheitDisplay[i]);
      }

      if (SMbusAlertOccurred == 1)
      {
        /* Set the Back Color */
        LCD_SetBackColor(LCD_COLOR_BLUE);
        /* Set the Text Color */
        LCD_SetTextColor(LCD_COLOR_RED);
        LCD_DisplayStringLine(LCD_LINE_2, "Warning: Temp exceed");
        LCD_DisplayStringLine(LCD_LINE_3, "        32 C        ");
      }
      if (SMbusAlertOccurred == 2)
      {
        /* Set the Back Color */
        LCD_SetBackColor(LCD_COLOR_WHITE);
        /* Set the Text Color */
        LCD_SetTextColor(LCD_COLOR_WHITE);
        LCD_ClearLine(LCD_LINE_2);
        LCD_ClearLine(LCD_LINE_3);
        SMbusAlertOccurred = 0;
        /* Set the Back Color */
        LCD_SetBackColor(LCD_COLOR_BLUE);
        /* Set the Text Color */
        LCD_SetTextColor(LCD_COLOR_GREEN);
      }
      pressedkey = Menu_ReadKey();
    }
  }
  else
  {
    LCD_Clear(LCD_COLOR_WHITE);
    LCD_DisplayStringLine(LCD_LINE_2, " No TSENSOR Present ");
    LCD_DisplayStringLine(LCD_LINE_3, "Exit:  Push JoyStick");
    while (Menu_ReadKey() == NOKEY)
    {}
  }
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Display menu */
  Menu_DisplayMenu();

  /* Enable the JoyStick interrupt */
  Demo_IntExtOnOffConfig(ENABLE);
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
