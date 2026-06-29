/**
 * @file    stm32f401re.h
 * @brief   MCU specific header file containing memory map and register definitions.
 */

#ifndef STM32F401RE_H
#define STM32F401RE_H

#include <stdint.h>
#include <stddef.h>
#include "core_cm4.h"

#define __weak __attribute__((weak))

/** * @defgroup MEMORY_MAP Base addresses of Flash and SRAM
 * @{
 */
#define FLASH_BASEADDR          0x08000000U
#define SRAM_BASEADDR           0x20000000U
#define ROM_BASEADDR            0x1FFF0000U
/** @} */

/** * @defgroup BUS_MAP AHBx and APBx Bus Peripheral base addresses
 * @{
 */
#define PERIPH_BASE             0x40000000U
#define APB1PERIPH_BASE         PERIPH_BASE
#define APB2PERIPH_BASE         0x40010000U
#define AHB1PERIPH_BASE         0x40020000U
#define AHB2PERIPH_BASE         0x50000000U
/** @} */

/** * @defgroup AHB1_PERIPHERALS Base addresses of Peripherals on AHB1 bus
 * @{
 */
#define GPIOA_BASEADDR          (AHB1PERIPH_BASE + 0x0000U)
#define GPIOB_BASEADDR          (AHB1PERIPH_BASE + 0x0400U)
#define GPIOC_BASEADDR          (AHB1PERIPH_BASE + 0x0800U)
#define GPIOD_BASEADDR          (AHB1PERIPH_BASE + 0x0C00U)
#define GPIOE_BASEADDR          (AHB1PERIPH_BASE + 0x1000U)
#define GPIOH_BASEADDR          (AHB1PERIPH_BASE + 0x1C00U)
#define RCC_BASEADDR            (AHB1PERIPH_BASE + 0x3800U)
/** @} */

/** * @defgroup APB1_PERIPHERALS Base addresses of Peripherals on APB1 bus
 * @{
 */
#define I2C1_BASEADDR           (APB1PERIPH_BASE + 0x5400U)
#define I2C2_BASEADDR           (APB1PERIPH_BASE + 0x5800U)
#define I2C3_BASEADDR           (APB1PERIPH_BASE + 0x5C00U)
#define SPI2_BASEADDR           (APB1PERIPH_BASE + 0x3800U)
#define SPI3_BASEADDR           (APB1PERIPH_BASE + 0x3C00U)
#define USART2_BASEADDR         (APB1PERIPH_BASE + 0x4400U)
/** @} */

/** * @defgroup APB2_PERIPHERALS Base addresses of Peripherals on APB2 bus
 * @{
 */
#define EXTI_BASEADDR           (APB2PERIPH_BASE + 0x3C00U)
#define SPI1_BASEADDR           (APB2PERIPH_BASE + 0x3000U)
#define SPI4_BASEADDR           (APB2PERIPH_BASE + 0x3400U)
#define SYSCFG_BASEADDR         (APB2PERIPH_BASE + 0x3800U)
#define USART1_BASEADDR         (APB2PERIPH_BASE + 0x1000U)
#define USART6_BASEADDR         (APB2PERIPH_BASE + 0x1400U)
/** @} */


/******************************************************************************/
/* Peripheral Register Structures                        */
/******************************************************************************/

/**
 * @brief General Purpose I/O (GPIO) register definition structure
 */
typedef struct {
    volatile uint32_t MODER;    /**< GPIO port mode register */
    volatile uint32_t OTYPER;   /**< GPIO port output type register */
    volatile uint32_t OSPEEDR;  /**< GPIO port output speed register */
    volatile uint32_t PUPDR;    /**< GPIO port pull-up/pull-down register */
    volatile uint32_t IDR;      /**< GPIO port input data register */
    volatile uint32_t ODR;      /**< GPIO port output data register */
    volatile uint32_t BSRR;     /**< GPIO port bit set/reset register */
    volatile uint32_t LCKR;     /**< GPIO port configuration lock register */
    volatile uint32_t AFR[2];   /**< GPIO alternate function register; AFR[0]: low, AFR[1]: high */
} GPIO_RegDef_t;

