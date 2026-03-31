/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : August 22, 2011
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use IAR Embedded Workbench for ARM
 * to develop code for a SPEAR600 board. It shows basic use of I/O, system
 * initialization PLL, DDR2, VIC, MMU, GPIO and timers.
 *
 *  Set the jumpers and the switches regarding the EVALSPEAR600 User's Manual
 *
 * It toggle port RS_PORT bit 4-5 (LEDs D5, D6) 15 times per second.
 *
 *    $Revision: 39 $
 *
 **************************************************************************/
#include <intrinsics.h>
#include <assert.h>
#include "board.h"
#include "arm_comm.h"
#include "arm926ej_cp15_drv.h"
#include "ttbl.h"

#define GPT_CLK       (48MHZ)
#define TICK_PER_SEC  15UL
#define USE_TIMER_ML  1

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

  vector = VIC0VECTADDR;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  if(interrupt_function != NULL)
  {
    interrupt_function();  // Call vectored interrupt function.
  }
  else
  {
    VIC0VECTADDR = 0;      // Clear interrupt in VIC.
  }
}

/*************************************************************************
 * Function Name: TIMERML_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: TIMERML interrupt handler
 *
 *************************************************************************/
void TIMERML_Handler (void)
{
static Int32U led_count;

#ifdef USE_TIMER_ML
  TIMERML_STATUS_INT_ACK1 = 1;  // Clear mach interrupt
#else
  TIMER1_STATUS_INT_ACK1 = 1;   // Clear mach interrupt
#endif
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
  ++led_count;
  VIC0VECTADDR = 0;
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
  VIC0INTSELECT  =  0;
  // Disable all interrupts
  VIC0INTENCLEAR = 0xFFFFFFFF;
  // Clear all software interrupts
  VIC0SOFTINTCLEAR = 0xFFFFFFFF;
  // VIC registers can be accessed in User or privileged mode
  VIC0PROTECTION = 0;
  // Clear interrupt
  VIC0VECTADDR = 0;
  // Clear address of the Interrupt Service routine (ISR) for non-vectored IRQs.
  VIC0DEFVECTADDR = 0;

  // Clear address of the Interrupt Service routine (ISR) for vectored IRQs.
  VIC0VECTADDR0  = 0;
  VIC0VECTADDR1  = 0;
  VIC0VECTADDR2  = 0;
  VIC0VECTADDR3  = 0;
  VIC0VECTADDR4  = 0;
  VIC0VECTADDR5  = 0;
  VIC0VECTADDR6  = 0;
  VIC0VECTADDR7  = 0;
  VIC0VECTADDR8  = 0;
  VIC0VECTADDR9  = 0;
  VIC0VECTADDR10 = 0;
  VIC0VECTADDR11 = 0;
  VIC0VECTADDR12 = 0;
  VIC0VECTADDR13 = 0;
  VIC0VECTADDR14 = 0;
  VIC0VECTADDR15 = 0;

  // Disable all vectored IRQ slots
  VIC0VECTCNTL0  = 0;
  VIC0VECTCNTL1  = 0;
  VIC0VECTCNTL2  = 0;
  VIC0VECTCNTL3  = 0;
  VIC0VECTCNTL4  = 0;
  VIC0VECTCNTL5  = 0;
  VIC0VECTCNTL6  = 0;
  VIC0VECTCNTL7  = 0;
  VIC0VECTCNTL8  = 0;
  VIC0VECTCNTL9  = 0;
  VIC0VECTCNTL10 = 0;
  VIC0VECTCNTL11 = 0;
  VIC0VECTCNTL12 = 0;
  VIC0VECTCNTL13 = 0;
  VIC0VECTCNTL14 = 0;
  VIC0VECTCNTL15 = 0;

  // Assign all interrupt channels to IRQ
  VIC1INTSELECT  =  0;
  // Disable all interrupts
  VIC1INTENCLEAR = 0xFFFFFFFF;
  // Clear all software interrupts
  VIC1SOFTINTCLEAR = 0xFFFFFFFF;
  // VIC registers can be accessed in User or privileged mode
  VIC1PROTECTION = 0;
  // Clear interrupt
  VIC1VECTADDR = 0;
  // Clear address of the Interrupt Service routine (ISR) for non-vectored IRQs.
  VIC1DEFVECTADDR = 0;

  // Clear address of the Interrupt Service routine (ISR) for vectored IRQs.
  VIC1VECTADDR0  = 0;
  VIC1VECTADDR1  = 0;
  VIC1VECTADDR2  = 0;
  VIC1VECTADDR3  = 0;
  VIC1VECTADDR4  = 0;
  VIC1VECTADDR5  = 0;
  VIC1VECTADDR6  = 0;
  VIC1VECTADDR7  = 0;
  VIC1VECTADDR8  = 0;
  VIC1VECTADDR9  = 0;
  VIC1VECTADDR10 = 0;
  VIC1VECTADDR11 = 0;
  VIC1VECTADDR12 = 0;
  VIC1VECTADDR13 = 0;
  VIC1VECTADDR14 = 0;
  VIC1VECTADDR15 = 0;

  // Disable all vectored IRQ slots
  VIC1VECTCNTL0  = 0;
  VIC1VECTCNTL1  = 0;
  VIC1VECTCNTL2  = 0;
  VIC1VECTCNTL3  = 0;
  VIC1VECTCNTL4  = 0;
  VIC1VECTCNTL5  = 0;
  VIC1VECTCNTL6  = 0;
  VIC1VECTCNTL7  = 0;
  VIC1VECTCNTL8  = 0;
  VIC1VECTCNTL9  = 0;
  VIC1VECTCNTL10 = 0;
  VIC1VECTCNTL11 = 0;
  VIC1VECTCNTL12 = 0;
  VIC1VECTCNTL13 = 0;
  VIC1VECTCNTL14 = 0;
  VIC1VECTCNTL15 = 0;
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
unsigned long slot;
unsigned long source;

  assert(((16 > VicIrqSlot) && (32 > VicIntSource)) || ((16 <= VicIrqSlot) && (32 <= VicIntSource)));
  slot = VicIrqSlot  & 0xF;
  source = VicIntSource & 0x1F;
  // load base address of vectored address registers
  pReg = (16 > VicIrqSlot)?&VIC0VECTADDR0:&VIC1VECTADDR0;
  // Set Address of callback function to corresponding Slot
  *(pReg+slot) = (unsigned long)pIRQSub;
  // load base address of ctrl registers
  pReg =  (16 > VicIrqSlot)?&VIC0VECTCNTL0:&VIC1VECTCNTL0;
  // Set source channel and enable the slot
  *(pReg+slot) = source | 0x20;
  // Clear FIQ select bit
  pReg =  (16 > VicIrqSlot)?&VIC0INTSELECT :&VIC1INTSELECT ;
  *pReg &= ~(1ul<<source);
}

