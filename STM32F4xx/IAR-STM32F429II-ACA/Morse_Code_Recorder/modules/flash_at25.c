/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2014
 *
 *    File name   : flash_at25.c
 *    Description : AT25DF041A SPI Flash Helper functions
 *
 *    History :
 *    1. Date        : October, 2014
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 **************************************************************************/
#include <yfuns.h>
#include "includes.h"

#define CMD_WEN    0x06
#define CMD_BE4K   0x20
#define CMD_PAGEWR 0x02
#define CMD_RDSTAT 0x05
#define CMD_WRSTAT 0x01   
#define CMD_READ   0x0B

#define STAT_BIT_BUSY ( 1<<0 )
#define STAT_BIT_EPE  ( 1<<5 )

uint8_t SPI_Transfer(SPI_TypeDef* SPIx, uint8_t data)
{
  while ( RESET == SPI_I2S_GetFlagStatus( SPI_FLASH_MODULE, SPI_I2S_FLAG_TXE ) );
  SPI_I2S_SendData( SPI_FLASH_MODULE, data );
  while( RESET == SPI_I2S_GetFlagStatus( SPI_FLASH_MODULE, SPI_I2S_FLAG_RXNE ) );
  return (uint8_t)( SPI_I2S_ReceiveData( SPI_FLASH_MODULE ) & 0x00FF );
}

/*************************************************************************
 * Function Name: AT25_WriteEnable
 * Parameters: none
 *
 * Return: none
 *
 * Description: Enable writing to the spi memory
 *
 *************************************************************************/
static void AT25_WriteEnable(void)
{
  GPIO_ResetBits( SPI_FLASH_GPIO_PORT, SPI_FLASH_NSS_PIN );
  SPI_Transfer( SPI_FLASH_MODULE, CMD_WEN );
  GPIO_SetBits( SPI_FLASH_GPIO_PORT, SPI_FLASH_NSS_PIN );
}

/*************************************************************************
 * Function Name: AT25_WaitBusy
 * Parameters: none
 *
 * Return: uint8_t
 *
 * Description: Waits until device becomes ready.
 *
 *************************************************************************/
static uint8_t AT25_WaitBusy()
{
uint8_t stat;
  do
  {
    GPIO_ResetBits( SPI_FLASH_GPIO_PORT, SPI_FLASH_NSS_PIN );   
    SPI_Transfer( SPI_FLASH_MODULE, CMD_RDSTAT );
    stat = SPI_Transfer(SPI_FLASH_MODULE, 0x00 );
    GPIO_SetBits(SPI_FLASH_GPIO_PORT, SPI_FLASH_NSS_PIN );
    if( stat & STAT_BIT_EPE ) return RES_ERROR;
  } while( stat & STAT_BIT_BUSY );
  return RES_OK;
}

/*************************************************************************
 * Function Name: AT25_UnprotectFlash
 * Parameters: none
 *
 * Return: uint8_t
 *
 * Description: Unprotect flash memory.
 *
 *************************************************************************/
void AT25_UnprotectFlash(void)
{
  AT25_WriteEnable();
  GPIO_ResetBits( SPI_FLASH_GPIO_PORT, SPI_FLASH_NSS_PIN );
  SPI_Transfer( SPI_FLASH_MODULE, CMD_WRSTAT );
  SPI_Transfer( SPI_FLASH_MODULE, 0x00 );
  GPIO_SetBits( SPI_FLASH_GPIO_PORT, SPI_FLASH_NSS_PIN ); 
}

/*************************************************************************
 * Function Name: AT25_EraseBlock4K
 * Parameters: uint32_t addr
 *
 * Return: uint8_t
 *
 * Description: Erases one 4K block at given address.
 *
 *************************************************************************/
