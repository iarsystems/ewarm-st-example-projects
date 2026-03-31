/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : audio_class.c
 *    Description : AUDIO CLASS module
 *
 *    History :
 *    1. Date        : February 28, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
**************************************************************************/
#include "audio_class.h"

volatile Boolean SempEna,MicEna;
Int32U  SempPeriod,DeltaPer,MicCurrBuffer;
volatile pInt16S pSpkData,pMicData;
volatile Int32U  SempPerCurrHold,Delta,MicSempCount,SempCount;
volatile int tt[100];
volatile int count = 0;

static union ValUnion
{
  Int32S Data;
  struct
  {
    Int16U DataLo;
    Int16S DataHi;
  };
} Val = {0x02000000};


const Int32U VolumeMul [] =
{
  // 256 - 0
  //-48dB- 0x0
  261,
  //-47dB- 0x1
  293,
  //-46dB- 0x2
  328,
  //-45dB- 0x3
  369,
  //-44dB- 0x4
  414,
  //-43dB- 0x5
  464,
  //-42dB- 0x6
  521,
  //-41dB- 0x7
  584,
  //-40dB- 0x8
  655,
  //-39dB- 0x9
  735,
  //-38dB- 0xA
  825,
  //-37dB- 0xB
  926,
  //-36dB- 0xC
  1039,
  //-35dB- 0xD
  1165,
  //-34dB- 0xE
  1308,
  //-33dB- 0xF
  1467,
  //-32dB- 0x10
  1646,
  //-31dB- 0x11
  1847,
  //-30dB- 0x12
  2072,
  //-29dB- 0x13
  2325,
  //-28dB- 0x14
  2609,
  //-27dB- 0x15
  2927,
  //-26dB- 0x16
  3285,
  //-25dB- 0x17
  3685,
  //-24dB- 0x18
  4135,
  //-23dB- 0x19
  4640,
  //-22dB- 0x1A
  5206,
  //-21dB- 0x1B
  5841,
  //-20dB- 0x1C
  6554,
  //-19dB- 0x1D
  7353,
  //-18dB- 0x1E
  8250,
  //-17dB- 0x1F
  9257,
  //-16dB- 0x20
  10387,
  //-15dB- 0x21
  11654,
  //-14dB- 0x22
  13076,
  //-13dB- 0x23
  14672,
  //-12dB- 0x24
  16462,
  //-11dB- 0x25
  18471,
  //-10dB- 0x26
  20724,
  //-9dB - 0x27
  23253,
  //-8dB - 0x28
  26090,
  //-7dB - 0x29
  29274,
  //-6dB - 0x2A
  32846,
  //-5dB - 0x2B
  36854,
  //-4dB - 0x2C
  41350,
  //-3dB - 0x2D
  46396,
  //-2dB - 0x2E
  52057,
  //-1dB - 0x2F
  58409,
  // 0dB - 0x30
  65536,
  // 1dB - 0x31
  73533,
  // 2dB - 0x32
  82505,
  // 3dB - 0x33
  92572,
  // 4dB - 0x34
  103868,
  // 5dB - 0x35
  116541,
  // 6dB - 0x36
  130762,
};

#pragma data_alignment=4
__no_init Int16S AudioSpkData[SampRerFrame * 3];

#pragma data_alignment=4
__no_init Int16S AudioMicData1[SampRerFrame * 2];
#pragma data_alignment=4
__no_init Int16S AudioMicData2[SampRerFrame * 2];

#pragma data_alignment=4
Int8U AudioBuf[2];

Int8U   AudioRequest,AudioCS,AudioCN,AudioId;
Int16U  AudioDataSize;

Int16S  AudioFeat1Vol;
Int32U  AudioSpkVolMul;
Boolean AudioFeat1Mute;

Int16S  AudioFeat2Vol;
Int32U  AudioMicVolMul;
Boolean AudioFeat2Mute;


