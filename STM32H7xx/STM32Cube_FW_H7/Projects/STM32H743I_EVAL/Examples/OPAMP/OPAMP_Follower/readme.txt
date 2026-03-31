/**
  @page OPAMP_Follower Follow an applied signal example  
  @verbatim
  ********************* COPYRIGHT(c) 2017 STMicroelectronics *******************
  * @file    OPAMP/OPAMP_Follower/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the OPAMP Follower signals generation example.
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

This example shows how to configure OPAMP peripheral in follower mode interconnected 
with DAC and COMP.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
- The CPU at 400MHz 
- The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
- The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz
 
OPAMP1 is configured as following:
   - Follower mode.
   - OPAMP Non Inverting input is connected internally to DAC_OUT1 that perform a simple
     conversion in 8 bits right alignment of 0x7F value.
   - OPAMP Output is connected to COMP1 inverting input internally (share same pin PC4).

Test Description:
   
   - Push the Tamper button to enter normal mode( LED1 On).
   - Push the Tamper alternatively to switch from normal power mode to high speed mode 
     (LED2 On)and vice versa.
   - Inject voltage higher than 1.65V to COMP1 non inverting input( PB2 pin 24 in CN6). 
   - check COMP1 output(PA8 pin 52 in CN7).
   - If COMP1 output is high test PASS otherwise test FAIL.
   

@par Directory contents 

  - OPAMP/OPAMP_Follower/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - OPAMP/OPAMP_Follower/Inc/stm32h7xx_it.h          Header for stm32h7xx_it.c
  - OPAMP/OPAMP_Follower/Inc/main.h                  Header for main.c  
  - OPAMP/OPAMP_Follower/Src/stm32h7xx_it.c          Interrupt handlers
  - OPAMP/OPAMP_Follower/Src/main.c                  Main program
  - OPAMP/OPAMP_Follower/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - OPAMP/OPAMP_Follower/Src/system_stm32h7xx.c      STM32H7xx system clock configuration file

@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board..

  - STM32H743I-EVAL Set-up
    
      - Inject Volatge in COMP1 non inverting input( PB2 pin 24 in CN6)  
      - Connect an oscilloscope to (COMP1_OUT, PA8 pin52 in CN7)
      
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

