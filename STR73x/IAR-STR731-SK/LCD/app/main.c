/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : Nov. 2, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 * Jumpers:
 *  EXT/JLINK - depend of power source
 *  M0.L/M0.H - 2-3 (M0.L)
 *  M1.L/M1.H - 2-3 (M1.L)
 *  R-T       - Absence
 *
 * Buttons:
 *  B1 - Left/Decrement
 *  B2 - Right/Increment
 *  B3 - Select/OK
 *  B4 - Exit/Cancel
 *
 * LEDs:
 *  L1 - Alarm Maximum
 *  L2 - Alarm Minimum
 *  L3 - Warning Maximum
 *  L4 - Warning Minimum
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM to develop
 * code for the IAR STR731-KS evaluation boards. It demonstrates temperature measurement
 * with a NTC sensor.
 *
 *    $Revision: 39 $
 *
**************************************************************************/

#include "includes.h"
#include "user_func.h"

#define DLY_100US  80

extern void ARMIRQ_Enable (void);   /* Imported from 73x_init.s Assembler file */
extern void ARMFIQ_Enable (void);   /* Imported from 73x_init.s Assembler file */
extern void ARMIRQ_Disable (void);  /* Imported from 73x_init.s Assembler file */
extern void ARMFIQ_Disable (void);  /* Imported from 73x_init.s Assembler file */


const Int8S SelectChar[] =
{
  0x00,
  0x00,
  0x00,
  0x01,
  0x02,
  0x14,
  0x08,
  0x00,
};

const Int8S CancelChar[] =
{
  0x00,
  0x00,
  0x11,
  0x0A,
  0x04,
  0x0A,
  0x11,
  0x00,
};


const Int8S DegreeChar[] =
{
  0x00,
  0x04,
  0x0A,
  0x04,
  0x00,
  0x00,
  0x00,
  0x00,
};

Boolean SysTickFl = 0;

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 *
 * Return: none
 *
 * Description: Delay ~ (arg * 100us)
 *
 *************************************************************************/
void DelayResolution100us(Int32U Dly)
{
  for(; Dly; Dly--)
  {
    for(volatile Int32U j = DLY_100US; j; j--)
    {
    }
  }
}

/*************************************************************************
 * Function Name: UserStart
 * Parameters: none
 * Return: none
 *
 * Description: User Startup subroutine
 *		
 *************************************************************************/
void UserStart(void)
{
RTC_InitTypeDef   RTC_InitStructure;

  /* Init RTI Timer */
  /* RTC Configuration */
  RTC_DeInit();
  RTC_InitStructure.RTC_Alarm   = 0xFFFFFFFF;
  RTC_InitStructure.RTC_Counter = 0xFFFFFFFA;
  RTC_InitStructure.RTC_Prescaler = RTC_FREQ/TICK_PER_SECOND; /* configure RTC =0.02S */
  RTC_Init (&RTC_InitStructure);

  /* Register the Interrupt function */
  EIC_IRQChannelConfig( RTC_IRQChannel , ENABLE );
  EIC_IRQChannelPriorityConfig( RTC_IRQChannel, 5);

  /* RTC Interrupt Config */
  RTC_ITConfig (RTC_IT_SEC, ENABLE);

  /* HD44780 LCD driver init */
  HD44780_PowerUpInit();

  /* Buttons Init */
  ButtonsInit();

  /* Light Init */
  LightInit();

  /* Menu Init */
  MenuInit(MENU_ENGLISH_LANG,MENU_WELCOME_SHOW,NULL,MENU_WELCOM_DLY,MEMU_ANIMATION_LEFT);

  /* Init custom chars */
  HD44780_WrCGRAM(SelectChar,0);
  HD44780_WrCGRAM(DegreeChar,1);
  HD44780_WrCGRAM(CancelChar,2);

  /* Init action state*/
  InitAction(FALSE,MIN_TEMP,MAX_TEMP,FALSE,MIN_TEMP,MAX_TEMP);

  /* Adc init */
  InitTermMeasure();
}

/*************************************************************************
 * Function Name: main
 * Parameters: void
 * Return: int
 *
 * Description: Main subroutine
 *		
 *************************************************************************/
int main()
{
CMU_InitTypeDef    CMU_InitStructure;
PRCCU_InitTypeDef  PRCCU_InitStructure;
EIC_InitTypeDef    EIC_InitStructure;
Int32U i;

  /* disable global interrupts */
  /* disable global interrupts */
  ARMIRQ_Disable ();  /* Imported from 73x_init.s Assembler file */
  ARMFIQ_Disable ();  /* Imported from 73x_init.s Assembler file */
  /* Map flash memory on 0x00000000 */
  CFG_RemapConfig(CFG_MEM_FLASH);
  /* Clock set */
  /* CMU Initialization */
  CMU_DeInit();
  CMU_StructInit(&CMU_InitStructure);
  /* Enable the external oscillator */
  CMU_InitStructure.CMU_CKSEL0 = CMU_CKSEL0_CKOSC;
  CMU_InitStructure.CMU_CKSEL2 = CMU_CKSEL2_CKOSC;
  CMU_Init(&CMU_InitStructure);
  /* PRCCU Initialization */
  PRCCU_DeInit();
  PRCCU_StructInit(&PRCCU_InitStructure);
  /* Set PLL frequency OSC*16/2*2 = 16MHz */
  PRCCU_InitStructure.PRCCU_DIV2        = ENABLE;
  PRCCU_InitStructure.PRCCU_MCLKSRC_SRC = PRCCU_MCLKSRC_PLL;
  PRCCU_InitStructure.PRCCU_PLLDIV      = PRCCU_PLLDIV_2;
  PRCCU_InitStructure.PRCCU_PLLMUL      = PRCCU_PLLMUL_16;
  PRCCU_InitStructure.PRCCU_FREEN       = DISABLE;
  PRCCU_Init(&PRCCU_InitStructure);

  PRCCU_SetExtClkDiv(4);

  /* EIC Initialization */
  /* EIC Clock Enable */
  EIC_DeInit();
  /* EIC Config */
  EIC_StructInit(&EIC_InitStructure);
  EIC_Init (&EIC_InitStructure);

  /* Enable interrupts. */
  EIC_IRQCmd(ENABLE);
  ARMIRQ_Enable ();

  /* User Startup */
  UserStart();

  while(1)
  {
    if(SysTickFl)
    {
      SysTickFl = 0;
      MenuSetEvent(MENU_TICK_EVENT);
      /* Buttons Implement */
      if(Buttons())
      {
        ResetLightTO();
      }
      /* Light Implement */
      Light();
      /* Action Implement */
      Action();
      /* Temperature is calculate one time per second */
      if(++i > 1*TICK_PER_SECOND)
      {
        i = 0;
        SetTemp(NtcRatioToTemperature((Flo32)MeasureTerm()*GetCalX()/1024));
        /* Wait until animation end */
        if((MenuGetAnimationProgress() == 0) && (MenuGetInd() == MENU_TEMP_SHOW))
        {
          MenuSetEvent(MENU_EVENT_DATA_UPDATE);
        }
      }
    }
    /* Menu Implement */
    Menu();
  }
}