/*************************************************************************
 * Function Name: AudioClassInit
 * Parameters: Int32U IntPrio
 *
 * Return: none
 *
 * Description: USB Class Audio Init
 *
 *************************************************************************/
void AudioClassInit (Int32U IntPrio)
{
TIM_InitTypeDef TIM_InitStructure;
GPIO_InitTypeDef  GPIO_InitStructure;
ADC_InitTypeDef   ADC_InitStructure;
Int32U Pclk;

  // Init Audio Class variables
  SempEna         =\
  AudioFeat1Mute  =\
  AudioFeat2Mute  = FALSE;
  SempCount       =\
  AudioFeat1Vol   =\
  AudioFeat2Vol   = 0;
  pSpkData        = AudioSpkData;
  SempPerCurrHold = SempPeriod;

  // Registered The Class Request
  USB_UserFuncRegistering(UsbClassAudioRequest,UsbUserClass);
  // Registered User Ep0 Data receive
  USB_UserFuncRegistering(UsbClassAudioData,UsbClassEp0OutPacket);

  // PWM DAC (TIM3) 10 bit GPIO4.6
  // Enable GPIO clocks
  SCU_APBPeriphClockConfig(__GPIO4, ENABLE);
  // Release GPIO reset
  SCU_APBPeriphReset(__GPIO4, DISABLE);

  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt2;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIO4, &GPIO_InitStructure);

  // Enable TIM23 clocks
  SCU_APBPeriphClockConfig(__TIM23, ENABLE);
  // Release TIM23 reset
  SCU_APBPeriphReset(__TIM23,DISABLE);
  // Timer 3
  // TIM Configuration in PWM GPIO 4.6
  TIM_InitStructure.TIM_Mode = TIM_PWM;             /* Timer mode                                    */
  TIM_InitStructure.TIM_OC1_Modes = TIM_WAVE;       /* Output Compare 1 Mode: Timing or Wave         */
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB; /* Timer Clock source APB/SCU/EXTERNAL           */
  TIM_InitStructure.TIM_Prescaler = 0;              /* Timer Prescaler factor                        */
  TIM_InitStructure.TIM_Pulse_Level_1 = TIM_LOW;    /* Level applied on the Output Compare Pin 1     */
  TIM_InitStructure.TIM_Period_Level = TIM_HIGH;    /* Level applied during the Period of a PWM Mode */
  TIM_InitStructure.TIM_Pulse_Length_1 = 0x200;     /* Pulse 1 Length used in Output Compare 1 Mode  */
  TIM_InitStructure.TIM_Full_Period = 0x3FF;        /* Period Length used in PWM Mode                */
  TIM_Init(TIM3, &TIM_InitStructure);

  // Clear TIM3 counter
  TIM_CounterCmd(TIM3, TIM_CLEAR);
  // Clear TIM3 flag OC2
  TIM_ClearFlag(TIM3,TIM_IT_OC2);
  // Disable all TIM3 interrupts
  TIM_ITConfig(TIM3,
               TIM_IT_OC1 | TIM_IT_IC1 |
               TIM_IT_IC2 | TIM_IT_OC2 |
               TIM_IT_TO,
               DISABLE);
  // Enable TIM3 counter
  TIM_CounterCmd(TIM3, TIM_START);

  // ADC init GPIO 4.7
  // Enable ADC clocks
  SCU_APBPeriphClockConfig(__ADC, ENABLE);
  // Release ADC reset
  SCU_APBPeriphReset(__ADC, DISABLE);
  // Assign P4.7 to ADC ch 7
  GPIO_ANAPinConfig(GPIO_ANAChannel7, ENABLE);
  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Channel_7_Mode = ADC_NoThreshold_Conversion;
  ADC_InitStructure.ADC_Select_Channel = ADC_Channel_7;
  ADC_InitStructure.ADC_Scan_Mode = DISABLE;
  ADC_InitStructure.ADC_Conversion_Mode = ADC_Single_Mode;
  // Enable the ADC
  ADC_Cmd(ENABLE);
  // Prescaler configure
  ADC_PrescalerConfig(0x2);
  // Configure the ADC
  ADC_Init(&ADC_InitStructure);
  // ADC interrupt configure
  ADC_ITConfig(ADC_IT_ECV | ADC_IT_AWD, DISABLE);

  // Init Tick Timer - TIM2
  // Timer 2
  // TIM Configuration in Output Compare Timing Mode
  TIM_InitStructure.TIM_Mode = TIM_OCM_CHANNEL_1;   // OUTPUT COMPARE CHANNEL 1 Mode
  TIM_InitStructure.TIM_OC1_Modes = TIM_TIMING;     // OCMP1 pin is disabled
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB; // assign PCLK to TIM_Clk
  TIM_InitStructure.TIM_Prescaler = 0;              // max resolution
  Pclk = SCU_GetPCLKFreqValue()*1000;
  SempPeriod = Pclk/SampFreq;
  DeltaPer = SempPeriod/(SampRerFrame*2);
  TIM_InitStructure.TIM_Pulse_Length_1 = SempPeriod;// period
  TIM_Init(TIM2, &TIM_InitStructure);

  // Clear TIM2 counter
  TIM_CounterCmd(TIM2, TIM_CLEAR);
  // Clear TIM2 flag OC1
  TIM_ClearFlag(TIM2,TIM_FLAG_OC1);
  // Enable TIM2 OC1 interrupt
  TIM_ITConfig(TIM2, TIM_IT_OC1, ENABLE);

  // VIC configuration
  VIC_Config(TIM2_ITLine, VIC_IRQ, IntPrio);
  VIC_ITCmd(TIM2_ITLine, ENABLE);

  // Enable TIM2 counter
  TIM_CounterCmd(TIM2, TIM_START);

}

