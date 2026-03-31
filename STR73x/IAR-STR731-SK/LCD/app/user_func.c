/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : user_func.c
 *    Description : User function module
 *
 *    History :
 *    1. Date        : Nov. 2, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#include "user_func.h"

#define SELECT_CHAR                   0x8
#define DEGREE_CHAR                   0x1

#define BUTT_LEFT_PIN                 !GPIO_BitRead(GPIO5,GPIO_PIN_8)
#define BUTT_RIGHT_PIN                !GPIO_BitRead(GPIO5,GPIO_PIN_9)
#define BUTT_SELECT_PIN               !GPIO_BitRead(GPIO5,GPIO_PIN_12)
#define BUTT_CANCEL_PIN               !GPIO_BitRead(GPIO6,GPIO_PIN_6)

#define LED_ALARM_MAX_PIN(Val)        GPIO_BitWrite(GPIO0,GPIO_PIN_0,Val?Bit_SET:Bit_RESET)
#define LED_ALARM_MIN_PIN(Val)        GPIO_BitWrite(GPIO0,GPIO_PIN_1,Val?Bit_SET:Bit_RESET)
#define LED_WARNING_MAX_PIN(Val)      GPIO_BitWrite(GPIO0,GPIO_PIN_2,Val?Bit_SET:Bit_RESET)
#define LED_WARNING_MIN_PIN(Val)      GPIO_BitWrite(GPIO0,GPIO_PIN_3,Val?Bit_SET:Bit_RESET)

#define SCREEN_SAVER_RIGHT            TRUE
#define SCREEN_SAVER_LEFT             FALSE

tButtonObj Bottons[4] =
{
  {0,BUTT_RELEASE},
  {0,BUTT_RELEASE},
  {0,BUTT_RELEASE},
  {0,BUTT_RELEASE},
};

Int32U LighToCounter;
Int8U CursorPosHold;
tLightMode LightMode;
tTemperatureUnits TempUnit = CELSIUS;
Flo32 Temperature = 0;
Flo32 AlarmMaxTemp, AlarmMinTemp, WarningMaxTemp, WarningMinTemp;
Boolean AlarmMode, WarningMode, Direction;

Flo32 K = 1.0;

const char * const IlluminationModeStr[] =
{
  "Off\x1b\x2",
  "Auto\x1b\x2",
  "On\x1b\x2",
};

const char * const ActionStateStr[] =
{
  "",
  "Alarm min",
  "Alarm max",
  "Warning min",
  "Warning max",
};

const MEMU_STING_DEF * const ScreenSaverTable [] =
{
  "  IAR Embedded  \x1b\2 Workbench  ARM \x1b\2",
  " STMicroelectr. \x1b\2    STR731F     \x1b\2",
};

/*************************************************************************
 * Function Name: ScreenSaver
 * Parameters: MEMU_STING_DEF ** ppVarArg,
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: Initialize menu screensaver
 *
 *************************************************************************/
void ScreenSaver (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                  MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
  Direction ^= TRUE;
  strcpy(*ppVarArg,ScreenSaverTable[Direction?1:0]);
  *AnimationType = MEMU_ANIMATION_RIGHT;
}

/*************************************************************************
 * Function Name: FindOffSet
 * Parameters: const char* Str,Int8U  Size
 *
 * Return: Int8U
 *
 * Description: Return needed offset to show string in the middle of the LCD
 *
 *************************************************************************/
Int8U FindOffSet(const char* Str,Int8U  Size)
{
  if(strlen(Str) < Size-1)
  {
    Size -= strlen(Str);
    return (Size>>1);
  }
  return 0;
}
/*************************************************************************
 * Function Name: ReplaceStr
 * Parameters: char* DestStr,const char* SourStr,Int8U  Offset,Int8U  Size
 *
 * Return: none
 * Description: Replace "Size" number of characters into *DestStr from *SourStr
 *
 *************************************************************************/
void ReplaceStr(char* DestStr,const char* SourStr,Int8U  Offset,Int8U  Size)
{
  if((SourStr == NULL) || (DestStr == NULL)
    || (Size == 0) || (strlen(DestStr) <= Offset))
  {
    return;
  }
  DestStr += Offset;
  while(*SourStr)
  {
    *DestStr = *SourStr;
    ++SourStr;++DestStr;
    if(--Size == 0)
    {
      break;
    }
  }
}

