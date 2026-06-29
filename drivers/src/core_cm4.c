/**
 * @file    core_cm4.c
 * @brief   ARM Cortex-M4 Core specific driver implementation.
 */

#include "stm32f401re.h"

/******************************************************************************/
/* NVIC Control API Implementations                                           */
/******************************************************************************/

/**
 * @brief  Configures the interrupt delivery for a given IRQ number in the NVIC.
 * @param  IRQNumber: IRQ number to configure (0 to 95).
 * @param  EnorDi: ENABLE to set (ISER), DISABLE to clear (ICER).
 * @return None
 */
void NVIC_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi) {
    if (EnorDi == ENABLE) {
        if (IRQNumber <= 31U) {
            *NVIC_ISER0 = (1U << IRQNumber);
        } 
        else if (IRQNumber > 31U && IRQNumber <= 63U) {
            *NVIC_ISER1 = (1U << (IRQNumber % 32U));
        } 
        else if (IRQNumber > 63U && IRQNumber <= 95U) {
            *NVIC_ISER2 = (1U << (IRQNumber % 32U));
        }
    } else {
        if (IRQNumber <= 31U) {
            *NVIC_ICER0 = (1U << IRQNumber);
        } 
        else if (IRQNumber > 31U && IRQNumber <= 63U) {
            *NVIC_ICER1 = (1U << (IRQNumber % 32U));
        } 
        else if (IRQNumber > 63U && IRQNumber <= 95U) {
            *NVIC_ICER2 = (1U << (IRQNumber % 32U));
        }
    }
}

/**
 * @brief  Configures the priority of a given IRQ number in the NVIC.
 * @param  IRQNumber: IRQ number to configure.
 * @param  IRQPriority: Priority value (depends on implemented priority bits).
 * @return None
 */
void NVIC_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority) {
    uint8_t iprx = IRQNumber / 4U;
    uint8_t iprx_section = IRQNumber % 4U;

    /* Calculate the shift amount based on the priority bits implemented by the vendor */
    uint8_t shift_amount = (8U * iprx_section) + (8U - NO_PR_BITS_IMPLEMENTED);

    /* Clear the old priority before setting the new one */
    *(NVIC_PR_BASE_ADDR + iprx) &= ~(0xFFU << shift_amount);      
    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount); 
}