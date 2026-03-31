/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : iar_stm32f429ii_sk.h
 *    Description : IAR-STM32F429II-SK board support package definitions
 *
 *    History :
 *    1. Date        : 01 July 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __IAR_STM32F429II_SK
#define __IAR_STM32F429II_SK

#include "stm32f4xx.h"

#define LEDn                             4

#define LED1_PIN                         GPIO_Pin_4
#define LED1_GPIO_PORT                   GPIOA
#define LED1_GPIO_CLK                    RCC_AHB1Periph_GPIOA

#define LED2_PIN                         GPIO_Pin_3
#define LED2_GPIO_PORT                   GPIOG
#define LED2_GPIO_CLK                    RCC_AHB1Periph_GPIOG

#define LED3_PIN                         GPIO_Pin_2
#define LED3_GPIO_PORT                   GPIOE
#define LED3_GPIO_CLK                    RCC_AHB1Periph_GPIOE

#define LED4_PIN                         GPIO_Pin_3
#define LED4_GPIO_PORT                   GPIOE
#define LED4_GPIO_CLK                    RCC_AHB1Periph_GPIOE

/**
  * @BUZZER
  */
#define BUZZER_GPIO_PIN                 GPIO_Pin_5
#define BUZZER_GPIO_PORT                GPIOA
#define BUZZER_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define BUZZER_PIN_SOURCE               GPIO_PinSource5

/**
  * @TRIMER
  */
#define TRIMER_PIN                      GPIO_Pin_6
#define TRIMER_PORT                     GPIOA
#define TRIMER_CLK                      RCC_AHB1Periph_GPIOA
#define TRIMER_SOURCE                   GPIO_PinSource6
#define TRIMER_CHANNEL                  ADC_Channel_6


/**
  * @TEMP_SENSOR
  */
#define TEMP_SENSOR_INT_PIN             GPIO_Pin_13
#define TEMP_SENSOR_INT_PORT            GPIOD
#define TEMP_SENSOR_INT_CLK             RCC_AHB1Periph_GPIOD


/**
  * @LCD
  */
#define LCD_BACKLIGHT_PIN               GPIO_Pin_7
#define LCD_BACKLIGHT_PORT              GPIOC
#define LCD_BACKLIGHT_CLK               RCC_AHB1Periph_GPIOC
#define LCD_BACKLIGHT_SOURCE            GPIO_PinSource7



/** @addtogroup IAR-STM32F429II-SK LOW_LEVEL_BUTTONS
  * @{
  */
#define BUTTONn                          6 /**/

/**
 * @brief Key push-button
 */
#define USER_BUTTON_PIN                   GPIO_Pin_0
#define USER_BUTTON_GPIO_PORT             GPIOA
#define USER_BUTTON_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define USER_BUTTON_EXTI_LINE             EXTI_Line0
#define USER_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOA
#define USER_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource0
#define USER_BUTTON_EXTI_IRQn             EXTI0_IRQn
/**
 * @brief Joystick Right
 */
#define RIGHT_BUTTON_PIN                  GPIO_Pin_8
#define RIGHT_BUTTON_GPIO_PORT            GPIOI
#define RIGHT_BUTTON_GPIO_CLK             RCC_AHB1Periph_GPIOI
#define RIGHT_BUTTON_EXTI_LINE            EXTI_Line8
#define RIGHT_BUTTON_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOI
#define RIGHT_BUTTON_EXTI_PIN_SOURCE      EXTI_PinSource8
#define RIGHT_BUTTON_EXTI_IRQn            EXTI9_5_IRQn
/**
 * @brief Joystick Left
 */
#define LEFT_BUTTON_PIN                   GPIO_Pin_9
#define LEFT_BUTTON_GPIO_PORT             GPIOI
#define LEFT_BUTTON_GPIO_CLK              RCC_AHB1Periph_GPIOI
#define LEFT_BUTTON_EXTI_LINE             EXTI_Line9
#define LEFT_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOI
#define LEFT_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource9
#define LEFT_BUTTON_EXTI_IRQn             EXTI9_5_IRQn
/**
 * @brief Joystick Up
 */