/*************************************************************************
 * Function Name: ButtonsInit
 * Parameters: none
 * Return: none
 * Description: Init buttons
 *
 *************************************************************************/
void ButtonsInit (void)
{
GPIO_InitTypeDef GPIO5_InitStructure;
GPIO_InitTypeDef GPIO6_InitStructure;
  /* Init buttons Inputs */
  /* GPIO Initialization */
  /* GPIO5/GPIO6 Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_GPIO5 , ENABLE);
  CFG_PeripheralClockConfig(CFG_CLK_GPIO6 , ENABLE);
  /* GPIO5/GPIO6 Configuration */
  GPIO5_InitStructure.GPIO_Mode = GPIO_Mode_IN_TRI_TTL;
  GPIO5_InitStructure.GPIO_Pins = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_12;
  GPIO_Init (GPIO5, &GPIO5_InitStructure);

  GPIO6_InitStructure.GPIO_Mode = GPIO_Mode_IN_TRI_TTL;
  GPIO6_InitStructure.GPIO_Pins = GPIO_PIN_6;
  GPIO_Init (GPIO6, &GPIO6_InitStructure);
}

/*************************************************************************
 * Function Name: ButtonFilt
 * Parameters: Boolean Press, tButton ButInd
 * Return: tButtonState
 *
 * Description: Button filter
 *
 *************************************************************************/
tButtonState ButtonFilt (Boolean Press,tButton ButInd)
{
tButtonObj * CurrentButton = &Bottons[ButInd];
static Int32U Count;
  if (Press)
  {
    if(++CurrentButton->ButtCount > BUTT_HOLD_LONG_TO)
    {
      if(CurrentButton->ButtState < BUTT_HOLD_LONG)
      {
        CurrentButton->ButtState = BUTT_HOLD_LONG;
        Count = BUTT_HOLD_LONG_TO + BUTT_REPEAT_TO;
        return(BUTT_HOLD_LONG);
      }
      else if (Count <= CurrentButton->ButtCount)
      {
        CurrentButton->ButtCount = BUTT_HOLD_LONG_TO;
        CurrentButton->ButtState = BUTT_HOLD_LONG;
        return(BUTT_HOLD_LONG);
      }

    }
    else if (CurrentButton->ButtCount > BUTT_HOLD_TO)
    {
      if(CurrentButton->ButtState < BUTT_HOLD)
      {
        CurrentButton->ButtState = BUTT_HOLD;
        Count = CurrentButton->ButtCount + BUTT_REPEAT_TO;
        return(BUTT_HOLD);
      }
      else if (Count <= CurrentButton->ButtCount)
      {
        Count = CurrentButton->ButtCount + BUTT_REPEAT_TO;
        CurrentButton->ButtState = BUTT_HOLD;
        return(BUTT_HOLD);
      }
    }
    else if (CurrentButton->ButtCount > BUTT_DEB)
    {
      if(CurrentButton->ButtState < BUTT_PRESS)
      {
        CurrentButton->ButtState = BUTT_PRESS;
        return BUTT_PRESS;
      }
    }
  }
  else
  {
    if(CurrentButton->ButtCount > BUTT_DEB)
    {
      CurrentButton->ButtCount = BUTT_DEB;
    }
    if(CurrentButton->ButtCount)
    {
      if(--CurrentButton->ButtCount == 0)
      {
        CurrentButton->ButtState = BUTT_RELEASE;
        return BUTT_RELEASE;
      }
    }
  }
  return BUTT_STATE_NO_CHANGE;
}

/*************************************************************************
 * Function Name: Buttons
 * Parameters: none
 * Return: Boolean
 * Description: Imlement buttons
 *
 *************************************************************************/
