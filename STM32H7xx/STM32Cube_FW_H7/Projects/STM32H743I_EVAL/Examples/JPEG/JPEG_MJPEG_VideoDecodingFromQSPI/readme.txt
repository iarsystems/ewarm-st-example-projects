/**
  @page MJPEG_VideoDecoding : MJPEG video decoding from QSPI Flash example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    JPEG/MJPEG_VideoDecodingFromQSPI/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017  
  * @brief   Description of the MJPEG video decoding from QSPI Flash example.
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

This example demonstrates how to use the HW JPEG decoder to decode an MJPEG video file
located in the external QSPI flash and display it on the LCD-TFT screen.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The example performs the following actions :
  - Inittialize the QSPI interface ans configure it to memory mapped mode.
    
  - Function "AVI_ParserInit" is called to initialize the AVI parser and read the avi file header.
    then In the video decoding loop and for each iteration 
      - function "AVI_GetFrame" is called to retrieve the address and size of one frame from the AVI file. 
      - If the frame corresponds to a video frame then start the decoding using the function "JPEG_Decode_DMA" (Non blocking method).
        Function "JPEG_Decode_DMA" is implemented in a separate module decode_dma.c. 
        This module is a wrapper on top of the JPEG HAL driver.
     
  - Once the decoding ends, 
    - if the decoded frame is the first one, call HAL routine "HAL_JPEG_GetInfo" 
      to retrieve the image parameters : image width, image height, image quality (from 1% to 100%) color space and Chroma sampling.
      These parameters are used to initialize the DMA2D that will perform the copy of the decoded frame 
      to the display frame buffer all with the YCbCr to RGB conversion (necessary for the display on the RGB LCD).
        
  - The jpeg HW decoder outputs are organized in YCbCr blocks. The DMA2D is used to convert YCbCr Blocks to 
        ARGB8888 pixels into the LCD Frame buffer for display.
        The DMA2D is configured in order to copy the result image in the center of the LCD screen.
     
  - When the end of file is reached, the file decoding characteristics are displayed:
       Resolution, JPEG quality, Chroma sampling, average decoding time and number of decoded frames.
       
  - Video rate is by default set to native framerate of original video (to disable regulation , you can comment "USE_FRAMERATE_REGULATION" flag in main.h)

  - The Red LED3 is On in case of error.
   
@note To Run this example , you can use an MJPEG “.avi” file from 
      \Utilities\Media\Video or use your own MJPEG file.
     This file must have the following properties:
        - renamed as "video.avi"
        - resolution : less or equal to the LCD-TFT screen (i.e 640x480 for the STM32H743I_EVAL board)
        - Width  : <= 640
        - Height : <= 480
      The MJPEG file can be in “AVI” container format. 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@note The compiler optimizations level could be set to High/Balanced giving a good trade-off between the code size and the performance (decode time).


@par Keywords

Display, Graphic, MJPEG, Video, Decoding, LCD, JPEG Hardware Decoder,
DMA, FatFS, File system, avi, YCbCr, RGB

@Note  If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),
       it is recommended to configure the latters as Write Through.
       This is ensured by configuring the memory attributes at MPU level in order to ensure cache coherence on SRAM1 and SRAM2.
       Please, refer to Template project for a typical MPU configuration.

@Note  If external memory is shared between several processors, it is recommended to configure it as Write Back (bufferable), shareable and cacheable.
       The memory base address and size must be properly updated.
       The user needs to manage the cache coherence at application level.

For more details about the MPU configuration and use, please refer to AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”

@par Directory contents

  - JPEG/MJPEG_VideoDecodingFromQSPI/Inc/stm32h7xx_hal_conf.h          HAL configuration file
  - JPEG/MJPEG_VideoDecodingFromQSPI/Inc/stm32h7xx_it.h                Interrupt handlers header file
  - JPEG/MJPEG_VideoDecodingFromQSPI/Inc/ffconf.h                      FAT file system module configuration file
  - JPEG/MJPEG_VideoDecodingFromQSPI/Inc/main.h                        Header for main.c module  
  - JPEG/MJPEG_VideoDecodingFromQSPI/Inc/decode_dma.h                  Header for decode_dma.c module   
  - JPEG/MJPEG_VideoDecodingFromQSPI/Inc/AVI_parser.h                  Header for AVI_parser.c module   
  
  - JPEG/MJPEG_VideoDecodingFromQSPI/Src/stm32h7xx_it.c                Interrupt handlers
  - JPEG/MJPEG_VideoDecodingFromQSPI/Src/main.c                        Main program
  - JPEG/MJPEG_VideoDecodingFromQSPI/Src/decode_dma.c                  JPEG Decoding with MDMA module
  - JPEG/MJPEG_VideoDecodingFromQSPI/Src/AVI_parser.c                  AVI parser module 
  - JPEG/MJPEG_VideoDecodingFromQSPI/Src/stm32h7xx_hal_msp.c           HAL MSP module 
  - JPEG/MJPEG_VideoDecodingFromQSPI/Src/system_stm32h7xx.c            STM32H7xx system source file  
  

@par Hardware and Software environment

  - This example runs on STM32H743xx device.
    
  - This example has been tested with STMicroelectronics STM32H743I_EVAL revB 
    evaluation boards and can be easily tailored to any other supported device 
    and development board. 
    
  - STM32H743I_EVAL Set-up
  - Flash one of the MJPEG video files to the QSPI external Flash using STM32 ST-LINK utility
      The file must have the following properties:
        - resolution : less or equal to the LCD-DSI screen (i.e 640x480 for the STM32H743I_EVAL board)
        - Width  : <= 640
        - Height : <= 480
      The MJPEG file can be in “AVI” container format.
  - Use STM32 ST-LINK utility version v4.0.0.1 or higher.
  - Click on "External Loader" from the bar menu then check "MT25TL01G_STM32H7xxI-EVAL" box
  - Rename the video file : change the extension to .bin : example video1.avi.bin
  - From ST-LINK utility go to "File->Open File" and Open the renamed video file.
  - From ST-LINK utility go to Target->Program and set the "Start address" to 0x90000000. Press "Start" button and wait to the end of the programming operation.
  - Once the video file programmed , close the ST-LINK utility and run the example.
  
    
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 