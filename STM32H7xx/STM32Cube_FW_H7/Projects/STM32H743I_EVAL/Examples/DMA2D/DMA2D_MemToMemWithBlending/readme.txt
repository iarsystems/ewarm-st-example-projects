/**
  @page DMA2D_MemToMemWithBlending DMA2D Memory to Memory with blending example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DMA2D/DMA2D_MemToMemWithBlending/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the STM32H7xx DMA2D Memory to Memory with blending example.
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

  This example provides a description of how to configure DMA2D peripheral in 
  Memory_to_Memory with blending transfer mode.

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
  The CPU at 400MHz 
  The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
  The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.
 
  The LCD is initialized using the BSP functions. the LCD layer 0 is configured for display
  with color format set to ARGB8888.
  
  Two images in RGB565 color format are used respectively for foreground and background.
   - Image 1 : 
		- input memory address at Flash memory (static image array coded in FLASH).
		- Format : RGB565
		- Size   : 240x130
   - Image 2 : 
		- input memory address at Flash memory (static image array coded in FLASH).
		- Format : RGB565
		- Size   : 240x130

    Then in an infinite while loop the following sequence is implemented :
	
      - Step 1- The DMA2D is used in Pixel Format Conversion mode (PFC) to transfer
          the "Image 1" from the input buffer located in the Flash to the
          LCD frame buffer. The DMA2D destination address and output offset are set 
          in such a way to transfer the input image to the center of the LCD.
          The DMA2D is configured in Pixel Format Conversion mode (PFC) with 
          input color format set to RGB565 (color format of the input image)
          and output color format set to ARGB8888 (color format of LCD display).
		  
      -Step 2- The DMA2D is used in Pixel Format Conversion mode (PFC) to transfer
          the "Image 2" from the input buffer located in the Flash to the
          LCD frame buffer. The DMA2D destination address and output offset are set 
          in such a way to copy the input image to the center of the LCD.
          The DMA2D is configured in Pixel Format Conversion mode (PFC) with 
          input color format set to RGB565 (color format of the input image)
          and output color format set to ARGB8888 (color format of LCD display).
		  
      -Step 3- The DMA2D is used in memory to memory with Blending mode (M2M_BLEND),
	      to blend the foreground and background images. the DMA2D is configured as follow :
		- The DMA2D foreground layer is configured with input pixel format set to RGB565
		  the source address set to the "Image 1" input buffer located in the Flash. 
                          the input offset is set to zero and the alpha transparency set to 0x7F (semi-transparent).
                        - The DMA2D background layer is configured with input pixel format set to ARGB8888,
                          The source address set to the center of LCD frame buffer and the input offset
                          is set in such a way to fetch the background from the center of LCD frame buffer.
                          Knowing that the LCD frame buffer already contains the "Image 2" in ARGB8888 format 
                          transferred in previous step.
                          The alpha transparency set to 0xFF (fully opaque) for the background layer. 
                        - The DMA2D output color format is set to ARGB8888 (color format of LCD display).
                          The output offset and destination address are configured in in such a way to place the 
                          result of the blending in the center of the LCD frame buffer :
                          The output offset is configured same as the background layer input offset.
                          The destination address is configured same as the background layer source address. 	

        For each step a message indicates which image is currently displayed 
          - Display Foreground Image : life.augmented
          - Display Background Image : ST Logo
          - Display Blended Image    : ST Logo + life.augmented
        
        a delay of 2sec is inserted after each step.
 
  Next step in the pixel pipeline is the use of the LTDC with only 1 layer which 
  is the output of the DMA2D operation.
  This in order to show on LCD panel the result of the blending of the 
  foreground and background.      
  
  In this example, the background object represents the ST logo and the foreground
  object represents the "life.augmented" slogan.

STM32H743I-EVAL board's LEDs can be used to monitor the transfer status:

 - LED1 is ON when the DMA2D transfer is complete.
 - LED2 is ON when there is a DMA2D transfer error.
 - LED3 is ON when there is an error in LTDC transfer/Init process.
         
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, Graphic, DMA2D, LCD, SRAM, ARGB8888, Blending, Memory to memory, Foreground,
Background, RGB565, LTDC, Pixel

@par Directory contents

    - DMA2D/DMA2D_MemToMemWithBlending/Inc/main.h                      Main configuration file
    - DMA2D/DMA2D_MemToMemWithBlending/Inc/stm32h7xx_it.h              Interrupt handlers header file
    - DMA2D/DMA2D_MemToMemWithBlending/Inc/stm32h7xx_hal_conf.h        HAL configuration file 
    - DMA2D/DMA2D_MemToMemWithBlending/Inc/RGB565_240x130_1.h          Image used for DMAD2D validation
    - DMA2D/DMA2D_MemToMemWithBlending/Inc/RGB565_240x130_2.h          Image used for DMAD2D validation
    - DMA2D/DMA2D_MemToMemWithBlending/Src/main.c                      Main program  
    - DMA2D/DMA2D_MemToMemWithBlending/Src/stm32h7xx_it.c              Interrupt handlers
    - DMA2D/DMA2D_MemToMemWithBlending/Src/stm32h7xx_hal_msp.c         HAL MSP module
    - DMA2D/DMA2D_MemToMemWithBlending/Src/system_stm32h7xx.c          STM32H7xx system clock configuration file

@par Hardware and Software environment  

  - This example runs on STM32H743xx devices.
  
  - This example has been tested with STM32H743I-EVAL boards and can be easily 
    tailored to any other supported device and development board. 

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
                          
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
                                   