Boolean Buttons (void)
{
Boolean ButtFlag = FALSE;
  // Button 1 (Left)
  switch(ButtonFilt(!BUTT_LEFT_PIN,ButtonLeft))
  {
  case BUTT_PRESS:
    MenuSetEvent(MENU_EVENT_B_LEFT_PRESS);
    ButtFlag = TRUE;
    break;
  case BUTT_HOLD:
    MenuSetEvent(MENU_EVENT_B_LEFT_HOLD);
    ButtFlag = TRUE;
    break;
  case BUTT_HOLD_LONG:
    MenuSetEvent(MENU_EVENT_B_LEFT_HOLD_LONG);
    ButtFlag = TRUE;
    break;
  }

  // Button 2 (Right)
  switch(ButtonFilt(!BUTT_RIGHT_PIN,ButtonRigth))
  {
  case BUTT_PRESS:
    MenuSetEvent(MENU_EVENT_B_RIGHT_PRESS);
    ButtFlag = TRUE;
    break;
  case BUTT_HOLD:
    MenuSetEvent(MENU_EVENT_B_RIGHT_HOLD);
    ButtFlag = TRUE;
    break;
  case BUTT_HOLD_LONG:
    MenuSetEvent(MENU_EVENT_B_RIGHT_HOLD_LONG);
    ButtFlag = TRUE;
    break;
  }

  // Button 3 (Select)
  if(ButtonFilt(!BUTT_SELECT_PIN,ButtonSelect) == BUTT_PRESS)
  {
    MenuSetEvent(MENU_EVENT_B_SELECT_PRESS);
    ButtFlag = TRUE;
  }

  // Button 4 (Cancel)
  if(ButtonFilt(!BUTT_CANCEL_PIN,ButtonCancel) == BUTT_PRESS)
  {
    MenuSetEvent(MENU_EVENT_B_CANCEL_PRESS);
    ButtFlag = TRUE;
  }
  return ButtFlag;
}

/*************************************************************************
 * Function Name: LightInit
 * Parameters: none
 * Return: none
 * Description: Init backlight
 *
 *************************************************************************/
void LightInit(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
  /* Init Backlight control pin to output */
  /* GPIO Initialization */
  /* GPIO6 Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_GPIO6 , ENABLE);
  /* GPIO6 Configuration */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
  GPIO_InitStructure.GPIO_Pins = GPIO_PIN_0;
  GPIO_Init (GPIO6, &GPIO_InitStructure);
  LightMode = LightModeAuto;
  LighToCounter = 0;
}

/*************************************************************************
 * Function Name: LightCntr
 * Parameters: Boolean On
 *
 * Return: none
 * Description: Backlight control
 *
 *************************************************************************/
void LightCntr (Boolean On)
{
  GPIO_BitWrite(GPIO6,GPIO_PIN_0,On?Bit_SET:Bit_RESET);
}

/*************************************************************************
 * Function Name: IlluminationShow
 * Parameters: MEMU_STING_DEF ** ppVarArg,
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: Illumination show
 *
 *************************************************************************/
void IlluminationShow (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                       MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
tLightMode Mode = GetLightMode();
  if(**ppVarArg != ' ')
  {
    if(Mode+1 > LightModeOn)
    {
      Mode = LightModeOff;
    }
    else
    {
      ++Mode;
    }
    SetLightMode(Mode);
  }
  strcpy(*ppVarArg,IlluminationModeStr[GetLightMode()]);
}

/*************************************************************************
 * Function Name: CursorOff
 * Parameters: MEMU_STING_DEF ** ppVarArg
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: Cursor off
 *
 *************************************************************************/
void CursorOff (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
  HD44780_CursorPosSet(HD44780_CURSOR_OFF, HD44780_CURSOR_NORM,1, 1);
}

/*************************************************************************
 * Function Name: TempShow
 * Parameters: MEMU_STING_DEF ** ppVarArg,
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: Temperature and action state show
 *
 *************************************************************************/
void TempShow (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
               MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
MEMU_STING_DEF Str[16];
tTemperatureUnits CurrentTempUnit = GetTempUnit();
Int8U i,j;

  sprintf(Str,"%4.1f",TempConverter(Temperature,KELVIN,CurrentTempUnit));
  switch(CurrentTempUnit)
  {
  case CELSIUS:
    strcat(Str,"\x1\x43");
    break;
  case FAHRENHEIT:
    strcat(Str,"\x1\x46");
    break;
  default:
    strcat(Str,"\x1\x4B");
    break;
  }
  j = FindOffSet(Str,16);
  for(i = 0; i < j ; ++i)
  {
    *(*ppVarArg+i) = ' ';
  }
  *(*ppVarArg+i) = 0;
  strcat(Str,"\x1b\2");
  strcat(*ppVarArg,Str);

  j = FindOffSet(ActionStateStr[GetActionState()],16);
  Str[j] = 0;
  for(i = 0; i < j; ++i)
  {
    Str[i] = ' ';
  }
  strcat(*ppVarArg,Str);
  strcat(*ppVarArg,ActionStateStr[GetActionState()]);
  strcat(*ppVarArg,"\x1b\x2");
}