/*************************************************************************
 * Function Name: UsbClassAudioConfigure
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: USB Class Audio configure
 *
 *************************************************************************/
void * UsbClassAudioConfigure (void * pArg)
{
UsbDevCtrl_t * pUsbDevCtrl = (UsbDevCtrl_t *)pArg;
  if(pUsbDevCtrl == NULL)
  {
    if(UsbCoreReq(UsbCoreReqConfiquration) != 0)
    {
      MicEna = FALSE;
      // disable all class EPs
      USB_UserFuncRegistering(NULL,SpkEp);
      USB_RealizeEp((USB_Endpoint_t)SpkEp,
                     0,
                     EP_ISO,
                     SpkEpSlot);
      USB_UserFuncRegistering(NULL,MicEp);
      USB_RealizeEp((USB_Endpoint_t)MicEp,
                     0,
                     EP_ISO,
                     MicEpSlot);
    }
  }
  else
  {
    if((pUsbDevCtrl->Configuration == 1))
    {
      if(pUsbDevCtrl->Interface == 1)
      {
        if(pUsbDevCtrl->AlternateSetting == 0)
        {
          USB_UserFuncRegistering(NULL,SpkEp);
          USB_RealizeEp((USB_Endpoint_t)SpkEp,
                         0,
                         EP_ISO,
                         SpkEpSlot);
        }
        else if (pUsbDevCtrl->AlternateSetting == 1)
        {
          USB_UserFuncRegistering(AudioInHadler,SpkEp);
          USB_RealizeEp((USB_Endpoint_t)SpkEp,
                         SpkEpMaxSize,
                         EP_ISO,
                         SpkEpSlot);
        }
      }
      else if (pUsbDevCtrl->Interface == 2)
      {
        if(pUsbDevCtrl->AlternateSetting == 0)
        {
          MicEna = FALSE;
          USB_UserFuncRegistering(NULL,UsbUserSofEvent);
          USB_RealizeEp((USB_Endpoint_t)MicEp,
                         0,
                         EP_ISO,
                         MicEpSlot);
        }
        else if (pUsbDevCtrl->AlternateSetting == 1)
        {
          pMicData = AudioMicData1;
          MicCurrBuffer = 0;
          USB_UserFuncRegistering(AudioOutHadler,UsbUserSofEvent);
          USB_RealizeEp((USB_Endpoint_t)MicEp,
                         MicEpMaxSize,
                         EP_ISO,
                         MicEpSlot);
        }
      }
    }
  }
  return(NULL);
}

