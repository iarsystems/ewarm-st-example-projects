/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : audio_desc.h
 *    Description : AUDIO descriptor definitions
 *
 *    History :
 *    1. Date        : November 29, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#ifndef __AUDIO_DESC_H
#define __AUDIO_DESC_H

#include "arm_comm.h"
#include "usb_desc.h"

typedef enum
{
  SUBCLASS_UNDEFINED = 0, AUDIOCONTROL, AUDIOSTREAMING,
  MIDISTREAMING,
} AudioInterfaceSubclass_t;

typedef enum
{
  PR_PROTOCOL_UNDEFINED = 0,
} AudioInterfaceProtocol_t;

typedef enum
{
  CS_UNDEFINED = 0x20,CS_DEVICE,CS_CONFIGURATION,CS_STRING,
  CS_INTERFACE,CS_ENDPOINT,
} AudioClassSpecificDescriptorTypes_t;

typedef enum
{
  AC_DESCRIPTOR_UNDEFINED = 0,HEADER,INPUT_TERMINAL,OUTPUT_TERMINAL,
  MIXER_UNIT,SELECTOR_UNIT,FEATURE_UNIT,PROCESSING_UNIT,EXTENSION_UNIT,
} AudioACInterfaceDescriptorSubtypes_t;

typedef enum
{
  AS_DESCRIPTOR_UNDEFINED = 0,AS_GENERAL,FORMAT_TYPE,FORMAT_SPECIFIC,
} AudioASInterfaceDescriptorSubtypes_t;

typedef enum
{
  DESCRIPTOR_UNDEFINED = 0,EP_GENERAL,
} AudioASEndpoinDescriptorSubtypes_t;

typedef enum
{
  FORMAT_TYPE_UNDEFINED = 0,FORMAT_TYPE_I,FORMAT_TYPE_II,FORMAT_TYPE_III,
} AudioFormatTypeCodes_t;

typedef enum
{
  TYPE_I_UNDEFINED = 0,PCM,PCM8,IEEE_FLOAT,ALAW,MULAW,
} AudioDataFormatTypeI_t;

typedef enum
{
  TYPE_II_UNDEFINED = 0x1000,MPEG,AC_3,
} AudioDataFormatTypeII_t;

typedef enum
{
  TYPE_III_UNDEFINED = 0x2000,IEC1937_AC_3,IEC1937_MPEG_1,
  IEC1937_MPEG_2_NOEXT, IEC1937_MPEG_2_EXT, IEC1937_MPEG_2_Layer1_LS,
  IEC1937_MPEG_2_Layer2_3_LS,
} AudioDataFormatTypeIII_t;

typedef enum
{
  Undefined = 0, Milliseconds, DecodedPCMsamples,
} ClassEpLockDelay_t;

typedef union
{
  u16 wChannelConfig;
  struct
  {
     u16 LF : 1;   // Left Front
     u16 RF : 1;   // Right Front
     u16 CF : 1;   // Center Front
     u16 LFE: 1;   // Low Frequency Enhancement
     u16 Ls : 1;   // Left Surround
     u16 Rs : 1;   // Right Surround
     u16 Lc : 1;   // Left of Center
     u16 Rc : 1;   // Right of Center

     u16 S  : 1;   // Surround
     u16 Sl : 1;   // Side Left
     u16 Sr : 1;   // Side Right
     u16 T  : 1;   // Top
     u16    : 4;   // Reserved
  };
} ChannelConfig_t;

#define FeatUnitMute                        (1UL<<0)
#define FeatUnitVolume                      (1UL<<1)
#define FeatUnitBass                        (1UL<<2)
#define FeatUnitMid                         (1UL<<3)
#define FeatUnitTreble                      (1UL<<4)
#define FeatUnitGraphicEqualizer            (1UL<<5)
#define FeatUnitAutomaticGain               (1UL<<6)
#define FeatUnitDelay                       (1UL<<7)
#define FeatUnitBassBoost                   (1UL<<8)
#define FeatUnitLoudness                    (1UL<<9)

// Predefined Audio Channel Configuration Bits
#define AUDIO_CHANNEL_M                     0x0000  // Mono
#define AUDIO_CHANNEL_L                     0x0001  // Left Front
#define AUDIO_CHANNEL_R                     0x0002  // Right Front
#define AUDIO_CHANNEL_C                     0x0004  // Center Front
#define AUDIO_CHANNEL_LFE                   0x0008  // Low Frequency Effects
#define AUDIO_CHANNEL_LS                    0x0010  // Left Surround
#define AUDIO_CHANNEL_RS                    0x0020  // Right Surround
#define AUDIO_CHANNEL_LC                    0x0040  // Left of Center
#define AUDIO_CHANNEL_RC                    0x0080  // Right of Center
#define AUDIO_CHANNEL_S                     0x0100  // Surround
#define AUDIO_CHANNEL_SL                    0x0200  // Side Left
#define AUDIO_CHANNEL_SR                    0x0400  // Side Right
#define AUDIO_CHANNEL_T                     0x0800  // Top

//  USB Terminal Types
#define AudioUsbUndefined                   0x0100
#define AudioUsbStreaming                   0x0101
#define AudioUsbVendorSpecific              0x01FF

// Input Terminal Types
#define AudioInputUndefined                 0x0200
#define AudioInputMic                       0x0201
#define AudioInputDesktopMic                0x0202
#define AudioInputPersonalMic               0x0203
#define AudioInputOmniDirectionalMic        0x0204
#define AudioInputMicArray                  0x0205
#define AudioInputProcessingMicArray        0x0206

