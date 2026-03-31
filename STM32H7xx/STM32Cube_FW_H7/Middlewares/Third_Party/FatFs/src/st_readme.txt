
  @verbatim
  ******************************************************************************
  *  
  *           Portions COPYRIGHT 2017 STMicroelectronics                       
  *           Portions Copyright (C) 2017, ChaN, all right reserved            
  *
  * @file    st_readme.txt 
  * @author  MCD Application Team
  * @brief   This file lists the main modification done by STMicroelectronics on
  *          FatFs for integration with STM32Cube solution.
  *          For more details on FatFs implementation on STM32Cube, please refer
  *          to UM1721 "Developing Applications on STM32Cube with FatFs"  
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  @endverbatim

### V2.0.0/07-March-2017 ###
============================
  + Upgrade to use FatFS R0.12c. The R0.12c breaks the API compatibility with R0.11b.
  - f_mkfs() API has a new signature.
  - The _CODE_PAGE got new values.
  - For more details check the files (doc/updates.txt) and the following urls:
       http://elm-chan.org/fsw/ff/en/mkfs.html
       http://elm-chan.org/fsw/ff/en/config.html
  
  + Add USB, RAMDISK and uSD template drivers under src/drivers.
    - The diskio drivers aren't part of fatfs anymore, they are just templates instead.
    - User has to copy the suitable template .c/.h file under the project, rename them by
      removing the "_template" suffix then link them into the final application.
    - The diskio driver .c/.h files have to be edited according to the used platform.
 
  + Define the macros "ff_malloc" and "ff_free" in the ff_conf_template.h and use
    them in the syscall.c instead of direct calls to stdlib malloc and free functions.
  + Define the "__weak" attribute in diskio.c for the GNU GCC compiler


### V1.4.0/09-September-2016 ###
================================
  + Upgrade to use FatFs R0.12b.
  + ff_conf.h: remove the use of define "_USE_BUFF_WO_ALIGNMENT".
     

### V1.3.0/08-May-2015 ###
==========================
  + Upgrade to use FatFs R0.11.
  + Add new APIs FATFS_LinkDriverEx() and FATFS_UnLinkDriverEx() to manage USB Key Disk having 
     multi-lun capability. These APIs are equivalent to FATFS_LinkDriver() and FATFS_UnLinkDriver()
     with "lun" parameter set to 0.
  + ff_conf.h: add new define "_USE_BUFF_WO_ALIGNMENT".
     This option is available only for usbh diskio interface and allow to disable
     the management of the unaligned buffer.
     When STM32 USB OTG HS or FS IP is used with internal DMA enabled, this define
     must be set to 0 to align data into 32bits through an internal scratch buffer
     before being processed by the DMA . Otherwise (DMA not used), this define must
     be set to 1 to avoid Data alignment and improve the performance.
     Please note that if _USE_BUFF_WO_ALIGNMENT is set to 1 and an unaligned 32bits
     buffer is forwarded to the FatFs Write/Read functions, an error will be returned. 
     (0: default value or 1: unaligned buffer return an error).


  + Important note:
      For application code based on previous FatFs version; when moving to R0.11
      the changes that need to be done is to update ffconf.h file, taking 
      ffconf_template.h file as reference.


### V1.2.1/20-November-2014 ###
===============================
  + Disk I/O drivers; change count argument type from BYTE to UINT

  + Important note:
      For application code based on previous FatFs version; when moving to R0.10b
      the only change that need to be done is to update ffconf.h file, taking 
      ffconf_template.h file as reference.


### V1.2.0/04-November-2014 ###
===============================
  + Upgrade to use FatFs R0.10b.
  + diskio.c: update disk_read() and disk_write() argument's type.

  + Important note:
      For application code based on previous FatFs version; when moving to R0.10b
      the only change that need to be done is to update ffconf.h file, taking 
      ffconf_template.h file as reference.


### V1.1.1/12-September-2014 ###
================================
  + ff_gen_drv.c: Update the Disk_drvTypeDef disk variable initialization to avoid
    warnings detected with Atollic TrueSTUDIO Complier.


### V1.1.0/22-April-2014 ###
============================
  + Update sd_diskio to use SD BSP in polling mode instead of DMA mode (the scratch
    buffer needed for DMA alignment is removed as well).
  + diskio.c and ff_gen_drv.c/.h: update to prevent multiple initialization.


### V1.0.0/18-February-2014 ###
===============================
   + First R0.10 customized version for STM32Cube solution.


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