#define UP_BUTTON_PIN                     GPIO_Pin_11
#define UP_BUTTON_GPIO_PORT               GPIOI
#define UP_BUTTON_GPIO_CLK                RCC_AHB1Periph_GPIOI
#define UP_BUTTON_EXTI_LINE               EXTI_Line11
#define UP_BUTTON_EXTI_PORT_SOURCE        EXTI_PortSourceGPIOI
#define UP_BUTTON_EXTI_PIN_SOURCE         EXTI_PinSource11
#define UP_BUTTON_EXTI_IRQn               EXTI15_10_IRQn
/**
 * @brief Joystick Down
 */
#define DOWN_BUTTON_PIN                   GPIO_Pin_10
#define DOWN_BUTTON_GPIO_PORT             GPIOI
#define DOWN_BUTTON_GPIO_CLK              RCC_AHB1Periph_GPIOI
#define DOWN_BUTTON_EXTI_LINE             EXTI_Line10
#define DOWN_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOI
#define DOWN_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource10
#define DOWN_BUTTON_EXTI_IRQn             EXTI15_10_IRQn
/**
 * @brief Joystick Sel
 */
#define SEL_BUTTON_PIN                    GPIO_Pin_5
#define SEL_BUTTON_GPIO_PORT              GPIOB
#define SEL_BUTTON_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define SEL_BUTTON_EXTI_LINE              EXTI_Line5
#define SEL_BUTTON_EXTI_PORT_SOURCE       EXTI_PortSourceGPIOB
#define SEL_BUTTON_EXTI_PIN_SOURCE        EXTI_PinSource5
#define SEL_BUTTON_EXTI_IRQn              EXTI9_5_IRQn

/**
  * @SD
  */
#define SD_CP_PIN                       GPIO_Pin_9
#define SD_CP_PORT                      GPIOG
#define SD_CP_CLK                       RCC_AHB1Periph_GPIOG
#define SD_CP_SOURCE                    GPIO_PinSource9

#define SD_WP_PIN                       GPIO_Pin_3
#define SD_WP_PORT                      GPIOG
#define SD_WP_CLK                       RCC_AHB1Periph_GPIOG
#define SD_WP_SOURCE                    GPIO_PinSource3

#define SD_CMD_PIN                      GPIO_Pin_2
#define SD_CMD_PORT                     GPIOD
#define SD_CMD_CLK                      RCC_AHB1Periph_GPIOD
#define SD_CMD_SOURCE                   GPIO_PinSource2

#define SD_D0_PIN                       GPIO_Pin_8
#define SD_D0_PORT                      GPIOC
#define SD_D0_CLK                       RCC_AHB1Periph_GPIOC
#define SD_D0_SOURCE                    GPIO_PinSource8

#define SD_D1_PIN                       GPIO_Pin_9
#define SD_D1_PORT                      GPIOC
#define SD_D1_CLK                       RCC_AHB1Periph_GPIOC
#define SD_D1_SOURCE                    GPIO_PinSource9

#define SD_D2_PIN                       GPIO_Pin_10
#define SD_D2_PORT                      GPIOC
#define SD_D2_CLK                       RCC_AHB1Periph_GPIOC
#define SD_D2_SOURCE                    GPIO_PinSource10

#define SD_D3_PIN                       GPIO_Pin_11
#define SD_D3_PORT                      GPIOC
#define SD_D3_CLK                       RCC_AHB1Periph_GPIOC
#define SD_D3_SOURCE                    GPIO_PinSource11

#define SD_CLK_PIN                      GPIO_Pin_12
#define SD_CLK_PORT                     GPIOC
#define SD_CLK_CLK                      RCC_AHB1Periph_GPIOC
#define SD_CLK_SOURCE                   GPIO_PinSource12

/**
  * @brief  SD FLASH SDIO Interface
  */
//#define SD_SDIO_DMA_STREAM3           3
#define SD_SDIO_DMA_STREAM6           6

#ifdef SD_SDIO_DMA_STREAM3

 #define SD_SDIO_DMA_STREAM            DMA2_Stream3
 #define SD_SDIO_DMA_CHANNEL           DMA_Channel_4
 #define SD_SDIO_DMA_FLAG_FEIF         DMA_FLAG_FEIF3
 #define SD_SDIO_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF3
 #define SD_SDIO_DMA_FLAG_TEIF         DMA_FLAG_TEIF3
 #define SD_SDIO_DMA_FLAG_HTIF         DMA_FLAG_HTIF3
 #define SD_SDIO_DMA_FLAG_TCIF         DMA_FLAG_TCIF3

