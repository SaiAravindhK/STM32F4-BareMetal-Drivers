#ifndef INC_CORE_CM4_H_
#define INC_CORE_CM4_H_

#include <stdint.h>

/******************************************************************************/
/* ARM Cortex-M4 Processor Specific Details                                   */
/******************************************************************************/

/** * @defgroup NVIC_ISER_REGISTERS NVIC Interrupt Set-Enable Registers
 * @{
 */
#define NVIC_ISER0              ((volatile uint32_t*)0xE000E100U)
#define NVIC_ISER1              ((volatile uint32_t*)0xE000E104U)
#define NVIC_ISER2              ((volatile uint32_t*)0xE000E108U)
#define NVIC_ISER3              ((volatile uint32_t*)0xE000E10CU)
/** @} */

/** * @defgroup NVIC_ICER_REGISTERS NVIC Interrupt Clear-Enable Registers
 * @{
 */
#define NVIC_ICER0              ((volatile uint32_t*)0xE000E180U)
#define NVIC_ICER1              ((volatile uint32_t*)0xE000E184U)
#define NVIC_ICER2              ((volatile uint32_t*)0xE000E188U)
#define NVIC_ICER3              ((volatile uint32_t*)0xE000E18CU)
/** @} */

/**
 * @brief NVIC Priority Register Base Address
 */
#define NVIC_PR_BASE_ADDR       ((volatile uint32_t*)0xE000E400U)

/**
 * @brief Number of priority bits implemented in this specific MCU
 */
#define NO_PR_BITS_IMPLEMENTED  (4U)


/******************************************************************************/
/* NVIC Control APIs (Function Prototypes)                                    */
/******************************************************************************/

/**
 * @brief  Configures the interrupt delivery for a given IRQ number in the NVIC.
 * @param  IRQNumber: IRQ number to configure (0 to 95).
 * @param  EnorDi: ENABLE to set (ISER), DISABLE to clear (ICER).
 * @return None
 */
void NVIC_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);

/**
 * @brief  Configures the priority of a given IRQ number in the NVIC.
 * @param  IRQNumber: IRQ number to configure.
 * @param  IRQPriority: Priority value (depends on implemented priority bits).
 * @return None
 */
void NVIC_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

#endif /* INC_CORE_CM4_H_ */