/*************************************************************************
 * Function Name: GetTempUnit
 * Parameters: none
 *
 * Return: enum
 * Description: Return current temperature units
 *
 *************************************************************************/
tTemperatureUnits GetTempUnit (void)
{
  return(TempUnit);
}

/*************************************************************************
 * Function Name: SetTempUnit
 * Parameters: tTemperatureUnits Unit
 *
 * Return: none
 *
 * Description: Set current temperature units
 *
 *************************************************************************/
void SetTempUnit (tTemperatureUnits Unit)
{
  TempUnit = Unit;
}

/*************************************************************************
 * Function Name: TempUnitShow
 * Parameters: MEMU_STING_DEF * pVarArg,
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: Change Temperature Units and Show
 *
 *************************************************************************/
void TempUnitShow(MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                  MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
tTemperatureUnits CurrUnit = GetTempUnit();
  if(**ppVarArg != 0x20)
  {
    if(++CurrUnit > FAHRENHEIT)
    {
      CurrUnit = KELVIN;
    }
    SetTempUnit(CurrUnit);
    ++*ppVarArg;
  }

  switch(CurrUnit)
  {
  case CELSIUS:
    *(*ppVarArg+5)  = SELECT_CHAR;
    break;
  case FAHRENHEIT:
    *(*ppVarArg+10) = SELECT_CHAR;
    break;
  default:
    *(*ppVarArg)  = SELECT_CHAR;
    break;
  }
}
/*************************************************************************
 * Function Name: SetLightMode
 * Parameters: tTemperatureUnits Unit
 *
 * Return: none
 *
 * Description: Set current illumination mode
 *
 *************************************************************************/
void SetLightMode(tLightMode Mode)
{
  LightMode = Mode;
}

/*************************************************************************
 * Function Name: GetLightMode
 * Parameters: none
 *
 * Return: tLightMode
 *
 * Description: Return current illumination mode
 *
 *************************************************************************/
tLightMode GetLightMode (void)
{
  return(LightMode);
}

/*************************************************************************
 * Function Name: AnimationEnaShow
 * Parameters: MEMU_STING_DEF ** ppVarArg,
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: Animation enable show
 *
 *************************************************************************/
void AnimationEnaShow (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                       MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
Boolean AnimFlag = MenuGetAnimMode();
  if(**ppVarArg != ' ')
  {
    **ppVarArg = ' ';
    AnimFlag ^= TRUE;
    MenuSetAnimMode(AnimFlag);
  }
  if(AnimFlag)
  {
    **ppVarArg = SELECT_CHAR;
  }
}

/*************************************************************************
 * Function Name: AnimationSpeed
 * Parameters: MEMU_STING_DEF ** ppVarArg,
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: Animation speed
 *
 *************************************************************************/
void AnimationSpeed (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                     MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
Int32U Speed = MenuGetAnimSpeed();
  if(Speed >= 9)
  {
    Speed = 3;
  }
  else
  {
    Speed += 3;
  }
  MenuSetAnimSpeed(Speed);
}

/*************************************************************************
 * Function Name: AnimationSpeedEnaShow
 * Parameters: MEMU_STING_DEF ** ppVarArg,
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: Animation speed menu enable
 *
 *************************************************************************/
void AnimationSpeedEnaShow (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                            MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
  if(MenuGetAnimMode())
  {
    *MenuIndex = MENU_ANIM_SPEED_SHOW;
  }
  else
  {
    if(** ppVarArg == '1')
    {
      strcpy(*ppVarArg," \x1B\2");
      *MenuIndex = MENU_ANIM_ENABLE_SHOW;
      AnimationEnaShow(ppVarArg,MenuIndex,MenuTO,AnimationType);
    }
    else
    {
      **ppVarArg = ' ';
      *MenuIndex = MENU_LIGHT_SETTINGS_SHOW;
      IlluminationShow(ppVarArg,MenuIndex,MenuTO,AnimationType);
    }
  }
}

/*************************************************************************
 * Function Name: CalTermShow
 * Parameters: MEMU_STING_DEF ** ppVarArg,
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: NTC Calibration show
 *
 *************************************************************************/
