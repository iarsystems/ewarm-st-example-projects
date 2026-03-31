/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : July 1, 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use IAR Embedded Workbench for ARM
 * to develop code for a EVALSPEAR320PLC board. It shows basic use
 * of I/O, system initialization PLL, DDR2, VIC, MMU, GPIO and timer.
 *
 *  Set the jumpers and the switches regarding the EVALSPEAR320PLC User's Manual
 *
 *    $Revision: 39 $
 *
 **************************************************************************/
#include <intrinsics.h>
#include "board.h"
#include "arm_comm.h"
#include "arm926ej_cp15_drv.h"
#include "ttbl.h"

#define GPT_CLK       (48MHZ)
#define TICK_PER_SEC  15UL

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ handler
 *
 *************************************************************************/
__irq __arm void irq_handler (void)
{
void (*interrupt_function)();
unsigned int vector;

  vector = VICVECTADDR;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  if(interrupt_function != NULL)
  {
    interrupt_function();  // Call vectored interrupt function.
  }
  else
  {
    VICVECTADDR = 0;      // Clear interrupt in VIC.
  }
}

/*************************************************************************
 * Function Name: GPT1_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPT1 interrupt handler
 *
 *************************************************************************/
void GPT1_Handler (void)
{
static Int32U led_count;

  TIMER1_STATUS_INT_ACK1 = 1; // Clear mach interrupt
  if(led_count & 1)
  {
    LED0_ON();
  }
  else
  {
    LED0_OFF();
  }
  if(led_count & 2)
  {
    LED1_ON();
  }
  else
  {
    LED1_OFF();
  }
  if(led_count & 4)
  {
    LED2_ON();
  }
  else
  {
    LED2_OFF();
  }
  if(led_count & 8)
  {
    LED3_ON();
  }
  else
  {
    LED3_OFF();
  }
  ++led_count;
  VICVECTADDR = 0;
}


/*************************************************************************
 * Function Name: VIC_Init
 * Parameters: void
 * Return: void
 *
 * Description: Initialize VIC
 *
 *************************************************************************/
void VIC_Init(void)
{
  // Set Vectors address LO
  Int32U reg = CP15_GetStatus();
  reg &= ~CP15_CTRL_V;
  CP15_SetMmu(reg);
  // Assign all interrupt channels to IRQ
  VICINTSELECT  =  0;
  // Disable all interrupts
  VICINTENCLEAR = 0xFFFFFFFF;
  // Clear all software interrupts
  VICSOFTINTCLEAR = 0xFFFFFFFF;
  // VIC registers can be accessed in User or privileged mode
  VICPROTECTION = 0;
  // Clear interrupt
  VICVECTADDR = 0;
  // Clear address of the Interrupt Service routine (ISR) for non-vectored IRQs.
  VICDEFVECTADDR = 0;

  // Clear address of the Interrupt Service routine (ISR) for vectored IRQs.
  VICVECTADDR0  = 0;
  VICVECTADDR1  = 0;
  VICVECTADDR2  = 0;
  VICVECTADDR3  = 0;
  VICVECTADDR4  = 0;
  VICVECTADDR5  = 0;
  VICVECTADDR6  = 0;
  VICVECTADDR7  = 0;
  VICVECTADDR8  = 0;
  VICVECTADDR9  = 0;
  VICVECTADDR10 = 0;
  VICVECTADDR11 = 0;
  VICVECTADDR12 = 0;
  VICVECTADDR13 = 0;
  VICVECTADDR14 = 0;
  VICVECTADDR15 = 0;

  // Disable all vectored IRQ slots
  VICVECTCNTL0  = 0;
  VICVECTCNTL1  = 0;
  VICVECTCNTL2  = 0;
  VICVECTCNTL3  = 0;
  VICVECTCNTL4  = 0;
  VICVECTCNTL5  = 0;
  VICVECTCNTL6  = 0;
  VICVECTCNTL7  = 0;
  VICVECTCNTL8  = 0;
  VICVECTCNTL9  = 0;
  VICVECTCNTL10 = 0;
  VICVECTCNTL11 = 0;
  VICVECTCNTL12 = 0;
  VICVECTCNTL13 = 0;
  VICVECTCNTL14 = 0;
  VICVECTCNTL15 = 0;
}

/*************************************************************************
 * Function Name: VIC_SetVectoredIRQ
 * Parameters:  void(*pIRQSub)()
 *              LPC_VicIrqSlots_t VicIrqSlot
 *              unsigned int VicIntSouce
 *
 * Return: void
 *
 * Description:  Init vectored interrupts
 *
 *************************************************************************/
void VIC_SetVectoredIRQ(void(*pIRQSub)(), Int32U VicIrqSlot, Int32U VicIntSource)
{
unsigned long volatile *pReg;
  // load base address of vectored address registers
  pReg = &VICVECTADDR0;
  // Set Address of callback function to corresponding Slot
  *(pReg+VicIrqSlot) = (unsigned long)pIRQSub;
  // load base address of ctrl registers
  pReg = &VICVECTCNTL0;
  // Set source channel and enable the slot
  *(pReg+VicIrqSlot) = VicIntSource | 0x20;
  // Clear FIQ select bit
  VICINTSELECT &= ~(1<<VicIntSource);
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
int main(void)
{
  // Init MMU
  CP15_Mmu(FALSE);            // Disable MMU
  // Privileged permissions  User permissions AP
  // Read-only               Read-only        0
  CP15_SysProt(FALSE);
  CP15_RomProt(TRUE);
  CP15_InitMmuTtb(TtSB,TtTB); // Build L1 and L2 Translation tables
  CP15_SetTtb(L1Table);       // Set base address of the L1 Translation table
  CP15_SetDomain( (DomainManager << 2*1) | (DomainClient << 0)); // Set domains
  CP15_Mmu(TRUE);             // Enable MMU
  CP15_Cache(TRUE);           // Enable ICache,DCache

  // LEDs Init
  LED_SEL_PORT1 |= LED_MASK1;
  LED_SEL_PORT2 |= LED_MASK2;
  LED_DIR1 &= ~LED_MASK1;
  LED_DIR2 &= ~LED_MASK2;
  LED_DIR1 &= ~LED_MASK1;
  LED_DIR2 &= ~LED_MASK2;
  LED0_OFF();
  LED1_OFF();
  LED2_OFF();
  LED3_OFF();

  // Init VIC
  VIC_Init();

  // Init GPT1
  TIMER1_CONTROL1 = (0x8UL << 0)  // /256
                  | (0UL   << 4)  // Auto-reload mode
                  ;
  // Set ptimer period
  TIMER1_COMPARE1 = ((GPT_CLK/256)/TICK_PER_SEC) - 1;
  // Registering interrupt handler
  VIC_SetVectoredIRQ(GPT1_Handler,0,4);
  // Clear match interrupt
  TIMER1_STATUS_INT_ACK1 = 1;
  // Enable timer intrrupt
  TIMER1_CONTROL1_bit.MATCH_INT = 1;
  // Enable Timer counting
  TIMER1_CONTROL1_bit.ENABLE = 1;
  // GPT1 VIC interrupt enable
  VICINTENABLE = 1UL<<4;
  // Enable interrupts
  __enable_interrupt();

  while(1)
  {
  }
}
