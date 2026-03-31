/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_pcd8544.c
 *    Description : PCD8544 driver
 *
 *    History :
 *    1. Date        : December 28, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "drv_pcd8544.h"

/*************************************************************************
 * Function Name: PCD8544_PowerUp
 * Parameters: const Int8U * pData
 *
 * Return: none
 *
 * Description: Power up initialization
 *
 *************************************************************************/
void PCD8544_PowerUp (const Int8U * pData)
{
  // Init Peripherals and reset LCD
  PCD8544_Init();
  // function set PD = 0 and V = 0, select extended instruction set (H = 1 mode)
  PCD8544_WrCmd(0x21);
  // // Set LCD Voltage to about 7V.
  PCD8544_WrCmd(0xC8);
  // Adjust voltage bias.
  PCD8544_WrCmd(0x13);
  // function set PD = 0 and V = 0, select normal instruction set (H = 0 mode)
  PCD8544_WrCmd(0x20);
  // Init DDRAM
  if(pData == NULL)
  {
    for(Int32U i = 0; i < (X_PIX_SIZE * Y_PIX_SIZE)/8; i++)
    {
      PCD8544_WrData(0x00);
    }
  }
  else
  {
    for(Int32U i = 0; i < (X_PIX_SIZE * Y_PIX_SIZE)/8; i++)
    {
      PCD8544_WrData(*pData++);
    }
  }
  // display control set normal mode (D = 1 and E = 0)
  PCD8544_WrCmd(0x0C);
}


/*************************************************************************
 * Function Name: PCD8544_StrShow
 * Parameters: Int8U X, Int8U Y, const Int8U * pData
 *
 * Return: none
 *
 * Description: Show zero terminate string
 *
 *************************************************************************/
void PCD8544_StrShow (Int8U X, Int8U Y, const Int8U * pData)
{
pInt8U pFontData;
Int32U ch;
  // parameter check
  assert(pData);
  // Set Address
  X *= FONT.H_Size;
  PCD8544_SetAddr(X,Y);
  // Fill LCD Data memory
  while(*pData)
  {
    // find character start address
    assert(*pData >= FONT.CharacterOffset);
    assert(*pData <  FONT.CharactersNuber + FONT.CharacterOffset);
    ch = *pData - FONT.CharacterOffset;
    pData++;
    pFontData = FONT.pFontStream + (ch * ((FONT.H_Size * FONT.V_Size)/8));
    for(Int32U i = FONT.H_Size; i; --i)
    {
      // print character
      PCD8544_WrData(*pFontData++);
    }
  }
}

/*************************************************************************
 * Function Name: PCD8544_LoadData
 * Parameters: Int8U X, Int8U Y, Int32U Size, const Int8U * pData
 *
 * Return: none
 *
 * Description: Load raw data in the LDC data memory
 *
 *************************************************************************/
void PCD8544_LoadData (Int8U X, Int8U Y, Int32U Size, const Int8U * pData)
{
  // parameter check
  assert(pData);
  // Set Address
  PCD8544_SetAddr(X,Y);
  // Fill LCD Data memory
  while(Size--)
  {
    PCD8544_WrData(*pData++);
  }
}

/*************************************************************************
 * Function Name: PCD8544_SetAddr
 * Parameters: Int8U X, Int8U Y
 *
 * Return: none
 *
 * Description: Set X and Y address of RAM
 *
 *************************************************************************/
static void PCD8544_SetAddr (Int8U X, Int8U Y)
{
  assert(Y < (Y_PIX_SIZE/8));
  assert(X < (X_PIX_SIZE/8));
  // Set Y address
  PCD8544_WrCmd(0x40 | Y);
  // Set X address
  PCD8544_WrCmd(0x80 | X);
}

