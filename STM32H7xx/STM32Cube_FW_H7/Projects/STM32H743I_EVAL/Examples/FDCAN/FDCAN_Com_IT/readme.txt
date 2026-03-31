/**
  @page FDCAN_Com_IT FDCAN Communication IT example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FDCAN/FDCAN_Com_IT/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the FDCAN_Com_IT ewample.
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

This example shows how to achieve Interrupt Process Communication between two
FDCAN units.

Boards: STM32H743I-EVAL (embeding an STM32H743xx device)

   _________________________                       _________________________ 
  |           ______________|                     |______________           |
  |          |FDCAN1        |                     |        FDCAN1|          |
  |          |              |                     |              |          |
  |          |       CAN    |_____________________|    CAN       |          |
  |          |    connector |                     | connector    |          |
  |          |              |                     |              |          |
  |          |______________|                     |______________|          |
  |                         |                     |                         |
  |   _    _           _    |                     |   _    _           _    |
  |  |_|  |_|         (_)   |                     |  |_|  |_|         (_)   |
  | LED3  LED1      Tamper  |                     | LED3  LED1      Tamper  |
  |                         |                     |                         |
  |                         |                     |                         |
  |      STM32 Board 1      |                     |      STM32 Board 2      |
  |                         |                     |                         |
  |_________________________|                     |_________________________|

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices:
 - The CPU at 400MHz 
 - The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
 - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 100MHz.

Then, on both boards :
 - FDCAN module is configured to receive messages with pre-defined ID to its Rx FIFO 0.
 - Rx FIFO 0 Watermark is configured to 2 and the corresponding notification is activated.
 - FDCAN module is started.

When Tamper push-button is pressed in any Board:
 - its LED1 is turned OFF
 - its FDCAN module sends 2 messages

On the other board, after receiving the 2 messages:
 - received payload is compared to expected data
 - if the result is OK, LED1 is tuned ON

If at any time of the process an error is encountered LED3 is turned ON.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@par Directory contents 

  - FDCAN/FDCAN_Com_IT/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - FDCAN/FDCAN_Com_IT/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - FDCAN/FDCAN_Com_IT/Inc/main.h                  Header for main.c module  
  - FDCAN/FDCAN_Com_IT/Src/stm32h7xx_it.c          Interrupt handlers
  - FDCAN/FDCAN_Com_IT/Src/main.c                  Main program
  - FDCAN/FDCAN_Com_IT/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - FDCAN/FDCAN_Com_IT/Src/system_stm32h7xx.c      stm32h7xx system source file

@par Hardware and Software environment 

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with a couple of STM32H743I-EVAL boards embedding a 
    STM32H743xx device and can be easily tailored to any other supported device 
    and development board.
    
  - STM32H743I-EVAL set-up
    - Connect a CAN cable to the CAN connectors of the two EVAL boards (CN3)
    - Jumpers JP1 and JP2 => fitted
    - Solder bridges SB59 and SB60 => open
    - Solder bridge SB50 => closed

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memories
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
