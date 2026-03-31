/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_pcd8544_l.c
 *    Description : STM32-103STK PCD8544 low level driver
 *
 *    History :
 *    1. Date        : December 28, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
**************************************************************************/
#include "drv_pcd8544_l.h"

/*************************************************************************
 * Function Name: PCD8544_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init peripherals and resetting the LCD
 *
 *************************************************************************/
void PCD8544_Init (void)
{
GPIO_InitTypeDef  GPIO_InitStructure;
SPI_InitTypeDef   SPI_InitStructure;
RCC_ClocksTypeDef Clk;
Int32U Div = 2;
Int32U DivVal = 0;

  // Init LCD IOs and SPI1
  RCC_APB2PeriphResetCmd(  RCC_APB2Periph_GPIOA
                         | RCC_APB2Periph_GPIOB
                         | RCC_APB2Periph_GPIOC
                         | RCC_APB2Periph_SPI1
                         | RCC_APB2Periph_AFIO,
                           DISABLE);
  RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOA
                         | RCC_APB2Periph_GPIOB
                         | RCC_APB2Periph_GPIOC
                         | RCC_APB2Periph_SPI1
                         | RCC_APB2Periph_AFIO,
                         ENABLE);

  // Configure LCD Enable(PC10) and Reset (PC7).
  GPIO_WriteBit(LCD_CS_PORT,LCD_CS,Bit_SET);    // disable LCD SPI
  GPIO_InitStructure.GPIO_Pin =  LCD_RST | LCD_CS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // Configure LCD Data/Command (PB2)
  GPIO_InitStructure.GPIO_Pin =  LCD_DC;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_DC_PORT, &GPIO_InitStructure);

  PCD8544_RST_L();
  for(volatile Int32U i = 10; i; i--);
  PCD8544_RST_H();

  // Configure SPI1
  // Deinitializes the SPI1
  SPI_I2S_DeInit(SPI1);
  // Configure SPI1_CLKa and SPI1_MOSI
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  // SPI1 no remap (NSS/PA4, SCK/PA5, MISO/PA6, MOSI/PA7)
  GPIO_PinRemapConfig(GPIO_Remap_SPI1, DISABLE);
  // Spi init
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  // Clock Freq. Identification Mode < 4MHz
  RCC_GetClocksFreq(&Clk);

  while((MAX_SPI_FREQ * Div) <=  Clk.PCLK1_Frequency)
  {
    Div <<= 1;
    if (++DivVal == 7)
    {
      break;
    }
  }

  SPI1->CR1 = (SPI1->CR1 & ~(0x7 << 3)) | ((DivVal&0x7) << 3);

  // Enable SPI1
  SPI_Cmd(SPI1, ENABLE);
}

/*************************************************************************
 * Function Name: PCD8544_WrData
 * Parameters: Int8U Data
 *
 * Return: none
 *
 * Description: Send Data Byte
 *
 *************************************************************************/
void PCD8544_WrData(Int8U Data)
{
  PCD8544_CS_L();
  PCD8544_DATA();
  SPI_I2S_SendData(SPI1, Data);
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
  PCD8544_CS_H();
}

/*************************************************************************
 * Function Name: PCD8544_WrCmd
 * Parameters: Int8U Command
 *
 * Return: none
 *
 * Description: Send Command
 *
 *************************************************************************/
void PCD8544_WrCmd(Int8U Command)
{
  PCD8544_CS_L();
  PCD8544_CMD();
  SPI_I2S_SendData(SPI1, Command);
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
  PCD8544_CS_H();
}
