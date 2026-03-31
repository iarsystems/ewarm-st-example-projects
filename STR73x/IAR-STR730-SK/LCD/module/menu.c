/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : menu.c
 *    Description : Menu module
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
#include "menu.h"
#include "menu_table.h"

typedef struct
{
  MENU_EVENT_TYPE_DEF * Push;
  MENU_EVENT_TYPE_DEF * Pop;
  MENU_EVENT_TYPE_DEF EventStackBuffer [MENU_EVENT_STACK_SIZE]; /* Hold events */
} EVENT_STACK_DEF;

#define   MENU_EVET_STACK_EMPTY       FALSE    /* Event stack is empty */
#define   MENU_EVET_STACK_DATA_VALID  TRUE     /* Event stack data valid */

EVENT_STACK_DEF EventStack;
const MENU_VISUAL_DEF *const * LanguageBaseAdd;
MEMU_IND_DEF MenuInd;
MEMU_TIME_OUT_DEF MenuTO;

Boolean AnimationEnableFlag;
Int16S MenuAnimationStep;
Int32U MenuAnimationSpeed;
MENU_ANIMATION_TYPE_DEF AnimationModeHold;

MENU_USER_FUNC_DEF UserFunc1Hold;

MEMU_STING_DEF StrLine1Prev[MENU_MAX_STR_LENG];
MEMU_STING_DEF StrLine2Prev[MENU_MAX_STR_LENG];

MEMU_STING_DEF StrLine1Next[MENU_MAX_STR_LENG];
MEMU_STING_DEF StrLine2Next[MENU_MAX_STR_LENG];

/*************************************************************************
 * Function Name: MenuStackInit
 * Parameters: None
 * Return: none
 * Description: Initialize menu event stack structure
 *
 *************************************************************************/
void MenuStackInit ()
{
  EventStack.Push = EventStack.Pop = EventStack.EventStackBuffer;
}

/*************************************************************************
 * Function Name: MenuShow
 * Parameters: MEMU_STING_DEF *pVarData,MENU_ANIMATION_TYPE_DEF AnimationMode
 * Return:  MENU_ERROR_CODE_DEF
 *          MENU_OK                 0: success
 *          MENU_LANG_ERROR         1: fail not valid language
 *          MENU_IND_ERROR          2: fail not valid menu index
 *          MENU_SHOW_OUT_OF_VISUAL 4: fail String out of visual space
 *          MENU_SHOW_ERROR         5: fail Error into LCD driver module
 *
 * Description: Show current visual chain to LCD
 *
 *************************************************************************/
MENU_ERROR_CODE_DEF MenuShow (MEMU_STING_DEF *pVarData,MENU_ANIMATION_TYPE_DEF AnimationMode)
{
MENU_VISUAL_DEF * pData = (MENU_VISUAL_DEF *)*(LanguageBaseAdd+MenuInd);
MEMU_STING_DEF DataStr[MENU_MAX_STR_LENG];
MEMU_STING_DEF * pDataHold = NULL;
MEMU_STING_DEF * pDestData;
const MEMU_STING_DEF * pSourceData;
Int8U i,Result = MENU_OK;
Boolean DelToEol = FALSE;
  if (MenuInd > MENU_MAX_INDEX-1)
  {
    return MENU_IND_ERROR;
  }
  else
  {
    while (pData->MenuText) {
      pSourceData = pData->MenuText;
      pDestData = DataStr;
      while(*pSourceData || (pDataHold != NULL))
      {
        if(*pSourceData == ESC_CTRL)
        {
          switch(*++pSourceData)
          {
          case END_OF_STR:
            pVarData = (char *)++pSourceData;
            pSourceData = pDataHold;
            pDataHold = NULL;
            break;
          case VARIABLE_DATA:
            if((pVarData != NULL) && (*pVarData))
            {
              pDataHold = (char *)++pSourceData;
              pSourceData = pVarData;
            }
            else
            {
              pVarData = NULL;
            }
            break;
          case END_OF_VAR_DATA:
            if(pDataHold != NULL)
            {
              pVarData = (char *)++pSourceData;
              pSourceData = pDataHold;
              pDataHold = NULL;
            }
            else
            {
              ++pSourceData;
            }
            break;
          case DISPLAY_CLEAR:
            HD44780_ClearDisplay();
            ++pSourceData;
            break;
          case DELETE_TO_EOL:
            for(i = pDestData - DataStr;i<MENU_MAX_VISIBILITY_X;++i)
            {
              DataStr[i] = ' ';
            }
            DelToEol = TRUE;
            ++pSourceData;
            break;
          }
        }
        else
        {
          if((pDestData - DataStr) >= MENU_MAX_STR_LENG-1) break;
          *pDestData = *pSourceData;
          ++pDestData;++pSourceData;
        }
      }
      if(DelToEol)
      {
        DataStr[MENU_MAX_VISIBILITY_X]=0;
      }
      else
      {
        *pDestData = 0;
      }
      DelToEol = FALSE;
      MenuInitAnimation(pData->X,pData->Y,DataStr,AnimationMode);
      ++pData;
    }
  }
  return (MENU_ERROR_CODE_DEF)Result;
}

