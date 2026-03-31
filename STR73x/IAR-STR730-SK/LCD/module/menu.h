/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : menu.h
 *    Description : Menu module include file
 *
 *    History :
 *    1. Date        : Feb 10, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : July 22, 2005
 *       Author      : Stanimir Bonev
 *       Description : Add menu animation
 *    $Revision: 39 $
 *
 **************************************************************************/
#include "includes.h"
#include "menu_cnfg.h"

#ifndef  __MENU_H
#define  __MENU_H

#define END_OF_STR        '\0'

#define ESC_CTRL          0x1B
#define VARIABLE_DATA     1
#define END_OF_VAR_DATA   2
#define DISPLAY_CLEAR     3
#define DELETE_TO_EOL     4

typedef char MEMU_STING_DEF ;
typedef enum
{
  MEMU_ANIMATION_DIS = 0,MEMU_ANIMATION_LEFT,
  MEMU_ANIMATION_RIGHT
}MENU_ANIMATION_TYPE_DEF;

/* Menu XY coordinate type def*/
typedef Int8U MENU_COORDINATE_XY_DEF;

typedef struct
{
  const MEMU_STING_DEF * MenuText;
  MEMU_XY_DEF X;
  MEMU_XY_DEF Y;
} MENU_VISUAL_DEF;

typedef enum
{
  MENU_OK = 0,              /* command pass */
  MENU_LANG_ERROR,          /* not valid language */
  MENU_IND_ERROR,           /* not valid menu index */
  MENU_EVET_STACK_OVERFLOW, /* Event stack overflow */
  MENU_SHOW_OUT_OF_VISUAL,  /* String out of visual space */
  MENU_SHOW_ERROR,          /* Error into LCD driver module */
  MENU_TO_ERROR,            /* Menu tome out is to long */
}MENU_ERROR_CODE_DEF;

typedef void (*MENU_USER_FUNC_DEF)(MEMU_STING_DEF ** ppVarArg,
                                   MEMU_IND_DEF * MenuIndex,
                                   MEMU_TIME_OUT_DEF * MenuTO,
                                   MENU_ANIMATION_TYPE_DEF * AnimationType);

typedef struct
{
  MENU_EVENT_TYPE_DEF     EventType;
  MEMU_IND_DEF            MenuIndex;
  MENU_ANIMATION_TYPE_DEF AnimationType;
  MEMU_TIME_OUT_DEF       MenuTO;
  MEMU_STING_DEF  *       pVarArg;
  MENU_USER_FUNC_DEF      UserFunc;
  MENU_USER_FUNC_DEF      UserFunc1;
} MENU_ACTION_SHAIN_DEF;

MENU_ERROR_CODE_DEF MenuInit (MENU_LANG_TYPE_DEF Language,
MEMU_IND_DEF MainMenuInd, MEMU_STING_DEF *pVarData, MEMU_TIME_OUT_DEF MenuTO, MENU_ANIMATION_TYPE_DEF AnimationType);

MENU_ERROR_CODE_DEF MenuSetEvent (MENU_EVENT_TYPE_DEF MenuEvent);
MENU_ERROR_CODE_DEF Menu (void);
MENU_EVENT_TYPE_DEF MenuGetInd(void);
MENU_ACTION_SHAIN_DEF * MenuGetActionChain(MEMU_IND_DEF MenuInd);
MENU_VISUAL_DEF * MenuGetVisualChain(MENU_LANG_TYPE_DEF Language,MEMU_IND_DEF MenuInd);
MENU_ERROR_CODE_DEF  MenuAnimation (void);
void MenuInitAnimation(Int8U X, Int8U Y,MEMU_STING_DEF *pData, MENU_ANIMATION_TYPE_DEF AnimationMode);
Boolean MenuGetAnimMode (void);
void MenuSetAnimMode (Boolean Flag);

Int32U MenuGetAnimSpeed (void);
void MenuSetAnimSpeed (Int32U Speed);
Int16S MenuGetAnimationProgress (void);

#endif  /* __MENU_H */