/**
 * @brief Reset and Clock Control (RCC) register definition structure
 */
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    uint32_t RESERVED0[2];
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    uint32_t RESERVED2[2];
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
    uint32_t RESERVED3[2];
    volatile uint32_t AHB1LPENR;
    volatile uint32_t AHB2LPENR;
    uint32_t RESERVED4[2];
    volatile uint32_t APB1LPENR;
    volatile uint32_t APB2LPENR;
    uint32_t RESERVED5[2];
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    uint32_t RESERVED6[2];
    volatile uint32_t SSCGR;
    volatile uint32_t PLLI2SCFGR;
    uint32_t RESERVED7;
    volatile uint32_t DCKCFGR;
} RCC_RegDef_t;

/**
 * @brief External Interrupt/Event Controller (EXTI) register definition structure
 */
typedef struct {
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
} EXTI_RegDef_t;

/**
 * @brief System Configuration Controller (SYSCFG) register definition structure
 */
typedef struct {
    volatile uint32_t MEMRMP;
    volatile uint32_t PMC;
    volatile uint32_t EXTICR[4];
    uint32_t RESERVED[2];
    volatile uint32_t CMPCR;
} SYSCFG_RegDef_t;

/**
 * @brief Serial Peripheral Interface (SPI) register definition structure
 */
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CRCPR;
    volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR;
    volatile uint32_t I2SCFGR;
    volatile uint32_t I2SPR;
} SPI_RegDef_t;

/**
 * @brief Inter-Integrated Circuit (I2C) register definition structure
 */
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t DR;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
    volatile uint32_t CCR;
    volatile uint32_t TRISE;
    volatile uint32_t FLTR;
} I2C_RegDef_t;

/**
 * @brief Universal Synchronous Asynchronous Receiver Transmitter (USART) register definition structure
 */
typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_RegDef_t;

/******************************************************************************/
/* Peripheral Base Address Type-Casting Macros                                */
/******************************************************************************/

/** * @defgroup PERIPH_MACROS Peripheral pointers
 * @{
 */
#define GPIOA           ((GPIO_RegDef_t*) GPIOA_BASEADDR)
#define GPIOB           ((GPIO_RegDef_t*) GPIOB_BASEADDR)
#define GPIOC           ((GPIO_RegDef_t*) GPIOC_BASEADDR)
#define GPIOD           ((GPIO_RegDef_t*) GPIOD_BASEADDR)
#define GPIOE           ((GPIO_RegDef_t*) GPIOE_BASEADDR)
#define GPIOH           ((GPIO_RegDef_t*) GPIOH_BASEADDR)

#define RCC             ((RCC_RegDef_t*) RCC_BASEADDR)

#define EXTI            ((EXTI_RegDef_t*) EXTI_BASEADDR)

#define SYSCFG          ((SYSCFG_RegDef_t*) SYSCFG_BASEADDR)

#define SPI1            ((SPI_RegDef_t*) SPI1_BASEADDR)
#define SPI2            ((SPI_RegDef_t*) SPI2_BASEADDR)
#define SPI3            ((SPI_RegDef_t*) SPI3_BASEADDR)
#define SPI4            ((SPI_RegDef_t*) SPI4_BASEADDR)

#define I2C1            ((I2C_RegDef_t*) I2C1_BASEADDR)
#define I2C2            ((I2C_RegDef_t*) I2C2_BASEADDR)
#define I2C3            ((I2C_RegDef_t*) I2C3_BASEADDR)

#define USART1          ((USART_RegDef_t*) USART1_BASEADDR)
#define USART2          ((USART_RegDef_t*) USART2_BASEADDR)
#define USART6          ((USART_RegDef_t*) USART6_BASEADDR)
/** @} */

/******************************************************************************/
/* Clock Enable & Disable Macros                                              */
/******************************************************************************/

/** * @defgroup GPIO_CLOCK_ENABLE GPIO Clock Enable/Disable Macros
 * @{
 */
#define GPIOA_PCLK_EN()     (RCC->AHB1ENR |= (1U<<0))
#define GPIOB_PCLK_EN()     (RCC->AHB1ENR |= (1U<<1))
#define GPIOC_PCLK_EN()     (RCC->AHB1ENR |= (1U<<2))
#define GPIOD_PCLK_EN()     (RCC->AHB1ENR |= (1U<<3))
#define GPIOE_PCLK_EN()     (RCC->AHB1ENR |= (1U<<4))
#define GPIOH_PCLK_EN()     (RCC->AHB1ENR |= (1U<<7))