/*************************************************************************
 * Function Name: MenuInit
 * Parameters: MENU_LANG_TYPE_DEF Language,
 *             MEMU_IND_DEF MainMenuInd,
 *             MEMU_STING_DEF *pVarData,
 *             MEMU_TIME_OUT_DEF MenuTO_Init
 *             MENU_ANIMATION_DEF AnimationType
 *
 * Return:  MENU_ERROR_CODE_DEF
 *          MENU_OK                 0: success
 *          MENU_LANG_ERROR         1: fail not valid language
 *          MENU_IND_ERROR          2: fail not valid menu index
 *          MENU_SHOW_OUT_OF_VISUAL 4: fail String out of visual space
 *          MENU_SHOW_ERROR         5: fail Error into LCD driver module
 *          MENU_TO_ERROR           6: fail Menu tome out is to long
 *
 * Description: Initialize Menu
 *
 *************************************************************************/
MENU_ERROR_CODE_DEF MenuInit (MENU_LANG_TYPE_DEF Language,
  MEMU_IND_DEF MainMenuInd, MEMU_STING_DEF *pVarData, MEMU_TIME_OUT_DEF MenuTO_Init,MENU_ANIMATION_TYPE_DEF AnimationType)
{
  /* Init Menu Events stack */
  MenuStackInit();
  AnimationEnableFlag = TRUE;
  MenuAnimationStep = 0;
  MenuAnimationSpeed = MENU_ANIMATION_DEF_SPEED;
  strcpy(StrLine1Prev,"                ");
  strcpy(StrLine2Prev,"                ");

  if (Language > MENU_LANGUAGE_NUMBER-1)
  {
    return MENU_LANG_ERROR;
  }
  LanguageBaseAdd = MenuLanguageBaseAddTable[Language];
  if (MainMenuInd > MENU_MAX_INDEX-1)
  {
    return MENU_IND_ERROR;
  }
  MenuInd = MainMenuInd;
  if (MenuTO_Init > MEMU_MAX_TIME_OUT)
  {
    return MENU_TO_ERROR;
  }
  MenuTO = MenuTO_Init;
  return MenuShow(pVarData,AnimationType);
}

/*************************************************************************
 * Function Name: MenuGetEvent
 * Parameters: MENU_EVENT_TYPE_DEF * MenuEvent
 * Return: Boolean
 *    MENU_EVET_STACK_EMPTY       0: Event stack is empty not valid data
 *    MENU_EVET_STACK_DATA_VALID  1: Event stack is not empty valid data
 *
 * Description: Pop menu event from stack
 *
 *************************************************************************/