#elif defined SD_SDIO_DMA_STREAM6

 #define SD_SDIO_DMA_STREAM            DMA2_Stream6
 #define SD_SDIO_DMA_CHANNEL           DMA_Channel_4
 #define SD_SDIO_DMA_FLAG_FEIF         DMA_FLAG_FEIF6
 #define SD_SDIO_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF6
 #define SD_SDIO_DMA_FLAG_TEIF         DMA_FLAG_TEIF6
 #define SD_SDIO_DMA_FLAG_HTIF         DMA_FLAG_HTIF6
 #define SD_SDIO_DMA_FLAG_TCIF         DMA_FLAG_TCIF6

#endif /* SD_SDIO_DMA_STREAM3 */

//Definition for COM port, connected to USART2
#define COMn                1

#define COM1_BASE           USART2
#define COM1_CLK            RCC_APB1Periph_USART2
#define COM1_TX_PIN         GPIO_Pin_5
#define COM1_TX_GPIO_PORT   GPIOD
#define COM1_TX_GPIO_CLK    RCC_AHB1Periph_GPIOD
#define COM1_TX_SOURCE      GPIO_PinSource5
#define COM1_TX_AF          GPIO_AF_USART2

#define COM1_RX_PIN         GPIO_Pin_6
#define COM1_RX_GPIO_PORT   GPIOD
#define COM1_RX_GPIO_CLK    RCC_AHB1Periph_GPIOD
#define COM1_RX_SOURCE      GPIO_PinSource6
#define COM1_RX_AF          GPIO_AF_USART2

#define COM1_RTS_PIN        GPIO_Pin_4
#define COM1_RTS_GPIO_PORT  GPIOD
#define COM1_RTS_GPIO_CLK   RCC_AHB1Periph_GPIOD
#define COM1_RTS_SOURCE     GPIO_PinSource4
#define COM1_RTS_AF         GPIO_AF_USART2

#define COM1_CTS_PIN        GPIO_Pin_0
#define COM1_CTS_GPIO_PORT  GPIOA
#define COM1_CTS_GPIO_CLK   RCC_AHB1Periph_GPIOA
#define COM1_CTS_SOURCE     GPIO_PinSource0
#define COM1_CTS_AF         GPIO_AF_USART2

#define COM1_IRQn           USART2_IRQn

/**
  * @USB HOST
  */
#define OTG_FS_VBUS_PIN                    GPIO_Pin_9
#define OTG_FS_VBUS_PORT                   GPIOA
#define OTG_FS_VBUS_CLK                    RCC_AHB1Periph_GPIOA
#define OTG_FS_VBUS_SOURCE                 GPIO_PinSource9

#define USB_FS_VBUSON_PIN                  GPIO_Pin_13
#define USB_FS_VBUSON_PORT                 GPIOC
#define USB_FS_VBUSON_CLK                  RCC_AHB1Periph_GPIOC
#define USB_FS_VBUSON_SOURCE               GPIO_PinSource13

#define USB_FS_FAULT_PIN                   GPIO_Pin_8
#define USB_FS_FAULT_PORT                  GPIOA
#define USB_FS_FAULT_CLK                   RCC_AHB1Periph_GPIOA
#define USB_FS_FAULT_SOURCE                GPIO_PinSource8

/**
  * @USB OTG
  */
#define OTG_HS_VBUS_PIN                    GPIO_Pin_13
#define OTG_HS_VBUS_PORT                   GPIOB
#define OTG_HS_VBUS_CLK                    RCC_AHB1Periph_GPIOB
#define OTG_HS_VBUS_SOURCE                 GPIO_PinSource13

#define USB_HS_VBUSON_PIN                  GPIO_Pin_9
#define USB_HS_VBUSON_PORT                 GPIOG
#define USB_HS_VBUSON_CLK                  RCC_AHB1Periph_GPIOG
#define USB_HS_VBUSON_SOURCE               GPIO_PinSource9

#define USB_HS_FAULT_PIN                   GPIO_Pin_7
#define USB_HS_FAULT_PORT                  GPIOD
#define USB_HS_FAULT_CLK                   RCC_AHB1Periph_GPIOD
#define USB_HS_FAULT_SOURCE                GPIO_PinSource7

#define OTG_HS_ID_PIN                      GPIO_Pin_12
#define OTG_HS_ID_PORT                     GPIOB
#define OTG_HS_ID_CLK                      RCC_AHB1Periph_GPIOB
#define OTG_HS_ID_SOURCE                   GPIO_PinSource12

