#ifndef INC_STM32F401RE_RCC_DRIVER_H_
#define INC_STM32F401RE_RCC_DRIVER_H_

#include "stm32f401re.h"

/******************************************************************************/
/* RCC Driver APIs (Function Prototypes)                                      */
/******************************************************************************/

/**
 * @brief  Calculates and returns the APB1 peripheral clock (PCLK1) frequency.
 * @return PCLK1 value in Hz.
 */
uint32_t RCC_GetPCLK1Value(void);

/**
 * @brief  Calculates and returns the APB2 peripheral clock (PCLK2) frequency.
 * @return PCLK2 value in Hz.
 */
uint32_t RCC_GetPCLK2Value(void);

#endif /* INC_STM32F401RE_RCC_DRIVER_H_ */
