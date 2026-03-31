/**
  ******************************************************************************
  * @file    USART/Printf/main.c 
  * @author  MCD Application Team
  * @version V1.0.0RC1
  * @date    07/02/2010
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "stm32_eval.h"

#include "main.h"

#include "spi_flash.h"

#include <stdio.h>

/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define Bank1_SRAM4_ADDR  ((uint32_t)0x6c000000)
#define nc 10 // Anzahl der Filterkoeffizienten

#define  FLASH_WRITE_ADDRESS      0x000000
#define  FLASH_READ_ADDRESS       FLASH_WRITE_ADDRESS
#define  FLASH_SECTOR_TO_ERASE    FLASH_WRITE_ADDRESS

#define  BufferSize (countof(Tx_Buffer)-1)

/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

uint8_t Tx_Buffer[] = "STM32F2xx SPI Firmware Library Example: communication with an M25P SPI FLASH";
uint8_t  Rx_Buffer[BufferSize];
__IO uint8_t Index = 0x0;
__IO uint32_t FlashID = 0;

char *mainmenu = "\n\n\r"
				 "(1) - UART Test\n\r"
                 "(2) - GPIO Toggle\n\r"
                 "(3) - scanf Test\n\r"
                 "(4) - OTG_SW_ON\n\r"
                 "(5) - OTG_ALARM\n\r"
                 "(6) - USB_DATA\n\r"
                 "(7) - SD-Card Test\n\r"
                 "(8) - FSMC Test\n\r"
                 "(9) - SPI Test\n\r"
				 "\n\r"
				 "Press Key\n\r";

/* Private function prototypes -----------------------------------------------*/

void USART_Configuration(void);
void FSMC_Init(void);
int sd_card_test(void);
void FSMC_test(void);
void SPI_test(void);
void Delay(__IO uint32_t nTime);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

	int i, x;
	char str[32];
	char c;

  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32l1xx_md.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32l1xx.c file
     */     

	USART_Configuration();
	
	/* Configure FSMC Bank1 NOR/SRAM4  */
	FSMC_Init();

	STM_EVAL_LEDInit(LED1);
	STM_EVAL_LEDInit(LED2);
	STM_EVAL_LEDInit(OTG_SW_ON);
	STM_EVAL_LEDInit(USB_DATA);

	STM_EVAL_LEDInit(FLASH_OE);
	STM_EVAL_LEDOff(FLASH_OE);	// set to low

	STM_EVAL_PBInit(OTG_ALARM, BUTTON_MODE_GPIO);

  /* Interrupt Config */
  NVIC_Configuration();

  printf("\n\n\r SILICA  XYNERGY-Demoboard  Testing\n\n\r");

  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
	printf("\n\n\rERROR: SysTick not working\n\n\r");
    while (1);
  }
  	
	do {
		printf(mainmenu);

		c = getchar();

		switch(c) {
			case '1' :	
						printf("\n\rUART Test\n\r");
						for (i=0; i<26; i++) {
							putchar('a'+i);
						}
						printf("\n\r");
						break;
			case '2' :
						printf("\n\rGPIO Toggle Test\n\r");
						printf("press key (x to exit)\n\r");
						c = 0;
						i = 0;

						while (c != 'x')
						{				
							c = getchar();

							if (i == 0) {
								/* Turn on LD1 */
								STM_EVAL_LEDOn(LED1);
								/* Turn off LD2 */
								STM_EVAL_LEDOff(LED2);
								i = 1;
							}
							else {
								/* Turn off LD1 */
								STM_EVAL_LEDOff(LED1);
								/* Turn on LD2 */
								STM_EVAL_LEDOn(LED2);								    
								i = 0;
							}       
						}
						c = 0;
						printf("\n\r");
						break;
			case '3' :
						printf("\n\rscanf Test\n\r");
						i = 1;
						x = 0;
						printf("\n\renter number (5678 to exit)\n\r");
						
						while (x != 5678)
						{
							scanf("%i", &x);
						
							// c = getchar();
							/* Output a message on Hyperterminal using printf function */
							printf("the number %d is: %d\n\r", i++, x);
						}
						printf("\n\r");
						break;
			case '4' :
						printf("\n\rOTG_SW_ON\n\r");
						printf("press key (x to exit)\n\r");
						c = 0;
						i = 0;

						while (c != 'x')
						{				
							c = getchar();

							if (i == 0) {
								/* Turn on OTG_SW_ON */
								STM_EVAL_LEDOn(OTG_SW_ON);
								i = 1;
							}
							else {
								/* Turn off OTG_SW_ON */
								STM_EVAL_LEDOff(OTG_SW_ON);
								i = 0;
							}       
						}
						c = 0;
						printf("\n\r");
						break;
			case '5' :
						printf("\n\rOTG_ALARM\n\r");
						printf("press key (x to exit)\n\r");
						c = 0;

						while (c != 'x')
						{				
							c = getchar();

							printf("OTG_ALARM = %d\n\r", STM_EVAL_PBGetState(OTG_ALARM));
						}
						c = 0;
						printf("\n\r");
						break;
			case '6' :
						printf("\n\rUSB_DATA\n\r");
						printf("press key (x to exit)\n\r");
						c = 0;
						i = 0;

						while (c != 'x')
						{				
							c = getchar();

							if (i == 0) {
								/* Turn on USB_DATA */
								STM_EVAL_LEDOn(USB_DATA);
								i = 1;
							}
							else {
								/* Turn off USB_DATA */
								STM_EVAL_LEDOff(USB_DATA);
								i = 0;
							}       
						}
						c = 0;
						printf("\n\r");
						break;
			case '7' :
						printf("\n\rSD-Card Test\n\r");
						sd_card_test();
						printf("\n\r");
						break;
			case '8' :	
						printf("\n\rFSMC Test\n\r");
						FSMC_test();
						printf("\n\r");
						break;
			case '9' :	
						printf("\n\rSPI Test\n\r");
						SPI_test();
						printf("\n\r");
						break;

		}
	} while (c != 'x');

	printf("\n\n\rProgram terminated\n\r");
	while(1);
}

