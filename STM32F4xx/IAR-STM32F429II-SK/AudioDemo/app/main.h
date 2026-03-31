/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : main.h
 *    Description : Main module header
 *
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

typedef int16_t AudioChannelSample16b_t;

#pragma pack(1)
typedef struct 
{
  AudioChannelSample16b_t leftChannel;
  AudioChannelSample16b_t rightChannel;
} AudioSample16b_t;
#pragma pack()

typedef enum 
{
  joyIdle,
  joyUp,
  joyDown,
  joyLeft,
  joyRight,
  joyCenter
} JoystickState_t;

typedef enum 
{
  stPlayWave,
  stLoopbackMic,
  stLoopbackLineIn,
  stGenerateSine,
  NumberOfPlaybackStates
} PlaybackState_t;
