/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2014
 *
 *    File name   : main.h
 *    Description : Main module header
 *
 *    History :
 *    1. Date        : October 2014
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

/* application states */
typedef enum {
  stIdle = 0,
  stRecordingInit,
  stRecToneInit,
  stRecTone,
  stRecPauseInit,
  stRecPause,
  stRecSave,
  stPlaybackInit,
  stPlayChunkInit,
  stPlayChunk,
  stDoToneInit,
  stDoTone
} MCRState_TypeDef;

/* buttons states */
typedef enum {
  evNone = 0,
  evButtonTonePressed,
  evButtonPlayPressed,
  evButtonRecPressed,
  evButtonsReleased
} MCREvent_TypeDef;

/* recorded chunk types */
typedef enum {
  ctPause = 0x5D,
  ctTone,
  ctEnd
} ChunkType_TypeDef;

/* sound system states */
typedef enum {
  sndStop = 0,
  sndStart
} SoundState_TypeDef;

/* chunk structure */
#pragma pack(1)
typedef struct
{
  ChunkType_TypeDef type;
  uint32_t          time;
} Chunk_TypeDef;
#pragma pack()