#define GPIOA_PCLK_DI()     (RCC->AHB1ENR &= ~(1U<<0))
#define GPIOB_PCLK_DI()     (RCC->AHB1ENR &= ~(1U<<1))
#define GPIOC_PCLK_DI()     (RCC->AHB1ENR &= ~(1U<<2))
#define GPIOD_PCLK_DI()     (RCC->AHB1ENR &= ~(1U<<3))
#define GPIOE_PCLK_DI()     (RCC->AHB1ENR &= ~(1U<<4))
#define GPIOH_PCLK_DI()     (RCC->AHB1ENR &= ~(1U<<7))
/** @} */


/** * @defgroup I2C_CLOCK_ENABLE I2C Clock Enable/Disable Macros
 * @{
 */
#define I2C1_PCLK_EN()      (RCC->APB1ENR |= (1U<<21))
#define I2C2_PCLK_EN()      (RCC->APB1ENR |= (1U<<22))
#define I2C3_PCLK_EN()      (RCC->APB1ENR |= (1U<<23))

#define I2C1_PCLK_DI()      (RCC->APB1ENR &= ~(1U<<21))
#define I2C2_PCLK_DI()      (RCC->APB1ENR &= ~(1U<<22))
#define I2C3_PCLK_DI()      (RCC->APB1ENR &= ~(1U<<23))
/** @} */


/** * @defgroup SPI_CLOCK_ENABLE SPI Clock Enable/Disable Macros
 * @{
 */
#define SPI1_PCLK_EN()      (RCC->APB2ENR |= (1U<<12))
#define SPI2_PCLK_EN()      (RCC->APB1ENR |= (1U<<14))
#define SPI3_PCLK_EN()      (RCC->APB1ENR |= (1U<<15))
#define SPI4_PCLK_EN()      (RCC->APB2ENR |= (1U<<13))

#define SPI1_PCLK_DI()      (RCC->APB2ENR &= ~(1U<<12))
#define SPI2_PCLK_DI()      (RCC->APB1ENR &= ~(1U<<14))
#define SPI3_PCLK_DI()      (RCC->APB1ENR &= ~(1U<<15))
#define SPI4_PCLK_DI()      (RCC->APB2ENR &= ~(1U<<13))
/** @} */


/** * @defgroup USART_CLOCK_ENABLE USART Clock Enable/Disable Macros
 * @{
 */
#define USART1_PCLK_EN()    (RCC->APB2ENR |= (1U<<4))
#define USART2_PCLK_EN()    (RCC->APB1ENR |= (1U<<17))
#define USART6_PCLK_EN()    (RCC->APB2ENR |= (1U<<5))

#define USART1_PCLK_DI()    (RCC->APB2ENR &= ~(1U<<4))
#define USART2_PCLK_DI()    (RCC->APB1ENR &= ~(1U<<17))
#define USART6_PCLK_DI()    (RCC->APB2ENR &= ~(1U<<5))
/** @} */


/** * @defgroup SYSCFG_CLOCK_ENABLE SYSCFG Clock Enable Macro
 * @{
 */
#define SYSCFG_PCLK_EN()    (RCC->APB2ENR |= (1U << 14))
/** @} */

/******************************************************************************/
/* Peripheral Reset Macros                                                    */
/******************************************************************************/

/** * @defgroup GPIO_RESET GPIO Reset Macros
 * @{
 */
#define GPIOA_REG_RESET()   do { (RCC->AHB1RSTR |= (1U<<0)); (RCC->AHB1RSTR &= ~(1U<<0)); } while(0)
#define GPIOB_REG_RESET()   do { (RCC->AHB1RSTR |= (1U<<1)); (RCC->AHB1RSTR &= ~(1U<<1)); } while(0)
#define GPIOC_REG_RESET()   do { (RCC->AHB1RSTR |= (1U<<2)); (RCC->AHB1RSTR &= ~(1U<<2)); } while(0)
#define GPIOD_REG_RESET()   do { (RCC->AHB1RSTR |= (1U<<3)); (RCC->AHB1RSTR &= ~(1U<<3)); } while(0)
#define GPIOE_REG_RESET()   do { (RCC->AHB1RSTR |= (1U<<4)); (RCC->AHB1RSTR &= ~(1U<<4)); } while(0)
#define GPIOH_REG_RESET()   do { (RCC->AHB1RSTR |= (1U<<7)); (RCC->AHB1RSTR &= ~(1U<<7)); } while(0)
/** @} */

/** * @defgroup SPI_RESET SPI Reset Macros
 * @{
 */
