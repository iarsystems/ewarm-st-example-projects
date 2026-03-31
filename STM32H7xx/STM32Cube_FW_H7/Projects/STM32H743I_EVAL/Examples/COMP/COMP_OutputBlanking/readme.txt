/**
  @page COMP_OutputBlanking COMP output blanking example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    COMP/COMP_OutputBlanking/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the COMP output blanking Example
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

This example shows how to use the output blanking feature of COMP peripheral.
The purpose of the output blanking feature in motor control is to prevent the
current regulation to trip upon short current spikes at the beginning of the PWM
period.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
- The CPU at 400MHz 
- The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
- The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz

In this example COMP1 and TIM1 are used:
COMP1 is configured as following:
 - Non Inverting input is connected to PB0
 - Inverting input is connected to VREFINT (1.22V)
 - Output is available on PA8
 - Blanking source is connected internally to TIM1 OC5

SystemCoreClock is set to 400 MHz for STM32H7xx Devices.
 - TIM1 is configured as following:

   In this example TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2),
   since APB2 prescaler is equal to 2.
      TIM1CLK = 2*PCLK2
      PCLK2 = HCLK/2
      => TIM1CLK = HCLK = SystemCoreClock/2
      
   TIM1 period is HCLK / period = 200000000 / (49999 +1) = 4 KHz.
   
 - TIM1 OC5 (internal channel: not available on GPIO) configured in PWM mode
   with a frequency equal to 4 KHz and high level equal to 
   pulse / period = 2000 / 200000000 = 10 micro second.
 - Timer channel intended to be connected to an external device (motor, ...) in user application:
   TIM1 CH1 (PA9) configured in PWM mode with a frequency equal to 4KHz
   and duty cycle pulse/period = 100 * (37500 / 50000) =  75%.

Connection needed:
 - either connect COMP1 input (PB0) to TIM1 CH1 (PA9).
 - either connect COMP1 input (PB0) to a signal generator, at the frequency of TIM1 period (to have comparator input signal frequency matching blanking signal frequency. To do this, possibility to use TIM1 CH1 (PA9) as trigger of signal generator).

Comparator output signal expected:
 - Between t=0us and t=10 us: Comparator positive input at level high, blanking source at level high.
   Blanking source is high, therefore comparator output is forced to level low.
   (otherwise, as comparator positive input is higher than negative input, comparator output would have been high.)
 - Between t=10 us and t=duty cycle pulse: Comparator positive input at level high, blanking source at level low.
   Comparator positive input is higher than negative input, blanking source is low, therefore comparator output is high.
 - Between t=duty cycle pulse and t=period: Comparator positive input at level low, blanking source at level low.
   Comparator positive input is higher than negative input, therefore comparator output is low.


@note Care must be taken when using HAL_Delay(), this function provides 
      accurate delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower) than the 
      peripheral interrupt. Otherwise the caller ISR process will be blocked. 
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set 
      to 1 millisecond to have correct HAL operation.

@par Directory contents 

  - COMP/COMP_OutputBlanking/Inc/stm32h7xx_hal_conf.h   HAL configuration file
  - COMP/COMP_OutputBlanking/Inc/stm32h7xx_it.h         COMP interrupt handlers header file
  - COMP/COMP_OutputBlanking/Inc/main.h                 Header for main.c module
  - COMP/COMP_OutputBlanking/Src/stm32h7xx_it.c         COMP interrupt handlers
  - COMP/COMP_OutputBlanking/Src/main.c                 Main program
  - COMP/COMP_OutputBlanking/Src/stm32h7xx_hal_msp.c    HAL MSP file
  - COMP/COMP_OutputBlanking/Src/system_stm32h7xx.c     STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.

  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.

  - STM32H743I-EVAL Set-up
    - Connect TIM1 CH1 (PA9) to a scope and to the input trigger of a wave generator
    - Connect the wave generator output to COMP1 Non Inverting input (PB0)
    - Configure the wave generator to output a pulse at rising edge event with 
      high level greater than VREFINT (1.22V) and pulse duration lower 10 micro second. 
      You can change the pulse delay and note that the break event is generated
      only when the pulse is triggered after 10 micro second.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
