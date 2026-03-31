/**
  @page SD_ReadWrite_IT  Description of the SD_ReadWrite_IT example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    SD/SD_ReadWrite_IT/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the SD read and write buffer with IT mode example.
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
This example shows how to support UHS-I SD Card and achieve a frequency of 100MHz.
This example performs some write and read transfers to SD Card with SDMMC IP IT
mode and calculate write and read transfer speed.To achieve a transfer frequency 
of 100 MHz, IP SDMMC configure the existing transceiver on the Eval board to switch 
in 1.8V mode.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

An initialization of IO expender  is performed to initialize Transceiver pin to switch in
1.8V mode. Enabling this pin is done following HAL_SD_Enable_1_8V_Mode() callback.

The transfer clock is set at 100MHz (SDMMC_CLK / Clock_DIV * 2) are configured within the 
SD Handle init parameters.
During initialization, the driver check SD Card capability and switch to 1.8V mode if 
supported. Next, a write transfer of 100MB to SD Card is performed to calculate the write speed.
Then with the same parameters, a read transfer of 100MB from SD Card is performed to calculate 
read speed.
The transfers of write and read are performed with the IT mode.
The operation of writing and reading is performed in infinity loop to measure the 
SD card performance.


STM32H743I-EVAL board's LEDs can be used to monitor the transfer status:
 - LED_Orange is Toggle when transfer is on going (read or write).
 - LED_Green is ON when write and read transfer is completed and check buffers are OK.
 - LED_Red is ON when there is an error in transfer process or initialization.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - SD/SD_ReadWrite_IT/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - SD/SD_ReadWrite_IT/Inc/main.h                  Header for main.c module  
  - SD/SD_ReadWrite_IT/Src/stm32h7xx_it.c          Interrupt handlers
  - SD/SD_ReadWrite_IT/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - SD/SD_ReadWrite_IT/Src/main.c                  Main program
  - SD/SD_ReadWrite_IT/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - SD/SD_ReadWrite_IT/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.
  - An SD card is need to be plugged to the CN13 connector in Eval Board.
   
  - You can display test message  on the HyperTerminal,
      * You need to connect a null-modem female/female RS232 cable between the DB9 connector 
        CN2 (USART1) and PC serial port.
      * Make sure that jumper JP7 is on TX position and JP8 is on RX position
      * Configure your HyperTerminal with UART configuration:
        - Data Length = 7 Bits
        - One Stop Bit
        - Odd parity
        - BaudRate = 9600 baud
        - Flow control: None


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Plug a UHS-I SD Card in Eval board
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
