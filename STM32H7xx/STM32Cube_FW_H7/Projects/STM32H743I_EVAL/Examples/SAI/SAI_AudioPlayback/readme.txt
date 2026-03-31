/**
  @page SAI_AudioPlayback  Description of the SAI audio playback example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    SAI/SAI_AudioPlayback/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the SAI audio playback example.
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

This example shows how to use the SAI to playback audio data coming from two Microphones.
In this example:
- The SAI4 PDM interface receives audio data coming from two Microphones using the
  BDMA in circular mode.
- The audio PDM to PCM library is used to convert audio PDM data to PCM format.
- The SAI1 transmits audio PCM data to the WM8994 Codec for playback using the DMA2 in 
  circular mode.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
 - The CPU at 400MHz 
 - The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
   and D3 Domain AHB4  peripherals at 200MHz.
 - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals 
   and D3 Domain APB4 peripherals to  run at 100MHz
   
Plug a headphone to hear the sound  /!\ Take care of yours ears. Default volume is 60%.
 
Once started, LED1 is on during playback. 
If an error occured LED3 is turned ON.

@note Note the BDMA and the DMA run in circular buffer mode and never stop. If you break  
      with the debugger, they will keep running and a noise will be heard.

@note The Domain 3 BDMA has access to  Domain 3 SRAMs and peripherals only,  thus the
      audioPdmBuf buffer has been placed in the D3SRAM (@0x38000000)

@note The PDM to PCM library is used to convert PDM data to PCM format.

@note This example does not use the BSP_AUDIO driver so the MspInit is coded in the 
      stm32h7xx_hal_msp.c.

@par Directory contents  

  - SAI/SAI_AudioPlayback/Src/main.c                  Main program
  - SAI/SAI_AudioPlayback/Src/system_stm32h7xx.c      STM32H7xx system source file
  - SAI/SAI_AudioPlayback/Src/stm32h7xx_hal_msp.c     STM32H7xx MSP source file
  - SAI/SAI_AudioPlayback/Src/stm32h7xx_it.c          Interrupt handlers
  - SAI/SAI_AudioPlayback/Inc/main.h                  Main program header file
  - SAI/SAI_AudioPlayback/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - SAI/SAI_AudioPlayback/Inc/stm32h7xx_it.h          Interrupt handlers header file
	
@par Hardware and Software environment 

  - This example has been tested with STMicroelectronics STM32H743I-EVAL
    board and can be easily tailored to any other supported device
    and development board.      

  - STM32H743I-EVAL Set-up :
    - Put the SW2 switch to PDM position
    - Plug a headphone on CN17 connector (Headset). 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
