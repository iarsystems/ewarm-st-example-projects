/**
  @page BSP  Example on how to use the BSP drivers
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    BSP/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the BSP example.
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

This example provides a description of how to use the different BSP drivers. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

This example shows how to use the different functionalities of components 
available on the board by switching between all tests using Tamper push-button. 

 ** Push the Tamper push-button to start first Test.  
Green Led (LED1) will blink between each test. Press Tamper push-button to start another test:

 ** AUDIO PLAY **
This example shows how to play an audio file using the DMA in circular mode and 
how to handle the buffer update.
Plug a headphone to the headset connector (CN17)  to hear the sound.  
@Note: Copy file 'audio_sample_tdm.bin' (available in Binary) directly in 
       the flash at @0x08080000 using ST-Link utility.

 ** AUDIO RECORD **
This example shows how to record an audio stream from microphones mounted on STM32H743I-EVAL
and play it using the DMA circular mode and how to handle the buffer update.
Plug a headphone to the headset connector (CN17)  to hear the sound. 

 ** JOYSTICK **
Use the joystick button to move a pointer inside a rectangle 
(up/down/right/left) and change the pointer color(select).

 ** Touchscreen **
Use touchscreen functionality to select or activate colored circle inside a rectangle.
A calibration is needed when using MB1046.

 ** LCD **
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

** SD **
This example shows how to erase, write and read the SD card and also 
how to detect the presence of the card. 

 ** EEPROM **
This example show how to read and write data in EEPROM I2C M24LR64 connected on STM32H743I-EVAL
   * The I2C RF EEPROM memory (M24LR64) is available through the connector CN4

 ** NOR **
This example shows how to erase, write and read the NOR memory.

 ** SRAM **
This example shows how to erase, write and read the SRAM memory.

 ** SRAM DMA **
This example shows how to erase, write and read the SRAM memory in DMA mode.

 ** SDRAM **
This example shows how to erase, write and read the SDRAM memory.

 ** SDRAM DMA **
This example shows how to erase, write and read the SDRAM memory in DMA mode.

 ** LCD LOG **
This example shows how to use the LCD log features. 

 ** QSPI test **
This example provides of how to write, read and buffers compare for external 
flash memory using QSPI communication.

** Potentiometer **
This example shows how to get the potentiometer’s voltage level. 
      
@par Directory contents 

  - BSP/Src/main.c                 Main program
  - BSP/Src/system_stm32h7xx.c     STM32H7xx system clock configuration file
  - BSP/Src/stm32h7xx_it.c         Interrupt handlers 
  - BSP/Src/joystick.c             joystick feature
  - BSP/Src/lcd.c                  LCD drawing features
  - BSP/Src/log.c                  LCD Log firmware functions
  - BSP/Src/eeprom.c               EEPROM Read/Write features
  - BSP/Src/audio_play.c           Play audio 
  - BSP/Src/audio_record.c         Record and Play audio
  - BSP/Src/sram.c                 SRAM Read/Write features
  - BSP/Src/sdram.c                SDRAM Read/Write features
  - BSP/Src/sdram_dma.c            SDRAM Read/Write DMA mode
  - BSP/Src/nor.c                  NOR Read/Write features 
  - BSP/Src/qspi.c                 QUADSPI Read/Write features
  - BSP/Src/potentiometer.c        potentiometer features   
  - BSP/Src/sd.c                   SD Erase/Read/Write features
  - BSP/Src/touchscreen.c          touchscreen features
  - BSP/Src/ts_calibration.c       calibration of the touchscreen  
  - BSP/Inc/main.h                 Main program header file  
  - BSP/Inc/stm32h7xx_hal_conf.h   HAL Configuration file
  - BSP/Inc/stm32h7xx_it.h         Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h         lcd_log configuration template file
  - BSP/Inc/stlogo.h               Image used for BSP example
        
@par Hardware and Software environment  

  - This example runs on STM32H743xx device.
    
  - This example has been tested with STMicroelectronics STM32H743I-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    
  - STM32H743I-EVAL Set-up :
   if using MB1063 (AMPIRE 640x480 LCD ) :
   - Make sure that JP10 is set to PSU position.
   - Make sure that the board is powered using an external 5v power supply through CN10.
   For NOR Module:
    - Make sure that JP13 is fitted.
   For SD module:
    - Use a UHS-I usD card (Ultra High Speed usD Card)
  
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