/*************************************************************************
 * Function Name: AudioInHadler
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: USB Class Audio Out EP handler
 *
 *************************************************************************/
void * AudioInHadler (void *Arg)
{
Int32U ReceievedBytes = SpkEpMaxSize;
volatile Int32U Status;

  Status = USB_EpRead((USB_Endpoint_t)SpkEp,
            (pInt8U)pSpkData,
            &ReceievedBytes);
  if(Status != USB_OK)
  {
    Status = Status;
  }
  if(ReceievedBytes)
  {
    pSpkData += ReceievedBytes/sizeof(Int16U);
    if(pSpkData >= AudioSpkData + sizeof(AudioSpkData)/sizeof(Int16U))
    {
      pSpkData = AudioSpkData;
    }

    if(SempEna)
    {
      // Adapt sample rate
      if (Delta > (SampRerFrame * SubFrameSize))
      {
        SempPerCurrHold = SempPeriod - DeltaPer;
      }
      else if (Delta < (SampRerFrame * SubFrameSize))
      {
        SempPerCurrHold = SempPeriod + DeltaPer;
      }
      else
      {
        SempPerCurrHold = SempPeriod;
      }
    }
    else
    {
      if(pSpkData >= (AudioSpkData + (2*sizeof(AudioSpkData))/(3*sizeof(Int16U))))
      {
        // Enable output
        SempCount = 0;
        SempEna   = TRUE;
      }
    }
    Int32U Save = __get_interrupt_state();
    __disable_interrupt();
    Delta  += ReceievedBytes;
    __set_interrupt_state(Save);
  }
  return(NULL);
}

/*************************************************************************
 * Function Name: AudioOutHadler
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: USB Class Audio In EP handler
 *
 *************************************************************************/
void * AudioOutHadler (void *Arg)
{
Int32U PacketLength;
Int32U Save = __get_interrupt_state();
  __disable_interrupt();
  MicEna = TRUE;
  // Set pointer to free buffer
  if(++MicCurrBuffer & 1)
  {
    PacketLength = (Int32U)pMicData - (Int32U)AudioMicData1;
    pMicData = AudioMicData2;
  }
  else
  {
    PacketLength = (Int32U)pMicData - (Int32U)AudioMicData2;
    pMicData = AudioMicData1;
  }

  __set_interrupt_state(Save);
  // Send microphone output buffer
  USB_EpWrite((USB_Endpoint_t)MicEp,
              (pInt8U)((MicCurrBuffer & 1)?AudioMicData1:AudioMicData2),
              &PacketLength);
  return(NULL);
}

/*************************************************************************
 * Function Name: Tim2Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 2 interrupt handler
 *		
 *************************************************************************/
