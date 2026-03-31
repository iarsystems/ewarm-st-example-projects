/**
  @page OPAMP_PGA_ExternalBias Amplify the input signal with bias voltage for non inverting mode.  
  @verbatim
  ********************* COPYRIGHT(c) 2017 STMicroelectronics *******************
  * @file    OPAMP/OPAMP_PGA_ExternalBias/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the OPAMP PGA with Bias voltage example.
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

This example shows how to configure OPAMP peripheral in PGA mode with bias voltage for 
the non inverting mode. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
- The CPU at 400MHz 
- The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
- The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz
 
OPAMP1 is configured as following:
   - PGA mode.
   - OPAMP Non Inverting input is connected to DAC_OUT1 that injects a 0.5V internally.
   - OPAMP Inverting input connected to an external voltage called bias voltage.
   - OPAMP Output is connected to an oscilloscope to observe the variation of output voltage.

Test Description:

The purpose of the test is to amplify the amplitude of an input signal with bias voltage for 
non inverting mode.
User will push User button to switch between the non inverting gain 2 and 4.
Meanwhile, the user will try to modify the voltage of the bias and observe the variation of
the opamp output.
  
   - Push the User Push button to set the programmable gain amplifier to 2. 
   - Push the User alternatively to switch from gain equals to 2 to 4 and vice versa.
   - OPAMP1 Non Inverting input(VP) is connected internally to DAC_OUT1 that injects a 0.5V.
   - Inject a bias voltage to OPAMP1 Inverting input VN (PC5 pin 6 in CN12 connector)
   - check OPAMP1 output Vout(PC4 pin 34 in CN12 connector) with an oscilloscope, it will be equal to: 
   
                   Vout = Gain x VP +(1 - Gain) x  VN 
	Step 1: 	
	   * When Gain equals to 2, LED1 is ON , Connect VN(PC5 pin 6 of CN12) to GND:
	     If Vout(PC4 pin 34 in CN12 )  equals to 1V , Test PASS.
	   * When Gain equals to 4, LED1 is OFF,LED2 is ON , Keep the connection of VN(PC5 pin 6 of CN12) to GND:
	     If Vout(PC4 pin 34 in CN12 ) equals to 2V , Test PASS.
		 
	Step 2: 	
	   * When Gain equals to 2, LED1 is ON , Inject 1v to VN(PC5 pin 6 of CN12):
	     If Vout(PC4 pin 34 in CN12 ) equals to 0V , Test PASS.
	   * When Gain equals to 4, LED1 is OFF,LED2 is ON , Inject 0.5v to VN(PC5 pin 6 of CN12):
	     If Vout(PC4 pin 34 in CN12 ) equals to 0.5V , Test PASS.	 
        	    

@par Directory contents 

  - OPAMP/OPAMP_PGA_ExternalBias/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - OPAMP/OPAMP_PGA_ExternalBias/Inc/stm32h7xx_it.h          Header for stm32h7xx_it.c
  - OPAMP/OPAMP_PGA_ExternalBias/Inc/main.h                  Header for main.c  
  - OPAMP/OPAMP_PGA_ExternalBias/Src/stm32h7xx_it.c          Interrupt handlers
  - OPAMP/OPAMP_PGA_ExternalBias/Src/main.c                  Main program
  - OPAMP/OPAMP_PGA_ExternalBias/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - OPAMP/OPAMP_PGA_ExternalBias/Src/system_stm32h7xx.c      STM32H7xx system clock configuration file

@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with STM32H743ZI-Nucleo board and can be
    easily tailored to any other supported device and development board..

      
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

