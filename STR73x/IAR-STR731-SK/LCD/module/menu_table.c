/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : menu_table.c
 *    Description : Define menu tables and strings
 *
 *    History :
 *    1. Date        : Feb 10, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : July 22, 2005
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *                     Add menu animation
 *
 *    $Revision: 39 $
 **************************************************************************/

#include "menu_table.h"

/**********************************************************************************
 *                              Action section
 **********************************************************************************/
/*
  Menu Action structure
  Pos1 EventType
  Pos2 MenuIndex
  Pos3 Animation Type
  Pos3 Menu Time out (0 - time out is disable)
  Pos4 Pointer to String (Variable String)
  Pos5 Pointer to User Function Pre  (UserFunc(MEMU_STING_DEF * pVarArg,MEMU_IND_DEF * MenuIndex))
  Pos6 Pointer to User Function Post (UserFunc(MEMU_STING_DEF * pVarArg,MEMU_IND_DEF * MenuIndex))
*/

/* Arg format */
const MEMU_STING_DEF ArgFormate       [] = "\x1B\x4\x1B\2\x1B\x4\x1B\2";
const MEMU_STING_DEF ArgFormate1      [] = " \x1B\x4\x1B\2\x1B\x4\x1B\2";
const MEMU_STING_DEF TempUnitFormate  [] = "x \x1\x4B   \x1\x43   \x1\x46\x1B\2";
const MEMU_STING_DEF TempUnitFormate1 [] = " \x1\x4B   \x1\x43   \x1\x46\x1B\2";
const MEMU_STING_DEF AnimationFormate [] = "x \x1B\2";
const MEMU_STING_DEF AnimationFormate1[] = " \x1B\2";

/* Welcome */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain0 [] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_LEFT,  MENU_SCREEN_SAVER_DLY,    NULL,                                 TempShow,       NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE,NULL,                                 NULL,           NULL},
};

