/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : audio_class.h
 *    Description : AUDIO CLASS definitions
 *
 *    History :
 *    1. Date        : February 28, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
**************************************************************************/
#include <includes.h>

#ifndef __AUDIO_CLASS_H
#define __AUDIO_CLASS_H

#define SubFrameSize  2
#define SampRerFrame  16
#define SampFreq      (SampRerFrame KHZ)

#define MinVol        0x8000

#define Feat1MinVol   0xD000UL
#define Feat1MaxVol   0x0600UL
#define Feat1ResVol   0x0001UL

#define Feat2MinVol   0xD000UL
#define Feat2MaxVol   0x0600UL
#define Feat2ResVol   0x0001UL

typedef enum
{
  FeatUnit1Id = 1, FeatUnit2Id,
  SpkInTermID, SpkOutTermID,
  MicInTermID, MicOutTermID,
  SelUnit1ID,
} AudioID_t;

// Audio Class-Specific Request Codes
#define REQUEST_CODE_UNDEFINED        			0x00
#define REQUEST_SET_CUR               			0x01
#define REQUEST_GET_CUR               			0x81
#define REQUEST_SET_MIN               			0x02
#define REQUEST_GET_MIN               			0x82
#define REQUEST_SET_MAX               			0x03
#define REQUEST_GET_MAX               			0x83
#define REQUEST_SET_RES               			0x04
#define REQUEST_GET_RES               			0x84
#define REQUEST_SET_MEM               			0x05
#define REQUEST_GET_MEM               			0x85
#define REQUEST_GET_STAT              			0xFF
                                      			
// Terminal Control Selectors         			
#define TE_CONTROL_UNDEFINED          			0x00
#define TE_COPY_PROTECT_CONTROL       			0x01
                                      			
// Feature Unit Control Selectors     			
#define FU_CONTROL_UNDEFINED          			0x00
#define FU_MUTE_CONTROL               			0x01
#define FU_VOLUME_CONTROL             			0x02
#define FU_BASS_CONTROL               			0x03
#define FU_MID_CONTROL                			0x04
#define FU_TREBLE_CONTROL             			0x05
#define FU_GRAPHIC_EQUALIZER_CONTROL  			0x06
#define FU_AUTOMATIC_GAIN_CONTROL     			0x07
#define FU_DELAY_CONTROL              			0x08
#define FU_BASS_BOOST_CONTROL         			0x09
#define FU_LOUDNESS_CONTROL           			0x0A

// Endpoint Control Selectors
#define EP_CONTROL_UNDEFINED           			0x00
#define SAMPLING_FREQ_CONTROL         			0x01
#define PITCH_CONTROL                  			0x02

/*************************************************************************
 * Function Name: AudioClassInit
 * Parameters: Int32U IntPrio
 *
 * Return: none
 *
 * Description: USB Class Audio Init
 *
 *************************************************************************/
void AudioClassInit (Int32U IntPrio);

/*************************************************************************
 * Function Name: UsbClassAudioConfigure
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: USB Class Audio configure
 *
 *************************************************************************/
void * UsbClassAudioConfigure (void * pArg);

/*************************************************************************
 * Function Name: AudioInHadler
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: USB Class Audio Out EP handler
 *
 *************************************************************************/
void * AudioInHadler (void *Arg);

/*************************************************************************
 * Function Name: AudioOutHadler
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: USB Class Audio In EP handler
 *
 *************************************************************************/
void * AudioOutHadler (void *Arg);


/*************************************************************************
 * Function Name: TIM0_ISR
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *		
 *************************************************************************/
void TIM0_ISR (void);

/*************************************************************************
 * Function Name: AudioFeatureGetReg
 * Parameters:  Int32U CS, Int32U Id
 *
 * Return: Boolean
 *
 * Description:
 *
 *************************************************************************/
static Boolean AudioFeatureGetReg (Int32U CS, Int32U Id);

/*************************************************************************
 * Function Name: UsbClassAudioRequest
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: USB Class Audio Requests
 *
 *************************************************************************/
void * UsbClassAudioRequest (void * pArg);

/*************************************************************************
 * Function Name: UsbClassAudioData
 * Parameters:  void * pArg
 *
 * Return: void *
 *
 * Description: USB Class Audio Data receive
 *
 *************************************************************************/
void * UsbClassAudioData (void * pArg);

#endif //__AUDIO_CLASS_H