void CalTermShow (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                  MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
tTemperatureUnits CurrentTempUnit = GetTempUnit();
static Flo32 BaseTemp,TempHold,MinTemp,MaxTemp;
  if(**ppVarArg == ' ')
  {
    // Calibrate Sensor
    BaseTemp = TempConverter(BaseTemp,CurrentTempUnit,KELVIN);
    TempHold = TempConverter(TempHold,CurrentTempUnit,KELVIN);
    SetCalX(NtcTemperatureToRatio(BaseTemp)/NtcTemperatureToRatio(TempHold));
    return;
  }
  else if (**ppVarArg == '>')
  {
    BaseTemp += 0.1;
    if(BaseTemp > MaxTemp)
    {
      BaseTemp = MaxTemp;
    }
  }
  else if (**ppVarArg == '<')
  {
    BaseTemp -= 0.1;
    if(BaseTemp < MinTemp)
    {
      BaseTemp = MinTemp;
    }
  }
  else if (**ppVarArg == 'I')
  {
    BaseTemp += 1;
    if(BaseTemp > MaxTemp)
    {
      BaseTemp = MaxTemp;
    }
  }
  else if (**ppVarArg == 'D')
  {
    BaseTemp -= 1;
    if(BaseTemp < MinTemp)
    {
      BaseTemp = MinTemp;
    }
  }
  else
  {
    BaseTemp = NtcRatioToTemperature((Flo32)MeasureTerm()/1024);
    TempHold = BaseTemp = TempConverter(BaseTemp,KELVIN,CurrentTempUnit);
    MinTemp  = BaseTemp*0.9;
    MaxTemp  = BaseTemp*1.1;
  }
  sprintf(*ppVarArg,"%4.1f",BaseTemp);
  CursorPosHold = strlen(*ppVarArg)+3;
  switch(CurrentTempUnit)
  {
  case CELSIUS:
    strcat(*ppVarArg,"\x1\x43\x1b\2");
    break;
  case FAHRENHEIT:
    strcat(*ppVarArg,"\x1\x46\x1b\2");
    break;
  default:
    strcat(*ppVarArg,"\x1\x4B\x1b\2");
    break;
  }
}

/*************************************************************************
 * Function Name: CursorOn
 * Parameters: MEMU_STING_DEF ** ppVarArg,
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: Cursor On
 *
 *************************************************************************/
void CursorOn (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
               MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
  HD44780_CursorPosSet(HD44780_CURSOR_OFF, HD44780_CURSOR_BLINK,CursorPosHold, 2);
}

/*************************************************************************
 * Function Name: AlarmMin
 * Parameters: MEMU_STING_DEF ** ppVarArg,
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: Set Alarm min parameter
 *
 *************************************************************************/
void AlarmMin (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
               MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
tTemperatureUnits CurrentTempUnit = GetTempUnit();
static Flo32 BaseTemp,MinTemp,MaxTemp;
  if(**ppVarArg == ' ')
  {
    AlarmMinTemp = TempConverter(BaseTemp,CurrentTempUnit,KELVIN);
  }
  else if (**ppVarArg == '>')
  {
    BaseTemp += 0.1;
    if(BaseTemp > MaxTemp)
    {
      BaseTemp = MaxTemp;
    }
  }
  else if (**ppVarArg == '<')
  {
    BaseTemp -= 0.1;
    if(BaseTemp < MinTemp)
    {
      BaseTemp = MinTemp;
    }
  }
  else if (**ppVarArg == 'I')
  {
    BaseTemp += 1;
    if(BaseTemp > MaxTemp)
    {
      BaseTemp = MaxTemp;
    }
  }
  else if (**ppVarArg == 'D')
  {
    BaseTemp -= 1;
    if(BaseTemp < MinTemp)
    {
      BaseTemp = MinTemp;
    }
  }
  else
  {
    BaseTemp = TempConverter(AlarmMinTemp,KELVIN,CurrentTempUnit);
    MinTemp  = TempConverter(MIN_TEMP,KELVIN,CurrentTempUnit);
    MaxTemp  = TempConverter(AlarmMaxTemp,KELVIN,CurrentTempUnit);
  }
  sprintf(*ppVarArg,"%4.1f",BaseTemp);
  CursorPosHold = strlen(*ppVarArg)+3;
  switch(CurrentTempUnit)
  {
  case CELSIUS:
    strcat(*ppVarArg,"\x1\x43\x1b\2");
    break;
  case FAHRENHEIT:
    strcat(*ppVarArg,"\x1\x46\x1b\2");
    break;
  default:
    strcat(*ppVarArg,"\x1\x4B\x1b\2");
    break;
  }
}

