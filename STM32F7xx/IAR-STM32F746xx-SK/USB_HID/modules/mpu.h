/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2015
 *
 *    File name   : mpu.h
 *    Description : Cortex-M MPU initialization header file
 *
 *    History :
 *    1. Date        : May, 2015
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: #1 $
 **************************************************************************/
#ifndef __MPU_H
#define __MPU_H

#define RAM_ADDRESS_START                        (0x20000000UL)
#define RAM_SIZE                                 MPU_REGION_SIZE_512KB
#define PERIPH1_ADDRESS_START                    (0x40000000UL)
#define PERIPH1_SIZE                             MPU_REGION_SIZE_512MB
#define PERIPH2_ADDRESS_START                    (0xA0000000UL)
#define PERIPH2_SIZE                             MPU_REGION_SIZE_256MB
#define FLASH_ADDRESS_START                      (0x08000000UL)
#define FLASH_SIZE                               MPU_REGION_SIZE_1MB
#define SDR_ADDRESS_START                        (0xC0000000UL)
#define SDR_SIZE                                 MPU_REGION_SIZE_2GB
#define QSPI_ADDRESS_START                       (0x90000000UL)
#define QSPI_SIZE                                MPU_REGION_SIZE_256MB
#define RAM_REGION_NUMBER                        MPU_REGION_NUMBER0
#define FLASH_REGION_NUMBER                      MPU_REGION_NUMBER1
#define PERIPH1_REGION_NUMBER                    MPU_REGION_NUMBER2
#define PERIPH2_REGION_NUMBER                    MPU_REGION_NUMBER3
#define SDR_REGION_NUMBER                        MPU_REGION_NUMBER4
#define QSPI_REGION_NUMBER                       MPU_REGION_NUMBER5

void MPU_RegionConfig(void);

#endif // __MPU_H
