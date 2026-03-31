/**
  @page DMA2D_MemToMemWithBlendingAndCLUT DMA2D Memory to Memory with blending and CLUT example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017  
  * @brief   Description of the STM32H7xx DMA2D Memory to Memory with blending and CLUT example.
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
  Memory_to_Memory with blending transfer mode and indexed 256 color images (L8).
  The examples shows also how to use the DMA2D foreground/background CLUT with L8 color mode.   

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
  The CPU at 400MHz 
  The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
  The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

  Two L8 images are used :
  - L8_Image1 : L8 256 color Indexed 320x240 image
  - L8_Image0 : L8 256 color Indexed 320x240 image

  In first stage the DMA2D is used to configure the layer 1 in memory to memory with Pixel Format Conversion.
  - Configure the output color mode as ARGB8888 pixel format.
  - Configure the Foreground (layer 1)
  - Foreground image is loaded from FLASH memory (L8_Image1)
  - CLUT (256 color index table ) loaded from FLASH memory (L8_CLUT_1)
  - Input color mode as L8 pixel format
  - Load the L8_CLUT_1 CLUT (color index table for image L8_Image1) to DMA2D Foreground CLUT and wait for CLUT loading ends.
  - Start the DMA2D transfer where source address is L8_Image1 and destination address is the LTDC display frame buffer.
  - Once the DMA2D transfer ends, the image L8_Image1 is displayed on the LCD. 
  - Wait for 2 sec before next stage.

  In second stage the DMA2D is used to configure the layer 1 in memory to memory with Pixel Format Conversion.
  - Configure the output color mode as ARGB8888 pixel format.
  - Configure the Foreground (layer 1)
  - Foreground image is loaded from FLASH memory (L8_Image0)
  - CLUT (256 color index table ) loaded from FLASH memory (L8_CLUT_0)
  - Input color mode as L8 pixel format
  - Load the L8_CLUT_0 CLUT (color index table for image L8_Image0) to DMA2D Foreground CLUT and wait for CLUT loading ends.
  - Start the DMA2D transfer where source address is L8_Image0 and destination address is the LTDC display frame buffer.
  - Once the DMA2D transfer ends, the image L8_Image0 is displayed on the LCD.
  - Wait for 2 sec before next stage
  
  In third stage the DMA2D is used to configure the layer 1 and layer 0 in memory to memory with blending.
  - Configure the output color mode as ARGB8888 pixel format.
  - Configure the Foreground (layer 1)
  - Foreground image is loaded from FLASH memory (L8_Image1)
  - CLUT for layer 1 (256 color index table ) loaded from FLASH memory (L8_CLUT_1)
  - Input color mode as L8 pixel format.
  - Transparency factor for layer 1 is set to 0x7F (semi-transparent)
  - Load the L8_CLUT_1 CLUT (color index table for image L8_Image1) to DMA2D Foreground CLUT and wait for CLUT loading ends.
  - Configure the Background (layer 0)
  - Background image is loaded from FLASH memory (L8_Image0)
  - CLUT for layer 0 (256 color index table ) loaded from FLASH memory (L8_CLUT_0)
  - Input color mode as L8 pixel format
  - Load the L8_CLUT_0 CLUT (color index table for image L8_Image0) to DMA2D Background CLUT and wait for CLUT loading ends.
  - Start the DMA2D transfer with blending where :
      - source address 1 : for foreground is L8_Image1
      - source address 2 : for background is L8_Image0      
      - destination address is the LTDC display frame buffer.
  - Once the DMA2D transfer ends,  the result blend image from L8_Image0 and L8_Image1 is displayed on the LCD.
  - wait for 2 sec then loop to stage 1.
  
  
  The DMA2D blends the two sources pixels from FLASH to internal FIFOs to compute 
  the resulting pixel in dedicated output buffer (using foregound and background CLUTs to convert L8 source images to ARGB8888 blend image). 
  The transferred data to the output memory address is the result of the blending 
  operation between background and foreground.
  
  Files L8_Image1.h and L8_Image0.h provides the definition of the L8 images where:
  - L8_CLUT_x is the 256 colors CLUT for L8_Imagex : color palette (Color index tables).
  - L8_Imagex is the 256 colors image itself. each pixel is coded on one byte.

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


 @note  The header files of the images used in this example can be generated with 
        STemWin bitmap converter released with this package.
        \Middlewares\ST\STemWin\Software\BmpCvtST.exe      

@par Keywords

Display, Graphic, DMA2D, LCD, SDRAM, ARGB8888, Blending, Memory to memory, Foreground, L8, CLUT
Background, LTDC, Pixel, Indexed colors


@par Directory contents

    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Inc/main.h                Main configuration file
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Inc/stm32h7xx_it.h        Interrupt handlers header file
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Inc/stm32h7xx_hal_conf.h  HAL configuration file 
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Inc/L8_Image1.h           L8_Image1 definition
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Inc/L8_Image0.h           L8_Image0 definition
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Src/main.c                Main program  
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Src/stm32h7xx_it.c        Interrupt handlers
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Src/stm32h7xx_hal_msp.c   HAL MSP module
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Src/system_stm32h7xx.c    STM32H7xx system clock configuration file

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
                                   