/*************************************************************************
 * Function Name: VIC_EnaIRQ
 * Parameters:  unsigned int VicIntSouce
 *
 * Return: void
 *
 * Description:  Enable interrupt
 *
 *************************************************************************/
void VIC_EnaIRQ(Int32U VicIntSource)
{
unsigned long volatile *pReg;
unsigned long source;
  source = VicIntSource & 0x1F;
  pReg = (32 > VicIntSource)?&VIC0INTENABLE :&VIC1INTENABLE;
  *pReg = 1ul<<source;
}

/*************************************************************************
 * Function Name: VIC_DisIRQ
 * Parameters:  unsigned int VicIntSouce
 *
 * Return: void
 *
 * Description:  Disable interrupt
 *
 *************************************************************************/
void VIC_DisIRQ(Int32U VicIntSource)
{
unsigned long volatile *pReg;
unsigned long source;
  source = VicIntSource & 0x1F;
  pReg =  (32 > VicIntSource)?&VIC0INTENCLEAR :&VIC1INTENCLEAR;
  *pReg = 1ul<<source;
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
  GPIOAFSEL &= ~(
                 LED0
               | LED1
             );
  LED_DIR  |=  LED_MASK;
  LED0_OFF();
  LED1_OFF();

  // Init VIC
  VIC_Init();

#ifdef USE_TIMER_ML
  // Init TIMER ML
  TIMERML_CONTROL1 = (0x8UL << 0)  // /256
                   | (0UL   << 4)  // Auto-reload mode
                   ;
  // Set ptimer period
  TIMERML_COMPARE1 = ((GPT_CLK/256)/TICK_PER_SEC) - 1;
  // Registering interrupt handler
  VIC_SetVectoredIRQ(TIMERML_Handler,0,16);
  // Clear match interrupt
  TIMERML_STATUS_INT_ACK1 = 1;
  // Enable timer intrrupt
  TIMERML_CONTROL1_bit.MATCH_INT = 1;
  // Enable Timer counting
  TIMERML_CONTROL1_bit.ENABLE = 1;
  // TIMERML VIC interrupt enable
  VIC_EnaIRQ(16);
#else
  // Init TIMER ML
  TIMER1_CONTROL1 = (0x8UL << 0)  // /256
                  | (0UL   << 4)  // Auto-reload mode
                   ;
  // Set ptimer period
  TIMER1_COMPARE1 = ((GPT_CLK/256)/TICK_PER_SEC) - 1;
  // Registering interrupt handler
  VIC_SetVectoredIRQ(TIMERML_Handler,16,32);
  // Clear match interrupt
  TIMER1_STATUS_INT_ACK1 = 1;
  // Enable timer intrrupt
  TIMER1_CONTROL1_bit.MATCH_INT = 1;
  // Enable Timer counting
  TIMER1_CONTROL1_bit.ENABLE = 1;
  // TIMERML VIC interrupt enable
  VIC_EnaIRQ(32);
#endif
  // Enable interrupts
  __enable_interrupt();

  while(1)
  {
  }
}
