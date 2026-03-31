/**
  @page IOExpander_Example IOExpander_Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2009 STMicroelectronics *******************
  * @file    IOExpander/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    06/19/2009
  * @brief   Descrition of the IOExpander example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Example Description 

This firmware provides an example of configuring and using the IO Expander STMPE811 
in order to control input Joystick IOs, output IOs and the Touch Screen feature.
This example provides the elementary steps to control the following modules and
use them in an application:
The IO pins are used for:
  - Getting Joystick pins status (Polling and interrupt).
  - Getting MEMS Interrupt pins status (Polling and interrupt).
  - Controlling the Audio Codec, The VBAT_DIV and the MII_INT pins (set or reset)
The Touch Screen controller is used to get the Touched area detection information
in a single point mode (Polling and interrupt).
The input information can be used either in Polling mode (uncomment the define
#define IOE_POLLING_MODE) or in Interrupt mode (uncomment the define #define
IOE_INTERRUPT_MODE). These defines are in main.h file.

After configuring the IO Expander modules and Interrupts (if needed), the output
IOs are set to their default state. These operations are done by the functions: 
IOE_Config() and IOE_ITConfig(). If the IO Expander is not operational (or the 
I2C pins are not in the correct level) the IOE_Config() function returns a value
different from 0 and a message is displayed on the LCD screen.

To get the status of an input:
 - IOE_JoyStickGetState() function is used for Joystick and returns a JOY_State_TypeDef
    value indicating the currently pressed key. This information is used to update 
    the LCD display (the Joystick pressed key name is displayed).
 - IOE_TS_GetState() function is used for Touch Screen and returns a pointer to 
    a structure holding the Touch Screen status (did a Touch Detection happened, 
    X, Y, and Z positions). Only detection into the drawn rectangles areas is 
    considered and used to update the LEDs status and LCD display (corresponding  
    LED turned ON when its rectangle is touched, and the LED name is displayed on
    the LCD screen).

Besides, the examples checks the status of the other Push-Buttons implemented on
the board (Key, Tamper and WakeUp push-buttons). They are checked in parallel 
with the IO Expander IOs and the LCD display and LEDs are updated according to 
the push buttons state. 

 
@par Directory contents 

  - IOExpander/stm32f10x_conf.h     Library Configuration file
  - IOExpander/stm32f10x_it.c       Interrupt handlers
  - IOExpander/stm32f10x_it.h       Header for stm32f10x_it.c
  - IOExpander/main.c               Main program
  - IOExpander/main.h               Header for main.c


@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity-Line Devices.
  
  - This example has been tested with STMicroelectronics STM3210C-EVAL (STM32F10x 
    Connectivity line) evaluation board and can be easily tailored to any other 
    supported device and development board (after adding necessary hardware 
    and connections).

  - STM3210C-EVAL Set-up 
    - Use two IO Expanders connected to I2C1 SCL and SDA pins PB6 and PB7 and 
      interrupt output pin connected to PB14 (EXTI mode). IO Expander 1 address 
      pins is connected to Gnd (address is 0x82) and IO Expander 2 address pins 
      is connected to Vcc (address is 0x88).  
    - Use LED1, LED2, LED3 and LED4 connected respectively to PD.07, PD.13, PF.03
      and PD.04 pins
    - Use LCD connected to SPI3 remapped pins SCK, MISO and MOSI: PC10, PC11, 
      PC12 and the CS pin PB2.
    - Use LCD Touch Screen module connected to IO Expander 1 pins IO_Pin_1, 
      IO_Pin_2, IO_Pin_3 and IO_Pin_4 (respectively Y-, X-, Y+, X+ lines).
    - Use Joystick module connected to IO Expander 2 pins IO_Pin_3, IO_Pin_4, 
      IO_Pin_5, IO_Pin_6 and IO_Pin_7 (respectively UP, Right, Left, Down and Center)
    - Key, Tamper and WakeUp push buttons connected respecively to PB9, PC13 and 
      PA0 pins.

          
@par How to use it ? 

 + RVMDK:
   - Open the GPIO.uv2 project
   - Rebuild all files: Project->Rebuild all target files
   - Load project image: Debug->Start/Stop Debug Session
   - Run program: Debug->Run (F5)
 
 + EWARMv5:
   - Open the GPIO.eww workspace.
   - Rebuild all files: Project->Rebuild all
   - Load project image: Project->Debug
   - Run program: Debug->Go(F5)

@note
 - Low-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 32 and 128 Kbytes.
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.
   
 * <h3><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h3>
 */