void USART_Configuration(void) {

	USART_InitTypeDef USART_InitStructure;

	/* USARTx configured as follow:
	        - BaudRate = 115200 baud  
	        - Word Length = 8 Bits
	        - One Stop Bit
	        - No parity
	        - Hardware flow control disabled (RTS and CTS signals)
	        - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	STM_EVAL_COMInit(COM1, &USART_InitStructure);
}

void FSMC_Init(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOE |
                         RCC_AHB1Periph_GPIOF, ENABLE);

  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

  /*-- GPIO Configuration ------------------------------------------------------*/
  /* SRAM Data lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                GPIO_Pin_15;

  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

  /* SRAM Address lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 |
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource2, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource3, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource4, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource5, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource12, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource13, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource14, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource15, GPIO_AF_FSMC);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                GPIO_Pin_4 | GPIO_Pin_5;

  GPIO_Init(GPIOG, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOG, GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource1, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource2, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource3, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource4, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource5, GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;

  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_FSMC);

  /* NOE and NWE configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;

  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);

  /* NE4 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;

  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_FSMC);

  /* NBL0, NBL1 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_FSMC);

  /*-- FSMC Configuration ------------------------------------------------------*/
  p.FSMC_AddressSetupTime = 0;  //0
  p.FSMC_AddressHoldTime = 0;
  p.FSMC_DataSetupTime = 0x4; //4
  p.FSMC_BusTurnAroundDuration = 0;
  p.FSMC_CLKDivision = 0;
  p.FSMC_DataLatency = 0;
  p.FSMC_AccessMode = FSMC_AccessMode_A;

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

  /*!< Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
}

void FSMC_test(void)
{
uint16_t pBuffer, address, index;

int i = 0, zeiger = 0;
int new_sample, y, b[nc+1], circular_buffer[nc];


	address = 0;
	index = 0;
	pBuffer = 0;

//	  *(__IO uint16_t *) (Bank1_SRAM4_ADDR + address) = index+0x1234;
//	  pBuffer = *(__IO uint16_t*) (Bank1_SRAM4_ADDR + address);



// Filterkoeffizienten b[0] = b_N, ..., b[nc - 1] = b_0
//b[0] = 4;
//b[1] = 0;
//b[2] = -7;
//b[3] = 0;
//b[4] = 20;
//b[5] = 31;
//b[6] = 20;
//b[7] = 0;
//b[8] = -7;
//b[9] = 0;
//b[10] = 4;
//for(i = 0; i < nc; i++)
//	circular_buffer[i] = 0;
//
//	new_sample = 0;
//*(__IO uint16_t *) (Bank1_SRAM4_ADDR + address) = 1;
//	// Der folgende Code wird jedes Mal ausgeführt, wenn ein
//	// neuer Eingangswert (new_sample) zur Verfügung steht
//	// Schreibe neuen Eingangswert in Buffer
//	// Eingangswerte werden um 6 bit nach rechts verschoben
//	// (entspricht Multiplikation mit 1/64)
//	circular_buffer[zeiger] = new_sample >> 6;
//	// Inkrementiere Zeiger modulo nc
//	zeiger = (zeiger + 1) % nc;
//	// Berechne neuen Ausgangswert
//	y = 0;
//	for(i = 0; i < nc; i++)
//		y += (b[i] * circular_buffer[(zeiger + i) % nc]);
//*(__IO uint16_t *) (Bank1_SRAM4_ADDR + address) = 0;
//

	for (index=0; index<4; index++) {
	  printf("write to FSMC = %d\n\r", index);
	  *(__IO uint16_t *) (Bank1_SRAM4_ADDR) = index;
	  printf("read Push Button (SW200) = %04X\n\r", *(__IO uint16_t*) (Bank1_SRAM4_ADDR+1));

	  printf("check FSMC = %d - ", index);
	  if (*(__IO uint16_t*) (Bank1_SRAM4_ADDR) == index) {
	  	printf("done\n\r");
	  }
	  else {
	  	printf("failed with %d\n\r", *(__IO uint16_t*) (Bank1_SRAM4_ADDR));
	  };
	  Delay(500);
	}



//while(1) {
//	for (index = 0; index <0xff00; index++) {
//	  *(__IO uint16_t *) (Bank1_SRAM4_ADDR + address) = index+0x0000;
//	  *(__IO uint16_t *) (Bank1_SRAM4_ADDR + address+2) = index+0x0001;
//
//	  pBuffer = *(__IO uint16_t*) (Bank1_SRAM4_ADDR + address);
//	  if (pBuffer != index+0x0000) {
//	    STM_EVAL_LEDOff(LED1);
//		while(1);
//	  }
//
//	  pBuffer = *(__IO uint16_t*) (Bank1_SRAM4_ADDR + address+2);
//	  if (pBuffer != index+0x0001) {
//	    STM_EVAL_LEDOff(LED2);
//		while(1);
//	  }
//	}
//	
//}
}


void SPI_test(void)
{
TestStatus TransferStatus1 = FAILED;

  STM_EVAL_LEDOn(FLASH_OE);	// set to high

  /* Initialize the SPI FLASH driver */
  sFLASH_Init();

  /* Get SPI Flash ID */
  FlashID = sFLASH_ReadID();
  
  /* Check the SPI Flash ID */
  if (FlashID == sFLASH_M25P128_ID)
  {
    /* OK: Turn on LD1 */
  	printf("Flash-ID = %0X\n\r", FlashID);
  	printf("using Flash write address %0X and Flash read address %0X\n\r", FLASH_WRITE_ADDRESS, FLASH_READ_ADDRESS);

    /* Perform a write in the Flash followed by a read of the written data */
    /* Erase SPI FLASH Sector to write on */
  	printf("Flash Erase Sector - ");
    sFLASH_EraseSector(FLASH_SECTOR_TO_ERASE);
  	printf("done\n\r");

STM_EVAL_LEDOff(FLASH_OE);	// set to low
STM_EVAL_LEDOn(FLASH_OE);	// set to high

    /* Write Tx_Buffer data to SPI FLASH memory */
  	printf("Flash Write Buffer - ");
    sFLASH_WriteBuffer(Tx_Buffer, FLASH_WRITE_ADDRESS, BufferSize);
  	printf("done\n\r");

STM_EVAL_LEDOff(FLASH_OE);	// set to low
STM_EVAL_LEDOn(FLASH_OE);	// set to high

    /* Read data from SPI FLASH memory */
  	printf("Flash Read Buffer - ");
    sFLASH_ReadBuffer(Rx_Buffer, FLASH_READ_ADDRESS, BufferSize);
  	printf("done\n\r");

STM_EVAL_LEDOff(FLASH_OE);	// set to low
STM_EVAL_LEDOn(FLASH_OE);	// set to high

    /* Check the correctness of written dada */
    TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
    /* TransferStatus1 = PASSED, if the transmitted and received data by SPI1
       are the same */
    /* TransferStatus1 = FAILED, if the transmitted and received data by SPI1
       are different */
  	printf("Buffer compare = %s\n\r", TransferStatus1 ? "done" : "failed");


    /* Perform an erase in the Flash followed by a read of the written data */
    /* Erase SPI FLASH Sector to write on */
  	printf("Flash Erase Sector again - ");
    sFLASH_EraseSector(FLASH_SECTOR_TO_ERASE);
  	printf("done\n\r");

    /* Read data from SPI FLASH memory */
    sFLASH_ReadBuffer(Rx_Buffer, FLASH_READ_ADDRESS, BufferSize);

    /* Check the correctness of erasing operation dada */
    for (Index = 0; Index < BufferSize; Index++)
    {
      if (Rx_Buffer[Index] != 0xFF)
      {
	  	printf("ERROR: Flash not deleted at address %0X\n\r", Index);
      }
    }
    /* TransferStatus2 = PASSED, if the specified sector part is erased */
    /* TransferStatus2 = FAILED, if the specified sector part is not well erased */
  }
  else
  {
    /* Error: Turn on LD2 */
  	printf("ERROR: wrong Flash-ID = %0X\n\r", FlashID);
  }

  STM_EVAL_LEDOff(FLASH_OE);	// set to low

}

void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
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


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
