/**
  @page DMAMUX_RequestGen DMA & DMAMUX request generator Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DMA/DMAMUX_RequestGen/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the DMA & request generator Example.
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
This example shows how to use the DMA with the DMAMUX request generator to generate DMA transfer requests upon 
LPTIM2 output signal, knowing that the LPTIM2 is configured in PWM with 2sec period.

The example uses the BDMA channel 0 configured in memory to peripheral mode with the DMAMUX request generator 0.


At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The BDMA channel 0 is configured in memory to peripheral mode to ensure data transfer from the source transmission 
buffer (SRC_Buffer_LED1_Toggle) to the GPIOB ODR register (in order to toggle LED1).
The DMA is configured in circular mode so the transfer will restart automatically each time the amount of data 
to be transmitted has been reached.

@note the Domain 3 BDMA has access to  Domain 3 SRAMs and peripherals only,  thus the source buffer (SRC_Buffer_LED1_Toggle)
has been placed to the D3 SRAM (@0x38000000)

The DMAMUX request generator block is configured using function "HAL_DMAEx_ConfigMuxRequestGenerator" 
with the following parameters :
- SignalID      : set to HAL_DMAMUX2_REQUEST_GEN_LPTIM2_OUT which corresponds to LPTIM2 output signal.
- Polarity      : Set to RISING to use rising edge the LPTIM2 output signal for DMA requests generation.
- RequestNumber : 1 i.e. on each rising edge of the LPTIM2 output signal, a DMA request is generated.

The DMA request generator is then enabled using function “HAL_DMAEx_EnableMuxRequestGenerator”.

The function "LPTIM_Config" is then used to configure the LPTIM2 to generate a PWM with a period of 2sec. 
Note that the example doesn’t need to configure a GPIO to LPTIM2 output alternate function as the LPTIM2 output signal 
is used internally to trigger the DMAMUX request generator in order to generate DMA transfer requests every 2sec.

Then the DMA transfer is started in non-blocking mode using the HAL function "HAL_DMA_Start_IT"
On each LPTIM2 output event (i.e every 2sec) the DMAMUX request generator generates a DMA request.
As consequence the DMA will serve the request and write a new value to the GPIOB ODR register to toggle the LED1 
without any CPU intervention.
As consequence LED1 will toggle every 2sec.

The CPU is only used to intercept a DMA transfer interrupt error and DMAMUX overflow interrupt if any and sets the LED3 (Red LED) to On in this case.  


STM32 board's LEDs can be used to monitor the transfer status:
 - LED1 toggles every 2 seconds.
 - LED3 is ON when there is an error during the DMA transfer.  



@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - DMA/DMAMUX_RequestGen/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - DMA/DMAMUX_RequestGen/Inc/stm32h7xx_it.h          DMA interrupt handlers header file
  - DMA/DMAMUX_RequestGen/Inc/main.h                  Header for main.c module  
  - DMA/DMAMUX_RequestGen/Src/stm32h7xx_it.c          DMA interrupt handlers
  - DMA/DMAMUX_RequestGen/Src/main.c                  Main program
  - DMA/DMAMUX_RequestGen/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx device.
    
  - This example has been tested with STM32H743ZI-NUCLEO board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