#define SPI1_REG_RESET()    do { (RCC->APB2RSTR |= (1U<<12)); (RCC->APB2RSTR &= ~(1U<<12)); } while(0)
#define SPI2_REG_RESET()    do { (RCC->APB1RSTR |= (1U<<14)); (RCC->APB1RSTR &= ~(1U<<14)); } while(0)
#define SPI3_REG_RESET()    do { (RCC->APB1RSTR |= (1U<<15)); (RCC->APB1RSTR &= ~(1U<<15)); } while(0)
#define SPI4_REG_RESET()    do { (RCC->APB2RSTR |= (1U<<13)); (RCC->APB2RSTR &= ~(1U<<13)); } while(0)
/** @} */

/** * @defgroup I2C_RESET I2C Reset Macros
 * @{
 */
#define I2C1_REG_RESET()    do { (RCC->APB1RSTR |= (1U<<21)); (RCC->APB1RSTR &= ~(1U<<21)); } while(0)
#define I2C2_REG_RESET()    do { (RCC->APB1RSTR |= (1U<<22)); (RCC->APB1RSTR &= ~(1U<<22)); } while(0)
#define I2C3_REG_RESET()    do { (RCC->APB1RSTR |= (1U<<23)); (RCC->APB1RSTR &= ~(1U<<23)); } while(0)
/** @} */

/** * @defgroup USART_RESET USART Reset Macros
 * @{
 */
#define USART1_REG_RESET()  do { (RCC->APB2RSTR |= (1U<<4));  (RCC->APB2RSTR &= ~(1U<<4)); } while(0)
#define USART2_REG_RESET()  do { (RCC->APB1RSTR |= (1U<<17)); (RCC->APB1RSTR &= ~(1U<<17)); } while(0)
#define USART6_REG_RESET()  do { (RCC->APB2RSTR |= (1U<<5));  (RCC->APB2RSTR &= ~(1U<<5)); } while(0)
/** @} */


/**
 * @brief  Returns a numerical code for a given GPIO port base address.
 * Useful for EXTI line configuration.
 * @param  x: Base address of the GPIO port (e.g., GPIOA, GPIOB).
 */
#define GPIO_BASEADDR_TO_CODE(x)    ((x == GPIOA) ? 0 :\
                                     (x == GPIOB) ? 1 :\
                                     (x == GPIOC) ? 2 :\
                                     (x == GPIOD) ? 3 :\
                                     (x == GPIOE) ? 4 :\
                                     (x == GPIOH) ? 7 : 0)

/******************************************************************************/
/* Interrupt Request (IRQ) Numbers                                            */
/******************************************************************************/

/** * @defgroup IRQ_NUMBERS NVIC IRQ Numbers for STM32F401RE
 * @{
 */
#define IRQ_NO_EXTIO        6
#define IRQ_NO_EXTI1        7
#define IRQ_NO_EXTI2        8
#define IRQ_NO_EXTI3        9
#define IRQ_NO_EXTI4        10
#define IRQ_NO_EXTI9_5      23
#define IRQ_NO_EXTI15_10    40
#define IRQ_NO_SPI1         35
#define IRQ_NO_SPI2         36
#define IRQ_NO_SPI3         51
#define IRQ_NO_SPI4         84
#define IRQ_NO_I2C1_EV      31
#define IRQ_NO_I2C1_ER      32
#define IRQ_NO_I2C2_EV      33
#define IRQ_NO_I2C2_ER      34
#define IRQ_NO_I2C3_EV      72
#define IRQ_NO_I2C3_ER      73
#define IRQ_NO_USART1       37
#define IRQ_NO_USART2       38
#define IRQ_NO_USART6       71
/** @} */

/******************************************************************************/
/* SPI Register Bit Positions                                                 */
/******************************************************************************/

/** * @defgroup SPI_CR1_BIT_POS SPI Control Register 1 (SPI_CR1) Bit Positions
 * @{
 */
#define SPI_CR1_CPHA            (0U)
#define SPI_CR1_CPOL            (1U)
#define SPI_CR1_MSTR            (2U)
#define SPI_CR1_BR              (3U)    /**< Bits [5:3]: Baud rate control position */
#define SPI_CR1_SPE             (6U)
#define SPI_CR1_LSBFIRST        (7U)
#define SPI_CR1_SSI             (8U)
#define SPI_CR1_SSM             (9U)
#define SPI_CR1_RXONLY          (10U)
#define SPI_CR1_DFF             (11U)
#define SPI_CR1_CRCNEXT         (12U)
#define SPI_CR1_CRCEN           (13U)
#define SPI_CR1_BIDIOE          (14U)
#define SPI_CR1_BIDIMODE        (15U)
/** @} */

