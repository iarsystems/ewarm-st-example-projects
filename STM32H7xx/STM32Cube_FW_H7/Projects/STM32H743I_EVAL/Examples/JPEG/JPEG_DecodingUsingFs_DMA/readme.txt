/**
  @page JPEG_DecodingUsingFs_DMA : JPEG example : JPEG decoding using Fatfs and with DMA example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    JPEG/JPEG_DecodingUsingFs_DMA/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the JPEG decoding using Fatfs and with DMA example.
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

This example demonstrates how to read jpeg file from SDCard memory using Fatfs, decode it 
using the JPEG HW decoder in DMA mode and display the final ARGB8888 image on the LCD-TFT screen.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The example performs the following actions :
  - Link the uSD disk I/O driver.
  - Register the file system object (mount) to the FatFs module for the uSD drive.
  - Open the jpg file "image.jpg" in read access.
  - Call the function "JPEG_Decode_DMA" to start decoding the jpg file with DMA (Non blocking method).
    Function "JPEG_Decode_DMA" is implemented in a separate module decode_dma.c.
    This module is a wrapper on top of the JPEG HAL driver
  
  - Once the decoding ends, 
    - call HAL routine "HAL_JPEG_GetInfo" to retrieve the image parameters : 
      image width, image height, image quality (from 1% to 100%) color space and Chroma sampling.
      These parameters are used to initialize the DMA2D that will perform the copy of the decoded image 
      to the display frame buffer all with the YCbCr to RGB conversion (necessary for the display on the RGB LCD).
    - Close the jpg file.

   - The jpeg HW decoder outputs are organized in YCbCr blocks. The DMA2D is used to convert YCbCr Blocks to 
        ARGB8888 pixels into the the LCD Frame buffer for display.
        The DMA2D is configured in order to copy the result image in the center of the LCD screen.

   - The Red LED3 is On in case of error.		
        
@note To run this example , you can use an JPEG “.jpg” file from\Utilities\Media\Pictures\JPG
      or use your own .jpg image.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@note The compiler optimizations level could be set to High/Balanced giving a good tradeoff between the code size and the performance (decode time).

@note In case of EVAL's LCD use in example code, the DATA_IN_ExtSDRAM define
      need to be added  in the project preprocessor to avoid SDRAM multiple configuration
     (the LCD uses SDRAM as frame buffer, and its configuration is done by the BSP_SDRAM_Init()). 

@par Keywords

Display, Graphic, JPEG, Decoding, Flash memory, DMA, DMA2D, MDMA, ARGB8888, Hardware decoding, LCD, YCbCr, RGB,
FatFS, File system, SD Card, SDRAM

@par Directory contents

  - JPEG/JPEG_DecodingUsingFs_DMA/Inc/stm32h7xx_hal_conf.h          HAL configuration file
  - JPEG/JPEG_DecodingUsingFs_DMA/Inc/stm32h7xx_it.h                Interrupt handlers header file
  - JPEG/JPEG_DecodingUsingFs_DMA/Inc/ffconf.h                      FAT file system module configuration file
  - JPEG/JPEG_DecodingUsingFs_DMA/Inc/main.h                        Header for main.c module  
  - JPEG/JPEG_DecodingUsingFs_DMA/Inc/decode_dma.h                  Header for decode_dma.c module
  - JPEG/JPEG_DecodingUsingFs_DMA/Inc/sd_diskio.h                   Header for sd_diskio.c module

  - JPEG/JPEG_DecodingUsingFs_DMA/Src/stm32h7xx_it.c                Interrupt handlers
  - JPEG/JPEG_DecodingUsingFs_DMA/Src/main.c                        Main program
  - JPEG/JPEG_DecodingUsingFs_DMA/Src/decode_dma.c                  JPEG Decoding with DMA module
  - JPEG/JPEG_DecodingUsingFs_DMA/Src/sd_diskio.c                   SD Disk I/O driver module
  - JPEG/JPEG_DecodingUsingFs_DMA/Src/stm32h7xx_hal_msp.c           HAL MSP module 
  - JPEG/JPEG_DecodingUsingFs_DMA/Src/system_stm32h7xx.c            STM32H7xx system source file  
  

@par Hardware and Software environment

  - This example runs on STM32H743xx device.
    
  - This example has been tested with STMicroelectronics STM32H743I_EVAL revB 
    evaluation boards and can be easily tailored to any other supported device 
    and development board. 
    
  - STM32H743I_EVAL Set-up
  - Copy a jpg image to a uSD card (under the root)
      The image must have the following properties:
        - named as "image.jpg"
        - dimensions : less or equal to the display screen:
               - Witdh  : <= 640
               - Height : <= 480
  - Connect the uSD Card to the Micro SD connector (CN13).
    
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */