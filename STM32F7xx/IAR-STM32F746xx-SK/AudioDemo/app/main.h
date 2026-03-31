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

#define QUAD_OUT_FAST_READ_CMD               0x6B
#define WRITE_ENABLE_CMD                     0x06
#define READ_STATUS1_REG_CMD                 0x05
#define READ_STATUS2_REG_CMD                 0x35
#define WRITE_STATUS_REG_CMD                 0x01

/* Size of buffers */
#define BUFFERSIZE                 (COUNTOF(aTxBuffer) - 1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)        (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

#define QSPI_FLASH_SIZE                      20

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
