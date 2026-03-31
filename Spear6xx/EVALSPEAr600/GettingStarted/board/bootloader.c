/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : bootloader.c
 *    Description : bootloader for SPEAR600 board
 *
 *
 *    History :
 *    2. Date        : August 23, 2011
 *       Author      : Stanimir Bonev
 *       Description : initial vestion
 *
 *    $Revision: 39 $
 **************************************************************************/

/** include files **/
#include "board.h"

/** local definitions **/
#pragma section =".bootloader"
#pragma section =".table"

#define APP_IMAGE_BASE 0xF8010000

/** default settings **/

/** external functions **/
extern __arm void __vector(void);

/** external data **/

/** internal functions **/

/** public data **/

/** private data **/
static const Int32U mt47h64m16_3_333_cl5[] @ ".table"=
{
	 	0x00000001
	,	0x00000000
	,	0x01000000
	,	0x00000101
	,	0x00000101
	,	0x01000000
	,	0x00010001
	,	0x00000100
	,	0x01010001
	,	0x00000201
	,	0x01000101
	,	0x06000002
	,	0x06060106
	,	0x03050502
	,	0x03040404
	,	0x02020503
	,	0x02010106
	,	0x03000405
	,	0x03040202
	,	0x04000305
	,	0x0707073f
	,	0x07070707
	,	0x06060607
	,	0x06060606
	,	0x05050506
	,	0x05050505
	,	0x04040405
	,	0x04040404
	,	0x03030304
	,	0x03030303
	,	0x02020203
	,	0x02020202
	,	0x01010102
	,	0x01010101
	,	0x0a0a0a01
	,	0x0000023f
	,	0x00050a00
	,	0x11000000
	,	0x00001302
	,	0x00000A0A
	,	0x72000000
	,	0x00550000
	,	0x2b050e86
	,	0x00640064
	,	0x00640064
	,	0x00640064
	,	0x00000064
	,	0x00200020
	,	0x00200020
	,	0x00200020
	,	0x00200020
	,	0x00200020
	,	0x00200020
	,	0x00200020
	,	0x00000a24
	,	0x43C20000
	,	0x5b1c00c8
	,	0x00c8002e
	,	0x00000000
	,	0x0001046b
	,	0x00000000
	,	0x03030100
	,	0x03030303
	,	0x03030303
	,	0x03030303
	,	0x00210000
	,	0x00010021
	,	0x00200000
	,	0x006c0090
	,	0x003fffff
	,	0x003fffff
	,	0x00000000
	,	0x00000000
	,	0x003fffff
	,	0x003fffff
	,	0x00000000
	,	0x00000000
	,	0x003fffff
	,	0x003fffff
	,	0x00000000
	,	0x00000000
	,	0x003fffff
	,	0x003fffff
	,	0x00000000
	,	0x00000000
	,	0x003fffff
	,	0x003fffff
	,	0x00000000
	,	0x00000000
	,	0x003fffff
	,	0x003fffff
	,	0x00000000
	,	0x00000000
	,	0x003fffff
	,	0x003fffff
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
};

/** public functions **/

/*************************************************************************
 * Function Name: bootload
 * Parameters: none
 *
 * Return: none
 *
 * Description: bootloader for iMX31 PDK. Inits Clock and SDRAM controller
 *              Copies a program from NAND flash to SDRAM
 *
 *************************************************************************/