/*************************************************************************
 * Function Name: AlarmMax
 * Parameters: MEMU_STING_DEF ** ppVarArg,
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: Set Alarm max parameter
 *
 *************************************************************************/
void AlarmMax (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
               MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
tTemperatureUnits CurrentTempUnit = GetTempUnit();
static Flo32 BaseTemp,MinTemp,MaxTemp;
  if(**ppVarArg == ' ')
  {
    AlarmMaxTemp = TempConverter(BaseTemp,CurrentTempUnit,KELVIN);
  }
  else if (**ppVarArg == '>')
  {
    BaseTemp += 0.1;
    if(BaseTemp > MaxTemp)
    {
      BaseTemp = MaxTemp;
    }
  }
  else if (**ppVarArg == '<')
  {
    BaseTemp -= 0.1;
    if(BaseTemp < MinTemp)
    {
      BaseTemp = MinTemp;
    }
  }
  else if (**ppVarArg == 'I')
  {
    BaseTemp += 1;
    if(BaseTemp > MaxTemp)
    {
      BaseTemp = MaxTemp;
    }
  }
  else if (**ppVarArg == 'D')
  {
    BaseTemp -= 1;
    if(BaseTemp < MinTemp)
    {
      BaseTemp = MinTemp;
    }
  }
  else
  {
    BaseTemp = TempConverter(AlarmMaxTemp,KELVIN,CurrentTempUnit);
    MinTemp  = TempConverter(AlarmMinTemp,KELVIN,CurrentTempUnit);
    MaxTemp  = TempConverter(MAX_TEMP,KELVIN,CurrentTempUnit);
  }
  sprintf(*ppVarArg,"%4.1f",BaseTemp);
  CursorPosHold = strlen(*ppVarArg)+3;
  switch(CurrentTempUnit)
  {
  case CELSIUS:
    strcat(*ppVarArg,"\x1\x43\x1b\2");
    break;
  case FAHRENHEIT:
    strcat(*ppVarArg,"\x1\x46\x1b\2");
    break;
  default:
    strcat(*ppVarArg,"\x1\x4B\x1b\2");
    break;
  }
}

/*************************************************************************
 * Function Name: WarningMin
 * Parameters: MEMU_STING_DEF ** ppVarArg,
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: Set Warning min parameter
 *
 *************************************************************************/
void WarningMin(MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
tTemperatureUnits CurrentTempUnit = GetTempUnit();
static Flo32 BaseTemp,MinTemp,MaxTemp;
  if(**ppVarArg == ' ')
  {
    WarningMinTemp = TempConverter(BaseTemp,CurrentTempUnit,KELVIN);
  }
  else if (**ppVarArg == '>')
  {
    BaseTemp += 0.1;
    if(BaseTemp > MaxTemp)
    {
      BaseTemp = MaxTemp;
    }
  }
  else if (**ppVarArg == '<')
  {
    BaseTemp -= 0.1;
    if(BaseTemp < MinTemp)
    {
      BaseTemp = MinTemp;
    }
  }
  else if (**ppVarArg == 'I')
  {
    BaseTemp += 1;
    if(BaseTemp > MaxTemp)
    {
      BaseTemp = MaxTemp;
    }
  }
  else if (**ppVarArg == 'D')
  {
    BaseTemp -= 1;
    if(BaseTemp < MinTemp)
    {
      BaseTemp = MinTemp;
    }
  }
  else
  {
    BaseTemp = TempConverter(WarningMinTemp,KELVIN,CurrentTempUnit);
    MinTemp  = TempConverter(MIN_TEMP,KELVIN,CurrentTempUnit);
    MaxTemp  = TempConverter(WarningMaxTemp,KELVIN,CurrentTempUnit);
  }
  sprintf(*ppVarArg,"%4.1f",BaseTemp);
  CursorPosHold = strlen(*ppVarArg)+3;
  switch(CurrentTempUnit)
  {
  case CELSIUS:
    strcat(*ppVarArg,"\x1\x43\x1b\2");
    break;
  case FAHRENHEIT:
    strcat(*ppVarArg,"\x1\x46\x1b\2");
    break;
  default:
    strcat(*ppVarArg,"\x1\x4B\x1b\2");
    break;
  }
}

/*************************************************************************
 * Function Name: WarningMax
 * Parameters: MEMU_STING_DEF ** ppVarArg,
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: Set Warning max parameter
 *
 *************************************************************************/
