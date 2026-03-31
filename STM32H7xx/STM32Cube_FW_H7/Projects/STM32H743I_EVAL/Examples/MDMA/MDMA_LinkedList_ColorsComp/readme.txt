/**
  @page MDMA linked list for colors decomposing example description
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    MDMA/MDMA_LinkedList_ColorsComp/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017  
  * @brief   Description of the MDMA Linked List Colors Comp example.
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

  This example demonstrates how to use the MDMA in linked list mode to extract Red/Green and blue
  colors of an ARGB8888 image, resize each sub image (with a decimation factor /2) and 
  display the result Red/green/blue decimated sub-images on the LCD.

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
  The CPU at 400MHz 
  The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
  The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.
 
  After LCD initialization, the LCD layer 0 is configured for display. 
  
  The MDMA is configured in linked list mode as follow : 
  
  Node0 which corresponds to the MDMA initialization parameters is used to transfer the original image
  to the top center of the display screen.
  
  other linked list nodes are created using function "HAL_MDMA_LinkedList_CreateNode" then connected to each other using
  function "HAL_MDMA_LinkedList_AddNode"
  
  Node0_1,Node0_2 and Node0_3 are used to reset the 3 sub-images display area with default ARGB8888 color
  0xFF000000 which corresponds to black.
  
  Node1, Node2 and Node3 are respectively used to extract Red, Green and blue components from 
  the original image and to copy the result   to the display areas previously reset 
  by respectively Node0_1,Node0_2 and Node0_3.
  
  Note that the nodes are configured also to subsample the original image by 2 horizontally and vertically.
  The nodes are configured as follow :
  - Source and destination data size set to byte : to copy one color component (Red, green or blue) only from the source image
  - Source Data increment is set to double word (64 bits) : in order to jump one 32 bits word over 2.
  - Destination data increment is set to Word (32bits)
    knowing that an ARGB8888 pixel is defined on one word (32bits), this allows to decimate horizontally the original image by 2.
  - The source block address offset is set Image_width * bytes_per_pixels = Image_width * 4. This allows
    to jump one line over 2 from the source image allowing to decimate vertically the original image by 2.
  
  
  Note : The parameter "pPrevNode" of function "HAL_MDMA_LinkedList_AddNode"
  is set to zero in order to connect the given node to the end of the linked list.
      
  The transfer is started using function "HAL_MDMA_Start_IT". The parameters source and destination addresses
  correspond to the Node0 source and destination. for all other nodes, the source and destination are passed
  during the node creation (function HAL_MDMA_LinkedList_CreateNode).   
  
  When the MDMA transfer ends the LCD will display the original image on (Top) then 
  3 images that correspond to Red/Green and Blue components decimated(resized) by 2 vertically and horizontally.
  

 The STM32H743I Eval board's LED can be used to monitor the transfer status:
  - LED1 is ON when the transfer ends without error.
  - LED3 is ON when if a transfer error occurs.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents

    - MDMA/MDMA_LinkedList_ColorsComp/Inc/stm32h7xx_hal_conf.h  HAL configuration file
    - MDMA/MDMA_LinkedList_ColorsComp/Inc/stm32h7xx_it.h        Interrupt handlers header file
    - MDMA/MDMA_LinkedList_ColorsComp/Inc/main.h                Main configuration file
    - MDMA/MDMA_LinkedList_ColorsComp/image_320x240_argb8888.h  Image of size 320x240 and format ARGB8888 to be displayed on LCD
    - MDMA/MDMA_LinkedList_ColorsComp/Src/stm32h7xx_it.c        Interrupt handlers
    - MDMA/MDMA_LinkedList_ColorsComp/Src/main.c                Main program 	
    - MDMA/MDMA_LinkedList_ColorsComp/Src/system_stm32h7xx.c    STM32H7xx system source file


@par Hardware and Software environment  

  - This example runs on STM32H743xx device.
  
  - This example has been tested with STM32H743I-EVAL  board revB and can be
    easily tailored to any other supported device and development board.    

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
                           
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
                                   
