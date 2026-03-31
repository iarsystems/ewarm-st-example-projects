/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : October 4, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 * Jumpers:
 *  POWER_SELECT  - depend of power source
 *  USB_DIS         - 2-3
 *  RST-TRST        - Absence
 *  PWDW_D          - 1-2
 *  Jumper array J16 and J15 - 2-3 (near prototype area) when jtrace is used
 *
 *  The example implements WEB server.
 *   The default IP address is:
 *    192.168.0.100
 *   The physical MAC address is (defined in STR912_enet.h):
 *    00-ff-ff-ff-ff-ff
 *
 **************************************************************************
 * Copyright (c) 2001-2003, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 **************************************************************************
 *
 *    $Revision: 39 $
 *
 **************************************************************************/
#include "includes.h"
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

volatile Int32U DlyCount;
/*************************************************************************
 * Function Name: Tim0Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void Tim0Handler (void)
{
  // Clear TIM0 counter
  TIM_CounterCmd(TIM0, TIM_CLEAR);
  if(DlyCount)
  {
    --DlyCount;
  }
  // Clear TIM0 flag OC1
  TIM_ClearFlag(TIM0,TIM_FLAG_OC1);
}

/*************************************************************************
 * Function Name: InitClock
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init MCU clock
 *
 *************************************************************************/
void InitClock (void)
{
  // Clock
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);       // master clk - OSC clk
  // Flash controller init
  SCU_FMICLKDivisorConfig(SCU_FMICLK_Div1);
  FMI_Config(FMI_READ_WAIT_STATE_2,FMI_WRITE_WAIT_STATE_0, FMI_PWD_ENABLE,\
             FMI_LVD_ENABLE,FMI_FREQ_HIGH);
  // Set clks dividers
  SCU_RCLKDivisorConfig(SCU_RCLK_Div1);
  SCU_HCLKDivisorConfig(SCU_HCLK_Div1);
  SCU_PCLKDivisorConfig(SCU_PCLK_Div1);
  // Init PLL = 48 MHz
  SCU_PLLFactorsConfig(192,25,3);
  // PLL Enabled
  SCU_PLLCmd(ENABLE);
  // Switch clk MCLK = PLL
  SCU_MCLKSourceConfig(SCU_MCLK_PLL);
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Delay
 *
 * Return: none
 *
 * Description: Delay 100us * arg
 *
 *************************************************************************/
void DelayResolution100us(Int32U Delay)
{
  DlyCount = Delay;

  // Clear TIM0 counter
  TIM_CounterCmd(TIM0, TIM_CLEAR);
  // Clear TIM0 flag OC1
  TIM_ClearFlag(TIM0,TIM_FLAG_OC1);
  // Enable TIM0 OC1 interrupt
  TIM_ITConfig(TIM0, TIM_IT_OC1, ENABLE);
  // Enable TIM0 counter
  TIM_CounterCmd(TIM0, TIM_START);

  while(DlyCount);

  // Disable TIM0 OC1 interrupt
  TIM_ITConfig(TIM0, TIM_IT_OC1, DISABLE);
  // Disable TIM0 counter
  TIM_CounterCmd(TIM0, TIM_STOP);
}

/*************************************************************************
 * Function Name: InitDlyTimer
 * Parameters: Int32U IntrPriority
 *
 * Return: none
 *
 * Description: Init Delay Timer (TIM 0)
 *
 *************************************************************************/