/* Temperature Show */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain1 [] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_SCREENSAVER_SHOW,    MEMU_ANIMATION_RIGHT, MENU_SCREEN_SAVER_1_DLY,   NULL,                                ScreenSaver,    NULL},
  {MENU_EVENT_DATA_UPDATE,    MENU_TEMP_SHOW,           MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_SETTINGS_SHOW,       MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_TEMP_SETTINGS_SHOW,  MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_TEMP_SETTINGS_SHOW,  MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_TEMP_SHOW,           MEMU_ANIMATION_DIS,   MENU_SCREEN_SAVER_DLY,     NULL,                                TempShow,       NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Temperature Settings */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain2 [] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_SETTINGS_SHOW,       MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_ACTION_SETTINGS_SHOW,MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_TEMP_UNITS_SHOW,     MEMU_ANIMATION_RIGHT, MENU_TO_DLY,              (MEMU_STING_DEF *)TempUnitFormate1,   TempUnitShow,NULL},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_TEMP_SHOW,           MEMU_ANIMATION_LEFT,  MENU_SCREEN_SAVER_DLY,     NULL,                                TempShow,       NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Action Settings */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain3 [] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_TEMP_SETTINGS_SHOW,  MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_SETTINGS_SHOW,       MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_ALARM_SETTINGS_SHOW, MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_TEMP_SHOW,           MEMU_ANIMATION_LEFT,  MENU_SCREEN_SAVER_DLY,     NULL,                                TempShow,       NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Menu Settings */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain4 [] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_ACTION_SETTINGS_SHOW,MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_TEMP_SETTINGS_SHOW,  MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_LIGHT_SETTINGS_SHOW, MEMU_ANIMATION_RIGHT, MENU_TO_DLY,              (MEMU_STING_DEF *)ArgFormate1,        IlluminationShow,NULL},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_TEMP_SHOW,           MEMU_ANIMATION_LEFT,  MENU_SCREEN_SAVER_DLY,     NULL,                                TempShow,       NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Screen saver show */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain5 [] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_SCREENSAVER_SHOW,    MEMU_ANIMATION_DIS,   MENU_SCREEN_SAVER_1_DLY,  (MEMU_STING_DEF *)ArgFormate,         ScreenSaver,    NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_TEMP_SHOW,           MEMU_ANIMATION_LEFT,  MENU_SCREEN_SAVER_DLY,     NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_SCREEN_SAVER_DLY,     NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_SCREEN_SAVER_DLY,     NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_TEMP_SHOW,           MEMU_ANIMATION_LEFT,  MENU_SCREEN_SAVER_DLY,     NULL,                                TempShow,       NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Temperature Unit */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain6 [] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_NTC_CAL_SHOW,        MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_NTC_CAL_SHOW,        MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_TEMP_UNITS_SHOW,     MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)TempUnitFormate,    TempUnitShow,NULL},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_TEMP_SETTINGS_SHOW,  MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* NTC Calibration */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain7 [] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_TEMP_UNITS_SHOW,     MEMU_ANIMATION_LEFT,  MENU_TO_DLY,              (MEMU_STING_DEF *)TempUnitFormate1,   TempUnitShow,NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_TEMP_UNITS_SHOW,     MEMU_ANIMATION_RIGHT, MENU_TO_DLY,              (MEMU_STING_DEF *)TempUnitFormate1,   TempUnitShow,NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_NTC_CAL_TEMP_SHOW,   MEMU_ANIMATION_DIS,   MENU_TO_DLY,               NULL,                                CalTermShow,    CursorOn},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_TEMP_SETTINGS_SHOW,  MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Alarm Settings */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain8 [] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_WARNING_SETTINGS_SHOW,MEMU_ANIMATION_LEFT, MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_WARNING_SETTINGS_SHOW,MEMU_ANIMATION_RIGHT,MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_ALARM_ENA_SHOW,      MEMU_ANIMATION_RIGHT, MENU_TO_DLY,              (MEMU_STING_DEF *)AnimationFormate1,  AlarmEnaShow,NULL},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_ACTION_SETTINGS_SHOW,MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Warning Settings */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain9 [] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_ALARM_SETTINGS_SHOW, MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_ALARM_SETTINGS_SHOW, MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_WARNING_ENA_SHOW,    MEMU_ANIMATION_RIGHT, MENU_TO_DLY,              (MEMU_STING_DEF *)AnimationFormate1,  WarningEnaShow, NULL},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_ACTION_SETTINGS_SHOW,MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Illumination Settings */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain10[] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_ANIM_SPEED_SHOW,     MEMU_ANIMATION_LEFT,  MENU_TO_DLY,              (MEMU_STING_DEF *)"1",                AnimationSpeedEnaShow,NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_ANIM_ENABLE_SHOW,    MEMU_ANIMATION_RIGHT, MENU_TO_DLY,              (MEMU_STING_DEF *)ArgFormate1,        AnimationEnaShow,NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_LIGHT_SETTINGS_SHOW, MEMU_ANIMATION_DIS,   MENU_TO_DLY,               NULL,                                IlluminationShow,NULL},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_SETTINGS_SHOW,       MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Animation Enable */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain11[] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre               Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,         NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_LIGHT_SETTINGS_SHOW, MEMU_ANIMATION_LEFT,  MENU_TO_DLY,              (MEMU_STING_DEF *)ArgFormate1,        IlluminationShow, NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_ANIM_SPEED_SHOW,     MEMU_ANIMATION_RIGHT, MENU_TO_DLY,              (MEMU_STING_DEF *)"0",                AnimationSpeedEnaShow,NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_ANIM_ENABLE_SHOW,    MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)AnimationFormate,   AnimationEnaShow, NULL},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_SETTINGS_SHOW,       MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,             NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,             NULL},
};

/* Animation Speed */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain12[] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre               Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,         NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_ANIM_ENABLE_SHOW,    MEMU_ANIMATION_LEFT,  MENU_TO_DLY,              (MEMU_STING_DEF *)AnimationFormate1,  AnimationEnaShow, NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_LIGHT_SETTINGS_SHOW, MEMU_ANIMATION_RIGHT, MENU_TO_DLY,              (MEMU_STING_DEF *)ArgFormate1,        IlluminationShow, NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_ANIM_SPEED_SHOW,     MEMU_ANIMATION_RIGHT, MENU_TO_DLY,              (MEMU_STING_DEF *)NULL,               AnimationSpeed,   NULL},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_SETTINGS_SHOW,       MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,             NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,             NULL},
};