Boolean MenuGetEvent (MENU_EVENT_TYPE_DEF * MenuEvent)
{
  if (EventStack.Pop == EventStack.Push)
  {
    return MENU_EVET_STACK_EMPTY;
  }
  /* Function MenuSetEvent is reentrant */
  *MenuEvent = *EventStack.Pop;
  __disable_interrupt();
  if (++EventStack.Pop == EventStack.Push)
  {
    MenuStackInit();
  }
  __enable_interrupt();
  return MENU_EVET_STACK_DATA_VALID;
}

/*************************************************************************
 * Function Name: MenuSetEvent
 * Parameters: MENU_EVENT_TYPE_DEF MenuEvent
 * Return:  MENU_ERROR_CODE_DEF
 *          MENU_OK                   0: Push Event to stack
 *          MENU_EVET_STACK_OVERFLOW  3: Stack overflow
 *
 * Description: Push menu event to stack
 *
 *************************************************************************/
MENU_ERROR_CODE_DEF MenuSetEvent (MENU_EVENT_TYPE_DEF MenuEvent)
{
  if (EventStack.Push == &EventStack.EventStackBuffer[MENU_EVENT_STACK_SIZE])
  {
    return MENU_EVET_STACK_OVERFLOW;
  }
  *EventStack.Push = MenuEvent;
  ++EventStack.Push;
  return MENU_OK;
}

/*************************************************************************
 * Function Name: Menu
 * Parameters: none
 * Return: MENU_ERROR_CODE_DEF
 *        MENU_OK                   0: Push Event to stack
 *        MENU_EVET_STACK_OVERFLOW  3: Stack overflow
 *
 * Description: Implement menu function, pop menu event and determinate actions,
 * set new visual chain, call to User function ..
 *
 *************************************************************************/
MENU_ERROR_CODE_DEF Menu ()
{
MENU_EVENT_TYPE_DEF MenuEvent;
MENU_ACTION_SHAIN_DEF * pMenuActionChain = (MENU_ACTION_SHAIN_DEF *)MenuActionChainAdd[MenuInd];
MEMU_STING_DEF DataStr[MENU_MAX_STR_LENG]="";
MENU_ANIMATION_TYPE_DEF AnimType;
MEMU_STING_DEF * pDestData = DataStr;
const MEMU_STING_DEF * pSourceData;
Boolean ShowUpdate = FALSE;
MENU_ERROR_CODE_DEF Result = MENU_OK;
  if(MenuGetEvent(&MenuEvent) == MENU_EVET_STACK_DATA_VALID)
  {
    /* Menu time out implement */
    if (MenuEvent == MENU_TICK_EVENT)
    {
      Result = MenuAnimation();
      if(MenuAnimationStep)
      {
        return Result;
      }
      if (MenuTO)
      {
        if(--MenuTO)
        {
          return Result;
        }
      }
      else
      {
        return Result;
      }
    }
    /* Menu action  time out implement */
    while(pMenuActionChain->EventType)
    {
      if(pMenuActionChain->EventType != MenuEvent)
      {
        ++pMenuActionChain;
      }
      else
      {
        break;
      }
    }
    if (pMenuActionChain->MenuIndex)
    {
      if (MenuInd != pMenuActionChain->MenuIndex)
      {
        ShowUpdate = TRUE;
        MenuInd = pMenuActionChain->MenuIndex;
      }
    }
    if(pMenuActionChain->pVarArg != NULL)
    {
      ShowUpdate = TRUE;
      pSourceData = pMenuActionChain->pVarArg;
      while(*pSourceData)
      {
        *pDestData=*pSourceData;
        ++pSourceData;++pDestData;
      }
    }
    if (pMenuActionChain->MenuTO != MENU_TIME_OUT_NOT_UPADATE)
    {
      MenuTO = pMenuActionChain->MenuTO;
    }
    AnimType = pMenuActionChain->AnimationType;
    UserFunc1Hold = pMenuActionChain->UserFunc1;
    if(pMenuActionChain->UserFunc != NULL)
    {
      ShowUpdate = TRUE;
      pDestData = DataStr;
      pMenuActionChain->UserFunc(&pDestData,&MenuInd,&MenuTO,&AnimType);
    }
    if (ShowUpdate)
    {
      Result = MenuShow(pDestData,AnimType);
    }
  }
  return Result;
}

