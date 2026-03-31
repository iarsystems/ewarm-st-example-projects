/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : SDIO_Test.c
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : SDIO_Test program body
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "sdcard.h"
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BlockSize            512 /* Block Size in Bytes */
#define BufferWordsSize      (BlockSize >> 2)

#define NumberOfBlocks        6  /* For Multi Blocks operation (Read/Write) */
#define MultiBufferWordsSize ((BlockSize * NumberOfBlocks) >> 2)

#define SD_DMA_TRANSFER            0
//#define SD_INTERRUPT_TRANSFER      1
//#define SD_POLLING_TRANSFER        2

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SD_CardInfo SDCardInfo;
u32 Buffer_Block_Tx[BufferWordsSize], Buffer_Block_Rx[BufferWordsSize];
u32 Buffer_MultiBlock_Tx[MultiBufferWordsSize], Buffer_MultiBlock_Rx[MultiBufferWordsSize];
volatile TestStatus EraseStatus = FAILED, TransferStatus11 = FAILED, TransferStatus22 = FAILED;
SD_Error Status = SD_OK;
SD_Error Status1;
/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration1(void);
void Fill_Buffer1(u32 *pBuffer, u16 BufferLenght, u32 Offset);
TestStatus Buffercmp1(u32* pBuffer1, u32* pBuffer2, u16 BufferLength);
TestStatus eBuffercmp1(u32* pBuffer, u16 BufferLength);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SD_Card_Test(void)
{
  /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(DISABLE);
  
  /* Interrupt Config */
  NVIC_Configuration1();
  
  /*-------------------------- SD Init ----------------------------- */
 printf ("\f");
  //Status = SD_Init();

    
  /* Display Test name */
    GLCD_SetFont(&Terminal_9_12_6,0xA,0xFFF);
    GLCD_TextSetPos(3,1);
    printf( "SD Card Test\n ");
    GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
    GLCD_TextSetPos(1,2);
    printf ("SD Initialisation... "); 
 
  while(ReadKey() != User)
    {
  /*----------------- Read CSD/CID MSD registers ------------------*/
    
   Status = SD_Init();    
    if (Status != SD_OK)
  {

    GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
    GLCD_TextSetPos(1,3);
    printf ("SD Card NOT detected!...   ");
    GLCD_TextSetPos(1,4);
    printf ("Pls, Insert Card      ");
    GLCD_TextSetPos(1,5);
    printf ("                      ");
    GLCD_TextSetPos(1,6);
    printf ("                     ");
    GLCD_TextSetPos(1,7);
    printf ("                     ");
    GLCD_TextSetPos(1,8);
    printf ("                     ");    
  } 
  else 
  {
    GLCD_SetFont(&Terminal_9_12_6,0xF0,0xFFF);
    GLCD_TextSetPos(1,3);
    printf ("SDIO Card Init OK   \r");
  }
  
  Status = SD_GetCardInfo(&SDCardInfo);
   if (Status == SD_OK)
  {
    /*----------------- Select Card --------------------------------*/
    Status = SD_SelectDeselect((u32) (SDCardInfo.RCA << 16));
  }

  if (Status == SD_OK)
  {
    Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
  }

  /*------------------- Block Erase -------------------------------*/
  if (Status == SD_OK)
  {
    /* Erase NumberOfBlocks Blocks of WRITE_BL_LEN(512 Bytes) */
    Status = SD_Erase(0x00, (BlockSize * NumberOfBlocks));
  }

  /* Set Device Transfer Mode */
  if (Status == SD_OK)
  {
#ifdef SD_DMA_TRANSFER    
    Status = SD_SetDeviceMode(SD_DMA_MODE);
    GLCD_SetFont(&Terminal_9_12_6,0xA,0xFFF);
    GLCD_TextSetPos(1,3);
    printf ("SD DMA TRANSFER mode\n");
#elif defined SD_INTERRUPT_TRANSFER
    Status = SD_SetDeviceMode(SD_INTERRUPT_MODE);
    GLCD_SetFont(&Terminal_9_12_6,0xA,0xFFF);
    GLCD_TextSetPos(1,3);
    printf ("SD INT TRANSFER mode\n");
#else
    Status = SD_SetDeviceMode(SD_POLLING_MODE);
    printf ("\f");
    GLCD_SetFont(&Terminal_9_12_6,0xA,0xFFF);
    GLCD_TextSetPos(1,3);
    printf ("SD POLLING mode\n");
#endif
  }

  if (Status == SD_OK)
  {
    Status = SD_ReadMultiBlocks(0x00, Buffer_MultiBlock_Rx, BlockSize, NumberOfBlocks);
  }

  if (Status == SD_OK)
  {  
    EraseStatus = eBuffercmp1(Buffer_MultiBlock_Rx, MultiBufferWordsSize);
    GLCD_SetFont(&Terminal_9_12_6,0xA,0xFFF);
    GLCD_SetFont(&Terminal_9_12_6,0xF0,0xFFF);    
    GLCD_TextSetPos(1,4);
    printf ("Multi Block Erase OK     ");
    //Delay (40);
    for (unsigned int i=0; i<500;i++);
  }
  
  /*------------------- Block Read/Write --------------------------*/
  /* Fill the buffer to send */
  Fill_Buffer1(Buffer_Block_Tx, BufferWordsSize, 0xFFFF);

  if (Status == SD_OK)
  {
    /* Read block of 512 bytes from address 0 */
    Status = SD_ReadBlock(0x00, Buffer_Block_Rx, BlockSize);

  }

  if (Status == SD_OK)
  {
    /* Write block of 512 bytes on awddress 0 */
    Status = SD_WriteBlock(0x00, Buffer_Block_Tx, BlockSize);
  }

  if (Status == SD_OK)
  {
    /* Read block of 512 bytes from address 0 */
    Status = SD_ReadBlock(0x00, Buffer_Block_Rx, BlockSize);
  }

  if (Status == SD_OK)
  {
    GLCD_SetFont(&Terminal_9_12_6,0xA,0xFFF);
    GLCD_TextSetPos(1,5);
    printf ("Block Write Test...  ");
    //Delay (40);
   // DelayResolution100us(Dly-6000);
        for (unsigned int i=0; i<500;i++);
    /* Check the corectness of written dada */
    TransferStatus11 = Buffercmp1(Buffer_Block_Tx, Buffer_Block_Rx, BufferWordsSize);
  

  	if (TransferStatus11==1)
	{
          GLCD_SetFont(&Terminal_9_12_6,0xF0,0xFFF);
          GLCD_TextSetPos(1,6);
          printf ("Block Write OK      ");
	}
	else
	{
          GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);         
          GLCD_TextSetPos(1,6);
          printf ("Block Write FAILED!      ");
	}
 }
  /*--------------- Multiple Block Read/Write ---------------------*/
  /* Fill the buffer to send */
  Fill_Buffer1(Buffer_MultiBlock_Tx, MultiBufferWordsSize, 0x0);

  if (Status == SD_OK)
  {
    /* Write multiple block of many bytes on address 0 */
    Status = SD_WriteMultiBlocks(0x00, Buffer_MultiBlock_Tx, BlockSize, NumberOfBlocks);
  }

  if (Status == SD_OK)
  {
    /* Read block of many bytes from address 0 */
    Status = SD_ReadMultiBlocks(0x00, Buffer_MultiBlock_Rx, BlockSize, NumberOfBlocks);
  }

  if (Status == SD_OK)
  {
    /* Check the corectness of written dada */
          TransferStatus22 = Buffercmp1(Buffer_MultiBlock_Tx, Buffer_MultiBlock_Rx, MultiBufferWordsSize);
          GLCD_SetFont(&Terminal_9_12_6,0xA,0xFFF);
          GLCD_TextSetPos(1,7);
          printf ("Multi Block Write...   ");
          //DelayResolution100us(Dly-6000);
              for (unsigned int i=0; i<500;i++);
	if (TransferStatus22==1)
	{
          GLCD_SetFont(&Terminal_9_12_6,0xF0,0xFFF);
          GLCD_TextSetPos(1,8);
          printf ("Multi Block Write OK      ");
	}
	else
	{
          GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
          GLCD_TextSetPos(1,8);
          printf ("Multi Block Write FAILED    ");
	}

  }
    
    GLCD_SetFont(&Terminal_9_12_6,0xA0,0xFFF);
    GLCD_TextSetPos(0,9);
    printf( "Press User key to exit\n ");
   
 }


  ReturnFunc();
  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE); 
}
/*******************************************************************************
* Function Name  : NVIC_Config
* Description    : Configures SDIO IRQ channel.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration1(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : Buffercmp
* Description    : Compares two buffers.
* Input          : - pBuffer1, pBuffer2: buffers to be compared.
*                : - BufferLength: buffer's length
* Output         : None
* Return         : PASSED: pBuffer1 identical to pBuffer2
*                  FAILED: pBuffer1 differs from pBuffer2
*******************************************************************************/
TestStatus Buffercmp1(u32* pBuffer1, u32* pBuffer2, u16 BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

/*******************************************************************************
* Function name : Fill_Buffer
* Description   : Fill the gloal buffer
* Input         : - pBuffer: pointer on the Buffer to fill
*                 - BufferSize: size of the buffer to fill
*                 - Offset: first value to fill on the Buffer
* Output        : None
* Return        : None
*******************************************************************************/
void Fill_Buffer1(u32 *pBuffer, u16 BufferLenght, u32 Offset)
{
  u16 index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLenght; index++ )
  {
    pBuffer[index] = index + Offset;
  }
}

/*******************************************************************************
* Function name : eBuffercmp
* Description    : Checks if a buffer has all its values are equal to zero.
* Input          : - pBuffer1, pBuffer2: buffers to be compared.
*                : - BufferLength: buffer's length
* Output         : None
* Return         : PASSED: pBuffer values are zero
*                  FAILED: At least one value from pBuffer buffer is diffrent 
*                          from zero.
*******************************************************************************/
TestStatus eBuffercmp1(u32* pBuffer, u16 BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer != 0x00)
    {
      return FAILED;
    }

    pBuffer++;
  }

  return PASSED;
}


/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
