/**
  @page DMAMUX_SYNC DMA & DMAMUX Synchronization Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DMA/DMAMUX_SYNC/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the DMA & DMAMUX Synchronization Example.
  ******************************************************************************
  * 
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 
This example shows how to use the DMA with the DMAMUX to synchronize a transfer with LPTIM1 output signal.
The USART1 is used in DMA synchronized mode to send a countdown from 10 to 00 with a period of 2sec. 

The example uses the USART1 in DMA synchronized mode to send a countdown from 10 to 00 with 2sec period.
The DMAMUX synchronization block is configured to synchronize the DMA transfer with the LPTIM1 output signal.
Each rising edge of the synchronization signal (i.e LPTIM1 output signal) will authorize 4 USART1 requests to be
transmitted to the the USART1 peripheral using the DMA. these four requests represent the 2 characters '\n\r' plus 
the 2 characters count down itself from '10' to '00'.
The LPTIM1 is configured to generate a PWM with 2 seconds period.


The example requires to connect the board to an HyperTerminal PC application through USART1 connector (CN2).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.


The DMA is configured in memory to peripheral mode to ensure data transfer 
from the source transmission buffer (TxSyncMessage) to the USART1 (in order to be transmitted to the UART/HyperTerminal).

The DMAMUX synchronization block is configured using function "HAL_DMAEx_ConfigMuxSync" with the following parameters :
- SyncSignalID  : set to HAL_DMAMUX1_SYNC_LPTIM1_OUT which corresponds to LPTIM1_OUT signal.
- SyncPolarity  : Set to RISING to use rising edge of LPTIM1 output signal for synchronization.
- RequestNumber : 4 i.e four USART1 requests are authorized after each rising edge of the sync signal.
- EventState : enabled , in order to generate an event each time "RequestNumber" are transmitted.
  Note that this event could be used with the DMAMUX external request generator to trigger another DMA stream transfer.

The LPTIM1 is configured using function "LPTIM_Config" in order to configure and start a PWM on the LPTIM1 timer with
2 seconds period and 50% duty cycle.

The USART1 peripheral configuration is ensured by the HAL_UART_Init() function.
This later is calling the HAL_UART_MspInit()function which core is implementing
the configuration of the needed UART resources according to the used hardware (CLOCK, 
GPIO and NVIC). 
DMA & DMAMUX configurations are performed in the main.c module as the example first purpose is to show 
the DMA & DMAMUX configuration when using the DMAMUX synchronization block. 
In normal conditions DMA and DMAMUX configuration should be placed in the HAL_UART_MspInit()function.

A first example brief description message is then transmitted using the USART1 to the PC HyperTerminal
in Polling mode (using message buffer ).

Then a second UART transfer is started in DMA synchronized mode to transfer the countdown buffer "TxEndMessage".
As result the countdown from 10 to 00 with a period of 2 seconds should be displayed on the HyperTerminal. 
The end of this step is monitored through the HAL_UART_GetState() function result.

In the last step an ending message is sent using the USART1 in polling mode through the "TxEndMessage" buffer.  

STM32 board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the transmission process is complete.
 - LED3 is ON when there is an error in transmission process.  

The UART is configured as follows:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - parity none
    - Hardware flow control disabled (RTS and CTS signals)

@note USARTx/UARTx instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

@note This example can be easily modified to use any other peripheral (than the USART1) with a DMA synchronized transfer.
In this case user will need to replace the USART1 initialization and processing by the used IP initialization and processing functions.
User can also change the synchronization signal (based on the sync signals list available in the reference manual),
in this case the sync signal IP should be configured (replacing the function LPTIM_Config by the required sync signal IP configuration)

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - DMA/DMAMUX_SYNC/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - DMA/DMAMUX_SYNC/Inc/stm32h7xx_it.h          DMA interrupt handlers header file
  - DMA/DMAMUX_SYNC/Inc/main.h                  Header for main.c module  
  - DMA/DMAMUX_SYNC/Src/stm32h7xx_it.c          DMA interrupt handlers
  - DMA/DMAMUX_SYNC/Src/main.c                  Main program
  - DMA/DMAMUX_SYNC/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - DMA/DMAMUX_SYNC/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx device.
    
  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.

  - STM32H743I-EVAL Set-up
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN2 (USART1) and PC serial port in order to display data on the HyperTerminal.


  - HyperTerminal configuration:
    - Word Length = 8 Bits
    - One Stop Bit
    - None parity
    - Baud Rate = 115200 baud
    - flow control: None 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