/** * @defgroup SPI_CR2_BIT_POS SPI Control Register 2 (SPI_CR2) Bit Positions
 * @{
 */
#define SPI_CR2_RXDMAEN         (0U)
#define SPI_CR2_TXDMAEN         (1U)
#define SPI_CR2_SSOE            (2U)
/* Bit 3 Reserved */
#define SPI_CR2_FRF             (4U)
#define SPI_CR2_ERRIE           (5U)
#define SPI_CR2_RXNEIE          (6U)
#define SPI_CR2_TXEIE           (7U)
/* Bits [15:8] Reserved */
/** @} */

/** * @defgroup SPI_SR_BIT_POS SPI Status Register (SPI_SR) Bit Positions
 * @{
 */
#define SPI_SR_RXNE             (0U)
#define SPI_SR_TXE              (1U)
#define SPI_SR_CHSIDE           (2U)
#define SPI_SR_UDR              (3U)
#define SPI_SR_CRCERR           (4U)
#define SPI_SR_MODF             (5U)
#define SPI_SR_OVR              (6U)
#define SPI_SR_BSY              (7U)
#define SPI_SR_FRE              (8U)
/* Bits [15:9] Reserved */
/** @} */


/******************************************************************************/
/* I2C Register Bit Positions                                                 */
/******************************************************************************/

/** * @defgroup I2C_CR1_BIT_POS I2C Control Register 1 (I2C_CR1) Bit Positions
 * @{
 */
#define I2C_CR1_PE              (0U)
#define I2C_CR1_SMBUS           (1U)
#define I2C_CR1_SMBTYPE         (3U)
#define I2C_CR1_ENARP           (4U)
#define I2C_CR1_ENPEC           (5U)
#define I2C_CR1_ENGC            (6U)
#define I2C_CR1_NOSTRETCH       (7U)
#define I2C_CR1_START           (8U)
#define I2C_CR1_STOP            (9U)
#define I2C_CR1_ACK             (10U)
#define I2C_CR1_POS             (11U)
#define I2C_CR1_PEC             (12U)
#define I2C_CR1_ALERT           (13U)
#define I2C_CR1_SWRST           (15U)
/** @} */

/** * @defgroup I2C_CR2_BIT_POS I2C Control Register 2 (I2C_CR2) Bit Positions
 * @{
 */
#define I2C_CR2_FREQ            (0U)    /**< Starts at Bit 0 (Bits 5:0) */
#define I2C_CR2_ITERREN         (8U)
#define I2C_CR2_ITEVTEN         (9U)
#define I2C_CR2_ITBUFEN         (10U)
#define I2C_CR2_DMAEN           (11U)
#define I2C_CR2_LAST            (12U)
/** @} */

/** * @defgroup I2C_SR1_BIT_POS I2C Status Register 1 (I2C_SR1) Bit Positions
 * @{
 */
#define I2C_SR1_SB              (0U)
#define I2C_SR1_ADDR            (1U)
#define I2C_SR1_BTF             (2U)
#define I2C_SR1_ADD10           (3U)
#define I2C_SR1_STOPF           (4U)
#define I2C_SR1_RXNE            (6U)
#define I2C_SR1_TXE             (7U)
#define I2C_SR1_BERR            (8U)
#define I2C_SR1_ARLO            (9U)
#define I2C_SR1_AF              (10U)
#define I2C_SR1_OVR             (11U)
#define I2C_SR1_PECERR          (12U)
#define I2C_SR1_TIMEOUT         (14U)
#define I2C_SR1_SMBALERT        (15U)
/** @} */

/** * @defgroup I2C_SR2_BIT_POS I2C Status Register 2 (I2C_SR2) Bit Positions
 * @{
 */
#define I2C_SR2_MSL             (0U)
#define I2C_SR2_BUSY            (1U)
#define I2C_SR2_TRA             (2U)
#define I2C_SR2_GENCALL         (4U)
#define I2C_SR2_SMBDEFAULT      (5U)
#define I2C_SR2_SMBHOST         (6U)
#define I2C_SR2_DUALF           (7U)
#define I2C_SR2_PEC             (8U)    /**< Starts at Bit 8 (Bits 15:8) */
/** @} */