/* NTC Calibration set temperature */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain13[] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       CursorOff},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_NTC_CAL_TEMP_SHOW,   MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"<",                CalTermShow,    CursorOn},
  {MENU_EVENT_B_LEFT_HOLD,    MENU_NTC_CAL_TEMP_SHOW,   MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"<",                CalTermShow,    CursorOn},
  {MENU_EVENT_B_LEFT_HOLD_LONG,MENU_NTC_CAL_TEMP_SHOW,  MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"D",                CalTermShow,    CursorOn},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_NTC_CAL_TEMP_SHOW,   MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)">",                CalTermShow,    CursorOn},
  {MENU_EVENT_B_RIGHT_HOLD,   MENU_NTC_CAL_TEMP_SHOW,   MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)">",                CalTermShow,    CursorOn},
  {MENU_EVENT_B_RIGHT_HOLD_LONG,MENU_NTC_CAL_TEMP_SHOW, MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"I",                CalTermShow,    CursorOn},
  {MENU_EVENT_B_SELECT_PRESS, MENU_NTC_CAL_SHOW,        MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)ArgFormate1,        CalTermShow,    CursorOff},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_NTC_CAL_SHOW,        MEMU_ANIMATION_DIS,   MENU_TO_DLY,               NULL,                                NULL,           CursorOff},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Alarm Enable */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain14[] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_ALARM_MAX_SHOW,      MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                AlarmMax,       NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_ALARM_MIN_SHOW,      MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                AlarmMin,       NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_ALARM_ENA_SHOW,      MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)AnimationFormate,   AlarmEnaShow,   NULL},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_ALARM_SETTINGS_SHOW, MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Alarm Min */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain15[] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_ALARM_ENA_SHOW,      MEMU_ANIMATION_LEFT,  MENU_TO_DLY,              (MEMU_STING_DEF *)AnimationFormate1,  AlarmEnaShow,   NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_ALARM_MAX_SHOW,      MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                AlarmMax,       NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_ALARM_MIN_ENT_SHOW,  MEMU_ANIMATION_DIS,   MENU_TO_DLY,               NULL,                                AlarmMin,       CursorOn},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_ALARM_SETTINGS_SHOW, MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Alarm Max */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain16[] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_ALARM_MIN_SHOW,      MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                AlarmMin,       NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_ALARM_ENA_SHOW,      MEMU_ANIMATION_RIGHT, MENU_TO_DLY,              (MEMU_STING_DEF *)AnimationFormate1,  AlarmEnaShow,   NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_ALARM_MAX_ENT_SHOW,  MEMU_ANIMATION_DIS,   MENU_TO_DLY,               NULL,                                AlarmMax,       CursorOn},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_ALARM_SETTINGS_SHOW, MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Warning Enable */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain17[] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_WARNING_MAX_SHOW,    MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                WarningMax,     NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_WARNING_MIN_SHOW,    MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                WarningMin,     NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_WARNING_ENA_SHOW,    MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)AnimationFormate,   WarningEnaShow, NULL},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_WARNING_SETTINGS_SHOW,MEMU_ANIMATION_LEFT, MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Warning Min */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain18[] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_WARNING_ENA_SHOW,    MEMU_ANIMATION_LEFT,  MENU_TO_DLY,              (MEMU_STING_DEF *)AnimationFormate1,  WarningEnaShow, NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_WARNING_MAX_SHOW,    MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                WarningMax,     NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_WARNING_MIN_ENT_SHOW,MEMU_ANIMATION_DIS,   MENU_TO_DLY,               NULL,                                WarningMin,     CursorOn},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_WARNING_SETTINGS_SHOW,MEMU_ANIMATION_LEFT, MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Warning Max */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain19[] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       NULL},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_WARNING_MIN_SHOW,    MEMU_ANIMATION_LEFT,  MENU_TO_DLY,               NULL,                                WarningMin,     NULL},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_WARNING_ENA_SHOW,    MEMU_ANIMATION_RIGHT, MENU_TO_DLY,              (MEMU_STING_DEF *)AnimationFormate1,  WarningEnaShow, NULL},
  {MENU_EVENT_B_SELECT_PRESS, MENU_WARNING_MAX_ENT_SHOW,MEMU_ANIMATION_DIS,   MENU_TO_DLY,               NULL,                                WarningMax,     CursorOn},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_WARNING_SETTINGS_SHOW,MEMU_ANIMATION_LEFT, MENU_TO_DLY,               NULL,                                NULL,           NULL},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Alarm Min Enter */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain20[] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       CursorOff},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_ALARM_MIN_ENT_SHOW,  MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"<",                AlarmMin,       CursorOn},
  {MENU_EVENT_B_LEFT_HOLD,    MENU_ALARM_MIN_ENT_SHOW,  MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"<",                AlarmMin,       CursorOn},
  {MENU_EVENT_B_LEFT_HOLD_LONG,MENU_ALARM_MIN_ENT_SHOW, MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"D",                AlarmMin,       CursorOn},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_ALARM_MIN_ENT_SHOW,  MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)">",                AlarmMin,       CursorOn},
  {MENU_EVENT_B_RIGHT_HOLD,   MENU_ALARM_MIN_ENT_SHOW,  MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)">",                AlarmMin,       CursorOn},
  {MENU_EVENT_B_RIGHT_HOLD_LONG,MENU_ALARM_MIN_ENT_SHOW,MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"I",                AlarmMin,       CursorOn},
  {MENU_EVENT_B_SELECT_PRESS, MENU_ALARM_MIN_SHOW,      MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)ArgFormate1,        AlarmMin,       CursorOff},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_ALARM_MIN_SHOW,      MEMU_ANIMATION_DIS,   MENU_TO_DLY,               NULL,                                AlarmMin,       CursorOff},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Alarm Max Enter */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain21[] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       CursorOff},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_ALARM_MAX_ENT_SHOW,  MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"<",                AlarmMax,       CursorOn},
  {MENU_EVENT_B_LEFT_HOLD,    MENU_ALARM_MAX_ENT_SHOW,  MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"<",                AlarmMax,       CursorOn},
  {MENU_EVENT_B_LEFT_HOLD_LONG,MENU_ALARM_MAX_ENT_SHOW, MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"D",                AlarmMax,       CursorOn},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_ALARM_MAX_ENT_SHOW,  MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)">",                AlarmMax,       CursorOn},
  {MENU_EVENT_B_RIGHT_HOLD,   MENU_ALARM_MAX_ENT_SHOW,  MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)">",                AlarmMax,       CursorOn},
  {MENU_EVENT_B_RIGHT_HOLD_LONG,MENU_ALARM_MAX_ENT_SHOW,MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"I",                AlarmMax,       CursorOn},
  {MENU_EVENT_B_SELECT_PRESS, MENU_ALARM_MAX_SHOW,      MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)ArgFormate1,        AlarmMax,       CursorOff},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_ALARM_MAX_SHOW,      MEMU_ANIMATION_DIS,   MENU_TO_DLY,               NULL,                                AlarmMax,       CursorOff},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Warning Min Enter */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain22[] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       CursorOff},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_WARNING_MIN_ENT_SHOW,MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"<",                WarningMin,     CursorOn},
  {MENU_EVENT_B_LEFT_HOLD,    MENU_WARNING_MIN_ENT_SHOW,MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"<",                WarningMin,     CursorOn},
  {MENU_EVENT_B_LEFT_HOLD_LONG,MENU_WARNING_MIN_ENT_SHOW,MEMU_ANIMATION_DIS,  MENU_TO_DLY,              (MEMU_STING_DEF *)"D",                WarningMin,     CursorOn},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_WARNING_MIN_ENT_SHOW,MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)">",                WarningMin,     CursorOn},
  {MENU_EVENT_B_RIGHT_HOLD,   MENU_WARNING_MIN_ENT_SHOW,MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)">",                WarningMin,     CursorOn},
  {MENU_EVENT_B_RIGHT_HOLD_LONG,MENU_WARNING_MIN_ENT_SHOW,MEMU_ANIMATION_DIS, MENU_TO_DLY,              (MEMU_STING_DEF *)"I",                WarningMin,     CursorOn},
  {MENU_EVENT_B_SELECT_PRESS, MENU_WARNING_MIN_SHOW,    MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)ArgFormate1,        WarningMin,     CursorOff},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_WARNING_MIN_SHOW,    MEMU_ANIMATION_DIS,   MENU_TO_DLY,               NULL,                                WarningMin,     CursorOff},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/* Warning Max Enter */