__ramfunc __arm
void Tim2Handler (void)
{
union ValUnion MicTemp;
Int32U  IntrState;

  IntrState =__get_CPSR();
  __set_CPSR(IntrState | 0x80);
  IntrState &= 0x80;

  if (SempEna)
  {
    if(Delta > 1)
    {
      // Get next input sample
      Val.Data = AudioSpkData[SempCount++];
      Delta   -= 2;
      // Correct pointer to sample buffer
      if(SempCount >= sizeof(AudioSpkData)/sizeof(Int16U))
      {
        SempCount = 0;
      }
      if(!AudioFeat1Mute && AudioSpkVolMul)
      {
        // Apply volume
        Val.Data   *= AudioSpkVolMul;
        // Add offset
        Val.DataHi += 0x200;
        // Check for overflow and correct value
        if(Val.DataHi < 50)
        {
          Val.DataHi = 50;
        }
        else if(Val.DataHi > 975)
        {
          Val.DataHi = 975;
        }
      }
      else
      {
        // set middle of range
        Val.DataHi = 0x200;
      }
    }
    else
    {
      // Disable output stream after emptying the buffer
      SempEna = FALSE;
      SempPerCurrHold = SempPeriod;
    }
  }
  else
  {
    SempPerCurrHold = SempPeriod;
  }
  // Clear TIM3 flag OC2
  TIM3->SR &= ~TIM_FLAG_OC2;
  while(!(TIM3->SR & TIM_FLAG_OC2));
  TIM3->OC1R =  Val.DataHi;
  if(MicEna)
  {
    if(!AudioFeat2Mute && AudioMicVolMul)
    {
      // Get ADC sample and remove offset
      MicTemp.Data  = (ADC->DR7 & 0x3FF) - 0x200;
      // Start conversion
      ADC->CR |= 1;
      // Add volume
      MicTemp.Data *= AudioMicVolMul;
      // check for overflow and correct the result
      if(MicTemp.DataHi < -511)
      {
        MicTemp.Data = -511 * 65536;
      }
      else if(MicTemp.DataHi > 511)
      {
        MicTemp.Data = 511 * 65536;
      }
      // Scale to 16bit
      MicTemp.Data <<= 6;
    }
    else
    {
      // set middle of range
      MicTemp.DataHi = 0;
    }
    // write to output buffer
    *pMicData++ = MicTemp.DataHi;
  }
  TIM2->OC1R += SempPerCurrHold; // reload output compare
  TIM2->SR &= ~TIM_FLAG_OC1;     // clear interrupt flag
  IntrState = __get_CPSR() & ~0x80;
  __set_CPSR(IntrState);
}

/*************************************************************************
 * Function Name: AudioFeatureGetReg
 * Parameters:  Int32U CS, Int32U Id
 *
 * Return: Boolean
 *
 * Description:
 *
 *************************************************************************/
static Boolean AudioFeatureGetReg (Int32U CS, Int32U Id)
{
  switch (CS)
  {
  case REQUEST_GET_CUR:
    // Load current value of Volume in the transmit buffer
    if (Id == FeatUnit1Id)
    {
      AudioBuf[1] = (AudioFeat1Vol>>8)& 0xFF;
      AudioBuf[0] =  AudioFeat1Vol    & 0xFF;
    }
    else if (Id == FeatUnit2Id)
    {
      AudioBuf[1] = (AudioFeat2Vol>>8)& 0xFF;
      AudioBuf[0] =  AudioFeat2Vol    & 0xFF;
    }
    else
    {
      return(FALSE);
    }
    break;
  case REQUEST_GET_MIN:
    // Load minimum value of Volume in the transmit buffer
    if (Id == FeatUnit1Id)
    {
      AudioBuf[1] = (Feat1MinVol>>8)  & 0xFF;
      AudioBuf[0] =  Feat1MinVol      & 0xFF;
    }
    else if (Id == FeatUnit2Id)
    {
      AudioBuf[1] = (Feat2MinVol>>8)  & 0xFF;
      AudioBuf[0] =  Feat2MinVol      & 0xFF;
    }
    else
    {
      return(FALSE);
    }
    break;
  case REQUEST_GET_MAX:
    // Load maximum value of Volume in the transmit buffer
    if (Id == FeatUnit1Id)
    {
      AudioBuf[1] = (Feat1MaxVol>>8)  & 0xFF;
      AudioBuf[0] =  Feat1MaxVol      & 0xFF;
    }
    else if (Id == FeatUnit2Id)
    {
      AudioBuf[1] = (Feat2MaxVol>>8)  & 0xFF;
      AudioBuf[0] =  Feat2MaxVol      & 0xFF;
    }
    else
    {
      return(FALSE);
    }
    break;
  case REQUEST_GET_RES:
    // Load resolution value of Volume in the transmit buffer
    if (Id == FeatUnit1Id)
    {
      AudioBuf[1] = (Feat1ResVol>>8)  & 0xFF;
      AudioBuf[0] =  Feat1ResVol      & 0xFF;
    }
    else if (Id == FeatUnit2Id)
    {
      AudioBuf[1] = (Feat2ResVol>>8)  & 0xFF;
      AudioBuf[0] =  Feat2ResVol      & 0xFF;
    }
    else
    {
      return(FALSE);
    }
    break;
  default:
    return(FALSE);
  }
  return(TRUE);
}