void WarningMax(MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
tTemperatureUnits CurrentTempUnit = GetTempUnit();
static Flo32 BaseTemp,MinTemp,MaxTemp;
  if(**ppVarArg == ' ')
  {
    WarningMaxTemp = TempConverter(BaseTemp,CurrentTempUnit,KELVIN);
  }
  else if (**ppVarArg == '>')
  {
    BaseTemp += 0.1;
    if(BaseTemp > MaxTemp)
    {
      BaseTemp = MaxTemp;
    }
  }
  else if (**ppVarArg == '<')
  {
    BaseTemp -= 0.1;
    if(BaseTemp < MinTemp)
    {
      BaseTemp = MinTemp;
    }
  }
  else if (**ppVarArg == 'I')
  {
    BaseTemp += 1;
    if(BaseTemp > MaxTemp)
    {
      BaseTemp = MaxTemp;
    }
  }
  else if (**ppVarArg == 'D')
  {
    BaseTemp -= 1;
    if(BaseTemp < MinTemp)
    {
      BaseTemp = MinTemp;
    }
  }
  else
  {
    BaseTemp = TempConverter(WarningMaxTemp,KELVIN,CurrentTempUnit);
    MinTemp  = TempConverter(WarningMinTemp,KELVIN,CurrentTempUnit);
    MaxTemp  = TempConverter(MAX_TEMP,KELVIN,CurrentTempUnit);
  }
  sprintf(*ppVarArg,"%4.1f",BaseTemp);
  CursorPosHold = strlen(*ppVarArg)+3;
  switch(CurrentTempUnit)
  {
  case CELSIUS:
    strcat(*ppVarArg,"\x1\x43\x1b\2");
    break;
  case FAHRENHEIT:
    strcat(*ppVarArg,"\x1\x46\x1b\2");
    break;
  default:
    strcat(*ppVarArg,"\x1\x4B\x1b\2");
    break;
  }
}

/*************************************************************************
 * Function Name: ResetLightTO
 * Parameters: none
 *
 * Return: none
 * Description: Reset Light Auto off time out
 *
 *************************************************************************/
void ResetLightTO (void)
{
  LighToCounter = LIGHT_AUTO_TO;
}

/*************************************************************************
 * Function Name: Light
 * Parameters: none
 *
 * Return: none
 * Description: Light control
 *
 *************************************************************************/
void Light (void)
{
  switch(GetLightMode())
  {
  case LightModeOff:
    LightCntr(FALSE);
    break;
  case LightModeOn:
    LightCntr(TRUE);
    break;
  default:
    if(LighToCounter)
    {
      --LighToCounter;
      LightCntr(TRUE);
    }
    else
    {
      LightCntr(FALSE);
    }
  }
}

/*************************************************************************
 * Function Name: AlarmEnaShow
 * Parameters: MEMU_STING_DEF ** ppVarArg,
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: Alarm enable show
 *
 *************************************************************************/
void AlarmEnaShow (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                   MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
Boolean Flag = GetAlarmMode();
  if(**ppVarArg != ' ')
  {
    **ppVarArg = ' ';
    Flag ^= TRUE;
    SetAlarmMode(Flag);
  }
  if(Flag)
  {
    **ppVarArg = SELECT_CHAR;
  }
}

/*************************************************************************
 * Function Name: AnimationEnaShow
 * Parameters: MEMU_STING_DEF ** ppVarArg,
 *             MEMU_IND_DEF * MenuIndex,
 *             MEMU_TIME_OUT_DEF * MenuTO
 *             MENU_ANIMATION_TYPE_DEF * AnimationType
 * Return: none
 * Description: Warning enable show
 *
 *************************************************************************/
void WarningEnaShow (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                     MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType)
{
Boolean Flag = GetWarningMode();
  if(**ppVarArg != ' ')
  {
    **ppVarArg = ' ';
    Flag ^= TRUE;
    SetWarningMode(Flag);
  }
  if(Flag)
  {
    **ppVarArg = SELECT_CHAR;
  }
}

/*************************************************************************
 * Function Name: SetAlarmMode
 * Parameters: Boolean Mode
 *
 * Return: none
 *
 * Description: Set current alarm mode
 *
 *************************************************************************/
void SetAlarmMode(Boolean Mode)
{
  AlarmMode = Mode;
}

/*************************************************************************
 * Function Name: GetAlarmMode
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Return current alarm mode
 *
 *************************************************************************/
Boolean GetAlarmMode (void)
{
  return(AlarmMode);
}

