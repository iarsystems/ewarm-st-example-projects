/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2015
 *
 *    File name   : iar_stm32f746xx_sk.h
 *    Description : IAR-STM32F746xx-SK board support package definitions
 *
 *    History :
 *    1. Date        : May, 2015
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: #6 $
 **************************************************************************/

#ifndef __IAR_STM32F746xx_SK
#define __IAR_STM32F746xx_SK

#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

#define LEDn                             4

#define LED1_PIN                         GPIO_PIN_8
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()

#define LED2_PIN                         GPIO_PIN_3
#define LED2_GPIO_PORT                   GPIOG
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOG_CLK_ENABLE()

#define LED3_PIN                         GPIO_PIN_4
#define LED3_GPIO_PORT                   GPIOB
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()

#define LED4_PIN                         GPIO_PIN_3
#define LED4_GPIO_PORT                   GPIOE
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()

/**
  * @BUZZER
  */
#define BUZZER_GPIO_PIN                 GPIO_PIN_7
#define BUZZER_GPIO_PORT                GPIOB
#define BUZZER_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()

/**
  * @TRIMER
  */
#define TRIMER_PIN                      GPIO_PIN_6
#define TRIMER_PORT                     GPIOA
#define TRIMER_CLK_ENABLE()             __HAL_RCC_GPIOA_CLK_ENABLE()
#define TRIMER_CHANNEL                  ADC_CHANNEL_6

/**
  * @TEMP_SENSOR
  */
#define TEMP_SENSOR_INT_PIN             GPIO_PIN_13
#define TEMP_SENSOR_INT_PORT            GPIOD
#define TEMP_SENSOR_INT_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()

/**
  * @LCD
  */
#define LCD_BACKLIGHT_PIN               GPIO_PIN_7
#define LCD_BACKLIGHT_PORT              GPIOC
#define LCD_BACKLIGHT_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

/**
  * @I2C
  */
#define I2C2_SCL_PIN                    GPIO_PIN_4
#define I2C2_SCL_PORT                   GPIOH
#define I2C2_SCL_AF                     GPIO_AF4_I2C2
#define I2C2_SCL_CLK_ENABLE()           __HAL_RCC_GPIOH_CLK_ENABLE()

#define I2C2_SDA_PIN                    GPIO_PIN_5
#define I2C2_SDA_PORT                   GPIOH
#define I2C2_SDA_AF                     GPIO_AF4_I2C2
#define I2C2_SDA_CLK_ENABLE()           __HAL_RCC_GPIOH_CLK_ENABLE()

#define I2C2_CLK_ENABLE()               __HAL_RCC_I2C2_CLK_ENABLE()
#define I2C2_FORCE_RESET()              __HAL_RCC_I2C2_FORCE_RESET()
#define I2C2_RELEASE_RESET()            __HAL_RCC_I2C2_RELEASE_RESET()


/**
  * @PWM header
  */
#define PWM_CH0_PIN                     GPIO_PIN_7
#define PWM_CH0_PORT                    GPIOB
#define PWM_CH0_CLK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()

#define PWM_CH1_PIN                     GPIO_PIN_2
#define PWM_CH1_PORT                    GPIOI
#define PWM_CH1_CLK_ENABLE()            __HAL_RCC_GPIOI_CLK_ENABLE()

#define PWM_CH2_PIN                     GPIO_PIN_4
#define PWM_CH2_PORT                    GPIOB
#define PWM_CH2_CLK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()

#define PWM_CH3_PIN                     GPIO_PIN_15
#define PWM_CH3_PORT                    GPIOA
#define PWM_CH3_CLK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()

/**
  * @I2S
  */
#define I2S_IN_MODULE                  SPI1
#define I2S_OUT_MODULE                 SPI2

#define I2S_IN_SD_PIN                  GPIO_PIN_5
#define I2S_IN_SD_PORT                 GPIOB
#define I2S_IN_SD_AF                   GPIO_AF5_SPI1
#define I2S_IN_SD_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()