//  Output Terminal Types
#define AudioOutputUndefined                0x0300
#define AudioOutputSpk                      0x0301
#define AudioOutputHeadphones               0x0302
#define AudioOutputHeadMountedDisplayAudio  0x0303
#define AudioOutputDesktopSpk               0x0304
#define AudioOutputRoomSpk                  0x0305
#define AudioOutputCommunicationSpk         0x0306
#define AudioOutputLowFrequencyEffectsSpk   0x0307

// Audio Class-Specific Request Codes
#define REQUEST_CODE_UNDEFINED              0x00
#define REQUEST_SET_CUR                     0x01
#define REQUEST_GET_CUR                     0x81
#define REQUEST_SET_MIN                     0x02
#define REQUEST_GET_MIN                     0x82
#define REQUEST_SET_MAX                     0x03
#define REQUEST_GET_MAX                     0x83
#define REQUEST_SET_RES                     0x04
#define REQUEST_GET_RES                     0x84
#define REQUEST_SET_MEM                     0x05
#define REQUEST_GET_MEM                     0x85
#define REQUEST_GET_STAT                    0xFF

// Terminal Control Selectors
#define TE_CONTROL_UNDEFINED                0x00
#define TE_COPY_PROTECT_CONTROL             0x01

// Feature Unit Control Selectors
#define FU_CONTROL_UNDEFINED                0x00
#define FU_MUTE_CONTROL                     0x01
#define FU_VOLUME_CONTROL                   0x02
#define FU_BASS_CONTROL                     0x03
#define FU_MID_CONTROL                      0x04
#define FU_TREBLE_CONTROL                   0x05
#define FU_GRAPHIC_EQUALIZER_CONTROL        0x06
#define FU_AUTOMATIC_GAIN_CONTROL           0x07
#define FU_DELAY_CONTROL                    0x08
#define FU_BASS_BOOST_CONTROL               0x09
#define FU_LOUDNESS_CONTROL                 0x0A

// Endpoint Control Selectors
#define EP_CONTROL_UNDEFINED                0x00
#define SAMPLING_FREQ_CONTROL               0x01
#define PITCH_CONTROL                       0x02

#pragma pack(1)
typedef struct
{
  u8             bLength;
  u8             bDescriptorType;
  u8             bDescriptorSubtype;
  u16            bcdADC;
  u16            wTotalLength;
  u8             bInCollection;
  u8             baInterfaceNr1;
} UsbClassSpecificACInterfaceDescriptor_t, *pUsbClassSpecificACInterfaceDescriptor_t;

typedef struct
{
  u8             bLength;
  u8             bDescriptorType;
  u8             bDescriptorSubtype;
  u8             bTerminalID;
  u16            wTerminalType;
  u8             bAssocTerminal;
  u8             bNrChannels;
  ChannelConfig_t   wChannelConfig;
  u8             iChannelNames;
  u8             iTerminal;
} UsbInputTerminalDescriptor_t, *pUsbInputTerminalDescriptor_t;

typedef struct
{
  u8             bLength;
  u8             bDescriptorType;
  u8             bDescriptorSubtype;
  u8             bTerminalLink;
  u8             bDelay;
  u16            wFormatTag;
} UsbClassSpecificASInterfaceDescriptor_t, *pUsbClassSpecificASInterfaceDescriptor_t;

typedef struct
{
  u8             bLength;
  u8             bDescriptorType;
  u8             bDescriptorSubtype;
  u8             bTerminalID;
  u16            wTerminalType;
  u8             bAssocTerminal;
  u8             bSourceID;
  u8             iTerminal;
} UsbOutputTerminalDescriptor_t, *pUsbOutputTerminalDescriptor_t;

typedef struct
{
  u8             bLength;
  u8             bDescriptorType;
  u8             bDescriptorSubtype;
  u8             bFormatType;
  u8             bNrChannels;
  u8             bSubframeSize;
  u8             bBitResolution;
  u8             bSamFreqType;
} UsbTypeIFormatTypeDescriptor_t, *pUsbTypeIFormatTypeDescriptor_t;

typedef struct
{
  u8             bLength;
  u8             bDescriptorType;
  u8             bEndpointAddress;
  EpAttr_t          bmAttributes;
  u16            wMaxPacketSize;
  u8             bInterval;
  u8             bRefresh;
  u8             bSynchAddress;
} StandardASEndpointDescriptor_t, *pStandardASEndpointDescriptor_t;

typedef union
{
  u8 Attribute;
  struct
  {
    u8 SamplFreq     : 1;
    u8 Pitch         : 1;
    u8               : 5;
    u8 MaxPacketsOnly: 1;
  };
} ClassEpAttr_t, *pClassEpAttr_t;

typedef struct
{
  u8             bLength;
  u8             bDescriptorType;
  u8             bDescriptorSubtype;
  ClassEpAttr_t     bmAttributes;
  u8             bLockDelayUnits;
  u16            wLockDelay;
} ClassSpecASEndpointDescriptor_t, *pClassSpecASEndpointDescriptor_t;

typedef struct
{
  u8             bLength;
  u8             bDescriptorType;
  u8             bDescriptorSubtype;
  u8             bUnitID;
  u8             bSourceID;
  u8             bControlSize;
} ClassSpecFeatureUnitDescriptor_t, *pClassSpecFeatureUnitDescriptor_t;

typedef struct
{
  u8             bLength;
  u8             bDescriptorType;
  u8             bDescriptorSubtype;
  u8             bUnitID;
  u8             bNrInPins;
} ClassSpecSelectorUnitDescriptor_t, *pClassSpecSelectorUnitDescriptor_t;

typedef struct
{
  u8             bLength;
  u8             bDescriptorType;
  u8             bDescriptorSubtype;
  u8             bUnitID;
  u8             bNrInPins;
} ClassSpecMixUnitDescriptor_t, *pClassSpecMixrUnitDescriptor_t;

#pragma pack()

#endif //__AUDIO_DESC_H
