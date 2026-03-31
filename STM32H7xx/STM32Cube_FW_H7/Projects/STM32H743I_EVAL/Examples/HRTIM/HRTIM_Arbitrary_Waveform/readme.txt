/**
  @page HRTIM/HRTIM_Arbitary_Waveform HRTIM Arbitary Waveform example
  
  @verbatim
  ********************* COPYRIGHT(c) 2017 STMicroelectronics *******************
  * @file    HRTIM/HRTIM_Arbitary_Waveform/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the HRTIM1 Arbitary waveform generation
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

This example shows how to configure the HRTIM1 peripheral to generate an arbitary signals.
In fact the reset source and set sources of the TIMD TD1 and TD2 can be a combination of several 
sources, in this example we select more than sources for set and reset in order 
to show the capability of our HRTIM to generate different waveforms.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals 
and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals 
and D3 Domain APB4 peripherals to  run at 100MHz

HRTIM1 TIMD is used in this example, the set source for TD1 output is selected to be:
the TIMD period, the compare unit 1 and the compare unit 2, the reset source are also
the same sources, so the result waveform will be a logical AND between the three sources.
The set source for TD2 output is selected to be: the TIMD period and the compare unit 2, 
the reset source are: the compare unit 1 and the compare unit 3
so the result waveform will be a combination between these sources.

SystemCoreClock is set to 400 MHz for STM32H743xx Devices.

    In this example HRTIM1 input clock (HRTIM1CLK) is set to APB2 clock (PCLK2),
    since APB2 prescaler is equal to 2.
      HRTIM1CLK = 2*PCLK2
      PCLK2 = HCLK/2

      => HRTIM1CLK = HCLK = SystemCoreClock/2

The waveforms can be displayed using an oscilloscope.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - HRTIM/HRTIM_Arbitary_Waveform/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - HRTIM/HRTIM_Arbitary_Waveform/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - HRTIM/HRTIM_Arbitary_Waveform/Inc/main.h                  Header for main.c module  
  - HRTIM/HRTIM_Arbitary_Waveform/Src/stm32h7xx_it.c          Interrupt handlers
  - HRTIM/HRTIM_Arbitary_Waveform/Src/main.c                  Main program
  - HRTIM/HRTIM_Arbitary_Waveform/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - HRTIM/HRTIM_Arbitary_Waveform/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with STMicroelectronics STM32H743I-EVAL 
    board and can be easily tailored to any other supported device 
    and development board.      

  - STM32H743I-EVAL Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
        - HRTIM1_TIMD_TD1: PA.11 (pin 54 in CN7)
        - HRTIM1_TIMD_TD1: PA.12 (pin 56 in CN7)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
