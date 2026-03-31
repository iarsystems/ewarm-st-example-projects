/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : bootloader.c
 *    Description : bootloader for SPEAR300 board
 *
 *
 *    History :
 *    2. Date        : December 9, 2010
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
	 	0x02020201
	,	0x02020202
	,	0x01000000
	,	0x00000101
	,	0x00000101
	,	0x01000000
	,	0x00010001
	,	0x00000100
	,	0x00010001
	,	0x01020203
	,	0x01000102
	,	0x04000202
	,	0x03030104
	,	0x03020202
	,	0x01040000
	,	0x00000001
	,	0x00000000
	,	0x03000405
	,	0x03040002
	,	0x04000305
	,	0x0505053f
	,	0x05050505
	,	0x04040405
	,	0x04040404
	,	0x03030304
	,	0x03030303
	,	0x02020203
	,	0x02020202
	,	0x01010102
	,	0x01010101
	,	0x00000001
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x0a0c0a00
	,	0x0000023f
	,	0x00050a00
	,	0x11000000
	,	0x00001302
	,	0x00001c1c
	,	0x7c000000
	,	0x005c0000
	,	0x2b050e00
	,	0x00640064
	,	0x00640064
	,	0x00000064
	,	0x00000000
	,	0x00200020
	,	0x00200020
	,	0x00200020
	,	0x00200020
	,	0x00200020
	,	0x00000000
	,	0x00000000
	,	0x00000a24
	,	0x00000000
	,	0x5b1c00c8
	,	0x00c8002e
	,	0x00000000
	,	0x0001046b
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x001c0000
	,	0x0019001c
	,	0x00100000
	,	0x001e007a
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x01010001
	,	0x01000000
	,	0x00000001
	,	0x00400000
	,	0x00000000
	,	0x00000000
	,	0x00000000
	,	0x00860000
	,	0x00000002
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

  /* SYSTEM PLL INIT */
  /* setting SYSCTL to SLOW mode */
  SCCTRL_bit.ModeCtrl = 2;
  /* setting 100us pll timer */
  SCPLLCTRL_bit.PllTime = 0x2000000UL - 2500UL;
  /* programming PLL1 */
  /* M=166, P=1, N=12 */
  PLL1_FRQ  = 166UL << 24;
  PLL1_FRQ_bit.pll_postdiv_P = 1;
  PLL1_FRQ_bit.pll_prediv_N  = 12;
  /*  power down : pll1 ctrl programming */
  PLL1_CTR = (1UL << 1) /* Reset */
           | (1UL << 3)
           ;
  /* enable pll1 */
  PLL1_CTR_bit.pll_enable   = 1;
  /* Strobe pll1 */
  PLL1_CTR_bit.pll_control1 = 0;
  PLL1_CTR_bit.pll_control1 = 1;
  /* wait for lock */
  while(!PLL1_CTR_bit.pll_lock);
  /* enable plltimeen */
  PRPH_CLK_CFG_bit.plltimeen = 1;
  /* set pclkdiv & hclkdiv */
  CORE_CLK_CFG = (1UL << 0) /* 1:2 ARM subsystem PCLK clock */
               | (1UL << 4) /* 1:2 Basic subsystem PCLK clock */
               | (1UL << 8) /* 1:2 HCLK to PCLK clock */
               | (1UL <<10) /* 1:2 PLL1_clkout to HCLK */
               ;
  /* SMI init */
  SMI_CR1 = (0xFUL << 0)   /* Enable all banks */
          | (0x5UL << 4)   /* Deselect time */
          | (0x3UL << 8)   /* Prescaler */
          | (0x1UL <<15)   /* Fast */
          ;
  SMI_CR2 = 0;  // select bank 0

  /* setting SYSCTL to NORMAL mode */
  SCCTRL_bit.ModeCtrl = 4;
  /* wait for normal mode */
  while(4 != SCCTRL_bit.ModeStatus);
  /* enable clock of all peripherals */
  PERIP1_CLK_ENB =  PERIP1_CLK_ENB | 0xFFFFFFF8;
  /* remove reset of all peripherals */
  PERIP1_SOF_RST = 0;

  /* Init DDR2 memory */
  /* EXT REGS CONFIG */
  /* DDRCORE disable to switch core frequency */
  PERIP1_CLK_ENB_bit.ddr_core_enb = 0;
  PERIP1_CLK_ENB_bit.ddr_core_enb = 0;
  /* PLL_REG (switch core frequency) PLL1 */
  PLL_CLK_CFG_bit.mctr_clk_sel = 1;
  /* DDRCORE enable after switch core frequency */
  PERIP1_CLK_ENB_bit.ddr_core_enb = 1;
  /* PAD_REG */
  DDR_PAD = 0x00033AAC;
  /* COMP1V8_REG (NORMAL) */
  COMPSSTL_1V8_CFG_bit.rasrc = 0x78;
  COMPSSTL_1V8_CFG_bit.compen = 0x0;
  COMPSSTL_1V8_CFG_bit.freeze = 0x0;
  COMPSSTL_1V8_CFG_bit.comptq = 0x0;
  COMPSSTL_1V8_CFG_bit.accurate = 0x1;
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
