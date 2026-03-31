/**
  @page BSP Example on how to use the BSP drivers
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples/BSP/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description STM32H7xx_Nucleo_144 Demo description.
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

This example provides a description of how to use the different BSP drivers.


At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The BSP examples detects the presence of Adafruit 1.8" TFT shield with joystick and uSD.

If the Adafruit shield is NOT connected, then:
 - Blue led (led2) blinks waiting the user button is pushed.
 - Then blue led swithces off and green & red leds start blinking at high frequency.
 - Pushing user button again and again blinking frequency decreases until looping 
   to high frequency.

If the Adafruit shield is connected, then this example shows how to use the different 
functionalities of LCD, SD card and joystick by switching between tests using user button.
At the beginning of each example the blue led (led2) blinks waiting the user button to start.
  - Firstly, use the joystick button to move a pointer inside a rectangle 
    (up/down/right/left) and change the pointer color(select).
  - Secondly, this example shows how to use the different LCD features to display 
    string with different fonts, to display different shapes and to draw a bitmap.
  - Thirdly, this example shows how to erase, write and read the SD card.
At the end of the three examples when pushing the user button the application loops 
to the beginning (first examples). 

@par Keywords

Board Support Package, BSP, Adafruit, TFT, LCD, Joystick, SD Card

@Note  If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2), it’s recommended to configure the latters as Write Through.
            This is ensured by configuring the memory attributes at MPU level in order to ensure cash coherence on SRAM1 and SRAM2.
            Please, refer to Template project for a typical MPU configuration.

@Note  If external memory is shared between several processors, it’s recommended to configure it as Write Back (bufferable), shareable and cacheable.
            The memory base address and size must be properly updated.
            The user needs to manage the cache coherence at application level

For more details about the MPU configuration and use, please refer to AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”

@par Directory contents

  - BSP/Src/main.c                     Main program
  - BSP/Src/system_stm32h7xx.c         STM32H7xx system clock configuration file
  - BSP/Src/stm32h7xx_it.c             Interrupt handlers 
  - BSP/Src/joystick.c                 Joystick feature
  - BSP/Src/lcd.c                      LCD drawing features
  - BSP/Src/sd.c                       SD features
  - BSP/Inc/main.h                     Main program header file  
  - BSP/Inc/stm32h7xx_hal_conf.h       HAL configuration file
  - BSP/Inc/stm32h7xx_it.h             Interrupt handlers header file
  - BSP/Inc/stlogo.h                   Image used for BSP example


@par Hardware and Software environment

 - This example runs on STM32H743ZI Devices 
  
 - This example has been tested with STMicroelectronics NUCLEO-H743ZI Rev.B board
   and can be easily tailored to any other supported device and development board.

 - NUCLEO-H743ZI Rev.B board Set-up
   - Connect the Adafruit 1.8" TFT shield (https://www.adafruit.com/products/802)
   - Make sure the SB146 is closed and the SB165 and SB174 are opened on the NUCLEO-H743ZI board
   
@par How to use it ? 

In order to make the program work, you must do the following:
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
- Run the example
 
      
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