uint8_t AT25_EraseBlock4K(uint32_t addr)
{
  AT25_WriteEnable();
  GPIO_ResetBits( SPI_FLASH_GPIO_PORT, SPI_FLASH_NSS_PIN );
  SPI_Transfer( SPI_FLASH_MODULE, CMD_BE4K );
  SPI_Transfer( SPI_FLASH_MODULE, ( addr>>16 ) );
  SPI_Transfer( SPI_FLASH_MODULE, ( addr>>8 )  );
  SPI_Transfer( SPI_FLASH_MODULE, 0x00 );
  GPIO_SetBits( SPI_FLASH_GPIO_PORT, SPI_FLASH_NSS_PIN );
  return AT25_WaitBusy();
}

/*************************************************************************
 * Function Name: AT25_PageWrite
 * Parameters: uint32_t addr, unsigned char * buffer, size_t size
 *
 * Return: uint8_t
 *
 * Description: Writes one 256-byte page starting at the given address.
 *
 *************************************************************************/
uint8_t AT25_PageWrite(uint32_t addr, unsigned char * buffer, size_t size)
{
  AT25_WriteEnable();
  GPIO_ResetBits( SPI_FLASH_GPIO_PORT, SPI_FLASH_NSS_PIN );
  SPI_Transfer( SPI_FLASH_MODULE, CMD_PAGEWR );
  SPI_Transfer( SPI_FLASH_MODULE, ( addr>>16 ) );
  SPI_Transfer( SPI_FLASH_MODULE, ( addr>>8 )  );
  SPI_Transfer( SPI_FLASH_MODULE, 0x00 );
  if( size > 256 ) return RES_ERROR;
  for( int n=0; n < size; n++ )
  {
    SPI_Transfer( SPI_FLASH_MODULE, *(buffer+n) );
  }
  GPIO_SetBits( SPI_FLASH_GPIO_PORT, SPI_FLASH_NSS_PIN );
  return AT25_WaitBusy();
}

/*************************************************************************
 * Function Name: AT25_WriteData
 * Parameters: uint32_t addr, unsigned char * buffer, size_t size
 *
 * Return: uint8_t
 *
 * Description: Writes size-count bytes to flash starting at the given address.
 *
 *************************************************************************/
uint8_t AT25_WriteData(uint32_t addr, unsigned char * buffer, size_t size)
{
uint32_t addrCurr, pageSize;

  addrCurr = addr;
  /* Erase all needed blocks */
  while( ( addrCurr & 0xFFFFF000 ) < ( addr + size ) ) 
  {
    if( RES_ERROR == AT25_EraseBlock4K( addrCurr ) ) return RES_ERROR;
    addrCurr += 0x1000;
  }
  while( size )
  {
    if( size >= 256 ) 
    {
      pageSize = 256; 
      size -= 256;
    }
    else 
    {
      pageSize = size;
      size = 0;
    }
    if( RES_ERROR == AT25_PageWrite( addr, buffer, pageSize ) ) return RES_ERROR;
    buffer += 256;
    addr   += 256;
  }
  return RES_OK;
}

/*************************************************************************
 * Function Name: AT25_ReadData
 * Parameters: uint32_t addr, unsigned char * buffer, size_t size
 *
 * Return: uint8_t
 *
 * Description: Reads size-count bytes from flash starting at the given address.
 *
 *************************************************************************/
void AT25_ReadData(uint32_t addr, unsigned char * buffer, size_t size)
{
  GPIO_ResetBits( SPI_FLASH_GPIO_PORT, SPI_FLASH_NSS_PIN );
  SPI_Transfer( SPI_FLASH_MODULE, CMD_READ );
  SPI_Transfer( SPI_FLASH_MODULE, ( addr>>16 ) );
  SPI_Transfer( SPI_FLASH_MODULE, ( addr>>8 )  );
  SPI_Transfer( SPI_FLASH_MODULE, ( addr )     );
  SPI_Transfer( SPI_FLASH_MODULE, 0x00 );
  while(size)
  {   
    *( buffer++ ) = SPI_Transfer( SPI_FLASH_MODULE, 0x00 );
    size--;
  }
  GPIO_SetBits( SPI_FLASH_GPIO_PORT, SPI_FLASH_NSS_PIN );
}