/*************************************************************************
 * Function Name: UsbClassAudioRequest
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: USB Class Audio Requests
 *
 *************************************************************************/
void * UsbClassAudioRequest (void * pArg)
{
UsbEpCtrl_t * pAdioReqCtrl = (UsbEpCtrl_t *) pArg;
UsbSetupPacket_t * pAudioReqPacket = (UsbSetupPacket_t *)pAdioReqCtrl->pData;
  // Validate Request
  switch (pAudioReqPacket->mRequestType.Recipient)
  {
  case UsbRecipientInterface:
    // Feature Unit requests only Imlement for interface 0
    if ((pAudioReqPacket->wIndex.Word == (FeatUnit1Id << 8)) ||
        (pAudioReqPacket->wIndex.Word == (FeatUnit2Id << 8)))
    {
      // Request type
      switch (pAudioReqPacket->wValue.Hi)
      {
      case FU_MUTE_CONTROL:
        if ((pAudioReqPacket->bRequest == REQUEST_SET_CUR) &&
            (pAudioReqPacket->wLength.Word == 1))
        {
          // Set mute flag
          AudioRequest  = pAudioReqPacket->bRequest;
          AudioId       = pAudioReqPacket->wIndex.Hi;
          AudioCS       = pAudioReqPacket->wValue.Hi;
          AudioCN       = pAudioReqPacket->wValue.Lo;
          AudioDataSize = pAudioReqPacket->wLength.Word;
          pAdioReqCtrl->Counter= 1;
          pAdioReqCtrl->pData = AudioBuf;
          return((void *)UsbPass);
        }
        else if ((pAudioReqPacket->bRequest == REQUEST_GET_CUR) &&
                 (pAudioReqPacket->wLength.Word == 1))
        {
          // Read mute flag
          if(pAudioReqPacket->wIndex.Hi == FeatUnit1Id)
          {
            AudioBuf[0]  = AudioFeat1Mute;
          }
          else
          {
            AudioBuf[0]  = AudioFeat2Mute;
          }
          pAdioReqCtrl->Counter= 1;
          pAdioReqCtrl->pData = AudioBuf;
          return((void*)UsbPass);
        }
        break;
      case FU_VOLUME_CONTROL:
        if(pAudioReqPacket->bRequest & 0x80)
        {
          // Read different volume values
          if((pAudioReqPacket->wLength.Word == 2) &&
              AudioFeatureGetReg(pAudioReqPacket->bRequest,pAudioReqPacket->wIndex.Hi))
          {
            pAdioReqCtrl->Counter = 2;
            pAdioReqCtrl->pData = AudioBuf;
            return((void*)UsbPass);
          }
        }
        else if((pAudioReqPacket->bRequest == REQUEST_SET_CUR) &&
                (pAudioReqPacket->wLength.Word  == 2))
        {
          // Set volume value
          AudioRequest  = pAudioReqPacket->bRequest;
          AudioId       = pAudioReqPacket->wIndex.Hi;
          AudioCS       = pAudioReqPacket->wValue.Hi;
          AudioCN       = pAudioReqPacket->wValue.Lo;
          AudioDataSize = pAudioReqPacket->wLength.Word;
          pAdioReqCtrl->Counter = 2;
          pAdioReqCtrl->pData = AudioBuf;
          return((void *)UsbPass);
        }
        break;
      }
    }
    // Selector Unit requests only Imlement for interface 0
    else if (pAudioReqPacket->wIndex.Word == (SelUnit1ID << 8))
    {
      if(pAudioReqPacket->wValue.Word == 0)
      {
        // Read different selector unit values
        if(pAudioReqPacket->bRequest & 0x80)
        {
          if(pAudioReqPacket->wLength.Word == 1)
          {
            AudioBuf[0] = 1;
            pAdioReqCtrl->Counter = 1;
            pAdioReqCtrl->pData = AudioBuf;
            return((void*)UsbPass);
          }
        }
        else
        {
          // Set channel
          AudioRequest  = pAudioReqPacket->bRequest;
          AudioId       = pAudioReqPacket->wIndex.Hi;
          AudioDataSize = pAudioReqPacket->wLength.Word;
          pAdioReqCtrl->Counter = 1;
          pAdioReqCtrl->pData = AudioBuf;
          return((void *)UsbPass);
        }
      }
    }
    return((void *)UsbFault);
  case UsbRecipientEndpoint:
    return((void *)UsbFault);
  }
  return((void *)UsbFault);
}

