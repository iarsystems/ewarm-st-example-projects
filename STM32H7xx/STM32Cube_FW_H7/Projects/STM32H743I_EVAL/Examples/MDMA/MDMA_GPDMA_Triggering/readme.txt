/**
  @page MDMA_GPDMA_Triggering MDMA with GPDMA triggering Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    MDMA/MDMA_GPDMA_Triggering/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the MDMA with GPDMA triggering Example.
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

This example describes how to use the MDMA with HW trigger set to D2 Domain GP-DMA
transfer complete flag.

Note that the MDMA has access to the  D-TCM RAM (Data Tightly Coupled Memory located @0x20000000) 
which is not the case of the D2 domain GP-DMAs. 
This examples shows how to receive data using a D2 GP-DMA
(DMA1 stream 5 in this case) and transfer these data automatically to the D-TCM RAM
where it can be processed by the CPU . Knowing that the CPU access to the D-TCM
RAM is faster then any other RAM of the device, CPU data processing will be faster.


At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The UART peripheral configuration is ensured by the UART_Config() function.
This later is calling the HAL_UART_Init() which in return calls HAL_UART_MspInit()
function which core is implementing the configuration of the needed UART resources 
according to the used hardware .You may update the function HAL_UART_MspInit() 
to change UART configuration.
Function UART_Config() send a start message to the user PC HyperTerminal to ask for
sending 12 characters.

The MDMA channel 0 is then configured in HW request mode with HW transfer request trigger
set to DMA1 stream5 transfer complete flag. Meaning that the MDMA will proceed for the transfer
once the DMA1 stream 5 ends its own transfer.
The MDMA channel 0 is configured in block transfer mode with source and destination
data size set to "byte".

The MDMA transfer is then started to transfer content of "aUART_RxBuffer" buffer 
to "aMDMA_RxBuffer" buffer knowing that MDMA HW trigger is DMA1 stream 5 which is 
used for UART reception.
As consequence MDMA transfer will be triggered once the UART received user message to 
"aUART_RxBuffer" buffer through the DMA1 stream 5, the MDMA will then automatically 
transfer it to the "aMDMA_RxBuffer" buffer.
note that "aMDMA_RxBuffer" buffer is located in the D-TCM RAM (@0x20000000).

The function HAL_UART_Receive_DMA() is then called to  allow 
the reception of Data from HyperTerminal to buffer "aUART_RxBuffer". 
Instead of waiting on the end of the UART reception operation, the program waits 
for the MDMA transfer operation ends or error. Indeed when MDMA transfer ends it means 
that the user message has been transferred using UART through the DMA1 stream 5 to 
"aUART_RxBuffer" buffer and the MDMA has transferred the content to the final 
destination "aMDMA_RxBuffer" buffer. 


the received MDMA Buffer "aMDMA_RxBuffer" is then sent back to the user using UART 
in polling model as the DMA1 has no access to the DTCM RAM where the  buffer "aMDMA_RxBuffer" 
is located.
    
In the final step an ending message is sent to the user. 

STM32 Eval board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the MDMA transfer process is complete.
 - LED3 is ON when there is an error in transmission/reception process.

The UART is configured as follows:
    - BaudRate = 9600 baud  
    - Word Length = 8 Bits (7 data bit + 1 parity bit)
    - One Stop Bit
    - Odd parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

@note USARTx/UARTx instance used and associated resources can be updated in "main.h"
      file depending hardware configuration used.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - MDMA/MDMA_GPDMA_Triggering/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - MDMA/MDMA_GPDMA_Triggering/Inc/stm32h7xx_it.h          DMA  and MDMA interrupt handlers header file
  - MDMA/MDMA_GPDMA_Triggering/Inc/main.h                  Header for main.c module
  - MDMA/MDMA_GPDMA_Triggering/Src/stm32h7xx_it.c          DMA and MDMA interrupt handlers
  - MDMA/MDMA_GPDMA_Triggering/Src/main.c                  Main program
  - MDMA/MDMA_GPDMA_Triggering/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - MDMA/MDMA_GPDMA_Triggering/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx device.
    
  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.

  - STM32H743I-EVAL Set-up
      Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN2 (USART1) and PC serial port if you want to display data on the HyperTerminal.

@note Make sure that jumper JP8 is on RX position for STM32H743I-EVAL RevB and jumper JP7 is on TX position. 

  - Hyperterminal configuration:
    - Data Length = 7 Bits
    - One Stop Bit
    - Odd parity
    - BaudRate = 9600 baud
    - Flow control: None

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
