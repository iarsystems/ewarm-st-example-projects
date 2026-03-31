/**
  @page MDMA_DMA2D_Triggering MDMA with DMA2D Transfer complete flag used as MDMA trigger Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    MDMA/MDMA_DMA2D_Triggering/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the MDMA with DMA2D Transfer complete flag 
  *          used as MDMA trigger Example.
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

This example describes how to use the MDMA with HW trigger set to the DMA2D transfer complete flag.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The MDMA channel 0 is then configured in HW request mode with HW transfer request trigger
set to DMA2D Transfer Complete flag. 
The MDMA is initialized to copy the DMA2D destination image to the LCD frame buffer at the bottom-center 
position with 180° rotation and mirror effect.
Each block corresponds to a line of the source/destination image.
The number of blocks corresponds to the number of lines of the image.

The MDMA Request is set to DMA2D Transfer complete flag and the MDMA trigger Mode is set 
to Repeat block mode. As consequence each DMA2D transfer complete event 
(DMA2D has end copying an image to the Top-center of the LCD frame buffer ), the MDMA will start 
a repeat block transfer which correspond of copying the DMA2D destination image with 180° 
and rotation effect to the bottom-center of the LCD frame buffer.
    
The DMA2D is initialized to copy an image from the internal flash to the Top-Center 
of the LCD frame buffer.
In an infinite loop, each time a DMA2D transfer is started to copy a new image. 
As the MDMA Request is set to DMA2D Transfer complete flag each time the DMA2D ends
transferring an image to the Top-center of the LCD frame buffer, the MDMA will use 
this DMA2D destination image to transfer it to the bottom-center of the LCD frame buffer 
with 180° rotation an mirror effect.
A delay of 2sec is inserted after each DMA2D transfer

AS consequence user can check that every 2sec a new image is transferred to the 
Top-Center of the LCD (Using the DMA2D) and the same image with 180° rotation and
mirror effect is transferred to the bottom-center of the LCD (Using the MDMA) 

Note that in order to make the MDMA transfer infinite, a linked list node that have 
the same parameters as the initial transfer is created.
The MDMA linked list is made circular by connecting this node to itself using HAL function 
HAL_MDMA_LinkedList_EnableCircularMode.
The purpose of the linked list node is to create a circular list. the MDMA 
transfer will then loop infinitely. Each DMA2D Transfer complete flag will trig
a Node transfer (a repeat block transfer) that correspond to a new image transfer.

@note the MDMA linked list Nodes addresses must be 64 bits aligned and must be in the AXI address space.
this constraints must be respected when defining the MDMA_LinkNodeTypeDef nodes variables.
As the MDMA Node descriptor "Xfer_Node" is located in the D1 AXI-SRAM which 
is cacheable, it is necessary to clean the data cache after creating and configuring the node 
in order to make sure that the MDMA will load up-to-date data from the linked list node.

STM32H743I Eval board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the MDMA transfer process is complete.
 - LED3 is ON when there is an error in MDMA or DMA2D transfer process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - MDMA/MDMA_DMA2D_Triggering/Inc/stm32h7xx_hal_conf.h        HAL configuration file
  - MDMA/MDMA_DMA2D_Triggering/Inc/stm32h7xx_it.h              DMA  and MDMA interrupt handlers header file
  - MDMA/MDMA_DMA2D_Triggering/Inc/main.h                      Header for main.c module
  - MDMA/MDMA_DMA2D_Triggering/Inc/life_augmented_argb8888.h   Image 320x240 in ARGB8888 to display on LCD
  - MDMA/MDMA_DMA2D_Triggering/Inc/image_320x240_argb8888.h    Image 320x240 in ARGB8888 to display on LCD    
  - MDMA/MDMA_DMA2D_Triggering/Src/stm32h7xx_it.c              MDMA interrupt handlers
  - MDMA/MDMA_DMA2D_Triggering/Src/main.c                      Main program
  - MDMA/MDMA_DMA2D_Triggering/Src/stm32h7xx_hal_msp.c         HAL MSP module
  - MDMA/MDMA_DMA2D_Triggering/Src/system_stm32h7xx.c          STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx device.
    
  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 