void InitDlyTimer (Int32U IntrPriority)
{
TIM_InitTypeDef TIM_InitStructure;

  // Enable TIM0 clocks
  SCU_APBPeriphClockConfig(__TIM01, ENABLE);
  // Release TIM0 reset
  SCU_APBPeriphReset(__TIM01,DISABLE);

  // Timer 0
  // TIM Configuration in Output Compare Timing Mode period 100us
  TIM_InitStructure.TIM_Mode = TIM_OCM_CHANNEL_1;   // OUTPUT COMPARE CHANNEL 1 Mode
  TIM_InitStructure.TIM_OC1_Modes = TIM_TIMING;     // OCMP1 pin is disabled
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB; // assign PCLK to TIM_Clk
  TIM_InitStructure.TIM_Prescaler = 48 - 1;         // 1us resolution
  TIM_InitStructure.TIM_Pulse_Length_1 = 100;       // 100 us period
  TIM_Init(TIM0, &TIM_InitStructure);

  // VIC configuration
  VIC_Config(TIM0_ITLine, VIC_IRQ, IntrPriority);
  VIC_ITCmd(TIM0_ITLine, ENABLE);
}

/*************************************************************************
 * Function Name: uip_log
 * Parameters: none
 *
 * Return: none
 *
 * Description: Events loggin
 *
 *************************************************************************/
void uip_log (char *m)
{
  printf("uIP log message: %s\n", m);
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
int main(void)
{
unsigned int i;
uip_ipaddr_t ipaddr;
struct timer periodic_timer, arp_timer;
  // MCU clock init
  InitClock();
  // Enable the clock for the VIC
  SCU_AHBPeriphClockConfig(__VIC, ENABLE);
  // VIC Deinitialization
  VIC_DeInit();
  // Delay timer init
  InitDlyTimer(1);

  // Sys timer init 1/100 sec tick
  clock_init(2);

  timer_set(&periodic_timer, CLOCK_SECOND / 2);
  timer_set(&arp_timer, CLOCK_SECOND * 10);

  // Initialize the ethernet device driver
  tapdev_init();

  // Enable interrupts
  __enable_interrupt();

  // LCD Init
  HD44780_PowerUpInit();

  // Show meassges on LCD
  HD44780_StrShow(1, 1,  "  IAR Systems   ");
  HD44780_StrShow(1, 2,  " uIP WEB Server ");
  LCD_LIGHT_ON();

  // uIP web server
  // Initialize the uIP TCP/IP stack.
  uip_init();

  uip_ipaddr(ipaddr, 192,168,0,100);
  uip_sethostaddr(ipaddr);
  uip_ipaddr(ipaddr, 192,168,0,1);
  uip_setdraddr(ipaddr);
  uip_ipaddr(ipaddr, 255,255,255,0);
  uip_setnetmask(ipaddr);

  // Initialize the HTTP server.
  httpd_init();

  while(1)
  {
    uip_len = tapdev_read(uip_buf);
    if(uip_len > 0)
    {
      if(BUF->type == htons(UIP_ETHTYPE_IP))
      {
	      uip_arp_ipin();
	      uip_input();
	      /* If the above function invocation resulted in data that
	         should be sent out on the network, the global variable
	         uip_len is set to a value > 0. */
	      if(uip_len > 0)
        {
	        uip_arp_out();
	        tapdev_send(uip_buf,uip_len);
	      }
      }
      else if(BUF->type == htons(UIP_ETHTYPE_ARP))
      {
        uip_arp_arpin();
	      /* If the above function invocation resulted in data that
	         should be sent out on the network, the global variable
	         uip_len is set to a value > 0. */
	      if(uip_len > 0)
        {
	        tapdev_send(uip_buf,uip_len);
	      }
      }
    }
    else if(timer_expired(&periodic_timer))
    {
      timer_reset(&periodic_timer);
      for(i = 0; i < UIP_CONNS; i++)
      {
      	uip_periodic(i);
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if(uip_len > 0)
        {
          uip_arp_out();
          tapdev_send(uip_buf,uip_len);
        }
      }
#if UIP_UDP
      for(i = 0; i < UIP_UDP_CONNS; i++) {
        uip_udp_periodic(i);
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if(uip_len > 0) {
          uip_arp_out();
          tapdev_send();
        }
      }
#endif /* UIP_UDP */
      /* Call the ARP timer function every 10 seconds. */
      if(timer_expired(&arp_timer))
      {
        timer_reset(&arp_timer);
        uip_arp_timer();
      }
    }
  }
}
