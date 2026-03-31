/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : menu_cnfg.h
 *    Description : Define menu common definition
 *
 *    History :
 *    1. Date        : July 20, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#ifndef  __MENU_CONFIG_H
#define  __MENU_CONFIG_H

/* USER Menu Index definition */
typedef enum
{
  MENU_WELCOME_SHOW = 0,      /*0 Welcome */
  MENU_TEMP_SHOW,             /*1 Temperature Show */
  MENU_TEMP_SETTINGS_SHOW,    /*2 Temperature settings */
  MENU_ACTION_SETTINGS_SHOW,  /*3 Action settings */
  MENU_SETTINGS_SHOW,         /*4 Menu settings */
  MENU_SCREENSAVER_SHOW,      /*5 ScreenSaver show */

  MENU_TEMP_UNITS_SHOW,       /*6 2.1 temperature unit */
  MENU_NTC_CAL_SHOW,          /*7 2.2 NTC Calibration */

  MENU_ALARM_SETTINGS_SHOW,   /*8 3.1 Alarm Settings */
  MENU_WARNING_SETTINGS_SHOW, /*9 3.2 Warning Settings */

  MENU_LIGHT_SETTINGS_SHOW,   /*10 4.1 Illumination Settings */
  MENU_ANIM_ENABLE_SHOW,      /*11 4.2 Animation Enable */
  MENU_ANIM_SPEED_SHOW,       /*12 4.3 Animation Speed */

  MENU_NTC_CAL_TEMP_SHOW,     /*13 2.2.1 NTC Calibration Set temperature */

  MENU_ALARM_ENA_SHOW,        /*14 3.1.1 Alarm Enable */
  MENU_ALARM_MIN_SHOW,        /*15 3.1.2 Alarm Min */
  MENU_ALARM_MAX_SHOW,        /*16 3.1.3 Alarm Max */

  MENU_WARNING_ENA_SHOW,      /*17 3.2.1 Warning Enable */
  MENU_WARNING_MIN_SHOW,      /*18 3.2.2 Warning Min */
  MENU_WARNING_MAX_SHOW,      /*19 3.2.3 Warning Max */

  MENU_ALARM_MIN_ENT_SHOW,    /*20 3.1.2.1 Alarm Min Enter */
  MENU_ALARM_MAX_ENT_SHOW,    /*21 3.1.3.1 Alarm Max Enter */

  MENU_WARNING_MIN_ENT_SHOW,  /*22 3.2.2.1 Warning Min Enter */
  MENU_WARNING_MAX_ENT_SHOW,  /*23 3.2.3.1 Warning Max Enter */

  MENU_LAST_IND               /* Last menu index */
} MENU_IND_NAME_DEF;

/* USER Event definition */
typedef enum
{
  /* 0 is reserve for internal using */
  MENU_TICK_EVENT = 1,  /* SYS TICK */
  MENU_EVENT_B_LEFT_PRESS,
  MENU_EVENT_B_LEFT_HOLD,
  MENU_EVENT_B_LEFT_HOLD_LONG,
  MENU_EVENT_B_RIGHT_PRESS,
  MENU_EVENT_B_RIGHT_HOLD,
  MENU_EVENT_B_RIGHT_HOLD_LONG,
  MENU_EVENT_B_SELECT_PRESS,
  MENU_EVENT_B_CANCEL_PRESS,
  MENU_EVENT_DATA_UPDATE,
} MENU_EVENT_NAME_DEF;

/* User Language definition */
typedef enum
{
  MENU_ENGLISH_LANG = 0,  /*0 English language */
} MENU_LANGUAGE_NAME_DEF;

/* Menu time out definition */
#define MENU_WELCOM_DLY           3 *TICK_PER_SECOND    /* 3  sec */
#define MENU_SCREEN_SAVER_DLY     25*TICK_PER_SECOND    /* 10 sec */
#define MENU_SCREEN_SAVER_1_DLY   4 *TICK_PER_SECOND    /* 4  sec */
#define MENU_TO_DLY               10*TICK_PER_SECOND    /* 10 sec */
#define MENU_LOW_POWER_DLY        20*TICK_PER_SECOND    /* 20 sec */

#define MENU_ANIMATION_DEF_SPEED  3

/* Common menu definitions */
#define MENU_EVENT_STACK_SIZE 5

#define MENU_LANGUAGE_NUMBER  1

#define MENU_MAX_INDEX        MENU_LAST_IND

#define MENU_MAX_EVENT_NUBER  256

#define MENU_MAX_STR_LENG     41

#define MENU_MAX_VISIBILITY_X 16
#define MENU_MAX_VISIBILITY_Y 2

#define MEMU_MAX_TIME_OUT     1000

/* Menu Events type def */
#if MENU_MAX_EVENT_NUBER < 256
#define MENU_EVENT_TYPE_DEF Int8U
#elif MENU_MAX_EVENT_NUBER < 65536
#define MENU_EVENT_TYPE_DEF Int16U
#elif MENU_LANGUAGE_NUMBER < 0x100000000
#define MENU_EVENT_TYPE_DEF Int32U
#else
#error "MENU_MAX_EVENT_NUBER is to large number > 32bit!!!"
#endif

/* Menu Language type def */
#if MENU_LANGUAGE_NUMBER < 256
#define MENU_LANG_TYPE_DEF Int8U
#elif MENU_LANGUAGE_NUMBER < 65536
#define MENU_LANG_TYPE_DEF Int16U
#elif MENU_LANGUAGE_NUMBER < 0x100000000
#define MENU_LANG_TYPE_DEF Int32U
#else
#error "MENU_LANGUAGE_NUMBER is to large number > 32bit!!!"
#endif

/* Menu Index type def */
#if MENU_MAX_INDEX < 256
#define MEMU_IND_DEF Int8U
#elif MENU_MAX_INDEX < 65536
#define MEMU_IND_DEF Int16U
#elif MENU_MAX_INDEX < 0x100000000
#define MEMU_IND_DEF Int32U
#else
#error "MENU_MAX_INDEX is to large number > 32bit!!!"
#endif

/* XY coordinate type def */
#if ((MENU_MAX_VISIBILITY_X < 256) && (MENU_MAX_VISIBILITY_Y < 256))
#define MEMU_XY_DEF Int8U
#elif ((MENU_MAX_VISIBILITY_X < 65536) && (MENU_MAX_VISIBILITY_Y < 65536))
#define MEMU_XY_DEF Int16U
#elif ((MENU_MAX_VISIBILITY_X < 0x100000000) && (MENU_MAX_VISIBILITY_Y < 0x100000000))
#define MEMU_XY_DEF Int32U
#else
#error "X or Y is to large number > 32bit!!!"
#endif

/* time out type def */
#if MEMU_MAX_TIME_OUT < 255
#define MEMU_TIME_OUT_DEF Int8U
#define MENU_TIME_OUT_NOT_UPADATE 255
#elif MEMU_MAX_TIME_OUT < 65535
#define MEMU_TIME_OUT_DEF Int16U
#define MENU_TIME_OUT_NOT_UPADATE 65535
#elif MEMU_MAX_TIME_OUT < 0xFFFFFFFF
#define MEMU_TIME_OUT_DEF Int32U
#define MENU_TIME_OUT_NOT_UPADATE 0xFFFFFFFF
#else
#error "Menu Time out is to large number > 32bit!!!"
#endif

#endif  /* __MENU_CONFIG_H */