/*************************************************************************
 * Function Name: MenuGetInd
 * Parameters: none
 * Return: MENU_EVENT_TYPE_DEF
 *
 * Description: Return current menu index
 *
 *************************************************************************/
MENU_EVENT_TYPE_DEF MenuGetInd(void)
{
  return MenuInd;
}

/*************************************************************************
 * Function Name: MenuGetActionChain
 * Parameters: MEMU_IND_DEF MenuInd
 * Return: MENU_ACTION_SHAIN_DEF *
 *
 * Description: Return pointer to structure of action chain
 *
 *************************************************************************/
MENU_ACTION_SHAIN_DEF * MenuGetActionChain(MEMU_IND_DEF MenuInd)
{
  if (MenuInd > MENU_MAX_INDEX-1)
  {
    return (void *)0;
  }
  return (MENU_ACTION_SHAIN_DEF *)MenuActionChainAdd[MenuInd];
}

/*************************************************************************
 * Function Name: MenuGetVisualChain
 * Parameters:  MENU_LANG_TYPE_DEF Language,
 *              MEMU_IND_DEF MenuInd
 * Return:      MENU_VISUAL_DEF *
 *
 * Description: Return pointer to structure of visual chain
 *
 *************************************************************************/
MENU_VISUAL_DEF * MenuGetVisualChain(MENU_LANG_TYPE_DEF Language,MEMU_IND_DEF MenuInd)
{
  if ((MenuInd < MENU_MAX_INDEX) && (Language < MENU_LANGUAGE_NUMBER))
  {
    return (MENU_VISUAL_DEF *)(*MenuLanguageBaseAddTable[Language]+MenuInd);
  }
  return (MENU_VISUAL_DEF *)0;
}

/*************************************************************************
 * Function Name: MenuAnimation
 * Parameters: none
 *
 * Return: MENU_ERROR_CODE_DEF
 *
 * Description: makes animation
 *
 *************************************************************************/
MENU_ERROR_CODE_DEF MenuAnimation (void)
{
static Int16U i = 0;
MENU_ERROR_CODE_DEF Result = MENU_OK;

  if(++i < MenuAnimationSpeed)
  {
    return Result;
  }
  i = 0;
  if(MenuAnimationStep == 0)
  {
    return Result;
  }
  --MenuAnimationStep;
  switch (AnimationModeHold)
  {
  case MEMU_ANIMATION_LEFT:
    memmove(StrLine1Prev+1,StrLine1Prev,HD44780_HORIZONTAL_SIZE-1);
    StrLine1Prev[HD44780_HORIZONTAL_SIZE] = 0;
    StrLine1Prev[0] = StrLine1Next[MenuAnimationStep];
    memmove(StrLine2Prev+1,StrLine2Prev,HD44780_HORIZONTAL_SIZE-1);
    StrLine2Prev[HD44780_HORIZONTAL_SIZE] = 0;
    StrLine2Prev[0] = StrLine2Next[MenuAnimationStep];
    break;
  case MEMU_ANIMATION_RIGHT:
    memmove(StrLine1Prev,StrLine1Prev+1,HD44780_HORIZONTAL_SIZE-1);
    StrLine1Prev[HD44780_HORIZONTAL_SIZE-1] = StrLine1Next[HD44780_HORIZONTAL_SIZE-1 - MenuAnimationStep];
    memmove(StrLine2Prev,StrLine2Prev+1,HD44780_HORIZONTAL_SIZE-1);
    StrLine2Prev[HD44780_HORIZONTAL_SIZE-1] = StrLine2Next[HD44780_HORIZONTAL_SIZE-1 - MenuAnimationStep];
    break;
  default:
    strcpy(StrLine1Prev,StrLine1Next);
    strcpy(StrLine2Prev,StrLine2Next);
  }
  MEMU_STING_DEF *pData = StrLine1Prev;
  for(Int8U Y = 1; Y < 3; ++Y)
  {
    switch (HD44780_StrShow(1,Y,(Int8S *)pData))
    {
    case HD44780_BUSY_TO_ERROR:
    case HD44780_ERROR:
      Result =  MENU_SHOW_ERROR;
    case HD44780_OUT_OF_VISUAL:
      Result = MENU_SHOW_OUT_OF_VISUAL;
    }
    pData = StrLine2Prev;
  }

  if(MenuAnimationStep == 0)
  {
    if(UserFunc1Hold != NULL)
    {
      UserFunc1Hold(NULL,&MenuInd,&MenuTO,NULL);
    }
    return Result;
  }

  return(Result);
}