/** * @defgroup I2C_CCR_BIT_POS I2C Clock Control Register (I2C_CCR) Bit Positions
 * @{
 */
#define I2C_CCR_CCR             (1U)
#define I2C_CCR_DUTY            (14U)
#define I2C_CCR_FS              (15U)
/** @} */


/******************************************************************************/
/* USART Register Bit Positions                                               */
/******************************************************************************/

/** * @defgroup USART_SR_BIT_POS USART Status Register (USART_SR) Bit Positions
 * @{
 */
#define USART_SR_CTS            (9U)
#define USART_SR_LBD            (8U)
#define USART_SR_TXE            (7U)
#define USART_SR_TC             (6U)
#define USART_SR_RXNE           (5U)
#define USART_SR_IDLE           (4U)
#define USART_SR_ORE            (3U)
#define USART_SR_NF             (2U)
#define USART_SR_FE             (1U)
#define USART_SR_PE             (0U)
/** @} */

/** * @defgroup USART_BRR_BIT_POS USART Baud Rate Register (USART_BRR) Bit Positions
 * @{
 */
#define USART_BRR_DIV_MANTISSA  (4U)    /**< Bits 15:4: Mantissa of USARTDIV */
#define USART_BRR_DIV_FRACTION  (0U)    /**< Bits 3:0: Fraction of USARTDIV */
/** @} */

/** * @defgroup USART_CR1_BIT_POS USART Control Register 1 (USART_CR1) Bit Positions
 * @{
 */
#define USART_CR1_OVER8         (15U)
#define USART_CR1_UE            (13U)
#define USART_CR1_M             (12U)
#define USART_CR1_WAKE          (11U)
#define USART_CR1_PCE           (10U)
#define USART_CR1_PS            (9U)
#define USART_CR1_PEIE          (8U)
#define USART_CR1_TXEIE         (7U)
#define USART_CR1_TCIE          (6U)
#define USART_CR1_RXNEIE        (5U)
#define USART_CR1_IDLEIE        (4U)
#define USART_CR1_TE            (3U)
#define USART_CR1_RE            (2U)
#define USART_CR1_RWU           (1U)
#define USART_CR1_SBK           (0U)
/** @} */

/** * @defgroup USART_CR2_BIT_POS USART Control Register 2 (USART_CR2) Bit Positions
 * @{
 */
#define USART_CR2_LINEN         (14U)
#define USART_CR2_STOP          (12U)   /**< Bits 13:12: STOP bits */
#define USART_CR2_CLKEN         (11U)
#define USART_CR2_CPOL          (10U)
#define USART_CR2_CPHA          (9U)
#define USART_CR2_LBCL          (8U)
#define USART_CR2_LBDIE         (6U)
#define USART_CR2_LBDL          (5U)
#define USART_CR2_ADD           (0U)    /**< Bits 3:0: Address of the USART node */
/** @} */

/** * @defgroup USART_CR3_BIT_POS USART Control Register 3 (USART_CR3) Bit Positions
 * @{
 */
#define USART_CR3_ONEBIT        (11U)
#define USART_CR3_CTSIE         (10U)
#define USART_CR3_CTSE          (9U)
#define USART_CR3_RTSE          (8U)
#define USART_CR3_DMAT          (7U)
#define USART_CR3_DMAR          (6U)
#define USART_CR3_SCEN          (5U)
#define USART_CR3_NACK          (4U)
#define USART_CR3_HDSEL         (3U)
#define USART_CR3_IRLP          (2U)
#define USART_CR3_IREN          (1U)
#define USART_CR3_EIE           (0U)
/** @} */

/** * @defgroup USART_GTPR_BIT_POS USART Guard Time and Prescaler Register (USART_GTPR) Bit Positions
 * @{
 */
#define USART_GTPR_GT           (8U)    /**< Bits 15:8: Guard time value */
#define USART_GTPR_PSC          (0U)    /**< Bits 7:0: Prescaler value */
/** @} */

/******************************************************************************/
/* Generic Macros                                                             */
/******************************************************************************/

/** * @defgroup GENERIC_MACROS General state definitions
 * @{
 */
#define ENABLE                  (1U)
#define DISABLE                 (0U)
#define SET                     ENABLE
#define RESET                   DISABLE
#define GPIO_PIN_SET            SET
#define GPIO_PIN_RESET          RESET
#define FLAG_SET                SET
#define FLAG_RESET              RESET
/** @} */

#endif /* STM32F401RE_H */
