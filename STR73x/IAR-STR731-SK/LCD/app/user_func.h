/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : user_func.h
 *    Description : User functions include file
 *
 *    History :
 *    1. Date        : Nov. 2, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#ifndef  __USER_FUNC_H
#define  __USER_FUNC_H

#define MIN_TEMP                      223.15
#define MAX_TEMP                      453.15

typedef enum
{
  OK = 0,ALARM_MIN,ALARM_MAX,WARNING_MIN,WARNING_MAX
} tActionState;

typedef enum
{
  ButtonLeft = 0,ButtonRigth,ButtonSelect, ButtonCancel
} tButton;

typedef enum
{
  LightModeOff = 0,LightModeAuto,LightModeOn
} tLightMode;

typedef enum
{
  BUTT_STATE_NO_CHANGE = 0 ,BUTT_RELEASE,BUTT_PRESS,BUTT_HOLD,BUTT_HOLD_LONG
} tButtonState;

typedef struct
{
  Int32U ButtCount;
  tButtonState ButtState;
} tButtonObj;

#define BUTT_DEB          2                               /* 20ms */
#define BUTT_HOLD_TO      (Int32U)(1.5*TICK_PER_SECOND)   /* 1.5s */
#define BUTT_REPEAT_TO    (Int32U)(0.5*TICK_PER_SECOND)   /* 0.5s */
#define BUTT_HOLD_LONG_TO (Int32U)(5.5*TICK_PER_SECOND)   /* 5.5s */

#define LIGHT_AUTO_TO     10*TICK_PER_SECOND

//#define LIGHT_ON          1
//#define LIGHT_OFF         0

void ScreenSaver  (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                   MEMU_TIME_OUT_DEF * MenuTO,MENU_ANIMATION_TYPE_DEF * AnimationType);

void ButtonsInit  (void);
Boolean Buttons   (void);
tButtonState ButtonFilt (Boolean Press,tButton ButInd);

void LightCntr    (Boolean On);
void LightInit    (void);
void ResetLightTO (void);
void Light        (void);

void SetLightMode (tLightMode Mode);
tLightMode GetLightMode (void);

void IlluminationShow (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                       MEMU_TIME_OUT_DEF * MenuTO,MENU_ANIMATION_TYPE_DEF * AnimationType);

void CursorOff (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType);

tTemperatureUnits GetTempUnit (void);
void SetTempUnit (tTemperatureUnits Unit);

void TempShow (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
               MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType);

void TempUnitShow (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                   MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType);
void TempUnitChange (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                     MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType);

void AnimationEnaShow (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                       MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType);
void AnimationSpeedEnaShow (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                            MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType);
void AnimationSpeed (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                     MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType);

void CalTermShow (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                  MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType);
void CursorOn (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
               MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType);
void AlarmMin (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
               MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType);
void AlarmMax (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
               MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType);
void WarningMin(MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType);
void WarningMax(MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType);

void AlarmEnaShow (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                   MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType);
void WarningEnaShow (MEMU_STING_DEF ** ppVarArg,MEMU_IND_DEF * MenuIndex,
                     MEMU_TIME_OUT_DEF * MenuTO, MENU_ANIMATION_TYPE_DEF * AnimationType);

tActionState GetActionState (void);
void SetAlarmMode(Boolean Mode);
Boolean GetAlarmMode (void);
void SetWarningMode(Boolean Mode);
Boolean GetWarningMode (void);
void InitAction (Boolean AlarmStateL, Flo32 AlarmMinL, Flo32 AlarmMaxL,
                 Boolean WarningStateL, Flo32 WarningMinL, Flo32 WarningMaxL);
void Action (void);

void SetTemp (Flo32 Temp);
Flo32 GetTemp (void);

Flo32 GetCalX (void);
void SetCalX (Flo32 Coef);

#endif  /* __USER_FUNC_H */
