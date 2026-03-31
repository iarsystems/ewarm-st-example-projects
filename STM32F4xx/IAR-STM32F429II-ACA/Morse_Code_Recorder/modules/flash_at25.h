/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2014
 *
 *    File name   : flash_at25.h
 *    Description : AT25DF041B SPI Flash Helper functions
 *
 *    History :
 *    1. Date        : October, 2014
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 **************************************************************************/
#include <yfuns.h>
#include "includes.h"

#define RES_OK     0x00
#define RES_ERROR  0x01

/*************************************************************************
 * Function Name: AT25_UnprotectFlash
 * Parameters: none
 *
 * Return: uint8_t
 *
 * Description: Unprotects the flash memory.
 *
 *************************************************************************/
void AT25_UnprotectFlash(void);

/*************************************************************************
 * Function Name: AT25_EraseBlock4K
 * Parameters: uint32_t addr
 *
 * Return: uint8_t
 *
 * Description: Erases one 4K block at given address.
 *
 *************************************************************************/
uint8_t AT25_EraseBlock4K(uint32_t addr);

/*************************************************************************
 * Function Name: AT25_PageWrite
 * Parameters: uint32_t addr, unsigned char * buffer, size_t size
 *
 * Return: uint8_t
 *
 * Description: Writes one 256-byte page starting at the given address.
 *
 *************************************************************************/
uint8_t AT25_PageWrite(uint32_t addr, unsigned char * buffer, size_t size);

/*************************************************************************
 * Function Name: AT25_WriteData
 * Parameters: uint32_t addr, unsigned char * buffer, size_t size
 *
 * Return: uint8_t
 *
 * Description: Writes size-count bytes to flash starting at the given address.
 *
 *************************************************************************/
uint8_t AT25_WriteData(uint32_t addr, unsigned char * buffer, size_t size);

/*************************************************************************
 * Function Name: SPI_ReadData
 * Parameters: uint32_t addr, unsigned char * buffer, size_t size
 *
 * Return: uint8_t
 *
 * Description: Reads size-count bytes from flash starting at the given address.
 *
 *************************************************************************/
void AT25_ReadData(uint32_t addr, unsigned char * buffer, size_t size);