/**
  * @SDRAM
  */
#define FMC_A0_PIN                         GPIO_Pin_0
#define FMC_A1_PIN                         GPIO_Pin_1
#define FMC_A2_PIN                         GPIO_Pin_2
#define FMC_A3_PIN                         GPIO_Pin_3
#define FMC_A4_PIN                         GPIO_Pin_4
#define FMC_A5_PIN                         GPIO_Pin_5
#define FMC_RAS_PIN                        GPIO_Pin_11
#define FMC_A6_PIN                         GPIO_Pin_12
#define FMC_A7_PIN                         GPIO_Pin_13
#define FMC_A8_PIN                         GPIO_Pin_14
#define FMC_A9_PIN                         GPIO_Pin_15

#define FMC_A0_SOURCE                      GPIO_PinSource0
#define FMC_A1_SOURCE                      GPIO_PinSource1
#define FMC_A2_SOURCE                      GPIO_PinSource2
#define FMC_A3_SOURCE                      GPIO_PinSource3
#define FMC_A4_SOURCE                      GPIO_PinSource4
#define FMC_A5_SOURCE                      GPIO_PinSource5
#define FMC_RAS_SOURCE                     GPIO_PinSource11
#define FMC_A6_SOURCE                      GPIO_PinSource12
#define FMC_A7_SOURCE                      GPIO_PinSource13
#define FMC_A8_SOURCE                      GPIO_PinSource14
#define FMC_A9_SOURCE                      GPIO_PinSource15

#define FMC_PORT1                          GPIOF

#define FMC_A10_PIN                        GPIO_Pin_0
#define FMC_A11_PIN                        GPIO_Pin_1
#define FMC_A12_PIN                        GPIO_Pin_2
#define FMC_BA0_PIN                        GPIO_Pin_4
#define FMC_BA1_PIN                        GPIO_Pin_5
#define FMC_CLK_PIN                        GPIO_Pin_8
#define FMC_CAS_PIN                        GPIO_Pin_15

#define FMC_A10_SOURCE                     GPIO_PinSource0
#define FMC_A11_SOURCE                     GPIO_PinSource1
#define FMC_A12_SOURCE                     GPIO_PinSource2
#define FMC_BA0_SOURCE                     GPIO_PinSource4
#define FMC_BA1_SOURCE                     GPIO_PinSource5
#define FMC_CLK_SOURCE                     GPIO_PinSource8
#define FMC_CAS_SOURCE                     GPIO_PinSource15

#define FMC_PORT2                          GPIOG

#define FMC_BL0_PIN                        GPIO_Pin_0
#define FMC_BL1_PIN                        GPIO_Pin_1
#define FMC_D4_PIN                         GPIO_Pin_7
#define FMC_D5_PIN                         GPIO_Pin_8
#define FMC_D6_PIN                         GPIO_Pin_9
#define FMC_D7_PIN                         GPIO_Pin_10
#define FMC_D8_PIN                         GPIO_Pin_11
#define FMC_D9_PIN                         GPIO_Pin_12
#define FMC_D10_PIN                        GPIO_Pin_13
#define FMC_D11_PIN                        GPIO_Pin_14
#define FMC_D12_PIN                        GPIO_Pin_15

#define FMC_BL0_SOURCE                     GPIO_PinSource0
#define FMC_BL1_SOURCE                     GPIO_PinSource1
#define FMC_D4_SOURCE                      GPIO_PinSource7
#define FMC_D5_SOURCE                      GPIO_PinSource8
#define FMC_D6_SOURCE                      GPIO_PinSource9
#define FMC_D7_SOURCE                      GPIO_PinSource10
#define FMC_D8_SOURCE                      GPIO_PinSource11
#define FMC_D9_SOURCE                      GPIO_PinSource12
#define FMC_D10_SOURCE                     GPIO_PinSource13
#define FMC_D11_SOURCE                     GPIO_PinSource14
#define FMC_D12_SOURCE                     GPIO_PinSource15

#define FMC_PORT3                          GPIOE

#define FMC_D0_PIN                         GPIO_Pin_14
#define FMC_D1_PIN                         GPIO_Pin_15
#define FMC_D2_PIN                         GPIO_Pin_0
#define FMC_D3_PIN                         GPIO_Pin_1
#define FMC_D13_PIN                        GPIO_Pin_8
#define FMC_D14_PIN                        GPIO_Pin_9
#define FMC_D15_PIN                        GPIO_Pin_10

