/**
  @page CRYP_AESModes  AES ECB/CBC & CTR  Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    CRYP/CRYP_AES_CCM/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   Description of the CRYP AES encryption/decryption mode
  *          with ECB, CBC and CTR algorithm.
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


This example provides a short description of how to use the CRYP peripheral to
encrypt/decrypt data(Plaintext/Ciphertext) using AES ECB, CBC and CTR algorithm.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H753xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.


The AES mode requires:
1. Plaintext/Ciphertext: which will be encrypted with several algorithm & keys.    
   CiphertextAESECB128,CiphertextAESCBC192 and CiphertextAESCTR256 : which will 
   be decrypted respectively with ECB algorithm & 128bits key, CBC algorithm &
   192bits key and CTR algorithm & 256 key. 

3. InitVector or the Initialization Vector used only for CBC and CTR.

4. Key: is the parameter which determines the Ciphertext. In this example several
   key size are used,  128 bits for ECB, 192 bits for CBC and 256 bits for CTR.

CRYP peripheral must be initialized once from the beginning, then for each 
operation of encryption/decryption, only configuration should be made if needed.
 
The AES-ECB encryption/decryption in polling mode provide :
1. Encryptedtext: which is the encryption result of Plaintext, it is compared 
   to CiphertextAESECB128.
2. Decryptedtext: which is the Decryption result of Ciphertext,it is compared
   to Plaintext.

The AES-CBC encryption/decryption in DMA mode provide :
1. Encryptedtext: which is the encryption result of Plaintext, it is compared
   to CiphertextAESCBC192.
2. Decryptedtext: which is the Decryption result of Ciphertext,it is compared 
   to Plaintext.

The AES-CTR encryption/decryption in interrupt mode provide:
1. Encryptedtext: which is the encryption result of Plaintext, it is compared
   to CiphertextAESCTR256.
2. Decryptedtext: which is the Decryption result of Ciphertext,it is compared
   to Plaintext.
   
STM32H743I-EVAL LEDs are used to monitor the encryption/decryption  status:
 - LED1(GREEN) is ON when encryption/decryption  are right.
 - LED3(RED) is ON when encryption or decryption are wrong.
 
@note No CPU Data Cache maintenance is performe in this example, you can adress to
      CRYP_AESModes_DMA example to see how to manage CPU Data Cache.
            
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Security, CRYP, AES, ECB, CBC,CTR, NIST FIPS publication 197, hardware CRYP, 

@par Directory contents 
  
  - CRYP/CRYP_AESModes/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_AESModes/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_AESModes/Inc/main.h                  Header for main.c module
  - CRYP/CRYP_AESModes/Src/stm32h7xx_it.c          Interrupt handlers
  - CRYP/CRYP_AESModes/Src/main.c                  Main program
  - CRYP/CRYP_AESModes/Src/stm32h7xx_hal_msp.c     HAL MSP module 
  - CRYP/CRYP_AESModes/Src/system_stm32h7xx.c      STM32H7xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32H753xx devices.
  
  - This example has been tested with an STMicroelectronics STM32H753I-EVAL
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 