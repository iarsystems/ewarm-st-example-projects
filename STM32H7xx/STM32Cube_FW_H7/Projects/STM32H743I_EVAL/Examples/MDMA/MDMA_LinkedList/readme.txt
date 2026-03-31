/**
  @page MDMA_LinkedList MDMA linked list transfer Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    MDMA/ MDMA_LinkedList /readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the MDMA linked list transfer Example.
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
This example shows how to use the MDMA to perform a list of transfers. The transfer list is organized 
as a linked list , each time the current transfer ends the MDMA automatically relaod the next transfer parameters
and starts it (without CPU intervention). 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The MDMA is configured using the function "MDMA_Config"
This function configure the MDMA for a linked list transfer, the linked list contains 3 nodes.
Node 0 transfer parameters are configured within the MDMA Handle init parameters.
Next nodes are filled using HAL function "HAL_MDMA_LinkedList_CreateNode"
Nodes are connected to the linked list using the HAL function "HAL_MDMA_LinkedList_AddNode"
After setting all nodes parameters (node 0 using HAL_MDMA_Init and next nodes using HAL_MDMA_LinkedList_CreateNode/HAL_MDMA_LinkedList_AddNode) 
start the transfer in interrupt mode using function "HAL_MDMA_Start_IT". 
Note that the source and destination addresses given to the function HAL_MDMA_Start_IT 
must correspond to the node 0 source and destination addresses, same for the transfer data length.
Note that using the MDMA the transfer data length is always expressed in bytes whatever
is the source and data size (byte, half word, word or double word)

Wait for end of the MDMA transmission or error interrupts ; the MDMA transmission ends when all the transfer nodes are served.

STM32H743I Eval board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when transfer  is complete (all transfer nodes are served).

 - LED3 is ON when there is an error in transmission process.  

@note using the MDMA in linked list transfer the following requirement must be filled :
- Node 0 parameters are always given within the MDMA handle init parameter and set during the function "HAL_MDMA_Init"
- Next nodes must be defines as MDMA_LinkNodeTypeDef types and filled using function "HAL_MDMA_LinkedList_CreateNode".
- Nodes addresses must be 64 bits aligned and must be in the AXI address space.
  this constraints must be respected when defining the MDMA_LinkNodeTypeDef nodes variables.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - MDMA/MDMA_LinkedList/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - MDMA/MDMA_LinkedList/Inc/stm32h7xx_it.h          MDMA interrupt handlers header file
  - MDMA/MDMA_LinkedList/Inc/main.h                  Header for main.c module  
  - MDMA/MDMA_LinkedList/Src/stm32h7xx_it.c          MDMA interrupt handlers
  - MDMA/MDMA_LinkedList/Src/main.c                  Main program
  - MDMA/MDMA_LinkedList/Src/system_stm32h7xx.c      STM32H7xx system source file


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