/*************************************************************************
 * Function Name: UsbClassAudioData
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: USB Class Audio Data receive
 *
 *************************************************************************/
void * UsbClassAudioData (void * pArg)
{
  if((AudioId == FeatUnit1Id) ||
     (AudioId == FeatUnit2Id))
  {
    switch (AudioCS)
    {
    case FU_MUTE_CONTROL:
      // Set mute flag
      if (AudioId == FeatUnit1Id)
      {
        AudioFeat1Mute = AudioBuf[0];
      }
      else if (AudioId == FeatUnit2Id)
      {
        AudioFeat2Mute = AudioBuf[0];
      }
      else
      {
        return((void*)UsbFault);
      }
      break;
    case FU_VOLUME_CONTROL:
      // Set volume value
      if (AudioId == FeatUnit1Id)
      {
        AudioFeat1Vol = AudioBuf[0] + ((Int16U)AudioBuf[1]<<8);
        if((Int16U)AudioFeat1Vol == MinVol)
        {
          AudioSpkVolMul = 0;
        }
        else
        {
          if (AudioFeat1Vol < (Int16S)Feat1MinVol)
          {
            AudioFeat1Vol = Feat1MinVol;
          }
          else if(AudioFeat1Vol > (Int16S)Feat1MaxVol)
          {
            AudioFeat1Vol = Feat1MaxVol;
          }
          // -48dB - +6dB multiply
          AudioSpkVolMul = (VolumeMul[((AudioFeat1Vol>>8)+0x30) & 0x3F]) >> 6;
        }
      }
      else if (AudioId == FeatUnit2Id)
      {
        AudioFeat2Vol = AudioBuf[0] + ((Int16U)AudioBuf[1]<<8);
        if((Int16U)AudioFeat2Vol == MinVol)
        {
          AudioMicVolMul = 0;
        }
        else
        {
          if (AudioFeat2Vol < (Int16S)Feat2MinVol)
          {
            AudioFeat2Vol = Feat2MinVol;
          }
          else if(AudioFeat2Vol > (Int16S)Feat2MaxVol)
          {
            AudioFeat2Vol = Feat2MaxVol;
          }
          // -48dB - +6dB multiply
          AudioMicVolMul = VolumeMul[((AudioFeat2Vol>>8)+0x30) & 0x3F];
        }
      }
      else
      {
        return((void*)UsbFault);
      }
      break;
    default:
      return((void*)UsbFault);
    }
  }
  else if (AudioId == SelUnit1ID)
  {
    // empty
  }
  else
  {
    return((void*)UsbFault);
  }
  return((void*)UsbPass);
}
