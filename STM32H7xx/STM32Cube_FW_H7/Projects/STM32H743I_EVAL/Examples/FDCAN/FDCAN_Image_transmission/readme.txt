/**
  @page FDCAN_Image_transmission FDCAN Image transmission example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FDCAN/FDCAN_Image_transmission/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the FDCAN_Image_transmission ewample.
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

This example shows the gain in time obtained by the activation of the Bit Rate
Switching (BRS) feature.
The same image is transmitted two times via FDCAN as follow:
 - first, without Bit Rate Switching, at 1 MBits/s bit rate
 - then, with Bit Rate Switching activated, at 4 MBits/s data bit rate

Boards: STM32H743I-EVAL (embeding an STM32H743xx device)

   _________________________                       _________________________ 
  |           ______________|                     |______________           |
  |          |FDCAN1        |                     |        FDCAN1|          |
  |          |       CAN    |_____________________|    CAN       |          |
  |          |    connector |                     | connector    |          |
  |          |______________|                     |______________|          |
  |  _____________________  |                     |                         |
  | |                     | |                     |                         |
  | |                     | |                     |                         |
  | |         LCD         | |                     |                         |
  | |                     | |                     |                         |
  | |_____________________| |                     |                         |
  |   _                _    |                     |   _                     |
  |  |_|              (_)   |                     |  |_|                    |
  | LED3            Tamper  |                     | LED3                    |
  |                         |                     |                         |
  |      STM32 Board 1      |                     |      STM32 Board 2      |
  |        Receiver         |                     |       Transmitter       |
  |_________________________|                     |_________________________|

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices:
 - The CPU at 400MHz 
 - The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
 - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 100MHz.

Then:
 - Reception filters and Synchronization message are configured.
 - Tx frames are configured to be sent without Bit Rate Switching.
 - FDCAN modules are started.
 - On Board 1, main program waits for Tamper push-button to be pressed to send
   Transmission request to Board 2.
 - On Board 2, main program waits for Reception of Transmission request message
   to begin sending image data by blocks of 32 messages (maximum length of the
   Tx FIFO).

First step (No BRS):
  When Tamper push-button is pressed in Board 1, a Transmission request is sent to
  Board 2 to inform that it is ready to receive data.
  Then, Board 2 sends first 32 blocks (messages) of image data and waits for
  acknowledgment of receipt to send next 32 blocks.
  On the Board 1 side, every time 32 messages are received and payload retrived,
  the acknowledgment is automatically sent to Board 2.
  This process ends when the full image is transferred. It is then displayed on the
  LDC via DMA2D as well as the whole measured communication time.

Second step (BRS activated):
  Tx frames are reconfigured to be sent with Bit Rate Switching activated.
  The same process as step 1 is repeated.

If an error is encountered LED3 is turned ON.

@note The measured communication time includes:
       - Adding messages to Tx FIFO
       - Trasmission (propagation time)
       - Retrieving messages from Rx FIFO
       - Sending acknowledgment of receipt every 32 messages

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@par Directory contents 

  - FDCAN/FDCAN_Image_transmission/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - FDCAN/FDCAN_Image_transmission/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - FDCAN/FDCAN_Image_transmission/Inc/main.h                  Header for main.c module  
  - FDCAN/FDCAN_Image_transmission/Inc/Image.h                 Image data
  - FDCAN/FDCAN_Image_transmission/Src/stm32h7xx_it.c          Interrupt handlers
  - FDCAN/FDCAN_Image_transmission/Src/main.c                  Main program
  - FDCAN/FDCAN_Image_transmission/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - FDCAN/FDCAN_Image_transmission/Src/system_stm32h7xx.c      stm32h7xx system source file

@par Hardware and Software environment 

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with a couple of STM32H743I-EVAL boards embedding a 
    STM32H743xx device and can be easily tailored to any other supported device 
    and development board.
    
  - STM32H743I-EVAL set-up
    - Connect a CAN cable to the CAN connectors of the two EVAL boards (CN3)
    - Jumpers JP1 and JP2 => fitted
    - Solder bridges SB59 and SB60 => open
    - Solder bridge SB50 => closed

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Uncomment the "#define FDCAN_receiver" into main.c (default configuration)
 - Rebuild all files and load your image into Board 1 target memory
 - Comment the "#define FDCAN_receiver" into main.c
 - Rebuild all files and load your image into Board 2 target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