void bootload(void) @ ".bootloader"
{
Int32U * pSrc;
Int32U * pDest;
Int32U size;
Int32U reg;

  /* SYSTEM PLL INIT */
  /* setting SYSCTL to SLOW mode */
  SCCTRL_bit.ModeCtrl = 2;
  /* setting 100us pll timer */
  SCPLLCTRL_bit.PllTime = 0x1FFFFFF;

  /* programming PLL1 */
  /* M=166, P=1, N=15 */
  PLL1_FRQ  = 0xA600010F;
  /*  power down : pll1 ctrl programming */
  PLL1_CTR = 0x1C0A;
  /* enable pll1 */
  PLL1_CTR = 0x1C0E;
  /* Strobe pll1 */
  PLL1_CTR = 0x1C06;
  PLL1_CTR = 0x1C0E;
  /* wait for lock */
  while(!PLL1_CTR_bit.pll_lock);

  /* programming PLL2 */
  /* M=166, P=1, N=15 */
  PLL2_FRQ  = 0xA600010F;
  /* enable pll2 */
  PLL2_CTR = 0x1C0A;
  /* enable pll2 */
  PLL2_CTR = 0x1C0E;
  /* Strobe pll2 */
  PLL2_CTR = 0x1C06;
  PLL2_CTR = 0x1C0E;
  /* wait for lock */
  while(!PLL2_CTR_bit.pll_lock);

  /* enable plltimeen */
  PRPH_CLK_CFG = 2;
  /* set pclkdiv & hclkdiv */
  CORE_CLK_CFG = 0x555;

  /* setting SYSCTL to NORMAL mode */
  SCCTRL_bit.ModeCtrl = 4;
  /* wait for normal mode */
  while(4 != SCCTRL_bit.ModeStatus);

  /* enable clock of all peripherals */
  PERIP1_CLK_ENB =  PERIP1_CLK_ENB | 0xFFFFFFF8;
  /* remove reset of all peripherals without Arm-2 subsystem */
  PERIP1_SOF_RST = 4;

  /* SMI init */
  SMI_CR1 = (0xFUL << 0)   /* Enable all banks */
          | (0x5UL << 4)   /* Deselect time */
          | (0x3UL << 8)   /* Prescaler */
          | (0x1UL <<15)   /* Fast */
          ;
  SMI_CR2 = 0;  // select bank 0

  /* Init DDR2 memory */
  /* EXT REGS CONFIG */
  /* DDRCORE disable to switch core frequency */
  reg = PERIP1_CLK_ENB;
  reg &= ~(1UL << 29);
  reg |=   1UL << 30;
  PERIP1_CLK_ENB = reg;
  PERIP1_CLK_ENB = reg;
  /* PLL_REG (switch core frequency) PLL2 */
  PLL_CLK_CFG_bit.mctr_clk_sel = 3;
  /* DDRCORE enable after switch core frequency */
  reg = PERIP1_CLK_ENB;
  reg &= ~(1UL << 30);
  reg |=   1UL << 29;
  PERIP1_CLK_ENB = reg;
  /* PAD_REG */
  reg = SSTLPAD_CFG_CTR;
  reg &= 0xffff0000UL;
  reg |= 0x0000eaadUL;
  SSTLPAD_CFG_CTR = reg;
  /* COMP3V3_REG */
  reg = COMPCOR_3V3_CFG;
  reg &=  0x8080ffe0UL;
  reg |=  0x78000002UL;
  COMPCOR_3V3_CFG = reg;
  /* COMP1V8_REG (START) */
  reg = COMPSSTL_1V8_CFG;
  reg &=  0x8080ffc0UL;
  reg |=  0x78000004UL;
  /* COMP2V5_REG (START) */
  reg = COMPSSTL_2V5_CFG;
  reg &=  0x8080ffc0UL;
  reg |=  0x78000004UL;
  COMPSSTL_2V5_CFG = reg;
  /* COMPENSATION 1V8 SELECTED */
	/* COMP2V5_REG (IDDQ) */
  reg = COMPSSTL_2V5_CFG;
  reg &=  0x8080ffc0UL;
  reg |=  0x78000003UL;
  COMPSSTL_2V5_CFG = reg;
  /* COMP1V8_REG (NORM) */
  reg = COMPSSTL_1V8_CFG;
  reg &=  0x8080ffc0UL;
  reg |=  0x78000010UL;
  COMPSSTL_1V8_CFG = reg;
  while(!COMPSSTL_1V8_CFG_bit.accurate);

  /* MPMC CONFIG */
  volatile unsigned long * p_reg = &MEM0_CTL;
  for(Int32U i = 0; i < sizeof(mt47h64m16_3_333_cl5)/sizeof(mt47h64m16_3_333_cl5[0]); i++)
  {
    *p_reg++ = mt47h64m16_3_333_cl5[i];
  }
  /* MPMC START */
  MEM7_CTL = 0x01000100;


  // transfer data from serial NOR to DDR
  pSrc  = (Int32U * )APP_IMAGE_BASE;
  pDest = (Int32U * )__vector;
  size = pSrc[5]; // reservet vector (14)

  for(Int32U i = 0; i < size; i += 4)
  {
    *pDest++ = *pSrc++;
  }
  /*Jump To pogram Start*/
  __vector();
}

/** private functions **/
