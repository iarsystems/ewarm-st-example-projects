/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This file contains ADC12 driver example.
********************************************************************************
* History:
* 24/05/05 : V3.0
* 30/11/04 : V2.0
* 16/05/03 : Created
********************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#include "71x_lib.h"

u32 value,var;
u16 Conv_Res;

/*******************************************************************************
* Function Name  : ADC12_Calib
* Description    : This is used for ADC12 result calibration
* Input          : Conv_Res : Conversion result  12 bit two complement
* Output         : None
* Return         : Conversion result calibrated 12 bit unsigned value.
*******************************************************************************/
u16 ADC12_Calib(u16 Conv_Res)
{
 if ((Conv_Res & 0x800) == 0x800)
     return (0x800 - (((~ Conv_Res)&0xFFF) +1));
 else
  {
   if ((Conv_Res & 0x800) == 0)
     return (Conv_Res + 0x800);
   else return 0;
  }
}

/*************************************************************************/

void Generic_Wait(u32 var)
{
  u32 i;
  for( i=0;i<=var;i++);
}
/***************************************************************************/

int main (void)
{

 #ifdef DEBUG
  debug();

 #endif /* DEBUG */
  /* GPIO peripheral configuration ------------------------------------------*/

  /*  Configure the used analog input channel to HI_AIN_TRI */
  GPIO_Config (GPIO1, 0x0001, GPIO_HI_AIN_TRI);

  /*  Configure  P1.8 pin to output push-pull */
  GPIO_Config (GPIO1, 0x0100, GPIO_OUT_PP);

  /* ADC12 peripheral configuration  ----------------------------------------*/

  /*  Initialize the converter register. */
  ADC12_Init();

  /*  Configure the prescaler register using the configured PCLK with
   a sampling frequency=100Hz */
  ADC12_PrescalerConfig(100);


  /*  Select the conversion mode=single channel */
  ADC12_ModeConfig (ADC12_SINGLE);

  /*  Select the channel to be converted */
  ADC12_ChannelSelect(ADC12_CHANNEL0);

  /*  Start the Converter */
  ADC12_ConversionStart();

  while(1)
{

  /*  Wait until the availabiliy of data in the specific flags */
  while (ADC12_FlagStatus (ADC12_DA0)==RESET);

  /*  Get the conversion result from the correponding Data register */
  Conv_Res = ADC12_ConversionValue(ADC12_CHANNEL0);

  /*  Get the conversion result calibrated */
  value=ADC12_Calib(Conv_Res);

 if (!GPIO_BitRead(GPIO1,9))
    {
//use the potentiometer to change the Luminosity of the Led
      GPIO_BitWrite(GPIO1,8,1);
      Generic_Wait(0xFFF-value );
      GPIO_BitWrite(GPIO1,8,0);
      Generic_Wait(value);
     }
// press The But Button to blink the Led.
 else
     {
          do
         {
        Generic_Wait(0xFFFF);
       GPIO_BitWrite(GPIO1,8,~GPIO_BitRead(GPIO1, 8));

         }
       while(GPIO_BitRead(GPIO1, 9)==0);

 GPIO_BitWrite(GPIO1,8,0);
 Generic_Wait(0x1FFFF);


  }
}
}

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
	
	
	
	
	
	




