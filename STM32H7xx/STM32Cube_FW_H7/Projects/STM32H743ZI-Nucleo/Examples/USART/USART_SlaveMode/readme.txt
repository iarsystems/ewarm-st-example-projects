/**
  @page USART_SlaveMode USART Slave Mode example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    USART/USART_SlaveMode/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the USART Slave Mode example.
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

This example describes an USART-SPI communication (transmit/receive) between two
boards where he USART is configured as a slave.

Boards: STM32H743ZI-Nucleo (embeds a STM32H743ZI device)

   _________________________                        _________________________
  |           ______________|                      |______________           |
  |          |     SPI1     |                      |    USART2    |          |
  |          |              |                      |              |          |
  |          |  PA5(CN7.D13)|SCK_________________CK|(CN9.D51)PD7  |          |
  |          |              |                      |              |          |
  |          |  PA6(CN7.D12)|MISO________________TX|(CN9.D53)PD5  |          |
  |          |              |                      |              |          |
  |          |  PA7(CN7.D11)|MOSI________________RX|(CN9.D52)PD6  |          |
  |          |              |                      |              |          |
  |          |______________|                      |______________|          |
  |      __                 |                      |                         |
  |     |__|                |                      |                         |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32H7_Master__________|                      |_STM32H7_Slave___________|


Two identical boards are connected as shown on the picture above.
Board 1: SPI Master board
Board 2: USART Slave board

In a first step, after the user press the User push-button, Master (SPI) starts
the communication by sending aTxBuffer and receiving aRxBuffer through
HAL_SPI_TransmitReceive(), at the same time Slave (USART) transmits aTxBuffer
and receives aRxBuffer through HAL_USART_TransmitReceive().
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to
check buffers correctness.

WARNING: as both boards do not behave the same way, "SLAVE_BOARD" compilation
switch is defined in /Src/main.c and must be enabled at compilation time before
loading the executable in the board that first transmits then receives.
The receiving then transmitting board needs to be loaded with an executable
software obtained with SLAVE_BOARD disabled.

STM32H743ZI-Nucleo board LEDs are used to monitor the transfer status:
- While board 1 is waiting for the user to press the User push-button, its
  LED_BLUE is blinking rapidly (100 ms period).
- After the transmission/reception processes begin LED_BLUE is turned off.
- When the test passes, LED_GREEN on both boards is turned on, otherwise the
  test has failed. 
- If there is an initialization or transfer error, LED_RED is turned on.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

@note USARTx/SPIx instances used and associated resources can be updated in
"main.h" file depending hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower) than
      the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use
      HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.

@par Directory contents 

  - USART/USART_SlaveMode/Inc/stm32h7xx_hal_conf.h HAL configuration file
  - USART/USART_SlaveMode/Inc/stm32h7xx_it.h       interrupt handlers header file
  - USART/USART_SlaveMode/Inc/main.h               Header for main.c module  
  - USART/USART_SlaveMode/Src/stm32h7xx_it.c       interrupt handlers
  - USART/USART_SlaveMode/Src/main.c               Main program
  - USART/USART_SlaveMode/Src/stm32h7xx_hal_msp.c  HAL MSP module
  - USART/USART_SlaveMode/Src/system_stm32h7xx.c   STM32H7xx system source file


@par Hardware and Software environment 

  - This example runs on STM32H743xx devices.    

  - This example has been tested with two STM32H743ZI-Nucleo boards embedding
    a STM32H743ZI device and can be easily tailored to any other supported device 
    and development board.

  - STM32H743ZI-Nucleo set-up
    - Connect Master board PA5 to Slave Board PD7
    - Connect Master board PA6 to Slave Board PD5
    - Connect Master board PA7 to Slave Board PD6
    - Connect Master board GND to Slave Board GND   

@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory :
    - Uncomment "#define SLAVE_BOARD" and load the project in Slave Board
    - Comment "#define SLAVE_BOARD" and load the project in Master Board
  - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