const MENU_ACTION_SHAIN_DEF MenuActionEventChain23[] =
{
/*  EventType                 MenuIndex                 Animation Type        Menu Time out             Pointer to String                     Pre             Post */
  {MENU_TICK_EVENT,           MENU_TEMP_SHOW,           MEMU_ANIMATION_RIGHT, MENU_TO_DLY,               NULL,                                TempShow,       CursorOff},
  {MENU_EVENT_B_LEFT_PRESS,   MENU_WARNING_MAX_ENT_SHOW,MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"<",                WarningMax,     CursorOn},
  {MENU_EVENT_B_LEFT_HOLD,    MENU_WARNING_MAX_ENT_SHOW,MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)"<",                WarningMax,     CursorOn},
  {MENU_EVENT_B_LEFT_HOLD_LONG,MENU_WARNING_MAX_ENT_SHOW,MEMU_ANIMATION_DIS,  MENU_TO_DLY,              (MEMU_STING_DEF *)"D",                WarningMax,     CursorOn},
  {MENU_EVENT_B_RIGHT_PRESS,  MENU_WARNING_MAX_ENT_SHOW,MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)">",                WarningMax,     CursorOn},
  {MENU_EVENT_B_RIGHT_HOLD,   MENU_WARNING_MAX_ENT_SHOW,MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)">",                WarningMax,     CursorOn},
  {MENU_EVENT_B_RIGHT_HOLD_LONG,MENU_WARNING_MAX_ENT_SHOW,MEMU_ANIMATION_DIS, MENU_TO_DLY,              (MEMU_STING_DEF *)"I",                WarningMax,     CursorOn},
  {MENU_EVENT_B_SELECT_PRESS, MENU_WARNING_MAX_SHOW,    MEMU_ANIMATION_DIS,   MENU_TO_DLY,              (MEMU_STING_DEF *)ArgFormate1,        WarningMax,     CursorOff},
  {MENU_EVENT_B_CANCEL_PRESS, MENU_WARNING_MAX_SHOW,    MEMU_ANIMATION_DIS,   MENU_TO_DLY,               NULL,                                WarningMax,     CursorOff},
  {0,                         0,                        MEMU_ANIMATION_DIS,   MENU_TIME_OUT_NOT_UPADATE, NULL,                                NULL,           NULL},
};