/*************************************************************************
 * Function Name: MenuInitAnimation
 * Parameters: Int8U X, Int8U Y,
 *             MEMU_STING_DEF *pData,
 *             MENU_ANIMATION_TYPE_DEF AnimationMode
 *
 * Return: HD44780_ERROR_CODE_DEF
 *
 * Description: Init the animation
 *
 *************************************************************************/
void  MenuInitAnimation(Int8U X, Int8U Y,MEMU_STING_DEF *pData, MENU_ANIMATION_TYPE_DEF AnimationMode)
{
  if(AnimationEnableFlag == FALSE)
  {
    AnimationMode = MEMU_ANIMATION_DIS;
  }
  AnimationModeHold = AnimationMode;
  switch (AnimationMode)
  {
  case MEMU_ANIMATION_LEFT:
  case MEMU_ANIMATION_RIGHT:
    MenuAnimationStep = HD44780_HORIZONTAL_SIZE;
    break;
  default:
    MenuAnimationStep = 1;
  }
  if(Y == 1)
  {
    strcpy(StrLine1Next,StrLine1Prev);
    for(;*pData; ++X)
    {
      StrLine1Next[X-1] = *pData++;
    }
  }
  else
  {
    strcpy(StrLine2Next,StrLine2Prev);
    for(; *pData; ++X)
    {
      StrLine2Next[X-1] = *pData++;
    }
  }
}

/*************************************************************************
 * Function Name: MenuGetAnimMode
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Get current animation mode
 *
 *************************************************************************/
Boolean MenuGetAnimMode (void)
{
  return AnimationEnableFlag;
}

/*************************************************************************
 * Function Name: MenuSetAnimMode
 * Parameters: Boolean Flag
 *
 * Return: none
 *
 * Description: Set animation mode
 *
 *************************************************************************/
void MenuSetAnimMode (Boolean Flag)
{
  AnimationEnableFlag = Flag;
}


/*************************************************************************
 * Function Name: MenuGetAnimSpeed
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Get current animation speed
 *
 *************************************************************************/
Int32U MenuGetAnimSpeed (void)
{
  return MenuAnimationSpeed;
}

/*************************************************************************
 * Function Name: MenuSetAnimSpeed
 * Parameters: Int32U Speed
 *
 * Return: none
 *
 * Description: Set animation mode
 *
 *************************************************************************/
void MenuSetAnimSpeed (Int32U Speed)
{
   MenuAnimationSpeed = Speed;
}

/*************************************************************************
 * Function Name: MenuGetAnimationProgress
 * Parameters: none
 *
 * Return:  Int16S
 *
 * Description: Return current animation steep
 *
 *************************************************************************/
Int16S MenuGetAnimationProgress (void)
{
   return MenuAnimationStep;
}