#define I2S_OUT_SD_PIN                 GPIO_PIN_3
#define I2S_OUT_SD_PORT                GPIOI
#define I2S_OUT_SD_AF                  GPIO_AF5_SPI2
#define I2S_OUT_SD_CLK_ENABLE()        __HAL_RCC_GPIOI_CLK_ENABLE()

#define I2S_IN_CK_PIN                  GPIO_PIN_5
#define I2S_IN_CK_PORT                 GPIOA
#define I2S_IN_CK_AF                   GPIO_AF5_SPI1
#define I2S_IN_CK_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()

#define I2S_OUT_CK_PIN                 GPIO_PIN_10
#define I2S_OUT_CK_PORT                GPIOB
#define I2S_OUT_CK_AF                  GPIO_AF5_SPI2
#define I2S_OUT_CK_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()

#define I2S_IN_WS_PIN                  GPIO_PIN_4
#define I2S_IN_WS_PORT                 GPIOA
#define I2S_IN_WS_AF                   GPIO_AF5_SPI1
#define I2S_IN_WS_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()

#define I2S_OUT_WS_PIN                 GPIO_PIN_9
#define I2S_OUT_WS_PORT                GPIOB
#define I2S_OUT_WS_AF                  GPIO_AF5_SPI2
#define I2S_OUT_WS_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()

#define I2S_MCK_PIN                    GPIO_PIN_6
#define I2S_MCK_PORT                   GPIOC
#define I2S_MCK_AF                     GPIO_AF5_SPI2
#define I2S_MCK_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()

#define I2S_IN_CLK_ENABLE()            __HAL_RCC_SPI1_CLK_ENABLE()
#define I2S_IN_CLK_DISABLE()           __HAL_RCC_SPI1_CLK_DISABLE()
#define I2S_IN_FORCE_RESET()           __HAL_RCC_SPI1_FORCE_RESET()
#define I2S_IN_RELEASE_RESET()         __HAL_RCC_SPI1_RELEASE_RESET()
#define I2S_IN_IRQ                     SPI1_IRQn

#define I2S_OUT_CLK_ENABLE()           __HAL_RCC_SPI2_CLK_ENABLE()
#define I2S_OUT_CLK_DISABLE()          __HAL_RCC_SPI2_CLK_ENABLE()
#define I2S_OUT_FORCE_RESET()          __HAL_RCC_SPI2_FORCE_RESET()
#define I2S_OUT_RELEASE_RESET()        __HAL_RCC_SPI2_RELEASE_RESET()
#define I2S_OUT_IRQ                    SPI2_IRQn


/**
  * @ACC_GYRO_SENSOR
  */
#define ACC_GYRO_INT1_PIN               GPIO_PIN_11
#define ACC_GYRO_INT1_PORT              GPIOD
#define ACC_GYRO_INT1_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()

#define ACC_GYRO_INT2_PIN               GPIO_PIN_10
#define ACC_GYRO_INT2_PORT              GPIOA
#define ACC_GYRO_INT2_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

/**
  * @MAG_SENSOR
  */
#define MAG_INT_PIN                     GPIO_PIN_12
#define MAG_INT_PORT                    GPIOD
#define MAG_INT_CLK_ENABLE()            __HAL_RCC_GPIOD_CLK_ENABLE()

/**
  * @LOW_LEVEL_BUTTONS
  */
#define BUTTONn                          6 /**/

/**
 * @brief Key push-button
 */
#define USER_BUTTON_PIN                   GPIO_PIN_0
#define USER_BUTTON_GPIO_PORT             GPIOA
#define USER_BUTTON_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER_BUTTON_EXTI_LINE             EXTI_Line0
#define USER_BUTTON_EXTI_IRQn             EXTI0_IRQn
/**
 * @brief Joystick Right
 */
#define RIGHT_BUTTON_PIN                  GPIO_PIN_8
#define RIGHT_BUTTON_GPIO_PORT            GPIOI
#define RIGHT_BUTTON_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOI_CLK_ENABLE()
#define RIGHT_BUTTON_EXTI_LINE            EXTI_Line8
#define RIGHT_BUTTON_EXTI_IRQn            EXTI9_5_IRQn
/**
 * @brief Joystick Left
 */
