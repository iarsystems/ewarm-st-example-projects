/**
  @page DMA2D_MemoryToMemory DMA2D Memory to Memory example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DMA2D/DMA2D_MemoryToMemory/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the STM32H7xx DMA2D Memory to Memory example.
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

This example provides a description of how to configure the DMA2D peripheral in 
Memory_to_Memory transfer mode.
The example transfers with DMA2D an internal SRAM static texture array aBufferInput[] of size 16x32 
and format ARGB4444 (16 bpp) to destination buffer in internal SRAM in array aBufferResult[].
The number of bytes transferred is then 16x32x2=1024 bytes.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the SYSTICK.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

In this basic example the goal is to explain the different fields of the DMA2D 
structure in the case of Memory_to_Memory transfer mode.

After DMA2D configuration, the data transfer is performed and then the transferred 
data are checked byte per byte in comparing the source and destination buffers aBufferInput[] and aBufferResult[].   

STM32H743I-EVAL board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the DMA2D transfer is complete and when the data are correctly transferred.
 - LED3 is ON when there is a DMA2D transfer error or when data are not correctly transferred
          or when there is an error in DMA2D transfer/Init process.
 If everything all right, at the end of the test, LED1  must be ON, LED3 must be OFF.

@note On the STM32H743xx the ITCM and DTCM RAM are only accessible by the Cortex M7 and the MDMA.
      The ITCM and DTCM RAM are not accessible by the DMA2D thus the source and destination buffer
      should not be located in these RAMs. 
      In this example the RAM area is configured (at the IDE linker file) to the D1 AXI-SRAM  (@0x24000000) 
      which is accessible by the DMA2D. The source buffer of the DMA2D is then located in the internal FLASH 
      and the destination buffer is located in the RAM area which is configured to the D1 AXI-SRAM. 
	  
@note CPU Data Cache maintenance :
      It is recommended to clean the CPU Data cache before starting the  DMA2D transfer. 
      As the source buffer may be prepared by the CPU, this guarantees that the source buffer 
      (if located in the D1 AXI-SRAM which is cacheable) will be up to date before starting the transfer.

      It is recommended to invalidate the CPU Data cache after the DMA2D transfer. 
      As the destination buffer may be used by the CPU, this guarantees Up-to-date data when CPU access 
      to the destination buffer located in the D1 AXI-SRAM (which is cacheable).	 
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, Graphic, DMA2D, LCD, SRAM, ARGB4444, Memory to Memory

@par Directory contents 

  - DMA2D/DMA2D_MemoryToMemory/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - DMA2D/DMA2D_MemoryToMemory/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - DMA2D/DMA2D_MemoryToMemory/Inc/main.h                        Header for main.c module  
  - DMA2D/DMA2D_MemoryToMemory/Src/stm32h7xx_it.c          Interrupt handlers
  - DMA2D/DMA2D_MemoryToMemory/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - DMA2D/DMA2D_MemoryToMemory/Src/main.c                        Main program
  - DMA2D/DMA2D_MemoryToMemory/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.      

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