#define FMC_D0_SOURCE                      GPIO_PinSource14
#define FMC_D1_SOURCE                      GPIO_PinSource15
#define FMC_D2_SOURCE                      GPIO_PinSource0
#define FMC_D3_SOURCE                      GPIO_PinSource1
#define FMC_D13_SOURCE                     GPIO_PinSource8
#define FMC_D14_SOURCE                     GPIO_PinSource9
#define FMC_D15_SOURCE                     GPIO_PinSource10

#define FMC_PORT4                          GPIOD

#define FMC_NWE_PIN                        GPIO_Pin_0
#define FMC_NE0_PIN                        GPIO_Pin_2
#define FMC_CKE0_PIN                       GPIO_Pin_3

#define FMC_NWE_SOURCE                     GPIO_PinSource0
#define FMC_NE0_SOURCE                     GPIO_PinSource2
#define FMC_CKE0_SOURCE                    GPIO_PinSource3

#define FMC_PORT5                          GPIOC


/**
  * @SPI FLASH
  */
#define SPI_FLASH_MODULE                   SPI5
#define SPI_FLASH_PERIPH_RCC               RCC_APB2Periph_SPI5
#define SPI_FLASH_PORT_PERIPH_RCC          RCC_AHB1Periph_GPIOF

#define SPI_FLASH_MISO_PIN                 GPIO_Pin_8
#define SPI_FLASH_MOSI_PIN                 GPIO_Pin_9
#define SPI_FLASH_NSS_PIN                  GPIO_Pin_6
#define SPI_FLASH_SCK_PIN                  GPIO_Pin_7

#define SPI_FLASH_MISO_SOURCE              GPIO_PinSource8
#define SPI_FLASH_MOSI_SOURCE              GPIO_PinSource9
#define SPI_FLASH_NSS_SOURCE               GPIO_PinSource6
#define SPI_FLASH_SCK_SOURCE               GPIO_PinSource7

#define SPI_FLASH_PORT                     GPIOF

#define SPI_FLASH_AF                       GPIO_AF_SPI5

/**
  * @Touch screen
  */
#define TS_X1_PIN          GPIO_Pin_6
#define TS_X1_SOURCE       GPIO_PinSource6
#define TS_X1_PORT         GPIOH
#define TS_X1_CLK          RCC_AHB1Periph_GPIOH
#define TS_X1_EXTI_LINE    EXTI_Line6
#define TS_X1_EXTI_PIN     EXTI_PinSource6
#define TS_X1_EXTI_PORT    EXTI_PortSourceGPIOH

#define TS_X2_PIN          GPIO_Pin_0
#define TS_X2_SOURCE       GPIO_PinSource0
#define TS_X2_PORT         GPIOB
#define TS_X2_CLK          RCC_AHB1Periph_GPIOB
#define TS_X2_CHANNEL      ADC_Channel_8

#define TS_Y1_PIN          GPIO_Pin_7
#define TS_Y1_SOURCE       GPIO_PinSource7
#define TS_Y1_PORT         GPIOH
#define TS_Y1_CLK          RCC_AHB1Periph_GPIOH

#define TS_Y2_PIN          GPIO_Pin_1
#define TS_Y2_SOURCE       GPIO_PinSource1
#define TS_Y2_PORT         GPIOB
#define TS_Y2_CLK          RCC_AHB1Periph_GPIOB
#define TS_Y2_CHANNEL      ADC_Channel_9

typedef enum
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3
} Led_TypeDef;

typedef enum
{
  BUTTON_USER = 0,
  BUTTON_RIGHT,
  BUTTON_LEFT,
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_SEL
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

typedef enum
{
  COM1 = 0
} COM_TypeDef;

void STM_GPIOReset(void);
void STM_LEDInit(Led_TypeDef Led);
void STM_LEDOn(Led_TypeDef Led);
void STM_LEDOff(Led_TypeDef Led);
void STM_LEDToggle(Led_TypeDef Led);
void STM_ButtonInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t STM_ButtonGetState(Button_TypeDef Button);
void STM_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);
void STM_I2S2Init(void);
void STM_BacklightInit(void);
void STM_BacklightOff(void);
void STM_BacklightOn(void);

#endif /* __IAR_STM32F429II_SK */