#define LEFT_BUTTON_PIN                   GPIO_PIN_9
#define LEFT_BUTTON_GPIO_PORT             GPIOI
#define LEFT_BUTTON_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOI_CLK_ENABLE()
#define LEFT_BUTTON_EXTI_LINE             EXTI_Line9
#define LEFT_BUTTON_EXTI_IRQn             EXTI9_5_IRQn
/**
 * @brief Joystick Up
 */
#define UP_BUTTON_PIN                     GPIO_PIN_11
#define UP_BUTTON_GPIO_PORT               GPIOI
#define UP_BUTTON_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOI_CLK_ENABLE()
#define UP_BUTTON_EXTI_LINE               EXTI_Line11
#define UP_BUTTON_EXTI_IRQn               EXTI15_10_IRQn
/**
 * @brief Joystick Down
 */
#define DOWN_BUTTON_PIN                   GPIO_PIN_10
#define DOWN_BUTTON_GPIO_PORT             GPIOI
#define DOWN_BUTTON_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOI_CLK_ENABLE()
#define DOWN_BUTTON_EXTI_LINE             EXTI_Line10
#define DOWN_BUTTON_EXTI_IRQn             EXTI15_10_IRQn
/**
 * @brief Joystick Sel
 */
#define SEL_BUTTON_PIN                    GPIO_PIN_2
#define SEL_BUTTON_GPIO_PORT              GPIOI
#define SEL_BUTTON_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOI_CLK_ENABLE()
#define SEL_BUTTON_EXTI_LINE              EXTI_Line2
#define SEL_BUTTON_EXTI_IRQn              EXTI2_IRQn

/**
  * @SD
  */
#define SD_CP_PIN                       GPIO_PIN_9
#define SD_CP_PORT                      GPIOG
#define SD_CP_CLK_ENABLE()              __HAL_RCC_GPIOG_CLK_ENABLE()

#define SD_WP_PIN                       GPIO_PIN_3
#define SD_WP_PORT                      GPIOG
#define SD_WP_CLK_ENABLE()              __HAL_RCC_GPIOG_CLK_ENABLE()

#define SD_CMD_PIN                      GPIO_PIN_2
#define SD_CMD_PORT                     GPIOD
#define SD_CMD_CLK_ENABLE()             __HAL_RCC_GPIOD_CLK_ENABLE()

#define SD_D0_PIN                       GPIO_PIN_8
#define SD_D0_PORT                      GPIOC
#define SD_D0_CLK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()

#define SD_D1_PIN                       GPIO_PIN_9
#define SD_D1_PORT                      GPIOC
#define SD_D1_CLK_ENABLE()              __HAL_RCC_GPIOC_CLK_ENABLE()

#define SD_D2_PIN                       GPIO_PIN_10
#define SD_D2_PORT                      GPIOC
#define SD_D2_CLK_ENABLE()              __HAL_RCC_GPIOC_CLK_ENABLE()

#define SD_D3_PIN                       GPIO_PIN_11
#define SD_D3_PORT                      GPIOC
#define SD_D3_CLK_ENABLE()              __HAL_RCC_GPIOC_CLK_ENABLE()

#define SD_CLK_PIN                      GPIO_PIN_12
#define SD_CLK_PORT                     GPIOC
#define SD_CLK_CLK_ENABLE()             __HAL_RCC_GPIOC_CLK_ENABLE()

/**
  * @COM1
  */
//Definition for COM port, connected to USART2
#define COMn                1

/* Definition for USARTx clock resources */
#define COM1_BASE                  USART2
#define COM1_CLK_ENABLE()          __HAL_RCC_USART2_CLK_ENABLE()
#define COM1_RX_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOD_CLK_ENABLE()
#define COM1_TX_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOD_CLK_ENABLE()

#define COM1_FORCE_RESET()         __HAL_RCC_USART2_FORCE_RESET()
#define COM1_RELEASE_RESET()       __HAL_RCC_USART2_RELEASE_RESET()

#define COM1_PERIPH_CLK            RCC_PERIPHCLK_USART2
#define COM1_CLKSOURCE             RCC_USART2CLKSOURCE_SYSCLK