/*************************************************************************
 * Function Name: SetWarningMode
 * Parameters: Boolean Mode
 *
 * Return: none
 *
 * Description: Set current Warning mode
 *
 *************************************************************************/
void SetWarningMode(Boolean Mode)
{
  WarningMode = Mode;
}

/*************************************************************************
 * Function Name: GetWarningMode
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Return current Warning mode
 *
 *************************************************************************/
Boolean GetWarningMode (void)
{
  return(WarningMode);
}

/*************************************************************************
 * Function Name: GetActionState
 * Parameters: none
 *
 * Return: tActionState
 *
 * Description: Return current state of alarm and Warning
 *
 *************************************************************************/
tActionState GetActionState (void)
{
  if(GetAlarmMode())
  {
    if(Temperature > AlarmMaxTemp)
    {
      return ALARM_MAX;
    }
    if(Temperature < AlarmMinTemp)
    {
      return ALARM_MIN;
    }
  }
  if(GetWarningMode())
  {
    if(Temperature > WarningMaxTemp)
    {
      return WARNING_MAX;
    }
    if(Temperature < WarningMinTemp)
    {
      return WARNING_MIN;
    }
  }
  return OK;
}

/*************************************************************************
 * Function Name: InitAction
 * Parameters: Boolean AlarmStateL, Flo32 AlarmMinL, Flo32 AlarmMaxL,
 *             Boolean WarningStateL, Flo32 WarningMinL, Flo32 WarningMaxL
 *
 * Return: tActionState
 *
 * Description: Init Action
 *
 *************************************************************************/
void InitAction (Boolean AlarmStateL, Flo32 AlarmMinL, Flo32 AlarmMaxL,
                 Boolean WarningStateL, Flo32 WarningMinL, Flo32 WarningMaxL)
{
GPIO_InitTypeDef GPIO_InitStructure;
  AlarmMaxTemp   = AlarmMaxL;
  AlarmMinTemp   = AlarmMinL;
  SetAlarmMode(AlarmStateL);
  WarningMaxTemp = WarningMaxL;
  WarningMinTemp = WarningMinL;
  SetWarningMode(WarningStateL);
  /* LEDs init */
  /* GPIO Initialization */
  /* GPIO0 Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_GPIO0 , ENABLE);
  /* GPIO0 Configuration */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
  GPIO_InitStructure.GPIO_Pins = 0xf;
  GPIO_Init (GPIO0, &GPIO_InitStructure);
}

/*************************************************************************
 * Function Name: Action
 * Parameters: none
 *
 * Return: tActionState
 *
 * Description: Set outputs depending of alarm and warning states
 *
 *************************************************************************/
void Action (void)
{
  if(GetAlarmMode())
  {
    LED_ALARM_MAX_PIN(Temperature > AlarmMaxTemp);
    LED_ALARM_MIN_PIN(Temperature < AlarmMinTemp);
  }
  else
  {
    LED_ALARM_MAX_PIN(0);
    LED_ALARM_MIN_PIN(0);
  }
  if(GetWarningMode())
  {
    LED_WARNING_MAX_PIN(Temperature > WarningMaxTemp);
    LED_WARNING_MIN_PIN(Temperature < WarningMinTemp);
  }
  else
  {
    LED_WARNING_MAX_PIN(0);
    LED_WARNING_MIN_PIN(0);
  }
}

/*************************************************************************
 * Function Name: SetTemp
 * Parameters: Flo32 Temp
 *
 * Return: none
 *
 * Description: Set current temperature
 *
 *************************************************************************/
void SetTemp (Flo32 Temp)
{
  Temperature = Temp;
}

/*************************************************************************
 * Function Name: GetTemp
 * Parameters: none
 *
 * Return: Flo32
 *
 * Description: Get current temperature
 *
 *************************************************************************/
Flo32 GetTemp (void)
{
  return Temperature;
}

/*************************************************************************
 * Function Name: GetCalX
 * Parameters: none
 *
 * Return: Flo32
 *
 * Description: Get Calibration coefficient
 *
 *************************************************************************/
Flo32 GetCalX (void)
{
  return K;
}

/*************************************************************************
 * Function Name: SetCalX
 * Parameters: Flo32 K
 *
 * Return: none
 *
 * Description: Set calibration coefficient
 *
 *************************************************************************/
void SetCalX (Flo32 Coef)
{
  K = Coef;
}