/*
  Action chain address table
*/
const MENU_ACTION_SHAIN_DEF * const MenuActionChainAdd [MENU_MAX_INDEX] =
{
  MenuActionEventChain0,
  MenuActionEventChain1,
  MenuActionEventChain2,
  MenuActionEventChain3,
  MenuActionEventChain4,
  MenuActionEventChain5,
  MenuActionEventChain6,
  MenuActionEventChain7,
  MenuActionEventChain8,
  MenuActionEventChain9,
  MenuActionEventChain10,
  MenuActionEventChain11,
  MenuActionEventChain12,
  MenuActionEventChain13,
  MenuActionEventChain14,
  MenuActionEventChain15,
  MenuActionEventChain16,
  MenuActionEventChain17,
  MenuActionEventChain18,
  MenuActionEventChain19,
  MenuActionEventChain20,
  MenuActionEventChain21,
  MenuActionEventChain22,
  MenuActionEventChain23,
};

/**********************************************************************************
 *                              Visual section
 **********************************************************************************/

/*
  Menu chain data format
  Pos1 Pointer to String1 (Last element is NULL)
  Pos2 X1 coordinate
  Pos3 Y1 coordinate
  .
  .
  PosN-2 Pointer to StringN (Last element is NULL)
  PosN-1 XN coordinate
  PosN YN coordinate

  Menu meta language :
  0x00 - end of menu string
  0x01 - get variable data from (*pMenuVarArg)
  0x02 - return to const data
*/
/*0 Welcome */
const MENU_VISUAL_DEF MenuVisualLang1Chain0 [] =
{
  /* Line1 */
  {"  IAR Embedded  ",1,1},
  /* Line2 */
  {" Workbench  ARM ",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*1 Temperature Show */
const MENU_VISUAL_DEF MenuVisualLang1Chain1 [] =
{
  /* Line1 */
  {"\x1B\x4\x1B\x1",1,1},
  /* Line2 */
  {"\x1B\x4\x1B\x1",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*2 Temperature settings */
const MENU_VISUAL_DEF MenuVisualLang1Chain2 [] =
{
  /* Line1 */
  {"Temperature\x1B\x4",1,1},
  /* Line2 */
  {"Settings\x1B\x4",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*3 Action settings */
const MENU_VISUAL_DEF MenuVisualLang1Chain3 [] =
{
  /* Line1 */
  {"Action\x1B\x4",1,1},
  /* Line2 */
  {"Settings\x1B\x4",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*4 Menu settings */
const MENU_VISUAL_DEF MenuVisualLang1Chain4 [] =
{
  /* Line1 */
  {"Menu\x1B\x4",1,1},
  /* Line2 */
  {"Settings\x1B\x4",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*6 2.1 temperature unit */
const MENU_VISUAL_DEF MenuVisualLang1Chain21 [] =
{
  /* Line1 */
  {"Unit Select\x1B\x4",1,1},
  /* Line2 */
  {"\x1B\x1\x1B\4",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*7 2.2 NTC Calibration */
const MENU_VISUAL_DEF MenuVisualLang1Chain22 [] =
{
  /* Line1 */
  {"NTC\x1B\x4",1,1},
  /* Line2 */
  {"Calibration\x1B\x4",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*8 3.1 Alarm Settings */
const MENU_VISUAL_DEF MenuVisualLang1Chain31 [] =
{
  /* Line1 */
  {"Alarm\x1B\x4",1,1},
  /* Line2 */
  {"setting\x1B\x4",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*9 3.2 Warning Settings */
const MENU_VISUAL_DEF MenuVisualLang1Chain32 [] =
{
  /* Line1 */
  {"Warning\x1B\x4",1,1},
  /* Line2 */
  {"setting\x1B\x4",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*10 4.1 Illumination Settings */
const MENU_VISUAL_DEF MenuVisualLang1Chain41 [] =
{
  /* Line1 */
  {"Illumination\x1B\x4",1,1},
  /* Line2 */
  {"setting - \x1B\x4\x1B\x1",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*11 4.2 Animation Enable */
const MENU_VISUAL_DEF MenuVisualLang1Chain42 [] =
{
  /* Line1 */
  {"Animation\x1B\x4",1,1},
  /* Line2 */
  {"Enable  \x1B\x4\x1B\x1",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*12 4.3 Animation Speed */
const MENU_VISUAL_DEF MenuVisualLang1Chain43 [] =
{
  /* Line1 */
  {"Animation\x1B\x4",1,1},
  /* Line2 */
  {"Speed \x1B\x4",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*13 2.2.1 NTC Calibration Set temperature */
const MENU_VISUAL_DEF MenuVisualLang1Chain221 [] =
{
  /* Line1 */
  {"NTC Calibration ",1,1},
  /* Line2 */
  {"\x1B\x4\x1B\x1",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*14 3.1.1 Alarm Enable */
const MENU_VISUAL_DEF MenuVisualLang1Chain311 [] =
{
  /* Line1 */
  {"Alarm\x1B\x4",1,1},
  /* Line2 */
  {"Enable \x1B\x4\x1B\x1",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*17 3.2.1 Warning Enable */
const MENU_VISUAL_DEF MenuVisualLang1Chain321 [] =
{
  /* Line1 */
  {"Warning\x1B\x4",1,1},
  /* Line2 */
  {"Enable \x1B\x4\x1B\x1",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*15 3.1.2 Alarm Min */
/*20 3.1.2.1 Alarm Min Enter */
const MENU_VISUAL_DEF MenuVisualLang1Chain3121 [] =
{
  /* Line1 */
  {"Alarm Min\x1B\x4",1,1},
  /* Line2 */
  {"\x1B\x4\x1B\x1",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*16 3.1.3 Alarm Max */
/*21 3.1.3.1 Alarm Max Enter */
const MENU_VISUAL_DEF MenuVisualLang1Chain3131 [] =
{
  /* Line1 */
  {"Alarm Max\x1B\x4",1,1},
  /* Line2 */
  {"\x1B\x4\x1B\x1",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*18 3.2.2 Warning Min */
/*22 3.2.2.1 Warning Min Enter */
const MENU_VISUAL_DEF MenuVisualLang1Chain3221 [] =
{
  /* Line1 */
  {"Warning Min\x1B\x4",1,1},
  /* Line2 */
  {"\x1B\x4\x1B\x1",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*19 3.2.3 Warning Max */
/*23 3.2.3.1 Warning Max Enter */
const MENU_VISUAL_DEF MenuVisualLang1Chain3231 [] =
{
  /* Line1 */
  {"Warning Max\x1B\x4",1,1},
  /* Line2 */
  {"\x1B\x4\x1B\x1",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*5 ScreenSaver show */
const MENU_VISUAL_DEF MenuVisualLang1Chain01 [] =
{
  /* Line1 */
  {"\x1B\x1",1,1},
  /* Line2 */
  {"\x1B\x1",1,2},
  /* end of chain */
  {NULL,0,0},
};

/*
  Language1 visualization chain address table
*/
const MENU_VISUAL_DEF * const MenuVisualLang1ChainAddTable [MENU_MAX_INDEX] =
{
/*0 Welcome */
  MenuVisualLang1Chain0,
/*1 Temperature Show */
  MenuVisualLang1Chain1,
/*2 Temperature settings */
  MenuVisualLang1Chain2,
/*3 Action settings */
  MenuVisualLang1Chain3,
/*4 Menu settings */
  MenuVisualLang1Chain4,
/*5 ScreenSaver show */
  MenuVisualLang1Chain01,
/*6 2.1 temperature unit */
  MenuVisualLang1Chain21,
/*7 2.2 NTC Calibration */
  MenuVisualLang1Chain22,
/*8 3.1 Alarm Settings */
  MenuVisualLang1Chain31,
/*9 3.2 Warning Settings */
  MenuVisualLang1Chain32,
/*10 4.1 Illumination Settings */
  MenuVisualLang1Chain41,
/*11 4.2 Animation Enable */
  MenuVisualLang1Chain42,
/*12 4.3 Animation Speed */
  MenuVisualLang1Chain43,
/*13 2.2.1 NTC Calibration Set temperature */
  MenuVisualLang1Chain221,
/*14 3.1.1 Alarm Enable */
  MenuVisualLang1Chain311,
/*15 3.1.2 Alarm Min */
  MenuVisualLang1Chain3121,
/*16 3.1.3 Alarm Max */
  MenuVisualLang1Chain3131,
/*17 3.2.1 Warning Enable */
  MenuVisualLang1Chain321,
/*18 3.2.2 Warning Min */
  MenuVisualLang1Chain3221,
/*19 3.2.3 Warning Max */
  MenuVisualLang1Chain3231,
/*20 3.1.2.1 Alarm Min Enter */
  MenuVisualLang1Chain3121,
/*21 3.1.3.1 Alarm Max Enter */
  MenuVisualLang1Chain3131,
/*22 3.2.2.1 Warning Min Enter */
  MenuVisualLang1Chain3221,
/*23 3.2.3.1 Warning Max Enter */
  MenuVisualLang1Chain3231,
};

/*
  Language table base address table
*/
const MENU_VISUAL_DEF * const * const MenuLanguageBaseAddTable [MENU_LANGUAGE_NUMBER] =
{
  MenuVisualLang1ChainAddTable,
};