/* Definition for USARTx Pins */
#define COM1_TX_PIN         GPIO_PIN_5
#define COM1_TX_GPIO_PORT   GPIOD
#define COM1_TX_AF          GPIO_AF7_USART2
#define COM1_RX_PIN         GPIO_PIN_6
#define COM1_RX_GPIO_PORT   GPIOD
#define COM1_RX_AF          GPIO_AF7_USART2

#define COM1_IRQn           USART2_IRQn
#define COM1_IRQHandler     USART2_IRQHandler

/**
  * @USB HOST
  */
#define USB_FS_VBUS_PIN                    GPIO_PIN_9
#define USB_FS_VBUS_PORT                   GPIOA
#define USB_FS_VBUS_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()

#define USB_FS_VBUSON_PIN                  GPIO_PIN_13
#define USB_FS_VBUSON_PORT                 GPIOC
#define USB_FS_VBUSON_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()

#define USB_FS_FAULT_PIN                   GPIO_PIN_8
#define USB_FS_FAULT_PORT                  GPIOA
#define USB_FS_FAULT_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()

/**
  * @USB OTG
  */
#define OTG_HS_VBUS_PIN                    GPIO_PIN_13
#define OTG_HS_VBUS_PORT                   GPIOB
#define OTG_HS_VBUS_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()

#define OTG_HS_VBUSON_PIN                  GPIO_PIN_9
#define OTG_HS_VBUSON_PORT                 GPIOG
#define OTG_HS_VBUSON_CLK_ENABLE()         __HAL_RCC_GPIOG_CLK_ENABLE()

#define OTG_HS_FAULT_PIN                   GPIO_PIN_7
#define OTG_HS_FAULT_PORT                  GPIOD
#define OTG_HS_FAULT_CLK_ENABLE()          __HAL_RCC_GPIOD_CLK_ENABLE()

#define OTG_HS_ID_PIN                      GPIO_PIN_12
#define OTG_HS_ID_PORT                     GPIOB
#define OTG_HS_ID_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()

/**
  * @SDRAM
  */
#define FMC_A0_PIN                         GPIO_PIN_0
#define FMC_A1_PIN                         GPIO_PIN_1
#define FMC_A2_PIN                         GPIO_PIN_2
#define FMC_A3_PIN                         GPIO_PIN_3
#define FMC_A4_PIN                         GPIO_PIN_4
#define FMC_A5_PIN                         GPIO_PIN_5
#define FMC_RAS_PIN                        GPIO_PIN_11
#define FMC_A6_PIN                         GPIO_PIN_12
#define FMC_A7_PIN                         GPIO_PIN_13
#define FMC_A8_PIN                         GPIO_PIN_14
#define FMC_A9_PIN                         GPIO_PIN_15

#define FMC_PORT1                          GPIOF
#define FMC_PORT1_CLK_ENABLE()             __HAL_RCC_GPIOF_CLK_ENABLE()

#define FMC_A10_PIN                        GPIO_PIN_0
#define FMC_A11_PIN                        GPIO_PIN_1
#define FMC_A12_PIN                        GPIO_PIN_2
#define FMC_BA0_PIN                        GPIO_PIN_4
#define FMC_BA1_PIN                        GPIO_PIN_5
#define FMC_CLK_PIN                        GPIO_PIN_8
#define FMC_CAS_PIN                        GPIO_PIN_15

#define FMC_PORT2                          GPIOG
#define FMC_PORT2_CLK_ENABLE()             __HAL_RCC_GPIOG_CLK_ENABLE()

#define FMC_BL0_PIN                        GPIO_PIN_0
#define FMC_BL1_PIN                        GPIO_PIN_1
#define FMC_D4_PIN                         GPIO_PIN_7
#define FMC_D5_PIN                         GPIO_PIN_8
#define FMC_D6_PIN                         GPIO_PIN_9
#define FMC_D7_PIN                         GPIO_PIN_10
#define FMC_D8_PIN                         GPIO_PIN_11
#define FMC_D9_PIN                         GPIO_PIN_12
#define FMC_D10_PIN                        GPIO_PIN_13
#define FMC_D11_PIN                        GPIO_PIN_14
#define FMC_D12_PIN                        GPIO_PIN_15

