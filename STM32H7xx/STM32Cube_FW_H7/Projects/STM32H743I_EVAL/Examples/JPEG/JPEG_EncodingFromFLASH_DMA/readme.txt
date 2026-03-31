/**
  @page JPEG_EncodingFromFLASH_DMA : JPEG example : JPEG encoding with DMA of an image stored in FLASH example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    JPEG/JPEG_EncodingFromFLASH_DMA/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the JPEG encoding from FLASH and with DMA example.
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

This example demonstrates how to read an RGB image stored in the internal FLASH, encode it 
using the JPEG HW encoder in DMA mode and save it in SDCard.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The example performs the following actions :
  - Link the uSD disk I/O driver.
  - Register the file system object (mount) to the FatFs module for the uSD drive.
  - Create the JPEG file with write access.
  - Call the function "JPEG_Encode_DMA" to start encoding the jpg file with DMA (Non blocking method).
    the RGB input image name is automatically selected according to the selected RGB format as follow :
    - Image_RGB888 for RGB888 (#define JPEG_RGB_FORMAT      JPEG_RGB888)
    - Image_RGB565 for RGB565 (#define JPEG_RGB_FORMAT      JPEG_RGB565)
    - Image_ARGB8888 for ARGB8888 (#define JPEG_RGB_FORMAT      JPEG_ARGB8888)
    Function "JPEG_Encode_DMA" is implemented in a separate module encode_dma.c.
    This module is a wrapper on top of the JPEG HAL driver
  - Save encoded data in jpg file
  - Once the encoding ends, close the jpg file.
  - The Green LED1 is On if the test ends properly.
  - The Red LED3 is On in case of error.
  
@note The jpeg HW encoder inputs are organized in YCbCr blocks. 
      The jpeg utilities functions implemented in "jpeg_utils.c" file are used to convert the input RGB pixels
      to YCbCr blocks.
      The JPEG utilities files allowing to convert RGB pixels to YCbCr blocks are available under \Utilities\JPEG

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The compiler optimizations level could be set to High/Balanced giving a good tradeoff between the code size and the performance (decode time).


@par Keywords

Display, Graphic, JPEG, Encoding, Flash memory, DMA, ARGB8888, Hardware Encoding, LCD, YCbCr, RGB,
SD Card, SDRAM

@par Directory contents

  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/stm32h7xx_hal_conf.h          HAL configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/stm32h7xx_it.h                Interrupt handlers header file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/ffconf.h                      FAT file system module configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/main.h                        Header for main.c module  
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/encode_dma.h                  Header for encode_dma.c module
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/sd_diskio.h                   Header for sd_diskio.c module    
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/jpeg_utils_conf.h             JPEG utilties module configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/image_320_240_rgb.h           Specification of RGB image to load in FLASH

  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/stm32H7xx_it.c                Interrupt handlers
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/main.c                        Main program
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/encode_dma.c                  JPEG Encoding with DMA module
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/sd_diskio.c                   SD Disk I/O driver module      
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/stm32h7xx_hal_msp.c           HAL MSP module 
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/system_stm32h7xx.c            STM32H7xx system source file  
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/Image_argb_8888.c             Example of ARGB888 image to load in FLASH
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/Image_rgb_565.c               Example of RGB565 image to load in FLASH
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/Image_rgb_888.c               Example of RGB888 image to load in FLASH
  

@par Hardware and Software environment

  - This example runs on STM32H743xx device.
    
  - This example has been tested with STMicroelectronics STM32H743I_EVAL revB 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    
  - STM32H743I_EVAL Set-up
  - Connect the uSD Card to the Micro SD connector (CN13).
    
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */