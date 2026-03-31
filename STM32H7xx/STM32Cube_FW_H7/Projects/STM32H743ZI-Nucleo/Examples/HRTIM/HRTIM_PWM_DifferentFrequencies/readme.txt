/**
  @page HRTIM_PWM_DifferentFrequencies HRTIM PWM Different Frequencies example
  
  @verbatim
  ********************* COPYRIGHT(c) 2017 STMicroelectronics *******************
  * @file    HRTIM/HRTIM_PWM_DifferentFrequencies/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the PWM signals generation with different frequencies
  *          using HRTIM1
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

This example shows how to configure the HRTIM1 peripheral to generate up to 6 
PWM signals with different time base configuration for each slave timer. 
Each of the five slave Timers (TIMA, TIMB and TIMC) is configured to generate 2 PWM
signals with same frequency and 2 different duty cycles.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz

In this example HRTIM1 input clock (HRTIM1CLK) is set to APB2 clock (PCLK2),
since APB2 prescaler is equal to 2.
  HRTIM1CLK = 2*PCLK2
  PCLK2 = HCLK/2

  => HRTIM1CLK = HCLK = SystemCoreClock/2

    HRTIM1 TIMA period is set to 0xFFFF, so the frequency of the TIMA PWM is 3.06 KHz
    HRTIM1 TIMA TA1 duty cycle = (HRTIM1_TIMA_CMP1/ HRTIM1_TIMA_PER)* 100 = 50%
    HRTIM1 TIMA TA2 duty cycle = (HRTIM1_TIMA_CMP1/ HRTIM1_TIMA_PER)* 100 = 25%
    
    HRTIM1 TIMB period is set to 0x1999, so the frequency of the TIMB PWM is 30,6 KHz
    HRTIM1 TIMB TB1 duty cycle = (HRTIM1_TIMB_CMP1/ HRTIM1_TIMB_PER)* 100 = 50%
    HRTIM1 TIMB TB2 duty cycle = (HRTIM1_TIMB_CMP1/ HRTIM1_TIMB_PER)* 100 = 25%
    
    HRTIM1 TIMC period is set to 0x28F, so the frequency of the TIMC PWM is 306 KHz
    HRTIM1 TIMC TC1 duty cycle = (HRTIM1_TIMC_CMP1/ HRTIM1_TIMC_PER)* 100 = 50%
    HRTIM1 TIMC TC2 duty cycle = (HRTIM1_TIMC_CMP1/ HRTIM1_TIMC_PER)* 100 = 25%


The PWM waveforms can be displayed using an oscilloscope.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - HRTIM/HRTIM_PWM_DifferentFrequencies/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - HRTIM/HRTIM_PWM_DifferentFrequencies/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - HRTIM/HRTIM_PWM_DifferentFrequencies/Inc/main.h                  Header for main.c module  
  - HRTIM/HRTIM_PWM_DifferentFrequencies/Src/stm32h7xx_it.c          Interrupt handlers
  - HRTIM/HRTIM_PWM_DifferentFrequencies/Src/main.c                  Main program
  - HRTIM/HRTIM_PWM_DifferentFrequencies/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - HRTIM/HRTIM_PWM_DifferentFrequencies/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with STMicroelectronics STM32H743ZI-Nucleo 
    board and can be easily tailored to any other supported device 
    and development board.      

  - STM32H743ZI-Nucleo Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
        - HRTIM1_TIMA_TA1 : PC.06 (pin 04 in CN12 connector)
        - HRTIM1_TIMA_TA2 : PC.07 (pin 19 in CN12 connector)
        - HRTIM1_TIMB_TB1 : PC.08 (pin 02 in CN12 connector)
        - HRTIM1_TIMB_TB2 : PA.08 (pin 23 in CN12 connector)
        - HRTIM1_TIMC_TC1 : PA.09 (pin 21 in CN12 connector)
        - HRTIM1_TIMC_TC2 : PA.10 (pin 33 in CN12 connector)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