#define FMC_PORT3                          GPIOE
#define FMC_PORT3_CLK_ENABLE()             __HAL_RCC_GPIOE_CLK_ENABLE()

#define FMC_D0_PIN                         GPIO_PIN_14
#define FMC_D1_PIN                         GPIO_PIN_15
#define FMC_D2_PIN                         GPIO_PIN_0
#define FMC_D3_PIN                         GPIO_PIN_1
#define FMC_D13_PIN                        GPIO_PIN_8
#define FMC_D14_PIN                        GPIO_PIN_9
#define FMC_D15_PIN                        GPIO_PIN_10

#define FMC_PORT4                          GPIOD
#define FMC_PORT4_CLK_ENABLE()             __HAL_RCC_GPIOD_CLK_ENABLE()

#define FMC_NWE_PIN                        GPIO_PIN_0
#define FMC_NE0_PIN                        GPIO_PIN_2
#define FMC_CKE0_PIN                       GPIO_PIN_3

#define FMC_PORT5                          GPIOC
#define FMC_PORT5_CLK_ENABLE()             __HAL_RCC_GPIOC_CLK_ENABLE()

/**
  * @SPI FLASH
  */
#define QSPI_FLASH_MODULE                  QUADSPI
#define QSPI_FLASH_FORCE_RESET()           __HAL_RCC_QSPI_FORCE_RESET()
#define QSPI_FLASH_RELEASE_RESET()         __HAL_RCC_QSPI_RELEASE_RESET()
#define QSPI_FLASH_CLK_ENABLE()            __HAL_RCC_QSPI_CLK_ENABLE()
#define QSPI_FLASH_CLK_DISABLE()           __HAL_RCC_QSPI_CLK_DISABLE()
#define QSPI_FLASH_GPIO_IO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_FLASH_GPIO_CS_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()

#define QSPI_FLASH_IO0_PIN                 GPIO_PIN_8
#define QSPI_FLASH_IO1_PIN                 GPIO_PIN_9
#define QSPI_FLASH_IO2_PIN                 GPIO_PIN_7
#define QSPI_FLASH_IO3_PIN                 GPIO_PIN_6

#define QSPI_FLASH_CS_PIN                  GPIO_PIN_6
#define QSPI_FLASH_CLK_PIN                 GPIO_PIN_2

#define QSPI_FLASH_IO_PORT                 GPIOF
#define QSPI_FLASH_CS_PORT                 GPIOB
#define QSPI_FLASH_CLK_PORT                GPIOB

/**
  * @Touch screen
  */
#define TS_X1_PIN          GPIO_PIN_6
#define TS_X1_PORT         GPIOH
#define TS_X1_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()

#define TS_X2_PIN          GPIO_PIN_0
#define TS_X2_PORT         GPIOB
#define TS_X2_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define TS_X2_CHANNEL      ADC_CHANNEL_8

#define TS_Y1_PIN          GPIO_PIN_7
#define TS_Y1_PORT         GPIOH
#define TS_Y1_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()

#define TS_Y2_PIN          GPIO_PIN_1
#define TS_Y2_PORT         GPIOB
#define TS_Y2_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define TS_Y2_CHANNEL      ADC_CHANNEL_9


/**
  * @RTC Setting
  */

#define RTC_CLOCK_SOURCE_LSE


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

void STM_GPIOReset(void);
void STM_LEDInit(Led_TypeDef Led);
void STM_LEDOn(Led_TypeDef Led);
void STM_LEDOff(Led_TypeDef Led);
void STM_LEDToggle(Led_TypeDef Led);
void STM_ButtonInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t STM_ButtonGetState(Button_TypeDef Button);
uint32_t STM_COMInit(UART_HandleTypeDef * pUartHandle, USART_TypeDef *COM_BASE);
uint32_t STM_I2SInit(I2S_HandleTypeDef * hi2s_out, I2S_HandleTypeDef * hi2s_in);
void STM_BacklightInit(void);
void STM_BacklightOff(void);
void STM_BacklightOn(void);

#endif /* __IAR_STM32F746xx_SK */
